// sim/energy_pool.cpp — EnergyPoolBehaviorClass::vfunc_6 (RVA 0x56c030) deterministic core.
// See energy_pool.h for the full identification and determinism notes.
#include "energy_pool.h"

namespace sim {

// Mirror the in-engine clamp ordering: the value is composed first, then bounded to [0, max].
// 56c030.c lines 58-61: fVar4 = 0;  if (0 <= v && v <= max) fVar4 = v;  else if (0 <= v) fVar4 = max;
// i.e. v<0 -> 0;  v>max -> max;  else v.  Written as the same two compares the binary performs.
static float clamp_energy(float v, float max) {
    float out = 0.0f;                 // fVar4 = 0.0  (the v<0 result)
    if (0.0f <= v) {
        out = max;                    // fVar4 = fVar5 (the v>max result)
        if (v <= max) out = v;        // in-range
    }
    return out;
}

EnergyResult energy_pool_tick(EnergyPoolState& st,
                              DrainDisposition drain,
                              float regen_rate, float dt, float max_energy,
                              bool local_selected) {
    // (0) prologue (interval refresh + base prestep/reschedule) handled by the spine — not here.

    bool wrote = false;

    // (1) DRAIN GATE. 56c030.c lines 15-36.
    switch (drain) {
    case DrainDisposition::SourceMissing:
        // Drain-capable unit whose source object is null this tick → bail before any write/ui flag.
        return EnergyResult::NoDrainSource;
    case DrainDisposition::Draining:
        // FORCE-DRAIN: value = min(max_energy, 0). Since max is always >= 0 the binary's
        // `if (0 <= fVar3) fVar3 = 0` collapses this to exactly 0 — modelled faithfully (the
        // max_energy query result is read then discarded by the >=0 clamp).
        if (!st.has_pool) return EnergyResult::Ok;     // pool==0 → Ok, no write, no ui flag
        st.value = (max_energy >= 0.0f) ? 0.0f : max_energy;
        wrote = true;
        break;
    case DrainDisposition::NoCapability:
    case DrainDisposition::NotDraining:
        // (2) NORMAL REGEN. 56c030.c lines 38-65.
        // Guards: pool present + regen not disabled. (template-present is a defensive in-engine
        // null check that never trips for a real EnergyPool unit; folded into the env's resolution.)
        if (!st.has_pool || st.disabled) return EnergyResult::Ok;  // guarded no-op, no ui flag
        st.value = clamp_energy(st.value + regen_rate * dt, max_energy);
        wrote = true;
        break;
    }

    // (3) UI FLAG — reached only after a write (LAB_14056c180). Presentation-only refresh bit.
    if (wrote && local_selected) st.ui_dirty = true;
    return EnergyResult::Ok;
}

} // namespace sim
