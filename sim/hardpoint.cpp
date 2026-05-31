// Hardpoint fire-control implementation (Phase-3 unit 4). See hardpoint.h for the source map and the
// total_w cancellation note. The float op order here matches the binary so the lift can be diffed
// bit-exact against a future DTFIRE capture.
#include "hardpoint.h"

namespace eaw {

// FUN_140540070
f32 weight_scaled(const HardPoint& hp, HardpointEnv& env) {
    const f32 w = hp.weight;                  // *(hp + 0x58)
    const int mode = env.game_speed_mode();   // (**DAT_140b15418 + 0xe0)()  (no manager -> unscaled)
    if (mode == 1) return w * env.scale_fast; // _DAT_140b16dcc
    if (mode == 2) return w * env.scale_slow; // _DAT_140b16dc8
    return w;
}

// FUN_140387f50 — consume + fire.
f32 consume_and_fire(HardPoint& hp, ShipFireControl& ship, f32 share, HardpointEnv& env) {
    // Guard order mirrors the binary: owner-record active, owner match, positive budget, positive share.
    if (!hp.has_owner_record || !hp.active) return hp.budget; // *(owner_record + 0x4d) != 1
    if (!hp.owner_match)                    return hp.budget; // *(hp + 0x10) != ship
    if (hp.budget <= 0.0f)                  return hp.budget;
    if (share  <= 0.0f)                     return hp.budget;

    hp.budget -= share;          // *(hp + 0x28) -= share
    env.fire_weapon(hp);         // FUN_140386660(hp, 1, 0)
    if (hp.budget <= 0.0f)       // depleted this tick -> conditional station-level-lost message
        env.on_budget_depleted(hp, ship);
    return hp.budget;
}

// FUN_140387010 — per-hardpoint update dispatcher.
void service_hardpoint(HardPoint& hp, HardpointEnv& env, u32 tick) {
    if (!hp.has_owner_record) return;          // *(hp + 0x20) == 0
    if (!hp.has_context)      return;          // *(hp + 0x10) == 0
    if (hp.context_blocked)   return;          // context.flags (+0x3a0) & 2
    if (env.global_fire_inhibited(hp)) return; // DAT_140b15418 && FUN_14035f790()

    const u32 delta = tick - hp.last_serviced_tick; // ticks since last service
    hp.last_serviced_tick = tick;

    // Target acquisition only while the owner is in a firing motion-state (5..10) and the mount is on.
    if ((hp.motion_state - 5u) < 6u && hp.enable_flag)
        env.acquire_targets(hp, delta);        // FUN_140387400 (deferred deep sub-lift)

    // Render-gated muzzle effect (bone-index caching + FUN_140381ff0) — presentation side.
    if (hp.render_node_gate && hp.enable_flag && hp.has_context && hp.weight > 0.0f)
        env.fire_effect(hp);

    if (hp.reload_active)
        env.reload(hp, delta);                 // FUN_140387170
}

// FUN_1403a76b0 — per-ship fire-control entry.
void hardpoint_fire_budget(ShipFireControl& ship, HardpointEnv& env, u32 tick) {
    // Skip if the ship has no hardpoint vector or fire-control is disabled (flags & 0x40).
    if (ship.hardpoints.empty() || ship.hardpoints_off) return;

    // Pass 1: count active mounts (present && owner_record && owner_record->active).
    int active = 0;
    for (const HardPoint& hp : ship.hardpoints)
        if (hp.present && hp.has_owner_record && hp.active) ++active;

    if (active > 0) {
        // Pass 2: sum the game-speed-scaled weight over the active mounts.
        f32 total_w = 0.0f;
        for (const HardPoint& hp : ship.hardpoints)
            if (hp.present && hp.has_owner_record && hp.active)
                total_w += weight_scaled(hp, env);

        // Ship base fraction: min(fire_fraction + bias, 1.0).
        f32 base = env.fire_fraction(ship) + env.fraction_bias;
        if (base >= FIRE_FRACTION_CLAMP) base = FIRE_FRACTION_CLAMP;

        // Available pool, clamped to >= 0. (total_w cancels out of the per-mount shares below.)
        f32 avail = (env.fire_capacity(ship) - base) * total_w;
        if (avail <= 0.0f) avail = 0.0f;

        if (avail > 0.0f) {
            // Pass 3: hand each eligible mount its proportional share and consume it.
            for (HardPoint& hp : ship.hardpoints) {
                // Engine gate: active && (owner_record==0 || budget>0).  active already implies
                // has_owner_record, so the first disjunct is dead; preserved for faithfulness.
                if (hp.present && hp.has_owner_record && hp.active &&
                    (!hp.has_owner_record || hp.budget > 0.0f)) {
                    const f32 share = (hp.budget / total_w) * avail; // == budget * (capacity - base)
                    if (share > 0.0f)
                        consume_and_fire(hp, ship, share, env);
                }
            }
        }
    }

    // Pass 4: per-hardpoint update over every present mount (target acquisition, effects, reload).
    for (HardPoint& hp : ship.hardpoints)
        if (hp.present)
            service_hardpoint(hp, env, tick);
}

} // namespace eaw
