# Audio Subsystem

**Status:** Phase 2 complete — MSS audio tick, music/speech managers, and listener update identified.
**Last verified:** 2026-04-19

---

## Overview

Audio is powered by **Miles Sound System (MSS)** (statically linked). Three separate tick functions
handle different audio concerns, all gated on `DAT_1409cf314 == 1` (game-active).

---

## Tick Functions

### `FUN_0x21caf0` — MSS Core Audio Tick (game-active)

- **Size:** large (exact not decompiled in Phase 2 — see Phase 1.4B)
- **Called:** game-active branch of main loop
- **Purpose:** Core MSS service tick
  - `AIL_set_listener_3D_position` — update 3D listener world position
  - `AIL_set_listener_3D_orientation` — update 3D listener facing/up vectors
  - `AIL_serve()` — MSS periodic service (streams, DSP, positional audio update)
  - Also dispatches music and SFX system sub-ticks

### `FUN_0x2d2ab0` — Music + Speech Event Manager Streaming Tick (game-active)

- **Size:** 1894 bytes
- **Signature:** `void FUN_1402d2ab0(longlong param_1, int abs_time, uint frame_counter)`
- **Purpose:** Manages streaming audio events for music and speech
  - String: `"MusicEventManager: Starting stream %s"` — starts music stream assets
  - `"SpeechEventManager"` — processes queued speech audio events
  - Advances stream state machines, handles buffering and crossfades

### `FUN_0x321dc0` — Speech Event Stream Lifecycle + Lua Callbacks (game-active)

- **Size:** 1532 bytes
- **Signature:** `void FUN_140321dc0(longlong param_1, int abs_time, uint frame_counter)`
- **Purpose:** Manages the lifecycle of active speech event streams
  - Stream start / stop / completion detection
  - Fires Lua callbacks on speech events (bridges audio completion back to scripts)
  - Pairs with `FUN_0x2d2ab0` — that one enqueues; this one manages the playing streams

### `FUN_0x325190` — 3D Audio Listener / Camera Position Update (game-active)

- **Size:** 461 bytes
- **Signature:** `void FUN_140325190(longlong param_1, int abs_time)`
- **Purpose:** Updates the 3D listener position to match the camera
  - Float 3D position operations, calls `FUN_1402d5240` with XYZ coordinates
  - Likely reads camera world matrix and writes listener position to MSS
  - Must run after camera update but before `AIL_serve()` to take effect this frame

---

## Shared State

| Data | Owner | Accessed by |
|------|-------|-------------|
| MSS `HDLSDEVICE` / `HSTREAM` handles | Audio subsystem | All audio ticks (main thread only) |
| 3D listener position | Camera / audio | `FUN_0x325190` writes; MSS reads |
| Music event queue | `FUN_0x2d2ab0` | Internal to audio subsystem |
| Speech event queue | `FUN_0x2d2ab0` / `FUN_0x321dc0` | Also read by Lua callback handler |
| Lua callbacks on speech | `FUN_0x321dc0` | Writes Lua event queue — shared with script engine |

---

## Parallelization Assessment

- **Rating:** Low priority (MSS is not a known CPU bottleneck)
- **Constraint:** MSS is not thread-safe — all MSS calls must remain on one thread (the thread that called `AIL_startup`)
- **Speech → Lua coupling:** `FUN_0x321dc0` fires Lua callbacks directly. Any audio offload must either keep this on the main thread or use a deferred callback queue.
- **Listener update:** `FUN_0x325190` is a pure write of 3D coordinates; low risk, but the write target (MSS listener state) is shared with the core tick.
