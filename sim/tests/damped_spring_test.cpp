// Host validation for the shared damped-spring primitive (sim/damped_spring.{h,cpp}).
//
// GROUND TRUTH = an inline-asm transcription of the binary's EXACT SSE instruction sequence (the same
// movss/mulss/addss/subss/divss in the same order and the same register-level grouping as
// 0x3c2389-0x3c242c / 0x437b9d-0x437c56). The lifted C `damped_spring_step` must reproduce it
// BIT-FOR-BIT across a wide randomized input stream — proving the lift matches the instruction
// sequence with no FMA, no reassociation, no double promotion (the #10 reveal_gate methodology). This
// is the host-side half; the in-game half is INHERITED from DTNEB, which validated this identical
// instruction sequence live at 614503/614503 (same opcodes, different state offsets).
//
// The test also documents WHY the denominator grouping is load-bearing: it counts how often the
// binary grouping `quad+(wdt+1)+cubic` diverges from the left-assoc `((quad+wdt)+1)+cubic` that a
// naive decompile transcription (the original nebula.cpp) would produce.
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

// Inline-asm transcription of the binary spring tail, instruction-for-instruction.
static void spring_asm(float& value, float& velocity, float equilibrium, float freq, float dt) {
    const float one = 1.0f, c2 = 2.0f, q = 0.48f, k = 0.235f;
    float f = freq, d = dt, v = value, e = equilibrium, vl = velocity;
    float rv = 0.0f, rvl = 0.0f;
    __asm__ volatile(
        "movss   %[f], %%xmm6\n\t"      // xmm6 = freq
        "mulss   %[c2], %%xmm6\n\t"     // xmm6 = w = freq*2.0
        "movss   %[one], %%xmm4\n\t"    // xmm4 = 1.0
        "movaps  %%xmm6, %%xmm2\n\t"    // xmm2 = w
        "mulss   %[d], %%xmm2\n\t"      // xmm2 = wdt = w*dt
        "movaps  %%xmm2, %%xmm3\n\t"    // xmm3 = wdt
        "movaps  %%xmm2, %%xmm1\n\t"    // xmm1 = wdt
        "mulss   %[q], %%xmm3\n\t"      // xmm3 = wdt*0.48
        "movaps  %%xmm2, %%xmm0\n\t"    // xmm0 = wdt
        "mulss   %[k], %%xmm1\n\t"      // xmm1 = wdt*0.235
        "addss   %%xmm4, %%xmm0\n\t"    // xmm0 = wdt + 1.0
        "mulss   %%xmm2, %%xmm3\n\t"    // xmm3 = (wdt*0.48)*wdt = quad
        "mulss   %%xmm2, %%xmm1\n\t"    // xmm1 = (wdt*0.235)*wdt
        "addss   %%xmm0, %%xmm3\n\t"    // xmm3 = quad + (wdt+1.0)
        "mulss   %%xmm2, %%xmm1\n\t"    // xmm1 = cubic
        "movss   %[v], %%xmm2\n\t"      // xmm2 = value
        "subss   %[e], %%xmm2\n\t"      // xmm2 = dx = value - eq
        "addss   %%xmm1, %%xmm3\n\t"    // xmm3 = denom = quad+(wdt+1.0)+cubic
        "movss   %[vl], %%xmm1\n\t"     // xmm1 = vel
        "divss   %%xmm3, %%xmm4\n\t"    // xmm4 = inv = 1.0/denom
        "movaps  %%xmm2, %%xmm3\n\t"    // xmm3 = dx
        "mulss   %%xmm6, %%xmm3\n\t"    // xmm3 = dx*w
        "addss   %[vl], %%xmm3\n\t"     // xmm3 = dx*w + vel
        "mulss   %[d], %%xmm3\n\t"      // xmm3 = tmp = (dx*w+vel)*dt
        "movaps  %%xmm3, %%xmm0\n\t"    // xmm0 = tmp
        "addss   %%xmm2, %%xmm3\n\t"    // xmm3 = tmp + dx
        "mulss   %%xmm6, %%xmm0\n\t"    // xmm0 = tmp*w
        "mulss   %%xmm4, %%xmm3\n\t"    // xmm3 = (tmp+dx)*inv
        "subss   %%xmm0, %%xmm1\n\t"    // xmm1 = vel - tmp*w
        "addss   %[e], %%xmm3\n\t"      // xmm3 = (tmp+dx)*inv + eq = value'
        "mulss   %%xmm4, %%xmm1\n\t"    // xmm1 = (vel-tmp*w)*inv = vel'
        "movss   %%xmm3, %[rv]\n\t"
        "movss   %%xmm1, %[rvl]\n\t"
        : [rv] "=m"(rv), [rvl] "=m"(rvl)
        : [f] "m"(f), [c2] "m"(c2), [one] "m"(one), [d] "m"(d), [q] "m"(q), [k] "m"(k),
          [v] "m"(v), [e] "m"(e), [vl] "m"(vl)
        : "xmm0", "xmm1", "xmm2", "xmm3", "xmm4", "xmm6");
    value = rv;
    velocity = rvl;
}

