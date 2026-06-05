// sim/spline_mover.cpp — see spline_mover.h. Asm-faithful transcription of SimpleSpaceLocomotor
// vfunc_59 (0x625990) "different-endpoints" cubic-Hermite path interpolation core.
#include "spline_mover.h"
#include <cmath>

namespace sim {

// One Hermite coefficient row applied to the boundary vector [p0, p1, m0, m1].
// Grouping is left-assoc to mirror the binary's matrix-vector product (DAT_140b2f1f0 rows):
//   ((p0*M0 + p1*M1) + m0*M2) + m1*M3.
static inline eaw::f32 coeff(int row, eaw::f32 p0, eaw::f32 p1, eaw::f32 m0, eaw::f32 m1) {
    return ((p0 * kHermite[row][0] + p1 * kHermite[row][1]) + m0 * kHermite[row][2])
           + m1 * kHermite[row][3];
}

eaw::f32 spline_normalize(eaw::vec3& v, SqrtFn sqrt_fn) {
    // FUN_140139800: len from the FULL 3D vector; scale by 1.0/len iff len > 0.
    eaw::f32 len = sqrt_fn(v.x * v.x + v.y * v.y + v.z * v.z);
    if (0.0f < len) {
        eaw::f32 inv = 1.0f / len;
        v.x = v.x * inv;
        v.y = v.y * inv;
        v.z = v.z * inv;
    }
    return len;
}

eaw::vec3 spline_tangent(const eaw::vec3& heading, eaw::f32 speed, eaw::f32 dT, SqrtFn sqrt_fn) {
    // m = normalize(heading) * speed * dT, grouped (speed * nhead) * dT as in the asm.
    eaw::vec3 n = heading;
    spline_normalize(n, sqrt_fn);
    return eaw::vec3{ speed * n.x * dT, speed * n.y * dT, speed * n.z * dT };
}

SplineStep spline_step(const Waypoint& w0, const Waypoint& w1, eaw::f32 time,
                       const eaw::vec3& prev_pos, SqrtFn sqrt_fn) {
    SplineStep out{};
    eaw::f32 dT = w1.time - w0.time;          // local_104 = pfVar5[0xc] - pfVar4[0xc]

    // t = clamp((time - t0) / dT, 0, 1)  (the binary's fVar7 = 0; if(0<=u){=1; if(u<=1)=u;})
    eaw::f32 u = (time - w0.time) / dT;
    eaw::f32 t = 0.0f;
    if (0.0f <= u) { t = 1.0f; if (u <= 1.0f) t = u; }
    eaw::f32 t2 = t * t;
    eaw::f32 t3 = t * t2;

    // Tangents (scaled by dT). normalize() uses the full 3D heading length; only x/y feed the 2D spline.
    eaw::vec3 m0 = spline_tangent(w0.heading, w0.speed, dT, sqrt_fn);
    eaw::vec3 m1 = spline_tangent(w1.heading, w1.speed, dT, sqrt_fn);

    // Per-axis cubic coefficients via the Hermite basis.
    eaw::f32 ax = coeff(0, w0.pos.x, w1.pos.x, m0.x, m1.x);
    eaw::f32 bx = coeff(1, w0.pos.x, w1.pos.x, m0.x, m1.x);
    eaw::f32 cx = coeff(2, w0.pos.x, w1.pos.x, m0.x, m1.x);
    eaw::f32 dx = coeff(3, w0.pos.x, w1.pos.x, m0.x, m1.x);
    eaw::f32 ay = coeff(0, w0.pos.y, w1.pos.y, m0.y, m1.y);
    eaw::f32 by = coeff(1, w0.pos.y, w1.pos.y, m0.y, m1.y);
    eaw::f32 cy = coeff(2, w0.pos.y, w1.pos.y, m0.y, m1.y);
    eaw::f32 dy = coeff(3, w0.pos.y, w1.pos.y, m0.y, m1.y);

    // Position: pos = ((b*t + a) + c*t2) + d*t3.   Z is passed through from the previous position.
    out.pos.x = ((bx * t + ax) + cx * t2) + dx * t3;
    out.pos.y = ((by * t + ay) + cy * t2) + dy * t3;
    out.pos.z = prev_pos.z;

    // Velocity: ((2c*t + b) + 3d*t2) / dT.
    eaw::f32 vx = ((cx * 2.0f * t + bx) + dx * 3.0f * t2) / dT;
    eaw::f32 vy = ((cy * 2.0f * t + by) + dy * 3.0f * t2) / dT;
    out.vel = eaw::vec3{ vx, vy, 0.0f };

    // |vel_xy| with the asm's y^2-first ordering; the "moving" gate uses sqrt(vy^2+vx^2), the output
    // speed uses sqrt(vy^2+vx^2+0.0) (same value, mirrored separately for faithfulness).
    eaw::f32 mag   = sqrt_fn(vy * vy + vx * vx);
    out.moving     = !(mag <= kSplineEps);
    out.speed      = sqrt_fn(vy * vy + vx * vx + 0.0f);
    return out;
}

// ── libm host overloads ─────────────────────────────────────────────────────────────────────────────
static eaw::f32 host_sqrt(eaw::f32 s) { return std::sqrt(s); }

eaw::f32  spline_normalize(eaw::vec3& v) { return spline_normalize(v, host_sqrt); }
eaw::vec3 spline_tangent(const eaw::vec3& heading, eaw::f32 speed, eaw::f32 dT) {
    return spline_tangent(heading, speed, dT, host_sqrt);
}
SplineStep spline_step(const Waypoint& w0, const Waypoint& w1, eaw::f32 time, const eaw::vec3& prev_pos) {
    return spline_step(w0, w1, time, prev_pos, host_sqrt);
}

} // namespace sim
