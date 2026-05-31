// Hardpoint fire-control — lifted from StarWarsG.exe (Phase-3 unit 4).
//
// Each GameObject (ship) owns a DynamicVectorClass<HardPointClass*> at entity+0x2d0. Once per tick
// the entity-update spine (FUN_1403a6b80) drives FUN_1403a76b0 over that vector. a76b0 does two
// things, in order:
//   (1) FIRE-BUDGET DISTRIBUTION — sum a per-hardpoint weight across the active mounts, derive a
//       ship-level "available pool", and hand each mount a proportional share (consuming it and
//       triggering a shot).  This is the deterministic, value-producing core, and it is what the
//       DIFFTRACE per-tick hash already observes (it folds each hardpoint's budget @+0x28 and
//       weight @+0x58).
//   (2) PER-HARDPOINT UPDATE — call FUN_140387010 on every mount: a gating dispatcher that, when the
//       owner is in a firing motion-state, runs the deep opportunity-target acquisition
//       (FUN_140387400, 1904 bytes — cross-entity target writes + a player-table RNG scan + command
//       emission). That body is Phase-A/Phase-B-entangled and is left as a sub-lift behind the Env.
//
// Source functions:
//   FUN_1403a76b0  per-ship fire-budget distribution + per-hardpoint update   -> hardpoint_fire_budget()
//   FUN_140387f50  consume a share from one mount + fire (+ depletion event)   -> consume_and_fire()
//   FUN_140540070  game-speed-scaled per-hardpoint weight (+0x58)              -> weight_scaled()
//   FUN_140387010  per-hardpoint update dispatcher (delta + motion gate)       -> service_hardpoint()
//   FUN_140396df0  ship "fire fraction" scalar                                 -> HardpointEnv::fire_fraction
//   FUN_140396070  ship "fire capacity" scalar                                 -> HardpointEnv::fire_capacity
//   FUN_140387400  opportunity-target acquisition (deferred sub-lift)          -> HardpointEnv::acquire_targets
//   FUN_140386660  the fire action                                             -> HardpointEnv::fire_weapon
//
// KEY FINDING — total_w cancels.  The engine computes
//     avail   = (capacity - base) * total_w          // one multiply
//     share_i = (budget_i / total_w) * avail         // a divide then a multiply
// so algebraically share_i = budget_i * (capacity - base): the summed weight total_w (and hence the
// game-speed weight scale FUN_140540070 applies) CANCELS OUT of every share. The weight path matters
// only as a *gate* — avail > 0 requires total_w > 0, i.e. at least one active mount with weight > 0.
// (Same shape as the locomotor difficulty-scale cancellation.) We still evaluate the expression in
// the engine's exact op order below, so the lift stays bit-faithful for a future DTFIRE oracle.
#pragma once
#include "eaw_types.h"
#include <vector>

