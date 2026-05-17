# AI Subsystem (Lua Coroutine Engine)

**Status:** Phase 5 Step 29g — prewarm confirmed stable; no crash, no stall across multiple space battles.
**Last verified:** 2026-05-17

---

## Overview

EaW AI runs as a collection of Lua coroutines managed by `TheGameScoringManagerClass`. Each coroutine corresponds to one AI "service" object. Per-frame, the scoring manager resumes as many coroutines as the configured `ServiceRate` allows. This is the dominant variable-time consumer in space and land battles.

---

## Key Functions

| RVA | Name (inferred) | Size | Notes |
|-----|----------------|------|-------|
| `0x247a90` | `FUN_140247a90` (Pump_Threads) | 953 bytes | Iterates Lua coroutine DVC; calls `lua_resume` per entry |
| `0x488660` | `FUN_140488660` (ScoringManager::Service) | 555 bytes | Reads `ServiceRate`/`LastService` from Lua state; calls Pump_Threads when due |
| `0x246fb0` | `FUN_140246fb0` | 67 bytes | Read Lua variable (ServiceRate/LastService lookup) |
| `0x247000` | `FUN_140247000` | 62 bytes | Set `LastService` Lua variable |
| `0x247700` | `FUN_140247700` | — | Lua state prepare helper called inside Pump_Threads loop |
| `0x2488e0` | `FUN_1402488e0` | 754 bytes | Lua cleanup (called on `param_1+0x121 != 0`; tears down LuaThreadTable) |
| `0x1407bc310` | `thunk_FUN_1407bc310` | — | `lua_resume` (Lua 5.1 statically linked) |

---

## Call Chain (confirmed — Phase 3)

```
WinMain game loop
  └─ (*DAT_140b15418 + 0x158)(...)   ← vtable[43] sim_tick dispatcher
       └─ signal dispatch (event 0x23)
            └─ FUN_140240940          ← signal fan-out, calls vftable[1] on all listeners
                 └─ FUN_140488660     ← TheGameScoringManagerClass::Service
                      ├─ reads ServiceRate / LastService (Lua vars via FUN_140246fb0)
                      ├─ [conditional: if elapsed > ServiceRate]
                      │    └─ FUN_140247a90(*(param_1+0x10))   ← Pump_Threads
                      │         └─ for each entry in Lua DVC:
                      │              └─ thunk_FUN_1407bc310(lua_state, …)  ← lua_resume
                      └─ FUN_140247000(…, "LastService")    ← update LastService
```

Pump_Threads is called **once per frame at most**, conditionally gated by `ServiceRate`. It does not execute during loading screens (ScoringManager is only active during galactic/land/space modes).

---

## Pump_Threads Inner Loop

`FUN_140247a90` iterates a vector-of-structs DVC at `param_1+0x40` (start) / `param_1+0x48` (end), stride 0x40. Each slot holds:
- `+0x00`: Lua coroutine pointer
- `+0x08`: Lua state pointer
- `+0x30`: nargs (int, passed to lua_resume)
- `+0x38`: completion callback pointer

For each slot it:
1. Calls `FUN_140247700(coroutine, state)` — advance/resume state machine
2. Calls `thunk_FUN_1407bc310(lua_state, callback, nargs)` — `lua_resume`
3. On `lua_resume` return == 0 (coroutine finished): removes entry from DVC, calls completion callback
4. Checks `*(param_1+0x121)` (abort flag) each iteration; breaks early if set

The early-exit check at `+0x121` is the natural deadline hook point: set it to non-zero to stop iteration after the current coroutine finishes.

---

## Threading Hazards

### Data hazard — Pump_Threads vs. command queue

`FUN_140247a90` reads entity hierarchy state (position, status) held in the game-object hierarchy.  
`FUN_1403b08c0` (command queue, WinMain inter-flush block line 584) **writes** game-object state.

**Ordering invariant:** Pump_Threads (sim_tick chain, line 556) **must complete before** the command queue (line 584) in the same frame. This ordering is guaranteed by the current single-threaded execution model.

Any future Pump_Threads offload to a background thread must complete (join/event) before WinMain line 584. The join point must be no later than the command queue call.

### Model B (flush async) interaction

Pump_Threads must also complete before flush#1 if flush#1 reads entity state submitted by Lua. Current serialized Model A: flush#1 runs after sim_tick returns, so Pump_Threads is always done by flush time.

---

## Prologue — Call-site Patch Required

`FUN_140247a90` prologue bytes (RVA 0x247a90):
```
48 89 5c 24 08   MOV [RSP+8],  RBX
48 89 74 24 10   MOV [RSP+10], RSI
48 89 7c 24 18   MOV [RSP+18], RDI
55               PUSH RBP
```

The first three instructions are **RSP-relative home-space saves**. If copied into a CALL-based trampoline, CALL pushes a return address (shifting RSP by 8), making all three offsets wrong by 8. This would corrupt the caller's saved registers.

