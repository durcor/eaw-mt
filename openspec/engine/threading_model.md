# Threading Model

**Status:** Phase 5 — Model A (render) hook installed but offload **dormant** (flush currently runs synchronously on the main thread; the render thread is created but unkicked per-frame — async was deferred, then backed off to serial during crash debugging). Measured ceiling is ~0 anyway: render flush = 0.01ms avg / 1ms max even under heavy load (DXVK executes draws async; CPU-side flush is negligible). Models B (AI) & C (movement) scoped & ruled out (pervasive shared state). Sim-side CPU parallelism beyond render not viable from a hook; the engine-source rewrite that *would* enable it is scoped below. Multithreading-for-perf goal CLOSED at the hook level.
**Last verified:** 2026-05-29

---

## OS Thread Roster — COMPLETE (Phase 1.4C)

All thread creation in `StarWarsG.exe` goes through a single wrapper `FUN_0x22e490` which calls `_beginthreadex`. That wrapper has exactly **5 call sites** creating **3 distinct thread types**:

| Thread Name | Creation Site RVA | Thread Class | Notes |
|-------------|-------------------|--------------|-------|
| Main Thread | (WinMain entry) | — | Game loop, all Lua, all game logic |
| LoadThread | `0x8df00` | `LoadingThreadClass` | Asset loading; owns loading screen loop |
| PacketHandler Thread | `0x2056d0`, `0x205de0`, `0x2059f0` | `PacketHandlerClass` | Network packet processing; can be re-bound |
| NATUtilsThread | `0x225bf0` | `NATUtilsClass::NATUtilsThreadClass` | NAT traversal |

**Total: 4 OS threads** (main + 3 workers). This is the complete picture — there are no other `CreateThread` or `_beginthreadex` calls anywhere in the binary.

**LuaScriptThread strings are Lua coroutine labels, not OS thread names.** See script_engine.md.

---

## Threading Abstractions (class names recovered from RTTI)

### `ThreadLockMutexClass`
- Custom Petroglyph mutex wrapper (string RVA 0x00851BF0)
- Has **deadlock detection**: logs owner name if mutex not obtained within **10 seconds**
- Error strings: `"ThreadLockMutexClass -- %s failed to obtain mutex within 10 seconds (current owner is %s)"`
- Wraps Win32 `CreateMutexA` / `ReleaseMutex`

### `FastThreadLockClass`
- RTTI: `.?AV?$PooledObjectClass@VPacketClass@@$0BE@VFastThreadLockClass@@@@`
- Lighter-weight lock used in packet buffer pools — likely a spinlock or critical section wrapper
- Used at least in `PacketClass` pooling

### `LuaCreateThread` (RTTI: `.?AVLuaCreateThread@@`)
- Lua-exposed API for creating threads from script
- Methods: `Get_Name`, `Kill`, `Is_Thread_Active`
- Error strings confirm it wraps function-name or function-reference creation
- Lua scripts can create and kill threads at runtime

### `ThreadValue` (RTTI: `.?AVThreadValue@@`)
- Lua-exposed thread-local value accessor
- Methods: `Get(string key)`, `Set(string key, value)`
- Provides per-thread named storage accessible from Lua scripts

### `LuaGetThreadID` (RTTI: `.?AVLuaGetThreadID@@`)
- Lua API function: returns current thread's numeric ID

### `NATUtilsThreadClass` (RTTI: `.?AVNATUtilsThreadClass@NATUtilsClass@@`)
- Concrete thread class for NAT utility operations

---

## Script Engine Threading

`LuaScriptThread` names confirm the Lua engine runs **multiple concurrent states**:
- A "Main State" (maps to the galactic map / primary script context)
- Additional named threads (space/land battle scripts, story arcs)

`LuaNetworkDebuggerClass` (strings RVA 0x008561C0+) exposes full thread introspection:
- Lists all threads by ID+name
- Supports per-thread breakpoints
- Callstack inspection per thread

This confirms Lua threads are real runtime entities, not just coroutines — they have IDs, names, and can be debugged over the network.

---

## Script Paths (source path fragments)

```
Data/Scripts/AI/SpaceMode/   (RVA 0x008799F0)
Data/Scripts/AI/LandMode/    (RVA 0x00879A10)
Data/Scripts/AI/StoryArc/    (RVA 0x00879A30)
.\Data\Resources\GUIDialog\Resource.h  (RVA 0x00800A08)
```

