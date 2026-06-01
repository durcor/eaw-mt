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

// FUN_140387400 — opportunity-target scan core (lines 244–288). See hardpoint.h for the full
// semantics + the player-table layout. This is the determinism-critical, RNG-consuming heart of the
// acquisition function; the surrounding guard cascade and command emission are env-modeled.
void* scan_opportunity_target(SimRng& rng, OppScanEnv& env) {
    const int  n        = env.player_count();      // iVar4 = (end - begin) >> 3
    const int  self_id  = env.self_skip_id();      // iVar8 = self_player[+0x4c]
    const bool vis_mode = env.visibility_mode();   // cVar6 = FUN_14039aa60(context), evaluated ONCE

    // The start index is drawn UNCONDITIONALLY (binary line 250, before the `0 < iVar4` loop guard),
    // so the RNG draw count is preserved even for degenerate player counts. range_i(0,0) (n==1) is a
    // no-draw per the LCG contract; range_i(0,-1) (n==0) still draws — both matched by SimRng.
    int idx = rng.range_i(0, n - 1);               // iVar9

    void* result = nullptr;                        // lVar13/lVar15 running result (initially 0)
    for (int count = 0; n > 0 && count < n; ++count) {
        if (idx == self_id) {                      // skip the owner's own player slot
            if (++idx >= n) idx = 0;
            continue;                              // -> iVar16++ (advance, no candidate eval)
        }
        void* cand = env.player_at(idx);           // FUN_140294bc0(&DAT_140a16fd0, iVar9)
        if (!cand || (vis_mode && env.candidate_blocked(idx))) {
            continue;                              // STALL: null/fog-blocked -> count++ WITHOUT advancing idx
        }
        const CandidateEval e = env.eval_candidate(idx);
        if (e.target) result = e.target;           // commit found unit (covers accept AND the score-leak)
        if (e.accept) break;                       // first valid target wins
        if (++idx >= n) idx = 0;                   // reject (incl. score-leak): advance
    }
    return result;
}

// FUN_140387400 lines 220–316 — the autonomous opportunity-target acquisition tail + sig-0x21
// emission. See hardpoint.h for the four outcomes, the precondition, and the determinism notes.
// Variable names track the decompile (pvVar11/bVar5 = the just-cleared-slot force-rescan markers).
void acquire_opportunity_target(SimRng& rng, OppAcquireEnv& env, sim::CommandSink& sink) {
    void* pvVar11 = nullptr;   // line 223
    bool  bVar5   = false;     // line 131 (init false; set true iff an existing slot is cleared)

    // [A] Existing-slot revalidation (lines 224–234). Keep the slot iff it is NOT fog-blocked AND is
    // still fire-allowed; otherwise clear it (which forces a rescan below).
    if (env.opp_slot() != nullptr) {
        if (!env.existing_fog_blocked() && env.existing_fire_allowed())
            return;                            // [A-keep] still valid -> done, no emit
        env.clear_opp_slot();                  // FUN_1403846c0
        pvVar11 = env.opp_slot();              // == nullptr after clear
        bVar5   = true;
    }

    // [R] Rescan gate (lines 237–242). COND_A = just-cleared OR interval elapsed; the last_scan_time
    // stamp is a comma side effect under COND_A (fires even when COND_B then suppresses the scan).
    const int frame = env.frame_counter();
    const bool cond_a = (pvVar11 == nullptr && bVar5) ||
                        (env.rescan_interval() < frame - env.last_scan_time());
    if (cond_a) {
        env.set_last_scan_time(frame);         // field_0x64 = iVar8 (stamp)
        if (!env.context_blocked() && !env.context_has_active_order()) {
            void* result = scan_opportunity_target(rng, env);   // lines 243–288 (the RNG draw)
            if (result != nullptr) {                            // bind (lines 283–288)
                if (env.opp_slot() != nullptr) env.clear_opp_slot();
                env.bind_opp_slot(result);     // FUN_140382510
            }
        }
    }

    // [E] Emission decision (lines 290–315): validate the (possibly newly-bound) slot; clear it if no
    // longer fire-allowed, else emit OpportunityTargetAcquired (sig 0x21) on the resolved emitter.
    if (env.opp_slot() != nullptr) {
        if (!env.new_slot_fire_allowed()) {    // FUN_1403825b0 == 0
            env.clear_opp_slot();              // [E-clear]
        } else {
            void* emitter = nullptr;
            if (!env.resolve_emitter(emitter)) // FUN_1404ec820 mismatch for a subordinate mount
                return;                        // bail entirely (line 305)
            // Payload = OpportunityTargetAcquiredDataClass {vftable (drain-filled), opp_slot, self}.
            sim::OpportunityTargetAcquiredData d;
            d.target_slot = env.opp_slot();    // local_60 = *opp_target_slot
            d.hardpoint   = env.self_hardpoint(); // local_58 = param_1
            sink.emit_opportunity_target_acquired(emitter, d); // FUN_140220ed0(_, emitter+0x38, 0x21, &d)
        }
    }
}

// FUN_140387400 lines 173–201 — the ordered-fire commit (sig-0x20 emission). See hardpoint.h for the
// state machine and the flag-persists-on-bail subtlety. Counterpart to acquire_opportunity_target;
// the two are mutually exclusive within a call (cVar6 gate at line 221 / state_flag return at 203).
void commit_ordered_fire(OrderedFireEnv& env, sim::CommandSink& sink) {
    // cVar6 enters as the regular-target fire-allowed result computed in lines 133–171.
    bool cVar6 = env.regular_target_fire_allowed();

    // ORDERED branch (line 173): state_flag==1 guarantees order_object!=0 (lines 86–88), but the
    // decompile keeps the explicit conjunction — preserved.
    if (env.state_flag() && env.order_object() != nullptr) {
        cVar6 = env.order_fire_allowed();          // line 174 — recompute for the ordered target
        if (!cVar6) {                              // line 175 — order not allowed -> LAB_1403877f0
            env.set_in_progress(false);            // line 199 (cVar6==0)
            return;
        }
        env.clear_order_object();                  // line 176 — order committed; clear it
        // fall through to the 0->1 transition (LAB_140387791)
    } else {                                        // REGULAR branch (state_flag==0)
        if (!cVar6) {                              // line 196 false -> LAB_1403877f0
            env.set_in_progress(false);            // line 199 (cVar6==0)
            return;
        }
        // cVar6==1 -> goto LAB_140387791 (line 196)
    }

    // LAB_140387791 (lines 177–193) — the in_progress_flag 0->1 transition. Emit only on the edge.
    if (env.in_progress()) return;                 // line 178 — already in progress, no re-emit
    env.set_in_progress(true);                     // line 180 — SET before the emitter resolution
    void* emitter = nullptr;
    if (!env.resolve_emitter(emitter))             // lines 181–186 — FUN_1404ec820 subordinate mismatch
        return;                                    // line 184 goto LAB_1403877f9: skip emit, flag SET
    sink.emit_fire_order_in_progress(emitter);     // lines 191–192 — sig 0x20, parameterless
}

} // namespace eaw
