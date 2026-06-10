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

// Stage J (414-491): the modifier ladder shared by both cooldown branches. 1.0 = DAT_1407ffaf8.
float firing_rate_multiplier(const CooldownModInputs& m) {
    f32 f = 1.0f;                                        // movaps %xmm9,%xmm6
    if (m.cat0_active) f = m.cat0_mult;                  // 39b950 ⇒ weapon+0x250[idx0]
    f *= m.owner_rof_mod;                                // 398010(owner, 1)
    if (m.cat1_active) f *= m.cat1_mult;                 // 39b950 ⇒ weapon+0x250[idx1]
    return f;
}

float firing_recharge_divisor(const CooldownModInputs& m) {
    return m.owner_recharge_mod * (m.buff_fold + 1.0f);  // 398010(owner,9) * (33fb70 fold + 1.0)
}

// Stage J, roll branch (3825b0:409-457). 0.5 = DAT_1408007c0; the (f32)(u32) conversions mirror the
// zero-extended cvtsi2ss %rax loads of +0x58; the truncating (int) casts are cvttss2si.
CooldownState firing_recharge_after_burst(int rolled_base, const CooldownModInputs& m) {
    CooldownState s;
    eaw::u32 r = (eaw::u32)rolled_base;
    const f32 f = firing_rate_multiplier(m);
    if (f < 1.0f)                                        // comiss: apply only when f < 1.0
        r = (eaw::u32)(int)((f32)r * f + 0.5f);
    f32 g = firing_recharge_divisor(m);
    if (0.0f < g && g < 1.0f)                            // apply only when 0 < g < 1
        r = (eaw::u32)(int)((f32)r / g + 0.5f);
    if (g < 0.0f) g = 0.0f;                              // maxss %xmm10,%xmm6
    s.recharge = (int)r;
    s.burst = (int)((f32)m.burst_count * g + 0.5f);      // (uint)weapon+0x230 via cvtsi2ss %rcx
    return s;
}

// Stage J, between-shots branch (3825b0:459-495). Tail asm-corrected (140383503-525): on g <= 0 the
// binary stores ebx — zeroed at 140382d3d, untouched since — to BOTH +0x5c and +0x58.
CooldownState firing_delay_between_shots(f32 gamespeed, const CooldownModInputs& m, int burst_after) {
    CooldownState s;
    // cvttss2si %xmm0,%rax of (float)gamespeed_int * weapon+0x234, stored as the low 32 bits.
    eaw::u32 r = (eaw::u32)(eaw::i64)(gamespeed * m.shot_delay);
    const f32 f = firing_rate_multiplier(m);
    r = (eaw::u32)(int)((f32)r * f + 0.5f);              // UNCONDITIONAL here (no f<1 guard)
    const f32 g = firing_recharge_divisor(m);
    if (0.0f < g) {
        s.recharge = (int)((f32)r / g + 0.5f);
        s.burst = burst_after;                           // +0x5c keeps the decremented counter
    } else {
        s.recharge = 0;                                  // mov %ebx,0x58(%rdi), ebx == 0
        s.burst = 0;                                     // mov %ebx,0x5c(%rdi)
    }
    return s;
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

    // ── Stage J: cooldown (402-493) ───────────────────────────────────────────────────────────────────
    // Own-state, only on the fired path (after the create). Decrement the burst counter; at 0 roll a new
    // recharge base (the :410 draw — last RNG seam, after the spread) and run the roll-branch modifier
    // ladder; otherwise recompute the between-shot delay (no draw).
    int counter = in.burst_counter;
    if (counter != 0) counter -= 1;                      // 3825b0:403-407
    if (counter == 0) {
        d.cooldown_base = firing_roll_cooldown_base(in.cooldown_min, in.cooldown_max,
                                                    in.cooldown_scale, in.gamespeed, rng);
        d.cooldown_rolled = true;
        d.cooldown = firing_recharge_after_burst(d.cooldown_base, in.cooldown_mods);
    } else {
        d.cooldown = firing_delay_between_shots(in.gamespeed, in.cooldown_mods, counter);
    }
    return d;
}

} // namespace sim
