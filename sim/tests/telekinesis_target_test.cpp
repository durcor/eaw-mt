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
#include <cmath>
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

// ── Mode-2 (HOLD) core ─────────────────────────────────────────────────────────────────────────────

static void test_hold_timeline() {
    std::printf("test_hold_timeline\n");
    // elapsed = now - start.
    CHECK(feq(telekinesis_hold_elapsed(5.0f, 2.0f), 3.0f));
    CHECK(feq(telekinesis_hold_elapsed(2.0f, 2.0f), 0.0f));
    // spin_t = max(elapsed/period, 0) — clamp-low only.
    CHECK(feq(telekinesis_spin_t(3.0f, 2.0f), 1.5f));
    CHECK(feq(telekinesis_spin_t(0.0f, 2.0f), 0.0f));
    CHECK(feq(telekinesis_spin_t(-4.0f, 2.0f), 0.0f));     // negative elapsed clamps to 0
    // angle = spin_t * omega.
    CHECK(feq(telekinesis_hold_angle(1.5f, 4.0f), 6.0f));
    CHECK(feq(telekinesis_hold_angle(0.0f, 4.0f), 0.0f));
}

static void test_hold_ramp() {
    std::printf("test_hold_ramp\n");
    // below ramp_dur -> linear ramp from 0; at/after -> pinned to 1.0.
    CHECK(feq(telekinesis_ramp(0.0f, 4.0f), 0.0f));
    CHECK(feq(telekinesis_ramp(1.0f, 4.0f), 0.25f));
    CHECK(feq(telekinesis_ramp(4.0f, 4.0f), 1.0f));        // exact boundary -> not (<) -> pinned 1.0
    CHECK(feq(telekinesis_ramp(10.0f, 4.0f), 1.0f));       // well past -> 1.0
    // future-dated start (elapsed<0) is NOT clamped low here (faithful to the engine).
    CHECK(feq(telekinesis_ramp(-2.0f, 4.0f), -0.5f));
}

static void test_hold_trig_guard() {
    std::printf("test_hold_trig_guard\n");
    // exponent field > 0x3a -> use the real routine; tiny / zero angles bypass to the small-angle limit.
    CHECK(telekinesis_use_real_trig(6.0f));                // ordinary angle -> real trig
    CHECK(telekinesis_use_real_trig(1.0f));
    CHECK(telekinesis_use_real_trig(-3.0f));               // magnitude-only (sign outside the mask)
    CHECK(!telekinesis_use_real_trig(0.0f));               // exactly zero -> exponent 0 -> bypass
    // a value whose biased exponent is exactly 0x3a (== threshold, not >) bypasses; the next exponent
    // step uses trig. NB: only the EXPONENT field is compared — a mantissa bit (0x1d000001) does not
    // cross the threshold; the exponent must actually increment (0x1d800000 -> field 0x3b).
    {
        uint32_t at_thr   = 0x1d000000u;                   // exp field == 0x3a, mantissa 0
        uint32_t mant_bit = 0x1d000001u;                   // same exp field, +1 mantissa -> still bypass
        uint32_t next_exp = 0x1d800000u;                   // exp field == 0x3b -> first value to use trig
        float fa, fm, fn;
        std::memcpy(&fa, &at_thr, 4); std::memcpy(&fm, &mant_bit, 4); std::memcpy(&fn, &next_exp, 4);
        CHECK(!telekinesis_use_real_trig(fa));             // not strictly greater -> bypass
        CHECK(!telekinesis_use_real_trig(fm));             // mantissa bit doesn't change the exp field
        CHECK(telekinesis_use_real_trig(fn));
    }
}

