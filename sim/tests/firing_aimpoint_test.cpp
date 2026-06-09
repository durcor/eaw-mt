// Host-side behavioral test for the aim-point resolver (FUN_140383f70 → firing_resolve_aimpoint).
// Asserts the 3-phase ladder, the phase-1-skip / phase-2-aim-origin asymmetry, the extent-gate
// difference (PH1/2 gated vs PH3 unconditional), the muzzle midpoint + range kernels, and the phase-2
// substream draw (count + reproducibility). Bit-exact-vs-binary is a later in-game oracle.
#include "../firing_aimpoint.h"
#include <cstdio>
#include <cmath>

using namespace sim;
using eaw::vec3;
using eaw::f32;
using eaw::SimRng;

static int g_fail = 0;
#define CHECK(cond) do { if (!(cond)) { \
    std::printf("  FAIL %s:%d  %s\n", __FILE__, __LINE__, #cond); ++g_fail; } } while (0)

static bool near(f32 a, f32 b, f32 eps = 1e-4f) { return std::fabs(a - b) <= eps; }
static eaw::u32 state_after(eaw::u32 seed, int n) { SimRng r(seed); for (int i=0;i<n;++i) r.step(); return r.state; }

// ── kernels ───────────────────────────────────────────────────────────────────────────────────────────
static void test_midpoint() {
    std::printf("test_midpoint\n");
    const vec3 b1{0, 0, 0}, b2{10, 20, 30};
    CHECK((firing_muzzle_midpoint(b1, false, b2) == b1));            // no second bone → bone1
    const vec3 m = firing_muzzle_midpoint(b1, true, b2);
    CHECK(near(m.x, 5) && near(m.y, 10) && near(m.z, 15));           // (b2+b1)*0.5
}

static void test_in_range() {
    std::printf("test_in_range\n");
    const vec3 muzzle{0, 0, 0};
    CHECK(firing_point_in_range(muzzle, vec3{3, 4, 0}, 5.0f));       // dist 5 <= 5 (boundary inclusive)
    CHECK(firing_point_in_range(muzzle, vec3{3, 4, 0}, 5.001f));
    CHECK(!firing_point_in_range(muzzle, vec3{3, 4, 0}, 4.999f));    // dist 5 > 4.999
}

// ── phase 0: no model ─────────────────────────────────────────────────────────────────────────────────
static void test_phase0_no_model() {
    std::printf("test_phase0_no_model\n");
    FiringAimInputs in;
    in.have_target_model = false;
    in.target_center = vec3{7, 8, 9};
    SimRng r(123);
    FiringAimResult res = firing_resolve_aimpoint(in, r);
    CHECK((res.aim == vec3{7, 8, 9}) && !res.locked);
    CHECK(r.state == 123u);   // no draw
}

// ── phase 1: designated aim-bones ─────────────────────────────────────────────────────────────────────
static void test_phase1_hit_and_skips() {
    std::printf("test_phase1_hit_and_skips\n");
    // muzzle at origin (bone1={0,0,0}, no bone2). range 100.
    AimCandidate bones[3];
    bones[0] = { vec3{500, 0, 0}, true,  true  };   // valid but out of range → skip
    bones[1] = { vec3{10,  0, 0}, false, true  };   // IN range but INVALID bone → skipped (phase-1 rule)
    bones[2] = { vec3{20,  0, 0}, true,  true  };   // valid, in range, reachable → WIN
    FiringAimInputs in;
    in.have_target_model = true;
    in.muzzle_valid = true;                         // muzzle = {0,0,0}
    in.weapon_range = 100.0f;
    in.aim_bones = bones; in.aim_bone_count = 3;
    in.target_center = vec3{-1, -1, -1};
    SimRng r(555);
    FiringAimResult res = firing_resolve_aimpoint(in, r);
    CHECK((res.aim == vec3{20, 0, 0}) && res.locked);
    CHECK(r.state == 555u);   // phase 1 returned → phase-2 draw never happened
}

static void test_phase1_unreachable_falls_through() {
    std::printf("test_phase1_unreachable_falls_through\n");
    // a valid, in-range bone that is NOT reachable must NOT win → falls to phase 3 (no subsystems).
    AimCandidate bones[1] = { { vec3{10, 0, 0}, true, false } };
    FiringAimInputs in;
    in.have_target_model = true;
    in.muzzle_valid = true;
    in.weapon_range = 100.0f;
    in.aim_bones = bones; in.aim_bone_count = 1;
    in.target_center = vec3{30, 0, 0};
    in.center_reachable = true;                      // phase-3 center in range+reachable → locked
    SimRng r(1);
    FiringAimResult res = firing_resolve_aimpoint(in, r);
    CHECK((res.aim == vec3{30, 0, 0}) && res.locked);
}

// ── phase 2: subsystems, random round-robin, destroyed-skip, origin-aim ──────────────────────────────
static void test_phase2_round_robin_and_draw() {
    std::printf("test_phase2_round_robin_and_draw\n");
    AimSubsystem subs[4];
    subs[0] = { vec3{10, 0, 0}, false, true };
    subs[1] = { vec3{11, 0, 0}, false, true };
    subs[2] = { vec3{12, 0, 0}, false, true };
    subs[3] = { vec3{13, 0, 0}, false, true };
    FiringAimInputs in;
    in.have_target_model = true;
    in.muzzle_valid = true;                          // muzzle {0,0,0}; all in range
    in.weapon_range = 100.0f;
    in.subsystems = subs; in.subsystem_count = 4;
    in.target_center = vec3{-9, -9, -9};
    SimRng r(0xABCDEF);
    FiringAimResult res = firing_resolve_aimpoint(in, r);
    CHECK(r.state == state_after(0xABCDEF, 1));       // exactly ONE draw
    // whichever start was drawn, the FIRST alive+inrange+reachable wins = the start index itself.
    bool ok = (res.aim == subs[0].pos || res.aim == subs[1].pos ||
               res.aim == subs[2].pos || res.aim == subs[3].pos);
    CHECK(ok && res.locked);
}

static void test_phase2_destroyed_skip_and_wrap() {
    std::printf("test_phase2_destroyed_skip_and_wrap\n");
    // Only index 1 is a live, in-range, reachable target; all others destroyed. From ANY random start the
    // round-robin must wrap to index 1.
    AimSubsystem subs[3];
    subs[0] = { vec3{10, 0, 0}, true,  true  };       // destroyed → skip
    subs[1] = { vec3{20, 0, 0}, false, true  };       // the only valid target
    subs[2] = { vec3{30, 0, 0}, true,  true  };       // destroyed → skip
    FiringAimInputs in;
    in.have_target_model = true;
    in.muzzle_valid = true;
    in.weapon_range = 100.0f;
    in.subsystems = subs; in.subsystem_count = 3;
    in.target_center = vec3{0, 0, -100};
    // run many seeds → must ALWAYS land on subs[1]
    for (eaw::u32 seed = 1; seed <= 200; ++seed) {
        SimRng r(seed);
        FiringAimResult res = firing_resolve_aimpoint(in, r);
        CHECK((res.aim == vec3{20, 0, 0}) && res.locked);
    }
}

static void test_phase2_reproducible() {
    std::printf("test_phase2_reproducible\n");
    AimSubsystem subs[5];
    for (int i = 0; i < 5; ++i) subs[i] = { vec3{(f32)(100+i), 0, 0}, false, true };
    FiringAimInputs in;
    in.have_target_model = true; in.muzzle_valid = true; in.weapon_range = 1e9f;
    in.subsystems = subs; in.subsystem_count = 5;
    SimRng a(77), b(77);
    CHECK((firing_resolve_aimpoint(in, a).aim == firing_resolve_aimpoint(in, b).aim));   // I2 contract
}

// ── phase 3: center fallback + the unconditional-extent difference ────────────────────────────────────
static void test_phase3_extent_unconditional() {
    std::printf("test_phase3_extent_unconditional\n");
    // No aim-bones, no subsystems. Center is at dist 30; weapon_range 25, extent 10.
    // PH3 adds extent UNCONDITIONALLY even though add_target_extent=false ⇒ effective range 35 >= 30 → locked.
    FiringAimInputs in;
    in.have_target_model = true;
    in.muzzle_valid = true;                           // muzzle {0,0,0}
    in.weapon_range = 25.0f;
    in.add_target_extent = false;                     // would gate OUT the extent in phase 1/2...
    in.target_extent = 10.0f;
    in.target_center = vec3{30, 0, 0};
    in.center_reachable = true;
    SimRng r(9);
    FiringAimResult res = firing_resolve_aimpoint(in, r);
    CHECK((res.aim == vec3{30, 0, 0}) && res.locked); // 30 <= 25+10

    // out of even the extended range → bare center, unlocked.
    in.target_center = vec3{40, 0, 0};
    FiringAimResult res2 = firing_resolve_aimpoint(in, r);
    CHECK((res2.aim == vec3{40, 0, 0}) && !res2.locked);   // 40 > 35

    // in range but unreachable → bare center, unlocked.
    in.target_center = vec3{30, 0, 0};
    in.center_reachable = false;
    FiringAimResult res3 = firing_resolve_aimpoint(in, r);
    CHECK((res3.aim == vec3{30, 0, 0}) && !res3.locked);
}

int main() {
    std::printf("=== firing_aimpoint_test ===\n");
    test_midpoint();
    test_in_range();
    test_phase0_no_model();
    test_phase1_hit_and_skips();
    test_phase1_unreachable_falls_through();
    test_phase2_round_robin_and_draw();
    test_phase2_destroyed_skip_and_wrap();
    test_phase2_reproducible();
    test_phase3_extent_unconditional();
    if (g_fail == 0) std::printf("ALL PASS\n");
    else std::printf("%d FAIL\n", g_fail);
    return g_fail ? 1 : 0;
}
