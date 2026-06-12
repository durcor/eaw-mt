# Galactic Map (Strategic Layer)

**Status:** Phase 2 orientation — mode-1 dispatch decoded and classified. The galactic *UI shell*
is now fully mapped; the galactic *simulation* path is located but not yet decoded (see §"Where the
sim actually ticks").
**Last verified:** 2026-06-12 (galactic increment 1)

This file opens the galactic-layer reverse-engineering arc. The tactical (space-battle) sim slice is
closed (see `firing_body_lift_scope.md`, `threading_model.md`); the galactic layer is the next
parallelization frontier (`game_mode.md:102`, blueprint §"highest pain point — galactic map stalls").

---

## Increment 1 finding — the mode-1 dispatch is the UI shell, NOT the sim

`FUN_14030c3b0` (`0x30c3b0`, the always-path mode state machine — `game_loop.md:146`) dispatches, in
its mode-1 (galactic) branch, ten subsystem ticks (`decomp/30c3b0.c:200-210`). **All ten are galactic
HUD / shell UI refresh.** Every one is dominated by `FUN_140347b50(widget_id)` (get-widget-by-id)
followed by UI vtable calls (set text `+0xa0`/`+0x148`, set color `+0x128`, set visible `+0x78`/`+0x80`,
fill/progress `+0x168`). **None** of the ten calls Lua, the strategic event queue (`DAT_140b2ed18`),
the sim RNG (`DAT_140a13e24`), or `GameObjectManager::CreateObject` — i.e. none performs galactic
simulation. They read settled game state and push it to widgets, on the main thread.

> **Correction to prior hypothesis.** The state memory and blueprint §10 carried a hedged guess that
> "the GOM tick is likely inside the mode-1 galactic dispatch." That is **wrong**: the mode-1 dispatch
> is the galactic *UI shell*. `30c3b0` itself is the mode-dispatch + camera/fade/FOW/selection/
> shell-toggle coordinator (`decomp/30c3b0.c`), an always-path presentation routine — not the
> simulation. The sim ticks on a different path (below).

### The ten mode-1 ticks (`decomp/30c3b0.c:200`)

| Order | RVA | Lines | Widgets | Role |
|---|---|---|---|---|
| A | `0x3e8a80` | 789 | 15 | **Galactic HUD top bar** — credits (`$ %d`), tech (`TEXT_TECH`), pop-cap (`%d / %d`), cycles (`TEXT_CYCLES`), planet-info button, faction colors; also drives selected-planet camera focus + galactic-object highlight. *(read in full)* |
| B | `0x3e83a0` | 189 | 4 | UI panel refresh — GOM-registry-driven faction/planet widget update *(fingerprinted; exact panel TODO)* |
| C | `0x3145c0` | 995 | 7 | UI panel refresh — large, heavy string-buffer churn (fleet/unit list candidate) *(fingerprinted; exact panel TODO)* |
| D | `0x3edd00` | 276 | 9 | UI panel refresh *(fingerprinted; exact panel TODO)* |
| E | `0x3e5aa0` | 233 | 8 | UI panel refresh *(fingerprinted; exact panel TODO)* |
| F | `0x310e40` | 197 | 1 | **Planet build-queue button** — "Sell ($ %d)" / `TEXT_PRODUCTION_IN_PROGRESS01`, sets button text/color + progress-bar fill width from queue state (`this+0x20d0`). *(read in full)* |
| G | `0x3e4cb0` | 587 | 8 | UI panel refresh — one incidental `b2ed18` ref, 4 GOM-reg reads *(fingerprinted; exact panel TODO)* |
| H | `0x3e6ff0` | 339 | 7 | UI panel refresh *(fingerprinted; exact panel TODO)* |
| I | `0x3e6630` | 252 | 7 | UI panel refresh — 3 string formats *(fingerprinted; exact panel TODO)* |
| J | `0x3e79b0` | 31 | 2 | Tiny single-widget toggle *(read in full)* |

Classification confidence: A/F/J read in full; B–E,G–I classified by call-fingerprint (widget + string
buffer dominant, zero sim/Lua/RNG/event-queue calls). Exact panel identity of B–E,G–I is a `TODO`, but
the **role class (main-thread UI presentation)** is established for all ten.

### Parallelization implication

The ten mode-1 ticks are **main-thread UI presentation**, the galactic-layer analogue of the tactical
HUD/renderer — *not* the parallelization frontier. They write widget objects (UI tree), read settled
sim state, and run on the always-path regardless of `game_active`. Treat like the renderer/UI in the
tactical slice: main-thread-only, off the lockstep. The frontier is the galactic *sim* path below.

---

## Where the sim actually ticks (next increment — not yet decoded)

The galactic simulation is driven, like the tactical sim, off the **active game-mode object**
`DAT_140b15418` via `mode->vtable[0x2B]` (slot 43) — the per-frame simulation dispatch
(`game_loop.md:39-40`) — and the game-active "primary game-state update" `FUN_14021caf0`
(`game_loop.md:75`). For the galactic mode this resolves to `GalacticModeClass`'s slot-43, distinct
from the combat `FUN_14035cc10`. The strategic-layer concerns that must live on that path:

- **`OutgoingEventQueueClass` `DAT_140b2ed18`** — the genuine timed strategic event queue, ordered by
  `(fire_time, insertion)` (`sim_parallel_command_schema.md:195`, `decomp_plan.md:727`,
  `threading_model.md:354`). Drained each frame by `FUN_1403b08c0(&DAT_140b2ed18)`
  (`game_loop.md:57`, `0x3b08c0` "pending command queue processor"). This is the Class-2 galactic
  order/event seam — the analogue the tactical slice deliberately scoped *out*.
- **Galactic GameObject behaviors** — planets, fleets, and galactic units are GameObjects; their
  per-tick behaviors (`PlanetaryBehaviorClass`, economy, fleet movement, build-queue advance) tick
  through the GOM behavior dispatch. `PlanetaryBehaviorClass::FUN_14050bf10` already appears as a read
  in tick A.
- **Galactic AI / story** — the Lua "Main State" (galactic map + global scripts,
  `script_engine.md:19`, `threading_model.md:60`) drives faction AI and story plots. Blueprint flags
  galactic-map Lua stalls as the project's highest perf pain point.

> **TODO (galactic increment 2):** decode `GalacticModeClass` vtable[43] + `FUN_14021caf0` to locate
> the galactic GameObject behavior tick and the `b2ed18` event drain in context; classify the write
> set against the existing 3-class model (`threading_model.md`). Halt-and-document per blueprint rule 4
> until that path is actually decoded — do not assume it mirrors the tactical GOM tick.

---

## Cross-refs

- `game_mode.md` — mode enum + per-mode dispatch (this refines its "10 galactic map subsystem ticks"
  to "10 galactic UI shell ticks").
- `game_loop.md` — main loop, the `mode->vtable[43]` sim dispatch, `b2ed18` drain.
- `threading_model.md` — the 3-class write-set model the galactic sim must be measured against.
- `sim_parallel_command_schema.md:195` — the galactic `OutgoingEventQueueClass` noted as out-of-slice
  for the tactical work; it is now *in* slice for the galactic arc.
- `script_engine.md` — the Lua Main State (galactic AI/story).
