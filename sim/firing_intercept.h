// sim/firing_intercept.h — (b) firing-body lift, b2 piece 1: the projectile-intercept lead solver.
//
// The fire-control body FUN_1403825b0 (the §0 blocker) computes WHERE to aim a projectile so it hits a
// moving target. Per the b1 read-set classification (firing_body_lift_scope.md §7), the aim/intercept
// GEOMETRY is the clean, snapshot-able half: it reads only a flat target slice (pos/vel) + a reference-
// frame velocity, with no vfunc dispatch and no cross-entity write. This file lifts the matrix-free core
// of that half:
//
//   firing_predict_pos_linear  = FUN_140393b70 SIMPLE mode (DAT_140b2c37c == 0): linear extrapolation of
//                                the target's position at a future time t.  (The complex/curved mode —
//                                DAT_140b2c37c != 0, an iterative matrix integration via FUN_14022d390 —
//                                is a LATER b2 piece, alongside the circumcenter FUN_14038ee10.)
//   firing_intercept_lead      = FUN_140399e20: the quadratic projectile-intercept solver (the fallback /
//                                direct-aim path of the full lead solver FUN_140399450). Solves for the
//                                earliest positive impact time, then leads the aim to the predicted spot.
//
// SNAPSHOT-READY: every cross-object read is hoisted to an explicit `vec3` parameter, so a Phase-A worker
// feeds these from `FrozenSnapshot` (no live pointer chasing). The reference-frame velocity `frame_vel`
// is the engine's `ref + {0xc,0x1c,0x2c}` where `ref = (tgt+0xa8 ? *(tgt+0xa8)+0x164 : tgt+0x248)` —
// resolved by the caller/snapshot, passed in here.
//
// THREADING: the binary writes its result through GLOBAL scratch (FUN_140393b70 → DAT_140b2ec18..30;
// FUN_140399e20 has none). This lift returns by value — the global scratch is LOCALIZED, removing that
// parallel hazard (one of the three the b1 scope flagged).
//
// ── FP fidelity (per the #8-part-2 / DTARG2 banked non-associativity lessons) ─────────────────────────
// Every sum/product below preserves the binary's operand ORDER and grouping. Notable groupings from the
// 0x399e20 / 0x393b70 decompile:
//   a (quadratic 'A')   = (rvy*rvy + rvx*rvx + rvz*rvz) - (((g*speed)*g)*speed)
//   b (quadratic 'B')   = (rvy*sy + sx*rvx + rvz*sz) * 2.0
//   c (quadratic 'C')   = (sx*sx + sy*sy + sz*sz)
//   disc                = (b*b) - ((a*4.0)*c)
//   t1 = (sqrt(disc) - b) / (a*2.0)      t2 = ((-b) - sqrt(disc)) / (a*2.0)      // pick smallest > 0
//   linear (a==0): t    = (-c) / b   (used iff b != 0 and t >= 0)
//   predict_pos.k       = (tgt_vel.k - frame_vel.k) * (g*t) + tgt_pos.k    // k = x,y,z
// Constants (resolved via the targeting_aim notes): DAT_1408007d4 = 2.0, DAT_1408007e0 = 4.0,
// DAT_140800860 = sign-bit mask (≡ IEEE unary minus), DAT_140b0a340 = the game-speed scalar (`gamespeed`).
#pragma once
#include "eaw_types.h"
#include "sim_rng.h"

