// sim/ability_countdown.h — Phase-3 IN sim behavior #3: AbilityCountdownBehaviorClass::vfunc_6.
//
// Lifted from RVA 0x42f910 (decomp/42f910.c). Third of the 13 in-slice IN sim behaviors
// (decomp_plan.md Phase-3). Picked as the next candidate after EnergyPool (#2) for the same reason
// EnergyPool was picked after DamageTracking (#1): it is a self-contained, CONTINUOUSLY-observable
// Phase-A ticker (good oracle), but richer than EnergyPool —
//   * it ticks on an INTEGER tick-delta (tick - last_serviced_tick), NOT a game-speed float scale,
//     so the oracle needs no `dt` recovery at all (cleaner than DTNRG);
//   * it has TWO clamped modes (countdown floor-at-0, chargeup ceil-at-target); and
//   * it drives the channel-1 CommandSink seam with a NEW signal type (0x2c AbilityRecharged),
//     advancing the Phase-B command work past DamageTracking's lone 0x2d.
//
// ── What it is ───────────────────────────────────────────────────────────────────────────────────
// A per-entity ABILITY-COOLDOWN ticker over the entity's own recharge manager at `entity+0x1e8`.
// The manager holds a fixed array of 0x4d (77) ability slots; each slot is one of two modes:
//   mode==0  COUNTDOWN  — a cooldown ticking DOWN to 0 (e.g. an ability recharging after use);
//   mode!=0  CHARGEUP   — a meter charging UP to a per-slot target (e.g. a building/charging weapon).
// Self-contained: writes only its own manager array (+ the recharge-stamp); reads/writes no other
// entity's state directly. Its only cross-boundary acts are the two edge signals below.
//
// ── Per-tick algorithm (42f910) ──────────────────────────────────────────────────────────────────
//   0. prologue: LocomotorCommonClass::vfunc_6 (reschedule, RVA 0x4c2f70) — the shared spine
//      schedule stamp, already modelled by the entity spine; not repeated here.
//   1. mgr = entity+0x1e8; if mgr==0 → return (guarded no-op).
//      delta = tick - mgr->last_tick(+0x2c0)        // integer ticks since last service (>=0 normally)
//   2. for slot i in [0, 0x4d):
//        t = timer[i]   (mgr+0x8 + i*4)
//        if mode[i]==0  (COUNTDOWN, mgr+0x270+i == 0):
//            if t > 0:                               // only an ACTIVE cooldown ticks (edge guard)
//                t = max(t - delta, 0)               // floor at 0 (engine: if (t<1) t=0)
//                timer[i] = t
//                if t == 0 and ability_ready(owner,i):   // FUN_14039b480(owner,slot,1) gate
//                    EMIT sig 0x2c AbilityRecharged on owner+0x38, payload {vftable, slot=i}
//            // (t already 0 at entry → skipped: no re-emit; edge-triggered like DamageTracking 0x2d)
//        else  (CHARGEUP, mode[i]!=0):
//            max = target[i]   (mgr+0x13c + i*4 == timer[0x4d] past slot i)
//            if t < max:  t = min(t + delta, max); timer[i] = t     // ceil at target
//            if t == max: charge_complete(owner, i)  // FUN_14042f460(behavior, slot)
//   3. mgr->last_tick(+0x2c0) = tick                 // stamp this service tick
//
// ── The two cross-boundary acts ──────────────────────────────────────────────────────────────────
//   * COUNTDOWN→0 EMIT (sig 0x2c, kSigAbilityRecharged). Edge-triggered: fires on the exact tick a
//     slot's cooldown reaches 0 (old timer was >0, new is 0) AND the heavy availability predicate
//     FUN_14039b480(owner, slot, 1) passes. That predicate reads diplomacy/shields/parent-unit/
//     ability-specific gates (decomp/39b480.c, 1094 B) — world-coupled, modelled behind
//     AbilityCountdownEnv::ability_ready, not byte-lifted. Payload = AbilitySignalData{vftable, slot}
//     built on the emit-site stack (42f910 local_48/local_40), fired on owner+0x38 via FUN_140220ed0.
//   * CHARGEUP→target CALLBACK (FUN_14042f460, env-modelled). Invoked the tick a chargeup slot
//     reaches its target. 42f460 (410 B) is itself env-heavy: behind its own gates it emits sig 0x29
//     (kSigAbilityChargeComplete) on owner+0x38, fires a presentation SFX cue (FUN_1402d5290 = the
//     channel-2 SFX manager, NOT lockstep-relevant), and — the key observable — RECYCLES THE SLOT
//     INTO A COOLDOWN: it resets the mode byte to 0 (chargeup→countdown), resets the timer to a new
//     (larger) cooldown duration, and updates the target field. So in the live binary the slot's
//     post-tick value on a completion tick is NOT the core's clamp-to-target — it is this recycle
//     write (DTABIL oracle: completion ta always > tgt, then the next tick counts DOWN in mode 0).
//     Those interior effects are the env's responsibility; the deterministic core owns only the
//     DECISION to invoke charge_complete (timer reaches target), which is what host tests pin and
//     what the in-game oracle validates (the trigger), and the subsequent cooldown countdown is then
//     reproduced bit-exact by the core's COUNTDOWN path.
//
// ── Determinism notes ────────────────────────────────────────────────────────────────────────────
//   * No sim RNG drawn in the per-tick path. Writes only its own manager array (+ recharge stamp).
//     Fully Phase-A parallel-safe modulo the two buffered emissions, exactly like DamageTracking.
//   * INTEGER arithmetic throughout — no float, no game-speed scale. delta is a plain tick count, so
//     the in-game oracle reads timer-before/after and reproduces them with no scalar to recover.
//   * Slot iteration is a fixed 0..0x4d scan in index order; the two emits are per-slot and
//     index-independent of each other, so the per-slot order does not affect observable state.
//   * The recharge-stamp (+0x2c0) is written ONCE at the end with the current tick, so a slot that
//     was serviced this tick measures delta from the PREVIOUS service next time (catches up missed
//     ticks if the behavior was descheduled — delta can exceed 1).
#pragma once
#include "command_sink.h"
#include "eaw_types.h"

