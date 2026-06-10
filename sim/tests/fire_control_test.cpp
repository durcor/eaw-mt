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
    // in.gates default to all-pass.
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
    // stage-G muzzle select resolves to {0,0,0} (mat1==mat2==0, no second bone ⇒ no draw).
    in.muzzle_mat1_t = vec3{0, 0, 0};
    in.muzzle_mat2_t = vec3{0, 0, 0};
    in.full_random_dir = false;
    in.has_muzzle_bone2 = false;
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
    // any single blocked gate ⇒ NoFire_Ineligible, reporting THAT gate.
    { FireControlInputs in = firing_baseline(); in.gates.not_fogged = false;
      SimRng r(1); FireControlDecision d = fire_control_decide(in, r);
      CHECK(d.outcome == FireOutcome::NoFire_Ineligible);
      CHECK(d.blocked_gate == FireGate::NotFogged);
      CHECK(!d.cooldown_rolled); }                 // blocked before any draw

    // gate ORDER: when several fail, the FIRST in 3825b0 order is reported.
    { FireControlInputs in = firing_baseline();
      in.gates.weapon_selected = false;            // last gate
      in.gates.target_targetable = false;          // earlier gate
      in.gates.not_self_target = false;            // middle gate
      SimRng r(1);
      CHECK(fire_control_decide(in, r).blocked_gate == FireGate::TargetTargetable); }

    // the very first gate.
    { FireControlInputs in = firing_baseline(); in.gates.owner_present = false;
      SimRng r(1);
      CHECK(fire_control_decide(in, r).blocked_gate == FireGate::OwnerPresent); }

    // all gates pass ⇒ proceeds past eligibility (this baseline Fires).
    { FireControlInputs in = firing_baseline();
      SimRng r(1); FireControlDecision d = fire_control_decide(in, r);
      CHECK(d.blocked_gate == FireGate::None && d.outcome == FireOutcome::Fire); }
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

// expected RNG state after N steps from seed.
static eaw::u32 state_after(eaw::u32 seed, int n) { SimRng r(seed); for (int i=0;i<n;++i) r.step(); return r.state; }

static void test_muzzle_select_seam() {
    std::printf("test_muzzle_select_seam\n");
    const vec3 m1{0, 0, 0}, m2{10, 20, 30};
    // no second bone → mat1, NO draw.
    { SimRng r(5); CHECK((firing_select_muzzle_point(m1, m2, false, false, r) == m1)); CHECK(r.state == 5u); }
    // has bone2, not full-random → one percent(50) ROLL (rejection-sampled, ≥1 step); result mat1/mat2.
    { SimRng ref(5); ref.percent(50);
      SimRng r(5); vec3 p = firing_select_muzzle_point(m1, m2, false, true, r);
      CHECK(r.state == ref.state); CHECK((p == m1 || p == m2)); }
    // full random → THREE range_f draws (single-step each), each component within [mat1.k, mat2.k].
    { SimRng r(5); vec3 p = firing_select_muzzle_point(m1, m2, true, true, r);
      CHECK(r.state == state_after(5, 3));
      CHECK(p.x >= 0 && p.x <= 10 && p.y >= 0 && p.y <= 20 && p.z >= 0 && p.z <= 30); }
    // both branches of the 50/50 are reachable across seeds.
    { bool saw1=false, saw2=false;
      for (eaw::u32 s=1; s<=64 && !(saw1&&saw2); ++s) {
          SimRng r(s); vec3 p = firing_select_muzzle_point(m1, m2, false, true, r);
          if (p == m1) saw1 = true;
          if (p == m2) saw2 = true;
      }
      CHECK(saw1 && saw2); }
}

