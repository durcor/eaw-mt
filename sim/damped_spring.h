// sim/damped_spring.h — the engine's shared semi-implicit DAMPED HARMONIC OSCILLATOR step.
//
// This one primitive is INLINED, instruction-for-instruction identically, into at least two of the
// per-tick behaviors:
//   * NebulaBehaviorClass::vfunc_6   (RVA 0x437b60, IN  #5) — ramps a ship's nebula-effect intensity.
//   * SelectBehaviorClass::vfunc_6   (RVA 0x3c2310, OUT)    — the UI selection-marker "pulse" scalar.
// Their spring tails disassemble to the SAME SSE sequence and the SAME four .rdata constants; the only
// difference is which object's fields hold the {value, equilibrium, velocity, freq} state. So the
// faithful representation is a single shared step both behaviors call — not two copies.
//
// ── The step (verified instruction-by-instruction against 0x3c2310 @0x3c2389-0x3c242c and the
//    byte-identical 0x437b60 @0x437b9d-0x437c56) ──────────────────────────────────────────────────
//   w     = freq * 2.0                                  // natural angular frequency (DAT_1408007d4)
//   wdt   = w * dt                                      // dt = per-tick game delta (DAT_140b0a344)
//   quad  = (wdt * 0.48 ) * wdt                         // DAT_140819c54
//   cubic = ((wdt * 0.235) * wdt) * wdt                 // DAT_140819c50
//   denom = (quad + (wdt + 1.0)) + cubic                // <-- GROUPING IS LOAD-BEARING, see below
//   inv   = 1.0 / denom                                 // 1.0 = DAT_1407ffaf8
//   dx    = value - equilibrium
//   tmp   = ((dx * w) + velocity) * dt
//   value'    = ((tmp + dx) * inv) + equilibrium
//   velocity' = (velocity - (tmp * w)) * inv
//
// ── Why the denominator grouping is written with explicit parentheses ──────────────────────────────
//   The binary computes `wdt + 1.0` as its OWN sub-sum (xmm0) and then adds the quadratic term to it:
//   `quad + (wdt + 1.0)`. A naive transcription of the flattened decompile —
//   `wdt*0.48*wdt + wdt + 1.0 + ...` — is LEFT-associative in C: `((quad + wdt) + 1.0) + ...`, a
//   DIFFERENT grouping. Float addition is non-associative, so the two disagree in the last ULP for a
//   substantial fraction of inputs (measured ~25% over the realistic wdt range). The original
//   nebula.cpp used the left-assoc form and still passed its in-game oracle — but only because
//   Nebula's actual wdt distribution is small enough that the two groupings happen to coincide on
//   every observed sample. A sibling caller (Select, whose freq/wdt range differs) can land on inputs
//   where they diverge, so the SHARED primitive is written asm-faithfully to be correct for ALL
//   callers. (Methodology: the #8-part-2 FP-add-grouping lesson — verify the add grouping against the
//   asm, never trust the decompiler's left-assoc flattening.)
//
// ── Determinism / threading ────────────────────────────────────────────────────────────────────────
//   Pure float math on the caller's own fields plus the four fixed constants and the runtime dt.
//   No RNG, no cross-entity reads, no writes outside (value, velocity). The denominator is >= 1 for
//   any wdt >= 0 (freq, dt non-negative), so inv never divides by zero. With freq == 0 the step
//   degenerates to a plain linear glide `value += velocity * dt`.
#pragma once

namespace sim {

// The four fixed constants recovered from .rdata (shared by every caller of this primitive).
constexpr float kSpringFreqScale = 2.0f;    // DAT_1408007d4 — freq -> angular frequency w
constexpr float kSpringQuad      = 0.48f;   // DAT_140819c54 — wdt^2 damping coefficient
constexpr float kSpringCubic     = 0.235f;  // DAT_140819c50 — wdt^3 damping coefficient

// One semi-implicit damped-spring step. Reads `equilibrium`, `freq`, `dt`; mutates `value` and
// `velocity` in place. `dt` = DAT_140b0a344 (the per-tick game delta the in-game oracle recovers).
void damped_spring_step(float& value, float& velocity, float equilibrium, float freq, float dt);

} // namespace sim
