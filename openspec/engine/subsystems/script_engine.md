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

## Entry Point / Tick RVA

**TODO** — pending Phase 1.3 game loop analysis.

## Parallelization Assessment

- **Rating:** Medium risk (blueprint Model B)
- **Blocker:** Must confirm whether Lua states are OS threads or coroutines
- **If coroutines:** can be moved to a dedicated OS thread with minor synchronization work
- **If OS threads already:** the engine already parallelizes scripts — focus instead on reducing lock contention