static void test_cooldown_seam() {
    std::printf("test_cooldown_seam\n");
    // draw in [(int)(2*10),(int)(5*10)] = [20,50]; base = ((int64)(draw*1.0) & 0xffffffff)/100.
    { SimRng r(9); int draw = SimRng(9).range_i(20, 50);
      int base = firing_roll_cooldown_base(2.0f, 5.0f, 10.0f, 1.0f, r);
      CHECK(base == (int)(((int64_t)((float)draw * 1.0f) & 0xffffffffLL) / 100)); }
    // consumes exactly one range_i ROLL (rejection-sampled, ≥1 step).
    { SimRng ref(9); ref.range_i(20, 50);
      SimRng r(9); firing_roll_cooldown_base(2.0f, 5.0f, 10.0f, 1.0f, r);
      CHECK(r.state == ref.state); }
    // reproducible.
    { SimRng a(3), b(3);
      CHECK(firing_roll_cooldown_base(1.0f, 4.0f, 100.0f, 2.5f, a) ==
            firing_roll_cooldown_base(1.0f, 4.0f, 100.0f, 2.5f, b)); }
}

static void test_fire_rolls_cooldown() {
    std::printf("test_fire_rolls_cooldown\n");
    FireControlInputs in = firing_baseline();
    in.burst_counter = 1;                        // decrements to 0 ⇒ roll
    in.cooldown_min = 2.0f; in.cooldown_max = 5.0f; in.cooldown_scale = 10.0f;  // gamespeed 1.0
    in.cooldown_mods.burst_count = 3;
    SimRng r(1);
    FireControlDecision d = fire_control_decide(in, r);
    CHECK(d.outcome == FireOutcome::Fire);
    CHECK(d.cooldown_rolled);
    CHECK(d.cooldown_base >= 0);
    // neutral mods: f == 1 (no mult), g == 1 (no div) ⇒ recharge == base; burst = (int)(3*1.0+0.5) == 3.
    CHECK(d.cooldown.recharge == d.cooldown_base);
    CHECK(d.cooldown.burst == 3);
    // entry counter 0 also rolls (no decrement, already 0).
    { FireControlInputs in0 = firing_baseline(); in0.burst_counter = 0;
      in0.cooldown_min = 2.0f; in0.cooldown_max = 5.0f; in0.cooldown_scale = 10.0f;
      SimRng r0(1);
      CHECK(fire_control_decide(in0, r0).cooldown_rolled); }
    // no roll when the burst counter has not reached 0 (baseline default 2 → between-shots branch).
    FireControlInputs in2 = firing_baseline();
    in2.cooldown_mods.shot_delay = 100.0f;       // recharge = (int)(1.0*100) → /g=1 +0.5 ⇒ 100
    SimRng r2(1);
    FireControlDecision d2 = fire_control_decide(in2, r2);
    CHECK(!d2.cooldown_rolled);
    CHECK(d2.cooldown.recharge == 100);
    CHECK(d2.cooldown.burst == 1);               // decremented 2 → 1, kept (g > 0)
}

