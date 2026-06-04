// sim/select_marker.cpp — SelectBehaviorClass::vfunc_6 (RVA 0x3c2310) deterministic core.
// See select_marker.h for the full identification, the presentation-orchestration env, and the
// threading classification (Class 3, off-lockstep).
#include "select_marker.h"

namespace sim {

// The tail spring (3c2310.c lines 33-41) IS the engine's shared damped harmonic oscillator, inlined
// byte-identically to Nebula's STAGE 1. Delegate to the one asm-faithful transcription.
void select_marker_spring_step(SelectMarkerState& st, float dt) {
    damped_spring_step(st.value, st.velocity, st.equilibrium, st.freq, dt);
}

} // namespace sim
