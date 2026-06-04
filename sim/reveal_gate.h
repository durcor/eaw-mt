// sim/reveal_gate.h — Phase-3 IN sim behavior #10 (Reveal) numeric core: the per-unit fog-of-war
// re-reveal MOVE-THRESHOLD GATE inside RevealBehaviorClass::vfunc_6 (0x5373c0).
//
// RevealBehaviorClass is NOT a special ability — it is the passive fog-of-war revealer carried by
// essentially every mobile unit (`Land_FOW_Reveal_Range` on ~550 land units, `Space_FOW_Reveal_Range`
// on ~241 space units). Each tick the behavior must decide whether to RE-COMPUTE the set of objects it
// reveals. Doing the full scan every tick would be wasteful, so the engine throttles it on movement:
// it only re-scans once the unit has moved far enough from the position where it last revealed.
//
// Like Lure #9 / UnitAI #4 / Targeting #7, the bulk of vfunc_6 is a CROSS-ENTITY orchestration seam:
// when the gate opens it walks the global object list (DAT_140a16fd0), runs a visibility/team predicate
// (FUN_14039a2c0) per object, and reveals each (FUN_140365760 / FUN_14035d1b0). That scan + reveal-emit
// is the deferred Phase-B territory (live global-set reads + cross-boundary fog writes), NOT a numeric
// core. The ONE embedded, pure, bit-matchable numeric core is the move-threshold gate itself.
//
// ── The primitive (verified bit-for-bit against the 0x5373c0 asm) ───────────────────────────────────
//   dx = cur.x - last.x          // float32 subss   (cur = entity world pos +0x78/+0x7c; last = +0x40/+0x44)
//   dy = cur.y - last.y          // float32 subss   (XY ONLY — z/+0x80 is not consulted by the gate)
//   moved_sq = dx*dx + dy*dy     // float32 mulss, mulss, ADDSS   (no double promotion)
//   skip_rescan = moved_sq < threshold     // comiss + jb against the float32 threshold at +0x48
//
// Asm (0x5373c0, the gate):
//   movss  xmm6,[rbp+0x78]                     // cur.x  (also re-stored to last.x after a rescan)
//   subss  xmm1,[rsi+0x40]                     // dx = cur.x - last.x
//   movss  xmm7,[rbp+0x7c]                     // cur.y  (also re-stored to last.y after a rescan)
//   subss  xmm0,[rsi+0x44]                     // dy = cur.y - last.y
//   mulss  xmm1,xmm1                           // dx²  (float32)
//   mulss  xmm0,xmm0                           // dy²  (float32)
//   addss  xmm1,xmm0                           // moved_sq = dx² + dy²   (float32 — NOT addsd)
//   comiss xmm1,[rsi+0x48]                     // compare moved_sq vs threshold
//   jb     0x14053762a                         // if moved_sq < threshold -> skip the rescan
//
// ── Bit-exactness notes ─────────────────────────────────────────────────────────────────────────────
//   * Unlike #9's FUN_140397640 (3D, squares promoted to DOUBLE then summed in double), this gate is
//     2D and stays entirely in float32: addss, comiss — the threshold is a float32 too. So the lift must
//     keep every op in `float`, never widening to double.
//   * Only two terms, so the add-grouping (dx² + dy²) is trivially unambiguous (single addss).
//   * Comparison is strict `<` (comiss/jb, branch on CF). At the unordered (NaN) edge comiss sets CF so
//     the binary takes the skip branch, whereas C `<` is false for NaN; moved_sq is a sum of squares and
//     is never NaN for finite inputs, so this differs only theoretically. The DTREVEAL oracle compares
//     the float32 moved_sq bit-for-bit and the branch decision, over real (finite) game positions.
#pragma once
#include "eaw_types.h"

namespace sim {

// reveal_moved_sq — the squared planar (XY) displacement of a unit since its last fog re-reveal, in
// float32 exactly as the binary computes it: (cur-last) per axis (subss), squared (mulss), summed (addss),
// with NO promotion to double. `cur` is the entity world position (GameObjectClass+0x78/+0x7c); `last` is
// the behavior's stored last-reveal position (RevealBehavior slot +0x40/+0x44). The z axis is ignored.
eaw::f32 reveal_moved_sq(eaw::f32 cur_x, eaw::f32 cur_y, eaw::f32 last_x, eaw::f32 last_y);

// reveal_skip_rescan — the comiss/jb branch decision: returns true ("the unit has not moved far enough,
// keep the existing reveal set and skip the costly re-scan") iff moved_sq < threshold. `threshold` is the
// behavior's squared move-distance gate (slot +0x48). The compare is the binary's strict float32 `<`.
bool reveal_skip_rescan(eaw::f32 cur_x, eaw::f32 cur_y, eaw::f32 last_x, eaw::f32 last_y, eaw::f32 threshold);

} // namespace sim