namespace sim {

// FUN_140393b70 (simple/linear mode): predict the target's world position at a future time `t` seconds.
//   ref-relative velocity (tgt_vel - frame_vel) is extrapolated over (gamespeed * t), added to tgt_pos.
eaw::vec3 firing_predict_pos_linear(const eaw::vec3& tgt_pos, const eaw::vec3& tgt_vel,
                                    const eaw::vec3& frame_vel, eaw::f32 gamespeed, eaw::f32 t);

// FUN_140399e20: quadratic projectile-intercept lead solver. Returns the lead AIM POINT in the binary's
// convention: (muzzle - tgt_pos) + predicted_impact_pos. Returns {0,0,0} when there is no real positive-
// time solution (the binary's sentinel; the caller fires only if the result is non-zero). If
// `out_has_solution` is non-null it is set to whether a solution was found.
//   tgt_pos/tgt_vel : target world position / velocity (snapshot)
//   frame_vel       : reference-frame velocity ref+{0xc,0x1c,0x2c} (snapshot)
//   muzzle          : the firing point (param_4)
//   shooter_ref     : the relative-velocity reference point (param_5)
//   gamespeed       : DAT_140b0a340 game-speed scalar
//   proj_speed      : projectile muzzle speed (param_6)
eaw::vec3 firing_intercept_lead(const eaw::vec3& tgt_pos, const eaw::vec3& tgt_vel,
                                const eaw::vec3& frame_vel, const eaw::vec3& muzzle,
                                const eaw::vec3& shooter_ref, eaw::f32 gamespeed,
                                eaw::f32 proj_speed, bool* out_has_solution = nullptr);

// FUN_140381dc0: apply weapon DISPERSION (spread) to the already-aimed direction `dir`, returning the
// perturbed direction. The spread magnitudes are resolved from weapon stats by the caller (ENV: the
// binary's FUN_140374890 base, FUN_14053ff30 secondary, FUN_1403857d0 normalizer); this lift takes them
// as inputs so the body is pure + snapshot-ready. Draw policy, faithful to the binary's branch ladder:
//   • no_spread (the global accuracy-off flag DAT_140b3934d == 1)  → dir unchanged, NO draws.
//   • base_spread > 0                                              → dir + range_f(-base, base) per axis.
//   • base_spread <= 0 and sec_spread > 0 and dist > 0:
//         norm <= 0   → dir unchanged, NO draws.
//         norm  > 0   → m = (sec_spread * dist) / norm;  dir + range_f(-m, m) per axis.
//   • otherwise                                                    → dir unchanged, NO draws.
// When it perturbs, it draws EXACTLY 3 times in x,y,z order from `rng` (mirrors the binary's three
// FUN_1401ffbb0 calls). RNG NOTE: `rng` is a per-entity substream under the I2 retrofit, so the emitted
// numbers DIFFER from the global-LCG binary BY DESIGN — what matches is the branch taken, the draw COUNT
// (0 or 3), and the per-draw formula (SimRng::range_f == FUN_1401ffbb0). This is a determinism retrofit,
// not a bit-exact lift (sim_rng.h, sim_parallel_command_schema.md §4).
eaw::vec3 firing_apply_spread(const eaw::vec3& dir, bool no_spread, eaw::f32 base_spread,
                              eaw::f32 sec_spread, eaw::f32 dist, eaw::f32 norm, eaw::SimRng& rng);

// FUN_140405870 (UnitAIBehaviorClass): resolve the fire target when the firing body has none explicit —
// pick a UNIFORM random entry from the firer's candidate-target list. This is the §0-blocker leaf the
// fire body calls at 3825b0:169 (`param_2 = FUN_140405870(uVar8)`, the param_2 / target resolution; the
// `g_pf_reimpl_p2` "post-405870 redirect" in the proxy). The binary reads the list at
// *(behavior+0x28)+0x110 (element-array ptr at +0x10, count at +0x18) and draws the index from the
// GLOBAL LCG word via FUN_1401ffb40(&DAT_140a13e24, 0, count-1). This lift hoists the list to explicit
// (candidates, count) inputs (SNAPSHOT-READY: no live pointer chasing) and draws from a per-entity
// SUBSTREAM, so it carries NO opaque read of the fixed global word — exactly what §8.45 requires to make
// the fire path thread-safe host code.
//
// Faithful to the binary's branch ladder:
//   • candidates == null OR count <= 0  → return nullptr (the binary's `return 0`), NO draw.
//   • count == 1                        → range_i(0,0) returns 0 with NO draw (FUN_1401ffb40 a==b early-
//                                         out) → candidates[0].
//   • count >= 2                        → ONE draw: idx = range_i(0, count-1) → candidates[idx].
// RNG NOTE: per-entity substream ⇒ the chosen INDEX differs from the global-LCG binary BY DESIGN (a
// determinism RETROFIT, not a bit-exact lift, per sim_rng.h / sim_parallel_command_schema.md §4). What is
// preserved is the draw COUNT (0 for count<=1, else 1) and the uniform-over-[0,count-1] selection policy.
void* firing_pick_random_target(void* const* candidates, int count, eaw::SimRng& rng);

} // namespace sim
