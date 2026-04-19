# Function Registry

**Target binary:** `StarWarsG.exe` (x86-64, ImageBase `0x0000000140000000`)

All RVAs are relative to ImageBase unless marked as VA.

---

## Lua Runtime (statically linked, Lua 5.1)

### FUN_1407bc310 — lua_resume
- **RVA:** 0x7bc310
- **Name (confirmed):** `lua_resume`
- **Confidence:** High (error strings "cannot resume dead coroutine" / "cannot resume non-suspended coroutine" are exact Lua 5.1 text)
- **Signature:** `int lua_resume(lua_State* L, int nargs, lua_CFunction cont)`
- **Size:** 344 bytes
- **Purpose:** Resumes a suspended Lua coroutine. Returns 0 on yield, LUA_YIELD on completion, error code otherwise.
- **Evidence:** Error string literals, coroutine state checks (0x10 flag = suspended), stack manipulation matching Lua 5.1 source.
- **Called by:** `FUN_140247a90` (Pump_Threads) via `thunk_FUN_1407bc310`
- **Threading notes:** Operates on a single `lua_State*`. Not thread-safe — all calls must be on the same thread as the state owner.
- **Last verified:** 2026-04-19

---

## Per-Unit AI / Lua Coroutine Pump

### FUN_140247a90 — Pump_Threads
- **RVA:** 0x247a90
- **Name (confirmed):** `TheGameScoringManagerClass::Pump_Threads` (inferred from debug log "Pump_Threads")
- **Confidence:** High
- **Signature:** `void Pump_Threads(LuaScriptObject* script)` — `param_1` is the Lua script object at `TheGameScoringManager.lua_script`
- **Size:** 953 bytes
- **Purpose:** Advances all active Lua AI coroutines one step. Iterates the DVC at `script+0x40..+0x48` (stride 0x40/entry). For each non-null slot: calls `FUN_140247700` (push yield-return values) then `lua_resume`. If coroutine finishes (returns 0): calls `FUN_140248d10` (cleanup) and reinitializes the slot.
- **Evidence:** Debug log "Pump_Threads", thunk call to lua_resume, slot reinitialization on completion.
- **Called by:** `FUN_140488660` (TheGameScoringManagerClass::Service)
- **Calls:** `FUN_140247700`, `thunk_FUN_1407bc310` (lua_resume), `FUN_140248d10`, `FUN_1402488e0` (shutdown)
- **Shared state:** DVC at `script+0x40`; `script[0x121]` shutdown flag
- **Threading notes:** Not thread-safe. Primary candidate for Model B (Lua offload) — needs shared-state audit before threading.
- **Last verified:** 2026-04-19

### FUN_140247700 — push_yield_returns
- **RVA:** 0x247700
- **Name (inferred):** push yield-return values before lua_resume
- **Confidence:** Medium
- **Signature:** `void push_yield_returns(lua_State* L, coroutine_descriptor* desc)`
- **Size:** 622 bytes
- **Purpose:** Dispatches on the coroutine's last yield type (vftable[5] return, cases 1–9) to push appropriate response values back onto the Lua stack before the next `lua_resume`. Case 1 = entity hierarchy traversal, cases 4–6 = scalar/string push, case 9 = entity lookup + continuation.
- **Evidence:** Switch on vftable return value, calls to `FUN_1407b9520` (push string), `FUN_1407b9320` (push bool), `FUN_1407b9a60` (stack ops).
- **Called by:** `FUN_140247a90` (Pump_Threads), self-recursive for tree traversal
- **Last verified:** 2026-04-19

### FUN_140248d10 — coroutine_cleanup
- **RVA:** 0x248d10
- **Name (inferred):** post-coroutine cleanup / LuaThreadTable update
- **Confidence:** Medium
- **Signature:** `void coroutine_cleanup(LuaScriptObject* script, lua_State* coroutine)`
- **Size:** 131 bytes
- **Purpose:** Called after a coroutine finishes. Retrieves "LuaThreadTable" from the Lua state, checks if the result type is a table (type 5), and stores the finished coroutine's result back into the table.
- **Evidence:** `FUN_1407b94a0(L, "LuaThreadTable", 14)` call (lua_getfield), type check `iVar1 == 5`.
- **Called by:** `FUN_140247a90` (Pump_Threads) on coroutine completion
- **Last verified:** 2026-04-19

