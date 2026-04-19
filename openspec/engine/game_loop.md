# Game Loop

**Status:** Phase 3 complete — simulation tick and per-unit AI call chain fully traced.
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
    // *** SIMULATION TICK — first, before message pump ***
    // DAT_140b15418 = pointer to active game mode object (set during mode transitions)
    // Calls mode->vtable[0x2B] (slot 43, offset 0x158) — the per-frame simulation dispatch
    // In combat modes this resolves to FUN_14035cc10 (confirmed: fires event 0x23 = Lua AI)
    if (DAT_140b15418 != NULL) {
        (**(code **)(*DAT_140b15418 + 0x158))(DAT_140b15418, &viewport, &cam_pos, &cam_look);
    }

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
    FUN_140215b90((float)frame_delta_ms * time_scale);  // frame profiler (writes Slow_Frame_Profile_NNN.xml on slow frames)

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

| RVA | Call site | Purpose |
|-----|-----------|---------|
| `0x24bb80` | every frame | Lua object pool maintenance (pre-allocates up to 0x200 `LuaValue` objects) |
| `0x2505c0` | every frame | Player session state tick (calls `0x2089e0` then iterates player array) |
| `0x2089e0` | every frame | Player registry refresh — one-liner: `FUN_140207e50(&DAT_140a7c4e0)` |
| `0x3b08c0` | every frame | Pending command queue processor — drains linked list via `vftable[0x38]`, mode-gated |
| `0x2c2910` | every frame | Multiplayer game state sync — up to 59 sync objects per player |
| `0x4908c0` | every frame | Multiplayer dialog voting system |
| `0xcaa60`  | every frame | Player event queue cleaner — drains per-player event queues |
| `0x215b90` | every frame | **Frame profiler** — writes `Slow_Frame_Profile_NNN.xml` on slow frames |
| `0x339bc0` | every frame | `BroadcasterClass` network event dispatcher — mutex-guarded, handles player timeout |
| `0x22c50`  | every frame | Network session timeout monitor — 8s (SP) / 20s (MP) thresholds |
| `0x21caf0` | game active | **MSS audio tick** — `AIL_set_listener_3D_position/Orientation`, music/SFX ticks, `AIL_serve()` |
| `0x2d72c0` | game active | Timed animation/particle sort — qsort on a timed object array |
| `0x2d2ab0` | game active | Music + speech event streaming tick (`MusicEventManager`/`SpeechEventManager`) |
| `0x321dc0` | game active | Speech event stream lifecycle + Lua callbacks |
| `0x325190` | game active | 3D audio listener / camera position update |
| `0x30c3b0` | always      | **Game mode state machine** — dispatches galactic/space/land subsystem ticks (see `game_mode.md`) |
| `0x493f0`  | always (×2) | Timed callback queue + UI position interpolation / tweening (see `ui.md`) |
| `0x1c8420` | always      | Tooltip / hover info system — 33ms throttle, text-change detection (see `ui.md`) |

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

## Phase 1.4 Findings (complete)

**1.4A — Render flush:** `FUN_0x180dc0` confirmed — 12-pass task list iterator, batch geometry flush, per-task `vftable[0x10]` Execute call. See `openspec/engine/subsystems/renderer.md`.

**1.4B — GOM tick misidentification corrected:** `FUN_14021caf0` is the **Miles Sound System (MSS) audio tick** — calls `AIL_set_listener_3D_position`, `AIL_set_listener_3D_orientation`, `AIL_serve()`. The GOM tick is unidentified; the callee table above has been corrected.

**1.4C — Lua confirmed as coroutines:** Only 4 OS threads in the entire binary. `LuaScriptThread` labels are Lua coroutine names, not OS thread names. See `openspec/engine/threading_model.md`.

## Phase 2 Findings Summary

- ✅ All unconditional per-frame callees mapped (see table above)
- ✅ All game-active callees identified: audio (MSS tick + music/speech managers + listener update)
- ✅ Render flush call chain traced: `WinMain → FUN_0x180d90 → FUN_0x180dc0` (see `renderer.md`)
- ✅ Scene manager identified as game mode state machine (`game_mode.md`)
- ✅ Always-path callees: timed callbacks/tweening, tooltip system

---

## Phase 3 Findings: Simulation Tick and Per-Unit AI

### Simulation Tick (mode->vtable[0x158])

The simulation tick is the **first** call in each loop iteration, before the Win32 message pump.
It dispatches through `DAT_140b15418->vtable[0x2B]` (offset `0x158`) which in combat modes
resolves to `FUN_14035cc10` (266 bytes, shared by SpaceMode and LandMode, confirmed via no
direct xref callers + fires the per-frame signal).

#### FUN_14035cc10 body (combat mode slot-4, per-frame for ALL combat modes)

