# EaW Main Game Loop — openspec

## Overview

WinMain (FUN_14005d990, RVA 0x5d990, 7440 bytes) contains a single `do { … }
while (DAT_140a619ef == '\0')` loop that drives the entire frame.  Each
iteration is one "frame" from the engine's perspective — there is no separate
sim-thread or dedicated frame-pacing thread in the original binary.

## Loop Skeleton (decompiled line references)

```
do {
  /* Message pump */
  558: PeekMessageA(…, PM_REMOVE) → while loop → TranslateMessage / DispatchMessage
  571: Conditional heartbeat: if running && active && >1s elapsed → FUN_14004fba0 + 262af0

  /* flush#1 (render thread kick for the previous frame's draw list) */
  580: thunk_FUN_14024bb80   ← hook0 (24bb80)
  581: FUN_1402505c0         ← hook1 (2505c0)
  583: FUN_1402089e0         ← hook2 (2089e0)
  584: FUN_1403b08c0         ← hook3 (cmd_q)
  585: FUN_1402c2910         ← hook4 (2c2910)
  586: [cond] FUN_1402c0c70  ← hook5 (2c0c70) — 5-15ms spikes when condition true
  590: FUN_1404908c0         ← hook6 (4908c0)
  591: FUN_1400caa60         ← hook7 (caa60)

  /* Post-flush housekeeping, profiler, timer, simulation ticks */
  597: FUN_140051d10         ← hook8  (51d10)
  604: timeGetTime / frame profiler (FUN_140215b90)
  609: FUN_140339bc0         ← hook14 (339bc0)
  610-614: FUN_140022c50/67c80/6a1e0/25770/6d1d0  ← hooks20-24
  615: [if DAT_1409cf314==1] FUN_1402d72c0/2d2ab0/321dc0/325190  ← hooks9-11,25
  626: FUN_1401c8420         ← hook12 (1c8420)
  627: thunk_FUN_1401768f0 → [cond] FUN_14044d840/d780/f130
  639: FUN_14030c3b0         ← hook13 (30c3b0)

  /* Event/string logging block (OOS dialog) */
  649: [if DAT_140b0a355||DAT_140b1574c && !DAT_140a61b0d]
       → out-of-sync dialog string construction + FUN_140099d90

  /* Late-loop prep — always runs */
  797: FUN_14027c9c0
  826: FUN_140047860

  828: FUN_14004fba0   ← hook15 (4fba0)
  829: FUN_140262b80   ← hook16 (262b80)
  830: FUN_1403b1b50   ← hook17 (3b1b50)

  /* Idle path vs game path */
  831: if (DAT_1409cf314 == '\0' && !FUN_14028af60() && !FUN_14024cef0()) {
  834:   WaitMessage()           ← CLASS-A HITCH SOURCE — see below
       } else {
  837:   [cond inner] FUN_14027c5f0 / FUN_1401c0c70
  840:   [if DAT_140a61e70] FUN_14009b9f0
  843:   FUN_14027c360
  845:   FUN_1402c0c70           ← second call site for hook5 (NOT hooked — forward scan
                                    picks first occurrence at line 586)
  865:   FUN_14020f000 / FUN_14028d400 / FUN_1402718e0
  870:   FUN_140297e30   ← hook18 (297e30)
  871:   FUN_14001dc60   ← hook19 (1dc60)
  874:   FUN_1403271d0 / 490480 / 3268c0
  878:   FUN_14044a3f0
  880:   [cond] vtable call + FUN_140342dc0/d80 / FUN_140092bb0
  889:   FUN_14028a4c0 / FUN_14027cb70
       } else {
  897:   FUN_14009ba60
  898:   FUN_1404908c0   ← second call site for hook6 (NOT hooked)
  899:   Sleep(0)
       }

  907: [if DAT_140a61aac || DAT_1409cf314] timeGetTime / FUN_14027d800 → more subsystems
} while (DAT_140a619ef == '\0');
```

## Key Globals

| Symbol | Role |
|---|---|
| `DAT_1409cf314` | Active-simulation flag — 1 during live game, 0 during menus/cinematics |
| `DAT_140a619ef` | Loop-exit flag — set to 1 on WM_QUIT (0x12) |
| `DAT_140a61b0d` | Set by FUN_14027c9c0 at line 797; gates the OOS dialog block |
| `DAT_140b15418` | Current game-mode object pointer |

## WaitMessage — Class-A Hitch Source (Phase 5 Step 3, 2026-04-24)

### Finding

`WaitMessage()` at WinMain:834 is the sole source of all 1000ms+ inter-flush
hitches observed in Phase 5 Step 2 profiling.

