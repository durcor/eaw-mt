// sim/spline_mover.h — Phase-3 IN sim behavior: SimpleSpaceLocomotorBehaviorClass MOVER CORE.
//
// SimpleSpaceLocomotor (0x626420) is the common space mover. Its vfunc_6 is a hyperspace/jump/death
// state-machine WRAPPER (deferred Phase-B seam: GOM/RNG/SFX/global-scratch per-state movers). The
// actual per-tick motion is computed by the mover virtual vfunc_59 (0x625990, 2354 B) — a
// **2D (XY-plane) cubic Hermite waypoint-path interpolator**. THIS unit lifts that interpolation core;
// the FSM wrapper, the waypoint-container access (FUN_14054fc00 / iterator FUN_1405c4920/5c5910), the
// orientation slew (FUN_140624c10, reads unit-template turn-rate at entity+0x298+0x394/+0x39c) and the
// state-5 spline variant (FUN_140627930) stay deferred as the engine I/O boundary — exactly the
// boundary pattern used by targeting_aim (#7 part 2): a pure geometry core inside an engine FSM.
//
// The same runtime Hermite basis matrix is shared by THREE locomotor families' vfunc_59
// (0x625990 SimpleSpace, 0x63c340, 0x641010) — confirmed via xrefs to DAT_140b2f1f0.
//
// ── The Hermite basis (verified from the PE) ──────────────────────────────────────────────────────
// The matrix lives in .data at DAT_140b2f1f0 (16 floats, runtime-initialized by FUN_14042b9f0). The
// initializer copies a *constraint* matrix M from .rdata and stores its INVERSE (FUN_1402324a0):
//     source M rows = [ f(0) ; f(1) ; f'(0) ; f'(1) ] of a cubic a+bt+ct^2+dt^3:
//         row0 = [1 0 0 0]   row1 = [1 1 1 1]   row2 = [0 1 0 0]   row3 = [0 1 2 3]
//     => runtime M^-1 = the standard Hermite basis (exact integer entries):
//         a-row = [ 1  0  0  0 ]   (a = p0)
//         b-row = [ 0  0  1  0 ]   (b = m0)
//         c-row = [-3  3 -2 -1 ]   (c = -3p0 + 3p1 - 2m0 - m1)
//         d-row = [ 2 -2  1  1 ]   (d =  2p0 - 2p1 +  m0 + m1)
// Integer entries are exact in float, so the coefficient step (multiply-by-{0,1,-1,2,3,-2} then add) is
// rounding-free regardless of grouping. The in-game DTVEL oracle re-confirms the live matrix bit-for-bit.
//
// ── Tangent scaling (vfunc_59) ────────────────────────────────────────────────────────────────────
//     m_i = normalize(heading_i) * speed_i * dT      // dT = t1 - t0  (the waypoint time gap)
// normalize() is FUN_140139800: len=sqrt(x^2+y^2+z^2); if len>0 scale by (1.0f/len), else leave as-is.
//
// ── Evaluation (asm-faithful grouping, per the #8 part-2 FP-non-associativity lesson) ───────────────
//     t  = clamp((time - t0) / (t1 - t0), 0, 1)        dT = t1 - t0 ;  t2 = t*t ; t3 = t*t2
//     pos.{x,y} = ((b*t + a) + c*t2) + d*t3            // left-assoc
//     pos.z     = prev_pos.z                           // Z is NOT splined — passed through
//     vel.{x,y} = ((2c*t + b) + 3d*t2) / dT
//     speed     = sqrt(vel.y*vel.y + vel.x*vel.x)      // NOTE: y^2 first, then x^2 (matches asm)
// Orientation (when speed > kSplineEps) = targeting_aim_angles of (vel.x, vel.y, 0); else keep previous.
#pragma once
#include "eaw_types.h"

namespace sim {

// The Hermite basis matrix M^-1, as the binary holds it at runtime (DAT_140b2f1f0..22c). Exact integers.
// Row-major: kHermite[coeff][input], coeff in {a,b,c,d}, input in {p0,p1,m0,m1}.
constexpr eaw::f32 kHermite[4][4] = {
    { 1.0f,  0.0f,  0.0f,  0.0f },  // a
    { 0.0f,  0.0f,  1.0f,  0.0f },  // b
    {-3.0f,  3.0f, -2.0f, -1.0f },  // c
    { 2.0f, -2.0f,  1.0f,  1.0f },  // d
};

// Near-zero velocity threshold (DAT_140815de4 = 9.99999975e-05). Below this the mover keeps the
// previous orientation rather than aiming along a degenerate velocity.
constexpr eaw::f32 kSplineEps = 9.99999975e-05f;  // DAT_140815de4

using SqrtFn = eaw::f32 (*)(eaw::f32 s);

// A path keyframe (waypoint). Mirrors the 0x34-byte stride record at order+0x18[i]:
//   pos     = [0x00..0x08]   heading = [0x0c..0x14]   speed = [0x18]   time = [0x30]
// (Only the fields the interpolation core consumes are modeled.)
struct Waypoint {
    eaw::vec3 pos;       // pfVar[0..2]
    eaw::vec3 heading;   // pfVar[3..5]   (un-normalized; normalized internally)
    eaw::f32  speed = 0; // pfVar[6]
    eaw::f32  time  = 0; // pfVar[0xc]
};

// Output of one interpolation step (the values vfunc_59 writes to param_7 / param_9).
struct SplineStep {
    eaw::vec3 pos;        // param_7  (pos.z = prev_pos.z)
    eaw::vec3 vel;        // (vx, vy, 0)  — the XY velocity used for aim
    eaw::f32  speed = 0;  // param_9  = |vel_xy|
    bool      moving = false;  // speed > kSplineEps  → orientation should be re-aimed
};

// normalize(v) in place semantics, returning length; matches FUN_140139800 (sqrt injected).
eaw::f32 spline_normalize(eaw::vec3& v, SqrtFn sqrt_fn);

// The tangent m = normalize(heading) * speed * dT.
eaw::vec3 spline_tangent(const eaw::vec3& heading, eaw::f32 speed, eaw::f32 dT, SqrtFn sqrt_fn);

// The 2D cubic-Hermite interpolation core (vfunc_59 "different-endpoints" branch). `time` is the
// current sim time (param_3, cast from uint to float by the caller). prev_pos supplies the pass-through Z.
SplineStep spline_step(const Waypoint& w0, const Waypoint& w1, eaw::f32 time,
                       const eaw::vec3& prev_pos, SqrtFn sqrt_fn);

// libm convenience overloads (host).
eaw::f32   spline_normalize(eaw::vec3& v);
eaw::vec3  spline_tangent(const eaw::vec3& heading, eaw::f32 speed, eaw::f32 dT);
SplineStep spline_step(const Waypoint& w0, const Waypoint& w1, eaw::f32 time, const eaw::vec3& prev_pos);

} // namespace sim
