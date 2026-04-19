# Game Loop

**Status:** Phase 1.3 complete — WinMain identified, loop structure documented, subsystem ticks mapped.
**Last verified:** 2026-04-19

---

## Entry Point

- **WinMain** = `FUN_0x5d990` (RVA `0x5d990`, 7440 bytes)
- Signature: `undefined8 FUN_14005d990(HINSTANCE param_1, undefined8 param_2, undefined8 param_3, int param_4)`
- Window class name: `"TESTWINCLASS"`, title: `"Star Wars: Empire at War: Forces of Corruption"`

---

## Startup Sequence (pre-loop)

1. Single-instance check — `CreateMutexA("EAWIsRunning")` + `FindWindowA`
2. `SetCurrentDirectoryA` to executable directory
3. Load `Data\Patch.meg` / `Data\patch2.meg` / `Data\64Patch.meg` into the VFS
4. Registry: `HKCU\Software\Petroglyph\StarWars FOC` + `Profiles`
5. `CoInitialize(NULL)` — COM (required for DirectShow / NAT traversal)
6. `timeGetDevCaps` + `timeBeginPeriod(min)` — set highest timer resolution
7. `RegisterClassExA` + `CreateWindowExA`
8. Subsystem init: display monitors enumerated, window proc = `FUN_140060c60`
9. `FUN_14004b250()` — subsystem init gate; returns 0 on failure
10. Logo/intro sequence dispatched via `FUN_14044d920` ("Logo1")

---

## Main Loop (do…while)

Loop condition: `DAT_140a619ef == '\0'` (quit flag, set on `WM_QUIT` or error)

```c
do {
    // Message pump — drain all pending Win32 messages
    while (PeekMessageA(&msg, NULL, 0, 0, PM_REMOVE | PM_QS_ALL)) {
        if (msg.message == WM_QUIT) { quit = 1; break; }
        TranslateAcceleratorA(...);
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }

    // Unconditional per-frame ticks
    thunk_FUN_14024bb80();
    FUN_1402505c0();
    FUN_1402089e0();
    FUN_1403b08c0(&DAT_140b2ed18);
    FUN_1402c2910(&DAT_140b15690, frame_counter);

    FUN_1404908c0(&PTR_vftable_140a2aaa0);
    FUN_1400caa60();

    // Frame timing (timeGetTime-based, not QPC)
    DVar11 = timeGetTime();
    iVar14 = DVar11 - baseline_ms;        // absolute ms since startup
    frame_delta_ms = iVar14 - last_tick;  // ms since previous frame
    last_tick = iVar14;
    FUN_140215b90((float)frame_delta_ms * time_scale);  // timer subsystem tick

    FUN_140339bc0(&DAT_140b2ba20, iVar14);
    FUN_140022c50(&DAT_1409cf080, iVar14);

    // Game-state-active branch (DAT_1409cf314 == 1)
    if (game_active) {
        FUN_14021caf0(&DAT_140a7d0a0, iVar14, ...);   // primary game-state update
        FUN_1402d72c0(&DAT_140b27e60, iVar14, uVar5);
        FUN_1402d2ab0(&DAT_140b27dc0, iVar14, uVar5);
        FUN_140321dc0(&DAT_140b2b300, iVar14, uVar5);
        FUN_140325190(&DAT_140b2b3b0, iVar14);
    }

    // Scene manager / mode stack tick (always)
    FUN_14030c3b0(&DAT_140b27f60, iVar14);
    FUN_1400493f0(&DAT_1409cf320, iVar14);
    FUN_1400493f0(&DAT_1409cf390);

    // Idle path: WaitMessage when no active game state and no I/O
    if (!game_active && ...) {
        WaitMessage();
    }

    // Out-of-sync dialog (multiplayer desync)
    if (oos_flag) {
        // Shows "TEXT_OUT_OF_SYNC" / "TEXT_BUTTON_STOP" localized dialog
        FUN_140099d90(FUN_140061e30, ...);
    }
} while (!quit);
```

---

## Frame Timing

- **Clock source:** `timeGetTime()` — millisecond resolution
- `timeBeginPeriod()` at startup raises OS timer resolution to minimum (typically 1ms)
- Baseline `_DAT_140a7bec0` is set once at startup; all times are relative (`iVar14 = now - baseline`)
- Frame delta `uVar31 = iVar14 - _DAT_140a61b18` passed to `FUN_140215b90` as `(float)delta * fVar4`
  where `fVar4 = DAT_14080350c` (time scale multiplier, likely 0.001 to convert ms → seconds)

