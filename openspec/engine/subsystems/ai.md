# AI Subsystem (Lua Coroutine Engine)

**Status:** Phase 5 Step 30 — per-call deadline watchdog (measure-only); extended WAKE_SUPPR; prewarm stable across 4+ battles.
**Last verified:** 2026-05-25

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
| RVA `0x7ba12c` — `av_write @near-zero` **(Step 29h)** | Pump_Threads zeroes `hm+0x38` (a pointer field inside the global Lua state struct) during its C-level cleanup. Post-pump, `[hm+0x38]` = null; the game reads it as a base pointer and crashes. | Save `hm38_pre = *(hm+0x38)` before pump; restore after. |
| RVA `0x7bf960` — stale TString chain after major GC **(Step 29h)** | Despite luaC_step + luaC_fullgc RET-patches, hm74 drops >99% for some pumps — a partial GC still runs. The GC frees TString objects; their freed-block link values (non-canonical pointers) appear in arr bucket chains. Game traverses them → AV. | Detect major GC via `hm74_pre > 1000 && post_hm74 < hm74_pre/100`. On major GC: `calloc(cap, 8)` a fresh zeroed arr; install at `hm+0x00`; leave old arr_pre untouched in allocator free list (`memset_ok = 3`). Also zero other entities' arrs via second-pass gc-sweep. |
| RVA `0x7bfdf0` — `luaH_get` corrupted node `next` **(Step 29h)** | Same major GC scenario: other entities' arrs still contain freed TString pointers. `luaH_get` traverses these as Lua Table node chains (both structures are 8-byte pointer lists); the non-canonical `next` value causes #GP reported as `av_read @ffffffffffffffff`. | gc-sweep second pass: after major GC, iterate all entities and `memset(arr2, 0, cap2*8)` for any entity other than the one we calloc'd. |
| RVA `0x7ba0df` — `av_write @0x1e0` **(Step 29i)** | Pump_Threads zeroes `hm+0x48` (Lua `global_State->weak` array pointer, or equivalent GC bookkeeping pointer) during its cleanup. Post-pump the GC weak-table sweep code (`FUN_1407ba0c0`) reads this field and writes through it; with it null the write lands at `0x1e0`. Crash surfaced during `lua_close` → `luaC_fullgc` → GC sweep → `FUN_1407bd730` (sweep list) → `FUN_1407ba0c0` (Table with flags≠0). | Save `hm48_pre = *(hm+0x48)` before pump; restore after. Pattern identical to hm+0x38. |

### Result (Step 29i)

- Prewarm correctly warms the cap=4096 entity before each second battle.
- Second-battle Lua AI load time: **~1 second** (down from 33 seconds) for the pre-warmed entity type.
- Additional entity types (cap=1024) hit a one-time ~33-second cold-start on first encounter; after that first load, OS page cache retains scripts and all subsequent battles for that entity type load in under 2 seconds.
- No crashes from prewarm across 15+ battle profiles (4500+ frames) in session 2026-05-24.
- Remaining open issue: `wined3d.dll` crash at mod_rva=0x9fe50 after extended play — replaced by DXVK (`d3d9=native,builtin` Wine registry override).

---

## Step 30 — Per-Call Deadline Watchdog (MEASURE-ONLY)

### Goal

Enforce a per-call deadline on `Pump_Threads` so that a single slow coroutine cannot block the frame for more than `PUMP_DEADLINE_MS` (4ms). This complements the per-gsvc cumulative budget (`PUMPE_BUDGET_MS=33ms`).

### Abort Flag at `*(param_1+0x121)`

`FUN_140247a90` checks `*(param_1+0x121)` after each coroutine step. If non-zero, it breaks the DVC loop and calls `FUN_1402488e0` (Lua cleanup / `LuaThreadTable` teardown). This is the natural hook for external deadline enforcement.

### Why the Abort Flag is Unsafe to Set Externally

**Confirmed by experiment (2026-05-25):** Writing `*(param_1+0x121) = 1` from the watchdog thread triggers `FUN_1402488e0`, which then executes a sync-object notification sequence in the battle loop (`FUN_1403a6b80`, RVA `0x3a6b80`). This notification writes to a Wine sync object whose `wake_addr` field has been corrupted to `0x6fff00000000` (pre-existing Wine race condition). The write faults. Our WAKE_SUPPR handler skips it, but the subsequent sync state is broken, eventually causing an ntdll null-ptr crash (`av_write @0x8` in ntdll list insertion at `mod_rva=0x5160d`).

**The abort flag is NOT safe to write from outside `FUN_140247a90`.**

### Current Implementation

- Watchdog thread polls every 1ms and increments `g_wd_fired` when a call exceeds `PUMP_DEADLINE_MS`.
- Does NOT write `*(param_1+0x121)`.
- `wd=N` in the per-gsvc stats reports how many 1ms ticks exceeded the deadline for that stats period.
- The per-gsvc budget (`PUMPE_BUDGET_MS=33ms`) remains the primary stall guard.

### Wine WAKE_SUPPR Extension (same session)

Three crash sites in game code all write to the corrupted wake_addr:

| RVA | Instruction | skip |
|---|---|---|
| `0x3a9e7d` | `c6 00 00 48 8b 89 ...` (Wine JIT thunk + chain advance) | 10 bytes |
| `0x3aa62e` | `44 88 38` — `MOV [RAX], R15B` | 3 bytes |
| `0x3aab43` | `c6 00 01` — `MOV [RAX], 1` | 3 bytes |

