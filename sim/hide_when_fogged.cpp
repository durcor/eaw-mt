// sim/hide_when_fogged.cpp — HideWhenFoggedBehaviorClass::vfunc_6 (RVA 0x53ddc0) deterministic core.
// See hide_when_fogged.h for the full identification, the presentation-orchestration env (throttle,
// visibility decision, render toggle, instant-reveal snap), and the threading classification
// (Class 3, off-lockstep).
#include "hide_when_fogged.h"

namespace sim {

// The fog-fade spring (53ddc0.c lines 68-79) IS the engine's shared damped harmonic oscillator,
// inlined byte-identically to Nebula STAGE 1 and Select (same SSE sequence, same five constants, same
// load-bearing denominator add-grouping). Delegate to the one asm-faithful transcription.
void hide_when_fogged_spring_step(HideFogState& st, float dt) {
    damped_spring_step(st.value, st.velocity, st.equilibrium, st.freq, dt);
}

} // namespace sim
