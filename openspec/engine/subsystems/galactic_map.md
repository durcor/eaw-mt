# Galactic Map (Strategic Layer)

**Status:** Phase 2 orientation — mode-1 dispatch (UI shell) mapped, and the galactic *simulation*
driver identified: it is **Lua-coroutine-driven via Pump_Threads** (slot-43 `GameModeClass::vfunc_4`
→ signal 0x23 → ScoringManager → Pump_Threads), complemented by the C++ `OutgoingEventQueueClass`
(`b2ed18`) Add→drain. No separate galactic GOM C++ sweep exists. Producer enumeration = increment 3.
**Last verified:** 2026-06-12 (galactic increments 1, 2a, 2b)

This file opens the galactic-layer reverse-engineering arc. The tactical (space-battle) sim slice is
closed (see `firing_body_lift_scope.md`, `threading_model.md`); the galactic layer is the next
parallelization frontier (`game_mode.md:102`, blueprint §"highest pain point — galactic map stalls").

---

## Two distinct "mode" concepts (do not conflate)

- **Shell/scene enum** `*(int*)(DAT_140b27f60 + 0x211c)` — `0`=menu, `1`=galactic, `2`=space,
  `3`=land. Drives the UI dispatch in `FUN_14030c3b0` (`game_mode.md`). This is the enum below.
- **Active game-mode *class* object** `DAT_140b15418` — the `GalacticModeClass` / `LandModeClass` /
  `SpaceModeClass` instance (mode-class ids `0`=Galactic, `1`=Land, `2`=Space; blueprint:498). Drives
  the **simulation** via `vtable[0x2B]` (slot 43, offset `0x158`). This is the sim path.

These are different objects with different enumerations; "mode 1" means galactic *shell* but land
*mode-class*. Keep them straight.

## Increment 1 finding — the mode-1 dispatch is the UI shell, NOT the sim

`FUN_14030c3b0` (`0x30c3b0`, the always-path mode state machine — `game_loop.md:146`) dispatches, in
its shell-enum==1 (galactic) branch, ten subsystem ticks (`decomp/30c3b0.c:200-210`). **All ten are galactic
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

## Where the sim actually ticks (ANSWERED — Lua-coroutine-driven)

The per-frame **simulation dispatch** for *every* mode is `DAT_140b15418->vtable[0x2B]` (slot 43,
offset `0x158`), the first substantive work each frame (`game_loop.md:219-224, 311`). Per blueprint:498
(a confirmed finding) **all three mode classes — Galactic, Land, Space — share `FUN_14035cc10` at slot
43**, so the galactic dispatch is the *same already-decoded function*, not a separate one. Its body
(`game_loop.md:226-234`) is:

```
FUN_140493d50(mode[7])   // bombing-run scheduler        — combat-specific (empty/inert in galactic)
FUN_1405284b0(mode[8])   // orbital-bombardment scheduler — combat-specific (empty/inert in galactic)
FUN_1404d9cd0(...)       // per-player battle-cleanup service
FUN_140220ed0(... 0x23 ) // fire signal 0x23 → ScoringManager::Service → Pump_Threads (Lua AI pump)
FUN_1403c05d0(mode[0x3e])// SetupPhaseManager lazy init (guarded)
```

**✅ Increment 2b finding — the galactic sim is Lua-coroutine-driven, via this same path.** The slot-43
body is `GameModeClass::vfunc_4` (RTTI-named base method, `decomp/35cc10.c`) and it fires signal **0x23
unconditionally** — `FUN_140220ed0(DAT_140a10b50, &DAT_140b153e0, 0x23, mode)`, **no mode/combat gate**.
The combat schedulers `mode[7]`/`mode[8]` are gated only by `DAT_140b15490` (a pause flag, not mode) and
are empty in galactic. So in galactic mode the 0x23 signal still fires, and its **`TheGameScoringManager`
listener — registered in `System_Initialize`, active in galactic/land/space (`ai.md:44`), with a
dedicated "Galactic map load" AI-init path `FUN_14056480`←`0x55b60` (`ai.md:115`) — runs
`Service → Pump_Threads → lua_resume`** over the galactic AI coroutines (`ai.md:30-44`). EaW has no
native C++ Think loop (`game_loop.md:277-290`): the galactic faction AI, planet/economy logic, and story
are **Lua coroutines pumped here**, the *same* mechanism as combat. This is why galactic-map Lua stalls
are the project's #1 perf pain point (the Phase-5 prewarm fix was exactly first-run galactic/battle
coroutine cost — `ai.md:101-119`).

