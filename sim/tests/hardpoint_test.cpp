// Host-side behavioral test for the lifted hardpoint fire-control (Phase-3 unit 4).
// Validates the deterministic pieces recovered from FUN_1403a76b0 / FUN_140387f50 / FUN_140540070 /
// FUN_140387010: the fire-budget distribution (incl. the total_w cancellation), the consume guards,
// the avail/base clamps, eligibility, and the per-hardpoint update dispatcher's delta + motion gate.
// The ship scalars + deep target acquisition are supplied by a controllable HardpointEnv; the
// in-game oracle (a DTFIRE capture, folded already into the DIFFTRACE per-tick hash) is the
// end-to-end check.
#include "../hardpoint.h"
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
    if (g_fail) { std::printf("\nFAILED: %d check(s)\n", g_fail); return 1; }
    std::printf("\nAll hardpoint checks passed.\n");
    return 0;
}
