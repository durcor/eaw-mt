// sim/damage_tracking.cpp — DamageTrackingBehaviorClass::vfunc_6 (RVA 0x58bd80) deterministic core.
// See damage_tracking.h for the full identification and determinism notes.
#include "damage_tracking.h"

namespace sim {

void damage_tracking_tick(DamageTrackingState& st, void* entity,
                          float decay, int interval, float tick_hz,
                          CommandSink& sink) {
    // (1) base prestep (reschedule) handled by the spine schedule gate — not modelled here.

    // (2)+(3) decay each active effect; remove the expired ones. The live binary walks the intrusive
    // list head->tail, writing back `duration -= decay` and removing nodes whose new duration is NOT
    // > 0 (so an effect landing exactly on 0 is removed). We iterate the insertion-ordered vector and
    // compact survivors in place — same observable (the single empty-transition emit is removal-order
    // independent; see header). 58bd80.c lines 27-86.
    bool became_empty = false;
    if (!st.effects.empty()) {
        std::size_t w = 0;
        for (std::size_t r = 0; r < st.effects.size(); ++r) {
            const float nd = st.effects[r].duration - decay;   // node+0x14 -= decay (written back)
            if (nd > 0.0f) {                                    // 0.0 < fVar11 → keep
                st.effects[w] = st.effects[r];
                st.effects[w].duration = nd;
                ++w;
            } else {
                // expire: in-engine unlink + free(0x18) + count(+0x20)-=1; here just drop. The empty
                // transition (count==0) is what triggers the Phase-B emit, edge-triggered on the LAST
                // expiry, so only the FINAL surviving-count matters, not which node cleared it.
            }
        }
        if (w == 0) became_empty = true;
        st.effects.resize(w);
    }

    // (4) PHASE-B: list went empty THIS tick → emit parameterless 0x2d on entity+0x38. 58bd80.c:63-82.
    // Edge-triggered: only when at least one effect expired and none survive (an already-empty list
    // never enters the loop, so it never emits).
    if (became_empty) {
        sink.emit_signal(entity, kSigDamageEffectsCleared);
    }

    // (5) tail rate readout. 58bd80.c lines 87-91.  *(sub+0xc) = (tick_hz * accum) / interval;
    // accum := 0.  Float division mirrors the binary's `(float)iVar3` divisor (interval==0 → inf,
    // as it would in-engine — callers supply the behavior's real +0x34).
    st.damage_rate  = (tick_hz * st.damage_accum) / static_cast<float>(interval);
    st.damage_accum = 0.0f;
}

} // namespace sim
