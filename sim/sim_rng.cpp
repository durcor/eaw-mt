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

} // namespace eaw
