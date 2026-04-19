# Networking Subsystem

**Status:** Phase 2 complete — per-frame network ticks fully mapped.
**Last verified:** 2026-04-19

---

## Overview

All networking tick functions run unconditionally every frame (not gated on game-active). The
`PacketHandlerClass` threads handle raw packet I/O off the main thread; the main-thread ticks
below handle higher-level game-state sync, event dispatch, and session management.

---

## Per-Frame Tick Functions (unconditional)

### `FUN_0x339bc0` — BroadcasterClass Event Dispatcher

- **Size:** 1097 bytes
- **Signature:** `void FUN_140339bc0(longlong *param_1, int abs_time)`
- **Purpose:** Services the `BroadcasterClass` network event broadcast system
  - String: `"BroadcasterClass::Service"` — the periodic service call
  - String: `"BroadcasterClass::Time_Out_Player"` — disconnects players who have not responded within the timeout window
  - Uses `ThreadLockMutexClass` mutex — this is one of the few explicitly guarded shared-state accesses in the main loop
  - Dispatches queued network events to registered listeners

### `FUN_0x2c2910` — Multiplayer Game State Sync

- **Size:** 677 bytes
- **Signature:** `void FUN_1402c2910(longlong *param_1, uint frame_counter)`
- **Purpose:** Per-frame multiplayer state synchronization
  - Iterates all players, processes up to `0x3b` (59) network sync objects per player per frame
  - Sync objects carry game-state deltas (unit positions, health, commands)
  - Frame counter used to sequence sync packets

### `FUN_0x4908c0` — Dialog Voting System

- **Size:** 723 bytes
- **Purpose:** Coordinates multiplayer dialog/UI decisions via a vote mechanism
  - String: `"Received vote for dialog %s"` — logs incoming votes from remote players
  - String: `"Registered vote for dialog %s"` — logs local vote registration
  - When all players vote on the same dialog choice, the choice is confirmed and dispatched
  - Prevents UI state divergence in multiplayer

### `FUN_0xcaa60` — Player Event Queue Cleaner

- **Size:** 258 bytes
- **Purpose:** Drains per-player event queues
  - Iterates player array, processes all pending event queue items each frame
  - Lightweight — dequeues and frees processed event objects

### `FUN_0x22c50` — Network Session Timeout / Keep-Alive

- **Size:** 290 bytes
- **Signature:** `void FUN_140022c50(int *param_1, int abs_time)`
- **Purpose:** Monitors connection health and fires timeout callbacks
  - Single-player timeout: **8 seconds** (`uVar1 = 8000`) before triggering `FUN_140022f10`
  - Multiplayer timeout: **20 seconds** (`uVar1 = 20000`, when `DAT_140a61fc8 != '\0'`)
  - Skips when `DAT_1409cf314 == 0` (no game active) and certain conditions not met
  - `FUN_140022f10` — the timeout handler (disconnect / error dialog)

### `FUN_0x2505c0` — Player Session State Tick

- **Size:** substantial
- **Purpose:** Per-frame player session maintenance
  - Calls `FUN_1402089e0` first (player registry refresh)
  - Then iterates player array for session-level state updates

### `FUN_0x2089e0` — Player Registry Refresh

- **Size:** 12 bytes (one-liner)
- **Purpose:** `FUN_140207e50(&DAT_140a7c4e0)` — refreshes the player registry object

---

## Thread Architecture

The `PacketHandlerClass` (3 instances, see threading_model.md) handles raw UDP/TCP I/O on dedicated
OS threads. These threads write into packet buffers guarded by `FastThreadLockClass`. The main-thread
ticks above consume those buffers — they do not touch the network socket directly.

The only explicitly mutex-guarded main-thread call is `FUN_0x339bc0` via `ThreadLockMutexClass`.
The sync ticks (`FUN_0x2c2910`, `FUN_0x4908c0`) appear to rely on the packet handler's lock-free
buffer design rather than explicit mutexes.

---

## Parallelization Assessment

- **Rating:** Not a parallelization target — networking is I/O-bound, not CPU-bound
- **Thread safety concern for Model A/B:** `BroadcasterClass` has a mutex; all other ticks assume
  single-threaded access. If the render thread or Lua thread reads player state, add read locks.
- **Player disconnect during frame:** `FUN_0x339bc0` can call `Time_Out_Player` mid-frame.
  Any parallelization must handle the case where a player object is freed while other ticks still
  hold references to it.
