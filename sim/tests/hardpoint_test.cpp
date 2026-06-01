// Host-side behavioral test for the lifted hardpoint fire-control (Phase-3 unit 4).
// Validates the deterministic pieces recovered from FUN_1403a76b0 / FUN_140387f50 / FUN_140540070 /
// FUN_140387010: the fire-budget distribution (incl. the total_w cancellation), the consume guards,
// the avail/base clamps, eligibility, and the per-hardpoint update dispatcher's delta + motion gate.
// The ship scalars + deep target acquisition are supplied by a controllable HardpointEnv; the
// in-game oracle (a DTFIRE capture, folded already into the DIFFTRACE per-tick hash) is the
// end-to-end check.
#include "../hardpoint.h"
#include "../recording_command_sink.h"
#include <cstdio>
#include <cmath>
#include <vector>

using namespace eaw;

static int g_fail = 0;
#define CHECK(cond) do { if (!(cond)) { \
    std::printf("  FAIL %s:%d  %s\n", __FILE__, __LINE__, #cond); ++g_fail; } } while (0)
static bool approx(f32 a, f32 b, f32 eps = 1e-4f) { return std::fabs(a - b) <= eps; }

// Recording env: fixed ship scalars + tallies of the deferred callouts.
struct FakeEnv : HardpointEnv {
    f32 fraction = 0.0f;
    f32 capacity = 0.0f;
    int mode = 0;
    int fires = 0;
    int depletions = 0;
    std::vector<int> acquired;     // hardpoint ids that ran target acquisition
    std::vector<u32> acquire_delta;
    std::vector<int> effects;
    std::vector<int> reloads;
    bool inhibit = false;

    int game_speed_mode() override { return mode; }
    f32 fire_fraction(ShipFireControl&) override { return fraction; }
    f32 fire_capacity(ShipFireControl&) override { return capacity; }
    void fire_weapon(HardPoint&) override { ++fires; }
    void on_budget_depleted(HardPoint&, ShipFireControl&) override { ++depletions; }
    bool global_fire_inhibited(HardPoint&) override { return inhibit; }
    void acquire_targets(HardPoint& hp, u32 d) override { acquired.push_back(hp.id); acquire_delta.push_back(d); }
    void fire_effect(HardPoint& hp) override { effects.push_back(hp.id); }
    void reload(HardPoint& hp, u32) override { reloads.push_back(hp.id); }
};

static HardPoint mount(int id, f32 budget, f32 weight, bool active = true) {
    HardPoint hp;
    hp.id = id; hp.budget = budget; hp.weight = weight; hp.active = active;
    hp.has_owner_record = true; hp.has_context = true; hp.owner_match = true;
    return hp;
}

static void test_skip_disabled() {
    std::printf("test_skip_disabled\n");
    FakeEnv env; env.capacity = 1.0f;
    // No hardpoints -> no-op.
    ShipFireControl empty;
    hardpoint_fire_budget(empty, env, 10);
    CHECK(env.fires == 0);
    // hardpoints_off flag (flags & 0x40) -> whole path skipped, including service.
    ShipFireControl ship; ship.hardpoints.push_back(mount(1, 10.0f, 1.0f));
    ship.hardpoints[0].enable_flag = true; ship.hardpoints[0].motion_state = 5;
    ship.hardpoints_off = true;
    hardpoint_fire_budget(ship, env, 10);
    CHECK(env.fires == 0);
    CHECK(env.acquired.empty());           // pass-4 service also skipped
}

static void test_cancellation() {
    std::printf("test_cancellation\n");
    // Three active mounts; capacity - base = 0.25.  share_i should equal budget_i * 0.25 regardless
    // of weights (total_w cancels), and the budgets decrement by exactly that.
    FakeEnv env; env.capacity = 0.30f; env.fraction = 0.05f; // base = 0.05, cap-base = 0.25
    ShipFireControl ship;
    ship.hardpoints.push_back(mount(1, 8.0f, 1.0f));
    ship.hardpoints.push_back(mount(2, 4.0f, 3.0f));   // very different weight
    ship.hardpoints.push_back(mount(3, 2.0f, 0.5f));
    f32 b0 = 8.0f, b1 = 4.0f, b2 = 2.0f;
    hardpoint_fire_budget(ship, env, 1);
    CHECK(approx(ship.hardpoints[0].budget, b0 - b0 * 0.25f));
    CHECK(approx(ship.hardpoints[1].budget, b1 - b1 * 0.25f));
    CHECK(approx(ship.hardpoints[2].budget, b2 - b2 * 0.25f));
    CHECK(env.fires == 3);
    CHECK(env.depletions == 0);
}

static void test_scale_irrelevant() {
    std::printf("test_scale_irrelevant\n");
    // The headline: the game-speed weight scale cancels. Run the SAME distribution under mode 0
    // (unscaled) and mode 1 (scale_fast = 0.5) -> identical shares (to fp tolerance).
    auto run = [](int mode, f32 sfast) {
        FakeEnv env; env.capacity = 0.40f; env.fraction = 0.0f; env.mode = mode; env.scale_fast = sfast;
        ShipFireControl ship;
        ship.hardpoints.push_back(mount(1, 10.0f, 2.0f));
        ship.hardpoints.push_back(mount(2, 6.0f, 5.0f));
        hardpoint_fire_budget(ship, env, 1);
        return ship;
    };
    ShipFireControl a = run(0, 1.0f);
    ShipFireControl b = run(1, 0.5f);
    CHECK(approx(a.hardpoints[0].budget, b.hardpoints[0].budget));
    CHECK(approx(a.hardpoints[1].budget, b.hardpoints[1].budget));
    // ...and both equal the cancelled form budget*(cap-base) = budget*0.40.
    CHECK(approx(a.hardpoints[0].budget, 10.0f - 10.0f * 0.40f));
    CHECK(approx(a.hardpoints[1].budget, 6.0f - 6.0f * 0.40f));
}

static void test_avail_clamp() {
    std::printf("test_avail_clamp\n");
    // capacity - base <= 0 -> avail clamped to 0 -> no distribution, but pass-4 service still runs.
    FakeEnv env; env.capacity = 0.10f; env.fraction = 0.20f; // cap - base = -0.10
    ShipFireControl ship;
    HardPoint hp = mount(1, 10.0f, 1.0f);
    hp.enable_flag = true; hp.motion_state = 6;             // eligible for acquire_targets
    ship.hardpoints.push_back(hp);
    hardpoint_fire_budget(ship, env, 7);
    CHECK(approx(ship.hardpoints[0].budget, 10.0f));        // untouched
    CHECK(env.fires == 0);
    CHECK(env.acquired.size() == 1);                        // service still happened
}

static void test_base_clamp() {
    std::printf("test_base_clamp\n");
    // fraction + bias >= 1.0 clamps base to 1.0.  With capacity 0.9 -> cap-base = -0.1 -> no fire.
    FakeEnv env; env.capacity = 0.90f; env.fraction = 0.80f; env.fraction_bias = 0.50f; // 1.30 -> 1.0
    ShipFireControl ship; ship.hardpoints.push_back(mount(1, 5.0f, 1.0f));
    hardpoint_fire_budget(ship, env, 1);
    CHECK(approx(ship.hardpoints[0].budget, 5.0f));
    CHECK(env.fires == 0);
    // Same clamp but capacity 1.0 -> cap-base = 0 -> avail 0 (note avail<=0 -> 0) -> still no fire.
    env.capacity = 1.0f;
    hardpoint_fire_budget(ship, env, 1);
    CHECK(env.fires == 0);
}

static void test_eligibility() {
    std::printf("test_eligibility\n");
    // Mount A active with budget 0 -> counts toward active/total_w but gets no share (budget>0 gate).
    // Mount B inactive -> excluded from total_w AND distribution entirely.
    // Mount C active with budget -> fires.
    FakeEnv env; env.capacity = 0.50f; env.fraction = 0.0f; // cap-base = 0.5
    ShipFireControl ship;
    ship.hardpoints.push_back(mount(1, 0.0f, 4.0f, true));   // A: budget 0
    ship.hardpoints.push_back(mount(2, 9.0f, 4.0f, false));  // B: inactive
    ship.hardpoints.push_back(mount(3, 6.0f, 4.0f, true));   // C
    hardpoint_fire_budget(ship, env, 1);
    CHECK(approx(ship.hardpoints[0].budget, 0.0f));          // A: no share (budget 0)
    CHECK(approx(ship.hardpoints[1].budget, 9.0f));          // B: inactive, untouched
    CHECK(approx(ship.hardpoints[2].budget, 6.0f - 6.0f * 0.5f)); // C: shares
    CHECK(env.fires == 1);
}

static void test_owner_match_guard() {
    std::printf("test_owner_match_guard\n");
    // 387f50 refuses to consume a mount whose stored context != the ship, even with a positive share.
    FakeEnv env; env.capacity = 0.50f; env.fraction = 0.0f;
    ShipFireControl ship;
    HardPoint hp = mount(1, 8.0f, 1.0f);
    hp.owner_match = false;
    ship.hardpoints.push_back(hp);
    hardpoint_fire_budget(ship, env, 1);
    CHECK(approx(ship.hardpoints[0].budget, 8.0f));          // guard rejected the consume
    CHECK(env.fires == 0);
}

static void test_depletion_event() {
    std::printf("test_depletion_event\n");
    // A share that drives the budget to <= 0 emits the depletion (station-level-lost) command once.
    FakeEnv env; env.capacity = 1.0f; env.fraction = 0.0f;   // cap-base = 1.0 -> share == full budget
    ShipFireControl ship; ship.hardpoints.push_back(mount(1, 5.0f, 1.0f));
    hardpoint_fire_budget(ship, env, 1);
    CHECK(ship.hardpoints[0].budget <= 0.0f);
    CHECK(env.fires == 1);
    CHECK(env.depletions == 1);
}

static void test_service_dispatch() {
    std::printf("test_service_dispatch\n");
    // Pass-4 dispatcher: delta = tick - last_serviced (then updated); acquire only when
    // motion_state in [5,11) and enabled; reload only when reload_active; inhibit short-circuits.
    FakeEnv env; env.capacity = 0.0f; // cap-base < 0 -> no distribution, isolate the service pass
    ShipFireControl ship;
    HardPoint a = mount(1, 1.0f, 1.0f); a.enable_flag = true; a.motion_state = 5;  a.last_serviced_tick = 40; a.reload_active = true;
    HardPoint b = mount(2, 1.0f, 1.0f); b.enable_flag = true; b.motion_state = 4;  // motion gate closed
    HardPoint c = mount(3, 1.0f, 1.0f); c.enable_flag = true; c.motion_state = 10; // 10 in [5,11)
    HardPoint d = mount(4, 1.0f, 1.0f); d.enable_flag = true; d.motion_state = 11; // 11 out of range
    ship.hardpoints = {a, b, c, d};
    hardpoint_fire_budget(ship, env, 50);
    // a: acquired with delta 10, last_serviced -> 50, reload ran.
    CHECK(env.acquired.size() == 2);                       // a (5) and c (10)
    CHECK(env.acquired[0] == 1 && env.acquire_delta[0] == 10);
    CHECK(env.acquired[1] == 3);
    CHECK(ship.hardpoints[0].last_serviced_tick == 50);
    CHECK(env.reloads.size() == 1 && env.reloads[0] == 1);
    // b (motion 4) and d (motion 11) did not acquire.
    for (int id : env.acquired) CHECK(id != 2 && id != 4);

    // inhibit short-circuits before delta/gate (no acquire, last_serviced unchanged).
    FakeEnv env2; env2.capacity = 0.0f; env2.inhibit = true;
    ShipFireControl ship2; HardPoint e = mount(9, 1.0f, 1.0f); e.enable_flag = true; e.motion_state = 5; e.last_serviced_tick = 3;
    ship2.hardpoints.push_back(e);
    hardpoint_fire_budget(ship2, env2, 99);
    CHECK(env2.acquired.empty());
    CHECK(ship2.hardpoints[0].last_serviced_tick == 3);    // unchanged
}

// ─────────────────────────────────────────────────────────────────────────────────────────────────
// Opportunity-target scan (FUN_140387400 core) tests.

static void* tgt(int i) { return reinterpret_cast<void*>(static_cast<uintptr_t>(i) + 1); }

// Deterministic player-table oracle. Each index carries: present(slot non-null), blocked(fog),
// and an eval result (target + accept). The scan's RNG seeding is real (SimRng); the world queries
// are these fixed tables — so the test exercises the scan's control logic exactly.
struct ScanEnv : OppScanEnv {
    int n = 0, self = -1;
    bool vis = false;
    std::vector<bool>  present;
    std::vector<bool>  blocked;
    std::vector<void*> evtarget;
    std::vector<bool>  evaccept;
    int player_count() override { return n; }
    int self_skip_id() override { return self; }
    void* player_at(int idx) override { return present[idx] ? tgt(idx) : nullptr; }
    bool visibility_mode() override { return vis; }
    bool candidate_blocked(int idx) override { return blocked[idx]; }
    CandidateEval eval_candidate(int idx) override { return { evtarget[idx], evaccept[idx] }; }
    void resize(int count) {
        n = count;
        present.assign(count, true); blocked.assign(count, false);
        evtarget.assign(count, nullptr); evaccept.assign(count, false);
    }
};

// Independent prediction of the start index the scan will draw, for a given seed + player count.
static int predict_start(u32 seed, int n) { SimRng r(seed); return r.range_i(0, n - 1); }

static void test_scan_skip_self() {
    std::printf("test_scan_skip_self\n");
    // All non-self players accept on first look -> winner is the start index, or start+1 (wrapping,
    // skipping self) when the draw lands on self. Self is NEVER the chosen target.
    for (u32 seed = 1; seed <= 200; ++seed) {
        ScanEnv env; env.resize(4); env.self = 2;
        for (int i = 0; i < 4; ++i) { env.evtarget[i] = tgt(i); env.evaccept[i] = (i != 2); }
        env.evtarget[2] = nullptr; // self yields nothing even if examined (it never is)
        int start = predict_start(seed, 4);
        int expect = (start == env.self) ? (start + 1) % 4 : start;
        SimRng rng(seed);
        void* got = scan_opportunity_target(rng, env);
        CHECK(got == tgt(expect));
        CHECK(got != tgt(2));               // self never selected
    }
}

static void test_scan_circular_and_first_wins() {
    std::printf("test_scan_circular_and_first_wins\n");
    // Two accepters (indices 1 and 4) in a 6-player table, no self. The FIRST reached circularly from
    // the random start wins — which forces the wrap-around path when start > the nearest accepter.
    for (u32 seed = 1; seed <= 300; ++seed) {
        ScanEnv env; env.resize(6); env.self = 99;
        env.evtarget[1] = tgt(1); env.evaccept[1] = true;
        env.evtarget[4] = tgt(4); env.evaccept[4] = true;
        int start = predict_start(seed, 6);
        int expect = -1;                    // mirror the circular-first walk
        for (int c = 0, idx = start; c < 6; ++c) {
            if (idx == 1 || idx == 4) { expect = idx; break; }
            idx = (idx + 1) % 6;
        }
        SimRng rng(seed);
        void* got = scan_opportunity_target(rng, env);
        CHECK(got == tgt(expect));
    }
}

static void test_scan_fog_stall() {
    std::printf("test_scan_fog_stall\n");
    // With visibility mode ON, a fog-blocked candidate at the start index STALLS the scan (the index
    // never advances) -> no target, even though a valid accepter exists elsewhere. With vis OFF the
    // same layout reaches the accepter.
    u32 seed = 12345; int n = 4;
    int start = predict_start(seed, n);
    int other = (start + 2) % n;
    {
        ScanEnv env; env.resize(n); env.self = 99; env.vis = true;
        env.blocked[start] = true;
        env.evtarget[other] = tgt(other); env.evaccept[other] = true;
        SimRng rng(seed);
        CHECK(scan_opportunity_target(rng, env) == nullptr);   // stalled on the blocked start
    }
    {
        ScanEnv env; env.resize(n); env.self = 99; env.vis = false; // fog ignored
        env.blocked[start] = true;
        env.evtarget[other] = tgt(other); env.evaccept[other] = true;
        SimRng rng(seed);
        CHECK(scan_opportunity_target(rng, env) == tgt(other)); // reaches the accepter
    }
}

static void test_scan_null_slot_stall() {
    std::printf("test_scan_null_slot_stall\n");
    // A null player slot at the start index also stalls (player_at == nullptr, no advance).
    u32 seed = 777; int n = 5;
    int start = predict_start(seed, n);
    ScanEnv env; env.resize(n); env.self = 99;
    env.present[start] = false;                         // null slot at the draw
    int win = (start + 1) % n;
    env.evtarget[win] = tgt(win); env.evaccept[win] = true;
    SimRng rng(seed);
    CHECK(scan_opportunity_target(rng, env) == nullptr); // never advances off the null slot
}

static void test_scan_score_leak() {
    std::printf("test_scan_score_leak\n");
    // A candidate that is an enemy with a FOUND unit but a non-accept score still commits its unit as
    // the running result (the lVar13=lVar15 leak), so with no true accepter the leaked target is
    // returned. The LAST committed leak wins.
    u32 seed = 42; int n = 5;
    ScanEnv env; env.resize(n); env.self = 99;
    env.evtarget[1] = tgt(1); env.evaccept[1] = false;  // leak A
    env.evtarget[3] = tgt(3); env.evaccept[3] = false;  // leak B
    int start = predict_start(seed, n);
    // last leak committed in the circular walk:
    int last_leak = -1;
    for (int c = 0, idx = start; c < n; ++c) {
        if (idx == 1 || idx == 3) last_leak = idx;
        idx = (idx + 1) % n;
    }
    SimRng rng(seed);
    CHECK(scan_opportunity_target(rng, env) == tgt(last_leak));
    // No leak, no accept -> nullptr.
    ScanEnv env2; env2.resize(n); env2.self = 99;
    SimRng rng2(seed);
    CHECK(scan_opportunity_target(rng2, env2) == nullptr);
}

static void test_scan_rng_contract() {
    std::printf("test_scan_rng_contract\n");
    // n == 1 (only the owner): range_i(0,0) is a NO-DRAW per the LCG contract, and the lone slot is
    // self -> skipped -> nullptr, with the RNG state UNCHANGED.
    {
        ScanEnv env; env.resize(1); env.self = 0;
        SimRng rng(0xCAFEBABEu); u32 before = rng.state;
        CHECK(scan_opportunity_target(rng, env) == nullptr);
        CHECK(rng.state == before);                     // no draw
    }
    // n >= 2: exactly one draw (state advances) and same seed -> same result.
    {
        ScanEnv a; a.resize(3); a.self = 99; a.evtarget[2] = tgt(2); a.evaccept[2] = true;
        ScanEnv b = a;
        SimRng ra(0x1111u), rb(0x1111u);
        void* g1 = scan_opportunity_target(ra, a);
        void* g2 = scan_opportunity_target(rb, b);
        CHECK(g1 == g2);
        CHECK(ra.state == rb.state);
        SimRng pre(0x1111u); pre.range_i(0, 2);         // one draw reference
        CHECK(ra.state == pre.state);                   // scan drew exactly once
    }
}

// ─────────────────────────────────────────────────────────────────────────────────────────────────
// Opportunity-target acquisition tail (FUN_140387400 lines 220–316) — the sig-0x21 EMISSION decision,
// wired to a recording sim::CommandSink. The world queries are controllable; the scan underneath uses
// the real SimRng. These pin the four emission outcomes + the rescan-stamp side effect.

static void* const SELF_HP = tgt(1000);  // the emitting HardPointClass* (payload.hardpoint)
static void* const EMITTER = tgt(2000);  // resolved emitter context

// Controllable acquisition env. Owns the opp_target_slot; folds an inner ScanEnv for the scan.
struct AcqEnv : OppAcquireEnv {
    // scan side (OppScanEnv): a single accepter table by default.
    int   scan_n = 4, scan_self = 99;
    bool  scan_vis = false;
    std::vector<bool>  s_present, s_blocked;
    std::vector<void*> s_target;
    std::vector<bool>  s_accept;
    // acquisition side.
    void* slot = nullptr;
    bool  ex_blocked = false, ex_fire_ok = false;
    int   frame = 100, interval = 10, last_scan = 0;
    bool  ctx_blocked = false, ctx_order = false;
    bool  new_fire_ok = true;
    bool  emitter_ok = true;
    // tallies of the side effects.
    int   clears = 0, binds = 0, scans_predicted = 0;

    void init_scan() {
        s_present.assign(scan_n, true); s_blocked.assign(scan_n, false);
        s_target.assign(scan_n, nullptr); s_accept.assign(scan_n, false);
    }
    // OppScanEnv
    int player_count() override { return scan_n; }
    int self_skip_id() override { return scan_self; }
    void* player_at(int idx) override { return s_present[idx] ? tgt(idx) : nullptr; }
    bool visibility_mode() override { return scan_vis; }
    bool candidate_blocked(int idx) override { return s_blocked[idx]; }
    CandidateEval eval_candidate(int idx) override { return { s_target[idx], s_accept[idx] }; }
    // OppAcquireEnv
    void* opp_slot() override { return slot; }
    bool existing_fog_blocked() override { return ex_blocked; }
    bool existing_fire_allowed() override { return ex_fire_ok; }
    void clear_opp_slot() override { slot = nullptr; ++clears; }
    void bind_opp_slot(void* t) override { slot = t; ++binds; }
    int frame_counter() override { return frame; }
    int rescan_interval() override { return interval; }
    int last_scan_time() override { return last_scan; }
    void set_last_scan_time(int v) override { last_scan = v; }
    bool context_blocked() override { return ctx_blocked; }
    bool context_has_active_order() override { return ctx_order; }
    bool new_slot_fire_allowed() override { return new_fire_ok; }
    bool resolve_emitter(void*& out) override { out = EMITTER; return emitter_ok; }
    void* self_hardpoint() override { return SELF_HP; }
};

static void test_acq_keep_existing() {
    std::printf("test_acq_keep_existing\n");
    // Existing slot is not fog-blocked and still fire-allowed -> KEEP: no scan, no bind, no clear,
    // no emit, RNG untouched.
    AcqEnv env; env.init_scan();
    env.slot = tgt(7); env.ex_blocked = false; env.ex_fire_ok = true;
    sim::RecordingCommandSink sink;
    SimRng rng(0xABCDu); u32 before = rng.state;
    acquire_opportunity_target(rng, env, sink);
    CHECK(sink.commands.empty());
    CHECK(env.slot == tgt(7));          // unchanged
    CHECK(env.clears == 0 && env.binds == 0);
    CHECK(rng.state == before);         // no scan -> no draw
}

static void test_acq_emit_after_rescan() {
    std::printf("test_acq_emit_after_rescan\n");
    // Existing slot fails revalidation -> cleared -> forced rescan -> scan binds an accepter that is
    // fire-allowed -> EMIT 0x21 with payload {target = bound slot, hardpoint = self}.
    AcqEnv env; env.init_scan();
    env.scan_self = 99;                 // no self in table
    for (int i = 0; i < env.scan_n; ++i) { env.s_target[i] = tgt(i); env.s_accept[i] = true; }
    env.slot = tgt(7); env.ex_blocked = false; env.ex_fire_ok = false; // revalidation fails -> clear+force
    env.new_fire_ok = true; env.emitter_ok = true;
    sim::RecordingCommandSink sink;
    u32 seed = 0x1234u;
    int start = predict_start(seed, env.scan_n);     // first accepter == start (all accept)
    SimRng rng(seed);
    acquire_opportunity_target(rng, env, sink);
    CHECK(env.clears == 1);             // the failed existing slot
    CHECK(env.binds == 1);
    CHECK(env.slot == tgt(start));      // bound the scan winner
    CHECK(sink.commands.size() == 1);
    if (!sink.commands.empty()) {
        const sim::RecordedCommand& c = sink.commands[0];
        CHECK(c.sig_id == sim::kSigOpportunityTargetAcquired);
        CHECK(c.emitter == EMITTER);
        CHECK(c.has_payload);
        CHECK(c.opp.target_slot == tgt(start));
        CHECK(c.opp.hardpoint == SELF_HP);
        CHECK(c.opp.vftable == nullptr);   // drain-filled
    }
    CHECK(env.last_scan == env.frame);  // stamped
}

static void test_acq_bound_not_fire_allowed() {
    std::printf("test_acq_bound_not_fire_allowed\n");
    // Scan binds a target but it is no longer fire-allowed at the emission gate -> CLEAR, no emit.
    AcqEnv env; env.init_scan();
    for (int i = 0; i < env.scan_n; ++i) { env.s_target[i] = tgt(i); env.s_accept[i] = true; }
    env.slot = nullptr; env.last_scan = 0; env.frame = 100; env.interval = 10; // interval elapsed -> rescan
    env.new_fire_ok = false;            // bound target rejected at the gate
    sim::RecordingCommandSink sink;
    SimRng rng(0x55u);
    acquire_opportunity_target(rng, env, sink);
    CHECK(env.binds == 1);              // scan bound something
    CHECK(env.slot == nullptr);         // ...then cleared at the emission gate
    CHECK(env.clears == 1);
    CHECK(sink.commands.empty());       // no emit
}

static void test_acq_emitter_bail() {
    std::printf("test_acq_emitter_bail\n");
    // Bound + fire-allowed, but emitter resolution bails (FUN_1404ec820 != context) -> return, no emit
    // and no clear.
    AcqEnv env; env.init_scan();
    for (int i = 0; i < env.scan_n; ++i) { env.s_target[i] = tgt(i); env.s_accept[i] = true; }
    env.slot = nullptr; env.last_scan = 0; env.frame = 100; env.interval = 10;
    env.new_fire_ok = true; env.emitter_ok = false;   // bail
    sim::RecordingCommandSink sink;
    SimRng rng(0x66u);
    acquire_opportunity_target(rng, env, sink);
    CHECK(sink.commands.empty());
    CHECK(env.slot != nullptr);         // slot kept (no clear on the bail path)
}

static void test_acq_no_rescan_interval() {
    std::printf("test_acq_no_rescan_interval\n");
    // No existing slot and the interval has NOT elapsed -> rescan gate stays shut: no scan, no draw,
    // no emit, and last_scan_time is NOT stamped.
    AcqEnv env; env.init_scan();
    for (int i = 0; i < env.scan_n; ++i) { env.s_target[i] = tgt(i); env.s_accept[i] = true; }
    env.slot = nullptr; env.last_scan = 95; env.frame = 100; env.interval = 10; // 10 < 5 false
    sim::RecordingCommandSink sink;
    SimRng rng(0x77u); u32 before = rng.state;
    acquire_opportunity_target(rng, env, sink);
    CHECK(sink.commands.empty());
    CHECK(env.binds == 0);
    CHECK(rng.state == before);         // gate shut -> no draw
    CHECK(env.last_scan == 95);         // NOT stamped (COND_A false)
}

static void test_acq_stamp_even_when_blocked() {
    std::printf("test_acq_stamp_even_when_blocked\n");
    // COND_A holds (interval elapsed) but the context is blocked -> the scan is suppressed, yet the
    // last_scan_time stamp STILL fires (the comma side effect inside the &&). No emit, no draw.
    AcqEnv env; env.init_scan();
    for (int i = 0; i < env.scan_n; ++i) { env.s_target[i] = tgt(i); env.s_accept[i] = true; }
    env.slot = nullptr; env.last_scan = 0; env.frame = 100; env.interval = 10; // elapsed -> COND_A true
    env.ctx_blocked = true;             // COND_B suppresses the scan
    sim::RecordingCommandSink sink;
    SimRng rng(0x88u); u32 before = rng.state;
    acquire_opportunity_target(rng, env, sink);
    CHECK(env.binds == 0);
    CHECK(sink.commands.empty());
    CHECK(rng.state == before);         // scan suppressed -> no draw
    CHECK(env.last_scan == 100);        // ...but the stamp still fired
}

static void test_acq_scan_empty_no_emit() {
    std::printf("test_acq_scan_empty_no_emit\n");
    // Rescan runs but the scan finds nothing (no accepters, no leaks) -> nothing bound, slot stays
    // null -> no emit. The RNG draw still happens (the scan always draws its start index).
    AcqEnv env; env.init_scan();        // all targets null, no accept
    env.slot = nullptr; env.last_scan = 0; env.frame = 100; env.interval = 10;
    sim::RecordingCommandSink sink;
    SimRng rng(0x99u); u32 before = rng.state;
    acquire_opportunity_target(rng, env, sink);
    CHECK(env.binds == 0);
    CHECK(env.slot == nullptr);
    CHECK(sink.commands.empty());
    CHECK(rng.state != before);         // the scan drew its start index
    CHECK(env.last_scan == 100);        // gate opened -> stamped
}

int main() {
    test_skip_disabled();
    test_cancellation();
    test_scale_irrelevant();
    test_avail_clamp();
    test_base_clamp();
    test_eligibility();
    test_owner_match_guard();
    test_depletion_event();
    test_service_dispatch();
    test_scan_skip_self();
    test_scan_circular_and_first_wins();
    test_scan_fog_stall();
    test_scan_null_slot_stall();
    test_scan_score_leak();
    test_scan_rng_contract();
    test_acq_keep_existing();
    test_acq_emit_after_rescan();
    test_acq_bound_not_fire_allowed();
    test_acq_emitter_bail();
    test_acq_no_rescan_interval();
    test_acq_stamp_even_when_blocked();
    test_acq_scan_empty_no_emit();
    if (g_fail) { std::printf("\nFAILED: %d check(s)\n", g_fail); return 1; }
    std::printf("\nAll hardpoint checks passed.\n");
    return 0;
}