namespace eaw {

// DAT_1407ffaf8 = 1.0f — the upper clamp on the ship base fraction (FUN_1403a76b0 line 50).
inline constexpr f32 FIRE_FRACTION_CLAMP = 1.0f;

// HardPointClass view (entity+0x2d0[i]) — only the fields the fire-control path reads.
// The +0x20 "owner record" is the non-polymorphic HardPointOwnerRecord; `active` is its +0x4d flag
// and `motion_state` its +0x48 field. `context` (+0x10) is the owning ship (the consume-side owner
// match in FUN_140387f50). `budget` (+0x28) is consumed by the distribution; `weight` (+0x58) drives
// the (cancelling) total. Dispatcher fields back FUN_140387010's gates.
struct HardPoint {
    int  id = 0;                  // test/bookkeeping handle (not an engine field)
    // --- distribution (a76b0 / 387f50) ---
    bool present = true;          // the slot pointer in the ship's vector is non-null
    bool has_owner_record = true; // +0x20 != 0
    bool active = false;          // owner_record->active (+0x4d) == 1
    f32  budget = 0.0f;           // +0x28 — per-tick fire budget (consumed; observed by DIFFTRACE)
    f32  weight = 0.0f;           // +0x58 — distribution weight (game-speed scaled by 540070)
    bool owner_match = true;      // context (+0x10) == ship  (387f50 owner-match guard)
    // --- per-hardpoint update dispatcher (387010) ---
    bool has_context = true;      // +0x10 != 0
    bool context_blocked = false; // context.flags (+0x3a0) & 2
    u32  motion_state = 0;        // owner_record->motion_state (+0x48)
    bool enable_flag = false;     // +0x6c
    u32  last_serviced_tick = 0;  // +0x60
    bool render_node_gate = false;// owner_record->render_node_gate (+0x4e) == 1
    bool reload_active = false;   // +0x6f == 1
};

// GameObjectClass fire-control view (the ship owning the hardpoint vector).
struct ShipFireControl {
    std::vector<HardPoint> hardpoints; // entity+0x2d0 (DynamicVectorClass<HardPointClass*>)
    bool hardpoints_off = false;       // entity.flags (+0x3a0) & 0x40 — skip the whole path
};

// Deferred / world-coupled callouts of the fire-control path. The deterministic distribution
// arithmetic stays in hardpoint.cpp; everything that needs the wider game state (the ship scalars,
// the actual shot, target acquisition, presentation) is injected here — to be lifted as its own
// sub-units, exactly as LocomotorEnv stages the deep locomotor physics.
struct HardpointEnv {
    virtual ~HardpointEnv() = default;

    // FUN_140540070 game-speed weight scale.  mode (from (**DAT_140b15418+0xe0)()) 1 -> *scale_fast,
    // 2 -> *scale_slow, else (incl. no manager) unscaled.  scale_* are RUNTIME-loaded (the static
    // image bytes are unmapped, like the locomotor speed-mode multipliers) so they are Env inputs.
    f32 scale_fast = 1.0f;   // _DAT_140b16dcc
    f32 scale_slow = 1.0f;   // _DAT_140b16dc8
    virtual int game_speed_mode() { return 0; }      // 0/3 -> unscaled, 1 -> fast, 2 -> slow

    // Ship-level scalars (deferred sub-lifts).
    f32 fraction_bias = 0.0f;                         // _DAT_140b16d78 (runtime), added to fire_fraction
    virtual f32 fire_fraction(ShipFireControl&) { return 0.0f; } // FUN_140396df0
    virtual f32 fire_capacity(ShipFireControl&) { return 0.0f; } // FUN_140396070

    // Consume-side effects (FUN_140387f50).
    virtual void fire_weapon(HardPoint&) {}                          // FUN_140386660 — the shot
    virtual void on_budget_depleted(HardPoint&, ShipFireControl&) {} // station-level-lost (Phase-B command)

    // Per-hardpoint update sub-actions (FUN_140387010 callees, deferred).
    virtual bool global_fire_inhibited(HardPoint&) { return false; } // DAT_140b15418 && FUN_14035f790()
    virtual void acquire_targets(HardPoint&, u32 /*delta*/) {}       // FUN_140387400 (the deep sub-lift)
    virtual void fire_effect(HardPoint&) {}                          // bone-cache + FUN_140381ff0 (presentation)
    virtual void reload(HardPoint&, u32 /*delta*/) {}                // FUN_140387170
};

// FUN_140540070 — the game-speed-scaled per-hardpoint weight (+0x58).
f32 weight_scaled(const HardPoint& hp, HardpointEnv& env);

// FUN_140387f50 — consume `share` from one mount's budget and fire; on depletion emit the level-lost
// command. Returns the post-consume budget. Mirrors the engine's guard order.
f32 consume_and_fire(HardPoint& hp, ShipFireControl& ship, f32 share, HardpointEnv& env);

// FUN_140387010 — per-hardpoint update dispatcher: compute the service delta, gate, and dispatch the
// (deferred) sub-actions. Pins the determinism-relevant delta semantics + motion-state gate.
void service_hardpoint(HardPoint& hp, HardpointEnv& env, u32 tick);

// FUN_1403a76b0 — the per-ship fire-control entry: distribute the fire budget, then update each mount.
void hardpoint_fire_budget(ShipFireControl& ship, HardpointEnv& env, u32 tick);

} // namespace eaw
