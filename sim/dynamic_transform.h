// sim/dynamic_transform.h — (b) Pass-C body lift (b2′): DynamicTransformBehavior::vfunc FUN_1403ac530.
//
// Pass-C is the second per-object update pass of the GOM tick (I4: master-list 3a6b80 ∥ → barrier →
// second-list 3ac530 ∥ → deferred-create drain). DynamicTransform rebuilds the object's local-to-world
// 3×4 transform each tick from its STORED EULER POSE (degrees, GameObjectClass +0x90/+0x94/+0x98) and its
// world position (+0x78/+0x7c/+0x80). It is a pure SELF body — no cross-object read or write of sim state;
// its only cross-boundary outputs are render-node updates (Class-3 presentation, off-lockstep) and an
// own-child notify, both ENV here. So it is directly Phase-A parallel (1 object = 1 work unit), the cheap
// per-tick mass the b1 scope flagged as parallelizable regardless of the firing-gate decision.
//
// The matrix is 3 rows × 4 cols, row-major (12 floats): [ R | t ], rotation in cols 0-2, translation in
// col 3. The binary composes the rotation onto a fresh identity via three Givens (2D) rotations + a fixed
// +90° roll, each angle = deg2rad·euler where deg2rad = (PI·2)/360 (DAT_1408007dc·DAT_1408007d4/…7f4):
//   1. cols(0,1) by euler_z (+0x98)     FUN_1400480f0
//   2. cols(0,2) by euler_y (+0x94)     inline in 3ac530
//   3. cols(1,2) by euler_x (+0x90)     FUN_1402cf8d0
//   4. cols(0,1) by 90° const           FUN_1400480f0   (DAT_1408007ec = 90.0)
// Translation (col 3) is set once from position and untouched by the rotations.
//
// sin/cos use the engine's small-angle guard (the targeting_aim / telekinesis pattern): for |angle| above
// ~2^-69 (exponent bits > 0x1d000000) call sin/cos (FUN_140776650 / FUN_140776150), else use (angle, 1.0).
// The sin/cos providers are injected so the host test uses libm and the in-game oracle uses the engine's
// own intrinsics for bit-exactness.
#pragma once
#include "eaw_types.h"

namespace sim {

using SinFn = eaw::f32 (*)(eaw::f32);   // FUN_140776650
using CosFn = eaw::f32 (*)(eaw::f32);   // FUN_140776150

// FUN_1400480f0: in-place Givens rotation mixing matrix columns (0,1) across all three rows.
void dyn_rot_cols01(eaw::f32 m[12], eaw::f32 angle, SinFn sinf, CosFn cosf);
// FUN_1402cf8d0: in-place Givens rotation mixing matrix columns (1,2).
void dyn_rot_cols12(eaw::f32 m[12], eaw::f32 angle, SinFn sinf, CosFn cosf);
// the inline 3ac530 Givens rotation mixing matrix columns (0,2).
void dyn_rot_cols20(eaw::f32 m[12], eaw::f32 angle, SinFn sinf, CosFn cosf);

// FUN_1403ac530 numeric core: rebuild the 3×4 transform `m` from the stored Euler pose (degrees) and
// world position. (The render-node update + child notify that follow in the binary are ENV / Class-3.)
void dynamic_transform_rebuild(eaw::f32 m[12], eaw::f32 euler_x_deg, eaw::f32 euler_y_deg,
                               eaw::f32 euler_z_deg, const eaw::vec3& pos, SinFn sinf, CosFn cosf);

} // namespace sim
