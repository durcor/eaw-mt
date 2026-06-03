// sim/telekinesis_target.cpp — TelekinesisTargetBehaviorClass::vfunc_6 (RVA 0x63f210) interp core.
// See telekinesis_target.h for the full identification and determinism notes.
#include "telekinesis_target.h"

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

} // namespace sim
