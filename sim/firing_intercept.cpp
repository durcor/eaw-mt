// sim/firing_intercept.cpp — see firing_intercept.h. Literal transcription of FUN_140393b70 (simple mode)
// and FUN_140399e20, operand order/grouping preserved bit-for-bit; global scratch localized to returns.
#include "firing_intercept.h"
#include <cmath>

namespace sim {

using eaw::f32;
using eaw::vec3;

// FUN_140393b70, DAT_140b2c37c == 0 path:
//   _DAT_140b2ec18 = (tgt_vel.x - frame_vel.x) * (g*t) + tgt_pos.x   (and y/z at +1c/+20)
// where frame_vel = ref+{0xc,0x1c,0x2c}. The binary writes 3 globals and returns &them; we return a vec3.
vec3 firing_predict_pos_linear(const vec3& tgt_pos, const vec3& tgt_vel,
                               const vec3& frame_vel, f32 gamespeed, f32 t) {
    const f32 f = gamespeed * t;                       // fVar11 = (float)DAT_140b0a340 * param_2
    vec3 r;
    r.x = (tgt_vel.x - frame_vel.x) * f + tgt_pos.x;
    r.y = (tgt_vel.y - frame_vel.y) * f + tgt_pos.y;
    r.z = (tgt_vel.z - frame_vel.z) * f + tgt_pos.z;
    return r;
}

vec3 firing_intercept_lead(const vec3& tgt_pos, const vec3& tgt_vel, const vec3& frame_vel,
                           const vec3& muzzle, const vec3& shooter_ref, f32 gamespeed,
                           f32 proj_speed, bool* out_has_solution) {
    if (out_has_solution) *out_has_solution = false;

    const f32 g = gamespeed;                           // fVar5 = (float)DAT_140b0a340

    // rel = muzzle - tgt_pos   (fVar9/fVar11/fVar10 = x/y/z)
    const f32 rel_x = muzzle.x - tgt_pos.x;
    const f32 rel_y = muzzle.y - tgt_pos.y;
    const f32 rel_z = muzzle.z - tgt_pos.z;

    // sep = muzzle - shooter_ref   (fVar12/fVar6/fVar7 = x/y/z)
    const f32 sx = muzzle.x - shooter_ref.x;
    const f32 sy = muzzle.y - shooter_ref.y;
    const f32 sz = muzzle.z - shooter_ref.z;

    // relvel = (tgt_vel - frame_vel) * g   (binary order: y then x then z → fVar8/fVar3/fVar4)
    const f32 rvy = (tgt_vel.y - frame_vel.y) * g;     // fVar8
    const f32 rvx = (tgt_vel.x - frame_vel.x) * g;     // fVar3
    const f32 rvz = (tgt_vel.z - frame_vel.z) * g;     // fVar4

    // a = |relvel|² - (g·speed)²   — grouped exactly as the binary
    const f32 a = (rvy * rvy + rvx * rvx + rvz * rvz) - (((g * proj_speed) * g) * proj_speed);
    // b = (relvel·sep) * 2.0   — sum order y,x,z (middle term written sep.x*relvel.x)
    const f32 b = (rvy * sy + sx * rvx + rvz * sz) * 2.0f;
    // c = |sep|²
    const f32 c = sx * sx + sy * sy + sz * sz;

    // discriminant = b² - 4·a·c   (grouped (b*b) - ((a*4)*c))
    const f32 disc = b * b - a * 4.0f * c;

    f32 t = 0.0f;
    bool solved = false;
    if (0.0f <= disc) {
        if (a == 0.0f) {
            // linear: t = -c / b   (used iff b != 0 and t >= 0)
            if (b != 0.0f) {
                t = (-c) / b;
                if (0.0f <= t) solved = true;
            }
        } else {
            const f32 root = std::sqrt(disc);          // FUN_140776d48 = sqrtf
            f32 t1 = (root - b) / (a * 2.0f);          // fVar3
            f32 t2 = ((-b) - root) / (a * 2.0f);       // fVar5
            // pick the smallest strictly-positive root (binary's branch ladder)
            if (0.0f < t1) {
                if (0.0f < t2) {
                    if (t2 <= t1) t1 = t2;
                    t = t1; solved = true;
                } else {
                    t = t1; solved = true;             // only t1 > 0
                }
            } else if (0.0f < t2) {
                t = t2; solved = true;                 // only t2 > 0
            }
        }
    }

    if (!solved) return vec3{0.0f, 0.0f, 0.0f};

    // predicted impact position at time t, then aim = rel + predicted (componentwise).
    const vec3 pred = firing_predict_pos_linear(tgt_pos, tgt_vel, frame_vel, g, t);
    if (out_has_solution) *out_has_solution = true;
    return vec3{rel_x + pred.x, rel_y + pred.y, rel_z + pred.z};
}

// FUN_140381dc0 — apply weapon dispersion. Draws (when it perturbs) go x,y,z, each range_f(-m, m); the
// binary draws all three then assigns z,y,x, but draw order is x,y,z either way (draw 1 → x …).
vec3 firing_apply_spread(const vec3& dir, bool no_spread, f32 base_spread,
                         f32 sec_spread, f32 dist, f32 norm, eaw::SimRng& rng) {
    if (no_spread) return dir;                                  // DAT_140b3934d == 1

    if (base_spread > 0.0f) {                                   // primary spread path (3 draws)
        const f32 m = base_spread;
        vec3 o;
        o.x = dir.x + rng.range_f(-m, m);
        o.y = dir.y + rng.range_f(-m, m);
        o.z = dir.z + rng.range_f(-m, m);
        return o;
    }

    // base_spread <= 0: secondary (distance-scaled) spread path.
    if (sec_spread > 0.0f && dist > 0.0f) {
        if (norm <= 0.0f) return dir;                          // FUN_1403857d0 <= 0 → no spread
        const f32 m = (sec_spread * dist) / norm;
        vec3 o;
        o.x = dir.x + rng.range_f(-m, m);
        o.y = dir.y + rng.range_f(-m, m);
        o.z = dir.z + rng.range_f(-m, m);
        return o;
    }
    return dir;                                                 // no applicable spread
}

// FUN_140405870 (UnitAIBehaviorClass) — see header. Binary:
//   list = *(behavior+0x28)+0x110;
//   if (list != 0 && 0 < *(int*)(list+0x18))
//       return *(void**)( *(void**)(list+0x10) + FUN_1401ffb40(&DAT_140a13e24, 0, *(int*)(list+0x18)-1)*8 );
//   return 0;
// The list (elem-array + count) is hoisted to inputs; the global LCG draw becomes a substream range_i.
void* firing_pick_random_target(void* const* candidates, int count, eaw::SimRng& rng) {
    if (candidates == nullptr || count <= 0) return nullptr;     // binary: lVar1 != 0 && 0 < count
    const int idx = rng.range_i(0, count - 1);                  // FUN_1401ffb40(&word, 0, count-1)
    return candidates[idx];                                      // *(list[0x10] + idx*8)
}

} // namespace sim
