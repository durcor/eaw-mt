# Engine Source Decompilation Plan

**Status:** Phases 0–2 EXECUTED (2026-05-30); Phase 3 **LIFTING** — units 1 (tick clock),
2 (entity-update spine), 3 (locomotor integrators, Starship) DONE (2026-05-30, `sim/`,
host-validated via `just sim-test`). Companion to
`threading_model.md` → *Engine-Source Sim-Parallel Rewrite — Boundary Scope*, which defines
*what the rewrite needs*; this defines *how we'd obtain the source it needs*. Phase 0 (RTTI gate)
passed; Phase 1 (infra: RTTI applied, batch decompiler, call-graph attribution, diff harness) and
Phase 2 (struct recovery + full sim-tick spine) complete — see those sections + `movement_structs.md`.

---

## 0. The honest framing first

The sim-parallel rewrite is blocked on one thing: **editable engine source**. No Petroglyph/Alamo
source leak exists, so "getting the source" means **reconstructing it from the binary** — a
decompilation. The literal ask — a *full* engine source decomp — has a hard, measured size:

| Metric | Value | Source |
|---|---|---|
| Total functions | **21,744** | `logs/functions.tsv` |
| Unnamed (`FUN_*`) | 20,558 (94.5%) | same |
| Named (mostly Ghidra auto/thunks) | 1,185 | same |
| **Understood & documented by us** | **292** (1.3%) | `grep FUN_ openspec/` |
| `.text` size | 8.37 MB | `binary_metadata.md` |
| Binary | 12 MB, x86-64, MSVC, **symbol-stripped**, optimized, statically-linked Lua 5.1 + STL | same |

For calibration: the community *matching* decomps (SM64, OoT) reconstruct **~2–4 MB** binaries that
were **less optimized**, often had **symbols/debug data**, and still took **multiple contributors
several years each**. A 12 MB, symbol-stripped, modern-optimized PC binary is a strictly harder
target. **A 100%-recompilable full decomp is a person-decade-scale effort and is not the right goal.**

**The reframe that makes this tractable:** the sim-parallel rewrite does not need the renderer,
audio, Bink video, UI, or networking decompiled. It needs a **vertical slice** — the *simulation
core* plus its *struct layouts* plus its *determinism surface*. That slice is **hundreds of
functions, not 21,744**. This plan therefore delivers two products in priority order:

- **Product 1 (critical path): a recompilable sim-core slice** — enough to actually do the rewrite.
- **Product 2 (long tail, optional): full-breadth decomp** — only if a true standalone source
  replacement (not just the threading retrofit) ever becomes the goal.

We already have the assets to start Product 1: a Ghidra project, 134 analysis scripts under
`tools/ghidra_scripts/`, 292 mapped functions (including the *entire* gsvc→movement→command path the
rewrite targets), and a working in-process instrumentation/validation vehicle (the winmm hook DLL).

---

## 1. Strategy: lift-and-validate, not big-bang

We do **not** attempt a byte-matching decomp (the decomp.me model). We do a **functional
re-implementation of the sim core in C++**, validated by **differential testing against the live
binary**: same inputs → bit-identical per-tick sim state. The original binary stays the oracle
throughout; the hook DLL captures ground-truth state checksums to compare against.

This is the only model where partial progress is *useful*: each subsystem we lift can be validated
in isolation against the running game before we depend on it.

---

## 2. Phases

### Phase 0 — Recon & feasibility gate — ✅ EXECUTED 2026-05-30, GATE PASSED DECISIVELY

**Result: the binary retains full, descriptive MSVC RTTI. Feasibility for the sim-core slice is
strongly confirmed.** Measured (`tools/ghidra_scripts/Phase0RttiRecon.java` → `logs/Phase0RttiMap.tsv`):

| Metric | Value |
|---|---|
| RTTI TypeDescriptors (named classes) | **1,703** (1,628 game/engine-specific, not std/CRT) |
| Validated CompleteObjectLocators / vftables | 1,992 |
| Total vtable method slots | 20,346 |
| **Distinct functions named via vtables** | **5,310 = 24.4% of all 21,744** (5,300 are known entry points) |
| Descriptive `Class::Method` debug strings | present (e.g. `GamePlayUIClass::Init`, `DatabaseConversionSystemClass::Get().System_Initialize`) — a second, independent naming source |

**The sim-core / Phase-A target subsystem is fully RTTI-named** — every type the rewrite touches:
`GameObjectClass`, `GameObjectManagerClass`, `GameModeClass`/`GameModeManagerClass`, `PlayerClass`,
the entire **Locomotor** hierarchy (`LocomotorBehaviorClass` + 15 subclasses: Starship/Walk/Fighter/
Fleet/Flying/JetPack/Bike/LandBomber/SimpleSpace/Team/LandTeam…, ~97 vmethods each),
`MovementCoordinatorClass`/`Land`/`Space`, all five pathfinders (`Land/Space/DynamicLand/LandZone/
Galactic PathFinderClass`), `TheMovementClassManagerClass`, the command/event types
(`StopMovementEventClass` and the `EventFactoryClass<…>` family), and the determinism-relevant enums
(`MovementClassType`, `LocomotorStateType`, `GameObjectCategoryType`, `CellPassabilityType`).

