// sim/telekinesis_target.h — Phase-3 IN sim behavior #8: TelekinesisTargetBehaviorClass::vfunc_6.
//
// Lifted from RVA 0x63f210 (decomp/63f210.c). Eighth of the 13 in-slice IN sim behaviors
// (decomp_plan.md Phase-3). This is the engine's telekinesis "force-grip" effect — a Force user
// grabs a target, lifts/holds/spins it, then releases it. The behavior is a 3-MODE LIFECYCLE STATE
// MACHINE over a per-entity telekinesis slot at entity+0x160; the mode int lives at slot+0x8:
//
//   mode 1  GRAB     — interpolate the target's transform toward the grip pose over a fixed duration;
//                       on completion snap to final + ADVANCE the slot to mode 2.   (THIS FILE — lifted)
//   mode 2  HOLD     — FUN_14063f730: a spin-ramp that also emits PERIODIC DAMAGE + a GOM impact spawn.
//   mode 3  RELEASE  — FUN_14063f470: interpolate the transform back; on completion zero the slot +
//                       RE-ENABLE the unit's suppressed abilities + a GOM spawn.
//   else    IDLE     — no slot action (falls through, returns 0x650000).
//   slot==0          — no telekinesis active on this entity; returns the distinct NoOp 0x80650001.
//
// Modes 1 (GRAB) and 3 (RELEASE) share the SAME interp-timeline math; they differ only in what they
// write on completion (GRAB advances to HOLD; RELEASE tears the slot down + re-enables abilities).
// Mode 2 (HOLD) is a different body (spin + damage). Per the established incremental methodology
// (cf. Targeting #7 "dispatcher first"), this unit lifts the SHARED DETERMINISTIC INTERP-TIMELINE
// CORE + the mode dispatch, and DEFERS the mode-2/3 heavy side-effects (the damage tick, the GOM
// spawns, the ability re-enable) — those are the env/Phase-B seam, not numeric core.
//
// ── The interp timeline (shared by GRAB & RELEASE; 63f210.c lines 51-56, 63f470.c lines 28-34) ─────
//     now      = (float)sim_clock / (float)hz                  // seconds; sim_clock=DAT_140b0a320 (u32
//                                                              // tick counter), hz=DAT_140b0a340 (=30)
//     t_raw    = (now - start) / duration                      // start=slot+0x24, duration=_DAT_140b15ac4
//     t        = max(t_raw, 0.0)                               // clamp-low only (negative -> 0)
//     complete = (1.0 <= t)                                    // DAT_1407ffaf8 = 1.0
//   While !complete the transform is LERPed by t toward the grip pose; on `complete` the engine snaps
//   to the final pose, rebases start = now, and transitions the mode.
//
// ── Mode-1 (GRAB) specifics (63f210.c lines 39-84) ─────────────────────────────────────────────────
//   target value = DAT_140b15ac0 + slot+0x14                   // config angle base + per-grip offset
//   interp value = lerp(slot+0x30, target, t)                  // local_70 (the smoothly-driven scalar)
//   Two angle components are each GATED to a 360°/0° choice:
//       angle_out = (stored <= 180.0) ? 0.0 : 360.0            // 180.0 = DAT_1408524f8, 360.0 = 0x43b40000
//   gating slot+0x34 -> the first applied angle, slot+0x38 -> the second. On `complete` the gated
//   angles + the final pose are written back into the slot and the mode advances slot+0x8 := 2.
//   The interpolated pose is PUSHED to the entity via the two apply helpers (FUN_1403a8f90 set-position
//   entity+0x78, FUN_1403a8710 set-orientation entity+0x84/+0x90) and the vector angular-lerp helper
//   FUN_14063ea50 (per-component normalize FUN_14020b710 + scalar interp FUN_14063efd0). Those apply
//   helpers + the vector lerp are the ENV side (they mutate the entity transform + set its dirty bit);
//   the deterministic core here computes the timeline (t, complete), the target, the scalar lerp value,
//   and the angle gates — the bit-exact numeric quantities the in-game oracle validates.
//
// ── Determinism / threading notes ─────────────────────────────────────────────────────────────────
//   * The interp timeline is fully deterministic given (sim_clock, hz, start, duration). hz and the
//     two config scalars (angle base DAT_140b15ac0, duration _DAT_140b15ac4) are runtime-populated
//     globals the oracle recovers live (as EnergyPool/Nebula recovered theirs).
//   * Mode 1 (GRAB) writes only its OWN slot + its OWN entity transform -> Phase-A-safe. Mode 3
//     (RELEASE) likewise integrates own-state, but on COMPLETION re-enables abilities + spawns a GOM
//     effect, and Mode 2 (HOLD) emits damage + a GOM spawn every active window — those completion
//     side-effects are the cross-boundary Phase-B seam (cf. UnitAI #4 / AsteroidFieldDamage #6), which
//     is exactly why the mode-2/3 bodies are deferred and only the shared timeline core is lifted here.
#pragma once

