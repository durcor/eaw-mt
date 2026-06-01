// Host-side behavioral test for the lifted signal/slot DISPATCH FAN-OUT (Phase-3 unit 6, Phase-B).
// Validates the determinism-critical pieces recovered from FUN_140240940 (+ 220ed0/2406c0/2408c0):
// insertion-order firing, the lockstep-cursor dedup, the touched-flag restart (each slot fires once,
// appended slots fire), the restart-flag left-set / propagation, dispatch-depth bookkeeping, the
// has-slot query tripping the restart guard (the 2408c0 quirk), and the serial command drain order.
#include "../signal_dispatch.h"
#include "../recording_command_sink.h"
#include <cstdio>
#include <functional>
#include <vector>

using namespace sim;

static int g_fail = 0;
#define CHECK(cond) do { if (!(cond)) { \
    std::printf("  FAIL %s:%d  %s\n", __FILE__, __LINE__, #cond); ++g_fail; } } while (0)

// A slot that logs its id when fired and optionally runs an action (nested emit / connect / query).
struct TestSlot : SignalSlot {
    int id = 0;
    std::vector<int>* log = nullptr;
    std::vector<int>* depth_log = nullptr; // optional: record dispatcher.depth() at fire time
    std::function<void(SignalDispatcher&)> action;
    void on_signal(SignalDispatcher& d, uint32_t, const void*) override {
        if (log) log->push_back(id);
        if (depth_log) depth_log->push_back(d.depth());
        if (action) action(d);
    }
};

static TestSlot make_slot(int id, std::vector<int>* log) {
    TestSlot s; s.id = id; s.log = log; return s;
}

static void test_insertion_order() {
    std::printf("test_insertion_order\n");
    SignalDispatcher d;
    std::vector<int> log;
    TestSlot a = make_slot(1, &log), b = make_slot(2, &log), c = make_slot(3, &log);
    d.connect(5, &a); d.connect(5, &b); d.connect(5, &c);
    d.emit(5);
    CHECK(log == (std::vector<int>{1, 2, 3})); // head->tail == connect order
}

static void test_unconnected_signal() {
    std::printf("test_unconnected_signal\n");
    SignalDispatcher d;
    std::vector<int> log;
    TestSlot a = make_slot(1, &log);
    d.connect(5, &a);
    d.emit(99);                       // no list for sig 99 -> sentinel -> no dispatch
    CHECK(log.empty());
    d.emit(5);
    CHECK(log == (std::vector<int>{1}));
}

static void test_restart_on_connect_during() {
    std::printf("test_restart_on_connect_during\n");
    SignalDispatcher d;
    std::vector<int> log;
    TestSlot a = make_slot(1, &log), b = make_slot(2, &log), c = make_slot(3, &log);
    TestSlot e = make_slot(4, &log);  // appended mid-dispatch by b
    b.action = [&](SignalDispatcher& disp) { disp.connect(5, &e); }; // trips touched -> restart
    d.connect(5, &a); d.connect(5, &b); d.connect(5, &c);
    d.emit(5);
    // a, b fire; b connects e (restart); restart skips a,b via cursor, fires c, then the appended e.
    CHECK(log == (std::vector<int>{1, 2, 3, 4})); // each exactly once, in order, e last
}

static void test_no_double_fire_on_restart() {
    std::printf("test_no_double_fire_on_restart\n");
    SignalDispatcher d;
    std::vector<int> log;
    // Two slots; the FIRST trips a restart (via has_slot query). The cursor must skip it on the
    // re-walk so it does not fire twice.
    TestSlot a = make_slot(1, &log), b = make_slot(2, &log);
    bool once = false;
    a.action = [&](SignalDispatcher& disp) {
        if (!once) { once = true; disp.has_slot(5, &b); } // query trips touched -> one restart
    };
    d.connect(5, &a); d.connect(5, &b);
    d.emit(5);
    CHECK(log == (std::vector<int>{1, 2})); // a once (not twice), then b
}

static void test_has_slot_triggers_restart() {
    std::printf("test_has_slot_triggers_restart\n");
    // 2408c0: a pure has-slot QUERY of a present sig sets the +0x08 flag. So a slot querying the
    // dispatcher during dispatch forces a restart. Verify the flag is observably left set after an
    // emit that restarted.
    SignalDispatcher d;
    std::vector<int> log;
    TestSlot a = make_slot(1, &log), b = make_slot(2, &log);
    bool once = false;
    a.action = [&](SignalDispatcher& disp) {
        if (!once) { once = true; CHECK(disp.has_slot(5, &b)); } // b IS connected -> true, sets flag
    };
    d.connect(5, &a); d.connect(5, &b);
    d.emit(5);
    CHECK(log == (std::vector<int>{1, 2}));
    CHECK(d.flag_set()); // restart leaves the flag set (uVar9 = 1)
}

static void test_flag_restore_no_restart() {
    std::printf("test_flag_restore_no_restart\n");
    SignalDispatcher d;
    std::vector<int> log;
    TestSlot a = make_slot(1, &log);
    d.connect(5, &a);          // connect set the flag...
    d.emit(5);                 // ...emit clears it for the walk; no restart -> restored to saved
    // saved at emit entry was true (connect set it), so it is restored to true. A SECOND emit with
    // no intervening mutation saves `true`, clears, restores `true` again — but no restart occurred.
    CHECK(log == (std::vector<int>{1}));
    // After a quiescent emit the flag equals whatever it was at entry; the key invariant is that a
    // NON-restarting emit does not spuriously force the flag on via `restarted`.
    bool before = d.flag_set();
    d.emit(5);                 // no slot trips touched this pass
    CHECK(d.flag_set() == before); // unchanged by a restart-free dispatch
    CHECK(log == (std::vector<int>{1, 1}));
}

static void test_depth_nesting() {
    std::printf("test_depth_nesting\n");
    SignalDispatcher d;
    std::vector<int> log, depths;
    TestSlot inner = make_slot(9, &log); inner.depth_log = &depths;
    TestSlot outer = make_slot(1, &log); outer.depth_log = &depths;
    d.connect(6, &inner);
    outer.action = [&](SignalDispatcher& disp) { disp.emit(6); }; // nested dispatch
    d.connect(5, &outer);
    CHECK(d.depth() == 0);
    d.emit(5);
    CHECK(d.depth() == 0);                 // depth restored to 0 after the whole emit
    CHECK(depths == (std::vector<int>{1, 2})); // outer fired at depth 1, inner (nested) at depth 2
    CHECK(log == (std::vector<int>{1, 9}));
}

static void test_drain_order() {
    std::printf("test_drain_order\n");
    // Phase-B drain: a recorded buffer of commands across two emitters replays in buffer order,
    // each on its own dispatcher (emitter+0x38), routing the 0x21 payload.
    SignalDispatcher dispA, dispB;
    std::vector<int> log;
    // slots that tag which (emitter, sig) fired, in order
    TestSlot a20 = make_slot(120, &log), a21 = make_slot(121, &log), b21 = make_slot(221, &log);

    // Two distinct emitter tokens; resolver maps them to dispatchers (the +0x38 resolution).
    int tokenA = 0, tokenB = 0;
    void* emitterA = &tokenA;
    void* emitterB = &tokenB;
    dispA.connect(kSigFireOrderInProgress, &a20);
    dispA.connect(kSigOpportunityTargetAcquired, &a21);
    dispB.connect(kSigOpportunityTargetAcquired, &b21);

    struct Resolver : EmitterResolver {
        void *ea, *eb; SignalDispatcher *da, *db;
        SignalDispatcher* dispatcher_for(void* e) override {
            if (e == ea) return da;
            if (e == eb) return db;
            return nullptr;
        }
    } res; res.ea = emitterA; res.eb = emitterB; res.da = &dispA; res.db = &dispB;

    // Build a recorded buffer via a RecordingCommandSink, exactly as a Phase-A unit would emit.
    RecordingCommandSink sink;
    sink.emit_fire_order_in_progress(emitterA);                 // A: 0x20
    OpportunityTargetAcquiredData payload; payload.hardpoint = emitterB;
    sink.emit_opportunity_target_acquired(emitterB, payload);   // B: 0x21 (payload)
    sink.emit_opportunity_target_acquired(emitterA, payload);   // A: 0x21

    drain_commands(sink.commands, res);
    CHECK(log == (std::vector<int>{120, 221, 121})); // exact buffer order, correct dispatchers
    CHECK(sink.commands.size() == 3);
}

static void test_drain_null_emitter_skipped() {
    std::printf("test_drain_null_emitter_skipped\n");
    SignalDispatcher disp;
    std::vector<int> log;
    TestSlot s = make_slot(7, &log);
    int token = 0; void* known = &token;
    disp.connect(kSigFireOrderInProgress, &s);
    struct Resolver : EmitterResolver {
        void* k; SignalDispatcher* d;
        SignalDispatcher* dispatcher_for(void* e) override { return e == k ? d : nullptr; }
    } res; res.k = known; res.d = &disp;

    RecordingCommandSink sink;
    sink.emit_fire_order_in_progress(reinterpret_cast<void*>(0xdead)); // unknown -> resolver null
    sink.emit_fire_order_in_progress(known);                           // known -> fires
    drain_commands(sink.commands, res);
    CHECK(log == (std::vector<int>{7})); // the unresolvable command was skipped, not crashed
}

int main() {
    test_insertion_order();
    test_unconnected_signal();
    test_restart_on_connect_during();
    test_no_double_fire_on_restart();
    test_has_slot_triggers_restart();
    test_flag_restore_no_restart();
    test_depth_nesting();
    test_drain_order();
    test_drain_null_emitter_skipped();
    if (g_fail == 0) std::printf("All signal-dispatch checks passed.\n");
    else             std::printf("%d signal-dispatch CHECK(s) FAILED.\n", g_fail);
    return g_fail ? 1 : 0;
}
