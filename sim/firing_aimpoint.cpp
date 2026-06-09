// sim/firing_aimpoint.cpp — see firing_aimpoint.h. Faithful transcription of FUN_140383f70: the 3-phase
// aim-point ladder, with every skeleton/ENV read hoisted to input data and the one RNG seam (phase 2's
// random start) drawn from a per-entity substream. FP operand order/grouping preserved.
#include "firing_aimpoint.h"
#include <cmath>

namespace sim {

using eaw::f32;
using eaw::vec3;

// FUN_140383f70 muzzle midpoint. Single bone (line 83): muzzle = bone1. Two bones (lines 88-90 / 169-171
// / 217-219): muzzle.k = (bone2.k + bone1.k) * (DAT_1407ffaf8 / DAT_1408007d4). DAT_1407ffaf8 = 1.0,
// DAT_1408007d4 = 2.0 ⇒ scale = 0.5 (kept as the division form the binary recomputes).
vec3 firing_muzzle_midpoint(const vec3& bone1, bool has_bone2, const vec3& bone2) {
    if (!has_bone2) return bone1;
    const f32 s = 1.0f / 2.0f;                         // DAT_1407ffaf8 / DAT_1408007d4
    vec3 m;
    m.x = (bone2.x + bone1.x) * s;
    m.y = (bone2.y + bone1.y) * s;
    m.z = (bone2.z + bone1.z) * s;
    return m;
}

// FUN_140383f70 range test (lines 92-95 / 173-176 / 221-223): dist = sqrt(dx^2+dy^2+dz^2), dx=muzzle-point,
// accumulated x→y→z; the accept is dist <= range.
bool firing_point_in_range(const vec3& muzzle, const vec3& point, f32 range) {
    const f32 dx = muzzle.x - point.x;
    const f32 dy = muzzle.y - point.y;
    const f32 dz = muzzle.z - point.z;
    const f32 dist = std::sqrt(dx * dx + dy * dy + dz * dz);   // FUN_140776d48 = sqrtf
    return dist <= range;
}

FiringAimResult firing_resolve_aimpoint(const FiringAimInputs& in, eaw::SimRng& rng) {
    FiringAimResult r;
    r.aim    = in.target_center;     // phase-0 default (local_168); *param_4 = 0
    r.locked = false;

    // Phase 0 gate (lines 51-57): no target model / unresolved skeleton → bare center, unlocked.
    if (!in.have_target_model) return r;

    // Muzzle is invariant across candidates (same shooter bones); {0,0,0} when bone1 unresolved (the
    // binary's carry-from-local_res20=0 degenerate).
    const vec3 muzzle = in.muzzle_valid
        ? firing_muzzle_midpoint(in.muzzle_bone1, in.has_muzzle_bone2, in.muzzle_bone2)
        : vec3{};

    // Phases 1/2 add the target extent ONLY behind the template flag (lines 98-103 / 179-184).
    const f32 range12 = in.add_target_extent ? (in.weapon_range + in.target_extent) : in.weapon_range;

    // ── PHASE 1: target's designated aim-bones, in list order (lines 60-117) ──────────────────────────
    // Invalid bones are SKIPPED (the accept block is guarded by cVar2==1, line 76).
    for (int i = 0; i < in.aim_bone_count; ++i) {
        const AimCandidate& c = in.aim_bones[i];
        if (!c.valid) continue;
        if (firing_point_in_range(muzzle, c.pos, range12) && c.reachable) {
            r.aim = c.pos; r.locked = true; return r;
        }
    }

    // ── PHASE 2: subsystem list, round-robin from a RANDOM start (lines 119-205) ───────────────────────
    // Unlike phase 1, an invalid-bone subsystem is NOT skipped — it aims at {0,0,0} (caller sets pos).
    if (in.subsystem_count > 0) {
        int idx = rng.range_i(0, in.subsystem_count - 1);   // FUN_1401ffb40(&DAT_140a13e24, 0, n-1) → substream
        for (int k = 0; k < in.subsystem_count; ++k) {
            const AimSubsystem& s = in.subsystems[idx];
            if (!s.destroyed) {                              // bVar8 skip
                if (firing_point_in_range(muzzle, s.pos, range12) && s.reachable) {
                    r.aim = s.pos; r.locked = true; return r;
                }
            }
            idx = (idx + 1 < in.subsystem_count) ? idx + 1 : 0;   // wrap (lines 195-200)
        }
    }

    // ── PHASE 3: fall back to the target center (lines 206-238) ────────────────────────────────────────
    // The extent is added UNCONDITIONALLY here (no flag gate). Both branches aim at the center; only
    // `locked` differs (398440 is RNG-free/deterministic, so the phase-0 and phase-3 centers coincide).
    const f32 range3 = in.weapon_range + in.target_extent;
    r.aim    = in.target_center;
    r.locked = firing_point_in_range(muzzle, in.target_center, range3) && in.center_reachable;
    return r;
}

} // namespace sim
