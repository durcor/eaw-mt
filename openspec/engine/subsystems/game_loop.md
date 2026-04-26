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

## Class-B Hitches (frame_max >> inter_max)

Separate from WaitMessage.  These are hitches where `frame_max` is large but
`inter_ms` is normal, placing the stall in the **post-flush#2** section of the
loop (lines 907–931 or the `do-while` test + early-loop code before line 558).

### Attribution Chain — Phase 5, Steps 5–8 (2026-04-25)

The stall has been chased down through seven levels of call chain:

```
WinMain:865
  └─ FUN_14028d400  (RVA 0x28d400, "game service", 654 bytes)
       └─ vtable[22] dispatch → SpaceModeClass (RVA 0x4d95a0)  [vtable probe confirmed]
            └─ FUN_1404d95a0  (SpaceModeClass slot 22, 358 bytes)  [sslot22 ≈ gsvc]
                 └─ FUN_1403639d0  (JMP tail call, 3916 bytes)    [tail22i ≈ sslot22]
                      ├─ FUN_140364920  (0.00ms — eliminated)
                      ├─ FUN_1402be640  (×2 per call, avg 551ms each = 1102ms total)  ← CONFIRMED STALL
                      └─ FUN_140490580  (0.00ms — eliminated)
```

**Profiling data (Phase 5 Step 8, 3 windows):**

| window | tail22i avg | t2be640 avg | t2be640 n | t364920 | t490580 |
|---|---|---|---|---|---|
| 1 (stall) | 1102.60ms | 551.13ms | 52 (×2) | 0.00ms | 0.00ms |
| 2 (moderate) | 124.34ms | 62.14ms | 54 (×2) | 0.00ms | 0.00ms |
| 3 (quiet) | 2.04ms | 0.99ms | 54 (×2) | 0.00ms | 0.00ms |

The two `t2be640` calls together account for ~100% of `tail22i` time in every
window (max 22,804ms per call vs tail22i max 22,808ms — 4ms residual).

**`FUN_1402be640` (RVA 0x2be640, 1577 bytes) is the confirmed Class-B stall source.**

### FUN_1402be640 — Structure (decompiled, Phase 5 Step 8)

Called from `FUN_1403639d0` twice with different first arguments:
- Call 1: `FUN_1402be640(param_1[3], uVar1)` — line 143 of tail22 decompile
- Call 2: `FUN_1402be640(param_1[4], uVar1)` — line 147 of tail22 decompile

`param_1[3]` / `param_1[4]` are likely two different manager/collection objects
(possibly Space unit manager and Space structure manager, or similar partitions).

Internal structure:
1. **Preamble** — `FUN_14020ed70()` called if `*(param_1+0xd8) != 0`, then in a
   loop over `DAT_140a16fb0` items.
2. **Early-return gate** — if `*(param_1+0x20) == NULL` or `plVar6[0x13] == '\0'`.
3. **Per-entity service loop** (the dominant path):
   ```c
   for (; lVar12 != param_1 + 0xf0; lVar12 = *(longlong *)(lVar12 + 8)) {
       plVar6 = (longlong *)(*(longlong *)(lVar12 + 0x18) + -0x18);
       FUN_1403a6b80(plVar6, param_2, cVar3);   // per-entity call — PRIME SUSPECT
   }
   ```
   Linked list anchored at `param_1 + 0xf0/0xf8`.  Each entity serviced by
   `FUN_1403a6b80` (RVA 0x3a6b80).
4. **Second per-entity loop** — same list, vtable slot + `FUN_1403fc750`.
5. **Third per-entity loop** — list at `param_1+0x138/0x140`, calling `FUN_1403ac530`.
6. **Array iteration** — array at `param_1+0x5f8`, size at `param_1+0x600`,
   calling `FUN_14029f810` per element.
7. **Battle-state machine** — `param_1+0x618` state; `FUN_1402aca60`,
   `FUN_14044a1a0`, `FUN_1403de570`.
8. **`FUN_1402ad100`** — builds `DynamicVectorClass<GameObjectClass*>` list.
9. **`FUN_1402b92e0`** — conditional on result of step 8 + global flags.
10. **`FUN_1402b76a0`** — conditional on `param_1+0x490 != 0`.
11. **`FUN_1402a62d0(param_1)`** — unconditional final call.

### Sub-attribution inside FUN_1402be640 — Phase 5 Step 9 (2026-04-25)

E8 hooks inside FUN_1402be640 (1577 bytes):

