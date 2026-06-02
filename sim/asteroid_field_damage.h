// sim/asteroid_field_damage.h — Phase-3 IN sim behavior #6: AsteroidFieldDamageBehaviorClass::vfunc_6.
//
// Lifted from RVA 0x437310 (decomp/437310.c). Sixth of the 13 in-slice IN sim behaviors
// (decomp_plan.md Phase-3), and the SIBLING of Nebula #5 (0x437b60, adjacent RVA): same component
// slot at entity+0xf0, same "last-event tick" field pattern (here +0x10c, like Nebula's +0x104
// enter_tick). Where Nebula is a smooth float integrator, AsteroidFieldDamage is a PROBABILISTIC
// per-tick DAMAGE TRIGGER: while a ship sits inside an asteroid field, every tick it has a fixed
// chance to take a chunk of collision damage (and spawn the impact effect).
//
// ── Per-tick algorithm (437310) ──────────────────────────────────────────────────────────────────
//   GATE (437310.c lines 40-47): the behavior is live only when
//     * the entity has a sub-object (entity+0xf0 != 0),
//     * the current game mode FUN_140397900(owner) ∈ {1, 2, 0x20} (skirmish / campaign-space etc.),
//     * a player/unit predicate chain passes (FUN at vtbl+0x1a8/+0x1b8/+0x2b0 on the active unit),
//     * and both config scalars are non-zero: DAT_140b16d64 (per-tick hit PROBABILITY) and
//       DAT_140b16d60 (damage AMOUNT). If any fails, the behavior is a no-op (state untouched).
//
//   SCAN + ROLL (437310.c lines 55-108): a sphere query (FUN_1405d7c00) over the global object set
//     gathers objects near the ship's position (param_2+0x78/+0x7c xy, +0x84 transform, radius from
//     FUN_14041c000). The loop walks the hit list; for each object that carries the ASTEROID-FIELD
//     flag (object->template+0x298 deref, byte +0x54):
//        roll = rng_uniform(seed, 0.0, 1.0)              // FUN_1401ffbb0 — ONE sim-RNG draw per asteroid
//        if (roll <= probability)  apply_damage(...)     // FUN_140436920 — env: damage self + spawn fx
//        found_any = true
//        last_hit_tick = now                             // sub+0x10c stamped to the current sim tick
//     After the loop: found_any -> +0x10c already == now; else +0x10c = -1 (left/never-entered field).
//     The behavior always returns 0x650000 (the spine's "handled" sentinel).
//
//   apply_damage = FUN_140436920: applies the collision damage to the SHIP ITSELF (param_2) at a
//     randomly-chosen hardpoint (it draws a second float roll FUN_1401ffbb0 + an int roll
//     FUN_1401ffb40 to pick the impact point), and spawns a damage-effect entity into the GOM
//     (FUN_14029f810 on the object manager DAT_140b15418+0x18). The deterministic core only reports
//     the apply DECISION; the damage + spawn + its extra RNG draws are the env seam.
//
// ── Determinism / threading notes ────────────────────────────────────────────────────────────────
//   * The gate roll is fully deterministic given (seed, probability): it is exactly the sim LCG
//     (sim_rng.h) range_f(0,1). The per-tick gate-roll COUNT equals the number of in-range asteroid
//     objects, so it is data-dependent on neighbor membership — the determinism contract requires
//     both the recurrence AND that draw count to match (see sim_rng.h).
//   * NOT Phase-A parallel-safe — but for a DIFFERENT reason than UnitAI #4. UnitAI writes a FOREIGN
//     gameplay structure (the shared sensor grid). Here the gameplay write (damage) lands on SELF;
//     the parallel hazards are instead (a) the SHARED sim-RNG seed DAT_140a13e24 is advanced a
//     membership-dependent number of times, so draw ORDER across entities must be serialised, and
//     (b) apply_damage spawns an effect entity into the global GOM. Both route through Phase-B seams
//     (the RNG must be drawn in GOM tick order; the spawn through the CommandSink). The spatial scan
//     reads the live global object set, so it also needs a stable snapshot mid-sweep (as Nebula #5).
//   * Contrast the siblings: Nebula #5 is READ-ONLY cross-entity (writes only its own sub-object) and
//     uses NO RNG; AsteroidFieldDamage shares Nebula's +0xf0/last-tick skeleton but is the first IN
//     behavior whose non-safety is purely the shared-RNG + GOM-spawn seam.
#pragma once
#include "sim_rng.h"

namespace sim {

// Per-entity asteroid-field state. Lives on the sub-object at entity+0xf0 (the Nebula/EnergyPool
// component slot). Only one field participates in the deterministic core.
struct AsteroidFieldDamageState {
    int last_hit_tick = -1;   // sub+0x10c — sim tick of the most recent in-field tick, or -1 if clear
};

// One in-range asteroid-field object: draw a uniform[0,1) roll and decide whether the env applies
// the collision damage (FUN_140436920). Advances `rng` exactly once (the gate draw). 437310.c:85-88.
//   roll = rng.range_f(0.0f, 1.0f);  return roll <= probability;
bool asteroid_damage_roll(eaw::SimRng& rng, float probability);

// STAGE — proximity stamp (437310.c lines 89-110): set last_hit_tick = now if any asteroid was found
// in range this tick, else -1. Mutates st.last_hit_tick.
void asteroid_field_stamp(AsteroidFieldDamageState& st, bool found_any, int now);

// Result of one full per-tick behavior.
struct AsteroidFieldTickResult {
    bool active    = false;   // gate (mode + unit predicate + config non-zero) passed -> scan ran
    bool found_any = false;   // >=1 asteroid-field object was in range this tick
    int  rolled    = 0;       // sim-RNG gate draws made (== in-range asteroid count)
    int  applied   = 0;       // rolls that passed (env applied damage that many times)
};

// Full per-tick behavior. When `active` is false (gate failed) the state is left UNTOUCHED and the
// result reports active=false. When active, it draws `asteroids_in_range` gate rolls (advancing rng),
// counts the applies, and stamps last_hit_tick (now if found_any, else -1). 437310.c lines 40-113.
AsteroidFieldTickResult asteroid_field_tick(AsteroidFieldDamageState& st, eaw::SimRng& rng,
                                            bool active, int asteroids_in_range,
                                            float probability, int now);

// World-coupled per-tick inputs, modelled (not byte-lifted): the activation gate, the sphere-scan
// asteroid count, and the damage application (which itself advances the RNG + spawns into the GOM).
// Exists for wiring into the spine; the deterministic core above takes the resolved values.
struct AsteroidFieldEnv {
    virtual ~AsteroidFieldEnv() = default;
    virtual bool active(void* entity) = 0;                 // 437310.c gate (mode + predicate + config)
    virtual int  asteroids_in_range(void* entity) = 0;     // sphere scan -> asteroid-flag object count
    virtual void apply_damage(void* self, int asteroid_index) = 0;  // FUN_140436920 (self dmg + spawn)
};

} // namespace sim
