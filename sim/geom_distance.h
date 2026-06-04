// sim/geom_distance.h — Phase-3 IN sim behavior #9 (Lure) numeric core: the shared 3D squared-distance
// primitive FUN_140397640.
//
// LureBehaviorClass::vfunc_6 (0x62b4c0) is — like UnitAI #4 / Targeting #7 — predominantly a CROSS-ENTITY
// orchestration seam: once its cooldown is due and its use-count is under the cap, it scans the squads in
// the lure's target list, and for every member entity within the ability's radius (and passing a faction
// predicate) calls the per-target lure-apply FUN_14062b270. That scan + apply is the deferred Phase-B
// emit territory (live global-set reads + cross-boundary writes), NOT a numeric core.
//
// The ONE embedded, pure, bit-matchable numeric core Lure depends on is its RADIUS GATE: a member is in
// range iff  dist²(member, origin) ≤ range².  The squared distance is computed by FUN_140397640 — a tiny
// (62-byte) PURE LEAF with its own stable entry point that reads only an entity's world position
// (GameObjectClass+0x78/+0x7c/+0x80 = x/y/z) and a 3-float query point, and is reused by range checks
// ACROSS many behaviors (not Lure-specific). Per the #7-part-2 banked lesson, we lift + oracle THIS
// primitive directly at its own entry rather than hooking mid-62b4c0 frame: cleaner trampoline and far
// broader coverage (every range check in the sim, not just Lure's).
//
// ── The primitive (verified bit-for-bit against the 0x397640 asm) ─────────────────────────────────────
//   dx = entity.x - q.x   dy = entity.y - q.y   dz = entity.z - q.z      // float32 subss, each
//   return (double)(dx*dx) + (double)(dy*dy) + (double)(dz*dz)            // see grouping note below
//
// Asm (0x140397640):
//   movss xmm1,[rcx+0x7c] ; subss xmm1,[rdx+0x4]    // dy
//   movss xmm0,[rcx+0x78] ; subss xmm0,[rdx]        // dx
//   movss xmm2,[rcx+0x80] ; subss xmm2,[rdx+0x8]    // dz
//   mulss xmm1,xmm1 ; mulss xmm0,xmm0               // dy², dx²  (float32)
//   cvtps2pd xmm1,xmm1 ; cvtps2pd xmm0,xmm0         // promote dy², dx² to double
//   mulss xmm2,xmm2                                  // dz²  (float32)
//   addsd xmm0,xmm1                                  // dx² + dy²   (double)
//   cvtps2pd xmm1,xmm2                               // promote dz²
//   addsd xmm0,xmm1                                  // (dx²+dy²) + dz²   (double)  -> ret
//
// ── Bit-exactness notes (per the #8 part-2 FP-non-associativity lesson) ─────────────────────────────
//   * Each delta and each SQUARE is float32 (subss/mulss); only after squaring is each promoted to
//     double (cvtps2pd) — so the squares carry float32 rounding, the SUM carries double rounding.
//   * The sum is grouped left-associatively: ((dx² + dy²) + dz²). Source `a + b + c` matches.
//   * No FMA: the build is plain x86-64 (no -mfma/-march/-ffast-math), so dx*dx and the add stay
//     distinct mulss/addsd — consistent with every prior bit-exact lift (aim geometry, telekinesis).
#pragma once
#include "eaw_types.h"

namespace sim {

// FUN_140397640 — squared 3D distance between an entity's world position and a query point.
// `entity` is the GameObjectClass-layout position (+0x78/+0x7c/+0x80); `query` is the 3-float point the
// binary passes as `float* param_2`. Returns the squared distance in DOUBLE precision (the squares are
// float32, promoted then summed in double), exactly as the binary does — so a caller can compare it
// against a (double)(range*range) gate bit-for-bit.
eaw::f64 geom_dist_sq(const eaw::vec3& entity, const eaw::vec3& query);

// Lure's actual radius predicate (0x62b4c0 line: `dVar13 <= (double)(fVar1 * fVar1)`): a member entity is
// in range iff its squared distance to the origin is ≤ range². `range` is the ability-def field at
// AbilityDef+0x20; the binary squares it in float32 then promotes to double for the compare. Returns the
// same boolean the gate yields. (Documented + host-tested; the bit-exact in-game oracle validates the
// underlying geom_dist_sq, which is the only varying numeric input.)
bool geom_within_range(const eaw::vec3& entity, const eaw::vec3& origin, eaw::f32 range);

} // namespace sim
