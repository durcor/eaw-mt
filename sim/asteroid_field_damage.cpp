// sim/asteroid_field_damage.cpp — lifted AsteroidFieldDamageBehaviorClass::vfunc_6 (RVA 0x437310).
// See asteroid_field_damage.h for the full per-tick algorithm and threading notes.
#include "asteroid_field_damage.h"

namespace sim {

// 437310.c lines 85-88: one uniform[0,1) sim-RNG draw, apply iff roll <= probability.
// range_f advances the shared seed exactly once (the determinism-contract gate draw).
bool asteroid_damage_roll(eaw::SimRng& rng, float probability) {
    float roll = rng.range_f(0.0f, 1.0f);
    return roll <= probability;
}

// 437310.c lines 89-110: stamp the last-in-field tick. Inside the scan loop the engine sets
// sub+0x10c = now for every asteroid found (line 90); if the loop found none it falls through to
// sub+0x10c = -1 (line 110). Equivalent to: now if found_any else -1.
void asteroid_field_stamp(AsteroidFieldDamageState& st, bool found_any, int now) {
    st.last_hit_tick = found_any ? now : -1;
}

AsteroidFieldTickResult asteroid_field_tick(AsteroidFieldDamageState& st, eaw::SimRng& rng,
                                            bool active, int asteroids_in_range,
                                            float probability, int now) {
    AsteroidFieldTickResult r;
    // Gate fail (mode / unit predicate / config zero) -> the binary returns 0x650000 without ever
    // touching the sub-object. Leave state untouched. 437310.c lines 40-53.
    if (!active) {
        return r;   // active=false, state unchanged
    }
    r.active    = true;
    r.found_any = asteroids_in_range > 0;
    r.rolled    = asteroids_in_range;
    // One gate roll per in-range asteroid, in scan order — advances the RNG exactly `n` times.
    for (int i = 0; i < asteroids_in_range; ++i) {
        if (asteroid_damage_roll(rng, probability)) {
            ++r.applied;   // env would call FUN_140436920(self, asteroid, amount) here
        }
    }
    asteroid_field_stamp(st, r.found_any, now);
    return r;
}

} // namespace sim
