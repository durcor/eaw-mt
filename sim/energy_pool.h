// sim/energy_pool.h — Phase-3 IN sim behavior #2: EnergyPoolBehaviorClass::vfunc_6.
//
// Lifted from RVA 0x56c030 (decomp/56c030.c). Second of the 13 in-slice IN sim behaviors
// (decomp_plan.md Phase-3). Unlike DamageTracking (#1), this one is a pure self-contained
// Phase-A ticker: it writes ONLY its own energy-pool value (and an own presentation flag),
// draws no sim RNG, reads/writes no other entity, and drives no CommandSink seam. That makes it
// the cleanest possible in-game oracle target — energy pools regenerate every tick of combat, so
// the per-tick state change is CONTINUOUSLY observable (no DamageTracking-style dormancy where the
// only cross-boundary act fires only at death).
//
// ── Per-tick algorithm (56c030) ──────────────────────────────────────────────────────────────────
//   0. prologue: FUN_1404c3710 (interval refresh behavior+0x34 = vtable[5]()) + base prestep
//      LocomotorCommonClass::vfunc_6 (reschedule, RVA 0x4c2f70) — both shared spine machinery
//      already modelled by the entity spine's schedule gate; not repeated here.
//   1. DRAIN GATE (only if the unit type has a drain source, template+0x8b != 0):
//        src = manager-lookup(DAT_140b15418 vtable[0x220], entity+0x58)   // e.g. an active ability
//        if src == 0          -> return NoDrainSource (0x80650001); no write, no ui flag.
//        if drain-predicate(src) == false (FUN_140527520, a BitwiseAnd<bool> over src+0x198):
//                              -> FORCE-DRAIN: value = min(max_energy, 0) = 0  (max is always >=0,
//                                 so this zeroes the pool); set ui flag; done.
//        else (predicate true) -> fall through to normal regen.
//   2. NORMAL REGEN (no drain capability, or drain predicate true):
//        guards: template present, pool present (entity+0xf0), pool not disabled (pool+0x130 == 0);
//                any guard failing -> return Ok with NO write and NO ui flag.
//        value' = clamp( value + regen_rate * dt , 0 , max_energy )
//          value      = pool+0xf8   (current energy)
//          regen_rate = template+0xddc
//          dt         = FUN_140398010(entity, 4)   // per-tick game-speed/difficulty time-scale
//          max_energy = FUN_140372320(template, entity) = base(template+0xdd0)*(1 + Σ buff modifiers)
//        write pool+0xf8 = value'; set ui flag.
//   3. UI FLAG (reached only after a regen or drain WRITE, 56c030 LAB_14056c180): if this object is
//      the locally-selected one (entity+0x50 == DAT_140a286f0) set entity+0x3a0 |= 1 — a presentation
//      "stat changed, refresh the UI bar" dirty bit, NOT lockstep-relevant.
//
// ── Determinism notes ──────────────────────────────────────────────────────────────────────────
//   * No sim RNG drawn. No other entity read or written. Writes only own pool+0xf8 (+ own UI flag).
//     Fully Phase-A parallel-safe; no Phase-B command at all (contrast DamageTracking's 0x2d emit).
//   * The three world-coupled inputs (dt time-scale FUN_140398010, max_energy FUN_140372320, and the
//     drain disposition) are modelled behind EnergyEnv exactly as LocomotorEnv/HardpointEnv/DamageEnv.
//     The deterministic core needs only the resolved scalars + disposition.
//   * Float op order is faithful: new = current + rate*dt FIRST, then clamp to [0, max]; the max used
//     for the clamp is a SECOND max_energy query (56c030:56), separate from any drain-branch query.
//   * The game-speed time-scale FUN_140398010 is the SAME difficulty-multiplier family seen in the
//     locomotor and hardpoint lifts; recovering its exact value in-game is the oracle's job (the
//     oracle infers dt = (value' - value)/regen_rate from the captured before/after, like DTDMG).
#pragma once
#include "eaw_types.h"

namespace sim {

// Resolved drain disposition for this tick (the template+0x8b gate + source lookup + predicate,
// all world-coupled, collapsed to one enum the deterministic core switches on).
enum class DrainDisposition {
    NoCapability,   // template+0x8b == 0 — unit type has no drain source → straight to regen
    SourceMissing,  // drain source lookup returned null → early bail (NoDrainSource), no write
    Draining,       // drain predicate FALSE → force the pool to 0 this tick
    NotDraining,    // drain predicate TRUE → behaves as normal regen
};

// Status code the behavior returns (the engine's HRESULT-like values).
enum class EnergyResult {
    Ok,            // 0x650000 — the normal return (regen done, drained, or a guarded no-op)
    NoDrainSource, // 0x80650001 — drain-capable unit whose source object is missing this tick
};

// The per-entity EnergyPool state. `value` lives at (entity+0xf0)+0xf8; the booleans capture the
// guard fields the per-tick path reads.
struct EnergyPoolState {
    float value    = 0.0f;   // pool+0xf8 — current energy (the only field this behavior mutates)
    bool  has_pool = true;   // entity+0xf0 != 0 — the energy-pool sub-object exists
    bool  disabled = false;  // pool+0x130 != 0 — regen suppressed (e.g. powered-down / ionized)
    bool  ui_dirty = false;  // entity+0x3a0 bit 0 — presentation refresh flag (set on a write)
};

// EnergyPoolBehaviorClass::vfunc_6 (RVA 0x56c030), deterministic core.
//   st             — entity energy-pool state (mutated in place)
//   drain          — resolved drain disposition (EnergyEnv)
//   regen_rate     — template+0xddc, energy regenerated per unit time
//   dt             — FUN_140398010(entity,4) per-tick game-speed/difficulty time-scale (EnergyEnv)
//   max_energy     — FUN_140372320(template,entity) = base*(1+Σbuffs) (EnergyEnv)
//   local_selected — entity+0x50 == DAT_140a286f0 (this object is the UI-selected one; presentation)
// Returns the engine status code; mutates st.value / st.ui_dirty per the algorithm above.
EnergyResult energy_pool_tick(EnergyPoolState& st,
                              DrainDisposition drain,
                              float regen_rate, float dt, float max_energy,
                              bool local_selected);

// World-coupled per-tick inputs, modelled (not byte-lifted): drain disposition + time-scale + max.
// Exists for wiring into the spine; the deterministic core above takes the resolved scalars.
struct EnergyEnv {
    virtual ~EnergyEnv() = default;
    virtual DrainDisposition drain_disposition(void* entity) = 0; // template+0x8b gate + src + predicate
    virtual float time_scale(void* entity) = 0;                   // FUN_140398010(entity, 4)
    virtual float max_energy(void* entity) = 0;                   // FUN_140372320(template, entity)
    virtual float regen_rate(void* entity) = 0;                   // template+0xddc
    virtual bool  local_selected(void* entity) = 0;               // entity+0x50 == DAT_140a286f0
};

} // namespace sim