| hook | avg | max | n (w1) | total vs t2be640 |
|---|---|---|---|---|
| `ta6b80(3a6b80)` | 86.87ms | **22,310ms** | 322 | 27,972ms ≈ 27,974ms ✓ |
| `ta62d0(2a62d0)` | 0.00ms | 0.08ms | 52 | eliminated |
| `t20ed70(20ed70)` | 0.00ms | 0.01ms | 5304 | eliminated |

`ta6b80_total ≈ t2be640_total` in all three windows — `FUN_1403a6b80` is
virtually all of `FUN_1402be640`'s time.

**Critical pattern**: window 1 has ~6 entities per call (n=322/52) and max
22,310ms; window 3 has ~70 entities per call (n=3352/48) and max 1ms.  The
stall is NOT proportional to entity count — ONE entity hits a catastrophic code
path every few hundred frames.

### FUN_1403a6b80 — Structure and Root Cause (2862 bytes, decompiled 2026-04-25)

`FUN_1403a6b80(param_1, param_2, param_3)` is the per-entity service function.
It handles: position/physics update, pending command dispatch (lines 83-129),
behavior tick (vtable[0x30], lines 132-136), movement system (lines 336-361).

**Root cause — ServiceRate/LastService AI script execution (lines 258-334):**

```c
// Read entity's AI service rate and last service timestamp from Lua values
plVar10 = FUN_140246fb0(param_1[0x5b], "ServiceRate", 0);
plVar10 = FUN_140246fb0(param_1[0x5b], "LastService", 0);

// Gate: if elapsed_time >= ServiceRate → execute entity's AI coroutine
if (elapsed >= service_rate) {
    FUN_140247a90(param_1[0x5b]);   // Pump_Threads — runs entity Lua AI coroutine
    // Update LastService = current_time (+ random jitter for load balancing, line 316)
}
```

`FUN_140247a90` is `Pump_Threads` (RVA `0x247a90`, already tracked by `pump_ms`
hook at WinMain call site).  **This is a second, untracked Pump_Threads call
site** — called on the main thread, per-entity, rate-gated by a Lua value.

Lua interaction confirmed: line 311 `*plVar10 = (longlong)LuaValue<double,4>::vftable`.

**Class-B hitch mechanism:**
1. Entity's `ServiceRate` timer fires (every N seconds, jitter via random seed)
2. `Pump_Threads(entity_script_object)` runs that entity's Lua AI coroutine on
   the **main thread**
3. The Lua script executes synchronously — for Thrawn's Revenge fleet-AI, this
   can involve O(N²) planet/unit iteration, blocking for up to 22 seconds

This is distinct from the WinMain Pump_Threads call site (which shows avg ≤ 5ms
in `pump_ms`).  The entity-level call is invisible to the existing hook because
the E8 scan at WinMain only patches the WinMain call site.

### Full Attribution Chain (confirmed, 2026-04-25)

```
WinMain:865  FUN_14028d400 (gsvc, 654 bytes)
  └─ vtable[22] → SpaceModeClass FUN_1404d95a0 (358 bytes)
       └─ JMP FUN_1403639d0 (tail22, 3916 bytes)
            └─ FUN_1402be640 (×2, 1577 bytes) — two manager objects
                 └─ FUN_1403a6b80 (per-entity, 2862 bytes) — N entities
                      └─ ServiceRate gate → Pump_Threads(entity_script)
                           └─ Lua AI coroutine executes SYNCHRONOUSLY on main thread
                                ← 22,310ms stall on ONE entity per episode
```

### Fix Direction

The per-entity `Pump_Threads` call (line 304 of FUN_1403a6b80) needs to be
moved off the main thread.  Two approaches:

**Option A — Offload entity AI service to a worker thread:**  The ServiceRate/
LastService block could be executed on a background thread pool.  Requires
thread-safe access to the entity object (param_1) and the Lua state.  Lua
is not thread-safe by default — would need a mutex or separate Lua state per thread.

**Option B — Bound per-entity AI execution time:**  Replace the
`Pump_Threads` call with a time-bounded variant that yields after N ms and
reschedules remaining work.  This keeps everything single-threaded but caps
the per-frame AI budget.

**Option C — Increase ServiceRate for expensive scripts:**  If the Lua scripts
that trigger the 22s stall are fleet-AI functions (planet evaluation, unit
assignment), increasing their ServiceRate (e.g., from 0.5s to 2s) reduces
call frequency and amortizes cost — but doesn't eliminate the per-call stall.

### Per-entity Pump_Threads confirmed — Phase 5 Step 10 (2026-04-25)