**Solution:** patch the `CALL rel32` (E8) inside `FUN_140488660` (call-site at runtime-scanned offset) to redirect through a near stub → `pump_threads_hook`. The near stub is a 14-byte `FF 25` absolute JMP allocated within 2 GB of the call site via `alloc_near`.

---

---

## Phase 5 Prewarm — Second-Battle Lua Stall (CONFIRMED FIXED)

### Root Cause

On the **second** battle of a session, Pump_Threads stalls for ~14–21 seconds while resuming Lua AI coroutines for the first time. The first battle uses a distinct loading path that skips the AI scoring manager's `System_Initialize`; on scene re-entry the coroutines are freshly created and must run their initialization Lua code (property table construction, AI state machine setup) from scratch.

This is **not** JIT compilation latency — EaW uses Lua 5.1 interpreted bytecode. The cost is purely first-run Lua execution: iterating game object property tables, allocating hash map entries, building AI state.

### AI Manager and Entity Structures

The AI manager initializer runs from `FUN_14056480` (RVA `0x56480`, `FUN_140056480`). This function is called by three known loading paths:

| Caller RVA | Loading path |
|------------|-------------|
| `0x55b60` | Galactic map load |
| `0x52d10` | Save-game load |
| `0x2a8730` callsite | Skirmish / space battle load |

Inside `FUN_14056480`, it calls the AI initializer (`FUN_1402a8730`, RVA `0x2a8730`). We hook this callsite across all three loading paths to intercept `ai_manager`.

**AI manager linked list** (at `ai_manager + 0x40`):

```
ai_manager + 0x40  ← sentinel node (list tail == sentinel means empty)
ai_manager + 0x48  ← first real node pointer

node layout:
  node + 0x00  fwd pointer (next node)
  node + 0x08  bwd pointer (prev node)
  node + 0x18  ref pointer (non-null means entity is live)

entity = ref - 0x18
  entity + 0x2d8  ← lua_state pointer (Lua coroutine for this unit's AI)
  entity + 0x3a0  ← flag byte; bit 1 set = entity disabled/dead (skip pump)
```

### Service Message Buffer Layout

Each entity's `lua_state` has a service message buffer attached at `lua_state + 0x58`. This buffer is used by AI property lookups (`FUN_140246fb0`) to intern string keys for `ServiceRate` / `LastService` Lua variables. The pump modifies this buffer's hash map in flight.

```
lua_state + 0x58  → buf   (pointer to message buffer object)
buf       + 0x10  → wr    (linear write pointer into payload area)
buf       + 0x20  → hm    (pointer to string-interning hash map)
hm        + 0x00  → arr   (pointer to bucket array, int64_t[cap])
hm        + 0x08  → cnt   (int64 — element count in low 32 bits)
hm        + 0x0c  → cap   (int32 — bucket capacity; overlaps hm+0x08 high bytes)
```

`arr` is an open-addressing bucket array. Each 8-byte slot holds either:
- `0` — empty bucket
- a valid node pointer — live entry
- `0x15` — Lua GC DEADKEY sentinel written by `sweeptable` after pump
- `0xffffffffffffffef` — game-side freed-slot sentinel written by hash map cleanup

**Critical layout note:** `hm + 0x08` is an `int64_t` that packs both count (low 32) and cap (high 32), but `cap` is also independently accessible as `*(int32_t *)(hm + 0x0c)`. Writing 8 bytes to `hm + 0x08` will clobber cap. Restoring count must use a full 8-byte write of the pre-pump value.

`Pump_Threads` zeroes `lua_state + 0x58` (buf pointer) as part of its own cleanup after each coroutine resumes. We do **NOT** restore it after prewarm — see the Step 29g fix note below.

### Prewarm Mechanism (confirmed working)

Prewarm fires from `pumpe_hook` (our hook on `Pump_Threads` / `FUN_140247a90`) on the **first game-loop call after loading**:

1. `FUN_14056480` wrapper captures `ai_manager` into `g_deferred_ai_mgr`.
2. Guard: only when `g_pumpe_count > 0` (skips first-ever load where entity heap data is garbage).
3. On next `pumpe_hook` call: fire `prewarm_from_ai_manager(ai_manager)` and return (skip the normal pump for that one tick).

**Per-entity loop:**
- Walk the AI manager linked list (sentinel check at `ai_manager + 0x40`).
- Skip entities with `entity + 0x3a0` bit 1 set (disabled).
- Skip entities with `cap < 4096` — these crash the allocator during pump (see below).
- Before pump: save `arr_pre = *(hm + 0x00)` and `hm08_pre = *(hm + 0x08)`.
- Call `g_pumpe_orig(lua_state)` — runs the AI coroutine one step, warming all Lua state.
  After this, `lua_state + 0x58` is **0** (Pump_Threads zeroed it as cleanup).
