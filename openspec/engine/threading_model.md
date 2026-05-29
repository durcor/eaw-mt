# Threading Model

**Status:** Phase 5 — Model A (render thread) shipped; Model B (Lua pump offload) scoped, NOT recommended as-is (see Phase 5 scoping section).
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

## Known Threading Issues / TODO

- [ ] Phase 1.3: Locate `Main Thread` creation — is it the process main thread or an explicitly created thread?
- [ ] Phase 1.3: Locate `LoadThread` entry point — what assets does it load asynchronously?
- [ ] Phase 1.3: Determine if `LuaScriptThread`s are OS threads or Lua coroutines
- [ ] Phase 2: Map render task flush path — where does `alRenderTask` list get consumed?
- [ ] Phase 2: Determine if game logic and render submission are currently serialized or pipelined
- [ ] Phase 2: Map `ThreadLockMutexClass` usage sites — what state is guarded?
