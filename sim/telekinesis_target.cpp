// sim/telekinesis_target.cpp — TelekinesisTargetBehaviorClass::vfunc_6 (RVA 0x63f210) interp core.
// See telekinesis_target.h for the full identification and determinism notes.
#include "telekinesis_target.h"

#include <cstring>

namespace sim {

// now = (float)sim_clock / (float)hz. 63f210.c line 51: fVar5 = (float)DAT_140b0a320 / (float)DAT_140b0a340.
// sim_clock is a u32 tick counter cast to float; hz is the int32 sim-Hz (=30) cast to float.
float telekinesis_now(uint32_t sim_clock, int hz) {
    return (float)sim_clock / (float)hz;
}

// t = max((now - start)/duration, 0). 63f210.c lines 52-55:
//   fVar4 = (fVar5 - slot[0x24]) / _DAT_140b15ac4;  fVar6 = (0.0 <= fVar4) ? fVar4 : 0.0;
// Clamp-low only — there is no upper clamp here; the t >= 1.0 case is handled by the `complete` branch.
float telekinesis_t(float now, float start, float duration) {
    const float t_raw = (now - start) / duration;
    return (0.0f <= t_raw) ? t_raw : 0.0f;
}

TelekinesisTimeline telekinesis_timeline(uint32_t sim_clock, int hz, float start, float duration) {
    TelekinesisTimeline tl;
    tl.now      = telekinesis_now(sim_clock, hz);
    tl.t        = telekinesis_t(tl.now, start, duration);
    tl.complete = (kTeleComplete <= tl.t);   // 63f210.c line 57: if (DAT_1407ffaf8 <= fVar6)
    return tl;
}

// (to - from) * t + from. 63f210.c line 73: (local_80 - slot[0x30]) * fVar6 + slot[0x30].
// Op order preserved: the engine multiplies the delta by t then adds the base (one fmadd-shaped expr).
float telekinesis_lerp(float from, float to, float t) {
    return (to - from) * t + from;
}

// 63f210.c line 39: local_80 = DAT_140b15ac0 + slot[0x14].
float telekinesis_target(float angle_base, float slot_offset) {
    return angle_base + slot_offset;
}

// 63f210.c lines 40-50: each angle component starts at 360.0 (0x43b40000) and collapses to 0.0 when
// its stored value has decayed to <= 180.0 (DAT_1408524f8). A `<=` test, so exactly 180.0 collapses.
float telekinesis_angle_gate(float stored) {
    return (stored <= kTeleAngleThr) ? 0.0f : kTeleAngleFull;
}

// 63f210.c lines 32-91: slot null -> NoSlot (return 0x80650001); else read slot[0x8] and route
// 1 -> Grab (this file), 2 -> Hold (63f730, deferred), 3 -> Release (63f470, deferred), else Idle.
TelekinesisImpl telekinesis_dispatch(bool has_slot, int mode) {
    if (!has_slot) return TelekinesisImpl::NoSlot;
    if (mode == 1) return TelekinesisImpl::Grab;
    if (mode == 2) return TelekinesisImpl::Hold;
    if (mode == 3) return TelekinesisImpl::Release;
    return TelekinesisImpl::Idle;
}

// ── Mode-2 (HOLD) core — FUN_14063f730 ──────────────────────────────────────────────────────────────

// 63f730.c line 27: fVar5 = (float)DAT_140b0a320 / (float)DAT_140b0a340 - slot[0x24].
float telekinesis_hold_elapsed(float now, float start) {
    return now - start;
}

// 63f730.c lines 28-31: fVar6 = 0.0; if (0.0 <= fVar5/_DAT_140b15ac8) fVar6 = fVar5/_DAT_140b15ac8.
// Clamp-low only — the division is computed first, then the sign-gate keeps it or substitutes 0.0.
float telekinesis_spin_t(float elapsed, float spin_period) {
    const float s = elapsed / spin_period;
    return (0.0f <= s) ? s : 0.0f;
}

// 63f730.c lines 32-35: fVar7 = DAT_1407ffaf8 (1.0); if (fVar5 < DAT_140b15acc) fVar7 = fVar5/DAT_140b15acc.
// Below the ramp duration the factor grows linearly from 0; at/after it pins to 1.0. The comparison is on
// the raw elapsed seconds (not the clamped spin_t), so a future-dated start (elapsed<0) yields a negative
// ramp here — faithfully preserved (the engine does not clamp the ramp low).
float telekinesis_ramp(float elapsed, float ramp_dur) {
    return (elapsed < ramp_dur) ? (elapsed / ramp_dur) : kTeleComplete;
}

// 63f730.c line 42: fVar4 = fVar6 * DAT_140819b3c.
float telekinesis_hold_angle(float spin_t, float omega) {
    return spin_t * omega;
}

// 63f730.c lines 37/44/52: the real trig routine runs only when ((uint)angle & 0x7f800000) > 0x1d000000,
// i.e. the IEEE-754 biased exponent exceeds 0x3a. Reinterpret the float's bits and apply the same mask
// + compare exactly. (The sign bit is outside the 0x7f800000 mask, so this is magnitude-only.)
bool telekinesis_use_real_trig(float angle) {
    uint32_t bits;
    std::memcpy(&bits, &angle, sizeof(bits));
    return (bits & 0x7f800000u) > 0x1d000000u;
}

// 63f730.c line 57: local_50 = slot[0x14] + fVar6 * _DAT_140b15ad4 + DAT_140b15ac0.
// CAUTION — the decompiler flattens this to a left-assoc `slot_z + sin*bob + off`, but the ACTUAL codegen
// (FUN_14063f730 @0x63f8d3-0x63f905) groups it as slot_z + (sin*bob + off): it computes xmm6 = sin*bob,
// xmm6 += off, THEN xmm0(slot_z) += xmm6. FP add is non-associative, so the grouping matters — the wrong
// (left-assoc) grouping is off by a sub-ULP at large |sin*bob| (the engine's z-bob extrema). `sin_term` =
// sin(angle) when telekinesis_use_real_trig, else `angle`. Parenthesized to match the binary bit-for-bit.
float telekinesis_hold_pos_z(float slot_z, float sin_term, float bob_amp, float height_offset) {
    return slot_z + (sin_term * bob_amp + height_offset);
}

// 63f730.c line 59: if ((slot[0x48] <= (float)clock/hz) && (slot[0x48] != 0.0)).
bool telekinesis_damage_due(float next_deadline, float now) {
    return (next_deadline <= now) && (next_deadline != 0.0f);
}

// 63f730.c line 63: slot[0x48] = slot[0x44] + slot[0x48] — interval added on the LEFT, preserved.
float telekinesis_damage_rebase(float next_deadline, float interval) {
    return interval + next_deadline;
}

// ── Mode-3 (RELEASE) core — FUN_14063f470 ───────────────────────────────────────────────────────────

// 63f470.c line 91: local_60 = (slot[0x14] - slot[0x30]) * t + slot[0x30]. The "to" is the raw slot[0x14]
// — RELEASE does NOT add angle_base (contrast telekinesis_target). Pair with telekinesis_lerp(from,to,t).
float telekinesis_release_target(float slot_z) {
    return slot_z;
}

// 63f470.c lines 38-48: the completion teardown end-state. start := now (63f470.c:38), mode := 0
// (63f470.c:39 zeros the 8 bytes at slot+0x8, the low 4 of which are the mode), every other dynamic
// field 0 (so the damage amount/deadline are disarmed), and the lone sentinel slot+0x4c := 0x3fffff.
TelekinesisReleaseEnd telekinesis_release_end(float now) {
    TelekinesisReleaseEnd e;
    e.mode            = kTeleReleaseTermMode;   // 0
    e.sentinel        = kTeleReleaseSentinel;   // 0x3fffff
    e.start           = now;
    e.damage_disarmed = true;                   // slot+0x40 == slot+0x48 == 0
    return e;
}

} // namespace sim
