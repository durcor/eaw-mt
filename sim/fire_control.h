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
#include "command_sink.h"
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

// Stage-J rate-of-fire modifier values (3825b0:414-491) — every term is an opaque Phase-A read
// (category index 374b50 / active-check 39b950 / recursive owner modifier 398010 / the 33fb70
// Plus-of-per-bucket-max buff fold over the 395c70-collected modifier set), hoisted to its VALUE;
// the lifted ladder is the arithmetic that combines them. Defaults are the neutral element.
struct CooldownModInputs {
    bool  cat0_active = false;     // 39b950(owner, 374b50(owner+0x298, 0), 1)
    float cat0_mult   = 1.0f;      // *(weapon+0x250 + idx0*4), used iff cat0_active
    bool  cat1_active = false;     // 39b950(owner, 374b50(owner+0x298, 1), 1)
    float cat1_mult   = 1.0f;      // *(weapon+0x250 + idx1*4), used iff cat1_active
    float owner_rof_mod = 1.0f;    // 398010(owner, 1)  — rate-of-fire owner/group modifier
    float owner_recharge_mod = 1.0f; // 398010(owner, 9) — recharge owner/group modifier
    float buff_fold = 0.0f;        // 33fb70(GreaterThan, Plus) fold over the 395c70 modifier set
    eaw::u32 burst_count = 0;      // *(uint*)(weapon+0x230) — burst reset base (roll branch)
    float shot_delay = 0.0f;       // *(float*)(weapon+0x234) — between-shot delay (no-roll branch)
};

// New cooldown own-state after stage J: the (+0x58, +0x5c) pair the binary writes.
struct CooldownState {
    int recharge = 0;              // *(param_1+0x58)
    int burst = 0;                 // *(param_1+0x5c)
};

// Stage-K inputs (3825b0:494-499 → 3846c0 clear / 382510 set). The "elsewhere-tracked" guards are
// the binary's identity checks against the firer's OTHER target slots (+0xc0/+0xa8/+0x40/+0x50) —
// listener edits are skipped for an object that is still tracked through another slot. Opaque
// own-state reads, hoisted to bools (NB 3846c0 zeroes +0xa8 BEFORE its check, so for the old target
// the +0xa8 compare is against 0; for the new target +0xa8 still holds the pre-clear value's slot,
// already zeroed by the clear that ran first).
struct OppTargetInputs {
    void* firer   = nullptr;       // param_1 — the listener registered on the target's dispatcher
    void* current = nullptr;       // *(param_1+0xa8) at entry (the old opportunity target)
    void* target  = nullptr;       // param_2 — the fired-at target
    bool  old_elsewhere_tracked = false;  // old ∈ {+0xc0, +0x40, +0x50} (3846c0 guard)
    bool  new_elsewhere_tracked = false;  // target ∈ {+0xc0, +0xa8, +0x40, +0x50} (382510 guard)
};

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

    // ── J: cooldown (402-493) — own-state; the :410 RNG seam on the substream. The binary decrements
    // the burst counter (param_1+0x5c); at 0 it ROLLS a new recharge base (firing_roll_cooldown_base)
    // and recomputes the recharge + burst reset; otherwise it recomputes the between-shot delay (no
    // draw). Both branches run the rate-of-fire modifier ladder (CooldownModInputs, opaque hoisted
    // reads). Deferred side effects of the roll branch: the +0x68 sensor-frame stamp and the
    // 294bc0/285d70 manager deregistration (own-state / manager-list writes, not decision math). ─────
    int   burst_counter   = 2;            // *(param_1+0x5c) at entry; decremented on fire; 0 ⇒ roll
    float cooldown_min    = 0.0f;         // *(weapon+0x228)
    float cooldown_max    = 0.0f;         // *(weapon+0x22c)
    float cooldown_scale  = 1.0f;         // DAT_1408007f0 (= 100.0f in the binary)
    CooldownModInputs cooldown_mods{};    // stage-J modifier ladder values (414-491)

    // ── K: opportunity-target update (494-499) — runs only on the fired path, right before return 1.
    // The own-slot (+0xa8) write is own-state (→ FireControlDecision.opp_target); the listener edits
    // on the old/new target's +0x38 dispatcher are Class-2b → buffered via CommandSink. ─────────────
    OppTargetInputs opp{};
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
    eaw::vec3   lead_raw   = {};   // PRE-spread intercept lead (stage H) — the guided-branch base (S[16..18])
    eaw::vec3   launch_dir = {};   // post-lead, post-spread launch direction (stage I)
    SpawnCommand cmd{};            // valid iff outcome == Fire
    bool        cooldown_rolled = false;  // stage J: burst counter reached 0 ⇒ the :410 draw happened
    int         cooldown_base   = 0;      // the rolled PRE-modifier base, when cooldown_rolled
    CooldownState cooldown{};             // stage J result: the new (+0x58, +0x5c) own-state pair
    void*       opp_target = nullptr;     // stage K result: the new +0xa8 own-slot value
};

