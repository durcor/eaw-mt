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

### RESOLVED — the binding is a generic reflection dispatcher (not enumerable closures)
`lua_pushcclosure = FUN_1407b9340` (rva 0x7b9340); `luaL_register = FUN_1407ce7a0` (0x7ce7a0).
Enumerating all 11 `lua_pushcclosure` call sites (`Phase5EnumGameApi.java`) found only **9
distinct closures** beyond the stdlib — far too few to be the game API. Decompiling the
registrar `FUN_14024bfb0` (`Phase5DecompGameBinding.java`) shows why: it builds a single
metatable **"LuaWrapperMetaTable"** shared by all game-object userdata:

| metamethod | handler |
|---|---|
| `__index`    | `FUN_14024a8a0` — the reflection dispatcher |
| `__call`     | `FUN_14024a570` |
| `__gc`       | `FUN_14024a7f0` |
| `__tostring` | `FUN_14024c320` |
| `__eq`       | `FUN_14024c200` |

The `__index` dispatcher (`FUN_14024a8a0`): takes (object, key-name); for navigation it calls
the object's `vtable[6]` and marshals the result via `FUN_140247700`; for methods it **hashes
the key name (FNV-style) and looks it up in a per-type method table** (`FUN_14011f570`), then
invokes the resolved C++ member. **Every** `obj:Method()`/`obj.Prop` from Lua — read or write —
funnels through this one function into the full reflected C++ game-object API.

**Consequence:** there is no small, enumerable "sim-write closure set." The write surface is the
entire reflected game-object method API across all types (hundreds), reached by name through one
dispatcher, with no read/write classification exposed at the binding layer. See
`threading_model.md` Phase 5 for the resulting Model B verdict.

---

## Phase 6 — C-closure write-set enumeration RE-OPENED via the dispatcher chokepoint (2026-06-12)

User picked "pursue the shared AI C-closure write-set enumeration" (the Model B prerequisite, shared by
the tactical AND galactic Lua pumps — `galactic_map.md` increment 3). The Phase-5 "not enumerable"
verdict is true **statically** but the same reflection design makes the surface enumerable
**dynamically**, at one chokepoint. Mechanism, fully traced this session:

- **`__index` = `FUN_14024a8a0`** (the dispatcher). Two paths on the wrapped object `obj`:
  (a) *property/navigation* when `obj[2]==0` → `obj->vtable[0x30](obj,key)` returns a sub-object;
  (b) *method* → **FNV-1a hashes the key name** (seed `0x84222325`, mult `0x1b3`) then a Park-Miller
  scramble (`%0x1f31d*0x41a7…`), and looks it up via `FUN_14011f570` in the per-type method table. The
  **method-name string is live here** (marshalled into a local via `FUN_140022730`) before it is hashed.
- **method table = `std::map<name_string, descriptor>` per type** (`FUN_14011f570` is a hash-bucket +
  ordered-node lookup; nodes carry the name string at `node+0x10`, len `node+0x4`, value `*node`).
- **`__call` = `FUN_14024a570`** (invocation): marshals the Lua args, then calls the descriptor's
  **`vtable[0x50](obj, lua_state, arglist)`** = the real reflected C++ member; `vtable[0x88]` returns an
  **arg-type hint** consumed by arg marshalling (`FUN_1402473e0`) — **not** a const/read-write flag.

**Findings that decide the driver:**
1. **No static read/write classifier exists** at the descriptor level — there is no const/side-effect
   bit; classification requires knowing what each underlying C++ member does. Static full enumeration
   would mean finding every per-type method-table registration (hundreds) and decompiling each invoke
   target — intractable as a sweep.
2. **The reachable-during-pump set is capturable at ONE runtime chokepoint.** Every `obj:Method()` from
   any AI coroutine funnels through `__index` (`24a8a0`) with the **name string + object RTTI type**
   both available. A hook there, active across a galactic pump and a combat pump, yields the *actual*
   reachable method set — far smaller than the static hundreds, and exactly the Model B "closures
   reachable from `lua_resume`." This is the project's standard capture-oracle methodology.

> **Driver decision:** RUNTIME capture at `FUN_14024a8a0`, not static enumeration. Classification then
> = name heuristics (`Get_`/`Is_`/`Can_`/`Find_` = read; `Set_`/`Spawn_`/`Activate_`/`Give_`/`Change_`/
> `Add_`/`Remove_`/`Despawn_` = write) cross-checked by targeted decompile of the writers, mapped onto
> the existing 3-class write model (`threading_model.md`: Class 1 sensor/fog · Class 2 CreateObject+RNG
> · Class 2b cross-entity order · Class 3 SFX).

### Increment 2 design (build plan — validated against the existing `hooks/winmm_proxy.c` infra)

Concrete, reusing what already exists in the hook:
- **Gate "during pump":** add `volatile LONG g_in_pump` set/cleared around the existing `g_pumpe_orig(a)`
  call inside `pumpe_hook` (winmm_proxy.c ~L2695). The capture records only when `g_in_pump != 0`.