---

## Render Task System

RTTI reveals a **task-based renderer**:

| Class | Description |
|-------|-------------|
| `alRenderTask` | Base render task |
| `ValMeshRenderTask` | Mesh draw call |
| `ValD3dTerrainRenderTask` | Terrain draw |
| `ValD3dTerrainDecorationRenderTask` | Terrain decoration |
| `ValD3dWaterRenderTask` | Water surface |
| `ValParticleGroupRenderTask` | Particle systems |
| `ValSimpleRenderTask` | Simple geometry |
| `Val3DCableRenderTask` | Cable/rope effects |
| `ValLaserBeamFlushTask` / `ValLaserKiteFlushTask` | Laser FX |
| `ValBlobFlushTask` / `ValLensFlareTask` | Lens flares |
| `ValSplineRendererRenderTask` | Spline paths |
| `ValLightningBoltRenderTask` | Lightning |

`MultiLinkedListClass<alRenderTask>` — render tasks are queued in a linked list. The "Flush" task variants suggest a **two-phase render**: accumulate tasks, then flush/submit. This is the insertion point for render thread separation (Model A).

Error string: `render task for %s exceeded the dynamic geometry limits (%d pverts, %d lverts, %d indices)` — confirms tasks carry geometry budgets.

---

## Phase 5 — Model B (Lua Coroutine Pump Offload) Scoping (2026-05-29)

Scope of "offload `Pump_Threads` (`FUN_140247a90`) to a worker thread" given everything Phase 5 learned.

### The two-level AI architecture (why offload is hard)
Per `game_loop.md`, per-unit AI is split:
1. **Pump_Threads** (`FUN_140247a90`, via `FUN_140488660::Service`, rate-limited by `ServiceRate`) — advances the high-level Lua AI coroutines. Runs at frame step 1 (mode `vtable[0x158]` → `FUN_14035cc10`).
2. **Command queue** (`FUN_1403b08c0`, step 6, every frame) — drains queued unit orders; the native execution of what the Lua AI requested.

On the main thread these are **serialized** (pump at step 1, command queue at step 6), so the pump's reads and the command queue's writes never overlap. Offloading the pump breaks that serialization.

### Conflict surface (reads AND writes — larger than the blueprint's original gate)
The blueprint (Section 6, Model B) flagged one hazard: yield-type-1 (`FUN_140247700` case 1) **reads** the C++ entity hierarchy (`entity->list[4]/[5]`) that the command queue writes. But Phase 5 proved the surface is bigger:
- **The AI Lua C closures WRITE pervasive shared simulation state during `lua_resume`** — fog-of-war, entity spawns, resource counts, and they touch vtable objects. Direct evidence: prewarm had to **skip** `cap<4096` entities because "their Lua AI issues side-effecting C callbacks (fog-of-war writes, entity spawn, etc.) that modify game state outside the Lua VM; there is no way to undo those" (winmm_proxy.c prewarm skip path). These writes do **not** all funnel through the command queue.

### Empirical result: naive Model B fails (Step 30)
Dispatching `g_pumpe_orig` to a background worker (`pumpe_bg_worker`, still compiled but disabled) caused **main-thread vtable corruption → crash (`av_read`@heap addr)**: the worker's C-closure reads/writes raced the main thread. Naive offload is ruled out. The blueprint's proposed mitigation (snapshot/read-lock for yield-type-1) is **insufficient** — it addresses reads only, not the pervasive writes.

### Options
| Option | Idea | Verdict |
|---|---|---|
| **B0 naive** | dispatch pump to worker, join next frame | **Ruled out** — Step 30 crash |
| **B1 global sim lock** | worker holds lock; main acquires before any sim touch | Serializes pump vs all sim → ~no parallelism gain (render is already split by Model A) |
| **B2 snapshot-and-defer** | worker runs on an immutable sim snapshot; ALL writes captured as deferred ops applied on main thread post-join | The "correct" model, but requires enumerating & redirecting **every** AI C-closure side-effect (large, fragile; engine command queue covers only some) |
| **B3 partition** | offload only pure-compute coroutines (no C side-effects) | Needs per-coroutine closure analysis; uncertain payoff |