static void test_cooldown_modifier_ladder() {
    std::printf("test_cooldown_modifier_ladder\n");
    // rate multiplier composition: cat0 REPLACES the 1.0 start, cat1 multiplies.
    { CooldownModInputs m; m.cat0_active = true; m.cat0_mult = 0.5f;
      m.owner_rof_mod = 2.0f; m.cat1_active = true; m.cat1_mult = 0.25f;
      CHECK(firing_rate_multiplier(m) == 0.5f * 2.0f * 0.25f); }
    { CooldownModInputs m; m.cat0_mult = 0.5f;   // inactive ⇒ ignored
      CHECK(firing_rate_multiplier(m) == 1.0f); }
    { CooldownModInputs m; m.owner_recharge_mod = 2.0f; m.buff_fold = 0.5f;
      CHECK(firing_recharge_divisor(m) == 2.0f * 1.5f); }

    // roll branch: f < 1 applies, f >= 1 does NOT (the comiss asymmetry).
    { CooldownModInputs m; m.cat0_active = true; m.cat0_mult = 0.5f; m.burst_count = 4;
      CooldownState s = firing_recharge_after_burst(100, m);
      CHECK(s.recharge == (int)(100.0f * 0.5f + 0.5f));         // 50
      CHECK(s.burst == 4); }                                    // g == 1 ⇒ burst_count*1+0.5
    { CooldownModInputs m; m.cat0_active = true; m.cat0_mult = 3.0f;  // f > 1 ⇒ recharge untouched
      CooldownState s = firing_recharge_after_burst(100, m);
      CHECK(s.recharge == 100); }
    // roll branch: divisor applies only inside (0,1); g > 1 leaves recharge but scales the burst reset.
    { CooldownModInputs m; m.owner_recharge_mod = 0.5f; m.burst_count = 4;
      CooldownState s = firing_recharge_after_burst(100, m);
      CHECK(s.recharge == (int)(100.0f / 0.5f + 0.5f));         // 200
      CHECK(s.burst == (int)(4.0f * 0.5f + 0.5f)); }            // 2
    { CooldownModInputs m; m.owner_recharge_mod = 2.0f; m.burst_count = 4;
      CooldownState s = firing_recharge_after_burst(100, m);
      CHECK(s.recharge == 100);                                 // g >= 1 ⇒ no div
      CHECK(s.burst == (int)(4.0f * 2.0f + 0.5f)); }            // 8 (burst still scales)
    // roll branch, g < 0: clamped to 0 for the burst reset (maxss), recharge untouched (not in (0,1)).
    { CooldownModInputs m; m.owner_recharge_mod = -1.0f; m.burst_count = 4;
      CooldownState s = firing_recharge_after_burst(100, m);
      CHECK(s.recharge == 100);
      CHECK(s.burst == 0); }

    // between-shots branch: f applies UNCONDITIONALLY (even f > 1), then /g when g > 0.
    { CooldownModInputs m; m.shot_delay = 100.0f; m.cat0_active = true; m.cat0_mult = 2.0f;
      CooldownState s = firing_delay_between_shots(1.0f, m, 5);
      CHECK(s.recharge == (int)((f32)(int)(100.0f * 2.0f + 0.5f) / 1.0f + 0.5f));  // 200→/1+0.5→200
      CHECK(s.burst == 5); }
    // between-shots, g <= 0: BOTH recharge and burst zeroed (the asm-corrected ebx tail).
    { CooldownModInputs m; m.shot_delay = 100.0f; m.owner_recharge_mod = 0.0f;
      CooldownState s = firing_delay_between_shots(1.0f, m, 5);
      CHECK(s.recharge == 0);
      CHECK(s.burst == 0); }
}

static void test_range_gate() {
    std::printf("test_range_gate\n");
    // 2D distance only (z ignored): muzzle {0,0,99}, aim {3,4,-99} → dist 5.
    CHECK(fire_range_gate_pass(vec3{0,0,99}, vec3{3,4,-99}, 5.0f, 0.0f, 0.0f));   // 5 <= 5
    CHECK(!fire_range_gate_pass(vec3{0,0,0}, vec3{3,4,0}, 4.0f, 0.0f, 0.0f));      // 5 > 4
    // extent extends max range (always added): 5 <= 4+2.
    CHECK(fire_range_gate_pass(vec3{0,0,0}, vec3{3,4,0}, 4.0f, 2.0f, 0.0f));
    // min-range floor: dist 5 < min 6 → out.
    CHECK(!fire_range_gate_pass(vec3{0,0,0}, vec3{3,4,0}, 100.0f, 0.0f, 6.0f));
    CHECK(fire_range_gate_pass(vec3{0,0,0}, vec3{3,4,0}, 100.0f, 0.0f, 5.0f));     // 5 >= 5 (boundary)
}

int main() {
    std::printf("=== fire_control_test ===\n");
    test_range_gate();
    test_muzzle_select_seam();
    test_cooldown_seam();
    test_cooldown_modifier_ladder();
    test_fire_rolls_cooldown();
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
