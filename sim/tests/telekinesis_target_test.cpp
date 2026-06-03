// Host-side behavioral test for TelekinesisTargetBehaviorClass::vfunc_6 (RVA 0x63f210), the eighth
// in-slice IN sim behavior — the shared interp-timeline core + the mode dispatch. Validates:
//   * the dispatch: no slot -> NoSlot; mode 1/2/3 -> Grab/Hold/Release; everything else -> Idle,
//   * the interp timeline: now = sim_clock/hz; t = max((now-start)/duration, 0) (clamp-low only);
//     complete = (1.0 <= t), incl. the exact t==1.0 boundary and the negative-t clamp,
//   * the scalar lerp endpoints + midpoint, and that it stays consistent with the timeline,
//   * the angle gate: <= 180.0 collapses to 0.0, else 360.0 (incl. the exact 180.0 boundary),
//   * the grip-target value (angle base + slot offset).
#include "../telekinesis_target.h"
#include <cstdio>
#include <cstring>
#include <initializer_list>

using namespace sim;

static int g_fail = 0;
#define CHECK(cond) do { if (!(cond)) { \
    std::printf("  FAIL %s:%d  %s\n", __FILE__, __LINE__, #cond); ++g_fail; } } while (0)

// Bit-exact float compare (the in-game oracle validates these quantities bit-for-bit).
static bool feq(float a, float b) { return std::memcmp(&a, &b, sizeof(float)) == 0; }

static void test_dispatch() {
    std::printf("test_dispatch\n");
    CHECK(telekinesis_dispatch(false, 1) == TelekinesisImpl::NoSlot);   // no slot shadows the mode int
    CHECK(telekinesis_dispatch(false, 2) == TelekinesisImpl::NoSlot);
    CHECK(telekinesis_dispatch(true, 1)  == TelekinesisImpl::Grab);
    CHECK(telekinesis_dispatch(true, 2)  == TelekinesisImpl::Hold);
    CHECK(telekinesis_dispatch(true, 3)  == TelekinesisImpl::Release);
    // every other mode -> Idle (sample 0, negatives, and out-of-range positives).
    for (int m : {0, 4, 5, 0x20, -1, 100, 0x7fffffff}) {
        CHECK(telekinesis_dispatch(true, m) == TelekinesisImpl::Idle);
    }
}

static void test_now() {
    std::printf("test_now\n");
    CHECK(feq(telekinesis_now(0, 30), 0.0f));
    CHECK(feq(telekinesis_now(30, 30), 1.0f));    // one second of ticks at 30 Hz
    CHECK(feq(telekinesis_now(45, 30), 1.5f));
    CHECK(feq(telekinesis_now(90, 60), 1.5f));    // hz factors out
}

static void test_t_clamp_and_complete() {
    std::printf("test_t_clamp_and_complete\n");
    // mid-interp: start=1.0s, duration=2.0s, now=2.0s -> t = (2-1)/2 = 0.5, not complete.
    {
        TelekinesisTimeline tl = telekinesis_timeline(/*clock*/60, /*hz*/30, /*start*/1.0f, /*dur*/2.0f);
        CHECK(feq(tl.now, 2.0f));
        CHECK(feq(tl.t, 0.5f));
        CHECK(!tl.complete);
    }
    // exact boundary: now-start == duration -> t == 1.0 -> complete (the `1.0 <= t` test is inclusive).
    {
        TelekinesisTimeline tl = telekinesis_timeline(/*clock*/90, /*hz*/30, /*start*/1.0f, /*dur*/2.0f);
        CHECK(feq(tl.now, 3.0f));
        CHECK(feq(tl.t, 1.0f));
        CHECK(tl.complete);
    }
    // past the end: t > 1.0 -> complete (no upper clamp; t keeps growing).
    {
        TelekinesisTimeline tl = telekinesis_timeline(/*clock*/150, /*hz*/30, /*start*/1.0f, /*dur*/2.0f);
        CHECK(feq(tl.t, 2.0f));
        CHECK(tl.complete);
    }
    // negative raw t (start in the future) clamps to 0, not complete.
    {
        TelekinesisTimeline tl = telekinesis_timeline(/*clock*/30, /*hz*/30, /*start*/5.0f, /*dur*/2.0f);
        CHECK(feq(tl.t, 0.0f));
        CHECK(!tl.complete);
    }
}

static void test_lerp() {
    std::printf("test_lerp\n");
    CHECK(feq(telekinesis_lerp(10.0f, 20.0f, 0.0f), 10.0f));   // t=0 -> from
    CHECK(feq(telekinesis_lerp(10.0f, 20.0f, 1.0f), 20.0f));   // t=1 -> to
    CHECK(feq(telekinesis_lerp(10.0f, 20.0f, 0.5f), 15.0f));   // midpoint
    CHECK(feq(telekinesis_lerp(-4.0f, 4.0f, 0.25f), -2.0f));
    // op order: (to-from)*t + from — endpoints land exactly even with awkward values.
    CHECK(feq(telekinesis_lerp(3.0f, 3.0f, 0.7f), 3.0f));      // zero-length interp is a no-op
}

static void test_angle_gate() {
    std::printf("test_angle_gate\n");
    CHECK(feq(telekinesis_angle_gate(0.0f), 0.0f));
    CHECK(feq(telekinesis_angle_gate(90.0f), 0.0f));
    CHECK(feq(telekinesis_angle_gate(180.0f), 0.0f));         // exact boundary collapses (<=)
    CHECK(feq(telekinesis_angle_gate(180.001f), 360.0f));     // just past -> full
    CHECK(feq(telekinesis_angle_gate(270.0f), 360.0f));
    CHECK(feq(telekinesis_angle_gate(-5.0f), 0.0f));
}

static void test_target() {
    std::printf("test_target\n");
    CHECK(feq(telekinesis_target(100.0f, 5.0f), 105.0f));
    CHECK(feq(telekinesis_target(0.0f, 0.0f), 0.0f));
    CHECK(feq(telekinesis_target(-30.0f, 30.0f), 0.0f));
}

int main() {
    std::printf("telekinesis_target_test\n");
    test_dispatch();
    test_now();
    test_t_clamp_and_complete();
    test_lerp();
    test_angle_gate();
    test_target();
    if (g_fail == 0) std::printf("ALL PASS\n");
    else             std::printf("%d FAILURES\n", g_fail);
    return g_fail ? 1 : 0;
}
