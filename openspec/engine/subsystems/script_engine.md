# Script Engine

**Status:** Phase 1.2 complete — string/RTTI evidence. Exact RVAs pending Phase 1.3 Ghidra decompilation.
**Last verified:** 2026-04-18

---

## Linkage

- **Statically linked** — no `lua*.dll` in import table
- Version: **unknown** — no version string found yet. Lua 5.1 or 5.2 probable (pre-2010 Petroglyph style). TODO: search for `LUA_VERSION` or `lua_ident` string.

## Thread Model

The script engine runs **multiple concurrent Lua states**, each named:

| Thread Name | Purpose |
|-------------|---------|
| `LuaScriptThread: Main State` | Primary Lua state (galactic map, global scripts) |
| `LuaScriptThread: <name>` | Named per-script states (space/land battle scripts, story arcs) |

Script paths confirm three AI script domains:
- `Data/Scripts/AI/SpaceMode/`
- `Data/Scripts/AI/LandMode/`
- `Data/Scripts/AI/StoryArc/`

**TODO:** Determine whether LuaScriptThreads are OS-level threads (`_beginthreadex`) or Lua coroutines managed on one OS thread. This is the critical question for Model B (Script Engine Offload).

## Lua Threading API (Lua-exposed)

The engine exposes a `LuaCreateThread` class to scripts:

| Method | Description |
|--------|-------------|
| `LuaCreateThread(func)` | Create thread from Lua function reference |
| `LuaCreateThread(name)` | Create thread from function name string |
| `LuaCreateThread::Kill(id)` | Kill thread by ID |
| `LuaCreateThread::Get_Name(id)` | Get thread name by ID |
| `LuaCreateThread::Is_Thread_Active(id)` | Poll thread status |

`ThreadValue` class provides per-thread named key/value storage (accessible from Lua via `Get(key)` / `Set(key, value)`).

`LuaGetThreadID` returns the current thread's numeric ID.

## Script Pool

`LuaScript::Free_Script_Pool` — scripts are pooled. String: `"LuaScript::Free_Script_Pool -- Script: %s:%8.8X is still in use."` confirms reference-counted lifecycle.

`ScriptPoolCount` — pool size is tracked.

## Network Debugger

`LuaNetworkDebuggerClass` provides a full remote debug interface:
- Thread enumeration with IDs and names
- Per-thread breakpoints (add/remove by script ID + thread ID + line + condition)
- Callstack inspection
- Variable dump (single var and table)
- Execute arbitrary text in script context
- Output forwarding to debugger

This is a non-trivial debug infrastructure — it confirms Lua threads are first-class entities with proper thread IDs and scriptable control.

## Key Classes

| Class | Role |
|-------|------|
| `LuaScriptClass` | Individual script instance |
| `LuaScriptWrapper` | C++ ↔ Lua bridge (Call_Function, Get/Set_Variable) |
| `LuaNetworkDebuggerClass` | Remote debug interface |
| `LuaCreateThread` | Lua API: create/kill threads |
| `ThreadValue` | Lua API: thread-local storage |
| `LuaGetThreadID` | Lua API: get current thread ID |
| `DatabaseObjectManagerClass<LuaScriptClass>` | Script registry |

## Thread Model — CONFIRMED: Lua Coroutines

**Phase 1.4C finding:** `LuaScriptThreads` are **Lua coroutines running on the main OS thread**, not OS-level threads.

Evidence: all `_beginthreadex` calls in `StarWarsG.exe` go through a single wrapper (`FUN_0x22e490`). That wrapper is called from exactly 5 sites, creating exactly 3 named thread types:
- `LoadingThreadClass` (LoadThread)
- `PacketHandlerClass` (PacketHandler Thread)
- `NATUtilsThreadClass` (NATUtilsThread)

There is no `LuaScriptThread` OS thread creation anywhere. The "LuaScriptThread: Main State" and "LuaScriptThread: %s" strings appear only in error handlers and the `LuaNetworkDebuggerClass` callstack formatter — they are Lua coroutine *labels*, not OS thread names.

The `LuaScriptClass` structure stores coroutine data at:
- `+0x40`: array of coroutine descriptors (each 0x40 bytes)
- `+0x48`: array end pointer
- `+0x58`: primary `lua_State*`
- `+0x60`: active coroutine index (−1 = main state)

## Lua Object Pool Tick — RVA `0x24bb80`