**Note:** The QPC-based timer cluster at RVAs `0x1a7370` / `0x222e20–0x222fa0` is a **separate
subsystem** — likely used for high-precision animation/particle deltas. The main loop frame rate
is driven by `timeGetTime()`.

---

## Active vs. Idle Gating

`DAT_1409cf314` is the "game active" flag:
- `0` → pure menu / no game loaded → `WaitMessage()` (blocks until input, ~0% CPU)
- `1` → in-game → full update path, no sleep, tight spin loop

On every 64th frame (`frame_counter & 0x3f == 0x3f`), a 1-second `timeGetTime` check triggers
`FUN_14021e7b0` (likely periodic housekeeping / GC tick).

---

## Key Callee Map

| RVA | Call site | Preliminary purpose |
|-----|-----------|---------------------|
| `0x24bb80` | every frame | Unknown tick (unconditional) |
| `0x2505c0` | every frame | Unknown tick (unconditional) |
| `0x2089e0` | every frame | Unknown tick (unconditional) |
| `0x3b08c0` | every frame | Unknown tick (unconditional) |
| `0x2c2910` | every frame | Unknown tick, takes frame counter |
| `0x4908c0` | every frame | Unknown tick (unconditional) |
| `0xcaa60`  | every frame | Unknown tick (unconditional) |
| `0x215b90` | every frame | Timer subsystem tick — takes `delta_ms * scale` |
| `0x339bc0` | every frame | Unknown subsystem tick, takes abs time |
| `0x22c50`  | every frame | Unknown subsystem tick, takes abs time |
| `0x21caf0` | game active | **Primary game-state update** — takes abs time + viewport/camera params |
| `0x2d72c0` | game active | Subsystem tick — takes abs time + frame counter |
| `0x2d2ab0` | game active | Subsystem tick — takes abs time + frame counter |
| `0x321dc0` | game active | Subsystem tick — takes abs time + frame counter |
| `0x325190` | game active | Subsystem tick — takes abs time |
| `0x30c3b0` | always      | Scene manager / mode stack tick |
| `0x493f0`  | always      | Unknown tick (called twice, different args) |

---

## Other Loop-Adjacent Functions

### `FUN_0x8e9a0` — Loading Screen Tick (1359 bytes)

Called on a separate code path during asset loading (not from the main game loop).

- Updates `IDC_PROGRESS_SOLO` and `IDC_PROGRESS0–N` progress bar widgets
- Frame cap: 50ms (20 fps during loading) enforced via `Sleep(min(10, remaining))`
- Has its own `PeekMessageA` message pump
- Calls `FUN_1402718e0`, `FUN_14025ca30`, `FUN_140271840` — loading subsystem ticks

### `FUN_0xc9900` — LAN Lobby / NAT Traversal Tick (1274 bytes)

Not a background thread loop — a UI tick called by the mode stack when the LAN lobby screen is active.

- Event-driven dispatch: param `0` = init/cancel, `1` = button press, `0xc` = periodic tick
- Updates `IDC_NAT_PROGRESS_BAR` widget
- 20-second NAT traversal timeout → auto-cancel
- `Sleep(20)` / `Sleep(0)` during LAN peer discovery (polling delay, not a tight spin)

---

## Threading Observation

The main loop is **single-threaded** — no thread creation occurs inside it. All game subsystem
ticks in the loop body run sequentially on the main thread. Worker threads (`LoadThread`,
`PacketHandler Thread`, etc.) are created during startup, not per-frame.

This confirms the engine is currently **single-threaded for game logic + rendering** — no
concurrent frame work is happening. This is the baseline Model A assumes.

---

## Parallelization Assessment

- **Current state:** Single-threaded game loop, no intra-frame parallelism
- **Model A insertion point:** Between the accumulate phase (game-state update including
  render task pushes via `FUN_14021caf0`) and the flush phase. The render task flush RVA
  is pending identification in Phase 1.4.
- **Frame timing risk:** The `timeGetTime()`-based delta is calculated on the main thread;
  if the render thread introduces latency, the delta will need to be measured before the
  render wait, not after.
- **`WaitMessage` on idle:** Safe — only fires when `DAT_1409cf314 == 0` (no game loaded).
  Won't interfere with frame pacing during actual gameplay.

---

## TODOs for Phase 1.4

- [ ] Identify render task flush function (xref from `alRenderTask` vftable or `MultiLinkedListClass<alRenderTask>`)
- [ ] Confirm `FUN_14021caf0` is the game object manager tick (decompile + cross-reference with GOM strings)
- [ ] Map remaining unconditional per-frame callees (`0x24bb80`, `0x2505c0`, etc.) to subsystems
- [ ] Determine whether `FUN_140215b90` is the same timer subsystem initialized at `0x1a7370`
