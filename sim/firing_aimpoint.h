// sim/firing_aimpoint.h — (b) firing-body lift: the aim-POINT resolver FUN_140383f70.
//
// The fire body calls this at 3825b0:174 to decide WHERE ON THE TARGET to aim, BEFORE the lead solve
// (firing_intercept). It walks three candidate sources in order and accepts the first whose muzzle-to-
// point distance is within weapon range AND that passes the reach/LOS check (383ba0):
//   PHASE 1  the target's designated aim-bones (model bone list, in order).
//   PHASE 2  the target's subsystem list, traversed round-robin from a RANDOM start — the §8.45 blocker
//            draw (FUN_1401ffb40(&DAT_140a13e24, 0, count-1)). Destroyed subsystems are skipped.
//   PHASE 3  fall back to the target's center (398440); "locked" iff that center is itself in range +
//            reachable, else the bare center with locked=false.
// If no model/skeleton resolves at all, it returns the bare center immediately (phase-0 gate).
//
// SNAPSHOT-READY (the firing_spawn pattern): every cross-object / engine read is hoisted to explicit
// input data — there is NO live pointer chasing or vfunc dispatch in this body. Specifically:
//   • Bone WORLD POSITIONS (the skeleton accessor FUN_14012d2c0) → the `pos` fields below.
//   • The target center (FUN_140398440) → `target_center`.
//   • Weapon range (FUN_1403857d0) + target extent (FUN_140397780) + the extent-gate flag → scalars.
//   • The reach/LOS verdict (FUN_140383ba0) → the `reachable` flags. 383ba0/398440/3857d0/397780 are all
//     RNG-FREE (verified), so eager evaluation of `reachable` does NOT change the draw count — the ONE
//     RNG seam is phase 2's random start, which this lift draws from a per-entity SUBSTREAM (sim_rng.h).
//
// RNG NOTE: the substream makes the chosen subsystem START differ from the global-LCG binary BY DESIGN
// (a determinism RETROFIT, not a bit-exact lift — sim_parallel_command_schema.md §4). What is preserved
// is the draw COUNT (1 iff phase 2 is reached with subsystem_count>0; 0 otherwise; count==1 ⇒ no advance)
// and the round-robin traversal policy. The geometry (midpoint / distance / range test / phase ladder)
// IS bit-faithful to the binary's operand order.
#pragma once
#include "eaw_types.h"
#include "sim_rng.h"

namespace sim {

// One PHASE-1 candidate: a resolved target aim-bone. `valid` = the 12d2c0 bone fetch succeeded; an
// invalid bone is SKIPPED entirely (the binary guards the whole accept block on cVar2==1). `reachable`
// = FUN_140383ba0(firer, pos) (only consulted when in-range; RNG-free so eager-safe).
struct AimCandidate {
    eaw::vec3 pos;
    bool valid     = false;
    bool reachable = false;
};

// One PHASE-2 candidate: a target subsystem. Unlike phase 1, an invalid bone is NOT skipped — the binary
// aims it at the ORIGIN {0,0,0}; so the caller sets `pos` = bone pos when resolved, else {0,0,0}.
// `destroyed` = the bVar8 skip (targetable component present & flagged AND health <= 0). `reachable` is
// 383ba0 against this same `pos`.
struct AimSubsystem {
    eaw::vec3 pos;                 // bone pos if a bone resolved, else {0,0,0}
    bool destroyed = false;        // bVar8 → skip before any geometry
    bool reachable = false;
};

// All hoisted inputs for one FUN_140383f70 call. Offsets in comments are into the firing body / source
// objects; the caller resolves them (the firing_spawn ENV-getter pattern).
struct FiringAimInputs {
    bool      have_target_model = false;  // *(target+0x2a0)!=0 && skeleton resolved (phase-0 gate)
    eaw::vec3 target_center     = {};     // FUN_140398440(target) — phase-0/phase-3 fallback aim point
    bool      center_reachable  = false;  // FUN_140383ba0(firer, target_center) for the phase-3 lock

    // Muzzle = midpoint of two shooter bones (param_1+0x38 and the optional param_1+0x3c). Invariant
    // across candidates (same shooter); {0,0,0} when the shooter skeleton / bone1 fetch fails.
    bool      muzzle_valid     = false;   // FUN_1403858b0(firer)!=0 && bone1 (param_1+0x38) fetch ok
    eaw::vec3 muzzle_bone1     = {};
    bool      has_muzzle_bone2 = false;   // param_1+0x3c >= 0 && bone2 fetch ok
    eaw::vec3 muzzle_bone2     = {};

    float     weapon_range      = 0.0f;   // FUN_1403857d0(firer)
    bool      add_target_extent = false;  // tmpl+0xc48>0 && FUN_140373670(tmpl, owner+0x324)!=0 (PH1/2 only)
    float     target_extent     = 0.0f;   // FUN_140397780(target)

    const AimCandidate* aim_bones      = nullptr;  int aim_bone_count   = 0;  // phase 1 (target model bones)
    const AimSubsystem* subsystems     = nullptr;  int subsystem_count  = 0;  // phase 2 (0 ⇒ list ptr null)
};

struct FiringAimResult {
    eaw::vec3 aim    = {};      // the aim point written to param_2
    bool      locked = false;   // *param_4: a specific in-range point was chosen (else bare center)
};

// FUN_140383f70: resolve the aim point. Draws AT MOST ONE int from `rng` (phase 2's random start).
FiringAimResult firing_resolve_aimpoint(const FiringAimInputs& in, eaw::SimRng& rng);

// ── reusable geometry kernels (the body's shared math; exposed for unit tests) ─────────────────────────
// Muzzle midpoint: bone1 when no second bone, else componentwise (bone2 + bone1) * (1.0/2.0). Operand
// order (bone2 LEFT) and the divide-form scale preserve the binary's FP.
eaw::vec3 firing_muzzle_midpoint(const eaw::vec3& bone1, bool has_bone2, const eaw::vec3& bone2);

// In-range test: sqrt((m.x-p.x)^2 + (m.y-p.y)^2 + (m.z-p.z)^2) <= range, x→y→z accumulation order.
bool firing_point_in_range(const eaw::vec3& muzzle, const eaw::vec3& point, float range);

} // namespace sim
