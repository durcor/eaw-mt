// Entity-update spine — implementation. See entity_spine.h for the source-function map.
#include "entity_spine.h"

namespace eaw {

// The per-entity delayed-action queue drain (FUN_1403a6b80 lines 82-131). Each element counts down;
// when it reaches 0 it fires and is erased (the original compacts the vector in place). The drain
// order (front-to-back) and fire-on-zero semantics are determinism-relevant.
static void drain_timed_actions(GameObject& e, EntityUpdateHooks* hooks) {
    for (std::size_t i = 0; i < e.timed_actions.size();) {
        TimedAction& a = e.timed_actions[i];
        if (a.countdown < 1) { ++i; continue; }   // inactive: leave in place
        a.countdown -= 1;                          // decrement
        if (a.countdown > 0) { ++i; continue; }    // still waiting
        // reached 0: fire, then erase (compaction shifts the next element into slot i).
        if (hooks) hooks->fire_timed_action(e, a);
        e.timed_actions.erase(e.timed_actions.begin() + static_cast<std::ptrdiff_t>(i));
    }
}

// FUN_1403a6b80
void update_game_object(GameObject& e, u32 tick, bool mode_flag, EntityUpdateHooks* hooks) {
    if (!mode_flag) {
        // --- Block 1: locomotor pre-step + per-entity timers + BEHAVIOR DISPATCH ---
        // Skipped only when the transient flag is set AND the template marks the unit as not
        // needing a full update (original: (flags&8)==0 || *(template+0x69)==0).
        const bool run_block1 =
            (e.flags & GO_FLAG_TRANSIENT_8) == 0 || !e.template_blocks_update;
        if (run_block1) {
            if (e.has_locomotor && hooks) hooks->locomotor_prestep(e);

            drain_timed_actions(e, hooks);

            // *** The behavior dispatch loop — iterate the +0x278 array in REVERSE, gating each on
            // schedule (+0x30 <= tick) and enabled (+0x3c == 0). The original masks the index to a
            // byte; behavior counts are small. This is the Phase-A parallelizable body. ***
            for (i32 i = static_cast<i32>(e.behaviors.size()) - 1; i >= 0; --i) {
                Behavior* b = e.behaviors[static_cast<std::size_t>(static_cast<u8>(i))];
                if (b->next_service_tick <= tick && b->enabled())
                    b->update(e, tick);
            }
            // (presentation/scene/audio/lua sub-blocks omitted here — render-side or other slices.)
        }

        // --- Block 4: hardpoint fire-control (runs whenever mode_flag is clear) ---
        if (e.has_hardpoints && (e.flags & GO_FLAG_HARDPOINTS_OFF) == 0 && hooks)
            hooks->hardpoint_service(e, tick);
    }

    // --- Tail: shield/regen roll (runs in BOTH mode_flag paths) ---
    if ((e.flags & GO_FLAG_REGEN_ELIGIBLE) != 0 &&
        (e.flags & GO_FLAG_HARDPOINTS_OFF) == 0 && hooks)
        hooks->regen_roll(e, tick);
}

// FUN_1402be640 (spine core: the master-list per-entity pass)
void gom_update(GameObjectManager& gom, u32 tick, EntityUpdateHooks* hooks) {
    // Master update list: drive every entity in list order.
    for (GameObject* e : gom.master_update_list)
        update_game_object(*e, tick, gom.lightweight_mode, hooks);

    // Out-of-spine callouts in the original FUN_1402be640, kept here as documented ordering anchors:
    //   1. a second per-entity post-pass: entity->vtable[0x10](entity, 3) -> FUN_1403fc750.
    //   2. the second_update_list pass: DynamicTransformBehaviorClass::FUN_1403ac530(entity, 0),
    //      run with the sim tick (DAT_140b0a320) saved/restored so it does NOT advance the clock.
    //   3. deferred-spawn drain over the creation-params vector (entries due this tick).
    //   4. game-mode / victory housekeeping.
    // These are lifted with their owning units; the spine only guarantees the master pass + order.
}

// FUN_1403639d0 (spine entry)
void sim_frame_update(GameObjectManager& gom_a, GameObjectManager& gom_b, u32 tick,
                      EntityUpdateHooks* hooks) {
    gom_update(gom_a, tick, hooks);   // FUN_1402be640(param_1[3], tick)
    gom_update(gom_b, tick, hooks);   // FUN_1402be640(param_1[4], tick)
}

} // namespace eaw
