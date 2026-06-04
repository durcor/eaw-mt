// sim/nebula.h — Phase-3 IN sim behavior #5: NebulaBehaviorClass::vfunc_6.
//
// Lifted from RVA 0x437b60 (decomp/437b60.c). Fifth of the 13 in-slice IN sim behaviors
// (decomp_plan.md Phase-3). Picked as the "clean float integrator" candidate: its per-tick core is
// a textbook semi-implicit DAMPED HARMONIC OSCILLATOR that runs UNCONDITIONALLY every tick, so the
// state change is continuously observable (the cleanest possible oracle target — like EnergyPool #2,
// but a 2nd-order spring rather than a 1st-order regen). It models a ship's nebula-effect intensity
// smoothly ramping toward "fully inside" (1.0) on entry and back to "clear" (0.0) on exit.
//
// ── Per-tick algorithm (437b60) ──────────────────────────────────────────────────────────────────
//   The behavior runs in two stages every tick, in this order:
//
//   STAGE 1 — SPRING STEP (437b60.c lines 41-51, UNCONDITIONAL).
//     A semi-implicit damped oscillator drives `value` (sub+0x11c) toward `equilibrium` (sub+0x120):
//       w   = freq * 2.0                                  // natural angular frequency; freq = sub+0x12c
//       wdt = w * dt                                      // dt = DAT_140b0a344 (per-tick game dt, runtime)
//       dx  = value - equilibrium                          // displacement from rest
//       inv = 1 / (1 + wdt + 0.48*wdt^2 + 0.235*wdt^3)     // cubic-stabilised damping denominator
//       tmp = (dx*w + velocity) * dt
//       value'    = (tmp + dx)*inv + equilibrium           // sub+0x11c
//       velocity' = (velocity - tmp*w)*inv                 // sub+0x124
//     Pure float math; no env, no RNG, no cross-entity. Float op order is preserved bit-for-bit
//     (see nebula.cpp). With freq==0 it degenerates to a plain linear glide value += velocity*dt.
//
//   STAGE 2 — MEMBERSHIP / THROTTLE (437b60.c lines 52-139), gated by a LINGER throttle:
//     linger = FUN_140380bb0(sub): TRUE while (now - enter_tick)/30.0 < grace_seconds, i.e. the
//              entity entered a nebula recently (enter_tick = sub+0x104; 30 = DAT_140b0a340 sim-Hz;
//              grace_seconds = DAT_140b2701c config). It is a SCAN-SKIP optimisation:
//       * linger TRUE  -> LINGER: skip the expensive spatial scan; just re-pin the spring to full
//                         (if equilibrium < 1.0: value=1, equilibrium=1, velocity=0). 437b60:135-138.
//       * linger FALSE -> SPATIAL SCAN of nearby objects (sphere query over the global object set):
//           - found a nebula object (object's template+0x57 flag) -> IN NEBULA:
//               on the -1 -> in edge (enter_tick == -1): fire ENTER fx, set equilibrium = 1.0;
//               always refresh enter_tick = now. 437b60:96-109.
//           - found none -> OUT OF NEBULA:
//               was = enter_tick; enter_tick = -1; on the in -> -1 edge (was != -1): fire LEAVE fx,
//               set equilibrium = 0.0. 437b60:128-133.
//
//   ENTER fx = FUN_1404376f0: walks the 77 ability slots and emits signal 0x2b (AbilitySignalData)
//              per nebula-affected ability -> DISABLES sensors/abilities inside the nebula.
//   LEAVE fx = FUN_14039fb40: emits signal 0x2c per re-enabled ability -> the inverse on exit.
//   Both are channel-1 signal emissions (the CommandSink seam, [[command_sink.h]]); the deterministic
//   core only reports the EDGE (enter/leave bool) — the actual emissions are an env seam.
//
// ── Determinism / threading notes ────────────────────────────────────────────────────────────────
//   * The spring integrator is fully deterministic given (value, equilibrium, velocity, freq, dt) plus
//     the four fixed constants (2.0 / 0.48 / 0.235 / 1.0). dt and the two throttle constants
//     (sim-Hz 30, grace_seconds) are runtime-populated globals the in-game oracle recovers live
//     (exactly as EnergyPool recovered its dt and UnitAI recovered its +300 throttle).
//   * Cross-entity coupling is READ-ONLY: the spatial scan reads the live global object set and other
//     objects' template flags, but the behavior WRITES only its OWN sub-object state. Contrast
//     UnitAI #4, which writes the SHARED sensor grid. So nebula is closer to Phase-A-safe: the only
//     parallel hazards are (a) reading a stable snapshot of the global object set mid-sweep, and
//     (b) the enter/leave signal emissions, which route through the CommandSink (Phase-B) seam.
//   * The LINGER throttle makes the scan fire at most once per grace window after entry; the spring
//     keeps integrating every tick regardless, so intensity stays observably live during linger.
#pragma once