**Triggering condition (lines 831-833):**
```c
(DAT_1409cf314 == '\0') &&
(FUN_14028af60(&DAT_140b153e0) == '\0' || DAT_140b155c4 != '\0') &&
(FUN_14024cef0() == '\0')
```

All three must be true: the active-simulation flag is clear, and both
"pending work" and "pending input" checks report nothing.  During large space
battles, the main thread occasionally reaches this state even with AI
coroutines running on background threads, because those threads never call
`PostMessage` to wake the main thread.

**Evidence from profiling:**
- In every 1000ms+ Class-A hitch window, all 26 inter-flush hooks show 0ms max.
- All four gap timers (gapA–gapD) show small values (≤ 6ms).
- Yet `inter_max` = 1000–5462ms.
- `WaitMessage()` is the only blocking call in the inter-flush code path not
  covered by any hook or gap timer.  No E8 CALL to a tracked function is
  emitted — it calls the Windows API thunk directly.
- `hook18` (297e30) and `hook19` (1dc60) have n=2–34 per 300-frame window
  (vs n=86–109 for always-called hooks) because they live inside the `else`
  branch at line 836 — WaitMessage frames skip them entirely.
- `gapD` (hook19 exit → flush#2 entry) never captures hitch frames for the
  same reason: gapD base is set inside `ifc_hook19`, which doesn't run when
  WaitMessage fires, leaving `g_gap_base_D_valid = FALSE` at flush#2 entry.

### Fix — Option B (implemented, Phase 5 Step 3)

IAT patch: replace the `WaitMessage` entry in StarWarsG.exe's import address
table with `wait_message_hook`, which calls:

```c
MsgWaitForMultipleObjectsEx(0, NULL, 1, QS_ALLINPUT, 0)
```

This caps the stall at 1ms.  The function still returns `TRUE` so the caller's
message-dispatch loop continues as normal.  Profile output now includes:

```
wait_msg n=<total calls> timeout=<timed-out calls> (WaitMessage->1ms cap)
```

A high `timeout/n` ratio in a hitch window confirms WaitMessage was the
bottleneck; a low ratio means messages were arriving within 1ms and the hook
added negligible overhead.

### Deferred — Option A: per-invocation duration measurement

Instrument the WaitMessage **call site** (WinMain, the E8 CALL 5 bytes before
the `else` branch, approximately at byte offset `0x5dbc0` in WinMain) with a
QPC-before/after wrapper to record exact block duration per invocation.

**Why deferred:** The `inter_max` value already gives the upper bound, and with
the 1ms cap in place the stall no longer matters for performance.  Worth
revisiting if a later profiling pass needs the raw per-invocation duration
distribution (e.g., to answer "how often was it 1s vs 5s" rather than just
"how many times per window").

### Deferred — Option C: decompile the triggering condition

Decompile `FUN_14028af60` (RVA `0x28af60`) and `FUN_14024cef0` (RVA
`0x24cef0`) to understand what "pending work" and "pending input" check, and
why they return zero during active AI computation.

**Why this matters architecturally:** If AI/Lua background threads could call
`PostMessage(hwnd, WM_APP, 0, 0)` on coroutine completion, `WaitMessage` would
return immediately instead of stalling.  The IAT cap (Option B) treats the
symptom; Option C would reveal whether a self-wake mechanism is feasible and
eliminate the need for the cap entirely.

**Why deferred:** Requires a Ghidra decompile pass on two new functions.  The
MT project's immediate goal (bounding hitches) is achieved by Option B.
Revisit Option C if the mod's gameplay requires lower-latency AI feedback (e.g.,
real-time unit responses that currently stall 100–200ms due to the now-capped
WaitMessage repeatedly firing).

## Class-B Hitches (frame_max >> inter_max) — unresolved

Separate from WaitMessage.  These are hitches where `frame_max` is large but
`inter_ms` is normal, placing the stall in the **post-flush#2** section of the
loop (lines 907–931 or the `do-while` test + early-loop code before line 558).

Candidate functions (unhooked):
- `FUN_1402d72c0` (hook9, line 618) — avg grows to 0.10–0.21ms late-battle
- `FUN_1402d2ab0` (hook10, line 619)
- `FUN_140321dc0` (hook11, line 620)
- `FUN_14030c3b0` (hook13, line 639)

These are inside the `DAT_1409cf314 == 1` guard, so they only run during
active simulation.  A future profiling pass can add gapE/gapF timers or
extend the breakdown ODS to cover lines 907–931.
