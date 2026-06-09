// sim/fire_control.cpp — see fire_control.h. The host assembly of FUN_1403825b0's geometry decision
// core: composes the lifted leaves (firing_resolve_aimpoint / firing_intercept_lead / firing_apply_spread
// / firing_make_spawn) in the binary's stage order. FP grouping of the range gate preserved.
#include "fire_control.h"
#include <cmath>

namespace sim {

using eaw::f32;
using eaw::vec3;

FireControlDecision fire_control_decide(const FireControlInputs& in, eaw::SimRng& rng) {
    FireControlDecision d;

    // ── Stage A/C/D: eligibility (62-163) ────────────────────────────────────────────────────────────
    if (!in.eligible) { d.outcome = FireOutcome::NoFire_Ineligible; return d; }

    // ── Stage E: aim resolution (164-187) ────────────────────────────────────────────────────────────
    // param_3 != 0  → the explicit firing context: aim = 385c70 pose (hoisted), always "locked".
    // param_3 == 0  → the aim ladder (firing_resolve_aimpoint); if it does not lock, the binary returns 0
    //                 (line 178: `if ((char)local_res8 == '\0') return 0;`).
    if (in.has_explicit_context) {
        d.aim_point = in.context_aim;
    } else {
        FiringAimResult aim = firing_resolve_aimpoint(in.aim_inputs, rng);
        if (!aim.locked) { d.outcome = FireOutcome::NoFire_NoAim; return d; }
        d.aim_point = aim.aim;
    }

    // ── Stage F: muzzle + range gate (188-209) ───────────────────────────────────────────────────────
    // 385e70 must resolve the muzzle (cVar2 != 0) or the body never reaches the create. The range test is
    // 2D (x,y): dist = sqrt((muzzle.y-aim.y)^2 + (muzzle.x-aim.x)^2); accept iff
    //   min_range (weapon+0x23c) <= dist <= weapon_range (3857d0) + target_extent (397780).   (extent
    // is added UNCONDITIONALLY here, like 383f70's phase 3.)
    if (!in.muzzle_valid) { d.outcome = FireOutcome::NoFire_OutOfRange; return d; }
    {
        const f32 dy = in.muzzle.y - d.aim_point.y;
        const f32 dx = in.muzzle.x - d.aim_point.x;
        const f32 dist = std::sqrt(dy * dy + dx * dx);          // FUN_140776d48, y-term then x-term
        const f32 max_range = in.weapon_range + in.target_extent;
        if (!(dist <= max_range && in.min_range <= dist)) {
            d.outcome = FireOutcome::NoFire_OutOfRange; return d;
        }
    }

    // ── Stage H: lead solve + reach (226-238) ────────────────────────────────────────────────────────
    // 399450 lead solver (here its 399e20 fallback, firing_intercept_lead). A zero lead vector is the
    // no-solution sentinel (line 236: lead.x^2+lead.y^2+lead.z^2 != 0). Then 383ba0 reach/LOS (RNG-free).
    bool has_sol = false;
    const vec3 lead = firing_intercept_lead(in.target_pos, in.target_vel, in.frame_vel,
                                            d.aim_point, in.shooter_ref, in.gamespeed,
                                            in.proj_speed, &has_sol);
    if (lead == vec3{}) { d.outcome = FireOutcome::NoFire_NoLead; return d; }
    if (!in.aim_reachable) { d.outcome = FireOutcome::NoFire_Unreachable; return d; }

    // ── Stage I: dispersion + payload + spawn (250-401, 266) ──────────────────────────────────────────
    // 381dc0 perturbs the lead into the final launch dir; the b3 payload (firing_make_spawn) carries it.
    d.launch_dir = firing_apply_spread(lead, in.no_spread, in.base_spread, in.sec_spread,
                                       in.spread_dist, in.spread_norm, rng);

    ProjectileFiringInputs spawn = in.spawn;
    spawn.launch_dir = d.launch_dir;
    d.cmd = firing_make_spawn(spawn);
    d.outcome = FireOutcome::Fire;
    return d;
}

} // namespace sim
