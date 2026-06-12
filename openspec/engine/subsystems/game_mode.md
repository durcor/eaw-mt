# Game Mode State Machine

**Status:** Phase 2 complete — mode dispatch structure and per-mode subsystem ticks identified.
**Last verified:** 2026-04-19

---

## Overview

`FUN_0x30c3b0` (3157 bytes) is the **main game mode state machine tick**. It runs every frame on
the always-path (not gated on game-active). It dispatches to mode-specific subsystem ticks based
on the current game mode enum at `param_1 + 0x211c`.

---

## Mode Enum (`*(int *)(param_1 + 0x211c)`)

| Value | Mode | Dispatches |
|-------|------|------------|
| `0` | Menu / no game loaded | Minimal (no mode-specific ticks) |
| `1` | Galactic map (strategic layer) | ~10 galactic map subsystem ticks |
| `2` | Space battle | Space battle ticks |
| `3` | Land battle | Land battle ticks + 3D position math |
| other | Unknown / transitional | `FUN_14031d360` + widget updates |

---

## Per-Mode Dispatch (Phase 2 findings)

### Mode 1 — Galactic Map

> **Refined 2026-06-12 (`galactic_map.md`):** these ten ticks are all the galactic **UI/HUD shell**
> (widget refresh), *not* galactic simulation. None calls Lua, the strategic event queue
> (`DAT_140b2ed18`), the sim RNG, or `CreateObject`. The galactic sim ticks on a different path
> (`GalacticModeClass::vtable[43]` + `FUN_14021caf0`) — see `galactic_map.md`.

```c
FUN_1403e8a80(param_1);   // galactic map subsystem tick A
FUN_1403e83a0(param_1);   // galactic map subsystem tick B
FUN_1403145c0(param_1);   // galactic map subsystem tick C
FUN_1403edd00(param_1);   // galactic map subsystem tick D
FUN_1403e5aa0(param_1);   // galactic map subsystem tick E
FUN_140310e40(param_1);   // galactic map subsystem tick F
FUN_1403e4cb0(param_1);   // galactic map subsystem tick G
FUN_1403e6ff0(param_1);   // galactic map subsystem tick H
FUN_1403e6630(param_1);   // galactic map subsystem tick I
FUN_1403e79b0(param_1);   // galactic map subsystem tick J
```

Also:
- Planet selection state: `"## Delayed selection of planet %s"` string, `FUN_1403de570` to commit
- Player entity query: `FUN_140347b50(0)` (widget 0), `FUN_1403303d0` (planet selection queries)
- Fog of war: `FUN_14025c490` (enable) / `FUN_14025c080` (disable)
- Loading bar: `*(float *)(param_1 + 0x1000/0x1004)` — FPS/unit count display

### Mode 2 — Space Battle

```c
FUN_140317480(param_1);
FUN_140317d00(param_1);
FUN_140316260(param_1);
```

### Mode 3 — Land Battle

```c
FUN_140317480(param_1);
FUN_140312570(param_1);
// 3D position/matrix ops: FUN_1401f9340, FUN_1401f9320, FUN_1401f9440
// FUN_140494540(unit, player_query, pos)  — unit position query
// FUN_140528a70(unit, player_query, pos)  — unit position query (second unit)
// FUN_14012f570 / FUN_14012fba0          — unit transform update
FUN_140316260(param_1);
```

---

## Always-Path Logic (all modes)

Regardless of mode, `FUN_14030c3b0` also handles:

- **Transition effects:** `fVar = offset + 0x2a10`; counts down to 0 then fires `FUN_1402e4260` (screen flash / cinematic transition)
- **Fade-in/out timer:** `+0x5c0–0x5cc` float fields; lerp driven by `fVar24 = elapsed / fVar23`; calls `FUN_1402e7620` when fade completes
- **Fog of war toggle:** `FUN_1402cd0c0(&DAT_140b27a10)` — checks FOW enabled flag; `FUN_14025c490`/`FUN_14025c080`
- **Camera / selected entity sync:** reads `FUN_140294a40(&DAT_140a16fd0)` (active player/entity), syncs `+0x2b20` field; iterates a linked list of UI entities calling `vftable[0xf8]` (get name) and `vftable[0xa0]` (set name display)
- **Multiplayer pause check:** `DAT_140b153d5` pause flag; `*(int *)(param_1 + 0x2968) == 2` = paused state
- **`FUN_1402fca50`:** called when `iVar10 (network player count)` is available; drives a 10-element array at `+0x21c8` (player slots) — sets visibility per slot based on player index vs count
- **End of frame:** `*(bool *)(param_1 + 0x2120) = !*(char *)(param_1 + 0x2120)` — toggle flag
- **DAT_140b30392 flag:** triggers `FUN_1403def00` (mode reset / game-over cleanup)

---

## Nested Always-Path: `FUN_0x493f0`

Called twice after `FUN_14030c3b0` in the main loop:
```c
FUN_1400493f0(&DAT_1409cf320, abs_time);   // queue A + animations
FUN_1400493f0(&DAT_1409cf390);             // queue B (no time arg)
```
See `ui.md` for details.

---

## Parallelization Assessment

- **Not a direct target** — this is the top-level dispatcher; it cannot be parallelized itself
- **Galactic map ticks (mode 1):** The 10 galactic map sub-ticks are candidates for task parallelism if their data dependencies are independent — Phase 3 target
- **Space/land ticks:** Both call `FUN_140316260` last — likely a render-task submit; must run after the logic ticks
- **Transition effects and FOW:** Pure main-thread state writes; no parallelization concern
