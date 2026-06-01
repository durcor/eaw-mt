// Host-side behavioral test for AbilityCountdownBehaviorClass::vfunc_6 (RVA 0x42f910), the third
// in-slice IN sim behavior. Validates:
//   * COUNTDOWN: timer floors at 0 (timer = max(timer - delta, 0)),
//   * the 0x2c AbilityRecharged emit is EDGE-TRIGGERED: fires exactly on the tick a cooldown reaches
//     0 (old timer >0, new ==0), is GATED by ability_ready, and never re-fires / never fires for a
//     slot already at 0 at entry,
//   * CHARGEUP: timer ceils at the per-slot target (timer = min(timer + delta, target)) and the
//     charge_complete callback fires whenever the slot is at its target (incl. re-firing while pinned),
//   * the integer tick-delta = tick - last_tick (catches up missed ticks; delta can exceed 1),
//   * the recharge stamp (last_tick) advances to the current tick each service,
//   * the no-manager guard returns without writing or stamping.
#include "../ability_countdown.h"
#include "../command_sink.h"
#include <cstdio>
#include <vector>

using namespace sim;

static int g_fail = 0;
#define CHECK(cond) do { if (!(cond)) { \
    std::printf("  FAIL %s:%d  %s\n", __FILE__, __LINE__, #cond); ++g_fail; } } while (0)

// Recording CommandSink: captures every emit_signal (emitter, sig, slot).
struct RecSink : CommandSink {
    struct Rec { void* emitter; uint32_t sig; int slot; };
    std::vector<Rec> emits;
    void emit_signal(void* emitter, uint32_t sig, const void* payload, size_t) override {
        int slot = -1;
        if (sig == kSigAbilityRecharged && payload)
            slot = static_cast<const AbilitySignalData*>(payload)->slot;
        emits.push_back({emitter, sig, slot});
    }
};

// Recording env: ability_ready returns a configurable verdict; charge_complete is logged.
struct RecEnv : AbilityCountdownEnv {
    bool ready_verdict = true;
    std::vector<int> ready_queries;     // slots ability_ready was asked about
    std::vector<int> charge_done;       // slots charge_complete fired for
    bool ability_ready(void*, int slot) override { ready_queries.push_back(slot); return ready_verdict; }
    void charge_complete(void*, int slot) override { charge_done.push_back(slot); }
};

static int g_owner_tag = 0;
static void* owner() { return &g_owner_tag; }

static void test_countdown_decrements_and_floors() {
    std::printf("test_countdown_decrements_and_floors\n");
    AbilityCountdownState st;
    st.timer[3] = 10;          // active cooldown
    st.last_tick = 100;
    RecEnv env; RecSink sink;
    ability_countdown_tick(st, owner(), 104, env, sink);   // delta = 4
    CHECK(st.timer[3] == 6);                                // 10 - 4
    CHECK(sink.emits.empty());                              // not reached 0
    CHECK(st.last_tick == 104);                             // stamped
    // next tick, delta=4 again would go to 2; a big delta floors at 0
    ability_countdown_tick(st, owner(), 200, env, sink);   // delta = 96 → floor 0
    CHECK(st.timer[3] == 0);
    CHECK(sink.emits.size() == 1);                          // hit 0 this tick → emit
}

static void test_recharge_emit_edge_and_gate() {
    std::printf("test_recharge_emit_edge_and_gate\n");
    // Slot reaches 0 exactly; gate passes → one 0x2c emit on owner, payload slot.
    AbilityCountdownState st; st.timer[7] = 3; st.last_tick = 0;
    RecEnv env; RecSink sink;
    ability_countdown_tick(st, owner(), 3, env, sink);     // delta 3 → 0
    CHECK(st.timer[7] == 0);
    CHECK(sink.emits.size() == 1);
    CHECK(sink.emits[0].sig == kSigAbilityRecharged);
    CHECK(sink.emits[0].emitter == owner());
    CHECK(sink.emits[0].slot == 7);
    CHECK(env.ready_queries.size() == 1 && env.ready_queries[0] == 7);

    // Next tick: slot already 0 at entry → edge guard skips it entirely (no re-emit, no gate query).
    ability_countdown_tick(st, owner(), 4, env, sink);
    CHECK(sink.emits.size() == 1);                          // still one
    CHECK(env.ready_queries.size() == 1);                  // not re-queried
}

static void test_recharge_gate_suppresses_emit() {
    std::printf("test_recharge_gate_suppresses_emit\n");
    AbilityCountdownState st; st.timer[1] = 2; st.last_tick = 0;
    RecEnv env; env.ready_verdict = false; RecSink sink;
    ability_countdown_tick(st, owner(), 5, env, sink);     // reaches 0
    CHECK(st.timer[1] == 0);                                // timer still floored
    CHECK(env.ready_queries.size() == 1);                  // gate WAS consulted
    CHECK(sink.emits.empty());                              // but verdict false → no emit
}

static void test_no_emit_when_already_zero() {
    std::printf("test_no_emit_when_already_zero\n");
    AbilityCountdownState st;                               // all timers 0
    st.last_tick = 0;
    RecEnv env; RecSink sink;
    ability_countdown_tick(st, owner(), 50, env, sink);    // big delta, every slot already 0
    CHECK(sink.emits.empty());
    CHECK(env.ready_queries.empty());
    CHECK(st.last_tick == 50);
}

static void test_chargeup_ceils_and_callback() {
    std::printf("test_chargeup_ceils_and_callback\n");
    AbilityCountdownState st;
    st.mode[2] = 1; st.timer[2] = 10; st.target[2] = 30;
    st.last_tick = 0;
    RecEnv env; RecSink sink;
    ability_countdown_tick(st, owner(), 7, env, sink);     // delta 7 → 17, below target
    CHECK(st.timer[2] == 17);
    CHECK(env.charge_done.empty());                        // not at target yet
    ability_countdown_tick(st, owner(), 100, env, sink);   // delta 93 → ceil at 30
    CHECK(st.timer[2] == 30);
    CHECK(env.charge_done.size() == 1 && env.charge_done[0] == 2);
}

static void test_chargeup_callback_refires_at_target() {
    std::printf("test_chargeup_callback_refires_at_target\n");
    AbilityCountdownState st;
    st.mode[4] = 1; st.timer[4] = 30; st.target[4] = 30;   // already pinned at target
    st.last_tick = 0;
    RecEnv env; RecSink sink;
    ability_countdown_tick(st, owner(), 1, env, sink);
    ability_countdown_tick(st, owner(), 2, env, sink);
    CHECK(env.charge_done.size() == 2);                    // fires every tick while at target
    CHECK(st.timer[4] == 30);                              // no overshoot
}

static void test_no_manager_guard() {
    std::printf("test_no_manager_guard\n");
    AbilityCountdownState st; st.has_manager = false;
    st.timer[0] = 5; st.last_tick = 10;
    RecEnv env; RecSink sink;
    ability_countdown_tick(st, owner(), 99, env, sink);
    CHECK(st.timer[0] == 5);                                // untouched
    CHECK(st.last_tick == 10);                              // NOT stamped (guarded no-op)
    CHECK(sink.emits.empty());
}

static void test_mixed_slots_one_pass() {
    std::printf("test_mixed_slots_one_pass\n");
    // A countdown slot that reaches 0 and a chargeup slot that tops out in the same pass.
    AbilityCountdownState st;
    st.timer[5] = 2;                                       // countdown → 0
    st.mode[9] = 1; st.timer[9] = 28; st.target[9] = 30;   // chargeup → 30
    st.last_tick = 0;
    RecEnv env; RecSink sink;
    ability_countdown_tick(st, owner(), 2, env, sink);     // delta 2
    CHECK(st.timer[5] == 0);
    CHECK(st.timer[9] == 30);
    CHECK(sink.emits.size() == 1 && sink.emits[0].slot == 5);
    CHECK(env.charge_done.size() == 1 && env.charge_done[0] == 9);
}

int main() {
    test_countdown_decrements_and_floors();
    test_recharge_emit_edge_and_gate();
    test_recharge_gate_suppresses_emit();
    test_no_emit_when_already_zero();
    test_chargeup_ceils_and_callback();
    test_chargeup_callback_refires_at_target();
    test_no_manager_guard();
    test_mixed_slots_one_pass();
    if (g_fail == 0) std::printf("All ability-countdown checks passed.\n");
    else             std::printf("%d ability-countdown CHECK(s) FAILED.\n", g_fail);
    return g_fail ? 1 : 0;
}