- **Mode tag:** the hook already tracks galactic vs combat — `g_galactic_ever_active`
  (`galactic_slot22_hook`, `GalacticModeClass::Service` slot 22 = `FUN_14045e030`) vs
  `g_space_mode_seen` (`space_slot22_hook`). Tag each captured name with the active mode so one run
  separates galactic-pump vs combat-pump reachable sets.
- **Capture point — prefer the metatable-slot patch over an inline trampoline (lower risk).** The
  `__index` handler `FUN_14024a8a0` is stored in the "LuaWrapperMetaTable" built by `FUN_14024bfb0`;
  patching that one function-pointer slot to our recorder (which tail-calls the real `24a8a0`) avoids
  rewriting `24a8a0`'s prologue — same risk profile as the existing sim-tick **vtable** patch, not the
  hotter inline-trampoline (render-flush) path. Fallback: inline trampoline if the slot is not locatable.
- **Read the method name:** replicate the dispatcher's own arg fetch — `key_ptr = FUN_1407b9cc0(L, 2)`
  (already the call `24a8a0` makes), read the null-terminated Lua string. (Object RTTI type is a
  refinement: walk `obj->vtable → RTTI col`; name-prefix classification works without it for pass 1.)
- **Dedup + dump:** open-addressing string set (cap ~4096) keyed `(mode<<bit | name)`; flush the set to
  `eaw-mt.log` on the existing pump-summary cadence. Read-only — no sim writes.

**Capture campaign (needs game launches — disk-safety gate):** `just build-winmm-oracle` then
`just difftrace=1 launch-foc-desktop` for (1) a galactic-map session and (2) a space battle; per
`[[feedback_launch_disk_safety]]` the recipe hard-caps output at 512 MB, read `eaw-mt.log` (not the
`.out`), `pkill` after each. Then classify the union by name heuristic + targeted decompile of writers,
mapped onto the 3-class model (`threading_model.md`).

> Halt-and-document per blueprint rule 4. The hot-path hook build + launch campaign is a subsystem-level
> change (blueprint rule 6) — get sign-off before the build/launch, then validate the hook is
> behavior-neutral (a control run == baseline) before trusting any capture.

### Increment 2 RESULT — LUACAP built, behavior-neutral, first capture (2026-06-12, commit 2d64608)

Built as designed (`hooks/winmm_proxy.c`: `index_dispatch_hook` + `install_index_dispatch_hook`;
`g_in_pump` brackets both pump wrappers; `EAW_LUACAP=1` opt-in; `just luacap=1` knob). The inline
trampoline uses the verified 15-byte position-independent prologue of `24a8a0` (3× `MOV [RSP+N],reg`),
JMP back to `+0xF`. Names read via the binary's own non-throwing `lua_tolstring` `FUN_1407b9cc0(L,2)`.

**Behavior-neutrality PROVEN (the gold-standard gate):** the menu space-battle DTWORLD world-hash is
**bit-identical with vs without the hook** — `h@1024 = ea5f27913390cce2`, `h@2048 = 7f7fdaf6ca0fcbaa`
in both the baseline (`difftrace=1`) and capture (`luacap=1 difftrace=1`) runs. The read-only capture
does not perturb the simulation.

**First capture — 24 distinct methods reached during the menu-battle pump,** classified:

| Class (`threading_model.md`) | Methods |
|---|---|
| **Read** (pure / no sim write) | `Get`, `Get_Name`, `Get_Owner`, `Get_Position`, `Get_Type`, `Is_Category`, `Is_Pool_Safe`, `Is_Valid` |
| **Class 2 — GOM remove/death** | `Despawn`, `Mark_Parent_Mode_Object_For_Death` |
| **Class 2b — cross-entity order** | `Attack_Move` |
| **Self-state write** (own object fields) | `Make_Invulnerable`, `Set_Cannot_Be_Killed`, `Prevent_AI_Usage`, `Prevent_All_Fire`, `Set_Importance`, `Set`, `Reset`, `Teleport`, `Teleport_And_Face`, `Cancel_Hyperspace`, `Cinematic_Hyperspace_In` |
| **Class 3 — presentation** | `Hide` |
| Unclassified (needs decompile) | `Service_Wrapper`, `Get` (generic accessor) |

This **demonstrably breaks the Phase-5 "not enumerable" deadlock**: the pump-reachable game-API surface
*is* enumerable at the chokepoint, neutrally, and the captured names map directly onto the existing
3-class model. The write methods that cross entity/global boundaries (`Despawn`, `Mark_..._For_Death`,
`Attack_Move`) are exactly the Class-2/2b deferred-apply surface Model B must buffer; self-state writes
shard by object for free (same logic as the tactical slice).

> **Caveats / next captures:** (1) the mode tag logged `?` for all 24 — the capturing pumps fire during
> battle load/intro before `space_slot22_hook` sets `g_space_mode_seen` (cosmetic; refine the mode signal
> or tag by `g_in_pump` source). (2) 24 is the *menu-demo* AI surface; a longer live battle (combat
> methods `Fire`/`Take_Damage`/`Activate_Ability` not yet seen) and a **galactic-map session** (fleet/
> planet/economy methods — the galactic AI surface, requires a GC/save load the menu harness doesn't
> auto-reach) will broaden the set. (3) then targeted-decompile the boundary-crossing writers to confirm
> their write target vs the 3-class model.