- **Size:** 351 bytes
- **Called:** every frame, unconditionally (WinMain main loop)
- **Purpose:** Maintains a pool of `LuaValue` objects. Each tick, allocates objects up to a cap of `0x200` (512). This is a pre-allocation / free-list top-up, not a GC step.
- **Shared state:** Writes to the `LuaValue` pool heap. If Lua is moved to its own thread, this tick must move with it — or be guarded if it ever shares the pool with C++ callers.

The actual Lua coroutine scheduler tick (where scripts run) is not yet identified. It is likely inside the scene manager (`FUN_0x30c3b0`) or the game-active branch, not in the unconditional ticks.

## Parallelization Assessment

- **Rating:** Medium risk (blueprint Model B) — **updated after Phase 1.4C**
- **Confirmed:** Lua coroutines on one OS thread. Moving to a dedicated OS thread is viable.
- **Approach:** Wrap all `lua_State` access behind a mutex or lock-free queue; move the Lua coroutine scheduler to a dedicated worker thread. Main thread posts events; Lua thread processes and posts results back.
- **Key risk:** Scripts that call back into game object state synchronously (same-frame reads/writes to shared data structures). Must audit Lua ↔ C++ binding functions before implementing.
- **No longer blocked** by the "OS threads vs coroutines" question — next blocker is the Lua ↔ C++ shared-state audit (Phase 2).

---

## Phase 5 — Lua ↔ C++ binding audit (Model B prerequisite, kicked off 2026-05-29)

Goal: enumerate the C closures reachable from `lua_resume` during a pump and classify each
pure-compute vs. sim-write — the deferred-apply surface for threading Model B (B2). Driver:
`tools/ghidra_scripts/Phase5LuaApiSurface.java` (scans `.rdata` for `luaL_Reg`-style
`{name*, fn*}` tables). Output: `logs/Phase5LuaApiSurface.out`.

### Result of the `luaL_Reg` scan — 6 tables, 88 entries
- **Stock Lua 5.1 stdlib** (~50 fns): base (`pcall`, `pairs`, `type`, `tostring`, …),
  `coroutine.*`, `table.*`, `string.*`. Table RVAs `0x8fe120/0x8fe2c0/0x8fe560/0x8fe8e0`.
- **Custom crc lib** (`crc`, `crcstate`, `md5`, `dumpstrtable`) @ `0x8fe660`.
- **D3D shader-constant table** @ `0xa13200` (`MODEL`, `WORLD`, `FOW_TEXTURE`, … fn_rva
  `0x1adxxx`) — renderer shader-semantic setters, NOT Lua AI; matched the `{string,fn}`
  pattern coincidentally. Ignore for this audit.

### Classification of the stdlib (the part we found)
- **Pure-VM / thread-local**: `table.*`, `string.*`, most of base — operate on the calling
  coroutine's stack only.
- **Touch the shared `global_State`** (not C++ game state, but the same shared structure
  prewarm corrupted): `collectgarbage`/`gcinfo` (GC), `loadstring`/`loadfile`/`dofile`/
  `require` (compile + string interning), `coroutine.create/resume`. Under Model B these are
  the cross-coroutine hazard — one VM, one `global_State`, one allocator.
- **None of these touch C++ sim state** (entities/fog/resources).

### KEY FINDING — the sim-write surface is NOT in `luaL_Reg` tables
The EaW *game* API (the functions that write fog-of-war, spawn units, change resources — the
side-effects prewarm proved exist) is **absent** from the `luaL_Reg` scan. EaW exposes game
objects via an **OO Lua binding** (RTTI classes `LuaCreateThread`, `ThreadValue`, game-object
metatables), registered through `lua_pushcclosure` into metatables rather than flat
`{name,fn}[]` tables. So the write-closure enumeration needs a different driver.

### Next step (refined method)
1. Locate `lua_pushcclosure` by decompiling the function that references the base-lib table
   `0x8fe120` (that's `luaL_register`/`luaopen_base`; the per-entry call inside its loop is
   `lua_pushcclosure`).
2. Enumerate **all** `lua_pushcclosure` call sites across `.text` — the ones outside the
   stdlib openers are the OO game-API methods (the sim-write candidates).
3. Decompile + classify each game-API closure: reads-only vs. writes-C++-state (and which
   state). That write set is the B2 deferred-apply surface.
