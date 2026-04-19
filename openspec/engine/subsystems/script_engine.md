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

## Entry Point / Tick RVA

**TODO** — Phase 2. The Lua tick will be somewhere in the main-thread game-active update path; exact RVA pending call-graph tracing.

## Parallelization Assessment

- **Rating:** Medium risk (blueprint Model B) — **updated after Phase 1.4C**
- **Confirmed:** Lua coroutines on one OS thread. Moving to a dedicated OS thread is viable.
- **Approach:** Wrap all `lua_State` access behind a mutex or lock-free queue; move the Lua coroutine scheduler to a dedicated worker thread. Main thread posts events; Lua thread processes and posts results back.
- **Key risk:** Scripts that call back into game object state synchronously (same-frame reads/writes to shared data structures). Must audit Lua ↔ C++ binding functions before implementing.
- **No longer blocked** by the "OS threads vs coroutines" question — next blocker is the Lua ↔ C++ shared-state audit (Phase 2).
