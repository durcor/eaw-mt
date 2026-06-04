// sim/sim_rng.h — the global sim RNG (Phase-3, determinism primitive).
//
// Every non-deterministic sim decision draws from ONE shared 32-bit linear congruential
// generator whose state lives in a single global word (DAT_140a13e24 in the binary). 202 call
// sites pull from it: opportunity-target scan start index (FUN_140387400), Starship locomotor
// jitter (FUN_1406236b0), entity-spine perturbations (FUN_1403a6b80), path-validity sampling
// (FUN_1403825b0), and so on. They are serviced in GOM tick order, so the *sequence* of draws —
// and the exact *number* of state advances each call makes — is the determinism contract for
// MP lockstep and replay. Reproducing the recurrence is necessary but not sufficient; the draw
// COUNT per call (rejection loops below) must match too.
//
// Core recurrence (<- FUN_1401ffb40 / FUN_1401ffbb0 / FUN_1401ffdb0):
//     s' = s * 0x41c64e6d + 0xbdf   (mod 2^32)
// Output bits are taken from  s' >> 10.  (0x41c64e6d is the ANSI-C LCG multiplier; the
// increment 0xbdf and the >>10 output tap are EaW-specific.)
#pragma once
#include "eaw_types.h"

namespace eaw {

struct SimRng {
    u32 state;

    explicit SimRng(u32 seed = 0) : state(seed) {}

    // One LCG step. Returns the NEW state (the draw functions read s' >> 10).
    u32 step() {
        state = state * 0x41c64e6du + 0xbdfu;
        return state;
    }

    // FUN_1401ffb40(state, a, b): uniform int in [min(a,b), max(a,b)].
    // Power-of-two-mask rejection sampler over the 15-bit field (s>>10)&0x7fff: pick the
    // smallest (2^k - 1) mask covering the range, draw, reject while draw > range. Range is
    // capped at 0x7ffe (32766); a == b returns immediately WITHOUT advancing the state.
    int range_i(int a, int b);

    // FUN_1401ffbb0(state, a, b): uniform float in [min(a,b), max(a,b)].
    // SINGLE draw, no rejection:  (hi-lo) * ((s>>10)&0x7fff)/32767.0f + lo.  Computed in f32 in
    // the binary's operation order (subtract, divide, multiply, add) for bit-exactness.
    f32 range_f(f32 a, f32 b);

    // FUN_1401ffdb0(state, pct): percent-chance roll.
    // pct == 0  -> false  (no draw);  pct == 100 -> true (no draw).  Otherwise draw a 7-bit value
    // (s>>10)&0x7f rejected while > 100 (uniform 0..100), return draw < pct. (Slight off-by-one:
    // 101 buckets, so P(true) ≈ pct/101, matching the engine exactly.)
    bool percent(int pct);

    // ── Per-entity substream (parallel-rewrite retrofit; NOT in the original binary) ──────────────
    // The live engine draws every sim decision from ONE global LCG word (DAT_140a13e24), so the draw
    // SEQUENCE is a lockstep invariant (I2) that forces a serial draw order. The parallel rewrite
    // removes I2 entirely by giving each GameObject its OWN deterministic stream, seeded from a hash
    // of (base_seed, entity_id, channel, tick). Each Phase-A object draws from substream(...) instead
    // of the global word, so draws need NO cross-thread ordering and the Phase-B drain never
    // serializes RNG (sim_parallel_command_schema.md §4).
    //
    // IMPORTANT: this CHANGES which numbers come out vs. the global-LCG binary — it is a determinism
    // RETROFIT, not a bit-exact lift. The contract it must honour is *reproducibility*: identical
    // (base_seed, entity_id, channel, tick) yields an identical sequence on every platform and
    // regardless of when/in-what-order substreams are created. The per-call draw semantics
    // (range_i/range_f/percent, incl. the draw COUNT) are unchanged — only the stream SOURCE moves.
    //
    // Seeding uses a SplitMix64 finalizer over the tuple so adjacent entity ids fully decorrelate
    // (re-seeding the Park-Miller LCG with sequential ids would correlate their first outputs). All
    // ops are defined unsigned integer arithmetic → bit-reproducible across compilers/architectures.
    // `channel` separates independent draw purposes for one entity (e.g. 0 = targeting, 1 = spawn
    // rolls) so they don't alias; `tick` re-keys each frame.
    static SimRng substream(u32 base_seed, u32 entity_id, u32 channel, u32 tick);
};

} // namespace eaw