#include "damped_spring.h"   // the shared semi-implicit damped-spring step (STAGE 1 delegates to it)

namespace sim {

// Resolved per-tick disposition (the FUN_140380bb0 linger throttle + the spatial-scan result,
// all world-coupled, collapsed to one enum the deterministic core switches on).
enum class NebulaDisposition {
    Linger,         // FUN_140380bb0 TRUE — within grace window of entry; skip scan, pin to full
    InNebula,       // scan FALSE-throttle + found a nebula object this tick
    OutOfNebula,    // scan FALSE-throttle + found no nebula object this tick
};

// Edge-triggered effect hooks fired this tick (the channel-1 signal seam). The deterministic core
// reports the edges; wiring them to the actual 0x2b/0x2c emissions is the env/CommandSink's job.
struct NebulaFx {
    bool enter = false;  // FUN_1404376f0 — entered nebula this tick (emit 0x2b disable per ability)
    bool leave = false;  // FUN_14039fb40 — left nebula this tick   (emit 0x2c re-enable)
};

// Per-entity nebula state. Lives on the sub-object at entity+0xf0 (the same component slot EnergyPool
// uses, different fields). Field offsets noted against 437b60.c.
struct NebulaState {
    float value       = 0.0f;   // sub+0x11c — current effect intensity (the spring output, [0,1])
    float equilibrium = 0.0f;   // sub+0x120 — spring target: 1.0 inside a nebula, 0.0 clear
    float velocity    = 0.0f;   // sub+0x124 — spring velocity
    float freq        = 0.0f;   // sub+0x12c — natural-frequency config field (w = freq*2.0)
    int   enter_tick  = -1;     // sub+0x104 — tick the entity entered a nebula, or -1 if outside
};

// Fixed constants recovered from the binary (decomp_plan.md Phase-3 #5). These are now the SHARED
// spring constants (kSpring*) in damped_spring.h — kept here as Nebula-named aliases.
constexpr float kNebulaFreqScale = kSpringFreqScale;  // 2.0f, DAT_1408007d4 — freq -> w
constexpr float kNebulaQuad      = kSpringQuad;       // 0.48f, DAT_140819c54 — wdt^2 coefficient
constexpr float kNebulaCubic     = kSpringCubic;      // 0.235f, DAT_140819c50 — wdt^3 coefficient

// STAGE 1: the damped-oscillator integrator (437b60.c lines 41-51). Mutates st.value / st.velocity.
// Delegates to the engine's shared sim/damped_spring.{h,cpp} primitive (Select's spring tail is the
// byte-identical sibling). dt = DAT_140b0a344, the per-tick game-delta (runtime; the oracle recovers).
void nebula_spring_step(NebulaState& st, float dt);

// STAGE 2: the membership / throttle update (437b60.c lines 52-139). Mutates st.equilibrium /
// st.enter_tick and reports the enter/leave edges in `fx`. `now` is the current sim tick.
void nebula_membership_update(NebulaState& st, NebulaDisposition disp, int now, NebulaFx& fx);

// Full per-tick behavior: STAGE 1 then STAGE 2, in the engine's order. Returns the edge fx.
NebulaFx nebula_tick(NebulaState& st, NebulaDisposition disp, int now, float dt);

// World-coupled per-tick inputs, modelled (not byte-lifted): the disposition (linger throttle +
// spatial scan) and the per-tick game-delta. Exists for wiring into the spine; the deterministic
// core above takes the resolved values.
struct NebulaEnv {
    virtual ~NebulaEnv() = default;
    virtual NebulaDisposition disposition(void* entity) = 0; // FUN_140380bb0 throttle + sphere scan
    virtual float dt(void* entity) = 0;                       // DAT_140b0a344 per-tick game-delta
};

} // namespace sim
