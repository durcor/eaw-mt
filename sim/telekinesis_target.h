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

// ── Mode-2 (HOLD) core — FUN_14063f730 (decomp/63f730.c) ────────────────────────────────────────────
// HOLD is reached after GRAB completes (slot+0x8 := 2). Each tick it does two independent things:
//   (A) POSE: spin the entity's orientation in a circle + bob its z-position vertically, both driven by
//       a single phase angle = spin_t * omega, with the orientation radius ramped in over a short window.
//   (B) DAMAGE: a self-scheduled periodic damage clock — when the sim time reaches the next-damage
//       deadline (slot+0x48), apply slot+0x40 damage to SELF and advance the deadline by slot+0x44.
//
// Only the deterministic numeric CORE is lifted here (mirroring the part-1 GRAB scope): the timeline
// scalars, the phase angle, the small-angle trig guard *predicate*, the z-bob composition, and the
// damage scheduler (due-predicate + deadline rebase). The trig *value* (engine FUN_140776150 cos /
// FUN_140776650 sin) and the actual damage application (FUN_1403a9e30) + event dispatch (FUN_1402d5320)
// are the ENV seam, supplied/observed externally — host tests feed libm trig, the in-process oracle
// calls the engine's own trig for bit-exactness.
//
//   elapsed  = now - start                                   // start = slot+0x24 (rebased on GRAB-complete)
//   spin_t   = max(elapsed / spin_period, 0.0)               // spin_period = _DAT_140b15ac8; clamp-low
//   ramp     = (elapsed < ramp_dur) ? elapsed/ramp_dur : 1.0 // ramp_dur = DAT_140b15acc; clamps to 1.0
//   angle    = spin_t * omega                                // omega = DAT_140819b3c (angular frequency)
//   pose: orient_x = wrap360(slot+0x34 + cos(angle)*radius*ramp)   radius = DAT_140b15ad0
//         orient_y = wrap360(slot+0x38 + sin(angle)*radius*ramp)   (wrap360 = FUN_14020b710, mod 360.0)
//         orient_z = slot+0x3c (passthrough)
//         pos_x = slot+0xc, pos_y = slot+0x10
//         pos_z = slot+0x14 + sin(angle)*bob_amp + height_offset   bob_amp = _DAT_140b15ad4,
//                                                                  height_offset = DAT_140b15ac0
//   NOTE: the z-bob has NO ramp (unlike the orientation radius); height_offset is the SAME grip-height
//   constant DAT_140b15ac0 that GRAB adds in telekinesis_target(). pos_z lands in entity+0x80 — the SAME
//   observable the part-1 oracle validates for GRAB (FUN_1403a8f90 writes entity+0x78/0x7c/0x80).
constexpr float kTeleWrapMod = 360.0f;   // DAT_1408007f4 — FUN_14020b710 wrap modulus (orientation)

// elapsed since the (rebased) timeline start. 63f730.c line 27: (float)clock/hz - slot[0x24].
float telekinesis_hold_elapsed(float now, float start);

// spin_t = max(elapsed / spin_period, 0.0). 63f730.c lines 28-31 — clamp-low only (like the GRAB t).
float telekinesis_spin_t(float elapsed, float spin_period);

// ramp = (elapsed < ramp_dur) ? elapsed/ramp_dur : 1.0. 63f730.c lines 32-35 — a ramp-in to 1.0 that
// scales the orientation spin radius (NOT the z-bob). DAT_1407ffaf8 = 1.0 is the clamped ceiling.
float telekinesis_ramp(float elapsed, float ramp_dur);

// phase angle = spin_t * omega. 63f730.c line 42 (fVar4 = fVar6 * DAT_140819b3c).
float telekinesis_hold_angle(float spin_t, float omega);

// The engine guards every cos/sin behind a small-angle bypass: it calls the real trig ONLY when the
// argument's IEEE-754 exponent field exceeds 0x1d (i.e. (bits & 0x7f800000) > 0x1d000000); otherwise
// cos collapses to 1.0 and sin collapses to the angle itself (the small-angle limits). 63f730.c lines
// 37/44/52. Returns true iff the real trig routine should be called for this angle.
bool telekinesis_use_real_trig(float angle);

// pos_z = slot_z + (sin_term*bob_amp + height_offset). 63f730.c line 57. NB the parenthesization is
// load-bearing: the binary groups (sin*bob + offset) first, then adds slot_z (verified in the codegen,
// not the flattened decompile) — FP add is non-associative so the wrong grouping is off by a sub-ULP at
// the bob extrema. `sin_term` is sin(angle) when telekinesis_use_real_trig(angle), else the small-angle
// value `angle`. Pure arithmetic — the bit-exact quantity written to entity+0x80; the caller supplies
// sin_term (engine trig / libm / small-angle).
float telekinesis_hold_pos_z(float slot_z, float sin_term, float bob_amp, float height_offset);

// ── HOLD damage scheduler (the self-clocked periodic-damage seam) — 63f730.c lines 59-73 ───────────
// The damage is applied to SELF (the gripped entity, param_2) — like AsteroidFieldDamage #6 the gameplay
// write is self-directed; the cross-boundary part is only the event dispatch FUN_1402d5320 (deferred).
// The cadence is a pure deterministic clock: fire when sim time has reached the deadline, then push the
// deadline forward by one interval. A zero deadline means "no damage scheduled" (GRAB only arms it when
// the per-tick damage amount slot+0x40 > 0), so it is explicitly excluded.
//
// due iff (next_deadline <= now) && (next_deadline != 0.0).   63f730.c line 59.
bool telekinesis_damage_due(float next_deadline, float now);

// on fire: next_deadline := interval + next_deadline. 63f730.c line 63 (slot[0x48] = slot[0x44]+slot[0x48]).
// Op order preserved (interval first). interval = slot+0x44, amount (applied, not computed here) = slot+0x40.
float telekinesis_damage_rebase(float next_deadline, float interval);

} // namespace sim
