# Threading Model

**Status:** Phase 1.4 complete — OS thread roster fully mapped. Lua confirmed as coroutines.
**Last verified:** 2026-04-19

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

## Known Threading Issues / TODO

- [ ] Phase 1.3: Locate `Main Thread` creation — is it the process main thread or an explicitly created thread?
- [ ] Phase 1.3: Locate `LoadThread` entry point — what assets does it load asynchronously?
- [ ] Phase 1.3: Determine if `LuaScriptThread`s are OS threads or Lua coroutines
- [ ] Phase 2: Map render task flush path — where does `alRenderTask` list get consumed?
- [ ] Phase 2: Determine if game logic and render submission are currently serialized or pipelined
- [ ] Phase 2: Map `ThreadLockMutexClass` usage sites — what state is guarded?