The WAKE_SUPPR VEH handler was extended to decode generic `c6 /0 imm8` and `REX 88 /r` write patterns (not just the original Wine JIT thunk byte sequence). All three sites are now recovered.

### Step 31 — GALTEAR_NULL read-AV guard (2026-05-25)

`FUN_14020a8c0` (RVA 0x20a8c0, 142 bytes) is a galactic-mode linked-list bulk-removal
function.  Its first real instruction after the prologue reads `[RCX+8]` (param_1+8 = list
head pointer).  When called with `param_1=NULL` — a pre-existing game bug observed after 4+
battles — `RCX=0` and the read faults: `av_read @0x8`.

**Node structure** (0x28 bytes, freed with `thunk_FUN_1407864b8`):

| offset | field | role |
|---|---|---|
| +0x00 | `node[0]` | back-link in a sibling doubly-linked list |
| +0x08 | `node[1]` | forward back-pointer |
| +0x10 | `node[2]` | next in singly-linked list |
| +0x20 | `node[4]` | owner object pointer |

For each node: unlinks it from the sibling list, frees it, decrements `owner+0x30`
(refcount), writes `0xfffffc19` (-999) to `owner+0x38` (destroyed sentinel).

**Assembly context at crash site:**
```
0x20a8c0  push rdi
0x20a8c2  sub rsp, 0x20
0x20a8c6  mov rax, [rcx+8]   ← CRASH: RCX=param_1=0, reads [0+8]
0x20a8ca  mov rdi, rcx
0x20a8cd  test rax, rax
0x20a8d0  je  0x20a948        ← epilogue (add rsp,0x20 / pop rdi / ret)
```

**Recovery**: `GALTEAR_NULL` VEH handler — detect read-AV at fault_addr ≤ 0x10 and
RIP == base+0x20a8c6, set RAX=0, advance RIP by 4.  The existing `test rax,rax / je`
at +0xd takes the empty-list return path cleanly.

**Verified pre-existing**: none of our hooks call into or near 0x20a8c0.  All hook entry
points are in the battle loop (0x247a90, 0x385190, 0x387400, 0x3a6b80) or init
(0x6c8710) or MEG cache (0x141f70).

### Open Issues

- Hapan ship lighting/texture corruption — likely a DXVK rendering issue, unrelated to AI hook.

### Step 29j — calloc fix for cap=4096 restore path; cap=1024 arena approach attempted and reverted (2026-05-25)

#### Fix A: calloc in cap=4096 restore path (confirmed)

**Problem:** After a successful prewarm pump, the cap=4096 restore code called `fn_alloc(hm18_pre, NULL, 0, cap*8)` to allocate a fresh string-intern bucket array. In the Step 29i session this path was never reached (`hm10_pre` was 0). In Step 29j it was reached; `hm10_pre=0x56c7b090` — a CRT DLL function address that is non-executable in this Wine/session configuration → instruction-fetch AV (RIP=0x54ba97a0).

**Fix:** Replaced `fn_alloc(...)` with `calloc((size_t)cap, 8)`. EaW's Lua allocator uses the CRT heap (`realloc`/`free`), so `calloc`-allocated blocks are compatible. The function pointer is no longer called in the restore path.

**Confirmed:** `PREWARM-DBG: hm10_pre=0x56c7b090` visible in log on 2026-05-25; cap=4096 entity pumps and restores without crash.

#### Fix B: cap=1024 entity pump — arena approach attempted, reverted

**Initial hypothesis:** The original skip of cap=1024 entities was motivated by a heap-mutex deadlock risk: the game's custom Lua allocator holds a critical section at the crash site; VEH→longjmp recovery skips the CS release → deadlock.

**Arena approach (attempted 2026-05-25):** Replace the entity's Lua allocator (`hm+0x10` = `frealloc`, `hm+0x18` = `ud`) with a bump-pointer arena backed by `VirtualAlloc` (64 MB) during the pump — no heap CS held by our arena, so VEH→longjmp would be safe. On success: install a hybrid allocator at `hm+0x10` that routes new allocs to the original heap fn and treats arena-range pointer frees as no-ops.

**Result: approach reverted.** The arena successfully bypassed the allocator crash. However, the second battle (after a loading screen that fired the prewarm) showed fog-of-war over the entire map and no models loaded. Entity 27 (cap=1024) is **not a pure-compute AI entity**. Its Lua coroutine issues side-effecting C game-world callbacks during execution — fog-of-war writes, entity spawn, or equivalent — that modify game state **outside the Lua VM**. These modifications persist into the battle and are not undoable by restoring hm fields alone. A full game-state snapshot would be required to reverse them.

**Current status:** cap=1024 entities are unconditionally skipped (`cap < 4096` guard). The arena infrastructure (`HybridAllocCtx`, `arena_lua_alloc`, `hybrid_lua_alloc`, VirtualAlloc globals) remains compiled in `winmm_proxy.c` for future investigation but is not called.

**Fundamental constraint:** Prewarming any entity whose Lua AI issues C game-state callbacks during the loading screen is unsafe without a snapshot-restore mechanism for affected game-world state. The cap=1024 one-time 33s stall is accepted; OS page cache makes subsequent encounters fast.

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
