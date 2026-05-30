# Engine Source Decompilation Plan

**Status:** Plan / scoping document (2026-05-30). Nothing here is built. Companion to
`threading_model.md` → *Engine-Source Sim-Parallel Rewrite — Boundary Scope*, which defines
*what the rewrite needs*; this defines *how we'd obtain the source it needs*.

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

**Remaining Phase-1 work (next units):**
- **Call-graph attribution** — name the non-virtual helpers (e.g. `FUN_140387400` path-follow) by
  proximity to the named Locomotor/Coordinator vmethods that call them. Biggest remaining naming win.
- **Differential-test harness** in the hook DLL — per-tick world-state checksum vs the live binary
  (the correctness oracle for Phases 3–4).
- (Struct DB is folded into Phase 2.)

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

### Phase 2 — Struct & type recovery (weeks) — *the foundation*
Everything downstream is unreadable without this. Recover and document the layouts of:

- **Entity / game object** (we already touch `+0x38` listener list, `+0x60` tick field, `+0x3a0`
  flags, `+0x2d8` lua ctx, `+0x121` shutdown — consolidate into a real struct).
- **GOM (game object manager)** and its entity lists / spatial structures.
- **Game-mode objects + vtables** (mode vtable slot `0xb0` = service, etc. — partially mapped).
- **World/sim state** = exactly the set that must be **double-buffered** (boundary work item #2).
- **Command/order queue** `DAT_140b27e60` and order structs (boundary work item #3).
- **RNG + tick clock** (`DAT_140b0a320` counter, `DAT_140b0a340` rate) — determinism inputs.

Output: `openspec/data_structures/` entries + applied Ghidra types. **Gate:** the world-state
write-set from Model C scoping must be fully typed before Phase 3 — you can't classify writes you
can't name.

### Phase 3 — Sim-core function lift (months) — *the critical path*
Decompile → clean → re-implement in C++, validated against the oracle, in dependency order:

1. **Tick clock / scheduler** — `FUN_14027c360`, `DAT_140b0a320/340`, the FF gate `DAT_1409cf314`.
2. **Game service / loop spine** — `FUN_14028d400` (gsvc), `FUN_1403a76b0`.
3. **Per-component movement / path-follow** — `FUN_140387010` → `FUN_140387400` (the ~1700/tick
   bulk; this *is* the Phase-A body of the rewrite), deep path `FUN_1403825b0`, nav `FUN_14029f810`.
4. **Cross-entity write sites** — `FUN_1403846c0` (target listener list), `FUN_1402d5290` (global
   queue) — these become *command emitters* in the rewrite.
5. **Command-queue drain / apply** — the serial Phase-B target.

Each function: decompile → reconstruct types/control flow → C++ port → diff sim-state checksum vs
oracle over a fixed save+replay until bit-identical. **This slice is the deliverable that unblocks
the rewrite.** The boundary-scope work table maps 1:1 onto restructuring *these* functions into
Phase A / Phase B.

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

Run **Phase 0** — it's days of cheap, automatable work (RTTI pass + string auto-namer + SDK/Lua-API
recon) and its output is the only thing that turns "person-decade" into a real estimate for the
sim-core slice. Everything else is gated on that number.
