// Host-side behavioral test for the fire-control orchestration (FUN_1403825b0 assembly →
// fire_control_decide). Drives the composed pipeline end-to-end: each no-fire gate in stage order, the
// explicit-context aim path, and a full Fire that produces a SpawnCommand with the launch dir propagated.
#include "../fire_control.h"
#include <cstdio>
#include <cmath>

using namespace sim;
using eaw::vec3;
using eaw::f32;
using eaw::SimRng;

static int g_fail = 0;
#define CHECK(cond) do { if (!(cond)) { \
    std::printf("  FAIL %s:%d  %s\n", __FILE__, __LINE__, #cond); ++g_fail; } } while (0)

// A baseline that FIRES: explicit-context aim {50,0,0}, muzzle at origin (2D dist 50, range 100),
// stationary target → non-zero lead, reachable, spread off. The caller mutates one field per test.
static FireControlInputs firing_baseline() {
    FireControlInputs in;
    in.eligible = true;
    in.has_explicit_context = true;
    in.context_aim = vec3{50, 0, 0};

    in.muzzle_valid = true;
    in.muzzle = vec3{0, 0, 0};
    in.weapon_range = 100.0f;
    in.target_extent = 0.0f;
    in.min_range = 0.0f;

    in.target_pos = vec3{0, 0, 0};
    in.target_vel = vec3{0, 0, 0};
    in.frame_vel  = vec3{0, 0, 0};
    in.shooter_ref = vec3{0, 0, 0};
    in.gamespeed = 1.0f;
    in.proj_speed = 10.0f;
    in.aim_reachable = true;

    in.no_spread = true;                       // launch_dir == lead, no draws

    static int dummy_owner = 0;
    in.spawn.requester = &dummy_owner;
    in.spawn.template_id = 42;
    in.spawn.firer_id = 7;
    in.spawn.target = &dummy_owner;
    return in;
}

static void test_ineligible() {
    std::printf("test_ineligible\n");
    FireControlInputs in = firing_baseline();
    in.eligible = false;
    SimRng r(1);
    CHECK(fire_control_decide(in, r).outcome == FireOutcome::NoFire_Ineligible);
}

static void test_no_aim() {
    std::printf("test_no_aim\n");
    FireControlInputs in = firing_baseline();
    in.has_explicit_context = false;          // take the aim ladder
    in.aim_inputs.have_target_model = true;   // ... with no candidates and an unreachable center → unlocked
    in.aim_inputs.target_center = vec3{500, 0, 0};
    in.aim_inputs.center_reachable = false;
    SimRng r(1);
    CHECK(fire_control_decide(in, r).outcome == FireOutcome::NoFire_NoAim);
}

static void test_no_muzzle_and_out_of_range() {
    std::printf("test_no_muzzle_and_out_of_range\n");
    { FireControlInputs in = firing_baseline(); in.muzzle_valid = false;
      SimRng r(1); CHECK(fire_control_decide(in, r).outcome == FireOutcome::NoFire_OutOfRange); }
    // aim beyond weapon_range
    { FireControlInputs in = firing_baseline(); in.context_aim = vec3{500, 0, 0};
      SimRng r(1); CHECK(fire_control_decide(in, r).outcome == FireOutcome::NoFire_OutOfRange); }
    // inside min_range
    { FireControlInputs in = firing_baseline(); in.min_range = 80.0f;   // dist 50 < 80
      SimRng r(1); CHECK(fire_control_decide(in, r).outcome == FireOutcome::NoFire_OutOfRange); }
    // extent extends the range: aim at 120 with extent 30 → 120 <= 100+30 → NOT out of range (so it
    // proceeds past F; with a valid lead+reach it should Fire).
    { FireControlInputs in = firing_baseline(); in.context_aim = vec3{120, 0, 0}; in.target_extent = 30.0f;
      SimRng r(1); CHECK(fire_control_decide(in, r).outcome == FireOutcome::Fire); }
}

static void test_no_lead() {
    std::printf("test_no_lead\n");
    // target crossing far faster than the projectile → quadratic has no real positive root → zero lead.
    FireControlInputs in = firing_baseline();
    in.target_vel = vec3{0, 100000, 0};
    in.proj_speed = 1.0f;
    SimRng r(1);
    CHECK(fire_control_decide(in, r).outcome == FireOutcome::NoFire_NoLead);
}

static void test_unreachable() {
    std::printf("test_unreachable\n");
    FireControlInputs in = firing_baseline();
    in.aim_reachable = false;
    SimRng r(1);
    CHECK(fire_control_decide(in, r).outcome == FireOutcome::NoFire_Unreachable);
}

static void test_fire_emits_spawn() {
    std::printf("test_fire_emits_spawn\n");
    FireControlInputs in = firing_baseline();
    SimRng r(1);
    FireControlDecision d = fire_control_decide(in, r);
    CHECK(d.outcome == FireOutcome::Fire);
    CHECK((d.aim_point == vec3{50, 0, 0}));
    // spread off ⇒ launch_dir == the lead vector (stationary target ⇒ lead ≈ {50,0,0}).
    CHECK(d.launch_dir.x != 0.0f);
    // the SpawnCommand carries the b3 payload with our identity + the propagated launch dir.
    CHECK(d.cmd.template_id == 42);
    CHECK(d.cmd.projectile.present);
    CHECK(d.cmd.projectile.firer_id == 7);
    CHECK(d.cmd.projectile.launch_dir[0] == d.launch_dir.x &&
          d.cmd.projectile.launch_dir[1] == d.launch_dir.y &&
          d.cmd.projectile.launch_dir[2] == d.launch_dir.z);
}

static void test_aim_ladder_reproducible() {
    std::printf("test_aim_ladder_reproducible\n");
    // the param_3==0 path draws the phase-2 substream start; identical seed ⇒ identical decision.
    FireControlInputs in = firing_baseline();
    in.has_explicit_context = false;
    in.aim_inputs.have_target_model = true;
    static AimSubsystem subs[3];
    for (int i = 0; i < 3; ++i) subs[i] = { vec3{(f32)(40 + i), 0, 0}, false, true };
    in.aim_inputs.subsystems = subs; in.aim_inputs.subsystem_count = 3;
    in.aim_inputs.muzzle_valid = true; in.aim_inputs.weapon_range = 1e6f;
    SimRng a(123), b(123);
    FireControlDecision da = fire_control_decide(in, a);
    FireControlDecision db = fire_control_decide(in, b);
    CHECK(da.outcome == db.outcome && (da.aim_point == db.aim_point));
}

int main() {
    std::printf("=== fire_control_test ===\n");
    test_ineligible();
    test_no_aim();
    test_no_muzzle_and_out_of_range();
    test_no_lead();
    test_unreachable();
    test_fire_emits_spawn();
    test_aim_ladder_reproducible();
    if (g_fail == 0) std::printf("ALL PASS\n");
    else std::printf("%d FAIL\n", g_fail);
    return g_fail ? 1 : 0;
}