static void test_hold_pos_z() {
    std::printf("test_hold_pos_z\n");
    // pos_z = slot_z + sin_term*bob + offset. With sin_term=0 the bob vanishes, leaving slot_z+offset.
    CHECK(feq(telekinesis_hold_pos_z(/*slot_z*/100.0f, /*sin*/0.0f, /*bob*/5.0f, /*off*/12.0f), 112.0f));
    CHECK(feq(telekinesis_hold_pos_z(100.0f, 1.0f, 5.0f, 12.0f), 117.0f));      // full +bob
    CHECK(feq(telekinesis_hold_pos_z(100.0f, -1.0f, 5.0f, 12.0f), 107.0f));     // trough
    // grouping is slot_z + (sin*bob + offset) — verified against the binary codegen, NOT the flattened
    // decompile. The inner (sin*bob + offset) rounds first, then slot_z is added.
    {
        const float slot_z = 0.0f, off = 3.5f, bob = 2.0f, s = 0.25f;
        CHECK(feq(telekinesis_hold_pos_z(slot_z, s, bob, off), slot_z + (s * bob + off)));
    }
    // a magnitude that exposes the non-associativity: with a large slot_z and a small sin*bob the two
    // groupings round differently — the lift must match the (sin*bob + off)-first grouping.
    {
        const float slot_z = 1.0e7f, bob = 1.0f, off = 0.0f, s = 0.30000001f;
        const float right = slot_z + (s * bob + off);    // binary grouping
        CHECK(feq(telekinesis_hold_pos_z(slot_z, s, bob, off), right));
    }
    // end-to-end with a real angle: compose the lifted core exactly as the oracle will (libm trig here).
    {
        const float spin_t = telekinesis_spin_t(/*elapsed*/0.5f, /*period*/2.0f);   // 0.25
        const float angle  = telekinesis_hold_angle(spin_t, /*omega*/8.0f);          // 2.0
        const float sin_term = telekinesis_use_real_trig(angle) ? std::sin(angle) : angle;
        const float z = telekinesis_hold_pos_z(/*slot_z*/50.0f, sin_term, /*bob*/3.0f, /*off*/10.0f);
        CHECK(feq(z, 50.0f + (std::sin(2.0f) * 3.0f + 10.0f)));
    }
}

static void test_hold_damage_scheduler() {
    std::printf("test_hold_damage_scheduler\n");
    // due iff deadline <= now AND deadline != 0.
    CHECK(telekinesis_damage_due(/*deadline*/2.0f, /*now*/2.0f));     // exact deadline fires (<=)
    CHECK(telekinesis_damage_due(1.5f, 2.0f));                        // overdue fires
    CHECK(!telekinesis_damage_due(3.0f, 2.0f));                       // not yet
    CHECK(!telekinesis_damage_due(0.0f, 2.0f));                       // zero deadline == disarmed
    CHECK(!telekinesis_damage_due(0.0f, 0.0f));
    // rebase pushes the deadline forward by exactly one interval (interval added on the left).
    CHECK(feq(telekinesis_damage_rebase(2.0f, 0.2f), 2.2f));
    CHECK(feq(telekinesis_damage_rebase(0.2f, 0.2f), 0.4f));
    // a multi-tick cadence: stepping now forward fires once per interval crossing, deadline marching up.
    {
        float deadline = 1.0f;
        const float interval = 0.2f;
        int fires = 0;
        for (float now = 1.0f; now <= 1.81f; now += 0.1f) {
            // the engine re-checks after each rebase only on the next tick (single fire per tick), so we
            // model the per-tick single fire: fire at most once, then advance.
            if (telekinesis_damage_due(deadline, now)) { ++fires; deadline = telekinesis_damage_rebase(deadline, interval); }
        }
        // now sweeps 1.0,1.1,...,1.8 (9 ticks); deadlines 1.0,1.2,1.4,1.6,1.8 each get crossed -> 5 fires.
        CHECK(fires == 5);
    }
}

int main() {
    std::printf("telekinesis_target_test\n");
    test_dispatch();
    test_now();
    test_t_clamp_and_complete();
    test_lerp();
    test_angle_gate();
    test_target();
    test_hold_timeline();
    test_hold_ramp();
    test_hold_trig_guard();
    test_hold_pos_z();
    test_hold_damage_scheduler();
    if (g_fail == 0) std::printf("ALL PASS\n");
    else             std::printf("%d FAILURES\n", g_fail);
    return g_fail ? 1 : 0;
}
