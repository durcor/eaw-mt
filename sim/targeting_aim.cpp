// sim/targeting_aim.cpp — implementation of the Targeting team-targeting aim-geometry core.
// See targeting_aim.h for the derivation and the bit-exactness notes.
#include "targeting_aim.h"
#include <cmath>
#include <cstring>

namespace sim {

namespace {
// Negate a float by flipping its sign bit (the binary does `^ DAT_140800860` = `^ 0x80000000`, which
// differs from unary minus for NaN payloads — we replicate the bit op exactly).
inline eaw::f32 flip_sign(eaw::f32 v) {
    eaw::u32 b;
    std::memcpy(&b, &v, sizeof b);
    b ^= 0x80000000u;
    eaw::f32 r;
    std::memcpy(&r, &b, sizeof r);
    return r;
}

// Small-angle trig guard (633ae0 / 63f730): call real trig only when the magnitude's exponent field
// clears 0x1d000000; below that, sin(x) -> x and cos(x) -> 1.0.
inline bool trig_guard(eaw::f32 x) {
    eaw::u32 b;
    std::memcpy(&b, &x, sizeof b);
    return (b & 0x7f800000u) > 0x1d000000u;
}

// libm wrappers (so host overloads can pass stable function pointers without taking &libm-fn).
eaw::f32 host_atan2(eaw::f32 y, eaw::f32 x) { return std::atan2(y, x); }
eaw::f32 host_sqrt (eaw::f32 s)             { return std::sqrt(s); }
eaw::f32 host_sin  (eaw::f32 x)             { return std::sin(x); }
eaw::f32 host_cos  (eaw::f32 x)             { return std::cos(x); }
} // namespace

AimAngles targeting_aim_angles(const eaw::vec3& self, const eaw::vec3& tgt,
                               Atan2Fn atan2_fn, SqrtFn sqrt_fn) {
    const eaw::f32 dx = tgt.x - self.x;
    const eaw::f32 dy = tgt.y - self.y;
    const eaw::f32 dz = tgt.z - self.z;

    // rad2deg computed at runtime with the binary's grouping: 360.0 / (PI * 2.0). Reused for yaw+pitch.
    const eaw::f32 rad2deg = kAimFullCircle / (kAimPi * 2.0f);

    AimAngles a;
    a.roll = 0.0f;  // param_1[0] = 0

    // yaw (param_1[2]): 0 when the horizontal delta vanishes, else atan2(dy,dx) -> deg -> [0,360).
    if (dx == 0.0f && dy == 0.0f) {
        a.yaw = 0.0f;
    } else {
        eaw::f32 yaw = atan2_fn(dy, dx) * rad2deg;
        if (yaw < 0.0f) yaw += kAimFullCircle;
        a.yaw = yaw;
    }

    // pitch (param_1[1]): 0 when dz==0 && dx==0, else -(atan2(dz, sqrt(dy*dy+dx*dx)) * rad2deg).
    if (dz == 0.0f && dx == 0.0f) {
        a.pitch = 0.0f;
    } else {
        const eaw::f32 horiz = sqrt_fn(dy * dy + dx * dx);   // dy^2 first (matches mulss/mulss/addss)
        const eaw::f32 pitch = atan2_fn(dz, horiz) * rad2deg;
        a.pitch = flip_sign(pitch);                          // ^ 0x80000000
    }
    return a;
}

eaw::f32 targeting_wrap_180(eaw::f32 deg) {
    while (kAimWrapHi < deg)  deg -= kAimFullCircle;   // while (180 < d)  d -= 360
    while (deg <= kAimWrapLo) deg += kAimFullCircle;   // while (d <= -180) d += 360
    return deg;
}

Facing targeting_face_from_yaw(eaw::f32 yaw_deg, SinFn sin_fn, CosFn cos_fn) {
    // deg2rad computed with 633ae0's grouping: (PI * 2.0) / 360.0  (reciprocal grouping of rad2deg).
    const eaw::f32 deg2rad = (kAimPi * 2.0f) / kAimFullCircle;
    const eaw::f32 turn    = deg2rad * yaw_deg;
    const bool     g       = trig_guard(turn);

    Facing f;
    f.x = g ? cos_fn(turn) : 1.0f;    // local_e4: cos, or small-angle 1.0
    f.y = g ? sin_fn(turn) : turn;    // local_e0: sin, or small-angle turn
    f.z = 0.0f;                        // local_dc
    return f;
}

AimAngles targeting_aim_angles(const eaw::vec3& self, const eaw::vec3& tgt) {
    return targeting_aim_angles(self, tgt, &host_atan2, &host_sqrt);
}

Facing targeting_face_from_yaw(eaw::f32 yaw_deg) {
    return targeting_face_from_yaw(yaw_deg, &host_sin, &host_cos);
}

} // namespace sim