#include <cstdint>

namespace sim {

// Which sub-implementation the slot's mode int routes to (63f210.c lines 32-91).
enum class TelekinesisImpl {
    NoSlot  = 0,   // entity+0x160 == 0   -> return 0x80650001 (no telekinesis active on this entity)
    Grab    = 1,   // slot+0x8 == 1       -> the mode-1 GRAB interp        (this file: lifted)
    Hold    = 2,   // slot+0x8 == 2       -> FUN_14063f730 (spin + damage)  (DEFERRED)
    Release = 3,   // slot+0x8 == 3       -> FUN_14063f470 (release interp)  (DEFERRED)
    Idle    = 4,   // any other mode      -> no slot action (return 0x650000)
};

// The interp timeline shared by GRAB (mode 1) and RELEASE (mode 3).
struct TelekinesisTimeline {
    float now      = 0.0f;   // (float)sim_clock / (float)hz  — current sim time in seconds
    float t        = 0.0f;   // max((now - start)/duration, 0) — clamped-low interp fraction
    bool  complete = false;  // 1.0 <= t — the snap-to-final / mode-transition edge
};

// Fixed constants recovered from the binary (decomp_plan.md Phase-3 #8):
constexpr float kTeleComplete   = 1.0f;     // DAT_1407ffaf8 — interp-complete threshold (t >= 1.0)
constexpr float kTeleAngleThr   = 180.0f;   // DAT_1408524f8 — angle gate: <= this collapses to 0
constexpr float kTeleAngleFull  = 360.0f;   // 0x43b40000    — the "full" angle applied otherwise

// now = (float)sim_clock / (float)hz. sim_clock = DAT_140b0a320 (u32 tick counter), hz = DAT_140b0a340.
float telekinesis_now(uint32_t sim_clock, int hz);

// t = max((now - start)/duration, 0). start = slot+0x24, duration = _DAT_140b15ac4 (runtime config).
float telekinesis_t(float now, float start, float duration);

// The full timeline: now, t, and the complete edge (1.0 <= t). 63f210.c lines 51-56.
TelekinesisTimeline telekinesis_timeline(uint32_t sim_clock, int hz, float start, float duration);

// The scalar lerp driving the interpolated value (mode1 local_70, mode3 local_60):
//   (to - from) * t + from    — op order preserved bit-for-bit (63f210.c line 73 / 63f470.c line 91).
float telekinesis_lerp(float from, float to, float t);

// Mode-1 grip-target value (63f210.c line 39): config angle base + the per-grip offset slot+0x14.
float telekinesis_target(float angle_base, float slot_offset);

// Mode-1 angle gate (63f210.c lines 40-50): 360.0 unless the stored angle has decayed to <= 180.0,
// in which case it collapses to 0.0. Applied independently to the two angle components.
float telekinesis_angle_gate(float stored);

// Slot-mode dispatch (63f210.c lines 32-91): no slot -> NoSlot; else route the mode int 1/2/3/else.
TelekinesisImpl telekinesis_dispatch(bool has_slot, int mode);

} // namespace sim