- **arr cleanup (Step 29g):** after the pump, re-read `cur_arr = *(hm + 0x00)`.
  - If `cur_arr == arr_pre` (arr was not rehashed): `memset(arr_pre, 0, cap * 8)`. Clears all stale node pointers and DEADKEY sentinels. Safe because arr_pre is still our allocation at exactly `cap * 8` bytes.
  - If `cur_arr != arr_pre` (GC rehashed into a new arr during pump): leave both arr untouched. The new arr contains only live entries; arr_pre is now a free-list block (do not touch).
- Restore `hm + 0x08 = hm08_pre` (pre-pump count+cap) so hash lookups don't divide by zero.
- **Do NOT restore `lua_state + 0x58`.** Leave it at 0. The next battle-loop pumpe call for this entity allocates a fresh buffer, avoiding all stale buf/wr state.

**Battle-mode array (BMA) snapshot:** `DAT_140b153e0` (RVA `0xb153e0`) holds a battle-mode array. Prewarm snapshots `*(lv2 + 0x20)` for each live entry before pumping and restores all of them after, preventing the pump from corrupting battle-mode state that `FUN_14028a4d0` reads on the first real game tick.

**Fault recovery:** `setjmp`/`longjmp` + a VEH handler (`g_prewarm_veh`) catch any access violation inside the pump. The VEH triggers `longjmp` to skip the current entity. This prevents a faulting entity from killing the whole prewarm session.

### Crash Archaeology (resolved)

Several crashes were encountered and fixed during prewarm development:

| Crash site | Root cause | Fix |
|---|---|---|
| Main menu camera broken + save-load freeze | `fn56480_wrapper` fired prewarm on main-menu entities (first load, uninitialized heap) and during save loads (allocator deadlock via longjmp) | Guard: prewarm only when `g_pumpe_count > 0` |
| RVA `0x7bf960` — `[rbx+0x10]` where rbx=0x15 | After pump, arr[bucket]=0x15 (Lua DEADKEY from GC sweep). Game reads arr slot as a node pointer and dereferences 0x15. | In-place sentinel scan: zero slots < 0x10000 |
| RVA `0x642160d` — `av_write @0x8` | memset of arr clobbered allocator boundary tag adjacent to arr block (memset of `cap*8` bytes exceeded the actual allocation). | Replaced memset with targeted scan |
| RVA `0x642160d` — `av_write @0x8` (second form) | Full backup/restore (`memcpy` arr from `g_arr_backup`) wrote `backup[0]=0` (original Lua NIL empty-bucket) into arr_pre's first slot. When arr_pre was later freed as the sole block in the free list, the allocator set next=null and prev=null, then crashed at `[null+8]` during removal. | Remove backup/restore entirely; use in-place sentinel scan. Free-list chain pointers are large heap addresses (>0x10000) and are preserved. |
| cap=1024 entities crash inside the allocator during pump | These entities' Lua coroutines trigger an allocation path that crashes mid-allocation; longjmp from inside a locked heap mutex deadlocks all subsequent allocs. | Skip any entity with `cap < 4096`. The stall bottleneck has `cap=4096`; cap=1024 entities are not responsible for the 21s hitch. |
| RVA `0x7bf960` — `[rbx+0x10]` where rbx=0x00000001 **(Step 29g)** | Restoring `lua_state+0x58=buf` after prewarm caused the battle-loop pumpe to reuse the old buffer. Pump_Threads re-processed init messages against the memset'd arr (all zeros), writing `0x01` into `arr[bucket]` as an intermediate state. On the next hash table lookup, `[0x01+0x10]` AV. | Do not restore `lua_state+0x58`. Leave it at 0; next pumpe allocates a fresh buffer. |

### Result

- `Pump_Threads` (RVA `0x387010`) now runs at `avg=0.00 max=1.00 ms` across tens of thousands of calls across multiple sessions.
- The 14–21 second second-battle Lua AI stall is **eliminated**.
- Prewarm fires correctly for galactic, skirmish, and save-load entry paths.

---

## Phase 5 Plan

### Step 1 (current) — timing hook
- Call-site patch on `FUN_140247a90` (via near stub in `FUN_140488660`)
- `pump_threads_hook`: QPC before/after call, accumulates per-invocation ms into the profile window
- Profile output adds `pump_ms` metric alongside `flush`, `frame`, `inter`, `sim_tick`
- Baseline data needed: pump_ms share vs. frame_ms per mode, and worst-case pump_ms distribution

### Step 2 (not yet started) — deadline enforcement
- Use the `*(param_1+0x121)` abort flag inside `FUN_140247a90` to cap iterations
- Set abort flag if QPC elapsed > per-mode budget (suggested: 4–8 ms for land/space)
- Remaining coroutines deferred to next frame (natural Lua yield-resume semantics)
- **Prerequisite:** confirm via pump_ms distribution that hitches are iteration-count-driven, not single-coroutine runaway

### Step 3 (future) — offload to AI thread
- Move Pump_Threads to a background thread kicked from sim_tick
- Join before WinMain line 584 (command queue)
- Requires audit of all entity reads inside `lua_resume` call chain for write conflicts with command queue
