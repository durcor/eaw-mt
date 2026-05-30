# Engine Source Decompilation Plan

**Status:** Phases 0–2 EXECUTED (2026-05-30); Phase 3 SCOPED, not yet lifting. Companion to
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

1. **Tick clock / scheduler & determinism inputs.** `FUN_14027c360`, `DAT_140b0a320` (tick),
   `DAT_140b0a340` (rate), FF gate `0x9cf314`, game-speed-mode accessor (`DAT_140b15418` vfunc
   `0xe0` → 1/2). Smallest, no deps; establishes the tick.
2. **Entity-update spine.** `FUN_1403639d0` → `FUN_1402be640` → `FUN_1403a6b80` (minus the
   per-behavior bodies): the `ReferenceListClass<GameObject>` traversal (`node+0x8` next, `node+0x18`
   ref, `entity=*ref-0x18`), the behavior-dispatch loop (`entity+0x278` array / `+0x290` count,
   schedule + enable gates), and the child-unit recursion. **This is the loop the rewrite
   parallelizes over entities.**
3. **Locomotor movement integrators (the Phase-A body).** `LocomotorBehaviorClass::vfunc_6` per
   concrete type (Starship `0x6236b0`, Walk `0x61e930`, Fighter, Fleet, SimpleSpace, LandTeam*…) +
   the shared `LocomotorCommonClass::vfunc_6` pre-step + each one's `entity+0xa8`-state-machine
   helpers (e.g. Starship `622b80`/`622e90`/`559250`/`623340`, vector math `4aaa40`/`2bab90`). Reads
   `entity+0x78` pos & `+0xa8` state; writes `entity+0x78`. **Directly oracle-validated** (DIFFTRACE
   folds `entity+0x78` + motion state).
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
| other sim behaviors | the in-slice `BehaviorClass` subtypes on a typical unit (TBD) | **open** |
| command/event queue | `OutgoingEventQueue` + event types | ~10 |
| **Total** | | **~150–250 fns** (matches "hundreds") |

**The one open unknown — resolve FIRST in Phase 3 (gate from "scoped" → "lifting"):** the
heterogeneous **behavior set**. `FUN_1403a6b80` ticks *every* behavior on an entity via `vfunc_6`,
not just the locomotor — a unit carries many (locomotor, transform, select, targeting, abilities, …).
Only behaviors that **mutate determinism-relevant sim state** are in-slice; presentation behaviors
(render/UI/audio) are out. **First task: enumerate the behavior types on representative units**
(runtime RTTI-read of the `entity+0x278` behavior-array vtables — same technique that resolved
`HardPointClass`), then classify each `vfunc_6` as sim-mutating (in) vs presentation (out). This
sizes the real slice.

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

Phases 0–2 are done and Phase 3 is scoped (above). The next concrete action is the **Phase-3 gate
task: enumerate the behavior set.** Runtime-dump the `vfunc_6` vtables of each behavior in a
representative unit's behavior array (`entity+0x278`, count `+0x290`) via the proven in-hook RTTI
read (the one that resolved `HardPointClass`), then classify each behavior as sim-mutating (in-slice)
vs presentation (out-of-slice). That single capture turns "~150–250 fns" into a hard inventory and is
the gate between *scoped* and *lifting*. After it: begin the lift in dependency order (tick clock →
spine → locomotor integrators), diffing each unit against the DIFFTRACE oracle. Front-load the
determinism pins (Phase 4) — the `a76b0` order-sensitive float sum and RNG draws — since they are
*rewrite* risk surfaced early, not just decomp risk.
