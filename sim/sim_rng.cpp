// sim/sim_rng.cpp — lifted global sim RNG. See sim_rng.h for the determinism contract.
#include "sim_rng.h"

namespace eaw {

// <- FUN_1401ffb40
int SimRng::range_i(int a, int b) {
    // Order so hi >= lo (mirrors the binary's `if (a <= b) swap`).
    int hi = a, lo = b;
    if (a <= b) { hi = b; lo = a; }
    if (lo == hi) {
        return lo;              // empty range — NO draw, return the value
    }

    u32 range = (u32)hi - (u32)lo;

    // mask = (2^k - 1) for the smallest k with 2^k > range, found by walking the top bit down.
    u32 bit = 0x4000;
    if (range < 0x7fff) {
        u32 t = range & 0x4000u;
        while (t == 0) { bit >>= 1; t = range & bit; }
    } else {
        range = 0x7ffe;         // cap the range; bit stays 0x4000 -> mask 0x7fff
    }
    u32 mask = bit * 2u - 1u;

    u32 r;
    do {
        u32 s = step();
        r = (s >> 10) & 0x7fffu & mask;
    } while ((int)range < (int)r);   // reject while r > range

    return (int)(r + (u32)lo);
}

// <- FUN_1401ffbb0
f32 SimRng::range_f(f32 a, f32 b) {
    f32 lo = a, hi = b;
    if (b < a) { lo = b; hi = a; }
    u32 s = step();
    f32 field = (f32)(u32)((s >> 10) & 0x7fffu);
    return (hi - lo) * (field / 32767.0f) + lo;   // 32767.0f = DAT_1408511e4
}

// <- FUN_1401ffdb0
bool SimRng::percent(int pct) {
    if (pct == 0)   return false;   // never — no draw
    if (pct != 100) {
        u32 r;
        do {
            u32 s = step();
            r = (s >> 10) & 0x7fu;
        } while (r > 100);
        if (pct <= (int)r) return false;
    }
    return true;                     // pct == 100 -> always (no draw); else draw < pct
}

// Per-entity substream seeding (see sim_rng.h). SplitMix64 finalizer, fed the tuple one word at a
// time through distinct odd multipliers so every input bit avalanches into the 32-bit seed. Pure
// unsigned arithmetic — identical result on every platform (the lockstep requirement). NOT derived
// from any binary function; this is the parallel-rewrite RNG retrofit.
static u64 splitmix64(u64 z) {
    z += 0x9e3779b97f4a7c15ull;
    z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9ull;
    z = (z ^ (z >> 27)) * 0x94d049bb133111ebull;
    return z ^ (z >> 31);
}

SimRng SimRng::substream(u32 base_seed, u32 entity_id, u32 channel, u32 tick) {
    u64 h = (u64)base_seed;
    h = splitmix64(h ^ ((u64)entity_id * 0x9e3779b97f4a7c15ull));
    h = splitmix64(h ^ ((u64)channel   * 0xc2b2ae3d27d4eb4full));
    h = splitmix64(h ^ ((u64)tick      * 0x165667b19e3779f9ull));
    return SimRng((u32)(h ^ (h >> 32)));   // fold to the 32-bit LCG state width
}

} // namespace eaw
