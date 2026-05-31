# sim/ — Recompilable sim-core slice (Product 1)

This directory holds the **functional re-implementation in C++** of Empire at War's simulation
core, lifted from `StarWarsG.exe` per `openspec/engine/decomp_plan.md` Phase 3. It is **not** a
byte-matching decomp — it reproduces *behavior*, validated by **differential testing against the
live binary** (the winmm hook DLL captures ground-truth `DIFFTRACE` per-tick checksums; the lifted
code must reproduce the same per-tick sim state).

## Methodology (per decomp_plan.md §1)

- Lift in **dependency order**: tick clock → entity-update spine → locomotor integrators →
  hardpoint fire-control → the IN sim behaviors.
- Each unit is a clean, idiomatic C++ re-implementation. Original RVAs and struct offsets are kept
  in comments for traceability (`// <- FUN_14027c360 +0x14`), but the code does **not** mirror the
  decompiler's variable soup.
- Each unit ships a **host-side self-test** (`sim/tests/`) that replays a deterministic input
  stream and asserts the lifted behavior. The in-game `DIFFTRACE` stream is the end-to-end oracle;
  the host test is the fast inner loop.
- A unit is "lifted" only when its semantics are reproduced **and** its determinism-relevant
  behavior (ordering, fixed-point/float accumulation, RNG, lockstep gating) is called out explicitly.

## Build

Host validation (no game needed):
```
just sim-test            # compiles sim/ + runs sim/tests/*; all must pass
```
or directly:
```
nix develop --command g++ -std=c++17 -O2 -Wall -Isim sim/tests/tick_clock_test.cpp sim/tick_clock.cpp -o /tmp/tick_clock_test && /tmp/tick_clock_test
```

## Status

| Unit | Source RVAs | State | Determinism notes |
|---|---|---|---|
| **tick clock / scheduler** | `27c360` `27c2f0` `27c330` `27c5f0` | ✅ lifted (SP); MP lockstep gate behind `IFrameSynchronizer` (Phase-4 dep) | Fixed-timestep `1000/cap` ms; MP = lockstep `MaxAhead` + per-player command-ready gate |
| **entity-update spine** | `3639d0` `2be640` `3a6b80` | ✅ lifted (skeleton + hooks) | reverse-order behavior dispatch; GOM list order; **no child recursion** (flat list); per-unit bodies are `EntityUpdateHooks` |
| **locomotor integrators** | Starship `6236b0`+`6224b0` (+helpers); SimpleSpace `626420`+`41c000` | ✅ Starship skeleton + integrator lifted; SimpleSpace dispatch skeleton + direction/straight-move lifted (Hermite mover `625990` + special-mode `6269f0` are hooks); shared model + primitives | **✅ FIRST IN-GAME ORACLE PASS**: SimpleSpace `dir=(cos(hd·DEG2RAD),sin(hd·DEG2RAD))` matches live `state+0x14/18/1c` bit-exact (182 ticks) + `disp∥dir` (150/150). Two families (Starship raw-velocity vs SimpleSpace facing+spline-magnitude). Magnitude movers still to lift |
| hardpoint fire-control | `a76b0` `387010` `387400` … | ⬜ | order-sensitive fire-budget float sum; target-acq RNG |
| sim behaviors (13 IN) | see decomp_plan census | ⬜ | per-behavior Phase-A vs Phase-B write split |
| command/event queue | `OutgoingEventQueue` + event types | ⬜ | canonical Phase-B apply order |

The tick clock stays **serial** in the parallel rewrite — the parallelism is *within* a tick, over
entities (Phase A), not across ticks. Lifting it first pins the tick that every other unit is
validated against.