// Run the fire-control decision over hoisted inputs, composing the lifted leaves in 3825b0 order. Draws
// only from `rng` (the per-entity substream). On Fire, `cmd` is the SpawnCommand to emit. When `sink`
// is non-null, stage K runs on the fired path (its Class-2b listener edits buffered through the sink,
// the new own-slot value in `opp_target`); a null sink skips stage K (the pre-§8.56 behaviour — the
// caller does not model the opp-target record).
FireControlDecision fire_control_decide(const FireControlInputs& in, eaw::SimRng& rng,
                                        CommandSink* sink = nullptr,
                                        const eaw::vec3* inject_shooter_ref = nullptr);

// Stage K (3825b0:494-499): the opportunity-target record update, the binary's exact branch shape:
//   *(+0xa8) == target  ⇒ no-op (returns current)
//   current != 0        ⇒ 3846c0 clear: slot = 0; unless old_elsewhere_tracked, emit_disconnect(old,
//                         firer, 0x28) + emit_disconnect(old, firer, 1)
//   target != 0         ⇒ 382510 set: unless new_elsewhere_tracked, emit_connect(target, firer, 0x28)
//                         + emit_connect(target, firer, 1); slot = target
// Returns the new own-slot (+0xa8) value. Listener edits go through the sink (Class 2b, buffered).
void* fire_update_opp_target(const OppTargetInputs& in, CommandSink& sink);

// ── lifted RNG seams of the fire body (exposed for unit tests) ─────────────────────────────────────────
// Stage G (210-225): the launch/muzzle POINT select.
//   full_random       → per-axis range_f(mat1.k, mat2.k)  [3 draws, a point in the spread cone]
//   !has_bone2         → mat1                              [0 draws]
//   else percent(50)   → mat1 (true) / mat2 (false)        [1 draw]
eaw::vec3 firing_select_muzzle_point(const eaw::vec3& mat1_t, const eaw::vec3& mat2_t,
                                     bool full_random, bool has_bone2, eaw::SimRng& rng);

// Stage F (188-209): the 2D range gate — sqrt((muzzle.y-aim.y)^2 + (muzzle.x-aim.x)^2) within
// [min_range, weapon_range + target_extent] (extent added unconditionally). Exposed for the in-game
// geometry oracle (§8.52): replayed over captured 385e70/385c70 geometry vs the binary's verdict.
bool fire_range_gate_pass(const eaw::vec3& muzzle, const eaw::vec3& aim,
                          float weapon_range, float target_extent, float min_range);

// Stage J (402-413, the :410 draw): the base cooldown countdown.
//   draw = range_i((int)(min*scale), (int)(max*scale))
//   return (int)( ((int64_t)((float)draw * gamespeed) & 0xffffffff) / 100 )
int firing_roll_cooldown_base(float min_delay, float max_delay, float scale, float gamespeed,
                              eaw::SimRng& rng);

// Stage J (414-491): the modifier ladder shared by both cooldown branches.
//   rate multiplier f: start 1.0; cat0_active ⇒ f = cat0_mult; f *= owner_rof_mod;
//                      cat1_active ⇒ f *= cat1_mult.
//   recharge divisor g: owner_recharge_mod * (buff_fold + 1.0).
float firing_rate_multiplier(const CooldownModInputs& m);
float firing_recharge_divisor(const CooldownModInputs& m);

// Stage J, roll branch (3825b0:409-457; asm 1403831d7-3833d4) — burst counter hit 0:
//   recharge = rolled_base;  f < 1.0       ⇒ recharge = (int)((float)(u32)recharge * f + 0.5)
//              0 < g < 1.0   ⇒ recharge = (int)((float)(u32)recharge / g + 0.5)
//   burst    = (int)((float)burst_count * max(g, 0.0) + 0.5)
CooldownState firing_recharge_after_burst(int rolled_base, const CooldownModInputs& m);

// Stage J, between-shots branch (3825b0:459-495; asm 1403833d9-383525) — counter still > 0 after the
// decrement (`burst_after`). NOTE the tail is asm-corrected vs the Ghidra decompile: at g <= 0 the
// binary writes ebx (= 0 since 140382d3d) to BOTH +0x5c and +0x58 — recharge AND burst zeroed.
//   recharge = (int)(int64)(gamespeed * shot_delay)
//   recharge = (int)((float)(u32)recharge * f + 0.5)            [unconditional, unlike the roll branch]
//   g > 0 ⇒ recharge = (int)((float)(u32)recharge / g + 0.5), burst = burst_after
//   g <= 0 ⇒ recharge = 0, burst = 0
CooldownState firing_delay_between_shots(float gamespeed, const CooldownModInputs& m, int burst_after);

} // namespace sim