Our hand-mapped hot-path functions (`FUN_140387400` path-follow, `FUN_1403a76b0`, `FUN_14028d400`
gsvc, etc.) are **non-virtual helpers** — not in a vtable directly, but reachable by call-graph from
the named Locomotor/Coordinator vmethods, so Phase 1's call-graph attribution names them. The 24.4%
vtable figure is a *floor*: it excludes non-virtual methods (named by class proximity), free
functions, and the `Class::Method` debug-string set.

**Gate verdict: PASSED.** RTTI alone names a quarter of the binary and 100% of the sim-core class
skeleton — far above the ~40–50% sim-reachable threshold. **Product 1 (sim-core slice) proceeds.**
Remaining Phase-0 task before Phase 1: external-corpus recon (mod SDK / community Lua-API method
names) to label the reflected-method set (boundary item #6) — lower priority now that RTTI carries
the structural load.

---

#### (Original Phase-0 plan, for reference)
Cheap, high-leverage discovery that can multiply or kill the effort. **Decision gate at the end.**

- **RTTI recovery.** MSVC binaries retain RTTI. EaW is heavily C++ (`TheGameScoringManagerClass`,
  mode classes, GOM). Run Ghidra's RTTI analyzer / a ClassInformer-style pass → recover class names,
  vtable layouts, and inheritance for potentially **hundreds** of classes at once. This is the single
  biggest naming multiplier; do it before anything manual.
- **Debug-string harvest.** The binary names functions in its own log strings ("Pump_Threads",
  "Service", etc.). Build a string→xref→function auto-namer (extend `ExportStrings` + xref scripts)
  to label every function reachable from a descriptive log/assert string.
- **External corpus.** Survey for: the EaW/FoC **mod SDK**, the Alamo engine's public method names
  (Lua-exposed game-object API is documented by the modding community), `PerceptionFunctionG.dll`
  symbols, and any debug build / PDB fragments. Community Lua API docs directly name the reflected
  C++ method set (work item #6 in the boundary scope).
- **Gate:** tally auto-recovered names + struct count. If RTTI+strings name ≳40–50% of sim-reachable
  functions, Product 1 is a months-scale solo effort and proceeds. If recovery is poor, re-scope to
  "assisted re-implementation of just the gsvc Phase-A body" (much smaller) and stop there.

### Phase 1 — Infrastructure (1–2 weeks) — 🟡 IN PROGRESS (started 2026-05-30)

**Done this session:**
- **Persistence confirmed.** The Ghidra `.rep` is 173 MB and fully analyzed/durable (the 0-byte
  `.gpr` is just a pointer file — no problem). The portable source of truth is the scripts + TSVs;
  the `.rep` is a regenerable local cache (re-run `Phase1ApplyRtti` to rebuild symbols on any clone).
- **RTTI applied as persistent symbols** (`Phase1ApplyRtti.java`): 523 class namespaces, 1,992
  vftable labels, **5,207 DEFAULT methods renamed into `Class::vfunc_<slot>`** (93 hand-named
  preserved). `FUN_` defaults dropped 20,558 → **15,543**; class-qualified functions now **5,583**.
  Saved into the project — every future decompile resolves vtable calls to real class names
  (verified: the movement path now shows e.g. `OutgoingEventQueueClass::vfunc_1`, the command/event
  queue — a direct rewrite target).
- **Batch decompiler** (`DecompileAt.java` + `just decomp <rva…>` / `just decomp-list <file>`):
  reusable, replaces the one-off `Phase5Decomp*` scripts; outputs to `decomp/<rva>.c` with current
  names. Smoke-tested on the sim-core hot path (387400/3a76b0/28d400).
- `ExportFunctions.java` now emits fully-qualified names (`Class::method`).

- **Call-graph attribution DONE** (`Phase1CallGraphAttrib.java`): conservative rule (callers all in
  one class; ≥2 class-callers or sole caller), iterated to fixpoint in 7 rounds (1731→586→145→35→9→2
  →0 — exponential decay = attribution propagating down the call tree). **+2,508 helpers attributed →
  8,097 class-owned functions (37% of 21,744).** Attributed helpers keep their `FUN_` name (DEFAULT
  source) under a class namespace (`WalkLocomotorBehaviorClass::FUN_140386170`) — provenance stays
  explicit: inferred owner, not an RTTI method. Spot-checked correct (locomotor helpers cluster under
  their concrete subclasses; movement helpers in the 0x37xxxx–0x38xxxx neighborhood).
  - **Known gap — indirect-dispatch roots.** The gsvc/movement hot path (`FUN_14028d400` →
    `FUN_1403a76b0` → `FUN_140387400`) is *not* auto-attributed: it's reached by stored
    function-pointer / registered-callback dispatch (`3a76b0` walks an array at `*(this+0x2d0)` and
    calls through it), and Ghidra's static call graph has no edge for indirect calls. These roots are
    already hand-mapped (openspec) and get confirmed when lifted in Phase 3. Optional future tool: an
    indirect-dispatch resolver (map callback-registration sites / vtable-slot writes → callee).

- **Differential-test harness DONE** (`winmm_proxy.c`, `a76b0_hook`): a per-tick movement-state
  fingerprint — the correctness oracle for Phases 3–4. Once per sim tick (detected via the frame
  counter `DAT_140b0a320`/RVA 0xb0a320) it emits an FNV-1a hash over every active locomotor
  component the coordinator services, folding value-deterministic fields {iteration index, entity
  motion-state `+0x48`, component speed-float `+0x28`, countdown `+0x58`}; enumeration mirrors
  `FUN_1403a76b0` exactly (coordinator `+0x2d0` list, `+0x10` count, `+0x8` ptr array, entity at
  element `+0x20`, active iff `+0x4d`==1). Output: `DIFFTRACE\ttick=…\tn=…\th=…` log lines.
  - **Build/gate:** profile-build only (`#ifdef EAW_PROFILE`; release carries 0 `DIFFTRACE`
    symbols — verified), runtime-enabled by `EAW_DIFFTRACE=1`. Capture: `just difftrace=1
    launch-foc-desktop`, load a tactical battle, then `grep DIFFTRACE eaw-mt.log` (written to the
    process CWD = repo root, not `corruption/`). Both builds compile clean; in-game smoke test
    confirmed the profile DLL loads with all fixes active and no startup regression. Actual per-tick
    lines require a tactical battle (movement coordinator only runs there) — that's the golden-trace
    capture step, done when the rewrite work in Phase 3 needs a baseline.
  - **Coverage note:** v1 fingerprints the movement *state machine* (which entities serviced, in
    what order, with what cadence/speed) — catches iteration-order, entity-set, and cadence
    divergence. Raw transform/position values are added once Phase 2 recovers the entity layout; the
    framework is the deliverable, the hashed field set is a knob.

**Phase 1 COMPLETE.** Next: Phase 2 (struct recovery) — folds in the struct DB and unlocks
transform-value coverage in the harness.

#### (Original Phase-1 plan, for reference)
Turn the one-function-at-a-time workflow into a pipeline.

- **Persistent, fully-analyzed, type-propagated Ghidra project**, committed/shareable (or a headless
  re-analysis recipe). All Phase-0 RTTI/string naming applied and saved.
- **Batch decompile export.** Headless `DecompileScript` over all sim-core functions → versioned C
  dump under `decomp/` (regenerable, diffable). Today we decompile by hand into `logs/Phase5Decomp*`;
  make it a `just decomp <rva>` + `just decomp-all` recipe.
- **Struct/type database in-project.** As layouts are recovered they're applied as Ghidra types so
  every downstream decompile is readable (`entity->target` not `*(param+0x38)`). This compounds.
- **Differential-test harness.** Extend the hook DLL: per-tick canonical checksum of world state
  (entity transforms/health/targets, RNG state, command queue). Capture golden traces from saves +
  replays. This is the correctness oracle for everything lifted in Phases 3–4.

### Phase 2 — Struct & type recovery — ✅ SUBSTANTIALLY COMPLETE 2026-05-30 — *the foundation*

Done (`openspec/data_structures/movement_structs.md`). Phase 2 not only recovered layouts but
**corrected the sim-tick architecture** the original Phase-3 plan was based on. Key results:

- **Sim-tick spine, mapped end-to-end** (was unknown / mislabeled before): `FUN_1403639d0`
  (sim-frame update) → `FUN_1402be640` (GOM entity-list iterator) → `FUN_1403a6b80` (per-GameObject
  update: behavior loop + hardpoints + recurse into children).
- **`GameObjectClass`**: `+0x78/+0x7c/+0x80` = **world position** (the sim x/y/z floats, 4 readers),
  `+0xa8` locomotor state, `+0x4d` active, `+0x4e` render-node gate, `+0x278`/`+0x290` = **behavior
  array/count**, `+0x2d0` hardpoint vector, `+0x38` listener list, `+0x2d8` lua ctx.
- **GOM**: master per-tick list = `ReferenceListClass<GameObjectClass>` at **`GOM+0xe8`** (sentinel
  `+0xf0`, head `+0xf8`); owning `MultiLinkedListClass` at `+0x408`. **Double-buffer set** = the
  `GameObjectClass` instances reachable from `GOM+0xe8`.
- **The mover ≠ what we thought.** `a76b0`/`387400` (long called "movement") is actually
  **`HardPointClass` weapon fire-control** (live RTTI). The real mover is
  **`LocomotorBehaviorClass::vfunc_6`** (vtable slot 6, per type), driven from `FUN_1403a6b80`.
- **Determinism inputs**: tick `DAT_140b0a320`, rate `DAT_140b0a340`, FF gate `0x9cf314`, game-speed
  mode (`DAT_140b15418` vfunc `0xe0`).
- **Oracle upgraded**: DIFFTRACE folds movement state machine **+ world position** **+ hardpoint
  state** — validated in-game (smooth moving-ship `DTPOS` trajectory).

Residual (non-blocking): label the 11 GOM category index-lists; resolve the `HardPoint+0x20`
owner-record class; apply recovered layouts as Ghidra types for readability.

### Phase 3 — Sim-core function lift — 🟢 SCOPED 2026-05-30 (post-Phase-2) — *the critical path*

> The original Phase-3 list (kept at the bottom of this section for history) was drafted **before**
> Phase 2 and is **wrong about the architecture**: it called the hardpoint fire-control path
> (`387010`/`387400`) "movement" and never mentioned the GOM iterator, the per-entity driver, or the
> behavior-dispatch model. This is the corrected, concrete scope.

**The real per-tick sim spine (Phase-2 result):**
```
FUN_1403639d0                       sim-frame update   (advances tick DAT_140b0a320)
  └─ FUN_1402be640(GOM)             iterate GOM+0xe8 ReferenceList<GameObject> (2 passes)
       └─ FUN_1403a6b80(entity)     per-GameObject update; recurses into child/contained units
            ├─ behavior loop: for b in entity[+0x278][0..+0x290]:
            │     if b enabled (b+0x3c==0) & scheduled (b+0x30<=tick): b->vfunc_6(entity, tick)
            │        └─ LocomotorBehavior::vfunc_6 (per type) → integrate → entity+0x78 position
            └─ FUN_1403a76b0(entity) → hardpoint fire-control (387010→387400 target acquisition)
```

**Lift order** (dependency order; each unit validated against the DIFFTRACE oracle before anything
depends on it):

1. **Tick clock / scheduler & determinism inputs.** ✅ **LIFTED 2026-05-30** →
   `sim/tick_clock.{h,cpp}` (+ `sim/tests/tick_clock_test.cpp`, `just sim-test`, all green).
   Lifted the global frame-clock singleton at `&DAT_140b0a320` and its four driver functions, all
   called once/loop-iteration from `FUN_14005d990` (WinMain):
   - `FUN_14027c360` → `tick_advance` — `++DAT_140b0a320` (the authoritative sim tick) + a
     ticks/sec meter. Struct: tick `+0x00`, ticked-flag `+0x14`, sim_fps `+0x1c`, meter anchors
     `+0x38/+0x3c`.
   - `FUN_14027c2f0` → `render_frame_advance` — a **separate** render-frame counter at `+0x04`
     (+ its own fps meter `+0x40/+0x48/+0x4c`); sim-irrelevant (present path only).
   - `FUN_14027c330` → `realtime_accumulate` — wall-clock seconds accumulator at `+0xb09c`.
   - `FUN_14027c5f0` → `should_run_sim_tick` — **the per-frame scheduler**. Key result: it is a
     **fixed-timestep scheduler with an MP lockstep frame-synchronizer**. SP/Replay (`DAT_140b153f8`
     ∈ {0,3}): advance one tick every `1000/cap` ms where `cap = fps_cap (+0x18)`, clamped to 120
     under fast-forward (`+0xb0b5`) or 30 under slow (`+0x37`); stamps `last_tick_time (+0x50)`;
     also honors a hard pause (`+0x34`) and a pause-edge / single-step debug facility
     (`+0xb0b0..b3`). MP (`{1,2,4}`): the `FrameSynchronizer` — refuse to advance past `MaxAhead`
     (`+0x08`) or before every peer's commands for the target frame arrive (the "FrameSynchronizer
     waiting for player…" path; per-player tables at `+0x54` / ptr `+0xc68`, player list
     `DAT_140a16fd0`, command mgr `DAT_140b15690`).
   - **Determinism note:** the MP lockstep gate IS the hardest Phase-4 constraint (lockstep MP /
     replay). It is encapsulated behind `IFrameSynchronizer` and deferred to Phase 4 (the per-player
     table walk); the SP fixed-timestep path is fully lifted and host-validated. The FF gate
     `DAT_1409cf314` is the WinMain *idle* gate (skips `WaitMessage`), not a field of this struct —
     it lets the loop spin so the scheduler's cadence governs tick rate.
   - The tick clock stays **serial** in the rewrite (parallelism is *within* a tick); lifting it
     first pins the tick every later unit is diffed against.
2. **Entity-update spine.** ✅ **LIFTED 2026-05-30** → `sim/entity_spine.{h,cpp}` (+
   `sim/tests/entity_spine_test.cpp`, all green). `FUN_1403639d0` → `FUN_1402be640` →
   `FUN_1403a6b80`, the dispatch *skeleton* with per-unit bodies as `EntityUpdateHooks`:
   - `FUN_1403639d0` → `sim_frame_update` — the sim frame updates **two** GameObjectManagers
     (`param_1[3]`, `param_1[4]`) with the same tick.
   - `FUN_1402be640` → `gom_update` — the master-list (`GOM+0xe8`) per-entity pass. ReferenceList
     walk `node+0x8` next, `entity = *(node+0x18) - 0x18`, head→sentinel; iteration ORDER preserved
     (modeled as an ordered list). The full function's second per-entity post-pass, the
     `second_update_list` transform pass (run with the sim tick saved/restored so it does *not*
     advance the clock), the deferred-spawn drain, and game-mode housekeeping are documented
     ordering anchors, lifted with their owning units.
   - `FUN_1403a6b80` → `update_game_object` — the per-GameObject driver. `mode_flag` (param_3) fast
     path: when set, skip blocks 1 & 4, run only the tail regen. The **behavior-dispatch loop**
     iterates `entity+0x278[]` **in reverse** (count `+0x290` − 1 → 0, index masked to a byte),
     gating each on schedule (`behavior+0x30 <= tick`) and enabled (`FUN_1404c3700` = `+0x3c == 0`),
     calling vtable slot 6. Intra-tick order pinned: locomotor pre-step → per-entity timed-action
     drain → behaviors → hardpoint fire-control → tail regen roll. **This loop is the Phase-A body
     the rewrite parallelizes over entities.**
   - **CORRECTION:** `FUN_1403a6b80` does **not** self-recurse — there is no child-unit recursion
     (earlier notes were wrong). The GOM master list is flat; contained units are separate entries,
     iterated independently. (Better for parallelization — no nested-update ordering to preserve.)
   - **CONFIRMATION:** `entity+0x60/+0x68` is a `DynamicVector` of 0x38-stride timed-action records
     (countdown `+0xc`), **not** a `std::string` — independently confirming the Phase-2
     disambiguation that the `+0x60` std::string belongs to the HardPoint owner record, not
     `GameObjectClass`.
   - **Determinism notes:** behavior reverse-order + GOM list order + timed-action fire-on-zero
     ordering are all preserved; the tail regen roll draws RNG (Phase-4 pin).
3. **Locomotor movement integrators (the Phase-A body).** 🟢 **FIRST CUT LIFTED 2026-05-30** →
   `sim/locomotor.{h,cpp}` (+ `sim/tests/locomotor_test.cpp`, all green). Lifted for the **Starship**
   type, establishing the shared model every locomotor reuses:
   - **Data model:** `LocomotorState` (entity+0xa8) — velocity `+0x14/18/1c`, prev-pos `+0x2c/30/34`,
     state `+0x48`, substate `+0x50`, turn_rate `+0x54`, countdown `+0x5c`, vert_step `+0x60`,
     docking anchors `+0x88/+0xa0` — and the `LocoState` enum (0 Idle / 1 Accelerate / 2,3 Turn,
     Cruise / 4 PostMoveWait / 5 Burst / 6 Decelerate / 7 Stopped / 8 Docking).
   - **Primitives (concrete):** `set_position` (`FUN_1403a8f90` — writes `entity+0x78`, sets the
     transform-dirty flag `0x100` via the `+0x3a1` byte → **confirms `0x100` = transform dirty**),
     `reschedule` (`LocomotorCommonClass::vfunc_6` `0x4c2f70` — sets `behavior+0x30 = +0x34 + tick`,
     the schedule gate the spine reads), `set_state` (`0x559250`), `rotate_xz`/`rotate_xy`
     (`0x4aaa40`/`0x2bab90` 2D rotations with the engine's small-angle fast path).
   - **`starship_tick` (`0x6236b0` skeleton):** pre-step → read `entity+0x78` → switch on
     `state+0x48` → core velocity integration + terrain-follow vertical clamp (toward
     `destination.z + terrain` at `max_climb_rate`) → write `entity+0x78` → **arrival FSM**
     (substate `0xa → fire "arriving"(9) → 0xb`; within `arrival_threshold` → fire "arrived"(10) →
     `9`). The arrival events are **cross-entity listener writes = Phase-B commands** in the rewrite.
   - **accel→velocity integrator LIFTED (`FUN_1406224b0` → `integrate_velocity`, 2026-05-30):** the
     deterministic producer of the per-tick velocity that drives `entity+0x78`. Rotates the
     template's base accel (`LocomotorTemplate+0x384` × `state.accel_factor+0x54`) by the entity
     heading (`+0x88/+0x8c`), adds to velocity (`state+0x14/18/1c`), clamps to `max_speed+0x37c`
     (scale by max²/speed²), applies steering-alignment damping when `state.steer_blend+0x58 > 0`
     (per-axis `damp_axis` toward `|vel|·unit_accel`, gain `DAT_1408754bc·blend`), then drag
     (`+0x398`). Introduced `LocomotorTemplate` (the `entity+0x298` tuning block). Default
     `LocomotorEnv::integrate_accel` runs it when the entity has a template.
   - **Still behind `LocomotorEnv` (each its own future sub-unit):** the steering-decision bodies
     (accelerate `622b80`, turn `622e90` — set `accel_factor`/`steer_blend`/heading target), burst,
     docking + world queries (destination `623340`, terrain height `135140`, max climb `372440`,
     arrival threshold, listener events). **TODO:** lift Walk `0x61e930` + the other ~13 subclasses
     (same vfunc_6 contract) and the steering bodies.
   - **Constants RECOVERED 2026-05-30** (`tools/ghidra_scripts/ReadFloats.java`): the "game-speed
     scale" `(DAT_1408007dc·DAT_1408007d4)/DAT_1408007f4` = `(π·2)/360` = **2π/360 = degrees→radians**
     — so the entity heading (`+0x88/+0x8c`) is in **degrees**. Steering gain `DAT_1408754bc` = 1.0;
     `DAT_1407ffaf8` = 1.0; `DAT_1408007f8` = 1000.0 (confirms `realtime_accumulate`'s ms/sec);
     `DAT_140800860` = `0x80000000` (float sign-flip mask). Baked into `sim/locomotor.{h,cpp}`
     (`DEG2RAD`, `STEER_GAIN`).
   - **In-game oracle attempt 2026-05-30 — INCONCLUSIVE (wrong unit type in the available scenario).**
     Added a `DTVEL` capture to the DIFFTRACE harness (`winmm_proxy.c`: per tick, the sampled ship's
     locomotor velocity `state+0x14/18/1c` + state `+0x48`) + an offline checker
     (`tools/analyze_loco_oracle.py`) for the invariant `pos[t]-pos[t-1] == velocity[t]`. Captured
     the menu-demo space battle (1052 ticks, 2 ships). Result: **both sampled ships are
     capital/Fleet-class locomotors, NOT the Starship type** — `+0x14/18/1c` is a **unit direction
     vector** (`|v|≡1.0`) with speed stored elsewhere (displacement = dir × a separately-varying
     scale, e.g. ×750 in cruise), and their state codes are `19/22/44` (∉ the Starship enum `0..8`).
     The Starship integrator (`6236b0` lines 211-214) adds `state+0x14/18/1c` **directly** to position
     → for a Starship that field IS raw per-tick velocity; these capital ships are a **different
     locomotor family** with a normalized-direction representation. The menu background demo contains
     no maneuvering Starship-class units, so it cannot validate the Starship lift. **To close the
     Starship oracle:** capture a battle with maneuvering Starship-class units (fighter/corvette;
     player-driven or a loaded save). The lift itself remains internally consistent (16 host checks);
     validation is blocked on test data, not a known defect.
   - **Starship auto-targeting added 2026-05-30** (`winmm_proxy.c` `dt_loco_vtbl_rva`): the harness
     scans each ship's behavior array (`coord+0x278`/`+0x290`) for its locomotor vtable, tags `DTVEL`
     with the family (`loco=<rva>`), and emits a dedicated `DTVELS` line for the first ship whose
     locomotor is a `StarshipLocomotorBehaviorClass` (`0x8ae250`) — so a future battle capture
     auto-targets the right units. `analyze_loco_oracle.py` reports families and prefers `DTVELS`.
     Verified in-game (no fault): the menu-demo space ships are 100% `SimpleSpaceLocomotorBehaviorClass`
     (`0x8aeaf8`), `DTVELS`=0.
   - **SimpleSpace dispatch skeleton LIFTED 2026-05-30** → `sim/locomotor` `simplespace_tick`
     (`FUN_140626420` + `SimpleSpaceEnv`; 6 host checks). SimpleSpace is the **common space mover**
     (behavior census's representative space unit; 100% of menu-demo ships). Its `vfunc_6` is a thin
     dispatcher: a special-mode gate (`vfunc_66` = `state+0x260`) routing states `4/5/0x28/0x2c` to
     handlers, else the normal state machine `Init(0) → Moving(0x13) → Arrived(0x16)`. Moving calls
     the **mover virtual** (`vfunc_59` = `0x625990`, **2354 B**) to compute the new pos/vel, writes
     position via the shared `set_position` (`FUN_1403a8f90`), and transitions to Arrived on
     completion or no-motion; Arrived bleeds residual speed and clears `state+0xec`. Velocity model:
     **unit direction at `state+0x14/18/1c` + speed scalar at `state+0xec`** (≠ Starship's raw
     velocity — the two families differ). The 2354-B mover (`vfunc_59`) is the deep callout; **lifting
     it is the bit-exact oracle step** (the menu capture already confirms displacement is *along* the
     `state+0x14/18/1c` direction, `dir·disp ≈ 1.0`).
   - **Locomotor structure is uniform:** every type = a thin `vfunc_6` dispatcher over a large
     per-type mover (Starship integrator `6224b0`; SimpleSpace mover `625990`). The dispatcher
     skeletons + shared model/primitives are lifted; each mover is its own sizable sub-lift.
   - **SimpleSpace mover (`vfunc_59` = `0x625990`) is a Hermite spline path-follower** — it
     interpolates a precomputed curve (control points in `state+0x18`, indexed by `state+0x60`) with a
     4×4 basis matrix (`DAT_140b2f200…`) + quaternion orientation ops. Full bit-exact lift would also
     need the live spline state captured — a large effort. **But its straight-line / path-complete
     branch (lines 67-76/191-199 + `FUN_14041c000`) is lifted** → `simplespace_straight_move`:
     `out_pos = in_pos + (cos(h·DEG2RAD), sin(h·DEG2RAD), 0)·speed`, `h = entity+0x8c` (deg),
     `speed = state+0xec`, Z held. This is the **dominant steady-cruise behavior** (the menu ships fly
     straight lines). Host-validated (reproduces the menu cruise displacement, 23 locomotor checks).
   - **Oracle status — structurally confirmed, bit-exact run pending environment.** The harness now
     captures `hd` (`entity+0x8c`) + `sp` (`state+0xec`) at `%.6f`, and `analyze_loco_oracle.py`
     checks `disp == (cos hd, sin hd, 0)·sp` on cruise ticks. The earlier capture already shows the
     **exact structural match**: `disp = unit_dir × const_scale`, `|dir| ≡ 1.0`, `dir·disp ≈ 1.0`,
     Z held — i.e. `pos += (cos h, sin h, 0)·speed`. The bit-exact `hd`/`sp` capture run is set up but
     **blocked: the Steam client was accidentally killed and can't be restarted without the user's X
     session** (Proton needs it). To finish: with Steam running, `just difftrace=1 launch-foc-desktop`
     (menu demo) → `python3 tools/analyze_loco_oracle.py eaw-mt.log` → expect ORACLE PASS.
4. **Hardpoint fire-control.** `FUN_1403a76b0` (per-ship fire-budget distribution over the hardpoint
   vector at `entity+0x2d0`, weighted by `hardpoint+0x58` via `540070`), `387010`, `387400`
   (opportunity-target acquisition), capped search `385190` (Fix B2), target set `382510` / release
   `3846c0`, force integrator `387f50`, nav submit `29f810`. **Oracle-validated** (DIFFTRACE folds
   hardpoint `{target, weight +0x28, countdown +0x58}`).
5. **Cross-entity / global write sites → command emitters.** The compute→apply boundary: every write
   a per-entity update makes to *other* entities or globals — target listener-list registration on
   `target+0x38` (`220e90`/`ed0`/`eb0` via `058570`), the global event queue `DAT_140b27e60`
   (`2d5290`), `OutgoingEventQueueClass`. In the parallel rewrite these defer into a per-thread
   command buffer (Phase B) instead of executing inline.
6. **Command/event queue drain (serial Phase-B apply).** `OutgoingEventQueueClass`,
   `StopMovementEventClass` + the `EventFactoryClass<…>` family, the queue at `DAT_140b27e60` —
   applied single-threaded after the parallel compute pass, in canonical order.

**The parallel boundary (concrete, from the above):**
- **Phase A — parallel over entities (steps 2–4):** each entity reads its own + others' *last-tick*
  state and computes its new position/locomotor-state/target, writing **only its own** fields
  (`+0x78`, `+0xa8`, hardpoint slots) and **emitting commands** for anything cross-entity.
  Double-buffer the `GameObject` set reachable from `GOM+0xe8` so reads see frozen last-tick state.
- **Phase B — serial (steps 5–6):** drain command buffers in canonical order (cross-entity listener
  writes, global queue, deferred order creation).
- **Determinism pins (front-loaded into Phase 4):** the `a76b0` fire-budget sum accumulates a float
  across the hardpoint list (**order-sensitive** — preserve iteration order); the GOM iteration
  order; RNG draws inside target acquisition.

**Function inventory (the sim-core slice):**

| Group | Anchor RVAs | ~count |
|---|---|---|
| spine | `3639d0`, `2be640`, `3a6b80` + GOM list ops (`2ac2f0`/`20ac80`) | ~10 |
| tick/clock | `27c360` + globals | ~3 |
| locomotor | 12× `vfunc_6` + common + state-machine helpers | ~40–60 |
| hardpoint fire-control | `a76b0`,`387010`,`387400`,`3825b0`,`385cf0`,`385190`,`382510`,`3846c0`,`387f50`,`29f810` + callees | ~30 |
| sim behaviors (per-unit `vfunc_6`) | 13 in-slice classes (census below) + helpers | ~80–140 |
| command/event queue | `OutgoingEventQueue` + event types | ~10 |
| **Total** | | **~180–280 fns** |

**✅ Behavior set RESOLVED 2026-05-30 (runtime `BEHENUM` census + decompile classification).** A
representative space unit carries **~14 behaviors** (`entity+0x278` array, `+0x290` count), each with
its **own** `vfunc_6` override (none inherit the base no-op) — so `FUN_1403a6b80` runs ~14
sub-updates per unit per tick. RTTI named them all; classification (in = mutates determinism-relevant
sim state; out = presentation):

| Behavior class | `vfunc_6` | In/Out | Note |
|---|---|---|---|
| `SimpleSpaceLocomotorBehaviorClass` (+Walk/Fighter/Fleet/Starship…) | `0x626420` | **IN** | the mover — writes `entity+0x78` position [CORE] |
| `UnitAIBehaviorClass` | `0x4f6070` | **IN** | per-tick AI decision/orders (977 B; cross-entity — the Model-B write surface) |
| `TargetingBehaviorClass` | `0x633a30` | **IN** | weapon target selection (per game-speed mode) |
| `DamageTrackingBehaviorClass` | `0x58bd80` | **IN** | HP/damage application (cross-entity) |
| `ShieldBehaviorClass` | `0x5495e0` | **IN** | shield regen/state |
| `EnergyPoolBehaviorClass` | `0x56c030` | **IN** | energy/power resource |
| `IonStunEffectBehaviorClass` | `0x62bba0` | **IN** | ion-stun status effect |
| `AbilityCountdownBehaviorClass` | `0x42f910` | **IN** | ability cooldowns |
| `AsteroidFieldDamageBehaviorClass` | `0x437310` | **IN** | environmental damage |
| `NebulaBehaviorClass` | `0x437b60` | **IN** | nebula env effects (sensors/shields) |
| `TelekinesisTargetBehaviorClass` | `0x63f210` | **IN** | force-power target state (TR hero powers) |
| `LureBehaviorClass` | `0x62b4c0` | **IN** | decoy/AI lure |
| `RevealBehaviorClass` | `0x5373c0` | **IN** | fog-of-war reveal (sensor grid — MP-determinism-relevant) |
| `SelectBehaviorClass` | `0x3c2310` | **out** | UI selection-indicator animation (damped-spring on a *displayed* value) |
| `HideWhenFoggedBehaviorClass` | `0x53ddc0` | **out** | render-visibility reaction to fog |

**Scope consequence:** the per-entity sim tick is **not just movement** — it is ~13 sim behaviors +
hardpoint fire-control, several with **cross-entity writes** (damage to other units, AI order
issuance, target acquisition, fog reveal). The Phase-B command buffer must therefore carry a rich
write set (damage apply, order issue, target set, fog update), not only movement. This confirms the
slice at the upper end (~180–280 fns) and re-affirms why naive parallelization was ruled out (pervasive
cross-object writes) — the rewrite's value is in the compute/apply split, and each IN behavior must be
classified Phase-A-compute vs Phase-B-apply when lifted. Census artifact: `BEHENUM` lines (the hook's
one-shot RTTI dump of `entity+0x278`); decompiled `vfunc_6` bodies in `decomp/{3c2310,633a30,53ddc0,
58bd80,4f6070,5373c0}.c`.

**Validation:** the DIFFTRACE oracle already covers movement state machine + world position
(`entity+0x78`) + hardpoint fire-control state. Each lifted function is diffed by replaying a fixed
save and comparing the per-tick DIFFTRACE hash stream bit-for-bit. Coverage gap to close in Phase 4:
fold RNG state + command-queue contents.

#### (Original pre-Phase-2 Phase-3 list — SUPERSEDED, kept for history)
1. ~~Tick clock / scheduler — `FUN_14027c360`, `DAT_140b0a320/340`, FF gate `0x9cf314`.~~ (still valid)
2. ~~Game service / loop spine — `FUN_14028d400` (gsvc), `FUN_1403a76b0`.~~ *(`a76b0` is hardpoints,
   not the loop spine; the spine is `3639d0`/`2be640`/`3a6b80`.)*
3. ~~Per-component movement / path-follow — `FUN_140387010` → `FUN_140387400`.~~ *(this is HARDPOINT
   fire-control, not movement; the mover is `LocomotorBehaviorClass::vfunc_6`.)*
4. ~~Cross-entity write sites — `FUN_1403846c0`, `FUN_1402d5290`.~~ (still valid; now step 5 above)
5. ~~Command-queue drain / apply.~~ (still valid; now step 6 above)

### Phase 4 — Determinism surface (weeks, overlaps Phase 3) — *the hardest constraint*
Per boundary work item #4, this is what makes the rewrite *correct*, not just fast. Map exhaustively:

- Every **RNG draw** and its seeding/ordering.
- **Float-accumulation order** dependencies (the FF force-sum loop in `FUN_1403a76b0` accumulates
  across members — order-sensitive; the canonical apply order must reproduce it).
- **Save/load + replay + lockstep-MP** serialization paths — the divergence detectors.
  Output: a determinism test gate (replay a long battle; assert bit-identical end-state) that the
  parallel rewrite must pass. **Without this gate the rewrite is unverifiable.**

### Phase 5 — Full-breadth long tail (optional, person-years)
Only if a true standalone source build (not the threading retrofit) is ever wanted: renderer (D3D9),
audio (MSS), UI, networking, Bink, the remaining ~20,000 functions. **Out of scope for the rewrite**;
listed only to bound Product 2 honestly. Recommendation: do not pursue unless the goal changes.

---

## 3. What this buys, and the realistic cost

| | Critical-path slice (Product 1) | Full recompilable decomp (Product 2) |
|---|---|---|
| Functions | hundreds (sim core) | 21,744 |
| Unblocks sim-parallel rewrite | **Yes** | Yes (overkill) |
| Effort (solo, w/ good RTTI/string recovery) | **months** | **person-decade — not realistic solo** |
| Validation | differential vs live binary | matching/recompile |
| Recommendation | **the actual goal** | only if scope changes |

The rewrite payoff is already quantified (boundary scope): a correct compute→apply parallel sim
yields **~3–4× faster fast-forward** (Amdahl, s≈0.15, N=4→2.9×, N=8→4×). The decomp is the
*enabling cost* for that one bounded win.

---

## 4. Risks / kill criteria

- **Poor RTTI/string recovery (Phase-0 gate fails).** If <~40% of sim-reachable functions auto-name,
  manual effort balloons — re-scope to the gsvc Phase-A body only, or stop.
- **Optimizer-fused functions.** MSVC inlining/ICF merges logic across the source's original
  boundaries; some functions won't cleanly map to re-implementable units. Mitigation: re-implement by
  *behavior* (validated by the oracle), not by matching.
- **Determinism rabbit hole (Phase 4).** This is open-ended and is the true long pole. If the
  determinism gate can't be made green, the parallel rewrite is unsafe regardless of decomp progress
  — so Phase 4 risk is really *rewrite* risk surfaced early. Front-load it.
- **Hidden coupling.** Sim functions may reach into render/UI/script state we assumed out-of-slice,
  pulling scope creep. Mitigation: the differential harness catches missing dependencies as checksum
  drift the moment a lifted function is wired in.

## 5. Recommended next concrete step

Phases 0–2 done; Phase 3 scoped **and its gate task (behavior census) complete** — the in-slice set
is the 13 sim behaviors + locomotor + hardpoint fire-control + spine tabulated above (~180–280 fns).
**Units 1 (tick clock), 2 (entity-update spine), 3 (locomotor — Starship skeleton + model +
accel→velocity integrator) are LIFTED** (`sim/`, host-validated via `just sim-test`, 33 checks).
Concrete continuations, any order:
(a) **close the in-game differential loop** — read the 4 locomotor constants (`DAT_1408007dc/d4/f4`,
`DAT_1408754bc`) from the binary (one Ghidra/objdump pass), then replay a fixed save and diff the
lifted integrator's `entity+0x78` output vs the live DIFFTRACE `DTPOS` stream — the **first true
oracle check** and the highest-value validation milestone;
(b) **finish unit 3's steering bodies** — lift accelerate `622b80` + turn `622e90` (set
`accel_factor`/`steer_blend`/heading from the destination + template) and the Walk `0x61e930`
variant, replacing those `LocomotorEnv` callouts;
(c) **unit 4 — hardpoint fire-control** (`a76b0`/`387010`/`387400` …) to broaden the slice.
Recommended: (a) — it converts host-validated lifts into an oracle-validated integrator and
exercises the differential-test methodology end-to-end for the first time. Two
front-loads: (a) the determinism pins (Phase 4) — the `a76b0` order-sensitive float sum + RNG draws —
since they are *rewrite* risk surfaced early; (b) classify each IN behavior's writes as Phase-A
(own-entity, parallel) vs Phase-B (cross-entity, deferred-command) as it is lifted, since that split
*is* the rewrite. Before heavy lifting, apply the recovered struct layouts as Ghidra types so the
decompiles read as `entity->position`/`entity->behaviors[i]` rather than raw offsets.
