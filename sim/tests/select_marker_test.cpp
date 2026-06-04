// Host validation for the Select selection-marker behavior (sim/select_marker.{h,cpp}).
//
// Select's only deterministic core is the tail spring, which delegates to the shared damped-spring
// primitive (already ground-truthed against an inline-asm transcription of the binary's exact SSE
// sequence in damped_spring_test, and inheriting in-game bit-exactness from DTNEB's 614503/614503 on
// the identical instruction sequence). So this test confirms the DELEGATION is faithful — Select's
// step reproduces the shared primitive bit-for-bit — plus the marker's pulse semantics (springs to
// its selection-target equilibrium and rests there).
#include "../select_marker.h"
#include "../damped_spring.h"
#include <cstdio>
#include <cstdint>
#include <cstring>
#include <random>

using namespace sim;

static int g_fail = 0;
#define CHECK(cond) do { if (!(cond)) { \
    std::printf("  FAIL %s:%d  %s\n", __FILE__, __LINE__, #cond); ++g_fail; } } while (0)

static uint32_t bits(float f) { uint32_t u; std::memcpy(&u, &f, sizeof u); return u; }
static bool biteq(float a, float b) { return bits(a) == bits(b); }

int main() {
    // ---- 1. Select's step == the shared primitive, bit-for-bit, over a wide randomized stream. ----
    std::mt19937 rng(0x5E1EC7u);
    std::uniform_real_distribution<float> dfreq(0.0f, 40.0f);
    std::uniform_real_distribution<float> ddt(0.0f, 0.2f);
    std::uniform_real_distribution<float> dstate(-1000.0f, 1000.0f);
    long n = 0, bad = 0;
    for (int i = 0; i < 2000000; ++i) {
        float freq = dfreq(rng), dt = ddt(rng);
        float v0 = dstate(rng), e0 = dstate(rng), vl0 = dstate(rng);

        SelectMarkerState st; st.value = v0; st.equilibrium = e0; st.velocity = vl0; st.freq = freq;
        select_marker_spring_step(st, dt);

        float rv = v0, rvl = vl0;
        damped_spring_step(rv, rvl, e0, freq, dt);   // shared primitive directly

        ++n;
        if (!biteq(st.value, rv) || !biteq(st.velocity, rvl)) ++bad;
    }
    CHECK(bad == 0);
    std::printf("  select step vs shared primitive: %ld / %ld bit-exact (%ld bad)\n", n - bad, n, bad);

    // ---- 2. Pulse semantics: a marker kicked away from its target springs back and settles. ----
    {
        SelectMarkerState st; st.value = 0.0f; st.equilibrium = 1.0f; st.velocity = 0.0f; st.freq = 2.0f;
        for (int i = 0; i < 4000; ++i) select_marker_spring_step(st, 0.0333f);
        CHECK(st.value == st.value);                 // not NaN
        CHECK(st.value > 0.99f && st.value < 1.01f); // settled on the selection target
        CHECK(st.velocity > -0.001f && st.velocity < 0.001f);
    }

    // ---- 3. Deselect retarget: equilibrium flipped to 0 -> the pulse glides back toward rest. ----
    {
        SelectMarkerState st; st.value = 1.0f; st.equilibrium = 0.0f; st.velocity = 0.0f; st.freq = 2.0f;
        float first = st.value;
        select_marker_spring_step(st, 0.0333f);
        CHECK(st.value < first);                     // moved toward the new (0) target
    }

    if (g_fail == 0) std::printf("select_marker_test: ALL PASS\n");
    else             std::printf("select_marker_test: %d FAILURE(S)\n", g_fail);
    return g_fail ? 1 : 0;
}
