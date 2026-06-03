// sim/targeting_aim.h — Phase-3 IN sim behavior #7 (part 2): TargetingBehaviorClass team-targeting
// AIM GEOMETRY core.
//
// Targeting (#7) is the AI fire-control orchestrator. Part 1 lifted the thin game-mode DISPATCHER
// (0x633a30) and DEFERRED the two ~2-3 KB sub-bodies (FUN_140634810 mode 1 individual, FUN_140633ae0
// mode 2 team). Those sub-bodies are target-selection / fire-control orchestration FSMs — acquire ->
// validate (vfunc 0x160/0x168) -> assign (vfunc 0xd0) -> set-firing-state (vfunc 0xf8) -> face, plus
// GOM target-assign/move/fire emission. That orchestration IS the deferred Phase-B seam (UnitAI #4 /
// AsteroidFieldDamage #6 class: live global-set reads + cross-boundary emit). It is NOT a numeric core.
//
// The ONE embedded, pure, bit-matchable numeric core in the dominant in-combat path (633ae0 — the DTARG
// oracle showed 100% controller-sourced mode 2) is the AIM GEOMETRY: turning a self->target direction
// into the yaw/pitch the unit must face, and that yaw into a facing unit-vector. THIS unit lifts those
// three pure primitives; the selection/emit FSM around them stays deferred.
//
// ── The three primitives ────────────────────────────────────────────────────────────────────────────
//   (1) targeting_aim_angles  (FUN_14020acd0) — direction(self->tgt) -> Euler {roll=0, pitch, yaw} (deg).
//   (2) targeting_wrap_180     (FUN_14020b6d0) — wrap a heading delta (deg) into (-180, 180].
//   (3) targeting_face_from_yaw (633ae0 tail @0x634???..) — yaw(deg) -> facing unit {cos, sin, 0}.
//
// All three reuse the engine's OWN math intrinsics, called in-process for bit-exactness in the oracle:
//   FUN_14078437c = atan2(y, x),  FUN_140776d48 = sqrt(s),  FUN_140776650 = sin,  FUN_140776150 = cos.
// On host these are libm (atan2f/sqrtf/sinf/cosf); host tests therefore assert STRUCTURE + grouping +
// edge cases (not bit-exact-vs-binary — that is the DTARG2 in-game oracle's job, exactly as Telekinesis
// #8's host tests relate to its DTTK oracle).
//
// ── Constants (verified from the PE .rdata) ─────────────────────────────────────────────────────────
//   DAT_1408007dc = PI = 3.14159274101257324    DAT_1408007d4 = 2.0     DAT_1408007f4 = 360.0
//   DAT_1408524f8 = +180.0   DAT_1408524fc = -180.0   (wrap bounds)
//   DAT_140805898 = 10.0 (deg) = the heading-error threshold above which the body re-issues a face
//                                command (an ENV gate around the emit, NOT part of the geometry)
//   DAT_140800860 = 0x80000000 (float sign-bit mask, used to negate the pitch)
//
// ── Bit-exactness notes (per the #8 part-2 FP-non-associativity lesson) ─────────────────────────────
// The deg<->rad scale is computed at RUNTIME as separate float32 ops, NOT a precomputed constant, and
// the two directions use DIFFERENT groupings — both confirmed against the 14020acd0 / 633ae0 asm:
//   rad2deg = 360.0f / (PI * 2.0f)     // FUN_14020acd0: mulss(PI,2.0) then divss(360.0, that)
//   deg2rad = (PI * 2.0f) / 360.0f     // 633ae0 tail:   mulss(PI,2.0) then divss(that, 360.0)
//   horiz   = sqrt(dy*dy + dx*dx)      // dy^2 first, then dx^2, then add (mulss/mulss/addss order)
#pragma once
#include "eaw_types.h"

namespace sim {

// PI as the binary stores it (DAT_1408007dc). Exposed so the oracle and host share one literal.
constexpr eaw::f32 kAimPi          = 3.14159274101257324f;  // DAT_1408007dc
constexpr eaw::f32 kAimWrapHi      = 180.0f;                 // DAT_1408524f8
constexpr eaw::f32 kAimWrapLo      = -180.0f;                // DAT_1408524fc
constexpr eaw::f32 kAimFullCircle  = 360.0f;                 // DAT_1408007f4
constexpr eaw::f32 kAimTurnThreshDeg = 10.0f;               // DAT_140805898 (env emit gate, documented)

// Engine-intrinsic function-pointer types (atan2(y,x) and sqrt(s)). The oracle injects the engine's own
// FUN_14078437c / FUN_140776d48; host tests inject libm wrappers. Bit-exactness lives in the choice of
// these, not in this code.
using Atan2Fn = eaw::f32 (*)(eaw::f32 y, eaw::f32 x);
using SqrtFn  = eaw::f32 (*)(eaw::f32 s);
using SinFn   = eaw::f32 (*)(eaw::f32 x);
using CosFn   = eaw::f32 (*)(eaw::f32 x);

// Euler aim angles, in DEGREES. roll is always 0 (the binary writes param_1[0] = 0). Mirrors the 3-float
// struct FUN_14020acd0 fills: [0]=roll, [1]=pitch, [2]=yaw.
struct AimAngles {
    eaw::f32 roll  = 0.0f;
    eaw::f32 pitch = 0.0f;
    eaw::f32 yaw   = 0.0f;
};

// A facing unit-vector {x=cos, y=sin, z=0}, the layout 633ae0 passes to vfunc 0x218.
struct Facing {
    eaw::f32 x = 1.0f;
    eaw::f32 y = 0.0f;
    eaw::f32 z = 0.0f;
};

// (1) FUN_14020acd0 — direction (self -> tgt) to Euler {roll=0, pitch, yaw} in degrees.
//   delta = tgt - self
//   yaw   = (dx==0 && dy==0) ? 0 : normalize360( atan2(dy, dx) * rad2deg )   // [0,360)
//   pitch = (dz==0 && dx==0) ? 0 : -( atan2(dz, sqrt(dy*dy + dx*dx)) * rad2deg )
//   roll  = 0
// atan2/sqrt are injected (engine intrinsics in the oracle; libm on host).
AimAngles targeting_aim_angles(const eaw::vec3& self, const eaw::vec3& tgt,
                               Atan2Fn atan2_fn, SqrtFn sqrt_fn);

// (2) FUN_14020b6d0 — wrap a heading (deg) into (-180, 180]:  while(d>180) d-=360; while(d<=-180) d+=360.
eaw::f32 targeting_wrap_180(eaw::f32 deg);

// (3) 633ae0 tail — a (corrected) yaw in degrees to a facing unit-vector {cos, sin, 0}.
//   turn = deg2rad(yaw) = (PI*2.0)/360.0 * yaw
//   facing.x = guard ? cos(turn) : 1.0     // small-angle: cos -> 1.0
//   facing.y = guard ? sin(turn) : turn    // small-angle: sin -> turn
//   facing.z = 0
// guard = (bits(turn) & 0x7f800000) > 0x1d000000 — the SAME small-angle guard proven bit-exact by the
// Telekinesis #8 part-2 DTTK z-bob (same engine sin/cos). sin/cos are injected.
Facing targeting_face_from_yaw(eaw::f32 yaw_deg, SinFn sin_fn, CosFn cos_fn);

// libm convenience overloads (host).
AimAngles targeting_aim_angles(const eaw::vec3& self, const eaw::vec3& tgt);
Facing    targeting_face_from_yaw(eaw::f32 yaw_deg);

} // namespace sim