### FUN_140246940 — lua_error_handler
- **RVA:** 0x246940
- **Name (inferred):** Lua script error handler
- **Confidence:** High
- **Signature:** `undefined8 lua_error_handler(lua_State* L)`
- **Size:** 395 bytes
- **Purpose:** Called on the error path in Pump_Threads when lua_resume returns an error. Formats error message as `"LuaScriptClass::ERROR -- %s"` and passes to `DAT_140b09d40` (game error callback).
- **Evidence:** Format string literal "LuaScriptClass::ERROR -- %s".
- **Called by:** `FUN_140247a90` (Pump_Threads) on error return from lua_resume
- **Last verified:** 2026-04-19

---

## TheGameScoringManagerClass

### FUN_140488660 — TheGameScoringManagerClass::Service
- **RVA:** 0x488660
- **Name (confirmed):** `TheGameScoringManagerClass::Service`
- **Confidence:** High (debug string literal)
- **Signature:** `void Service(TheGameScoringManagerClass* this)`
- **Size:** 555 bytes
- **Purpose:** Per-frame AI pump, rate-limited. Reads "ServiceRate" and "LastService" Lua globals; only calls Pump_Threads if the elapsed time since last service exceeds ServiceRate. Updates "LastService" after pumping. vftable[1] (listener->vftable[1] called by signal dispatcher).
- **Evidence:** Debug strings `"TheGameScoringManagerClass::Service"` and `"Pump_Threads"`.
- **Called by:** `FUN_140240940` (signal dispatcher) when event 0x23 fires
- **Calls:** `FUN_140246fb0` (read Lua variable), `FUN_140247a90` (Pump_Threads), `FUN_140247000` (set LastService)
- **Last verified:** 2026-04-19

### FUN_140485340 — TheGameScoringManagerClass constructor
- **RVA:** 0x485340
- **Name (confirmed):** constructor
- **Confidence:** High
- **Signature:** `TheGameScoringManagerClass* ctor(TheGameScoringManagerClass* this)`
- **Size:** 28 bytes
- **Purpose:** Sets vtable, zeroes all fields (lua_script = 0, flags = 0).
- **Called by:** `FUN_14004fd50` (singleton getter)
- **Last verified:** 2026-04-19

### FUN_14004fd50 — TheGameScoringManagerClass::Get
- **RVA:** 0x4fd50
- **Name (confirmed):** `TheGameScoringManagerClass::Get()` singleton getter
- **Confidence:** High
- **Signature:** `TheGameScoringManagerClass* Get()`
- **Size:** 137 bytes
- **Purpose:** Lazily allocates and constructs the singleton at `DAT_140a109a0`. Registers instance in the global manager list.
- **Called by:** `FUN_14004b250` (main init), many other callers
- **Last verified:** 2026-04-19

### FUN_1404889d0 — TheGameScoringManagerClass::System_Initialize
- **RVA:** 0x4889d0
- **Name (confirmed):** `System_Initialize`
- **Confidence:** High
- **Signature:** `bool System_Initialize(TheGameScoringManagerClass* this)`
- **Size:** 222 bytes
- **Purpose:** Calls Attach_Lua_Script. If successful AND `DAT_140b15b10 != 0`, subscribes to signals 0x23 and 0x24 on `&DAT_140b153e0`.
- **Called by:** `FUN_14004b250` (main init) via `TheGameScoringManagerClass::Get()`
- **Last verified:** 2026-04-19

### FUN_1402488e0 — TheGameScoringManagerClass::Shutdown
- **RVA:** 0x2488e0
- **Name (inferred):** Shutdown / cleanup
- **Confidence:** High
- **Signature:** `void Shutdown(TheGameScoringManagerClass* this)`
- **Size:** 754 bytes
- **Purpose:** Sets shutdown flag, drains coroutine DVC, flushes Lua globals "Flush_G" and "Base_Definitions", closes Lua state. Deregisters from signal system.
- **Called by:** `FUN_140247a90` (Pump_Threads) on shutdown path
- **Last verified:** 2026-04-19

---

## Signal / Event System

### FUN_140240940 — signal_dispatcher
- **RVA:** 0x240940
- **Name (inferred):** signal dispatcher
- **Confidence:** High
- **Signature:** `void dispatch(SignalSource* source, uint event_id, void* data)`
- **Size:** 516 bytes
- **Purpose:** Finds all listeners registered for `(source, event_id)`. For each: calls `listener->vftable[1](listener, source, event_id, data)`.
- **Evidence:** Linked-list traversal, vtable[1] dispatch pattern, listener at node+0x18.
- **Called by:** `FUN_140220ed0` (signal send wrapper)
- **Threading notes:** Listener list is shared mutable state — must be read-locked before any threading patch that could fire signals concurrently.
- **Last verified:** 2026-04-19

