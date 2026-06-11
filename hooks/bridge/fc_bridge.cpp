// Companion DLL bridging the validated sim:: fire-control lift into the game: a thin extern "C"
// surface the (pure-C) winmm hook LoadLibrary's, so the in-game fire path runs the SAME code the
// §8.51-8.57 oracle validated instead of a parallel C transcription. §8.58 proved this links into a
// self-contained PE DLL (KERNEL32/msvcrt/ntdll only). B3.8 migrates the hook onto it, observe-first.
#include "fire_control.h"
#include "sim_rng.h"
#include <windows.h>

extern "C" __declspec(dllexport)
int fc_bridge_decide(const void* inputs_blob, unsigned int rng_seed, int* out_gate)
{
    // The hook fills a sim::FireControlInputs and passes &it as inputs_blob. Here we just
    // exercise the linked code so the linker pulls the whole sim/ fire_control closure.
    const sim::FireControlInputs& in = *reinterpret_cast<const sim::FireControlInputs*>(inputs_blob);
    eaw::SimRng rng(rng_seed);
    sim::FireControlDecision d = sim::fire_control_decide(in, rng, /*sink=*/nullptr);
    if (out_gate) *out_gate = static_cast<int>(d.blocked_gate);
    return static_cast<int>(d.outcome);
}

// A trivial second export to confirm a pure leaf links too.
extern "C" __declspec(dllexport)
int fc_bridge_first_blocked(const void* elig_blob)
{
    const sim::FireEligibility& g = *reinterpret_cast<const sim::FireEligibility*>(elig_blob);
    return static_cast<int>(sim::fire_first_blocked_gate(g));
}

// §8.58 B3.8.1 — the in-process equivalence entry. Takes the 12-bit gate mask the hook's
// dtfc_gate_bits already produces (bit i set = gate i passes, sim::FireGate order — the SAME
// encoding the offline §8.51 oracle replays) and returns the first-blocked sim::FireGate as an int.
// Scalar in / scalar out: NO struct layout crosses the C->C++ boundary, so the marshalling is
// immune to ABI/padding drift between the pure-C hook and the C++ bridge.
extern "C" __declspec(dllexport)
int fc_bridge_gate_from_bits(unsigned int bits)
{
    sim::FireEligibility g;
    g.owner_present     = (bits >> 0)  & 1;
    g.target_present    = (bits >> 1)  & 1;
    g.context_match     = (bits >> 2)  & 1;
    g.target_targetable = (bits >> 3)  & 1;
    g.target_queryable  = (bits >> 4)  & 1;
    g.capability_match  = (bits >> 5)  & 1;
    g.order_charge_ok   = (bits >> 6)  & 1;
    g.not_self_target   = (bits >> 7)  & 1;
    g.not_fogged        = (bits >> 8)  & 1;
    g.diplomacy_ok      = (bits >> 9)  & 1;
    g.firing_arc_ok     = (bits >> 10) & 1;
    g.weapon_selected   = (bits >> 11) & 1;
    return static_cast<int>(sim::fire_first_blocked_gate(g));
}

// §8.59 B3.8.2 — stage-F range gate (§8.52). Scalar boundary: the hook passes the same DTFCG geometry
// it already captures (muzzle x/y, aim x/y, weapon_range, target_extent, min_range — the gate is 2D, z
// unused) and gets the in-range verdict back. Calls the SAME sim::fire_range_gate_pass that
// fire_control_decide uses, so the in-process verdict is bit-identical to the offline §8.52 replay.
extern "C" __declspec(dllexport)
int fc_bridge_range_gate(float mx, float my, float ax, float ay,
                         float weapon_range, float target_extent, float min_range)
{
    eaw::vec3 muzzle; muzzle.x = mx; muzzle.y = my;
    eaw::vec3 aim;    aim.x = ax;    aim.y = ay;
    return sim::fire_range_gate_pass(muzzle, aim, weapon_range, target_extent, min_range) ? 1 : 0;
}

