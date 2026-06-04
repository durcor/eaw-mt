// Host-side behavioral test for the Reveal (#10) fog re-reveal move-threshold gate (0x5373c0). These
// assert STRUCTURE + float32-only grouping + the strict-`<` boundary; the bit-exact-vs-binary check is
// the DTREVEAL in-game oracle.
#include "../reveal_gate.h"
#include <cstdio>
#include <cmath>
#include <cstring>

using namespace sim;
using eaw::f32;
using eaw::f64;

static int g_fail = 0;
#define CHECK(cond) do { if (!(cond)) { \
    std::printf("  FAIL %s:%d  %s\n", __FILE__, __LINE__, #cond); ++g_fail; } } while (0)

static void test_moved_sq_basics() {
    std::printf("test_moved_sq_basics\n");
    // no movement -> zero.
    CHECK(reveal_moved_sq(0, 0, 0, 0) == 0.0f);
    CHECK(reveal_moved_sq(123.0f, -45.0f, 123.0f, -45.0f) == 0.0f);
    // pure-axis moves: result is the squared component.
    CHECK(reveal_moved_sq(3, 0, 0, 0) == 9.0f);
    CHECK(reveal_moved_sq(0, 4, 0, 0) == 16.0f);
    // 3-4-5 in the plane.
    CHECK(reveal_moved_sq(3, 4, 0, 0) == 25.0f);
    // never negative.
    CHECK(reveal_moved_sq(-7, 11, 2, -5) >= 0.0f);
    // z is irrelevant by construction (no z parameter); the API only takes XY.
}

static void test_moved_sq_symmetry_and_translation() {
    std::printf("test_moved_sq_symmetry_and_translation\n");
    // direction-independent: swapping cur/last negates the deltas, squares identical.
    CHECK(reveal_moved_sq(1.5f, -2.25f, -3.0f, 4.0f)
          == reveal_moved_sq(-3.0f, 4.0f, 1.5f, -2.25f));
    // depends only on the delta: shifting both endpoints by the same exact-representable offset is
    // bit-stable here (small magnitudes, no extra rounding).
    CHECK(reveal_moved_sq(11.5f, 24.0f, 7.0f, 30.25f)
          == reveal_moved_sq(11.5f - 10.0f, 24.0f - 20.0f, 7.0f - 10.0f, 30.25f - 20.0f));
}

static void test_moved_sq_is_float32() {
    std::printf("test_moved_sq_is_float32\n");
    // Reproduce the exact float32 expression and require bit-for-bit equality.
    const f32 cx = 1234.567f, cy = -890.123f, lx = -45.6789f, ly = 678.9012f;
    const f32 dx = cx - lx, dy = cy - ly;
    const f32 expect = dx * dx + dy * dy;     // float32 throughout, single addss
    const f32 got = reveal_moved_sq(cx, cy, lx, ly);
    CHECK(std::memcmp(&got, &expect, sizeof got) == 0);

    // The gate must NOT widen to double. With a large dx and a tiny dy, a double-summed result would
    // retain dy²; the float32 addss rounds it off. Confirm we match the FLOAT32 (lossy) sum, not the
    // double one — i.e. we reproduce the binary's precision, not a "more accurate" version.
    const f32 bdx = 8192.0f, bdy = 0.01f;     // dx²=67108864, dy²=1e-4 -> lost in float32 add
    const f32 f32_sum = bdx * bdx + bdy * bdy;                       // == 67108864.0f (dy² rounded away)
    const f64 dbl_sum = static_cast<f64>(bdx * bdx) + static_cast<f64>(bdy * bdy); // keeps the 1e-4
    const f32 g = reveal_moved_sq(bdx, bdy, 0.0f, 0.0f);
    CHECK(g == f32_sum);
    CHECK(static_cast<f64>(g) != dbl_sum);    // proves we stayed in float32 (the binary's behavior)
}

static void test_skip_rescan_boundary() {
    std::printf("test_skip_rescan_boundary\n");
    // Strict `<`: at EXACTLY the threshold the unit is NOT skipped -> it rescans (gate opens).
    const f32 thr = 25.0f;                    // i.e. moved 5 units
    CHECK(reveal_skip_rescan(3, 4, 0, 0, thr) == false);   // moved_sq == 25, not < 25 -> rescan
    CHECK(reveal_skip_rescan(0, 0, 0, 0, thr) == true);    // moved_sq == 0  < 25      -> skip
    // just inside the radius -> skip; just outside -> rescan.
    CHECK(reveal_skip_rescan(4.999f, 0, 0, 0, thr) == true);    // ~24.99 < 25
    CHECK(reveal_skip_rescan(5.001f, 0, 0, 0, thr) == false);   // ~25.01 >= 25
    // zero threshold: any movement (moved_sq >= 0, never < 0) -> always rescan; even no movement
    // (moved_sq == 0, not < 0) rescans.
    CHECK(reveal_skip_rescan(0, 0, 0, 0, 0.0f) == false);
    CHECK(reveal_skip_rescan(0.001f, 0, 0, 0, 0.0f) == false);
}

static void test_skip_rescan_matches_explicit_compare() {
    std::printf("test_skip_rescan_matches_explicit_compare\n");
    // The predicate is exactly `moved_sq < threshold` in float32.
    const f32 cx = 17.5f, cy = -3.2f, lx = 14.0f, ly = 1.1f, thr = 30.0f;
    const f32 ms = reveal_moved_sq(cx, cy, lx, ly);
    CHECK(reveal_skip_rescan(cx, cy, lx, ly, thr) == (ms < thr));
}

int main() {
    std::printf("reveal_gate_test\n");
    test_moved_sq_basics();
    test_moved_sq_symmetry_and_translation();
    test_moved_sq_is_float32();
    test_skip_rescan_boundary();
    test_skip_rescan_matches_explicit_compare();
    if (g_fail == 0) std::printf("ALL PASS\n");
    else             std::printf("%d FAILURES\n", g_fail);
    return g_fail ? 1 : 0;
}