```
FUN_140493d50(mode[7])       // bombing run scheduler (PlayerBombingRunClass DVC)
FUN_1405284b0(mode[8])       // orbital bombardment scheduler (PlayerPlanetaryBombardClass DVC)
FUN_1404d9cd0(singleton, mode)  // per-player battle-cleanup service (calls FUN_14027ff80)
FUN_140220ed0(DAT_140a10b50, &DAT_140b153e0, 0x23, mode)  // fire signal event 0x23
FUN_1403c05d0(mode[0x3e])    // one-time SetupPhaseManager lazy init (guarded by flag)
```

#### Signal event 0x23 → per-unit Lua AI call chain

Event 0x23 fired on `&DAT_140b153e0` (combat/battle state signal source):

```
FUN_140220ed0 → FUN_140240940(&DAT_140b153e0, 0x23, mode)
  // FUN_140240940 = signal dispatcher (516b):
  //   finds all listeners registered for (source=&DAT_140b153e0, event_id=0x23)
  //   for each listener: (*listener->vftable[1])(listener, source, 0x23, mode)

→ FUN_140488660(scoring_manager)   // TheGameScoringManagerClass::Service (555b)
  // Registered via FUN_1404889d0 during game init (System_Initialize)
  // Checks ServiceRate Lua variable: only executes if rate-limited interval has elapsed
  // Logs: "TheGameScoringManagerClass::Service" and "Pump_Threads"

→ FUN_140247a90(lua_script)        // Pump_Threads (953b): advances Lua AI coroutines
  // Iterates DVC at lua_script[0x40..0x48] — each entry (0x40 bytes) is a Lua coroutine
  // Calls thunk_FUN_1407bc310(coroutine, ...) per coroutine — lua_resume equivalent
  // If coroutine completed (returns 0): reinitializes the slot
  // Terminates if lua_script[0x121] set (shutdown flag)
```

### TheGameScoringManagerClass structure (0x20 bytes)

```c
struct TheGameScoringManagerClass {
    void**  vftable;        // offset 0x00  → TheGameScoringManagerClass::vftable (RVA 0x8742b8)
    void*   field_08;       // offset 0x08  (purpose unknown)
    void*   lua_script;     // offset 0x10  → attached Lua script object (set by Attach_Lua_Script)
    uint16  flags;          // offset 0x18  (2 bytes, shutdown flag at bit 0)
};
```

**Key methods:**
- `FUN_140485340` — constructor (28b): sets vtable, zeroes fields
- `FUN_14004fd50` — `::Get()` singleton getter (137b): lazily creates, stored at `DAT_140a109a0`
- `FUN_1404889d0` — `::System_Initialize()` (222b): calls `Attach_Lua_Script`, subscribes to events 0x23+0x24
- `FUN_140488660` — `::Service()` (555b) = vftable[1]: per-frame Lua AI pump (rate-limited)
- `FUN_140488ab0` — `::Shutdown()` (56b): clears lua_script reference, resets flags
- `FUN_1404854e0` — `::Attach_Lua_Script()` (536b): loads named Lua script onto param_1+0x10

### Per-unit AI architecture summary

EaW does **not** have a native C++ iterate-and-call-Think loop over game objects. Per-unit AI is:

1. **Lua coroutines** (high-level strategy): `FUN_140247a90` (Pump_Threads) advances all Lua
   coroutines once per `ServiceRate` interval. Each coroutine is one unit's or group's Lua script.
   Rate-limited: not every frame, controlled by the `ServiceRate` Lua global.

2. **Command queue** (low-level execution): `FUN_1403b08c0` (command queue processor) drains
   all queued unit orders (move, attack, etc.) via `vftable[0x38]` dispatch — this IS the per-unit
   native AI response to Lua commands. Runs every frame, no rate limiting.

3. **Combat schedulers** (direct: bombing + orbital bombardment): called directly from
   `FUN_14035cc10` each frame, not via signals or Lua.

### Mode vtable[0x158] context

```
WinMain frame tick order:
  1. mode->vtable[0x2B] (offset 0x158) ← FUN_14035cc10 in combat modes [BEFORE msg pump]
  2. Win32 message pump (PeekMessageA drain)
  3. thunk_FUN_14024bb80 (Lua object pool)
  4. FUN_1402505c0 (player session state)
  5. FUN_1402089e0 (player registry refresh)
  6. FUN_1403b08c0 (command queue — per-unit AI execution)
  7. FUN_1402c2910 (multiplayer sync)
  8. FUN_1404908c0 (multiplayer voting)
  9. FUN_1400caa60 (player event queue)
  10. Frame timing (timeGetTime)
  11. [game-active branch: audio, music, animation, speech ticks]
  12. FUN_14030c3b0 (game mode state machine) [ALWAYS]
  13. FUN_1400493f0 (×2) (timed callbacks + UI tweening) [ALWAYS]
```

**Correction to Phase 2 game_loop.md**: The simulation tick (`mode->vtable[0x158]`) was
previously unidentified and incorrectly omitted. It runs BEFORE the Win32 message pump,
making it the first substantive work each frame.
