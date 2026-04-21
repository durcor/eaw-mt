# AI Subsystem (Lua Coroutine Engine)

**Status:** Phase 5 Step 1 — call-site timing hook installed; deadline enforcement not yet implemented.
**Last verified:** 2026-04-20

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
