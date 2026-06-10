// sim/fire_control.h — (b) firing-body ORCHESTRATION: the host assembly of FUN_1403825b0.
//
// 3825b0 is the §0-blocker fire-control body. The leaf passes (b1/b2/b3) lifted its pieces into sim/:
//   firing_pick_random_target (405870), firing_resolve_aimpoint (383f70), firing_intercept_lead (399e20)
//   + firing_predict_pos_linear (393b70), firing_apply_spread (381dc0), and the b3 create+init payload
//   (firing_build_projectile_init / firing_make_spawn). THIS file is the orchestration that wires those
//   leaves together in the body's decision sequence, so the fire path runs as ONE host function driving
//   the lifted leaves — the "engine-source" assembly of the fire path (firing_body_lift_scope.md §8.48).
//
// SCOPE OF THIS INCREMENT — the GEOMETRY DECISION CORE (3825b0 stages E→I):
//   E aim       → firing_resolve_aimpoint (param_3==0 path) or the explicit-context pose (385c70, hoisted)
//   F range     → 2D muzzle→aim distance within [min_range, weapon_range + extent]   (lines 203-209)
//   H lead      → firing_intercept_lead; a zero lead vector means "no solution" → no fire   (line 236)
//   H reach     → 383ba0 verdict (RNG-free, hoisted as a bool)                              (line 238)
//   I dir/spread→ firing_apply_spread on the lead dir → ProjectileFiringInputs.launch_dir → firing_make_spawn
// On Fire it EMITS one SpawnCommand (the b3 payload), exactly as 3825b0:266 does inline.
//
// HOISTED / DEFERRED (subsequent increments, documented in §8.48):
//   • Stage A/C/D eligibility gates (62-163): ~10 opaque object-state early-outs → ONE `eligible`
//     precondition here (each is a Phase-A read-only predicate already characterized by threading_model;
//     enumerating them adds no verifiable geometry).
//   • Stage G dir-source jitter (210-225, the 3-way ffbb0/ffdb0 launch-ref select) → `shooter_ref`
//     hoisted; its RNG draws are an additional seam to lift.
//   • Stage H curved-lead mode (399450 full solver) — firing_intercept lifted only the 399e20 fallback.
//   • Stage J cooldown recharge (402-493, the :410 RNG) — own-state write, a separate piece.
//   • Stage K opportunity-target update (494-499, 382510 Class-2b) — buffered cross-entity write.
//   • The pose leaves 385c70/385e70 (muzzle/context pose) — hoisted (no RNG; §8.32 pre-warm class).
//
// RNG: this pipeline draws ONLY from the per-entity substream (firing_resolve_aimpoint's phase-2 start +
// firing_apply_spread's 0-or-3 draws) — no fixed-global read. A determinism RETROFIT, not a bit-exact
// lift (sim_rng.h / sim_parallel_command_schema.md §4); the geometry/branch structure IS faithful.
#pragma once
#include "eaw_types.h"
#include "sim_rng.h"
#include "firing_aimpoint.h"
#include "firing_intercept.h"
#include "firing_spawn.h"

namespace sim {

// The eligibility ladder (3825b0 stages A/C/D, lines 62-163) — the ordered early-out gates BEFORE any
// geometry. Each predicate is an opaque object-state read hoisted to a bool (true = the gate PASSES);
// `fire_control_decide` checks them in this exact order so the first failure reproduces the binary's
// return point (the diagnostic an in-game oracle compares against). Reproducing the *order* is the value
// here; the predicate VALUES stay hoisted (the reads are Phase-A own/snapshot state, no geometry).
enum class FireGate {
    None,               // all gates passed
    OwnerPresent,       // *(param_1+0x10) != 0                                   (62)
    TargetPresent,      // param_2 != 0                                           (65)
    ContextMatch,       // param_3==0 || *(param_3+0x10)==param_2                 (68)
    TargetTargetable,   // (*(param_2+0x74) & 0x40) == 0                          (71)
    TargetQueryable,    // vfunc(param_2,0x11) == 0                               (74)
    CapabilityMatch,    // !(39b140(param_2) && *(owner_tmpl+0xa4)==0)            (78-82)
    OrderCharge,        // owner+0x100 → +0x394 charge state                      (83-91)
    NotSelfTarget,      // 540140(owner_team, param_2) != 1                       (93-96)
    NotFogged,          // 35f470(combat, owner_id, param_2, 1) != 1             (97-101)
    Diplomacy,          // !((owner/target +0x3b4==1) && 39a540()==0)             (102-106)
    FiringArc,          // stage C cone: weapon+0x4e==1 ⇒ dot >= threshold        (111-146)
    WeaponSelected,     // stage D: a projectile template resolved                (147-163)
};

struct FireEligibility {
    bool owner_present     = true;
    bool target_present    = true;
    bool context_match     = true;
    bool target_targetable = true;
    bool target_queryable  = true;
    bool capability_match  = true;
    bool order_charge_ok   = true;
    bool not_self_target   = true;
    bool not_fogged        = true;
    bool diplomacy_ok      = true;
    bool firing_arc_ok     = true;   // stage C
    bool weapon_selected   = true;   // stage D
};

// First failing gate in 3825b0 order, or FireGate::None when all pass.
FireGate fire_first_blocked_gate(const FireEligibility& g);

// All hoisted inputs for one fire-control decision. Offsets in comments are into the 3825b0 body / source
// objects (the firing_spawn ENV-getter pattern); the caller resolves them from snapshot/own-state.
struct FireControlInputs {
    FireEligibility gates{};              // stages A/C/D ordered gates (62-163); first failure ⇒ NO fire

