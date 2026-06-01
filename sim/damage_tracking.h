// sim/damage_tracking.h — Phase-3 IN sim behavior #1: DamageTrackingBehaviorClass::vfunc_6.
//
// Lifted from RVA 0x58bd80 (decomp/58bd80.c). This is the FIRST of the 13 in-slice sim behaviors
// (decomp_plan.md Phase-3) and the first to drive the channel-1 CommandSink seam (command_sink.h)
// with a real behavior rather than synthetic host cases.
//
// ── What it actually is (census correction) ────────────────────────────────────────────────────
// The Phase-3 behavior census tagged DamageTracking "[HP, cross-entity]". The per-tick vfunc_6 does
// NOT write another entity's HP — it is a TIMED-EFFECT-LIST DECAY TICKER over this entity's own
// damage/status effects. (Actual HP application happens when an effect is ATTACHED — a slot/connect
// path — not in the periodic update.) Its only cross-boundary action is one signal emit.
//
// ── Per-tick algorithm (58bd80) ────────────────────────────────────────────────────────────────
//   1. base prestep  LocomotorCommonClass::vfunc_6 (reschedule, RVA 0x4c2f70) — the behavior+0x30
//      schedule stamp; already modelled by the entity spine's schedule gate, not repeated here.
//   2. decay = (rate0 + rate1) * _DAT_140b168fc   where
//        rate0 = FUN_1403727a0(template@+0x298, entity)  // template+0xdcc, game-speed(b16dcc/dc8)
//                                                          // + faction(player+0x360 → +0x50) + buff
//        rate1 = FUN_1403725f0(template@+0x298, entity)  // template+0xdd4, faction(+0x54) + buff,
//                                                          // (no game-speed scale)
//      Both are world-coupled (player-diplomacy table + buff stack) → modelled behind DamageEnv,
//      same as LocomotorEnv/HardpointEnv. Their composition into a single per-tick `decay` float is
//      the only thing the deterministic core needs.
//   3. walk the DamageEffectList (entity+0x1a0): for each active effect, duration -= decay; effects
//      whose new duration is NOT > 0 are removed (unlinked from the intrusive hash list + freed).
//   4. PHASE-B: when the list transitions to EMPTY (count hits 0 inside the expiry branch), emit
//      PARAMETERLESS signal 0x2d (kSigDamageEffectsCleared) on the entity's own +0x38 dispatcher
//      (FUN_140220ed0 — the global emitter arg DAT_140a10b50 is ignored by emit, per command_sink.h).
//      Edge-triggered: fires exactly on the tick the LAST effect expires; never if the list was
//      already empty at entry, never if ≥1 effect survives.
//   5. tail: rate_readout(+0x0c) = (tick_hz * accum(+0x08)) / interval(behavior+0x34); accum := 0.
//
// ── Determinism notes ──────────────────────────────────────────────────────────────────────────
//   * The in-engine container is a hash-indexed intrusive list (the +0x10 key feeds the Park-Miller
//     `(key^0xdeadbeef)&0x7fffffff` bucket hash, identical to SignalListClass removal). The hash is
//     ONLY an O(1)-removal index; iteration follows the doubly-linked list in INSERTION order. The
//     single 0x2d emit fires iff the list becomes empty this tick, which is independent of removal
//     order — so modelling `effects` as an insertion-ordered vector is faithful to the observable.
//   * No sim RNG is drawn. No other entity's state is read or written in the periodic path; the only
//     cross-boundary effect is the recorded 0x2d command. Fully Phase-A parallel-safe (own +0x1a0
//     sub-object) modulo that one buffered emission.
#pragma once
#include "command_sink.h"
#include "eaw_types.h"
#include <vector>

namespace sim {

// One active damage/status effect = a DamageEffectList node (0x18 bytes in-engine:
// +0x00 next, +0x08 prev, +0x10 key, +0x14 duration).
struct DamageEffect {
    eaw::u32 key      = 0;     // node+0x10 — effect id; in-engine the hash key for O(1) removal only
    float    duration = 0.0f;  // node+0x14 — remaining duration (ticks down by `decay` each tick)
};

// The per-entity DamageTracking state at entity+0x1a0 (the DamageEffectList sub-object).
struct DamageTrackingState {
    std::vector<DamageEffect> effects;       // intrusive list head/tail +0x18; live count +0x20
    float damage_accum = 0.0f;               // +0x08 — accumulated-damage accumulator (filled on hit)
    float damage_rate  = 0.0f;               // +0x0c — rate readout, recomputed & written each tick
};

// World-coupled per-tick decay rate, FUN_1403727a0 + FUN_1403725f0 summed × _DAT_140b168fc.
// Modelled, not byte-lifted (player-table + buff-stack queries). The deterministic core takes the
// resolved scalar; this interface exists for wiring into the spine.
struct DamageEnv {
    virtual ~DamageEnv() = default;
    virtual float decay_per_tick(void* entity) = 0;   // (3727a0 + 3725f0) * _DAT_140b168fc
};

// DamageTrackingBehaviorClass::vfunc_6 (RVA 0x58bd80), deterministic core.
//   st        — entity+0x1a0 DamageTracking state (mutated in place)
//   entity    — the owning GameObjectClass* (the dispatcher emitter; passed through to the sink)
//   decay     — per-tick duration decrement = (rate0+rate1)*_DAT_140b168fc  (= DamageEnv::decay_per_tick)
//   interval  — behavior+0x34 reschedule interval, the rate-readout divisor
//   tick_hz   — DAT_140b0a340 (ticks/sec; from the tick-clock unit)
//   sink      — channel-1 CommandSink; receives the 0x2d emit on the empty transition
void damage_tracking_tick(DamageTrackingState& st, void* entity,
                          float decay, int interval, float tick_hz,
                          CommandSink& sink);

} // namespace sim
