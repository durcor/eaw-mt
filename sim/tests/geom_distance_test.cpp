// Host-side behavioral test for the shared 3D squared-distance primitive (FUN_140397640), the bit-
// matchable numeric core of LureBehaviorClass::vfunc_6 (IN behavior #9). These assert STRUCTURE +
// FP-grouping + edge cases; the bit-exact-vs-binary check is the DTLURE in-game oracle.
#include "../geom_distance.h"
#include <cstdio>
#include <cmath>

using namespace sim;
using eaw::vec3;
using eaw::f32;
using eaw::f64;

static int g_fail = 0;
#define CHECK(cond) do { if (!(cond)) { \
    std::printf("  FAIL %s:%d  %s\n", __FILE__, __LINE__, #cond); ++g_fail; } } while (0)

static bool near(f64 a, f64 b, f64 eps = 1e-6) { return std::fabs(a - b) <= eps; }

static void test_dist_sq_basics() {
    std::printf("test_dist_sq_basics\n");
    const vec3 o{0, 0, 0};

    // zero distance.
    CHECK(geom_dist_sq(o, o) == 0.0);
    // pure-axis distances: result is the squared component.
    CHECK(geom_dist_sq(o, vec3{3, 0, 0}) == 9.0);
    CHECK(geom_dist_sq(o, vec3{0, 4, 0}) == 16.0);
    CHECK(geom_dist_sq(o, vec3{0, 0, 5}) == 25.0);
    // 3-4-5 in the xy-plane.
    CHECK(geom_dist_sq(o, vec3{3, 4, 0}) == 25.0);
    // classic 3D triple 1-2-2 -> 9, and 2-3-6 -> 49.
    CHECK(geom_dist_sq(o, vec3{1, 2, 2}) == 9.0);
    CHECK(geom_dist_sq(o, vec3{2, 3, 6}) == 49.0);
    // never negative.
    CHECK(geom_dist_sq(vec3{-7, 11, -3}, vec3{2, -5, 8}) >= 0.0);
}

static void test_dist_sq_symmetry_and_translation() {
    std::printf("test_dist_sq_symmetry_and_translation\n");
    const vec3 a{1.5f, -2.25f, 0.75f};
    const vec3 b{-3.0f, 4.0f, 6.5f};

    // symmetric: d²(a,b) == d²(b,a) (deltas negate, squares identical).
    CHECK(geom_dist_sq(a, b) == geom_dist_sq(b, a));

    // translation-invariant: shifting both by the same offset leaves d² unchanged (bit-for-bit when the
    // shift is exactly representable and incurs no extra rounding).
    const vec3 s{10.0f, 20.0f, 30.0f};
    const vec3 a2{a.x + s.x, a.y + s.y, a.z + s.z};
    const vec3 b2{b.x + s.x, b.y + s.y, b.z + s.z};
    CHECK(near(geom_dist_sq(a, b), geom_dist_sq(a2, b2)));
}

static void test_dist_sq_grouping_and_promotion() {
    std::printf("test_dist_sq_grouping_and_promotion\n");
    // The squares are float32 then promoted to double; reproduce the exact expression and compare.
    const vec3 e{123.4567f, -89.0123f, 45.6789f};
    const vec3 q{-12.3456f, 7.89012f, -34.5678f};
    const f32 dx = e.x - q.x, dy = e.y - q.y, dz = e.z - q.z;
    const f64 expect = static_cast<f64>(dx * dx)
                     + static_cast<f64>(dy * dy)
                     + static_cast<f64>(dz * dz);
    CHECK(geom_dist_sq(e, q) == expect);   // bit-for-bit: same grouping, same float32 squares.

    // The double accumulation is observably different from a pure-float32 sum when the squares are large:
    // confirm we are NOT collapsing to float32 (i.e. the (double) promotion is real).
    const vec3 big_e{1e7f, 0, 0};
    const vec3 big_q{0, 1.0f, 0};
    // dx=1e7 -> dx²=1e14 (float32 representable exactly-ish), dy=-1 -> dy²=1; the +1 survives in double.
    const f64 r = geom_dist_sq(big_e, big_q);
    const f32 fdx = 1e7f, fdy = -1.0f;
    const f64 expect_big = static_cast<f64>(fdx * fdx) + static_cast<f64>(fdy * fdy)
                         + static_cast<f64>(0.0f);
    CHECK(r == expect_big);
}

static void test_within_range() {
    std::printf("test_within_range\n");
    const vec3 origin{0, 0, 0};
    // range 5: a point at distance exactly 5 (3-4-0) is IN (<=), one past it is OUT.
    CHECK(geom_within_range(vec3{3, 4, 0}, origin, 5.0f) == true);    // 25 <= 25
    CHECK(geom_within_range(vec3{3, 4, 0.001f}, origin, 5.0f) == false);  // 25.000001 > 25
    CHECK(geom_within_range(vec3{0, 0, 0}, origin, 5.0f) == true);    // 0 <= 25
    CHECK(geom_within_range(vec3{5, 0, 0}, origin, 5.0f) == true);    // 25 <= 25 (boundary inclusive)
    CHECK(geom_within_range(vec3{6, 0, 0}, origin, 5.0f) == false);   // 36 > 25
    // zero range: only the exact origin qualifies.
    CHECK(geom_within_range(origin, origin, 0.0f) == true);
    CHECK(geom_within_range(vec3{0.001f, 0, 0}, origin, 0.0f) == false);
    // the predicate equals the explicit comparison against (double)(range*range).
    const vec3 p{7.5f, -3.2f, 1.1f};
    const f32 range = 8.25f;
    CHECK(geom_within_range(p, origin, range)
          == (geom_dist_sq(p, origin) <= static_cast<f64>(range * range)));
}

int main() {
    std::printf("geom_distance_test\n");
    test_dist_sq_basics();
    test_dist_sq_symmetry_and_translation();
    test_dist_sq_grouping_and_promotion();
    test_within_range();
    if (g_fail == 0) std::printf("ALL PASS\n");
    else             std::printf("%d FAILURES\n", g_fail);
    return g_fail ? 1 : 0;
}