    // ── E: aim ────────────────────────────────────────────────────────────────────────────────────
    bool      has_explicit_context = false;   // param_3 != 0 → use the 385c70 pose, skip the aim ladder
    FiringAimInputs aim_inputs{};             // param_3==0 path → firing_resolve_aimpoint
    eaw::vec3 context_aim = {};               // 385c70(param_3) resolved aim, when has_explicit_context

    // ── F: muzzle + range gate (385e70 muzzle pose; 2D distance) ────────────────────────────────────
    bool      muzzle_valid = false;       // 385e70 returned != 0
    eaw::vec3 muzzle = {};                 // muzzle world pos
    float     weapon_range = 0.0f;        // 3857d0(firer)
    float     target_extent = 0.0f;       // 397780(target)
    float     min_range = 0.0f;           // *(weapon+0x23c)

    // ── G: launch-point select (210-225) — the muzzle point fed to the lead solver (399450 param_5).
    // Picked from the two muzzle-bone matrix translations (385e70 mat1/mat2). RNG seam, now on the
    // substream: full_random ⇒ 3 range_f draws (a point in the spread cone); else a 50/50 pick (or mat1
    // when there is no second bone).
    eaw::vec3 muzzle_mat1_t = {};         // mat1 translation = (uStack_260,250,240)._4_4_
    eaw::vec3 muzzle_mat2_t = {};         // mat2 translation = (uStack_290,280,270)._4_4_
    bool      full_random_dir = false;    // weapon+0x4f == 1 (full-random spread cone)
    bool      has_muzzle_bone2 = false;   // param_1+0x3c >= 0 (a second launcher bone exists)

    // ── H: lead solve + reach ───────────────────────────────────────────────────────────────────────
    eaw::vec3 target_pos = {};            // snapshot target world pos
    eaw::vec3 target_vel = {};            // snapshot target velocity
    eaw::vec3 frame_vel  = {};            // reference-frame velocity (firing_intercept)
    float     gamespeed = 1.0f;           // DAT_140b0a340
    float     proj_speed = 0.0f;          // 370f00(template) muzzle speed
    bool      aim_reachable = false;      // 383ba0(firer, lead aim) verdict (RNG-free)

    // ── I: dispersion (firing_apply_spread) ─────────────────────────────────────────────────────────
    bool  no_spread   = false;            // DAT_140b3934d == 1
    float base_spread = 0.0f;
    float sec_spread  = 0.0f;
    float spread_dist = 0.0f;
    float spread_norm = 0.0f;

    // ── I: the projectile payload (firing_make_spawn). `launch_dir` is filled BY this pipeline. ───────
    ProjectileFiringInputs spawn{};

    // ── J: cooldown base roll (402-413) — own-state, the :410 RNG seam, now on the substream. Rolled
    // only when the burst counter reaches 0 (hoisted as `roll_cooldown`; the surrounding rate-of-fire
    // modifier math, 414-491, is own-state and a later stage-J completion). ──────────────────────────
    bool  roll_cooldown   = false;        // *(param_1+0x5c) decremented to 0 this shot → roll
    float cooldown_min    = 0.0f;         // *(weapon+0x228)
    float cooldown_max    = 0.0f;         // *(weapon+0x22c)
    float cooldown_scale  = 1.0f;         // DAT_1408007f0
};

enum class FireOutcome {
    NoFire_Ineligible,    // a stage A/C/D gate blocked
    NoFire_NoAim,         // aim ladder found no locked point (param_3==0 path)
    NoFire_OutOfRange,    // muzzle→aim distance outside [min_range, range+extent]
    NoFire_NoLead,        // the lead solver returned no positive-time solution
    NoFire_Unreachable,   // 383ba0 reach/LOS failed on the lead aim
    Fire                  // a SpawnCommand was produced
};

struct FireControlDecision {
    FireOutcome outcome = FireOutcome::NoFire_Ineligible;
    FireGate    blocked_gate = FireGate::None;  // which A/C/D gate blocked, when NoFire_Ineligible
    eaw::vec3   aim_point  = {};   // the resolved aim (stage E)
    eaw::vec3   launch_dir = {};   // post-lead, post-spread launch direction (stage I)
    SpawnCommand cmd{};            // valid iff outcome == Fire
    bool        cooldown_rolled = false;  // stage J fired (Fire + roll_cooldown)
    int         cooldown_base   = 0;      // the rolled base countdown (param_1+0x58), when cooldown_rolled
};

// Run the fire-control decision over hoisted inputs, composing the lifted leaves in 3825b0 order. Draws
// only from `rng` (the per-entity substream). On Fire, `cmd` is the SpawnCommand to emit.
FireControlDecision fire_control_decide(const FireControlInputs& in, eaw::SimRng& rng);

// ── lifted RNG seams of the fire body (exposed for unit tests) ─────────────────────────────────────────
// Stage G (210-225): the launch/muzzle POINT select.
//   full_random       → per-axis range_f(mat1.k, mat2.k)  [3 draws, a point in the spread cone]
//   !has_bone2         → mat1                              [0 draws]
//   else percent(50)   → mat1 (true) / mat2 (false)        [1 draw]
eaw::vec3 firing_select_muzzle_point(const eaw::vec3& mat1_t, const eaw::vec3& mat2_t,
                                     bool full_random, bool has_bone2, eaw::SimRng& rng);

// Stage J (402-413, the :410 draw): the base cooldown countdown.
//   draw = range_i((int)(min*scale), (int)(max*scale))
//   return (int)( ((int64_t)((float)draw * gamespeed) & 0xffffffff) / 100 )
int firing_roll_cooldown_base(float min_delay, float max_delay, float scale, float gamespeed,
                              eaw::SimRng& rng);

} // namespace sim
