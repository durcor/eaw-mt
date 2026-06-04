// sim/damped_spring.cpp — the shared damped harmonic oscillator step. See damped_spring.h.
//
// The float operation order and the addition GROUPING are preserved EXACTLY as the binary emits them
// (the in-game oracles rely on bit-for-bit reproduction). The intermediate names below correspond to
// the disassembly registers (Select 0x3c2389-0x3c242c / Nebula 0x437b9d-0x437c56):
//   xmm6 = w     xmm2 = wdt (then dx)   xmm3 = quad/denom/tmp/value'   xmm1 = cubic/vel'
//   xmm0 = wdt+1.0 (then tmp*w)         xmm4 = 1.0 (then inv)
#include "damped_spring.h"

namespace sim {

void damped_spring_step(float& value, float& velocity, float equilibrium, float freq, float dt) {
    const float w     = freq * kSpringFreqScale;                    // xmm6
    const float wdt   = w * dt;                                     // xmm2
    const float quad  = (wdt * kSpringQuad) * wdt;                  // xmm3 = (wdt*0.48)*wdt
    const float cubic = ((wdt * kSpringCubic) * wdt) * wdt;         // xmm1 = (wdt*0.235)*wdt*wdt
    const float lin1  = wdt + 1.0f;                                 // xmm0 = wdt + 1.0  (own sub-sum)
    const float denom = (quad + lin1) + cubic;                     // xmm3 = quad + (wdt+1.0) + cubic
    const float inv   = 1.0f / denom;                              // xmm4
    const float dx    = value - equilibrium;                       // xmm2
    const float tmp   = ((dx * w) + velocity) * dt;               // xmm3
    value    = ((tmp + dx) * inv) + equilibrium;                  // xmm3 -> store
    velocity = (velocity - (tmp * w)) * inv;                       // xmm1 -> store
}

} // namespace sim
