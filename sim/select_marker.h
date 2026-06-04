// sim/select_marker.h — Phase-3 OUT/presentation behavior: SelectBehaviorClass::vfunc_6.
//
// Lifted from RVA 0x3c2310 (decomp/3c2310.c). One of the two OUT (presentation) behaviors in the
// per-entity tick (the other is HideWhenFogged 0x53ddc0). Select drives the on-screen SELECTION
// MARKER under a unit — the ring / brackets / health-bar decals that appear when the unit is selected
// — and animates a "pulse" scalar that those decals read.
//
// ── What the behavior does each tick (3c2310) ──────────────────────────────────────────────────────
//   The marker's visual state lives in a select-state struct at entity+0xb0 holding up to five decal
//   objects (+0x10 label, +0x18/+0x28 two "primary" decals, +0x20/+0x30 two "secondary" decals). The
//   body is pure PRESENTATION orchestration, modelled here as env (not lifted):
//     * FUN_1403c2440 — lazily (re)build the unit's name label string (gated by the +0x40 latch).
//     * FUN_1403c19c0 — on first show, CREATE the decals (game-speed-mode-gated: mode 1 vs mode 2
//                       decal types via DAT_140b15418 vtbl +0xe0) and seed their own transforms.
//     * FUN_1403c1d80 — on hide (+0x40 set with +0x3a7 clear), DESTROY the decals.
//     * FUN_1403c2710 — every frame, PLACE the decals at the unit's world position (entity+0x78/
//                       +0x7c/+0x80) and set their visibility from fog/selection predicates
//                       (FUN_14039b020 etc.). The +0x3a7 entity byte is the "is-selected" gate.
//   None of that touches sim state; it reads the unit's transform and writes render/decal objects.
//
// ── The deterministic numeric core (3c2310.c lines 33-41) ──────────────────────────────────────────
//   The ONLY bit-matchable math is the tail SPRING — a semi-implicit damped harmonic oscillator that
//   animates a scalar stored ON THE BEHAVIOR OBJECT itself (param_1 = the SelectBehaviorClass
//   instance), the "selection pulse" the decals sample for their pulsing scale/alpha:
//       value       = param_1 + 0x44   (current pulse value, the spring output)
//       equilibrium = param_1 + 0x48   (target the pulse springs toward — set on select/deselect; env)
//       velocity    = param_1 + 0x4c   (spring velocity)
//       freq        = param_1 + 0x54   (natural-frequency config field; w = freq * 2.0)
//   This spring is INLINED byte-identically to NebulaBehaviorClass's STAGE-1 spring (RVA 0x437b60):
//   same SSE sequence, same four .rdata constants (2.0 / 0.48 / 0.235 / 1.0), same per-tick dt
//   (DAT_140b0a344). So the core delegates to the shared sim/damped_spring.{h,cpp} primitive, and its
//   in-game bit-exactness is INHERITED from DTNEB (which validated this identical instruction sequence
//   live at 614503/614503) — the same "shared primitive => inherited validation" pattern as #7's
//   targeting_face_from_yaw inheriting from #8's z-bob. The host damped_spring_test additionally
//   ground-truths the lift against an inline-asm transcription across Select's wider freq range.
//
// ── Determinism / threading ────────────────────────────────────────────────────────────────────────
//   Select is PRESENTATION (Class 3, off-lockstep): the spring writes only the behavior object's own
//   pulse fields; the orchestration writes render/decal objects, never sim state. No sim RNG, no GOM
//   CreateObject (the decals are presentation objects), no cross-entity sim writes. So it is fully
//   Phase-A-safe for the sim and need not participate in the lockstep command drain at all — it can
//   run on the render/presentation side. (equilibrium and freq are inputs set elsewhere on
//   select/deselect; the core reproduces the per-tick integration given them, exactly as Nebula's
//   core takes its env-resolved equilibrium.)
#pragma once

#include "damped_spring.h"

namespace sim {

// The selection-pulse spring state, on the behavior object (param_1). Offsets vs 3c2310.c.
struct SelectMarkerState {
    float value       = 0.0f;   // param_1+0x44 — current pulse value (spring output)
    float equilibrium = 0.0f;   // param_1+0x48 — pulse target (set on select/deselect; env input)
    float velocity    = 0.0f;   // param_1+0x4c — spring velocity
    float freq        = 0.0f;   // param_1+0x54 — natural-frequency config (w = freq*2.0)
};

// The per-tick deterministic core: advance the selection-pulse spring (3c2310.c lines 33-41).
// Delegates to the shared damped-spring primitive (identical to Nebula's STAGE 1). `dt` =
// DAT_140b0a344, the per-tick game delta. Mutates st.value / st.velocity; reads st.equilibrium/freq.
void select_marker_spring_step(SelectMarkerState& st, float dt);

} // namespace sim
