// Host-side behavioral test for AsteroidFieldDamageBehaviorClass::vfunc_6 (RVA 0x437310), the sixth
// in-slice IN sim behavior (sibling of Nebula #5). Validates:
//   * the per-asteroid gate roll matches an independent reference draw bit-exactly and advances the
//     sim RNG exactly once,
//   * roll <= probability is the apply predicate (boundary cases prob=0+ and prob>=1),
//   * the proximity stamp: last_hit_tick = now when an asteroid is in range, -1 when none,
//   * a full tick draws exactly `asteroids_in_range` gate rolls (the determinism-contract count),
//   * an inactive tick (gate failed) leaves state UNTOUCHED and draws nothing.
#include "../asteroid_field_damage.h"
#include <cstdio>
#include <cmath>
#include <cstring>
#include <initializer_list>

using namespace sim;

static int g_fail = 0;
#define CHECK(cond) do { if (!(cond)) { \
    std::printf("  FAIL %s:%d  %s\n", __FILE__, __LINE__, #cond); ++g_fail; } } while (0)

static bool biteq(float a, float b) { return std::memcmp(&a, &b, sizeof(float)) == 0; }

// Independent reference for FUN_1401ffbb0(seed, 0, 1) computed in the binary's op order, to confirm
// the gate roll is exactly the sim LCG draw mapped to [0,1).
static float ref_roll01(eaw::u32& s) {
    s = s * 0x41c64e6du + 0xbdfu;
    float field = (float)(unsigned)((s >> 10) & 0x7fffu);
    return (1.0f - 0.0f) * (field / 32767.0f) + 0.0f;
}

static void test_roll_matches_reference_and_advances_once() {
    std::printf("test_roll_matches_reference_and_advances_once\n");
    eaw::u32 seeds[] = {1u, 12345u, 0xDEADBEEFu, 0u, 0x7fffffffu};
    for (eaw::u32 seed : seeds) {
        eaw::SimRng rng(seed);
        eaw::u32 rs = seed;
        float ref = ref_roll01(rs);
        // probability 1.0 => roll <= 1.0 always true, but we only want to read the roll value here,
        // so reproduce manually: the roll must equal ref and the state must equal the reference state.
        eaw::SimRng probe(seed);
        float got = probe.range_f(0.0f, 1.0f);
        CHECK(biteq(got, ref));
        CHECK(probe.state == rs);
        // asteroid_damage_roll uses the same single draw.
        (void)asteroid_damage_roll(rng, 1.0f);
        CHECK(rng.state == rs);   // advanced exactly once
    }
}

static void test_apply_predicate_boundaries() {
    std::printf("test_apply_predicate_boundaries\n");
    // prob >= 1.0: a [0,1) roll is always <= 1.0 -> always apply.
    for (eaw::u32 seed : {1u, 999u, 0xABCDu}) {
        eaw::SimRng rng(seed);
        CHECK(asteroid_damage_roll(rng, 1.0f) == true);
    }
    // prob just above 0: applies only when the roll is exactly 0 (field==0). With a tiny positive
    // probability and a generic seed the roll is almost surely > prob -> no apply; verify it at least
    // returns a bool consistent with roll<=prob by recomputing.
    for (eaw::u32 seed : {1u, 12345u, 0xDEADBEEFu}) {
        eaw::SimRng a(seed), b(seed);
        float roll = b.range_f(0.0f, 1.0f);
        bool expect = roll <= 0.001f;
        CHECK(asteroid_damage_roll(a, 0.001f) == expect);
    }
}

static void test_proximity_stamp() {
    std::printf("test_proximity_stamp\n");
    AsteroidFieldDamageState st;
    asteroid_field_stamp(st, /*found_any=*/true, /*now=*/4242);
    CHECK(st.last_hit_tick == 4242);
    asteroid_field_stamp(st, /*found_any=*/false, /*now=*/4243);
    CHECK(st.last_hit_tick == -1);
}

static void test_tick_draw_count_and_stamp() {
    std::printf("test_tick_draw_count_and_stamp\n");
    // n in-range asteroids => exactly n draws, last_hit_tick = now, applied counts roll<=prob.
    for (int n : {0, 1, 3, 7}) {
        eaw::SimRng rng(54321u), ref(54321u);
        AsteroidFieldDamageState st; st.last_hit_tick = -999;
        AsteroidFieldTickResult r =
            asteroid_field_tick(st, rng, /*active=*/true, n, /*prob=*/0.5f, /*now=*/100 + n);
        // Reference: n manual draws, count applies.
        int expect_applied = 0;
        for (int i = 0; i < n; ++i) {
            float roll = ref.range_f(0.0f, 1.0f);
            if (roll <= 0.5f) ++expect_applied;
        }
        CHECK(r.active == true);
        CHECK(r.rolled == n);
        CHECK(r.applied == expect_applied);
        CHECK(rng.state == ref.state);                 // exactly n advances
        CHECK(r.found_any == (n > 0));
        CHECK(st.last_hit_tick == (n > 0 ? 100 + n : -1));
    }
}

static void test_inactive_tick_is_noop() {
    std::printf("test_inactive_tick_is_noop\n");
    eaw::SimRng rng(7u);
    AsteroidFieldDamageState st; st.last_hit_tick = 555;
    AsteroidFieldTickResult r =
        asteroid_field_tick(st, rng, /*active=*/false, /*n=*/5, /*prob=*/0.5f, /*now=*/9000);
    CHECK(r.active == false);
    CHECK(r.rolled == 0);
    CHECK(r.applied == 0);
    CHECK(rng.state == 7u);            // no draws — RNG untouched
    CHECK(st.last_hit_tick == 555);    // state untouched
}

int main() {
    std::printf("asteroid_field_damage_test\n");
    test_roll_matches_reference_and_advances_once();
    test_apply_predicate_boundaries();
    test_proximity_stamp();
    test_tick_draw_count_and_stamp();
    test_inactive_tick_is_noop();
    if (g_fail == 0) std::printf("ALL PASS\n");
    else             std::printf("%d FAILURES\n", g_fail);
    return g_fail ? 1 : 0;
}
