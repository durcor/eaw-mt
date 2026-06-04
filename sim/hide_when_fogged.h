// sim/hide_when_fogged.h — Phase-3 OUT/presentation behavior: HideWhenFoggedBehaviorClass::vfunc_6.
//
// Lifted from RVA 0x53ddc0 (decomp/53ddc0.c). The SECOND of the two OUT (presentation) behaviors in
// the per-entity tick (the other is Select 0x3c2310). HideWhenFogged drives a unit's FOG/DETECTION
// ALPHA-FADE — the smooth fade-in/fade-out as a unit enters or leaves the player's sensor coverage /
// fog-of-war — and toggles the unit's render visibility + fires reveal/hide notifications as the fade
// crosses its show/hide thresholds. With this, the OUT presentation slice is closed (2/2).
//
// ── What the behavior does each tick (53ddc0) ──────────────────────────────────────────────────────
//   Modelled here as env (not lifted), pure PRESENTATION orchestration:
//     * LocomotorCommonClass::vfunc_6() — chains the base-class per-tick first.
//     * An update-coalescing THROTTLE (lines 27-34): a 30-tick timer struct at param_1+0x58
//       (FUN_1404ad350 = "has interval elapsed?", FUN_1404ad380 = arm 0x1e ticks). When the fade is
//       essentially settled — fabs(equilibrium - value) < DAT_140815de4 (FUN_1407760bc = fabs) — and
//       the timer has NOT elapsed, the whole update is skipped; otherwise the timer re-arms and the
//       full recompute runs. (A presentation optimisation; the spring math below is unaffected.)
//     * VISIBILITY DECISION (lines 35-67): queries fog-of-war / detection state against the shroud
//       manager DAT_140b15418 (FUN_14039b950 / FUN_14035f470 / FUN_14039a450 / FUN_14039b140 and the
//       per-player flags DAT_140a284c4 / DAT_140b1574e) to pick the fade TARGET equilibrium
//       (param_1+0x44) = 0.0 (hidden) or 1.0 / 0x3f800000 (visible), and latches a visibility byte at
//       param_1+0x54. On a visibility-state EDGE it notifies the shroud manager
//       (thunk_FUN_1404eb020 reveal / FUN_1404eacf0 hide).
//     * RENDER TOGGLE + reveal callbacks (lines 85-117): edge-detects the alpha crossing its show
//       threshold and calls FUN_140264c80(entity+0x2a0, visible) to flip the renderable, plus shroud
//       vtbl callbacks (+0x160 / +0x168) for actual reveal. param_1+0x55 caches "alpha > show-thresh".
//   None of that touches sim state; it reads fog/detection state and writes render/visibility objects.
//
// ── The deterministic numeric core (53ddc0.c lines 68-79) ──────────────────────────────────────────
//   The ONLY bit-matchable math is the SPRING — a semi-implicit damped harmonic oscillator that drives
//   the alpha scalar stored ON THE BEHAVIOR OBJECT (param_1) toward the visibility target:
//       value       = param_1 + 0x40   (current fade alpha, the spring output)
//       equilibrium = param_1 + 0x44   (fade target: 0 hidden / 1 visible — set by the env above)
//       velocity    = param_1 + 0x48   (spring velocity)
//       freq        = param_1 + 0x50   (natural-frequency config field; w = freq * 2.0)
//   This is the SAME engine shared spring as Nebula STAGE 1 (0x437b60) and Select (0x3c2310): objdump
//   of 0x53ddc0 shows the identical SSE sequence and the same five .rdata constants (freq-scale 2.0
//   DAT_1408007d4, dt DAT_140b0a344, 1.0 DAT_1407ffaf8, quad 0.48 DAT_140819c54, cubic 0.235
//   DAT_140819c50), and — critically — the same load-bearing denominator ADD-GROUPING
//   (quad + (wdt+1.0)) + cubic verified in the disassembly (addss order wdt+1.0 -> +quad -> +cubic ->
//   divss). State offsets are shifted -4 relative to Select (value+0x40 vs +0x44 etc.) but the relative
//   layout {value, eq, vel} contiguous with freq at value+0x10 is identical. So the core delegates to
//   the shared sim/damped_spring.{h,cpp} primitive — HideWhenFogged is its THIRD confirmed caller — and
//   its in-game bit-exactness is INHERITED from DTNEB (614503/614503 on this identical instruction
//   sequence), the same "shared primitive => inherited validation" pattern as Select<-Nebula and
//   #7's face_from_yaw<-#8's z-bob. The host damped_spring_test ground-truths the lift against an
//   inline-asm transcription of the exact SSE sequence across a wide freq range.
//
//   A note on the "instant-reveal" SNAP (lines 80-84): when the entity's shroud sub-object byte
//   (param_2->0x298 + 0x88) is set, the engine overrides the spring result with value=equilibrium,
//   velocity=0 (no fade — pop straight to the target). That is an env-gated state override, not spring
//   math; it is documented here and left to the caller/env, exactly as Select documents its env.
//
// ── Determinism / threading ────────────────────────────────────────────────────────────────────────
//   HideWhenFogged is PRESENTATION (Class 3, off-lockstep): the spring writes only the behavior
//   object's own alpha fields; the orchestration writes render/visibility/shroud-notify objects, never
//   authoritative sim state. No sim RNG, no GOM CreateObject, no cross-entity sim writes. It is fully
//   Phase-A-safe for the sim and can run on the render/presentation side; it need not participate in
//   the lockstep command drain. (equilibrium and freq are env inputs resolved from fog/detection state;
//   the core reproduces the per-tick integration given them, exactly as Nebula and Select do.)
#pragma once

#include "damped_spring.h"

namespace sim {

// The fog/detection alpha-fade spring state, on the behavior object (param_1). Offsets vs 53ddc0.c.
struct HideFogState {
    float value       = 0.0f;   // param_1+0x40 — current fade alpha (spring output)
    float equilibrium = 0.0f;   // param_1+0x44 — fade target 0/1 (set from fog/detection state; env)
    float velocity    = 0.0f;   // param_1+0x48 — spring velocity
    float freq        = 0.0f;   // param_1+0x50 — natural-frequency config (w = freq*2.0)
};

// The per-tick deterministic core: advance the fog-fade spring (53ddc0.c lines 68-79). Delegates to
// the shared damped-spring primitive (identical to Nebula's STAGE 1 and Select). `dt` = DAT_140b0a344,
// the per-tick game delta. Mutates st.value / st.velocity; reads st.equilibrium / st.freq.
void hide_when_fogged_spring_step(HideFogState& st, float dt);

} // namespace sim