### Motivation has shifted — the original "why" is largely solved
Model B existed to remove AI-pump frame hitches. Phase 5 removed them by cheaper means: per-gsvc budget (`PUMPE_BUDGET_MS=33`) bounds a pass, steady-state `rv_max≈1ms`, and the first-encounter stall is fixed by `fscache` (~14s→~1.1s). **So Model B no longer fixes a current problem** — its value is now "multithreading headroom for the blueprint's mission," not "fix a hitch."

### Recommendation
Do **not** start Model B coding now. Either (a) declare the AI-perf objective met by the cheaper budget+fscache approach and pursue multithreading elsewhere (e.g., revisit Model A profiling, or a subsystem with a cleaner conflict surface), or (b) if Model B is pursued for the mission, the **prerequisite** is a complete enumeration of the AI C-closure write set (the deferred-apply surface for B2) — that RE must precede any threading code, with a human-approval gate.

### Open RE questions (prerequisites for any Model B work)
- Enumerate the C closures reachable from `lua_resume` during a pump and classify each: pure-compute vs. sim-write (and which state each writes).
- Do any AI writes already go through the command queue (`FUN_1403b08c0`), or are fog/spawn/resource writes all direct? (Determines B2 feasibility.)
- Is the main-thread pump actually a frame-time bottleneck now (post budget+fscache)? Profile before investing.

### Binding-audit result (2026-05-29) — B2 verdict: NOT tractable
The prerequisite audit (`script_engine.md` Phase 5; scripts `Phase5LuaApiSurface` /
`Phase5EnumGameApi` / `Phase5DecompGameBinding`) answered the gating question: **the AI's
game-state access is a generic reflection dispatcher, not an enumerable closure set.** All
game objects share one metatable ("LuaWrapperMetaTable"); every `obj:Method()`/`obj.Prop`
(read or write) funnels through `__index` = `FUN_14024a8a0`, which hashes the method name and
looks it up in a per-type C++ method table. So:
- There is **no small write-closure set to redirect** — the write surface is the entire
  reflected game-object API (hundreds of methods across all types), with no read/write flag
  exposed at the binding layer.
- B2 (snapshot-and-defer) would require either classifying every reflected C++ method by hand,
  or treating the whole dispatcher as an opaque sim-mutation boundary and serializing it — i.e.
  collapsing to B1 (global lock), which yields no parallelism gain over the current
  main-thread pump.

**Verdict:** Model B is not worth pursuing. The AI-perf objective it targeted is already met by
the per-gsvc budget + fscache. The single dispatcher (`FUN_14024a8a0`) is noted as the unified
choke point should this ever be revisited, but the cost/benefit is firmly negative now.
Recommend pursuing multithreading (if at all) in a subsystem with a cleaner conflict surface,
or treating the AI-threading goal as closed.

---

## Phase 5 — Model C (Movement / per-component path-following) Scoping (2026-05-29)

The fast-forward choppiness is the movement service tick: `gsvc(28d400)` avg ~21ms, dominated by
`FUN_140387400` (path-following) called ~2,147×/tick (N entities × ~55 components). Per-call cost
is now low (Source-B asset stall fixed), so it's pure *volume* — the one apparent
parallelization candidate (a per-component fork-join, blueprint Model C).

