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

// All hoisted inputs for one fire-control decision. Offsets in comments are into the 3825b0 body / source
// objects (the firing_spawn ENV-getter pattern); the caller resolves them from snapshot/own-state.
struct FireControlInputs {
    bool eligible = false;                // stages A/C/D gates all passed (62-163); else NO fire

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

    // ── H: lead solve + reach ───────────────────────────────────────────────────────────────────────
    eaw::vec3 target_pos = {};            // snapshot target world pos
    eaw::vec3 target_vel = {};            // snapshot target velocity
    eaw::vec3 frame_vel  = {};            // reference-frame velocity (firing_intercept)
    eaw::vec3 shooter_ref = {};           // stage-G dir/ref (399450 param_5; hoisted)
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
    eaw::vec3   aim_point  = {};   // the resolved aim (stage E)
    eaw::vec3   launch_dir = {};   // post-lead, post-spread launch direction (stage I)
    SpawnCommand cmd{};            // valid iff outcome == Fire
};

// Run the fire-control decision over hoisted inputs, composing the lifted leaves in 3825b0 order. Draws
// only from `rng` (the per-entity substream). On Fire, `cmd` is the SpawnCommand to emit.
FireControlDecision fire_control_decide(const FireControlInputs& in, eaw::SimRng& rng);

} // namespace sim