> Reconciles blueprint:498 ("0x23 has listeners only in combat, `DAT_140b15b10 != 0`"): the
> `DAT_140b15b10` gate governs the *extra combat per-unit* AI listeners, **not** the ScoringManager,
> which subscribes in every non-loading mode. Both are true; 0x23 always fires, the ScoringManager
> always listens (galactic included), combat merely adds more listeners.

The C++ side that complements the Lua pump:

- **`OutgoingEventQueueClass::Add` producers** — the strategic event/command *enqueue* API:
  `FUN_140375380` (scheduled — "OutgoingEventQueueClass::Add -- Adding scheduled event %s" `0x8657b0`,
  8 branch sites; 5644 bytes; **virtual**, vtable `0x97bcc4`) and `FUN_140565010` (non-scheduled —
  `0x8657f0`). These are network-replicated, player-id-keyed, serialized-payload events
  (PENDING_TACTICAL_BATTLE_VOTE etc.) — the MP-deterministic strategic command channel. Called
  virtually, so producers reach them through the queue vtable (full producer enumeration TODO).
- **`FUN_1403b08c0(&DAT_140b2ed18)`** — the per-frame command/event-queue **apply/drain** that consumes
  what `Add` enqueues (decoded, `decomp/3b08c0.c`). Runs in the loop always-section
  (`game_loop.md:54-57`), all modes. `param_1` = queue header (`+0x08` head, `+0x10` tail, `+0x18`
  count; singly-linked via `node[2]`). Gated by connection-state `DAT_140b153f8` (0/3 → local drain;
  1/2/4 → MP-ordered path `FUN_1403b03e0` when the GOM registry `[DAT_140a16fd0..fd8]` is non-empty and
  the active mode object `DAT_140b15418+0x98` flag is set). Per node it invokes the polymorphic command
  **apply** `node->vtable[0x38]` (returns char; on true stamps `node+0x14 = DAT_140b0a320` timestamp),
  then `ScheduledEventQueueClass::vfunc_1(&DAT_140b15690, node)` + release `node->vtable[1]`. This is the
  generic **Class-2 order/event drain** (apply side).

### Parallelization implication (galactic)

The galactic sim's heavy work is **Lua (Pump_Threads)** — the *same* offload target and the *same*
hazard as the tactical AI: Pump_Threads reads game-object state and **must complete before** the command
queue (`FUN_1403b08c0`) writes it, each frame (`ai.md:68-75`). The galactic C++ side is the
`b2ed18` Add→drain (Class-2 serial order/event channel). There is **no separate galactic GOM behavior
tick to parallelize** the way the tactical movement/fire slice was — the structural opportunity is the
Lua-pump offload + the Class-2 drain, not a per-object C++ sweep.

> **TODO (galactic increment 3):** enumerate the `OutgoingEventQueueClass::Add` virtual producers (what
> galactic gameplay enqueues which event types), and characterize the galactic Lua-pump↔command-queue
> ordering against the 3-class model. The Lua interpreter itself (single Main State) is the parallelism
> bottleneck — see `script_engine.md` / `threading_model.md` Model B (Lua offload, previously ruled
> medium-risk). Halt-and-document per blueprint rule 4 before assuming an offload is safe.

---

## Cross-refs

- `game_mode.md` — mode enum + per-mode dispatch (this refines its "10 galactic map subsystem ticks"
  to "10 galactic UI shell ticks").
- `game_loop.md` — main loop, the `mode->vtable[43]` sim dispatch, `b2ed18` drain.
- `threading_model.md` — the 3-class write-set model the galactic sim must be measured against.
- `sim_parallel_command_schema.md:195` — the galactic `OutgoingEventQueueClass` noted as out-of-slice
  for the tactical work; it is now *in* slice for the galactic arc.
- `script_engine.md` — the Lua Main State (galactic AI/story).