// §8.61 B3.8.3 — stage-H projectile-intercept lead solver (§8.53, the binary's 399e20 fallback path).
// Scalar boundary: the hook passes the same DTFCL inputs it captures (tgt_pos, tgt_vel, frame_vel,
// muzzle, shooter_ref, gamespeed, proj_speed); the lead is returned through out_lead[3] and the
// return value is the solution-existence verdict (1/0). Calls the SAME sim::firing_intercept_lead the
// offline §8.53 oracle replays bit-exact against the binary's 399e20 output.
extern "C" __declspec(dllexport)
int fc_bridge_intercept_lead(float tpx, float tpy, float tpz,
                             float tvx, float tvy, float tvz,
                             float fvx, float fvy, float fvz,
                             float mzx, float mzy, float mzz,
                             float srx, float sry, float srz,
                             float gamespeed, float proj_speed, float* out_lead)
{
    bool sol = false;
    eaw::vec3 lead = sim::firing_intercept_lead(
        eaw::vec3{tpx, tpy, tpz}, eaw::vec3{tvx, tvy, tvz}, eaw::vec3{fvx, fvy, fvz},
        eaw::vec3{mzx, mzy, mzz}, eaw::vec3{srx, sry, srz}, gamespeed, proj_speed, &sol);
    if (out_lead) { out_lead[0] = lead.x; out_lead[1] = lead.y; out_lead[2] = lead.z; }
    return sol ? 1 : 0;
}

// §8.62 B3.8.4 — the COMPOSED outcome observe. Runs the REAL sim::fire_control_decide end-to-end on the
// live marshalled inputs (gates → aim → range → lead → reach), returning its FireOutcome. Scalar
// boundary: the hook passes the outcome-determining fields it already captures + the binary's ACTUAL
// stage-G shooter_ref, which is injected so the lead is bit-comparable to the binary's LCG path (the
// takeover proper passes null and draws shooter_ref from the substream). The spawn/cooldown/opp inputs
// are left neutral — they do not affect the Fire/NoFire verdict. This is the §8.54 composed-outcome
// oracle, now run in-process through the giant function on the full marshalled struct (the foundation
// for the control-flow flip). Returns FireOutcome as int (0=Ineligible,1=NoAim,2=OutOfRange,3=NoLead,
// 4=Unreachable,5=Fire); *out_blocked_gate = the FireGate when Ineligible.
extern "C" __declspec(dllexport)
int fc_bridge_decide_observe(unsigned int gate_bits,
                             const float* context_aim, int muzzle_valid, const float* muzzle,
                             float weapon_range, float target_extent, float min_range,
                             const float* target_pos, const float* target_vel, const float* frame_vel,
                             float gamespeed, float proj_speed, const float* shooter_ref,
                             int aim_reachable, unsigned int rng_seed, int* out_blocked_gate)
{
    sim::FireControlInputs in;
    in.gates.owner_present     = (gate_bits >> 0)  & 1;
    in.gates.target_present    = (gate_bits >> 1)  & 1;
    in.gates.context_match     = (gate_bits >> 2)  & 1;
    in.gates.target_targetable = (gate_bits >> 3)  & 1;
    in.gates.target_queryable  = (gate_bits >> 4)  & 1;
    in.gates.capability_match  = (gate_bits >> 5)  & 1;
    in.gates.order_charge_ok   = (gate_bits >> 6)  & 1;
    in.gates.not_self_target   = (gate_bits >> 7)  & 1;
    in.gates.not_fogged        = (gate_bits >> 8)  & 1;
    in.gates.diplomacy_ok      = (gate_bits >> 9)  & 1;
    in.gates.firing_arc_ok     = (gate_bits >> 10) & 1;
    in.gates.weapon_selected   = (gate_bits >> 11) & 1;
    in.has_explicit_context = true;
    in.context_aim   = eaw::vec3{context_aim[0], context_aim[1], context_aim[2]};
    in.muzzle_valid  = muzzle_valid != 0;
    in.muzzle        = eaw::vec3{muzzle[0], muzzle[1], muzzle[2]};
    in.weapon_range  = weapon_range;  in.target_extent = target_extent;  in.min_range = min_range;
    in.target_pos    = eaw::vec3{target_pos[0], target_pos[1], target_pos[2]};
    in.target_vel    = eaw::vec3{target_vel[0], target_vel[1], target_vel[2]};
    in.frame_vel     = eaw::vec3{frame_vel[0], frame_vel[1], frame_vel[2]};
    in.gamespeed     = gamespeed;     in.proj_speed = proj_speed;
    in.aim_reachable = aim_reachable != 0;

    eaw::vec3 sref{shooter_ref[0], shooter_ref[1], shooter_ref[2]};
    eaw::SimRng rng(rng_seed);
    sim::FireControlDecision d = sim::fire_control_decide(in, rng, /*sink=*/nullptr, &sref);
    if (out_blocked_gate) *out_blocked_gate = static_cast<int>(d.blocked_gate);
    return static_cast<int>(d.outcome);
}

