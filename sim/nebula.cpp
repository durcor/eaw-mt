// sim/nebula.cpp — NebulaBehaviorClass::vfunc_6 (RVA 0x437b60) deterministic core.
// See nebula.h for the full identification and determinism notes.
#include "nebula.h"
#include "damped_spring.h"

namespace sim {

// STAGE 1 — the semi-implicit damped harmonic oscillator (437b60.c lines 41-51) is the engine's
// SHARED spring primitive, inlined here byte-identically to SelectBehaviorClass::vfunc_6's tail. It is
// now delegated to sim/damped_spring.{h,cpp} so both callers share one asm-faithful transcription.
//
// NOTE — denominator grouping correction: this body originally wrote the denominator as
//   1.0 / (wdt*0.48*wdt + wdt + 1.0 + wdt*0.235*wdt*wdt)
// whose C left-assoc grouping `((quad + wdt) + 1.0) + cubic` is NOT the binary's `quad + (wdt+1.0) +
// cubic`. The two diverge in the last ULP for ~9% of inputs over a wide range, but coincide on every
// sample of Nebula's actual (small-wdt) distribution — which is why DTNEB passed 614503/614503 with
// the old grouping. The shared primitive uses the binary's grouping, which still reproduces Nebula
// bit-for-bit (the two agree on Nebula's data) and is additionally correct for Select's wider range.
void nebula_spring_step(NebulaState& st, float dt) {
    damped_spring_step(st.value, st.velocity, st.equilibrium, st.freq, dt);
}

// STAGE 2 — membership / throttle. 437b60.c lines 52-139.
void nebula_membership_update(NebulaState& st, NebulaDisposition disp, int now, NebulaFx& fx) {
    fx = NebulaFx{};
    switch (disp) {
    case NebulaDisposition::Linger:
        // 437b60.c lines 135-138: within the post-entry grace window — skip the scan and re-pin the
        // spring to full, but only if it has drifted below 1.0. The engine writes equilibrium and
        // velocity together as one 8-byte store (0x3f800000 low, 0 high) -> equilibrium=1, velocity=0.
        if (st.equilibrium < 1.0f) {
            st.value       = 1.0f;
            st.equilibrium = 1.0f;
            st.velocity    = 0.0f;
        }
        break;

    case NebulaDisposition::InNebula:
        // 437b60.c lines 96-109: a nebula object was found in range.
        if (st.enter_tick == -1) {       // -1 -> in edge: newly entered
            fx.enter       = true;        // FUN_1404376f0 (emit 0x2b per nebula-affected ability)
            st.equilibrium = 1.0f;        // sub+0x120 (4-byte store; velocity untouched here)
        }
        st.enter_tick = now;              // always refresh the latch while inside
        break;

    case NebulaDisposition::OutOfNebula: {
        // 437b60.c lines 128-133: no nebula object in range.
        const int was = st.enter_tick;
        st.enter_tick = -1;
        if (was != -1) {                  // in -> -1 edge: newly left
            fx.leave       = true;        // FUN_14039fb40 (emit 0x2c re-enable)
            st.equilibrium = 0.0f;        // sub+0x120
        }
        break;
    }
    }
}

// STAGE 1 then STAGE 2, matching the engine's order: the spring integrates toward the CURRENT
// equilibrium first, then membership may move the equilibrium / fire an edge for next tick.
NebulaFx nebula_tick(NebulaState& st, NebulaDisposition disp, int now, float dt) {
    nebula_spring_step(st, dt);
    NebulaFx fx;
    nebula_membership_update(st, disp, now, fx);
    return fx;
}

} // namespace sim