### Scoped (decompiled `FUN_140387400` + its work-path callees) — NOT cleanly viable
`FUN_140387400`'s prologue is a long guard chain (component-local reads → early return), but the
portion that **passes the guards and does work** is interleaved with cross-object and global
writes, confirmed by decompile:
- **`FUN_1403846c0` (release previous target)** writes the *target entity's* `+0x38`
  listener/subscription list via `FUN_140220eb0(ctx, target+0x38, component, …)` — a
  **cross-entity** write (one component mutates another entity's state).
- **`FUN_1402d5290` (event dispatch)** → `FUN_1402d72c0(&DAT_140b27e60, …)` — a **global**
  event/timer-queue write.
- **`FUN_1403825b0`** deep path (the ~1.3%) creates movement orders — heavy shared writes
  (alloc + target acquisition across entities).

So component-level (and even entity-level) parallelism races on cross-entity target listener
lists and global queues. The parallel-safe part (the guard/early-return bulk) cannot be isolated
from the shared-write part because they are interleaved inside one game-binary function we cannot
refactor. Locking the shared callees would serialize the work and add per-call overhead, negating
the gain.

**Verdict:** Model C is not cleanly viable either — same root obstacle as Model B: EaW's
simulation has pervasive cross-object shared mutable state threaded through the per-entity /
per-component logic.

## Overall threading conclusion (2026-05-29)

After scoping all candidates: **Model A (render thread) is the only viable split** — it works
precisely because the renderer is a one-way consumer of a task list. Model B (AI) and Model C
(movement) are both intractable: the simulation mutates shared cross-object state (global_State,
entity target/listener lists, global event queues) pervasively and interleaved, with no clean
compute/apply boundary we can exploit without engine source. **Sim-side CPU parallelism beyond
the render thread is not viable here.** The realized performance wins on this title are the
render-thread split plus I/O/algorithmic caching (fscache, MEG index, nf_cache, b25ec10 result
cache) — not additional worker threads. Treat the multithreading-for-sim-perf goal as closed.

---

## Engine-Source Sim-Parallel Rewrite — Boundary Scope (2026-05-29)

Scoped at user request to **define what lies on the far side of the hook wall** — i.e. what it
would take to actually parallelize the fast-forward sim, the only lever measured to be a *notable*
improvement (FF frame is ~100% sim: `inter`≈4.5ms vs `flush`≈0.01ms). This is a boundary-definition
document, **not** a build plan; all of it requires engine source and is out of scope for binary
hooking.

### Why a hook can't do it (recap, measured)
Three independent walls, each confirmed this phase:
1. **Ticks are sequential** — tick N+1 reads the world state tick N writes (hard RAW dependency); no temporal pipelining.
2. **Intra-tick entity writes cross objects** — decompile-confirmed: a component writes *another* entity's `+0x38` listener list (`FUN_1403846c0`), the global order queue `DAT_140b27e60` (`FUN_1402d5290`), and the shared Lua `global_State`.
3. **No compute/apply boundary, and we can't insert one** — the parallel-safe work is interleaved with the shared writes inside single game-binary functions we cannot refactor from a hook.

### Target architecture: deterministic compute → apply
Convert the per-tick sim from "iterate entities, mutate shared state inline" to two phases:
- **Phase A (parallel, read-only):** each entity/component reads a *frozen* snapshot of last tick's
  world state and emits its intended mutations as **deferred ops** into a thread-local command buffer. No shared writes.
- **Phase B (serial):** a single thread drains all command buffers in a **canonical deterministic
  order** and applies them to the world state.

This is the standard command-buffer / SoA-ECS structure. Retrofitting it onto Alamo requires:

| # | Work item | Grounded in | Difficulty |
|---|---|---|---|
| 1 | **Restructure the tick** into Phase A / Phase B; the gsvc movement service (`FUN_14028d400` → `FUN_1403a76b0` → per-component `FUN_140387400`) becomes the Phase-A body, emitting commands instead of writing. | The ~1700 path-follow components/tick are the bulk of the tick. | High — touches the core loop. |
| 2 | **Double-buffer all sim state read during a tick** so Phase A sees a consistent frozen world: entity transforms/health/target ptrs, the entity lists, spatial structures. | Phase A must not observe mid-tick mutations. | High — memory + every read site. |
| 3 | **Command schema + buffers** for every write kind: move order, target acquire/release, damage, spawn, resource delta, fog reveal, listener subscribe/unsubscribe. | The write set found in Model C scoping (cross-entity + global queue writes). | Medium — but the schema must be exhaustive (see #6). |
| 4 | **Determinism preservation** — apply phase must yield bit-identical results to the serial order (canonical sort, e.g. by entity ID); no scheduling-dependent float-accumulation order. | EaW is lockstep-deterministic for **multiplayer + replays + save/load**. Any divergence = desync / broken replays. The FF force-sum loop in `FUN_1403a76b0` accumulates floats across members — order-sensitive. | **Hardest constraint** — pervades every system; the reason naive parallelism is unacceptable, not just unsafe. |
| 5 | **Lua VM isolation** — the AI pump runs coroutines that issue side-effecting C callbacks through the shared `global_State` via the single reflection dispatcher (`FUN_14024a8a0`). To parallelize AI you'd need per-AI `lua_State`s (memory blowup; they share game-object refs) or a copy-on-write VM. | The prewarm corruption (Step 35) proved how fragile the shared VM is. The dispatcher has no read/write flag to gate Phase A. | Very high — but **avoidable** (see partition). |
| 6 | **Exhaustive write-set classification** — every reflected C++ method + direct write must be tagged read vs write to know what becomes a command. | Binding audit (`script_engine.md`): access is a generic reflection dispatcher over hundreds of methods, no read/write flag. **With source** this is an audit; **without** it is per-function RE. | High volume, but mechanical with source. |

### Realistic partition (what you'd actually parallelize)
You would **not** parallelize everything — you'd peel off the write-light, high-cost subsystems and
keep the coupled ones serial:
- **Parallel (Phase A):** movement integration / path-following (the ~1700 components — the position
  math is per-entity; the cross-entity target/queue writes become commands), particle/FX sim, collision broad-phase.
- **Serial (Phase B):** AI Lua pump (already ~0.02ms/tick, rate-limited — cheap to leave serial,
  which sidesteps work item #5 entirely), command-queue drain, fog/resource/spawn application, the deterministic apply.

### Amdahl ceiling (measured)
Favorable inputs: in FF the tick is almost all movement service — AI pump ≈0.02–0.04ms, cmd-queue
≈0ms. So the parallelizable fraction is high (~80–90% of the tick), serial tail = apply + AI + cmd-queue.
With the cross-entity writes (a few hundred commands/tick, microseconds each to apply) the serial
apply is small. Estimated speedup ≈ 1/(s + p/N): for s≈0.15, **N=4 → ~2.9×, N=8 → ~4×**. So a
correct rewrite could make fast-forward roughly **3–4× faster** — real, but bounded (not linear),
and gated entirely on getting determinism (#4) and the snapshot (#2) right.

### Feasibility verdict
- **Without engine source: not feasible.** It requires changing struct layouts, calling conventions,
  and the core loop — none reachable by patching a closed binary. You would be "rewriting" functions
  you can only read as disassembly.
- **With engine source: large but tractable** — a months-scale, determinism-audited refactor (core
  loop restructure + state double-buffering + command system + a full replay/multiplayer determinism
  test gate). The Lua VM (#5) is the worst subsystem and is best **avoided** by keeping AI serial,
  which the measured AI cost (~0.02ms/tick) makes painless.

**Bottom line:** the biggest possible win (≈3–4× fast-forward) and the blocked lever are the same
lever. It is unreachable from a hook by construction; it becomes a bounded, well-understood refactor
only with source. This closes the multithreading investigation: the hook-level frontier is exhausted,
and the source-level frontier is now scoped and documented as the boundary.

---

## Phase-B Seam Characterization — the IN-behavior cross-boundary write set (2026-06-03)

The **phase pivot.** Across the Phase-3 sim-core lift, every cross-entity IN behavior
(#4 UnitAI, #6 AsteroidFieldDamage, #7 Targeting, #8 Telekinesis, #9 Lure, #10 Reveal) bottomed
out in the same residual: a *deferred Phase-B seam* — the cross-boundary write each behavior emits
once its own pure numeric core is computed. Those seams were tracked separately as "the shared
Phase-B seam" pending characterization. This section is that characterization: decoding the actual
write-target functions (`2d5320`, `657ce0`/`657db0`/`659760`, `365760`/`35d1b0`, `29f810`, `62b270`)
and the global structures they touch resolves the work-item #3/#4 "command schema + determinism"
question above for the IN slice.

**Headline finding: it is not one seam — it is three write-target classes with three different
verdicts.** "Serialise-or-shard the cross-boundary write" was the right frame, but the answer is
not uniform; classifying by write target is what makes the parallelization tractable.

### The shared structures (decoded)
- **Global object registry `DAT_140a16fd0`** — an **id-indexed `Object*` vector** (`+0x20` = pointer
  array, `+0x28` = count; lookup `FUN_140294bc0(reg, id)` bounds-checks then returns `reg[+0x20][id]`).
  Every scan reads it; only `CreateObject`/destroy mutate it. The id↔slot identity *is* the lockstep
  contract — object ids are append order.
- **Global SensorGrid singleton `DAT_140b31900`** — passed as `param_1` to the #4 grid writers.
  Internally **pre-sharded by observer**: `grid+0x40` is an array of per-observer cell-bucket grids
  indexed by `observer_slot*0x18`; `grid+0xb0/0xb4` = grid dims, `+0xb8/0xbc` = cell sizes. The
  observer's slot is `*(int*)(observer+0x48)` — unique per observer.
- **Shared sim-RNG seed `DAT_140a13e24`** — the lockstep LCG `s' = s*0x41c64e6d + 0xbdf` (lifted as
  `eaw::SimRng`); advanced a membership-dependent number of times per tick (#6: one draw per in-range
  asteroid). The draw *order* fixes the stream.

### Class 1 — Sensor / visibility / fog writes → **shardable by observer, NO serialization**
Behaviors: **#4 UnitAI** (`657ce0` became-visible, `657db0` became-hidden, `659760` recache-moved),
**#10 Reveal** (`365760`, `35d1b0`).
- **Every write word is keyed by the writing observer's own unique slot index.** `657ce0` writes each
  object's per-observer arrays (`object+0x40/+0x58/+0x70/+0xa0`) at column `[observer_slot]`; `657db0`
  scatters into `grid+0x40 + observer_slot*0x18` (that observer's *own* cell-bucket grid); #10's
  `365760`/`35d1b0` write the behavior's **own** array `this[0x33]` (size `this[0x32]`). No two
  observers ever touch the same memory word.
- **Reads** are the global registry `DAT_140a16fd0` (read-only id→ptr) + the visibility/team predicate
  (`FUN_14039a2c0` #10 / `FUN_14035f470` #4) reading *other* objects' fields read-only.
- **Determinism:** within an observer, objects are visited in registry order (deterministic);
  cross-observer there is no shared write → fully order-independent. **No command buffer, no serial
  drain.** The only requirement is a **sweep barrier**: the global registry + grid geometry must not
  be mutated (no concurrent `CreateObject`/destroy) *during* the parallel observer pass.
- **Verdict: embarrassingly parallel by observer.** #4/#10 — and the read-only-coupled #5 Nebula —
  parallelize directly. This is the cheap, large win (these are per-entity-per-tick passes).

### Class 2 — GameObjectManager mutation + shared RNG → **MUST serialize (buffer + ordered commit)**
Behaviors: **#6 AsteroidFieldDamage** (`29f810` impact-fx spawn + per-asteroid LCG draw); the general
spawn path for **any** behavior that creates an object (projectiles for **#7**'s fire emit, etc.).
- `FUN_14029f810` = **`GameObjectManager::CreateObject`** (2201 bytes): allocate (`769c58(0x8a0)`) →
  construct (`388b60`) → **append to the shared registry `DAT_140a16fd0`** (assigns the next object id)
  → **insert into ~12 global spatial/category buckets** (`20a9b0` into `mgr+0x38/+0x408/+0xc8[slot]/
  +0x1c8/+0x210/.../+0x5a8`) → grow the global growable array `mgr+0x3c8` (realloc, count `+0x3d0`) →
  sweep all objects to notify observers (`294bc0`+`2823e0`+`289010`, lines 229-242).
- **Two hard-serial hazards:** (a) **object-id assignment is append order** — if two shards create
  objects, every new id (and thus all downstream object identity) changes → lockstep desync / broken
  replays; (b) the **shared LCG `DAT_140a13e24`** is global — the draw sequence must be a fixed order.
- **Verdict: serialize.** Buffer spawn requests + RNG draw counts into per-shard queues in Phase A;
  drain in **canonical entity order** (entity id / registry index) in Phase B, assigning ids and
  advancing the LCG in that fixed order. This is the real bottleneck — but it is **low-frequency**
  (object spawn + occasional rolls, "a few hundred commands/tick"), not per-entity-per-tick, so the
  serial tail stays small (consistent with the Amdahl estimate above).

### Class 2b — cross-entity gameplay-state writes (no alloc, no RNG) → **buffer + ordered apply**
Behaviors: **#9 Lure** (`62b270`: order the lured unit `FUN_14038d730(victim, lure, …)` + mutate a
shared AI group/plan via `426010`/`425e30`/`426d50` off `DAT_140b15418`), **#7 Targeting** target-assign
(writes the *target's* state via the selection/fire-control FSM — emit level not yet fully decoded).
- Writes a *foreign* entity's order/AI state — **aliasable** (two writers can hit the same victim), so
  not slot-shardable like Class 1, but no allocation and no RNG, so cheaper than Class 2.
- **Verdict: buffer as commands, apply in canonical order.** Order matters only when two emitters
  target the same victim; a deterministic emitter sort resolves it. This is exactly work-item #3's
  "target acquire/release" command kind.

### Class 3 — SFXEvent dispatch → **presentation, NOT a sim hazard (correction)**
Behaviors: **#8 Telekinesis** HOLD/RELEASE (`2d5320`).
- **`FUN_1402d5320` is the SFXEvent player**, not a GOM/gameplay write: it checks the SFXEvent's 2D/3D
  flag (`+0x29`/`+0x2a`), forwards to `2d5440` to play, else logs *"SFXEvent '%s' not 2D or 3D
  specified"*. Sound only. (The adjacent `FUN_1402d5290` is the *same* SFX path — both `2d5320` and
  `2d5290` call the SFXEventManager start `2d5440`; per `sim/command_sink.h` the queue `DAT_140b27e60`
  that `2d5290` feeds is the **SFX event queue** — wall-clock-sorted on a SEPARATE LCG `DAT_140a13e20`,
  NOT a gameplay order queue. The genuine gameplay scheduled-event queue is `OutgoingEventQueueClass`
  `DAT_140b2ed18`, a galactic/strategic-layer MP queue *outside* this tactical sim slice. An earlier
  boundary-scope note above labelled `b27e60` a "global order queue" — that label is superseded here.)
- **Correction to prior tracking:** #8's deferred "event dispatch `1402d5320`" was assumed a
  GOM/state write. It is audio playback — outside lockstep state, no determinism impact.
- **Verdict: not a Phase-A sim hazard.** Buffer SFX triggers and flush post-sweep (or hand to the
  audio thread); ordering affects only which sound plays. With SFX peeled off, **#8 Telekinesis is
  Phase-A-safe** — its GRAB/HOLD/RELEASE numeric cores all write own-entity state only.

### Per-behavior seam map
| Behavior | Cross-boundary write | Class | Verdict |
|---|---|---|---|
| #4 UnitAI | sensor grid `DAT_140b31900` (`657ce0`/`657db0`/`659760`) | 1 | shard by observer |
| #5 Nebula | (read-only coupling only) | — | Phase-A-safe as-is |
| #6 Asteroid | `CreateObject` fx (`29f810`) + LCG `DAT_140a13e24` | 2 | serialize (buffer + ordered commit) |
| #7 Targeting | target-assign + projectile spawn (selection/fire FSM) | 2 / 2b | buffer + ordered apply (emit not fully decoded) |
| #8 Telekinesis | SFXEvent (`2d5320`) | 3 | presentation → defer; behavior is Phase-A-safe |
| #9 Lure | order lured unit + AI group/plan (`62b270`/`38d730`) | 2b | buffer + ordered apply |
| #10 Reveal | fog reveal arrays `this[0x33]` (`365760`/`35d1b0`) | 1 | shard by observer (observer-private) |

### Movement / per-component slice (extension, 2026-06-03)
The movement service is the bulk of fast-forward time (the ~1700 path-follow/fire-control
components/tick). Chain: the gsvc dispatcher `FUN_14028d400` (battle/network-vote coordinator —
PENDING_TACTICAL_BATTLE_VOTE, outgoing event queue) drives the per-object hardpoint service
`FUN_1403a76b0`, which ticks each hardpoint via `FUN_140387010` → the per-component body
`FUN_140387400` (opportunity-targeting / fire-control) + `FUN_140381ff0` (firing). Decoded write set:
- **Own-component / own-object state** (target ptr, `in_progress_flag`, `field_0x64`,
  `opp_target_slot`, `last_serviced_tick`, `cached_bone_idx`; the energy redistribution `387f50` to the
  object's *own* hardpoints) → **safe**, provided one object = one work unit.
- **SFX cue `DAT_140b27e60`** (`387400:99` → `FUN_1402d5290` → `2d72c0`) → **Class 3** (presentation).
  ⚠️ **Correction** to a first pass that called this a Class-2 "order queue": `2d5290` calls the same
  SFX start `2d5440` as `2d5320`, runs on a separate LCG `DAT_140a13e20`, and sorts by wall-clock —
  so it is *not* a gameplay/lockstep write at all (per `sim/command_sink.h`). No Class-2 global
  order-queue write exists in this tactical slice; the gameplay scheduled queue (`OutgoingEventQueueClass`
  `DAT_140b2ed18`) is galactic-layer.
- **Cross-entity per-object signal dispatcher `other+0x38`** — `FUN_1403846c0` and the
  `FUN_140220ed0`/`220e90`/`220eb0` emits/connects (signals 0x20 fire-order, 0x21 opp-target-acquired,
  0x28 detach) fire on *another* object's `+0x38` `SignalDispatcherClass`, whose connected slots mutate
  arbitrary entities. → **Class 2b** (aliasable; buffer + ordered apply). This is the same `+0x38`
  listener seam the boundary scope above named as hazard #2; `sim/command_sink.h` already frames it as
  the `CommandSink::emit_signal` channel (insertion-order-within-emission + GOM-order-across-entities
  is the determinism contract).
- **Shared sim-RNG `DAT_140a13e24`** — `387400:250` draws (`FUN_1401ffb40`, int range) to randomize
  each component's opportunity-target scan start. → **Class 2 determinism hazard, but on the HOT path**
  (≈one draw per component per acquisition, not the rare #6 spawn roll).
- **Reads** the global registry `DAT_140a16fd0` read-only during scans → **sweep barrier** (Class 1).

Two refinements this slice adds (it introduces **no new write-target class**):
1. **The order-sensitive force-sum is intra-object.** The FF accumulation the boundary scope flagged
   (`3a76b0:44` `fVar11 += fVar9` over the object's hardpoints, then the proportional energy
   redistribute) sums over **one object's own** members. If the unit of parallel work is the whole
   object (all its hardpoints on one thread), the float-accumulation order is bit-identical to serial.
   **Resolution: shard by object, never split an object's hardpoints across threads** — a sharding
   *granularity* constraint, not a buffering requirement. The flagged determinism hazard dissolves.
2. **The lockstep LCG is a hot shared dependency here**, not low-frequency. Serializing every global
   draw would bloat the serial tail. The clean retrofit is **per-entity RNG substreams** (counter-based
   / splittable RNG seeded by entity id) so each shard draws from its own deterministic stream and no
   global draw-order serialization is needed — preserving lockstep without a serial bottleneck. (This
   also subsumes #6's asteroid rolls.)

### What this resolves
The IN-behavior write set (work-item #3) is **fully enumerated and classified**: two of three classes
need only a sweep barrier (Class 1) or a low-frequency ordered command drain (Class 2/2b), and the
third (Class 3) is not a sim hazard at all. The determinism contract (#4) reduces to two concrete
invariants — **(i)** object-id append order and **(ii)** LCG draw order — both satisfied by draining
the Class-2/2b command buffers in canonical entity order. The expensive per-tick mass (sensor/fog
sweeps + the movement integration already scoped) is Class 1 / read-only → directly shardable; the
serial tail is the Class-2/2b drain, which the measured Amdahl numbers already assumed small. This is
the concrete realization of the boundary-scope's "command schema + buffers" and "determinism
preservation" rows for the IN slice — still source-only to *build*, but the RE question of *what the
commands are and which can avoid the buffer entirely* is now answered.

The **movement slice** extension (above) folds in cleanly: same three classes, no new write target.
The two determinism worries the boundary scope raised about it both downgrade — the order-sensitive
force-sum is intra-object (fixed by object-granular sharding, free) and the hot LCG dependency is best
solved by per-entity RNG substreams rather than serialization. So the per-tick mass (movement +
sensor/fog) is Class 1 / shard-by-object / read-only — directly parallel — and the residual serial
tail across **both** slices is the same low-volume Class-2/2b drain (object spawn + per-object signal
fan-out) in canonical order, plus the presentation-only Class-3 SFX flush which is off the lockstep
path entirely. The remaining open RE item is the **command-schema spec** —
the concrete op/buffer types per class — which is now a writing task, not a decode task.

---

## Known Threading Issues / TODO

- [ ] Phase 1.3: Locate `Main Thread` creation — is it the process main thread or an explicitly created thread?
- [ ] Phase 1.3: Locate `LoadThread` entry point — what assets does it load asynchronously?
- [ ] Phase 1.3: Determine if `LuaScriptThread`s are OS threads or Lua coroutines
- [ ] Phase 2: Map render task flush path — where does `alRenderTask` list get consumed?
- [ ] Phase 2: Determine if game logic and render submission are currently serialized or pipelined
- [ ] Phase 2: Map `ThreadLockMutexClass` usage sites — what state is guarded?