### FUN_140220e90 — signal_subscribe
- **RVA:** 0x220e90
- **Name (inferred):** subscribe listener to signal
- **Confidence:** High
- **Signature:** `void subscribe(listener, source_ptr, listener_obj, event_id)`
- **Purpose:** Registers a listener for `(source, event_id)` in the signal list.
- **Called by:** `FUN_1404889d0` (System_Initialize) for events 0x23 + 0x24
- **Last verified:** 2026-04-19

---

## Mode Dispatch / Game Loop

### FUN_14035cc10 — combat_mode_sim_tick
- **RVA:** 0x35cc10
- **Name (inferred):** combat mode per-frame simulation tick
- **Confidence:** High
- **Signature:** `void sim_tick(CombatMode* mode, ...)`
- **Size:** 266 bytes
- **Purpose:** Per-frame simulation for space/land combat modes. Runs bombing run scheduler, orbital bombardment scheduler, per-player cleanup, fires signal event 0x23 (triggering Lua AI), lazy-inits SetupPhaseManager.
- **Evidence:** Called via `mode->vtable[0x158]` from WinMain; fires event 0x23 to `&DAT_140b153e0`.
- **Called by:** WinMain via `(*DAT_140b15418 + 0x158)()` when DAT_140b15418 is a combat-mode object
- **Calls:** `FUN_140493d50`, `FUN_1405284b0`, `FUN_1404d9cd0`, `FUN_140220ed0` (fires 0x23), `FUN_1403c05d0`
- **Threading notes:** Runs before Win32 message pump each frame. All work is synchronous on main thread.
- **Last verified:** 2026-04-19

---

## Profiler / Diagnostics

### FUN_140215b90 — frame_profiler_tick
- **RVA:** 0x215b90
- **Name (confirmed):** frame profiler tick
- **Confidence:** High (string "Slow_Frame_Profile_%03d.xml", only called from WinMain)
- **Signature:** `void frame_profiler_tick(float delta_seconds)`
- **Size:** 474 bytes
- **Purpose:** If frame time exceeds configured threshold (`delta / DAT_140a7cc3c > DAT_140a15120`), dumps a `Slow_Frame_Profile_NNN.xml` file and resets a 20-frame cooldown counter. Also resets per-node frame accumulators in the profiler tree.
- **Called by:** WinMain (only caller), every frame, after `timeGetTime()` delta calculation
- **Last verified:** 2026-04-19

---

## Threading Infrastructure

### FUN_14022e490 — thread_create_wrapper
- **RVA:** 0x22e490
- **Name (inferred):** generic thread creation wrapper
- **Confidence:** High
- **Signature:** `void thread_create_wrapper(ThreadDescriptor* desc)`
- **Size:** 199 bytes
- **Purpose:** Calls `_beginthreadex(NULL, 0, FUN_14022e400, desc, 0, NULL)`. Spin-waits until thread signals ready via `desc+8`. Registers thread in global thread table at `DAT_140a155a0`.
- **Called by:** `FUN_14008df00` (LoadThread manager command 0xc), `FUN_140225bf0` (NATUtils), `FUN_1402056d0` / `FUN_140205de0` / `FUN_1402059f0` (PacketHandler variants)
- **Last verified:** 2026-04-19

### FUN_14022e400 — thread_entry_trampoline
- **RVA:** 0x22e400
- **Name (inferred):** thread entry trampoline
- **Confidence:** High
- **Signature:** `unsigned __stdcall thread_entry_trampoline(void* param)`
- **Size:** 58 bytes
- **Purpose:** Called by `_beginthreadex`. Stores thread handle + ID in descriptor. Sets started flag at `desc+8` (releases spin-wait in `FUN_14022e490`). Calls `desc->vftable[1](desc)` — the actual thread work. Sets done flag at `desc+9`.
- **Called by:** `_beginthreadex` in `FUN_14022e490`
- **Last verified:** 2026-04-19

### FUN_14008df00 — load_command_handler
- **RVA:** 0x8df00
- **Name (inferred):** LoadThread command handler
- **Confidence:** High (references string "LoadThread", uses `LoadingThreadClass::vftable`)
- **Signature:** `void load_command_handler(CommandStruct* cmd)`
- **Size:** 2340 bytes
- **Purpose:** Handles loading lifecycle commands: `0` = abort, `1` = begin transition, `0xc` = start load (creates LoadingThreadClass, calls thread_create_wrapper). Command `0xc` path: sets up `LoadingThreadClass` object, names it "LoadThread", starts it via `FUN_14022e490`.
- **Last verified:** 2026-04-19
