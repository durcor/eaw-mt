// sim/fire_control.cpp — see fire_control.h. The host assembly of FUN_1403825b0's geometry decision
// core: composes the lifted leaves (firing_resolve_aimpoint / firing_intercept_lead / firing_apply_spread
// / firing_make_spawn) in the binary's stage order. FP grouping of the range gate preserved.
#include "fire_control.h"
#include <cmath>

namespace sim {

using eaw::f32;
using eaw::vec3;

// Stage G (210-225): the launch/muzzle POINT select. ffbb0 = range_f, ffdb0(50) = percent(50).
vec3 firing_select_muzzle_point(const vec3& mat1, const vec3& mat2,
                                bool full_random, bool has_bone2, eaw::SimRng& rng) {
    if (full_random) {                                   // weapon+0x4f==1: 3 draws, x→y→z
        vec3 m;
        m.x = rng.range_f(mat1.x, mat2.x);
        m.y = rng.range_f(mat1.y, mat2.y);
        m.z = rng.range_f(mat1.z, mat2.z);
        return m;
    }
    if (!has_bone2) return mat1;                         // param_1+0x3c < 0 → mat1, NO draw (short-circuit)
    return rng.percent(50) ? mat1 : mat2;                // ffdb0(&lcg, 0x32): true → mat1, false → mat2
}

// Stage J (402-413, the :410 draw): the base cooldown countdown.
int firing_roll_cooldown_base(f32 min_delay, f32 max_delay, f32 scale, f32 gamespeed,
                              eaw::SimRng& rng) {
    const int lo = (int)(min_delay * scale);             // (int)(weapon+0x228 * DAT_1408007f0)
    const int hi = (int)(max_delay * scale);             // (int)(weapon+0x22c * DAT_1408007f0)
    const int draw = rng.range_i(lo, hi);                // FUN_1401ffb40(&DAT_140a13e24, lo, hi)
    const int64_t scaled = (int64_t)((f32)draw * gamespeed);
    return (int)((scaled & 0xffffffffLL) / 100);         // ((longlong)(...) & 0xffffffffU) / 100
}

FireGate fire_first_blocked_gate(const FireEligibility& g) {
    if (!g.owner_present)     return FireGate::OwnerPresent;
    if (!g.target_present)    return FireGate::TargetPresent;
    if (!g.context_match)     return FireGate::ContextMatch;
    if (!g.target_targetable) return FireGate::TargetTargetable;
    if (!g.target_queryable)  return FireGate::TargetQueryable;
    if (!g.capability_match)  return FireGate::CapabilityMatch;
    if (!g.order_charge_ok)   return FireGate::OrderCharge;
    if (!g.not_self_target)   return FireGate::NotSelfTarget;
    if (!g.not_fogged)        return FireGate::NotFogged;
    if (!g.diplomacy_ok)      return FireGate::Diplomacy;
    if (!g.firing_arc_ok)     return FireGate::FiringArc;
    if (!g.weapon_selected)   return FireGate::WeaponSelected;
    return FireGate::None;
}

// Stage F range gate (188-209): 2D muzzle→aim distance within [min_range, weapon_range + target_extent].
// The extent is added UNCONDITIONALLY here (like 383f70 phase 3). FP grouping faithful to 203-209: the
// distance accumulates y-term THEN x-term (FUN_140776d48 = sqrtf).
bool fire_range_gate_pass(const vec3& muzzle, const vec3& aim,
                          f32 weapon_range, f32 target_extent, f32 min_range) {
    const f32 dy = muzzle.y - aim.y;
    const f32 dx = muzzle.x - aim.x;
    const f32 dist = std::sqrt(dy * dy + dx * dx);
    const f32 max_range = weapon_range + target_extent;
    return dist <= max_range && min_range <= dist;
}

FireControlDecision fire_control_decide(const FireControlInputs& in, eaw::SimRng& rng) {
    FireControlDecision d;

    // ── Stage A/C/D: eligibility (62-163) — ordered early-out ladder ──────────────────────────────────
    d.blocked_gate = fire_first_blocked_gate(in.gates);
    if (d.blocked_gate != FireGate::None) { d.outcome = FireOutcome::NoFire_Ineligible; return d; }

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
    if (!fire_range_gate_pass(in.muzzle, d.aim_point, in.weapon_range, in.target_extent, in.min_range)) {
        d.outcome = FireOutcome::NoFire_OutOfRange; return d;
    }

    // ── Stage G: launch-point select (210-225) ───────────────────────────────────────────────────────
    // The muzzle point fed to the lead solver. Draws BEFORE the lead/spread/cooldown seams (binary order).
    const vec3 shooter_ref = firing_select_muzzle_point(in.muzzle_mat1_t, in.muzzle_mat2_t,
                                                        in.full_random_dir, in.has_muzzle_bone2, rng);

    // ── Stage H: lead solve + reach (226-238) ────────────────────────────────────────────────────────
    // 399450 lead solver (here its 399e20 fallback, firing_intercept_lead). A zero lead vector is the
    // no-solution sentinel (line 236: lead.x^2+lead.y^2+lead.z^2 != 0). Then 383ba0 reach/LOS (RNG-free).
    bool has_sol = false;
    const vec3 lead = firing_intercept_lead(in.target_pos, in.target_vel, in.frame_vel,
                                            d.aim_point, shooter_ref, in.gamespeed,
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

    // ── Stage J: cooldown base roll (402-413) ─────────────────────────────────────────────────────────
    // Own-state; rolled only when the burst counter reaches 0 this shot. Last RNG seam (after the spread).
    if (in.roll_cooldown) {
        d.cooldown_base = firing_roll_cooldown_base(in.cooldown_min, in.cooldown_max,
                                                    in.cooldown_scale, in.gamespeed, rng);
        d.cooldown_rolled = true;
    }
    return d;
}

} // namespace sim
