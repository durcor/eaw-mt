# Threading Model

**Status:** Phase 1.2 string survey complete. Named threads confirmed. Architecture TBD pending Phase 1.3 game loop analysis.
**Last verified:** 2026-04-18

---

## Confirmed Thread Roster (from string evidence)

| Thread Name | String RVA | Notes |
|-------------|-----------|-------|
| `Main Thread` | 0x00854DA8 | Primary game loop thread |
| `LoadThread` | 0x00805150 | Asset loading (async I/O) |
| `PacketHandler Thread` | 0x00851FE8 | Network packet processing |
| `NATUtilsThread` | 0x00853530 | NAT traversal / networking util |
| `LuaScriptThread: Main State` | 0x00855CD0 | Primary Lua state thread |
| `LuaScriptThread: %s` | 0x00855CF0 | Named Lua coroutines/threads |

**Confidence:** High — these are named thread identifiers used in diagnostics and error messages.

The engine already runs **multiple named threads**. It is not single-threaded. Threading primitives in the import table are in active use.

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

## Known Threading Issues / TODO

- [ ] Phase 1.3: Locate `Main Thread` creation — is it the process main thread or an explicitly created thread?
- [ ] Phase 1.3: Locate `LoadThread` entry point — what assets does it load asynchronously?
- [ ] Phase 1.3: Determine if `LuaScriptThread`s are OS threads or Lua coroutines
- [ ] Phase 2: Map render task flush path — where does `alRenderTask` list get consumed?
- [ ] Phase 2: Determine if game logic and render submission are currently serialized or pipelined
- [ ] Phase 2: Map `ThreadLockMutexClass` usage sites — what state is guarded?