// §8.63 B3.8.5 — APPLY-SIDE: the projectile-init payload. Runs the REAL sim::firing_build_projectile_init
// on the marshalled create-args and returns the THREE computed identity fields (damage / lifetime /
// vis_frame) — the ones whose owner-vs-template / charge / base+offset branch logic the sim lift owns
// (firer_id/target/sub_id are passthroughs already validated by the DTWA-B3 oracle). The hook compares
// these against the binary's actual projectile record (rec+0x64 / +0x68 / +0x60). Scalar boundary.
// §8.64 B3.8.6 extends this with the muzzle-speed payload (base/template + LEFT-added extent/subextent/
// |Δz| — RNG-free given the binary's aim_z/aimpoint_z), so o_muzzle_speed is bit-comparable to rec+0x6c.
extern "C" __declspec(dllexport)
int fc_bridge_build_init(float owner_damage, float template_damage, int apply_charge, float charge_mod,
                         unsigned int owner_lifetime, unsigned int template_lifetime,
                         int has_vis_frame, int vis_base, int vis_offset,
                         float base_speed, float template_speed, float target_extent,
                         int add_subextent, float target_subextent, float aim_z, float aimpoint_z,
                         float* o_damage, unsigned int* o_lifetime, int* o_vis, float* o_muzzle_speed)
{
    sim::ProjectileFiringInputs in;
    in.owner_damage      = owner_damage;     in.template_damage   = template_damage;
    in.apply_charge      = apply_charge != 0; in.charge_mod       = charge_mod;
    in.owner_lifetime    = owner_lifetime;   in.template_lifetime = template_lifetime;
    in.has_vis_frame     = has_vis_frame != 0; in.vis_base        = vis_base;  in.vis_offset = vis_offset;
    in.base_speed        = base_speed;       in.template_speed    = template_speed;
    in.target_extent     = target_extent;    in.add_subextent     = add_subextent != 0;
    in.target_subextent  = target_subextent; in.aim_z             = aim_z;    in.aimpoint_z = aimpoint_z;
    sim::ProjectileInit p = sim::firing_build_projectile_init(in);
    if (o_damage)       *o_damage       = p.damage;
    if (o_lifetime)     *o_lifetime     = p.lifetime;
    if (o_vis)          *o_vis          = p.vis_frame;
    if (o_muzzle_speed) *o_muzzle_speed = p.muzzle_speed;
    return 1;
}

// §8.67 B3.8.9 — APPLY-SIDE geometry drive: weapon dispersion. Runs the REAL sim::firing_apply_spread
// (the branch-exact lift of 381dc0, §8.66) on a per-fire SimRng substream, returning the dispersed launch
// direction. The hook drives the body's S[0..2] with this in place of the binary's 381dc0 — same spread
// cone, different RNG sample (the §8.42 substream retrofit). Scalar boundary.
extern "C" __declspec(dllexport)
int fc_bridge_apply_spread(float dx, float dy, float dz, int no_spread,
                           float base_spread, float sec_spread, float dist, float norm,
                           unsigned int seed, float* out_dir)
{
    eaw::vec3 dir{dx, dy, dz};
    eaw::SimRng rng(seed);
    eaw::vec3 o = sim::firing_apply_spread(dir, no_spread != 0, base_spread, sec_spread,
                                           dist, norm, rng);
    if (out_dir) { out_dir[0] = o.x; out_dir[1] = o.y; out_dir[2] = o.z; }
    return 1;
}

// §8.68 B3.8.10 — APPLY-SIDE geometry drive: muzzle-point select (spawn_pos cone). Runs the REAL
// sim::firing_select_muzzle_point (the faithful lift of the body's 210-225 muzzle-cone draw) on a per-fire
// SimRng substream, returning the muzzle point that becomes S[8/9/6] = the spawn position basis. The hook
// drives the body's muzzle-cone draw with this in place of the binary's ffbb0/ffdb0 — same cone, different
// RNG sample (§8.42). Scalar boundary.
extern "C" __declspec(dllexport)
int fc_bridge_select_muzzle(float m1x, float m1y, float m1z, float m2x, float m2y, float m2z,
                            int full_random, int has_bone2, unsigned int seed, float* out_m)
{
    eaw::vec3 mat1{m1x, m1y, m1z}, mat2{m2x, m2y, m2z};
    eaw::SimRng rng(seed);
    eaw::vec3 m = sim::firing_select_muzzle_point(mat1, mat2, full_random != 0, has_bone2 != 0, rng);
    if (out_m) { out_m[0] = m.x; out_m[1] = m.y; out_m[2] = m.z; }
    return 1;
}

BOOL WINAPI DllMain(HINSTANCE, DWORD, LPVOID) { return TRUE; }