`pumpe(247a90)` hook inside `FUN_1403a6b80` confirms per-entity Lua AI is
the stall.  3-window dataset:

| window | gsvc max | ta6b80 max | pumpe max | pumpe n | pumpe total / ta6b80 total |
|---|---|---|---|---|---|
| 1 (stall) | 22,001ms | 21,994ms | 21,994ms | 13 | 92% |
| 2 (moderate) | 2,046ms | 1,050ms | **0.03ms** | 4 | ~0% |
| 3 (quiet) | 3.36ms | 1.09ms | 0.04ms | 4 | — |

Window 1: 13 entity AI firings in 300 frames, avg 1958ms each, one 22s outlier.
pumpe total (25,463ms) / ta6b80 total (27,746ms) = 92% — Pump_Threads
dominates when entities service.

**Window 2 reveals a second, independent stall source:** `pumpe` is fast
(0.03ms) but `ta6b80_max = 1050ms` and `gsvc_max = 2046ms`.  Two entity
service calls each taking ~1000ms (compounding through two `FUN_1402be640`
invocations) with zero Pump_Threads involvement.

### Second stall source — inside FUN_1403a6b80 (non-Lua)

`FUN_1403a6b80` contains several non-Pump_Threads paths that could produce
the 1050ms window-2 max.  Candidates from decompile:

- **Lines 132–136** — behavior vtable dispatch (most suspicious):
  ```c
  while (iVar5 = iVar5 + -1, -1 < iVar5) {
      plVar6 = *(longlong **)(param_1[0x4f] + (longlong)(char)iVar5 * 8);
      if ((*(uint *)(plVar6 + 6) <= param_2) &&
          (FUN_1404c3700(plVar6) == '\x01'))
          (**(code **)(*plVar6 + 0x30))(plVar6, param_1, param_2);
  }
  ```
  Indirect vtable call — can be any behavior implementation.  Pathfinding,
  formation logic, or targeting could take O(N) time.

- **Line 361** — `FUN_1403a76b0(param_1, param_2)` — movement system, always
  called when `param_1[0x5a] != 0`.

- **Line 99** — `FUN_1403a9e30(...)` — pending command dispatch.

- **Line 79** — `FUN_1405369e0(...)` — conditional preamble call.

### Second stall source isolated — Phase 5 Step 11 (2026-04-26)

E8 hooks inside FUN_1403a6b80 (2862 bytes):

| hook | window 1 | window 2 (non-Lua) | window 3 |
|---|---|---|---|
| `a76b0(3a76b0)` | avg=10.64ms max=2108ms n=200 | avg=16.72ms **max=1236ms** n=200 | avg=0.13ms max=1.03ms |
| `a9e30(3a9e30)` | n=0 | n=0 | n=0 |
| `e369e0(5369e0)` | max=0.01ms | max=0.00ms | fast |

Window 2: `a76b0_total = 16.72×200 = 3344ms ≈ ta6b80_total = 2.04×1651 = 3368ms` (99.3%) — `FUN_1403a76b0` is the entire second stall.  `a9e30` never called (no pending commands).  `e369e0` eliminated.

**`FUN_1403a76b0` (RVA `0x3a76b0`, 499 bytes) confirmed as second stall source.**

### FUN_1403a76b0 — Structure (decompiled, Phase 5 Step 11)

Movement component service function.  Called with `(entity, frame_counter)`.

```
param_1[0x2d0] → movement array (iVar1 entries)
  Loop 1: count active entries (offset 0x4d == '\x01')
  Loop 2 (if active > 0): sum FUN_140540070() values per active entry
  FUN_140396df0 + FUN_140396070 → speed range
  Loop 3 (if speed > 0): FUN_140387f50(entry, entity, 1, speed_share, 0) per active entry
  Loop 4 (unconditional): FUN_140387010(entry, param_2) for EVERY entry
```

**Line 74: `FUN_140387010(lVar6, param_2)`** is called unconditionally for every movement component — prime suspect for the 1236ms per-entity spike.

TODO: Add E8 hook for `FUN_140387010` (RVA `0x387010`) inside `FUN_1403a76b0` (499 bytes).

### Fix scope revised

The Class-B hitches have two independent sources:
1. **Pump_Threads (entity Lua AI)** — rate-gated, 92% of stall budget in
   hitch windows.  Fix: offload to worker thread (Option A) or time-bound
   execution (Option B).
2. **Behavior vtable or movement** — 1050ms per entity with no Lua
   involvement.  Distinct root cause; needs separate profiling pass to
   isolate to a specific behavior type.

Both sources must be addressed before Class-B hitches are eliminated.
