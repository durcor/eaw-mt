# UI Subsystem

**Status:** Phase 2 complete — tooltip system and timed callback/animation queue identified.
**Last verified:** 2026-04-19

---

## Overview

The UI subsystem has two always-path per-frame ticks and one command queue processor. None of these
are gated on game-active — UI runs during menus, loading, and in-game.

---

## Tick Functions

### `FUN_0x493f0` — Timed Callback Queue + UI Position Interpolation

- **Size:** 1049 bytes
- **Signature:** `void FUN_1400493f0(longlong param_1, uint abs_time)`
- **Called:** always-path of main loop, **twice** — once with `&DAT_1409cf320, abs_time` and once with `&DAT_1409cf390` (no time arg → second form processes a different queue)
- **Purpose — Part 1: timed callbacks**
  - Processes a sorted queue of time-stamped callback entries
  - While `queue[0].fire_time <= abs_time && count > 0`: dequeue and call `vftable[0]` on the entry
  - Also calls `vftable[0]` on an associated "on-dequeue" object if present
  - This is a general-purpose **deferred event / scene transition callback** system
- **Purpose — Part 2: UI position interpolation**
  - After draining the queue, iterates remaining entries and lerps their 2D positions toward targets
  - Lerp formula: `current += (target - current) * speed`; snaps when `|delta| < threshold`
  - Calls `vftable[0x10]` (Execute/Update) when position changes
  - Reads `FUN_14030a6a0(&DAT_140b27f60)` — checks scene manager pause/active state to suppress motion
  - Also handles child objects linked at `+0x18` per entry: updates their positions and calls `vftable[0x28]`
- **Assessment:** This is the **UI tweening / scene transition system** — drives fade-ins, panel slides, mode transition animations, and deferred callbacks (e.g., "show this dialog after 500ms")

### `FUN_0x1c8420` — Tooltip / Hover Info System

- **Size:** 2679 bytes
- **Signature:** `void FUN_1401c8420(longlong param_1, int abs_time)`
- **Called:** always-path of main loop, unconditionally
- **Throttle:** early-exit if `(abs_time - last_update) < 0x21` (33ms = ~30 Hz update rate)
- **Purpose:**
  - Maintains a linked list of "hoverable" entities (at `param_1 + 0x90/0x98`)
  - First pass: removes expired/dead entities (flag at `+0xe0 == '\x01'` and `+0x138 == 0.0`)
  - Finds the highest-priority hovered entity via `FUN_1401e1f80`
  - Resolves tooltip text via `FUN_1401fa680` (string table lookup); falls back to `L"[MISSING]"` for untranslated entries
  - Uses `TEXT_GUI_DIALOG_TOOLTIP` as the tooltip widget string key
  - Compares new tooltip content against cached content at `param_1 + 0x1300` (`memcmp` on text + entity ID)
  - Only updates if content changed — prevents flickering
  - Calls `FUN_1401c0e60(param_1)` to clear the tooltip when hover ends
  - Optional `*(code **)(param_1 + 0x1370)` callback fired on tooltip change
- **State stored in `param_1`:**
  - `+0x1300`: last entity ID shown
  - `+0x1308–0x1340`: cached tooltip text strings (two strings)
  - `+0x1304`: extra flag field
  - `+0x1368`: "tooltip pending clear" flag
  - `+0x1370`: change callback function pointer

### `FUN_0x3b08c0` — Pending Command Queue Processor

- **Size:** 257 bytes
- **Called:** unconditional per-frame tick
- **Purpose:** Drains a linked list of pending game commands
  - Dispatches each command via `vftable[0x38]`
  - Gate: `DAT_140b153f8` game mode flag — only processes commands in appropriate game modes
  - After processing: frees or recycles the command object
- **Assessment:** This is the **deferred command queue** — commands queued from UI or Lua are batched here and dispatched once per frame in a safe state

---

## Widget System

`FUN_140347b50(uint widget_id)` — widget lookup by numeric ID. Referenced from the mode state
machine with IDs `0x239`, `0x26a–0x26d`, `0xe3`. Widget state is set via `vftable[0x80]` (show/hide)
and `vftable[0x300]` (set value/progress).

---

## Parallelization Assessment

- **Not a parallelization target** — UI is main-thread-only, tied to input and game state reads
- **Tooltip throttle (33ms):** Already frame-rate-independent; safe as-is
- **Timed callback queue:** Called twice per frame with different queue objects — both reads/writes are main-thread-only; no cross-thread concern unless game objects referenced in callbacks are moved to worker threads
