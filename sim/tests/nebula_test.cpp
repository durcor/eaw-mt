// Host-side behavioral test for NebulaBehaviorClass::vfunc_6 (RVA 0x437b60), the fifth in-slice IN
// sim behavior. Validates:
//   * the damped-oscillator spring step matches an independent reference computation bit-exactly,
//   * the spring relaxes toward equilibrium and degenerates to a linear glide when freq == 0,
//   * the membership SM enter edge (-1 -> in): fires enter fx, sets equilibrium=1, latches the tick,
//   * the already-inside case: no enter fx, latch refreshed,
//   * the leave edge (in -> -1): fires leave fx, sets equilibrium=0, clears the latch,
//   * out -> out: no leave fx (no spurious edge),
//   * the LINGER throttle: pins value/equilibrium/velocity to full only when equilibrium < 1,
//   * nebula_tick runs the spring BEFORE the membership update (engine order).
#include "../nebula.h"
#include <cstdio>
#include <cmath>
#include <cstring>

using namespace sim;

static int g_fail = 0;
#define CHECK(cond) do { if (!(cond)) { \
    std::printf("  FAIL %s:%d  %s\n", __FILE__, __LINE__, #cond); ++g_fail; } } while (0)

// Bit-exact equality on the float store (the in-game oracle demands exact reproduction).
static bool biteq(float a, float b) {
    return std::memcmp(&a, &b, sizeof(float)) == 0;
}
static bool feq(float a, float b) { return std::fabs(a - b) <= 1e-5f * (1.0f + std::fabs(b)); }

// Independent reference for the spring step, computed in the SAME op order as the lift, to confirm
// the lift's arithmetic is what we think it is (a guard against a typo in the operation order).
// Reference spring with the BINARY's denominator grouping `quad + (wdt+1.0) + cubic` (verified against
// the asm in damped_spring.cpp; the explicit parens are load-bearing — see damped_spring.h). Confirms
// nebula_spring_step delegates to the shared primitive faithfully.
static void ref_spring(float& value, float& velocity, float equilibrium, float freq, float dt) {
    const float w     = freq * 2.0f;
    const float wdt   = w * dt;
    const float dx    = value - equilibrium;
    const float quad  = (wdt * 0.48f) * wdt;
    const float cubic = ((wdt * 0.235f) * wdt) * wdt;
    const float inv   = 1.0f / ((quad + (wdt + 1.0f)) + cubic);
    const float tmp   = (dx * w + velocity) * dt;
    value    = (tmp + dx) * inv + equilibrium;
    velocity = (velocity - tmp * w) * inv;
}

static void test_spring_matches_reference() {
    std::printf("test_spring_matches_reference\n");
    // A spread of (value, equilibrium, velocity, freq, dt) including off-equilibrium and moving cases.
    const float cases[][5] = {
        {0.0f, 1.0f, 0.0f, 1.5f, 0.0333f},   // just entered, ramping up
        {0.8f, 1.0f, 0.3f, 2.0f, 0.0333f},   // nearly full, still moving
        {1.0f, 0.0f, 0.0f, 1.5f, 0.0333f},   // just left, ramping down
        {0.5f, 0.0f, -0.2f, 0.75f, 0.05f},   // low freq, fading
        {0.123f, 0.456f, 0.789f, 3.3f, 0.02f},
    };
    for (auto& c : cases) {
        NebulaState st; st.value = c[0]; st.equilibrium = c[1]; st.velocity = c[2]; st.freq = c[3];
        float rv = c[0], rvel = c[2];
        ref_spring(rv, rvel, c[1], c[3], c[4]);
        nebula_spring_step(st, c[4]);
        CHECK(biteq(st.value, rv));
        CHECK(biteq(st.velocity, rvel));
    }
}

static void test_spring_relaxes_to_equilibrium() {
    std::printf("test_spring_relaxes_to_equilibrium\n");
    NebulaState st; st.value = 0.0f; st.equilibrium = 1.0f; st.velocity = 0.0f; st.freq = 1.5f;
    for (int i = 0; i < 2000; ++i) nebula_spring_step(st, 0.0333f);
    CHECK(feq(st.value, 1.0f));            // converged to the target
    CHECK(feq(st.velocity, 0.0f));          // and came to rest
}

static void test_spring_freq_zero_is_linear_glide() {
    std::printf("test_spring_freq_zero_is_linear_glide\n");
    // freq==0 -> w=0, wdt=0, inv=1, tmp=velocity*dt -> value += velocity*dt, velocity unchanged.
    NebulaState st; st.value = 0.2f; st.equilibrium = 0.9f; st.velocity = 0.5f; st.freq = 0.0f;
    nebula_spring_step(st, 0.1f);
    CHECK(feq(st.value, 0.2f + 0.5f * 0.1f));
    CHECK(feq(st.velocity, 0.5f));
}

static void test_enter_edge() {
    std::printf("test_enter_edge\n");
    NebulaState st;                          // enter_tick = -1 (outside)
    NebulaFx fx;
    nebula_membership_update(st, NebulaDisposition::InNebula, 100, fx);
    CHECK(fx.enter == true);
    CHECK(fx.leave == false);
    CHECK(feq(st.equilibrium, 1.0f));
    CHECK(st.enter_tick == 100);
}

static void test_already_inside_no_reedge() {
    std::printf("test_already_inside_no_reedge\n");
    NebulaState st; st.enter_tick = 100; st.equilibrium = 1.0f;
    NebulaFx fx;
    nebula_membership_update(st, NebulaDisposition::InNebula, 130, fx);
    CHECK(fx.enter == false);                // no re-fire while already inside
    CHECK(st.enter_tick == 130);             // latch refreshed
    CHECK(feq(st.equilibrium, 1.0f));
}

static void test_leave_edge() {
    std::printf("test_leave_edge\n");
    NebulaState st; st.enter_tick = 130; st.equilibrium = 1.0f;
    NebulaFx fx;
    nebula_membership_update(st, NebulaDisposition::OutOfNebula, 160, fx);
    CHECK(fx.leave == true);
    CHECK(fx.enter == false);
    CHECK(feq(st.equilibrium, 0.0f));
    CHECK(st.enter_tick == -1);
}

static void test_out_to_out_no_edge() {
    std::printf("test_out_to_out_no_edge\n");
    NebulaState st;                          // enter_tick = -1, equilibrium = 0
    NebulaFx fx;
    nebula_membership_update(st, NebulaDisposition::OutOfNebula, 200, fx);
    CHECK(fx.leave == false);                // was already outside -> no spurious leave
    CHECK(st.enter_tick == -1);
    CHECK(feq(st.equilibrium, 0.0f));
}

static void test_linger_pins_when_below_full() {
    std::printf("test_linger_pins_when_below_full\n");
    NebulaState st; st.value = 0.4f; st.equilibrium = 0.7f; st.velocity = 0.9f; st.enter_tick = 100;
    NebulaFx fx;
    nebula_membership_update(st, NebulaDisposition::Linger, 110, fx);
    CHECK(feq(st.value, 1.0f));
    CHECK(feq(st.equilibrium, 1.0f));
    CHECK(feq(st.velocity, 0.0f));           // the 8-byte store zeroes velocity too
    CHECK(fx.enter == false && fx.leave == false);
    CHECK(st.enter_tick == 100);             // linger does NOT touch the latch
}

static void test_linger_noop_when_already_full() {
    std::printf("test_linger_noop_when_already_full\n");
    NebulaState st; st.value = 0.95f; st.equilibrium = 1.0f; st.velocity = 0.3f; st.enter_tick = 100;
    NebulaFx fx;
    nebula_membership_update(st, NebulaDisposition::Linger, 110, fx);
    CHECK(feq(st.equilibrium, 1.0f));
    CHECK(feq(st.velocity, 0.3f));           // equilibrium already 1.0 -> no re-pin, velocity intact
    CHECK(feq(st.value, 0.95f));
}

static void test_tick_runs_spring_before_membership() {
    std::printf("test_tick_runs_spring_before_membership\n");
    // On the enter tick, the spring integrates toward the OLD equilibrium (0) first, THEN membership
    // sets equilibrium=1 for next tick. So value should still be near 0 after this tick, not snapped.
    NebulaState st; st.value = 0.0f; st.equilibrium = 0.0f; st.velocity = 0.0f; st.freq = 1.5f;
    NebulaFx fx = nebula_tick(st, NebulaDisposition::InNebula, 100, 0.0333f);
    CHECK(fx.enter == true);
    CHECK(feq(st.equilibrium, 1.0f));        // membership moved the target
    CHECK(feq(st.value, 0.0f));              // but the spring this tick used the old target (0)
    CHECK(st.enter_tick == 100);
}

int main() {
    test_spring_matches_reference();
    test_spring_relaxes_to_equilibrium();
    test_spring_freq_zero_is_linear_glide();
    test_enter_edge();
    test_already_inside_no_reedge();
    test_leave_edge();
    test_out_to_out_no_edge();
    test_linger_pins_when_below_full();
    test_linger_noop_when_already_full();
    test_tick_runs_spring_before_membership();
    if (g_fail == 0) std::printf("All nebula checks passed.\n");
    else             std::printf("%d nebula CHECK(s) FAILED.\n", g_fail);
    return g_fail ? 1 : 0;
}