namespace sim {

// The per-entity ability-recharge manager at entity+0x1e8. Fixed 0x4d-slot arrays.
struct AbilityCountdownState {
    static constexpr int kNumSlots = 0x4d;   // 77 ability slots (the 42f910 loop bound)

    int     timer[kNumSlots]  = {};          // mgr+0x8   + i*4 — current cooldown/charge value
    int     target[kNumSlots] = {};          // mgr+0x13c + i*4 — chargeup ceiling (timer[0x4d+i])
    eaw::u8 mode[kNumSlots]   = {};           // mgr+0x270 + i   — 0 = countdown, !=0 = chargeup
    int     last_tick = 0;                    // mgr+0x2c0 — last serviced tick (delta source)
    bool    has_manager = true;               // entity+0x1e8 != 0 — the manager sub-object exists
};

// World-coupled per-slot side decisions, modelled (not byte-lifted) like LocomotorEnv/HardpointEnv.
struct AbilityCountdownEnv {
    virtual ~AbilityCountdownEnv() = default;

    // FUN_14039b480(owner, slot, 1): may this ability's recharge-complete signal be raised? A heavy
    // availability predicate (diplomacy, shields, parent unit, per-ability gates). The deterministic
    // core calls it ONLY on the tick a cooldown hits exactly 0, and emits 0x2c iff it returns true.
    virtual bool ability_ready(void* owner, int slot) = 0;

    // FUN_14042f460(behavior, slot): a chargeup slot is at its target. Env-modelled callback — its
    // interior (sig 0x29 emit, SFX cue, mode-byte reset) runs behind its own gates and is the env's
    // job; the core owns only the decision to invoke it (timer == target this tick).
    virtual void charge_complete(void* owner, int slot) = 0;
};

// AbilityCountdownBehaviorClass::vfunc_6 (RVA 0x42f910), deterministic core.
//   st     — entity+0x1e8 recharge-manager state (mutated in place)
//   owner  — the owning GameObjectClass* (= behavior+0x28; the dispatcher emitter + env subject)
//   tick   — the sim tick passed to vfunc_6 (param_3); delta = tick - st.last_tick
//   env    — per-slot availability gate + chargeup-complete callback (AbilityCountdownEnv)
//   sink   — channel-1 CommandSink; receives the 0x2c AbilityRecharged emit on a cooldown→0 edge
void ability_countdown_tick(AbilityCountdownState& st, void* owner, int tick,
                            AbilityCountdownEnv& env, CommandSink& sink);

} // namespace sim