int main() {
    std::mt19937 rng(0x5E1EC7u);
    // Wide ranges — deliberately wider than Nebula's so the grouping-sensitive inputs are exercised.
    std::uniform_real_distribution<float> dfreq(0.0f, 40.0f);
    std::uniform_real_distribution<float> ddt(0.0f, 0.2f);
    std::uniform_real_distribution<float> dstate(-1000.0f, 1000.0f);

    // ---- 1. Bit-for-bit equivalence to the inline-asm ground truth over a large input stream. ----
    long n = 0, bad = 0;
    long group_div = 0;   // how often the binary grouping differs from left-assoc (documents the trap)
    for (int i = 0; i < 4000000; ++i) {
        float freq = dfreq(rng), dt = ddt(rng);
        float v0 = dstate(rng), e0 = dstate(rng), vl0 = dstate(rng);

        float lv = v0, lvl = vl0;   // lifted
        damped_spring_step(lv, lvl, e0, freq, dt);
        float av = v0, avl = vl0;   // asm ground truth
        spring_asm(av, avl, e0, freq, dt);

        ++n;
        if (!biteq(lv, av) || !biteq(lvl, avl)) {
            if (bad < 5) std::printf("  MISMATCH freq=%g dt=%g v=%g e=%g vl=%g  lift=(%g,%g) asm=(%g,%g)\n",
                                     freq, dt, v0, e0, vl0, lv, lvl, av, avl);
            ++bad;
        }

        // Re-derive both denominator groupings to quantify the divergence the grouping guards against.
        float w = freq * 2.0f, wdt = w * dt;
        float quad = (wdt * 0.48f) * wdt, cubic = ((wdt * 0.235f) * wdt) * wdt;
        float dbin = (quad + (wdt + 1.0f)) + cubic;     // binary grouping
        float dlas = ((quad + wdt) + 1.0f) + cubic;     // naive left-assoc
        if (!biteq(dbin, dlas)) ++group_div;
    }
    CHECK(bad == 0);
    std::printf("  spring vs inline-asm ground truth: %ld / %ld bit-exact (%ld bad)\n", n - bad, n, bad);
    std::printf("  denominator grouping divergence (binary vs left-assoc): %ld / %ld (%.2f%%) — "
                "non-zero proves the explicit parenthesization is load-bearing\n",
                group_div, n, 100.0 * group_div / n);
    CHECK(group_div > 0);   // sanity: our wide range MUST exercise the grouping-sensitive inputs

    // ---- 2. Determinism: same inputs -> identical outputs (no hidden state). ----
    {
        float a = 3.0f, b = 0.0f, c = 3.0f, d = 0.0f;   // identical start state
        damped_spring_step(a, b, 1.0f, 2.0f, 0.0333f);
        damped_spring_step(c, d, 1.0f, 2.0f, 0.0333f);
        CHECK(biteq(a, c) && biteq(b, d));
    }

    // ---- 3. Stability: starting away from equilibrium with positive freq/dt, the spring converges
    //         (and never NaNs / blows up — denom >= 1 guarantees a finite inv). ----
    {
        float value = 0.0f, vel = 0.0f, eq = 1.0f;
        for (int i = 0; i < 4000; ++i) damped_spring_step(value, vel, eq, 1.5f, 0.0333f);
        CHECK(value == value);                    // not NaN
        CHECK(value > 0.99f && value < 1.01f);    // settled near equilibrium
        CHECK(vel > -0.001f && vel < 0.001f);     // velocity damped out
    }

    // ---- 4. freq == 0 degenerates to a linear glide: value += velocity*dt, velocity unchanged. ----
    {
        float value = 10.0f, vel = 4.0f, eq = -3.0f, dt = 0.25f;
        float v0 = value, vl0 = vel;
        damped_spring_step(value, vel, eq, 0.0f, dt);
        // w=0 -> wdt=0 -> denom=1 -> inv=1; dx=v0-eq; tmp=(0+vel)*dt; value'=tmp+dx+eq=v0+vel*dt; vel'=vel.
        CHECK(biteq(value, v0 + vl0 * dt));
        CHECK(biteq(vel, vl0));
    }

    if (g_fail == 0) std::printf("damped_spring_test: ALL PASS\n");
    else             std::printf("damped_spring_test: %d FAILURE(S)\n", g_fail);
    return g_fail ? 1 : 0;
}
