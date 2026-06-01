// sim/nebula.cpp — NebulaBehaviorClass::vfunc_6 (RVA 0x437b60) deterministic core.
// See nebula.h for the full identification and determinism notes.
#include "nebula.h"

namespace sim {

// STAGE 1 — the semi-implicit damped harmonic oscillator. 437b60.c lines 41-51.
//
// The float operation order is preserved EXACTLY as the binary emits it so the result is bit-for-bit
// reproducible against the live engine (the in-game oracle relies on this):
//   line 43: fVar16 = freq * 2.0                                                 -> w
//   line 44: fVar13 = w * dt                                                     -> wdt
//   line 45: fVar14 = value - equilibrium                                        -> dx
//   line 46-48: fVar15 = 1.0 / ( wdt*0.48*wdt + wdt + 1.0 + wdt*0.235*wdt*wdt )  -> inv
//              (left-assoc: (((wdt*0.48)*wdt) + wdt) + 1.0) + (((wdt*0.235)*wdt)*wdt))
//   line 49: fVar13 = (dx*w + velocity) * dt                                     -> tmp  (reuses fVar13)
//   line 50: value    = (tmp + dx) * inv + equilibrium
//   line 51: velocity = (velocity - tmp*w) * inv
// The denominator is >= 1 for any wdt >= 0 (freq, dt non-negative), so inv never divides by zero.
void nebula_spring_step(NebulaState& st, float dt) {
    const float w   = st.freq * kNebulaFreqScale;                  // fVar16
    const float wdt = w * dt;                                      // fVar13 (first use)
    const float dx  = st.value - st.equilibrium;                   // fVar14
    const float inv = 1.0f /
        (wdt * kNebulaQuad * wdt + wdt + 1.0f + wdt * kNebulaCubic * wdt * wdt);   // fVar15
    const float tmp = (dx * w + st.velocity) * dt;                 // fVar13 (reused)
    st.value    = (tmp + dx) * inv + st.equilibrium;               // sub+0x11c
    st.velocity = (st.velocity - tmp * w) * inv;                   // sub+0x124
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
