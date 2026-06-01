// sim/ability_countdown.cpp — AbilityCountdownBehaviorClass::vfunc_6 (RVA 0x42f910).
// See ability_countdown.h for the full per-tick algorithm and determinism notes.
#include "ability_countdown.h"

namespace sim {

void ability_countdown_tick(AbilityCountdownState& st, void* owner, int tick,
                            AbilityCountdownEnv& env, CommandSink& sink) {
    // 42f910: mgr = entity+0x1e8; null → guarded no-op (no stamp written).
    if (!st.has_manager) return;

    // delta = tick - mgr->last_tick(+0x2c0): integer ticks since the last service.
    const int delta = tick - st.last_tick;

    for (int i = 0; i < AbilityCountdownState::kNumSlots; ++i) {
        int t = st.timer[i];

        if (st.mode[i] == 0) {
            // COUNTDOWN: only an active cooldown (t>0) ticks; this is the edge guard that makes the
            // 0x2c emit fire exactly once (a slot already at 0 is skipped entirely).
            if (t > 0) {
                t -= delta;
                if (t < 1) t = 0;       // engine floor: if (iVar5 < 1) iVar5 = 0
                st.timer[i] = t;
                if (t == 0 && env.ability_ready(owner, i)) {
                    // 42f910: build AbilitySignalData{vftable, slot} and emit 0x2c on owner+0x38.
                    AbilitySignalData d;
                    d.slot = i;
                    sink.emit_signal(owner, kSigAbilityRecharged, &d, sizeof d);
                }
            }
        } else {
            // CHARGEUP: clamp UP toward the per-slot target; invoke the complete-callback whenever
            // the slot is at its target (re-fires while pinned — the engine's `if (iVar5 == iVar2)`
            // sits AFTER the `if (iVar5 < iVar2)` clamp block).
            const int max = st.target[i];
            if (t < max) {
                int v = t + delta;
                t = (v < max) ? v : max;   // engine: default to max, take t+delta only if below
                st.timer[i] = t;
            }
            if (t == max) env.charge_complete(owner, i);
        }
    }

    // 42f910: stamp this service tick (mgr+0x2c0 = param_3) so next tick's delta is measured here.
    st.last_tick = tick;
}

} // namespace sim
