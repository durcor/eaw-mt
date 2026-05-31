// Entity-update spine — lifted from StarWarsG.exe (Phase-3 unit 2).
//
// Source functions:
//   FUN_1403639d0  sim-frame update — calls the GOM update on TWO managers (param_1[3], param_1[4])
//                  with the same tick. We lift only the spine entry (gom_update); the rest of
//                  3639d0 is mode dispatch + non-spine work.
//   FUN_1402be640  GameObjectManager::Update — the master-list per-entity pass (+ second passes,
//                  deferred-spawn drain, game-mode housekeeping)            -> gom_update()
//   FUN_1403a6b80  per-GameObject update — the behavior-dispatch driver    -> update_game_object()
//
// This unit captures the spine's *control structure* — the GOM ReferenceList traversal, the
// per-entity behavior-dispatch loop, the schedule/enable gates, the mode-flag fast path, and the
// intra-tick ordering — while leaving the per-unit bodies (locomotor integrate, hardpoint
// fire-control, timed-action payloads, regen RNG) as hooks to be filled by later units. The
// behavior dispatch loop is **the loop the parallel rewrite parallelizes over entities** (Phase A).
//
// CORRECTION vs. earlier notes: FUN_1403a6b80 does NOT self-recurse — there is no child-unit
// recursion. The GOM master list is flat; every unit (including contained ones) is a separate list
// entry iterated independently. (This is strictly better for parallelization.)
//
// CONFIRMATION: in FUN_1403a6b80, entity+0x60/+0x68 is a DynamicVector of 0x38-stride timed-action
// records (countdown at +0xc), NOT a std::string — independently confirming that the +0x60
// std::string seen elsewhere belongs to the HardPoint owner record, not GameObjectClass.
#pragma once
#include "eaw_types.h"
#include <vector>

namespace eaw {

struct GameObject;

// GameObjectClass.flags (+0x3a0) bits the spine branches on.
enum : u32 {
    GO_FLAG_TRANSIENT_8       = 0x00000008, // gates the heavy "block 1" (locomotor + behaviors)
    GO_FLAG_HAS_TRANSFORM     = 0x00000100, // runs DynamicTransformBehavior (presentation, out)
    GO_FLAG_HARDPOINTS_OFF    = 0x00000040, // disables hardpoint fire-control + regen
    GO_FLAG_REGEN_ELIGIBLE    = 0x00040000, // eligible for the tail shield/regen roll
};

// A behavior in the entity's +0x278 array (count at +0x290). Each concrete BehaviorClass overrides
// vtable slot 6 (vtable+0x30) = Update(entity, tick). The 13 IN-slice behaviors are lifted later;
// here they are the abstract dispatch target.
struct Behavior {
    virtual ~Behavior() = default;
    u32 next_service_tick = 0;          // +0x30 — serviced only when this <= tick
    // +0x3c == 0 means enabled (FUN_1404c3700). Modeled as a predicate so subclasses can vary it.
    virtual bool enabled() const = 0;
    // vtable slot 6: BehaviorClass::Update(entity, tick).
    virtual void update(GameObject& entity, u32 tick) = 0;
};

// entity+0x60..+0x68 vector element (0x38 stride): a delayed action that fires when its countdown
// reaches 0, then is erased.  +0x00 = action id, +0xc = countdown.
struct TimedAction {
    u32 id       = 0;   // +0x00
    i32 countdown = 0;  // +0xc
};

// Per-unit work invoked by the driver in a fixed intra-tick order. Default no-ops; later units
// (locomotor, hardpoint, regen) supply real bodies. Keeping them here pins the ORDER, which is
// determinism-relevant.
struct EntityUpdateHooks {
    virtual ~EntityUpdateHooks() = default;
    virtual void locomotor_prestep(GameObject& /*e*/) {}                  // 3a6b80 lines 40-77 (unit 3)
    virtual void fire_timed_action(GameObject& /*e*/, const TimedAction&) {} // FUN_1403a9e30
    virtual void hardpoint_service(GameObject& /*e*/, u32 /*tick*/) {}    // FUN_1403a76b0 (unit 4)
    virtual void regen_roll(GameObject& /*e*/, u32 /*tick*/) {}           // tail FUN_1403ab890 (RNG)
};

// Sim-spine view of GameObjectClass (only the fields the spine reads).
struct GameObject {
    u32 flags = 0;                         // +0x3a0
    std::vector<Behavior*> behaviors;      // +0x278 array / +0x290 count
    std::vector<TimedAction> timed_actions;// +0x60..+0x68 (per-entity delayed-action queue)
    bool has_locomotor = false;            // +0xa8 (locomotor_state) != 0
    bool has_hardpoints = false;           // +0x2d0 (hardpoints vector) != 0
    // *(template@+0x298 +0x69) != 0 — when set together with GO_FLAG_TRANSIENT_8, block 1 is skipped.
    bool template_blocks_update = false;
};

// A GameObjectManager's update lists. Original: ReferenceListClass<GameObjectClass> intrusive lists
// (master @GOM+0xe8 sentinel/head, second @+0x130) walked head->sentinel with
// entity = *(node+0x18) - 0x18. Iteration ORDER is the determinism-relevant property; modeled as a
// vector preserving that order.
struct GameObjectManager {
    std::vector<GameObject*> master_update_list; // +0xe8 — the per-tick update list
    std::vector<GameObject*> second_update_list; // +0x130 — dynamic-transform pass (tick-preserving)
    // lightweight_mode = the param_3 forwarded to each per-entity update. Original cVar4:
    // ((*mode->vtable[0xe0])() == 0 && DAT_140b1540c > 0). Carried per GOM update call.
    bool lightweight_mode = false;
};

// FUN_1403a6b80 — per-GameObject update driver. `mode_flag` is the forwarded param_3.
void update_game_object(GameObject& e, u32 tick, bool mode_flag, EntityUpdateHooks* hooks);

// FUN_1402be640 (master-list pass) — iterate the GOM update list, driving each entity. The full
// function also runs a second per-entity post-pass, the second_update_list transform pass, the
// deferred-spawn drain, and game-mode housekeeping; those are out of the spine slice (callouts).
void gom_update(GameObjectManager& gom, u32 tick, EntityUpdateHooks* hooks);

// FUN_1403639d0 (spine entry) — the sim frame updates TWO managers with the same tick.
void sim_frame_update(GameObjectManager& gom_a, GameObjectManager& gom_b, u32 tick,
                      EntityUpdateHooks* hooks);

} // namespace eaw
