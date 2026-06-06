// Host-side behavioral test for the projectile-intercept lead solver (b2 piece 1: FUN_140399e20 +
// FUN_140393b70 simple mode). Asserts STRUCTURE, branch coverage, and FP grouping; the bit-exact-vs-
// binary check is the in-game oracle (DTFINT, to follow). Hand-verified geometry below.
#include "../firing_intercept.h"
#include <cstdio>
#include <cmath>

using namespace sim;
using eaw::vec3;
using eaw::f32;

static int g_fail = 0;
#define CHECK(cond) do { if (!(cond)) { \
    std::printf("  FAIL %s:%d  %s\n", __FILE__, __LINE__, #cond); ++g_fail; } } while (0)

static bool near(f32 a, f32 b, f32 eps = 1e-3f) { return std::fabs(a - b) <= eps; }

// ── the linear target-position predictor (393b70 simple mode) ─────────────────────────────────────────
static void test_predict_linear() {
    std::printf("test_predict_linear\n");

    // stationary in its own frame (tgt_vel == frame_vel) ⇒ predicted == current pos, any t.
    const vec3 p{1, 2, 3};
    CHECK(firing_predict_pos_linear(p, vec3{5, 5, 5}, vec3{5, 5, 5}, 1.0f, 99.0f) == p);

    // moving: pred.k = (vel.k - frame_vel.k) * (g*t) + pos.k.  g=2, t=3 → f=6; relvel=(3,4,5).
    const vec3 r = firing_predict_pos_linear(vec3{1, 2, 3}, vec3{4, 6, 8}, vec3{1, 2, 3}, 2.0f, 3.0f);
    CHECK(near(r.x, 19.0f) && near(r.y, 26.0f) && near(r.z, 33.0f));   // (3·6+1, 4·6+2, 5·6+3)

    // frame_vel cancels the carrier velocity: pred tracks only relative motion.
    const vec3 s = firing_predict_pos_linear(vec3{0, 0, 0}, vec3{10, 0, 0}, vec3{10, 0, 0}, 1.0f, 5.0f);
    CHECK((s == vec3{0, 0, 0}));
}

// ── the quadratic intercept (399e20) ──────────────────────────────────────────────────────────────────
static void test_intercept_stationary() {
    std::printf("test_intercept_stationary\n");
    // target at rest at origin; muzzle 100 away on +x; projectile speed 10.  t = dist/speed = 10.
    // For a stationary target pred == tgt_pos, so aim = (muzzle - tgt) + tgt ≈ muzzle.
    bool ok = false;
    const vec3 aim = firing_intercept_lead(vec3{0, 0, 0}, vec3{0, 0, 0}, vec3{0, 0, 0},
                                           vec3{100, 0, 0}, vec3{0, 0, 0}, 1.0f, 10.0f, &ok);
    CHECK(ok);
    CHECK(near(aim.x, 100.0f) && near(aim.y, 0.0f) && near(aim.z, 0.0f));
}

static void test_intercept_moving_lead() {
    std::printf("test_intercept_moving_lead\n");
    // target at origin moving +y at 10; muzzle {100,0,0}; shooter ref {0,0,0}; projectile speed 20.
    //   relvel=(0,10,0) a=100-400=-300 b=0 c=10000 disc=12e6 root=3464.10 → t=+5.7735
    //   pred = (0, 10·5.7735, 0) = (0, 57.735, 0); aim = rel(100,0,0)+pred = (100, 57.735, 0)
    bool ok = false;
    const vec3 aim = firing_intercept_lead(vec3{0, 0, 0}, vec3{0, 10, 0}, vec3{0, 0, 0},
                                           vec3{100, 0, 0}, vec3{0, 0, 0}, 1.0f, 20.0f, &ok);
    CHECK(ok);
    CHECK(near(aim.x, 100.0f, 1e-2f));
    CHECK(near(aim.y, 57.735f, 1e-2f));
    CHECK(near(aim.z, 0.0f));
}

static void test_intercept_no_solution() {
    std::printf("test_intercept_no_solution\n");
    // target crossing perpendicular (vel +y=1000) FASTER than the projectile (speed 5) ⇒ disc < 0.
    bool ok = true;
    const vec3 aim = firing_intercept_lead(vec3{0, 0, 0}, vec3{0, 1000, 0}, vec3{0, 0, 0},
                                           vec3{100, 0, 0}, vec3{0, 0, 0}, 1.0f, 5.0f, &ok);
    CHECK(!ok);
    CHECK((aim == vec3{0, 0, 0}));   // the binary's no-solution sentinel
}

static void test_intercept_smallest_positive_root() {
    std::printf("test_intercept_smallest_positive_root\n");
    // sanity: a valid intercept returns a finite, non-zero aim and reports a solution.
    // (muzzle != shooter_ref so sep != 0; target drifting -y, fast projectile ⇒ a real positive root.)
    bool ok = false;
    const vec3 aim = firing_intercept_lead(vec3{50, 0, 0}, vec3{0, -5, 0}, vec3{0, 0, 0},
                                           vec3{0, 50, 0}, vec3{0, 0, 0}, 1.0f, 30.0f, &ok);
    CHECK(ok);
    CHECK((!(aim == vec3{0, 0, 0})));
    CHECK(std::isfinite(aim.x) && std::isfinite(aim.y) && std::isfinite(aim.z));
}

// ── weapon dispersion (381dc0) ────────────────────────────────────────────────────────────────────────
using eaw::SimRng;

// expected RNG state after N range_f draws from `seed` (range_f = one LCG step each).
static eaw::u32 state_after(eaw::u32 seed, int n) {
    SimRng r(seed);
    for (int i = 0; i < n; ++i) r.step();
    return r.state;
}

static void test_spread_no_draws() {
    std::printf("test_spread_no_draws\n");
    const vec3 d{1, 2, 3};
    // accuracy-off flag → dir unchanged, state untouched.
    { SimRng r(12345); const vec3 o = firing_apply_spread(d, true, 5.0f, 5.0f, 100.0f, 1.0f, r);
      CHECK((o == d)); CHECK(r.state == 12345u); }
    // base<=0, secondary normalizer <= 0 → no spread, no draws.
    { SimRng r(777); const vec3 o = firing_apply_spread(d, false, 0.0f, 2.0f, 50.0f, 0.0f, r);
      CHECK((o == d)); CHECK(r.state == 777u); }
    // base<=0 and (sec<=0 or dist<=0) → no spread, no draws.
    { SimRng r(777); const vec3 o = firing_apply_spread(d, false, -1.0f, 0.0f, 50.0f, 1.0f, r);
      CHECK((o == d)); CHECK(r.state == 777u); }
    { SimRng r(777); const vec3 o = firing_apply_spread(d, false, -1.0f, 2.0f, 0.0f, 1.0f, r);
      CHECK((o == d)); CHECK(r.state == 777u); }
}

static void test_spread_primary() {
    std::printf("test_spread_primary\n");
    const vec3 d{10, 20, 30};
    const f32 base = 4.0f;
    SimRng r(0xC0FFEE);
    const vec3 o = firing_apply_spread(d, false, base, 0.0f, 0.0f, 0.0f, r);
    // exactly 3 draws consumed.
    CHECK(r.state == state_after(0xC0FFEE, 3));
    // each axis perturbed within [-base, base].
    CHECK(near(o.x, d.x, base) && near(o.y, d.y, base) && near(o.z, d.z, base));
}

static void test_spread_secondary_scaled() {
    std::printf("test_spread_secondary_scaled\n");
    const vec3 d{0, 0, 0};
    const f32 sec = 0.1f, dist = 200.0f, norm = 4.0f;   // m = sec*dist/norm = 5.0
    const f32 m = (sec * dist) / norm;
    SimRng r(42);
    const vec3 o = firing_apply_spread(d, false, 0.0f, sec, dist, norm, r);
    CHECK(r.state == state_after(42, 3));               // 3 draws
    CHECK(near(o.x, 0.0f, m) && near(o.y, 0.0f, m) && near(o.z, 0.0f, m));
}

static void test_spread_reproducible() {
    std::printf("test_spread_reproducible\n");
    const vec3 d{1, 1, 1};
    SimRng a(99), b(99);
    const vec3 oa = firing_apply_spread(d, false, 3.0f, 0.0f, 0.0f, 0.0f, a);
    const vec3 ob = firing_apply_spread(d, false, 3.0f, 0.0f, 0.0f, 0.0f, b);
    CHECK((oa == ob));   // identical seed ⇒ identical perturbation (reproducibility, the I2 contract)
}

int main() {
    std::printf("=== firing_intercept_test ===\n");
    test_predict_linear();
    test_intercept_stationary();
    test_intercept_moving_lead();
    test_intercept_no_solution();
    test_intercept_smallest_positive_root();
    test_spread_no_draws();
    test_spread_primary();
    test_spread_secondary_scaled();
    test_spread_reproducible();
    if (g_fail == 0) std::printf("ALL PASS\n");
    else std::printf("%d FAIL\n", g_fail);
    return g_fail ? 1 : 0;
}
