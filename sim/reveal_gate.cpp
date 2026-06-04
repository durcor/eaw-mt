// sim/reveal_gate.cpp — implementation of the Reveal (#10) move-threshold gate inside
// RevealBehaviorClass::vfunc_6 (0x5373c0). See reveal_gate.h for the asm and FP-grouping rationale.
#include "reveal_gate.h"

namespace sim {

eaw::f32 reveal_moved_sq(eaw::f32 cur_x, eaw::f32 cur_y, eaw::f32 last_x, eaw::f32 last_y) {
    // Deltas in float32 (subss). Every intermediate stays float32 — the squares (mulss) and their sum
    // (addss) are NOT widened to double, matching the binary exactly (contrast #9's double-promoted leaf).
    const eaw::f32 dx = cur_x - last_x;
    const eaw::f32 dy = cur_y - last_y;
    return dx * dx + dy * dy;   // (dx² + dy²), single float32 addss
}

bool reveal_skip_rescan(eaw::f32 cur_x, eaw::f32 cur_y, eaw::f32 last_x, eaw::f32 last_y, eaw::f32 threshold) {
    // comiss + jb: take the skip branch iff the squared move distance is strictly below the threshold.
    return reveal_moved_sq(cur_x, cur_y, last_x, last_y) < threshold;
}

} // namespace sim
