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

### FUN_140387010 confirmed as terminal stall carrier — Phase 5 Step 12 (2026-04-26)

E8 hook for `FUN_140387010` (RVA `0x387010`) added inside `FUN_1403a76b0` (499 bytes).

| Window | pumpe max | a76b0 max | b87010 max | Δ |
|---|---|---|---|---|
| W2 (non-Lua stall) | 0.03ms | 1,108ms | **1,108ms** | 0.02ms |
| W5 (non-Lua stall) | 0.11ms | 2,037ms | 1,067ms | multi-call sum |
| W8 (non-Lua stall) | 0.03ms | 1,063ms | **1,062ms** | 1ms |
| W11 (non-Lua stall) | 0.03ms | 1,070ms | **1,070ms** | 0.06ms |
| W12 (non-Lua stall) | 0.04ms | 1,079ms | **1,079ms** | 0.44ms |

In 4 of 5 non-Lua stall windows, a single `FUN_140387010` call accounts for ≥99.9% of the `a76b0` time.  `n ≈ 11,000–19,000` calls per 300-frame window (≈55 calls per `a76b0` invocation = movement components per entity).  `a9e30` never fires, `e369e0` eliminated.

**`FUN_140387010` (RVA `0x387010`, 344 bytes) confirmed as terminal stall carrier — single call takes 1+ seconds.**

### FUN_140387010 — Structure (decompiled, Phase 5 Step 12)

Per-movement-component tick.  Arguments: `(component, frame_counter)`.

```
param_1 = movement component ptr
param_2 = frame counter (int)

Guards (early return if any false):
  param_1+0x20 (entity ptr) != 0
  param_1+0x10 (owner ptr) != 0
  *(owner+0x3a0) & 2 == 0        (entity not flagged as suspended)
  DAT_140b15418 == 0 || FUN_14035f790() == 0  (global pause flag)

Tick:
  iVar7 = param_2 − param_1+0x60   (delta ticks)
  param_1+0x60 = param_2            (store current tick)

  if (entity+0x48 in [5,10]) && param_1+0x6c == 1:
    FUN_140387400(component, iVar7)   (path-following update)

  if entity+0x4e==1 && param_1+0x6c==1 && owner!=0 && param_1+0x28>0.0:
    if param_1+0x94 < 0:            (cold cache — no path slot yet)
      lVar5 = FUN_140385cf0(param_1)  (path-system lookup by entity name)
      if lVar5 == 0: goto tail
      iVar3 = FUN_14012d2a0(lVar5, entity+0x1c0_string)  (name→slot index)
      if iVar3 < 0: goto tail       (not found — STAYS uncached, retries every tick)
      param_1+0x94 = iVar3          (cache slot index)
      FUN_14012d430(lVar5, iVar3, 1)
      [if entity+0x1f0 != 0: same for secondary slot → param_1+0x98]
    FUN_140381ff0(param_1)            (main movement step — calls FUN_140385cf0 again)

tail:
  if param_1+0x6f == 1:
    FUN_140387170(component, iVar7)   (rotation update)
```

**Cold-cache failure bug:** if `FUN_14012d2a0` returns -1 (name not found in path system), `param_1+0x94` is never updated from -1.  The cold-cache block re-runs every tick for that component, calling `FUN_140385cf0` twice per tick (once in the cold-cache branch, once inside `FUN_140381ff0`).

### FUN_140385cf0 — Structure (decompiled, Phase 5 Step 12)

Path-system name lookup (RVA `0x385cf0`, 376 bytes).  Called from cold-cache branch and from `FUN_140381ff0`.

```
Guards: entity valid, entity+0x4e (move-enabled), owner valid, owner+0x2a0 (nav ptr) != 0
lVar4 = FUN_1402648b0()            (get global nav/physics manager singleton)
FUN_14001e680(local_138, entity+0x60)  (copy entity name string, SSO pattern)
if local_128 == 0: return lVar4    (empty name → return manager as-is)
if param_1+0x90 < 0:               (inner cold cache)
  _strupr(local_118)               (uppercase name)
  strip at DAT_140819540 delimiter
  iVar3 = FUN_14012d520(lVar4, local_118)  (name→index in manager)
  param_1+0x90 = iVar3
  if iVar3 < 0: return 0
lVar4 = FUN_14012d6f0(lVar4, iVar3)  (get object by cached index)
return lVar4
```

`FUN_14012d520` (name→index lookup in global nav manager) is the prime stall suspect inside `FUN_140385cf0`.  If it performs a linear scan of a large registry, it could account for 1+ seconds.

### FUN_140381ff0 — Structure (decompiled, Phase 5 Step 12)

Main movement step (RVA `0x381ff0`, 746 bytes).

```
lVar2 = FUN_140385cf0()            (path-system lookup — second call per tick)
if lVar2 == 0: return 0
FUN_140383650(param_1, local_98)   (compute position/direction)
cVar1 = FUN_140386170(local_98, param_1+0x9c)  (at destination?)
if not at destination:
  [speed interpolation math using entity+0x4c0 acceleration]
  FUN_14012d480(lVar2, param_1+0x94, &position_vec)  (write position to path system)
  if param_1+0x98 >= 0:
    FUN_14012d480(lVar2, param_1+0x98, &rotation_vec) (write rotation to path system)
return 1
```

`FUN_14012d480` writes position/rotation into the path system.  If the path system is physics-backed, this may synchronize with a background physics thread — a second candidate stall point.

### Phase 5 Step 13 result — d12d520 and d12d480 both n=0 (2026-04-26)

E8 hooks inside `FUN_140385cf0` (376 bytes) and `FUN_140381ff0` (746 bytes) showed `n=0` across all windows including non-Lua stall windows.

- `d12d520 n=0` — `FUN_14012d520` only fires when `param_1+0x90 < 0` (inner cold-cache); all movement components have warm inner caches by the time profiling windows are collected.
- `d12d480 n=0` — `FUN_140381ff0` is never called during stall windows (outer guard `entity+0x4e==1 && speed>0` fails for entities in motion state 5–10).

Stall is in a different callee of `FUN_140387010`.  Full call map dumped:

| Offset | RVA | Function |
|---|---|---|
| +0x040 | 0x35f790 | FUN_14035f790 (global pause check) |
| **+0x074** | **0x387400** | **FUN_140387400 (path-following update)** |
| +0x0b5 | 0x385cf0 | FUN_140385cf0 (path-system lookup, cold cache) |
| +0x0da | 0x12d2a0 | FUN_14012d2a0 (×2, tiny wrapper) |
| +0x0f1 | 0x12d430 | FUN_14012d430 (×2, path slot acquire) |
| +0x133 | 0x381ff0 | FUN_140381ff0 (main movement step) |
| +0x143 | 0x387170 | FUN_140387170 (rotation update) |

### FUN_140387400 confirmed as terminal stall carrier — Phase 5 Step 14 (2026-04-26)

E8 hooks for `FUN_140387400`, `FUN_140385cf0`, and `FUN_140381ff0` via scan inside `FUN_140387010` (344 bytes).

| Window | pumpe max | b87010 max | b387400 max | b385cf0 n | b381ff0 n |
|---|---|---|---|---|---|
| W1 (Lua stall) | 21,716ms | 1,113ms | **1,113ms** | 0 | 0 |
| W2 (non-Lua stall) | 0.04ms | 1,079ms | **1,079ms** | 0 | 0 |
| W6 (non-Lua stall) | 0.11ms | 1,090ms | **1,090ms** | 0 | 0 |
| W8 (non-Lua stall) | 0.04ms | 1,038ms | **1,038ms** | 0 | 0 |
| W10 (non-Lua stall) | 0.04ms | 1,069ms | **1,069ms** | 0 | 0 |
| W11 (non-Lua stall) | 0.04ms | 1,055ms | **1,055ms** | 0 | 0 |
| W12 (non-Lua stall) | 0.04ms | 1,091ms | **1,091ms** | 0 | 0 |

In every stall window: `b387400_max == b87010_max` (Δ<1ms). `b385cf0 n=0` always (cold cache not triggered). `b381ff0 n=0` always (outer guard never passes for entities in motion state 5–10).

**`FUN_140387400` (RVA `0x387400`, 1904 bytes) is the true terminal stall carrier.**
b387400 fires for ~89% of b87010 calls (entities in motion state `entity+0x48 ∈ [5,10]`).

### FUN_140387400 — Structure (decompiled, Phase 5 Step 14)

**Opportunity target acquisition loop.** `OpportunityTargetAcquiredDataClass::vftable` visible at line 300. Arguments: `(component, delta_ticks)`.

```
param_1 = movement component ptr
param_2 = delta ticks (uint)

Guards (10 early exits):
  entity ptr (param_1+0x20) != 0
  entity+0x48 in [0,5] (motion state check, redundant with caller)
  DAT_140a28640 != 0    (global opportunity-target enable flag)
  param_1+0x6c != 0     (component active)
  NOT (entity+0x4d==1 && speed<=0)
  param_1+0x6e == 0     (not suppressed)
  *(owner+0x3a0) & 0x10 == 0  (entity not in special state)
  FUN_140396fe0() == 0  (global game state check)
  FUN_14039a400(owner) == 0   (entity reachability check)
  FUN_14039b2a0(owner) == 0   (entity reachability check)

Rate-limit timer (param_1+0x58):
  if countdown > 0: decrement, return if not expired
  (fires only once per N delta-ticks)

Target validity:
  if entity+0x50==1 (hyperspacing/formation): check target at param_1+0xc0
  else: FUN_140384850(param_1, param_1+0x40) — can reach current target?

[Various entity validity/state checks...]

Opportunity target search (THE STALL):
  iVar2 = (DAT_140a16fd8 - DAT_140a16fd0) >> 3  ← TOTAL ENTITY COUNT in game
  fires when: current target cleared (bVar3) OR elapsed > DAT_140b0a340*DAT_1408007c0 threshold
  iVar7 = FUN_1401ffb40(..., 0, iVar2-1)         ← random start index
  do {
      lVar11 = FUN_140294bc0(&DAT_140a16fd0, iVar7)  (get entity from global list)
      if FUN_1402824f0(local_res20, lVar11) &&        (can path to entity?)
         FUN_140385190(param_1, lVar11, local_res18) != 0 &&
         local_res18[0] == DAT_140899780:              (reachability + score metric)
          → found → break
      iVar13++
  } while (iVar13 < iVar2)                        ← O(N_entities) iterations

  if found: FUN_140382510(param_1, lVar12, plVar8)  (acquire target)

Target acquired:
  FUN_1403825b0(param_1, *plVar8, 0)              (try to path to acquired target)
  if acquired: signal via FUN_140220ed0(..., OpportunityTargetAcquiredDataClass)
```

**Stall mechanism:** When an entity's opportunity target is cleared OR a periodic timer fires, the function iterates through **every entity in the game** calling `FUN_140385190` (reachability + score check) for each candidate.  With a large fleet (hundreds of entities) this loop is O(N) expensive per firing per movement component.

**Key globals:**
- `DAT_140a16fd0` / `DAT_140a16fd8` — global entity list (pointer array, `>> 3` gives count)
- `DAT_140b0a340 * DAT_1408007c0` — opportunity target search interval threshold
- `DAT_140899780` / `DAT_140899788` — score constants used by `FUN_140385190`
- `OpportunityTargetAcquiredDataClass::vftable` — event class for target acquisition signal

### Updated attribution chain (Phase 5 Step 14 final)

```
WinMain:865 → FUN_14028d400 (gsvc)
  → vtable[22] → SpaceModeClass FUN_1404d95a0
    → JMP FUN_1403639d0 (tail22)
      → FUN_1402be640 (×2, per manager)
        → FUN_1403a6b80 (per-entity)
          ├─ ServiceRate gate → Pump_Threads → Lua AI         [Source A — 22,310ms]
          └─ FUN_1403a76b0 (movement)
               → FUN_140387010 (per-component)
                    └─ FUN_140387400 (path-following / opp-target) [Source B — 1,090ms]
                         └─ O(N_entities) loop: FUN_140385190 per candidate
```

### Fix B profiling — Phase 5 Step 15 (2026-04-26)

Fix B (interval patch + FUN_140385190 budget cap) was built and profiled.
`FUN_140385190` and `FUN_140384850` were both hooked inside `FUN_140387400`
to determine which callee carries the stall.

**Results (3-window dataset, non-Lua stall windows):**

| hook | avg | max | n (per 300f) |
|---|---|---|---|
| `b387400(387400)` | 0.34ms | **1,072ms** | 12,090 |
| `b385190(385190)` | 0.00ms | 0.22ms | ~10,500 | ← search loop candidate check — eliminated |
| `b384850(384850)` | 0.00ms | **0.01ms** | ~8,700 | ← "can reach current target?" — eliminated |
| `b3825b0(3825b0)` | 1.10ms | **1,072ms** | ~3,750 | ← **matches b387400_max** |

`b3825b0_max = 1,072.79ms` vs `b387400_max = 1,072.80ms` (Δ = 0.01ms) — conclusive.

**Fix B2 (FUN_140385190 cap) was misdirected:** `b385190_capped = 0` in every window.
The search loop averages <1 FUN_140385190 call per b387400 invocation and max 0.22ms —
it was never the stall source.  Fix B2 had zero effect.

**Fix B1 (interval 0→60) effect unknown:** stall magnitude unchanged, but the stall
source is now confirmed to be FUN_1403825b0, not the search loop, so the interval
patch may have had no relevance.

### FUN_1403825b0 confirmed as true terminal stall carrier — Phase 5 Step 15 (2026-04-26)

**`FUN_1403825b0` (RVA `0x3825b0`) is the true terminal stall carrier inside
`FUN_140387400`.**

Called ~3,750 times per 300-frame window (~31% of b387400 invocations).  Four call
sites in FUN_140387400:
- Lines 171, 176: formation/hyperspacing path — updates path to cached target
  (`param_1+0xc0`) every tick for entities in formation or hyperspace
- Lines 232, 295: post-search path — called after the O(N) search finds a new
  opportunity target

The high call count (3,750/300 frames = ~12.5/frame) confirms lines 171/176
dominate — this is not just new-target acquisition, it's a per-tick path
update for formation/hyperspacing entities.

### Updated attribution chain (Phase 5 Step 15 final)

```
WinMain:865 → FUN_14028d400 (gsvc)
  → vtable[22] → SpaceModeClass FUN_1404d95a0
    → JMP FUN_1403639d0 (tail22)
      → FUN_1402be640 (×2, per manager)
        → FUN_1403a6b80 (per-entity)
          ├─ ServiceRate gate → Pump_Threads → Lua AI       [Source A — 22,310ms]
          └─ FUN_1403a76b0 (movement)
               → FUN_140387010 (per-component)
                    └─ FUN_140387400 (path-following / opp-target)
                         └─ FUN_1403825b0 (path update)   [Source B — 1,072ms]
```

### Fix scope — both sources identified

**Class-B hitch Source A — Lua AI (Pump_Threads):**
- Root: Lua AI coroutine runs synchronously on main thread, rate-gated by entity ServiceRate
- Fix A1: offload entity AI to worker thread (Lua thread-safety required)
- Fix A2: time-bound per-entity AI execution, yield after N ms

**Class-B hitch Source B — Path update (FUN_1403825b0):**
- Root: per-tick path update for formation/hyperspacing entities; pathfinding call
  inside FUN_1403825b0 occasionally takes 1+ seconds
- Fix scope requires decompile of FUN_1403825b0 to identify the blocking callee

### FUN_1403825b0 — Structure (decompiled, Phase 5 Step 15)

"Validate and submit movement command to path-following target" (4034 bytes).
Arguments: `(component, target_entity*, formation_data*)`.

```
Guards (lines 61–104 — fast, all return 0 on failure):
  entity ptr (param_1+0x10) != 0
  target entity (param_2) != 0
  formation_data target matches param_2 (if param_3 != 0)
  target not flagged 0x40 (team/faction check)
  vtable[0x10](param_2, 0x11) == 0  (target alive/valid)
  FUN_14039b140(param_2) — target reachability state
  FUN_140540140 — entity weapon/targeting gate
  FUN_14035f470 — game-mode pause / fleet zone check
  FUN_14039a540 — allied/neutral entity check

Param_3 setup (lines 106–108, if param_3==NULL):
  FUN_140398440 — get owner physics state
  param_3 = FUN_140394a80(param_2, …) — resolve formation slot from target

Path warmth check (lines 111–145, move-enabled entities only):
  FUN_140385cf0 — path-system lookup (warm cache only; returns 0 → early exit)
  FUN_14012d2c0 — read current position from path slot
  Distance gate: dot product of (current_pos − target_pos) vs speed tolerance

Movement type resolution (lines 157–162):
  FUN_140397e00(owner, target, team_id) — look up movement type by team
  FUN_14039b950 — fallback movement type checks

Deep path (lines 163–503, only when movement type found):
  FUN_140383f70 — compute approach position from target geometry
  FUN_140385c70 — alternative: read approach position from formation data
  FUN_140385e70 — formation/physics position compute     ← suspect (gates block)
  sqrt + FUN_1403857d0 + FUN_140397780 — distance + radii
  FUN_140399450(nav_mgr, movement_type, team_id, pos, dir, formation)
                  ← attack-position / approach-vector calc  ← PRIME SUSPECT
  FUN_140383ba0 — final path reachability check
  FUN_1403973b0 — get object type data (fast)
  FUN_140381dc0(component, type_data, dist, pos, xml_type, formation)
                  ← path direction compute                   ← PRIME SUSPECT
  FUN_14029f810(nav_mgr, movement_type, team_id, approach_dir, target_pos,
                formation_mode, 0)
                  ← MOVEMENT ORDER + PATHFINDING SUBMISSION  ← PRIME SUSPECT
                  returns plVar12 (movement order struct)
  [plVar12 populated: target entity, timing, speed, team, slot, …]
  FUN_1402d5240 — signal dispatch (fleet formation update)
  FUN_1403a06a0 — notify entity of new movement command
  Rate-limit timer update: param_1+0x58 = new countdown value
  FUN_1403846c0 — release previous target (if changed)
  FUN_140382510 — acquire new target
```

### FUN_14029f810 confirmed as terminal stall carrier — Phase 5 Step 16 (2026-04-26)

E8 hooks inside FUN_1403825b0 (4034 bytes) for all four suspects:

| hook | avg | max | n (per 300f) |
|---|---|---|---|
| `b385e70(385e70)` | 0.00ms | **0.00ms** | 27 | ← eliminated |
| `b399450(399450)` | 0.00ms | **0.00ms** | 27 | ← eliminated |
| `b381dc0(381dc0)` | 0.00ms | **0.00ms** | 5  | ← eliminated |
| `b29f810(29f810)` | 434.95ms | **1,094.33ms** | 5 | ← matches b3825b0_max (Δ=0.02ms) |

`b29f810_max = 1,094.33ms` vs `b3825b0_max = 1,094.35ms` — conclusive match.

**`FUN_14029f810` (RVA `0x29f810`) is the true terminal stall carrier.**

Called only 5 times per 300 frames (18% of b3825b0 invocations — only when all
guards pass AND the formation position gate AND the distance/reachability checks
succeed).  Average per call: 435ms.  The other three callees are fast and
eliminated as stall sources.

### Updated attribution chain (Phase 5 Step 16 final)

```
WinMain:865 → FUN_14028d400 (gsvc)
  → vtable[22] → SpaceModeClass FUN_1404d95a0
    → JMP FUN_1403639d0 (tail22)
      → FUN_1402be640 (×2, per manager)
        → FUN_1403a6b80 (per-entity)
          ├─ ServiceRate gate → Pump_Threads → Lua AI         [Source A — 22,310ms]
          └─ FUN_1403a76b0 (movement)
               → FUN_140387010 (per-component)
                    └─ FUN_140387400 (path-following / opp-target)
                         └─ FUN_1403825b0 (validate + submit movement cmd)
                              └─ FUN_14029f810 (movement order creation)  [Source B — 1,094ms]
                                   └─ FUN_140388b60 (GameObjectClass ctor)
                                        └─ FUN_1403989a0 (spatial/path registration) ← PRIME SUSPECT
```

### FUN_14029f810 — Structure (decompiled, Phase 5 Step 16)

"Create and dispatch movement order" (2201 bytes).

```
Line 38:  lVar3 = FUN_140769c58(0x8a0)      — pool-alloc 2208-byte movement order struct
Line 43:  plVar4 = FUN_140388b60(lVar3, nav_mgr, movement_type, team_id,
                                  approach_dir, target_pos, formation_mode, flag)
Lines 49–155: FUN_14020a9b0 × N            — queue/list insertions (entity slots)
Lines 96–107: loop over nav_mgr entity list — FUN_14037cab0 per entry (bounded)
Lines 168–176: weapon slot loop            — fast
Lines 228–241: O(N_entities) loop          — FUN_1402823e0(owner, entity) per entry,
               gated by plVar4[0x53]+0x2120 — fires only for certain entity types
Lines 250–265: movement component updates  — FUN_1403935b0, FUN_1403a8ad0, etc.
```

### Phase 5 Step 17 — FUN_140388b60 decompiled: GameObjectClass ctor, not pathfinding

Decompile of `FUN_140388b60` (RVA `0x388b60`, 1998 bytes) shows it is a
**`GameObjectClass` constructor / initializer** for the 2208-byte movement order
struct, **not** a pathfinding solver.  It:

1. Sets vtable pointers: `RootClass`, `MultiLinkedListMember`, `CullObjectClass`,
   `GameObjectClass` — the struct has multiple inheritance.
2. Zeroes ~130 fields in a do-loop and scattered explicit writes.
3. Copies position/orientation vectors from input params into struct fields.
4. Calls `FUN_1403989a0(param_1, 0, 1)` (line 188) — the only non-trivial call.
   This is almost certainly "register this movement order with the movement /
   pathfinding system" (spatial index insertion, or synchronous path solve).
5. The two waypoint-callback loops (lines 192–225 and 260–265) are gated by
   `*(char *)(param_1+0x51)` which is initialized to 0x00 (the short write
   `*(undefined2 *)(param_1+0x51) = 0x100` puts 0x00 in the low byte on LE),
   so they are **always skipped** at this call site.

The stall therefore lies in `FUN_1403989a0` (RVA `0x3989a0`) rather than in the
field-initialization work.  Two new hooks are installed (Phase 5 Step 17):

- **`b388b60`** — E8 scan in `FUN_14029f810` body (2201 bytes): measures the
  entire ctor (field init + `FUN_1403989a0` call).
- **`b3989a0`** — E8 scan in `FUN_140388b60` body (1998 bytes): measures only
  `FUN_1403989a0`, isolating whether the stall is the spatial/path insert or
  the field-init preamble.

### Phase 5 Step 17 — profiling confirmed: b3989a0 = b388b60 (Δ ≤ 0.01ms)

Multiple 300-frame windows confirm:

| Window | gsvc max | b29f810 max (n) | b388b60 max (n) | b3989a0 max (n) |
|--------|----------|-----------------|-----------------|-----------------|
| W1 (init) | 21872ms | 0.00ms (0) | 1034.09ms (16) | 1034.08ms (16) |
| W3 | 2318ms | 1178.89ms (4) | 1178.88ms (4) | 1178.88ms (4) |
| W5 | 2129ms | 1092.95ms (219) | 1092.93ms (227) | 1092.93ms (227) |
| W7 | 1150ms | 1144.54ms (541) | 1144.53ms (1053) | 1144.53ms (1053) |

Key observations:
- **b3989a0_max = b388b60_max (Δ ≤ 0.01ms always)** — struct field-init preamble
  is negligible; `FUN_1403989a0` accounts for 100% of `FUN_140388b60` time.
- **b388b60_count ≈ 2× b29f810_count** — `FUN_14029f810` has multiple callers
  beyond `FUN_1403825b0`; the movement order / pathfinding stall is systemic
  across all path requests, not just the Source B chain traced from b3825b0.
- In non-stall frames b3989a0 is fast (avg 0.02ms); in stall frames it blocks
  for up to 1178ms. This bimodal behavior points to a conditional slow path
  inside `FUN_1403989a0`.

### Phase 5 Step 18 — FUN_1403989a0 decompiled: movement order init + path solve

`FUN_1403989a0` (RVA `0x3989a0`, 2733 bytes, called as `(param_1, 0, 1, ?)`):

```
Lines 36–101:  Zero/reset fields — conditional on param_4 (likely 0 here, skipped)
Lines 102–128: Copy 7 default waypoint slots from global constants
Lines 129–150: Loop over movement-type slot array — update ref-counted object refs
Lines 151–170: Register path start pos; FUN_1403a4820, FUN_1403a8710, FUN_1403ac530
Lines 175–249: Movement-type path setup (fires always, param_2=0):
               FUN_14038cb30 × 2 (lines 178, 196) ← SUSPECT A
               Inner loop: alloc + FUN_1404b3580/1404b3900/1403b01f0 per type slot
Lines 251–404: Hardpoint resolution (gated by param_1[0x3a0] ≥ 0 AND path obj):
               Per hardpoint: FUN_140381a90 + FUN_14012d2a0 × 4 + FUN_140384740
Lines 406–425: Path solve (gated by FUN_140374da0 = "needs pathfinding?"):
               alloc 0x3b8 bytes → FUN_14055d530 (path struct init)
               FUN_14037c050(path_struct+8, move_order, 0, 1, …) ← SUSPECT B
               FUN_14037d7e0 → success check → FUN_14029bfe0 notification
Lines 431–438: FUN_140532990 (final spatial insertion or notification)
```

`FUN_14037c050` (RVA `0x37c050`) at line 418 is the prime suspect: it receives the
952-byte path result buffer and all movement parameters and is gated by a
"needs pathfinding?" check — this is the synchronous A\* or steering solve.
`FUN_14038cb30` (RVA `0x38cb30`, called twice) is the secondary suspect.

The bimodal timing (0.02ms fast / 1178ms slow) matches a conditional pathfinding
gate: in most frames `FUN_140374da0` returns false (path cached or trivial) so
`FUN_14037c050` is skipped; occasionally it returns true and the full path is
solved synchronously.

### Phase 5 Steps 19–21 — b3989a0 subcallee elimination and b3a4820 confirmed (2026-04-27)

**Step 19 — b38cb30/b37c050 eliminated, hardpoint loop never fires:**

Profiling data (3-window aggregate, b3989a0 n≈800 per window):
```
b38cb30(38cb30): avg=0.00  max=0.01 ms  (n=1608) ← ELIMINATED
b37c050(37c050): avg=0.00  max=0.00 ms  (n=0)    ← ELIMINATED — gate always false
b381a90(381a90): avg=0.00  max=0.00 ms  (n=0)    ← ELIMINATED — hardpoints n=0
b384740(384740): avg=0.00  max=0.00 ms  (n=0)    ← ELIMINATED — hardpoints n=0
b3989a0(3989a0): avg=20.17 max=1106.68 ms (n=214) ← STALL NOT IN ANY ABOVE
```

`FUN_140374da0` (the "needs pathfinding?" gate) always returns false for
space-battle movement orders in this scenario, so `FUN_14037c050` is never
invoked. The hardpoint loop (lines 251–404 of b3989a0) also never fires
(`iVar5 = *(int *)(*(longlong *)(param_1 + 0x298) + 0xe40)` is always ≤ 0).

**Step 20 — 6 more b3989a0 subcallees added; b3a4820 identified:**

Added hooks for: b3727a0, b3a4820, b3a8710, b3ac530, b38cf30, b3a59f0.
Lesson: `FUN_1403a59f0` and `FUN_140384740` both showed 0 args at their call
sites in the b3989a0 decompile but each has real args in its own decompile —
always check the callee's own decompile, not just the call site.

**Step 21 — FUN_1403a4820 confirmed as stall carrier (2026-04-27):**

Profiling data (two windows with b3989a0 n=730, n=1275):
```
b3989a0(3989a0): avg=2.95  max=1074.37 ms (n=730)
b3727a0(3727a0): avg=0.00  max=0.01  ms (n=730)  ← eliminated
b3a4820(3a4820): avg=2.95  max=1074.36 ms (n=730) ← STALL SOURCE — max matches b3989a0
b3a8710(3a8710): avg=0.00  max=0.00  ms (n=730)  ← eliminated
b3ac530(3ac530): avg=0.00  max=0.01  ms (n=730)  ← eliminated
b38cf30(38cf30): avg=0.00  max=0.00  ms (n=0)    ← eliminated
b3a59f0(3a59f0): avg=0.00  max=0.00  ms (n=0)    ← eliminated
```

`FUN_1403a4820` (RVA `0x3a4820`, 2481 bytes) accounts for 100% of b3989a0's
stall. Its max (1074.36ms) matches b3989a0's max (1074.37ms) to within 0.01ms.
It is called once per movement order init, gated by `DAT_140b2c37b == '\0'`
(always true in this scenario).

### FUN_1403a4820 — Structure (decompiled, Phase 5 Step 21)

`void FUN_1403a4820(longlong *param_1)` — 2481 bytes, 317 decompile lines.
Purpose: **opponent/target acquisition** — determines what enemy unit a
movement order should target, updates `param_1[0x54]` (the target slot).

```
Lines 32–33:   Guard — return if param_1[0x53] (fleet/group ptr) is null
Lines 35–51:   If "is attacking" flag set:
               FUN_14020bd00 — range/attack check → return if result==1
               FUN_14035f470 — conditional recheck → return if true
               FUN_1403751f0 — targeting valid check → return if false
Lines 53–76:   [UNCONDITIONAL] FUN_140294bc0 — frame/time query
               vtable[0xe0](plVar6) — get movement type
               FUN_1403729f0(fleet, movement_type, move_order, vehicle_list)
                   → returns candidate target; null → return early
Lines 62–73:   Optional loop: if formation flag set, walk formation members
               for a better target candidate (goto LAB_1403a49b0)
Lines 79–80:   [FAST EXIT] if candidate == current target → return (no change)
Lines 82–89:   Walk pending-cancel list → return if order is being cancelled
Lines 91–93:   FUN_140397680 — get fleet context → return if null
Lines 95–101:  FUN_1403a5840(param_1) — teardown existing assignment
Lines 102–105: vtable destroy old target ref (param_1[0x54])
Lines 106–111: Recheck movement type; FUN_140370320 × 2 → return if gated
Lines 112–165: Assign new target (vtable slot 0x20); setup attack tracking
Lines 170–265: FUN_140395920, FUN_140264a40, FUN_140265560(6 args);
               inner loop + FUN_14048a670, FUN_14020abe0;
               FUN_14033e340/FUN_140281940, FUN_14012dc40, FUN_14048a3e0
Lines 270–314: FUN_140373ac0; FUN_140264000(5 args); FUN_140265f20;
               FUN_140266340; FUN_140264c80; FUN_140264750 × 2;
               FUN_140264f00; FUN_140375250/260; FUN_1402646f0/264720
```

Key observation: `FUN_1403729f0` (line 61, 4 args, unconditional at this
point) is called before the fast-exit at line 79. If it does an O(N) scan of
potential targets (fleet-size dependent), it is the likely root cause.

### Phase 5 Step 22 — FUN_1403729f0 confirmed, FUN_140375380 identified (2026-04-27)

**b3729f0 confirmed (two windows):**
```
b3a4820(3a4820): avg=47.62 max=1047.38 ms (n=22)
b3729f0(3729f0): avg=47.61 max=1047.36 ms (n=22)  ← STALL SOURCE — max matches
b3a5840(3a5840): avg=0.00  max=0.00  ms (n=0)     ← eliminated
b265560(265560): avg=0.00  max=0.00  ms (n=0)     ← eliminated
```

b3729f0 n=22 matches b3a4820 n=22; max differs by 0.02ms (noise). 100% of
b3a4820's stall is in FUN_1403729f0.

FUN_1403729f0 (228 bytes, 32 decompile lines) is almost entirely a thin
dispatcher: it calls `FUN_140375380(param_1, param_2)` and returns a field
from param_1 based on conditions. The work is entirely in b375380.

### FUN_140375380 — Structure (decompiled, Phase 5 Step 22)

`void FUN_140375380(longlong param_1, int param_2)` — 5645 bytes, 989 lines.
Purpose: **opponent/target slot update** — resolves which enemy object should
be assigned to each target slot (param_1+0x288/0x290/0x298/0x2a0/0x2a8/0x2b0)
for a given fleet/movement-type. Updates ref-counted target object references.

```
Lines 58–66:   Normalize param_2 (-1 → use global DAT/vtable for movement type)
Lines 67–68:   Guard — return if DAT_140b15490 set OR object flag +0x11 not set
Lines 74–200:  param_2==0 (movement type 0): update slots +0x288, +0x290, +0x298
               via FUN_14025ec10(path_string, filter_ptr) — 3 call sites
Lines 203–415: param_2==1 (movement type 1 — combat): complex logic
               Lines 221–259: faction-list linear scan; FUN_140022730 × 2
               Lines 266–338: FUN_14025ec10 × 2 + FUN_1403718f0 (line 303)
Lines 417–590: param_2==2 (movement type 2): update slots +0x2a8, +0x2b8
               FUN_14025ec10 × 2 + FUN_1403718f0 (line 476)
               Line 513: RECURSIVE CALL FUN_140375380(param_1+0x8c0, 2)
Lines 592–end: Hardpoint loop: for each hardpoint at param_1+0xe38 array
               (count at param_1+0xe40): FUN_14025ec10 per hardpoint × N
               Line 977: RECURSIVE CALL FUN_140375380(lVar11, local_res10)
```

`FUN_14025ec10(char *path, char *filter)` — 139 lines, returns object pointer.
Called 8 times in b375380 + N times in hardpoint loop. This is a
**resource/object lookup by art-model path string** (`"Data/Art/Models/..."`).
If it does a linear scan of all loaded game objects or a slow hash miss,
it is the root cause of the stall.

### Phase 5 Step 23 — FUN_14025ec10 confirmed as terminal stall source (2026-04-27)

Sub-callee hooks inside `FUN_140375380` (5645 bytes):
- `b25ec10` (8 sites patched) — art-model path-string lookup
- `b3718f0` (2 sites patched) — hash-table slot lookup

Representative stall windows:

| Window | b3729f0 max | b25ec10 max | b25ec10 n | b3718f0 max |
|---|---|---|---|---|
| W1 | 3235.77ms | **1443.21ms** | 30 | 0.00ms |
| W2 | 1080.70ms | **1080.70ms** | 4 | 0.00ms |
| W3 | 1076.18ms | **1076.17ms** | 2 | 0.00ms |
| W4 | 1103.70ms | **1103.69ms** | 2 | 0.00ms |
| W5 | 1054.17ms | **1054.17ms** | 1 | 0.00ms |
| W6 | 1071.47ms | **1071.47ms** | 1 | 0.00ms |
| W7 | 1068.80ms | **1068.80ms** | 1 | 0.00ms |
| W8 | 1059.66ms | **1059.66ms** | 1 | 0.00ms |
| W9 | 1067.91ms | **1067.91ms** | 1 | 0.00ms |

**`FUN_14025ec10` max = `b3729f0` max to within <0.02ms in every stall window.**
**`FUN_1403718f0` always 0.00ms — eliminated.**

Key behavioral observations:
- `b25ec10` n is 1–4 per 300-frame stall window while `b3729f0` n is in the hundreds
  (typically 97–424) — a single rare call triggers the stall, the rest are sub-ms
- Non-stall windows: `b25ec10` n=0 (not called) or <1ms (fast path taken)
- The expensive invocation is **conditional** — most calls return quickly via an
  early-exit path; one call per movement order occasionally hits the slow path

**`FUN_14025ec10` (RVA `0x25ec10`) CONFIRMED as terminal stall source.**

### FUN_14025ec10 — Slow-path analysis (Phase 5 Step 23)

Signature: `undefined8 ******* FUN_14025ec10(char *param_1, char *param_2)` — 139 lines.
`param_1` = art-model path string (e.g. `"Data/Art/Models/..."`)
`param_2` = optional filter string (often NULL)

Fast path (most calls):
```
FUN_140142d70(DAT_140a62700, param_1)  → plVar2 (object by name)
if plVar2 == NULL → return NULL immediately
vtable walk: (**(code **)(*plVar2 + 0x20))(plVar2) → iterate linked list
  if sentinel == NULL → return pppppppuVar4 (no animation, fast return)
  if sentinel != &DAT_140a12370 → advance → eventually returns
```

Slow path (triggered when vtable walk hits `&DAT_140a12370`):
```
_splitpath(param_1, drive, dir, fname, ext)   // decompose path
if param_2 != NULL: _splitpath(param_2, ...)  // override fname from filter
builtin_strncpy(ext, "ALA", 4)                // force .ALA extension

loop i = 0 .. 0x76 (max 118 iterations):
    build string: local_528 = sprintf("%s_%s_%02d", fname, variant, i)
    build path:   local_628 = _makepath(drive, dir, local_528, "ALA")
    FUN_140142f80(DAT_140a62700, local_628)    // "is this asset registered?"
    if not found:
        FUN_140141f70(DAT_140a62700, local_628) // "try to load asset"
        if now found: FUN_140142f80(...)         // re-check after load
    if found:
        FUN_140263fa0(result, asset, i, variant, local_528)  // add to result
```

The inner loop calls `FUN_140141f70` up to 118 times. This function is a
**synchronous asset load** — it reads from disk (or the `.MEG` archive) on
cache miss. One stall = one call to `FUN_14025ec10` where the asset is not
cached, iterating up to 118 candidate filenames and blocking on each I/O check.

### FUN_1403718f0 — Eliminated (Phase 5 Step 23)

`FUN_1403718f0(longlong, uint, int, int)` — 59 lines, pure hash-table probe.
Always 0.00ms. Not a contributor.

### Updated attribution chain (Phase 5 Step 23 confirmed)

```
WinMain:865 → FUN_14028d400 (gsvc)
  → vtable[22] → SpaceModeClass FUN_1404d95a0
    → JMP FUN_1403639d0 (tail22)
      → FUN_1402be640 (×2, per manager)
        → FUN_1403a6b80 (per-entity)
          ├─ ServiceRate gate → Pump_Threads → Lua AI    [Source A — Class-B hitch]
          └─ FUN_1403a76b0 (movement)
               → FUN_140387010 (per-component)
                    └─ FUN_140387400 (path-following / opp-target)
                         └─ FUN_1403825b0 (validate + submit movement cmd)
                              └─ FUN_14029f810 (movement order creation)
                                   └─ FUN_140388b60 (GameObjectClass ctor)
                                        └─ FUN_1403989a0 (move order init)
                                             └─ FUN_1403a4820 (target acq)
                                                  └─ FUN_1403729f0 (dispatcher)
                                                       └─ FUN_140375380 (target slot update)
                                                            └─ FUN_14025ec10 ← CONFIRMED BOTTLENECK
                                                                 slow path: 118-iter loop
                                                                 calling FUN_140141f70 (sync asset load)
```

Note: `FUN_14029f810` is also called by callers other than `FUN_1403825b0`
(b388b60 count ≈ 2× b29f810 count); the stall is systemic to all movement
order creation, not just the b3825b0 path-following update.

### Phase 5 Step 24 — Root cause confirmed: 119 uncached MEG archive probes (2026-04-27)

Sub-callee hooks inside `FUN_14025ec10` (774 bytes):
- `b142f80` (2 sites patched) — in-memory asset registration check
- `b141f70` (1 site patched) — synchronous MEG archive probe

#### Data — clean single-call stall windows (n(b25ec10)=1):

| Window | b25ec10 max | b142f80 n | b142f80 max | b141f70 n | b141f70 avg | b141f70 max |
|---|---|---|---|---|---|---|
| W1 | 1002.58ms | 119 | 0.01ms | 119 | 8.42ms | 9.02ms |
| W2 | 1060.30ms | 119 | 0.01ms | 119 | 8.90ms | 10.07ms |
| W3 | 1065.52ms | 119 | 0.01ms | 119 | 8.95ms | 9.68ms |
| W4 | 1078.84ms | 119 | 0.00ms | 119 | 9.02ms | 9.57ms |

**119 × ~8.9ms avg = ~1,059ms** — accounts for 100% of the stall.

**`FUN_140142f80` eliminated** (max 0.01ms — pure in-memory scan, instant).
**`FUN_140141f70` confirmed** as the per-probe bottleneck.

#### Loop mechanics (confirmed by call counts)

The slow path inside `FUN_14025ec10` runs a two-level nested loop:

```
outer loop: i = 0 .. 0x76 (119 iterations) — variant name slots in global table
  inner loop: j = 0, 1, 2, ... — animation frame indices
    b142f80(db, path_i_j.ALA)  → 0 (not registered in asset db)
    b141f70(db, path_i_j.ALA)  → MEG archive probe (~9ms)
      if found: register asset, add to result, cVar1 = '\x01' → inner continues
      if not found: cVar1 = '\0' → inner exits (next outer iteration)
```

On first encounter of a ship type: **every variant slot fails** — b141f70 probes the MEG
archive for each of the 119 variant names and finds nothing. The inner loop exits after
one iteration per outer slot. Result: **119 MEG probes × ~9ms = ~1,060ms per call**.

The second `b142f80` call (at offset +0x26f, after a successful b141f70 load) fires
**0 times** in single-call stall windows — confirming b141f70 always returns failure.
Nothing is cached, nothing is added to the result set.

#### Root cause

`FUN_14025ec10` has **no result cache**. Every call that hits the slow path (animation
variant sentinel in vtable walk) re-probes all 119 MEG variant slots from scratch.
`FUN_140141f70` performs a **synchronous linear scan** of the MEG archive index
(`FUN_1402136f0`) for each probe — ~9ms per probe on Wine/HDD. On bare-metal Windows
this would be faster but still O(N) in archive size.

The stall fires once per movement order for each ship type whose animation variant table
has not been probed in the current session. After the first probe run, subsequent calls
for the SAME ship type still go through all 119 probes (b142f80 returning 0 each time
because the assets weren't found) — the function never writes a "not found" cache entry.

#### Phase 5 Step 35 — verified: fscache does NOT cover Source B (2026-05-29)

Checked whether the Step-35 `fscache` (CreateFile negative directory cache, which cured the
AI/Lua first-encounter `NtCreateFile` stall) also mitigates Source B. It does **not**.
`Phase5MegProbeMechanism.java` decompiled the entire `FUN_14025ec10`→`b142f80`/`b141f70`
call tree (173 functions): there is **no `CreateFile`/`ReadFile`/open primitive anywhere** —
`b141f70` is an **in-memory linear scan of the MEG archive index** (`FUN_1402136f0`), ~9ms ×
119 probes. (The lone `SetFilePointer`/`WriteFile` is a side branch — the NF-cache file
persistence.) Because fscache hooks `CreateFileA/W` and Source B issues no such calls, the
two stalls are unrelated despite both probing `.ALA` names. The `.ALA` opens fscache catches
come from a *different* (Lua-pump asset) path, not the movement-order path.

Consequence: Source B is **compute** (O(N) index scan × 119), not file I/O and not pathfinding —
so it is **not a threading target** either. The correct fix is algorithmic: a result cache at
`b25ec10` (below) and/or an O(1) hashed MEG-index lookup, in the same spirit as fscache but at
the in-memory index layer.

#### Fix options

**Fix A — Memoize at b25ec10 level (implementable in hook DLL now):**
In `b25ec10_hook`, maintain a string-keyed hash map of `path → result`. On first call,
let `g_b25ec10_orig` run (pay the ~1s cost once), store the result. On subsequent calls
with the same path, return the cached result immediately. Cost: one 1s stall per unique
ship type per session; all subsequent calls are sub-microsecond.

**Fix B — Pre-warm at scenario load:**
Before combat begins, call `FUN_14025ec10` for every ship type in the scenario.
Amortizes the cost to the loading screen. Requires knowing which ship types are present.

**Fix C — Optimize `FUN_140141f70` / `FUN_1402136f0`:**
Replace the linear MEG archive scan with a hash-indexed lookup. Would eliminate the
~9ms per-probe cost. Complex — requires modifying the archive subsystem.

**Fix D — Write a "not found" sentinel per path:**
On failure in the outer loop, record `(path → empty)` so the next call short-circuits
the 119-probe scan immediately. Simplest behavioral fix inside the game engine.

**Recommended immediate action:** Implement Fix A in `b25ec10_hook` — it is the only
option implementable entirely within the hook DLL with no game binary modification.
A simple open-addressing hash table keyed on `param_1` (path pointer contents) would
reduce all repeat stalls to 0ms.

### Fix A implemented and validated — Phase 5 Step 24 (2026-04-27)

Implemented in `b141f70_hook` (winmm_proxy.c): djb2-hashed open-addressing table
(`NF_SLOTS=65536`). On `r==0` from the real MEG probe, insert the path hash. On
future calls with the same path hash, return 0 immediately (skip the ~9ms probe).
Cache persists for the lifetime of the process (not reset per 300-frame window).

**Validation data:**

| Window | b25ec10 max | nf_cache hits | nf_cache miss | Notes |
|---|---|---|---|---|
| W1 (prime) | 1298ms | 1071 | 5831 | First encounter — all ship types primed |
| W2 | 1093ms | 119 | 238 | 1 cached type + 2 new types primed |
| W3 | 0.62ms | 0 | 0 | Fast path (no slow-path sentinel hit) |
| W4 | 1058ms | 238 | 238 | 2 cached + 2 new |
| W5 | 3.80ms | 0 | 0 | Fast path |
| **W6** | **0.15ms** | **119** | **0** | **All hits — stall eliminated** |

**W6: b25ec10 max = 0.15ms vs pre-fix ~1,060ms — 7,000× speedup.**

### Phase 5 Step 35 — MEG index repair + b25ec10 result cache (2026-05-29)

Two follow-ups while investigating fast-forward lag:

1. **MEG index (Fix E) was silently broken.** `build_meg_index()` located `Data\*.meg` via
   `GetCurrentDirectoryA`, but it runs at DLL init *before* the game `chdir`s to `corruption\`,
   so it indexed **0 files** (`meg_idx files=0 ready=0`) and `b2136f0_hook` never skipped.
   Fixed: derive the game dir from `GetModuleFileNameA(NULL)` (exe path, timing-independent) +
   recurse the full `Data\` tree and every `Mods\*\Data` tree for completeness. Now indexes
   **43 .meg / 36,941 filenames**. Note: at runtime the skip is largely *shadowed* by the
   nf_cache (which short-circuits at `b141f70` before `b2136f0` is reached), so `skip=0` in
   practice — the repair is correct but mostly helps the cold first-encounter. Kill switch
   `EAW_NO_MEGSKIP=1`.

2. **b25ec10 result cache.** nf_cache made each of the 119 probes fast, but the original still
   ran the 119-iteration filename-building loop (`_splitpath`/`_makepath`/`sprintf`) every
   slow-path call (~9.6k `b141f70` calls/window). A string-keyed `(path|filter)→result` cache
   in `b25ec10_hook` skips the whole loop on repeat: `b141f70` 9,681→0, `b25ec10` max 365→~0ms.
   Correctness: full-string `strcmp` (no hash-collision → wrong object); flush on asset-DB
   pointer (`DAT_140a62700`) change (battle reload). Models/audio verified intact. Kill switch
   `EAW_NO_B25CACHE=1`.

**Key finding (verify-don't-assume, again):** neither fix removes the **fast-forward
choppiness** — that is NOT asset probing. Profiling shows it's the movement/AI service tick
volume: `gsvc(28d400)` avg ~21ms (max 44–77ms), dominated by `b387400` (path-following) called
**121k–161k times per window** (~537 calls/frame). At fast-forward the sim runs many ticks per
render frame, so the ~21ms tick compounds. Source B's asset stall is fixed; fast-forward
smoothness is a separate movement-tick-volume problem — next investigation target.

### Phase 5 Step 35 — sim-tick scheduler mapped (lever 3 scoping, 2026-05-29)

Traced the full sim-tick scheduling (WinMain `FUN_14005d990` → gsvc):
- **Sim tick = `DAT_140b0a320`** — a frame counter incremented +1 per loop iteration by
  `FUN_14027c360` (which also does the once-per-1000ms FPS calc). Read at `0x5f20e` into EBX and
  passed to `gsvc` as `param_2` (`MOV EDX,EBX` @`0x5f2c8`), then forwarded to the mode service
  `vtable[0xb0]` and down to per-entity `FUN_1403a6b80(entity, tick)`, which diffs it against
  `entity+0x60` for delta-ticks. **No game-speed multiplier in the tick** (the `IMUL` at
  `0x5f227` is a `%100` autosave check).
- **`DAT_140b0a340`** = ticks-per-second constant (read in ~300 functions; `game_seconds =
  frame_counter / DAT_140b0a340`).
- **`DAT_1409cf314`** = fast-forward / run-unlimited flag. Gates the loop idle path
  (`if (DAT_1409cf314==0) WaitMessage()`, line 831); when set, the loop drops the idle wait and
  runs flat-out, so the frame counter (= sim tick) advances as fast as the CPU sustains.

**Mechanism of fast-forward choppiness:** `b387400` rate-limits per component via a countdown
(`+0x58 -= delta`, early-return until exhausted). Faster tick advance → countdowns exhaust more
often → more components do full path-following per gsvc → the ~21ms tick → frame drops. So
fast-forward is **already CPU-bound flat-out**; the choppiness is inherent, not a scheduler
mis-pacing.

**Lever 3 verdict:** A clean cap point exists (inject a minimum frame interval at the
`DAT_1409cf314`-gated pacing, lines 831/907), but it is a **tradeoff, not a fix** — it would make
fast-forward *slower but smoother* rather than faster. Since fast-forward already runs maximally,
this is a subjective gameplay-feel change. Not a clear win; recommend leaving fast-forward as-is
unless a capped/smooth fast-forward is explicitly desired.

### Per-tick cost-profile audit — NO ACTIONABLE FAT (2026-05-29)

Live measurement of a big-battle fast-forward (winmm_proxy.c sub-path timing profiler, dumps to
`eaw-mt.log` every PROFILE_WINDOW=300 frames). `gsvc` = **13–28 ms/tick** (max spikes ~100ms).
Per-tick call distribution across the movement hot path:

| Function | calls/tick | role |
|---|---|---|
| `b87010` (387010) | ~1746 | per-component entry |
| `b387400` (387400) | ~1701 | path-follow update |
| `b384850` (384850) | ~1576 | "current path still valid?" check |
| `b385190` (385190) | ~1373, **capped=0** | opp-target scan predicate |
| `b3825b0` (3825b0) | ~859 | path-to-target (deep path) |
| `b385e70` (385e70) | ~836 | formation position |
| `b29f810` (29f810) | ~5–31 | actual A*/nav submission |
| `b381ff0` (381ff0) | **0** | cosmetic bone transform |

**Both candidate hotspots confirmed innocent:**
- **Opp-target scan** — `b385190_capped = 0` in every window. The scan never hits its budget cap
  → Fix B1's interval throttle keeps it from firing most ticks; when it fires, random-start /
  first-match resolves fast. Not the cost.
- **Real pathfinding** (`b29f810`) — ~0.02ms/call × ~30/tick ≈ **0.5 ms/tick**. Already throttled.

**The funnel rules out redundant path recompute:** `1701 path-follows → 859 path-validity checks
→ only 31 actual nav submissions/tick`. `b3825b0` runs 859×/tick but 96% exit cheaply ("path
still good"); only ~31 reach the expensive `b29f810`. The engine already early-outs — there is no
cacheable redundancy here (unlike the b25ec10 asset-lookup case).

**Verdict:** the 13–28ms is **distributed per-component volume** (~1700 components/tick = N_entities
× subsystems), not a hotspot or wasteful recompute. Empirically confirms the static read. The two
expensive ops (scan, nav) are already gated to a handful of calls. The cosmetic-transform
decimation idea is **moot** (`b381ff0` ran 0×). Only levers remain: lever-3 cap (smoothness
tradeoff) or parallelism (ruled out — see threading_model.md).

Caveat: profiler timer = `timeGetTime` (~1–16ms granularity under Wine), too coarse to resolve the
exact ms split among microsecond-scale volume functions (their `avg` reads 0.00). The two
*actionable* forms of fat (cacheable hotspot; redundant recompute) are ruled out via exact
call-counts. Residual diffuse micro-inefficiency inside the binary functions is not hook-addressable
(would require rewriting function internals), so it does not change the verdict.

Behavior after priming:
- Each unique ship type pays the 119-probe cost exactly once (~1,060ms, first encounter only)
- All subsequent movement orders for known ship types: 119 cache hits, 0ms MEG I/O
- After all ship types in a battle are primed, stalls cease entirely for that session

Remaining stall source: first-encounter priming for new ship types. Eliminable with
Fix B (pre-warm at scenario load) but requires enumerating ship types before combat.

### Fix E attempted and abandoned — Phase 5 Step 25 (2026-04-28)

**Hypothesis:** Pre-build a hash index of all MEG archive filenames at DLL load time.
In `b2136f0_hook`, skip the ~9ms vtable+0x18 call for paths not present in the index,
returning 0 immediately. This would eliminate the first-encounter priming cost entirely.

**Implementation:** Scanned base game `Data/*.meg` and all workshop MEG files under
`steamapps/workshop/content/32470/<item>/Data/**/*.meg` (37 files, ~29 700 unique filenames)
using djb2 with backslash→slash / lowercase→uppercase normalization.

**Result: crashes every run.** Investigation via targeted false-negative logging revealed
that `FUN_1402136f0`'s vtable+0x18 method is **not a pure MEG archive searcher**. It
performs MEG lookup AND filesystem fallback: if the file is absent from all MEG archives
it still searches the loose-file tree and returns 1 (found) when it exists on disk.

Workshop mod assets (`i_EOTH_Govt_Menu.alo`, `NR_Lusankya.ALO`,
`endurance-class_fleet_carrier.ALO`, etc.) are shipped as loose files rather than being
packed into MEG archives. For those paths:
- MEG scan: ~9ms (full archive scan, not found)
- Filesystem fallback: ~0.4ms (stat → found)
- b2136f0 returns 1 (not 0)

Skipping b2136f0 entirely (returning 0) for `meg_has=0` paths causes the game to fail
to locate these loose-file assets → crash after mode transition.

**Fix E is not viable without hooking the vtable+0x18 method itself** to short-circuit
only the MEG portion of the scan while preserving the filesystem fallback. That
approach requires identifying and hooking a deeper vtable call with unknown arity/ABI —
out of scope for the current phase.

**Current state:** `b2136f0_hook` is a pure call-through with timing statistics.
Fix A (b141f70 NF cache) remains the active optimization — it eliminates all repeat
stalls after the first encounter per ship type. The once-per-ship-type priming cost
(~1 060ms) remains.

---

## Phase 5 Step 26 — Pump_Threads Time Budget (Fix B3) — 2026-04-28/29

### Symptom
W1 profiling showed pumpe max ~14 000ms with skip=0. The single slow entity's
Lua AI initialization stall blocked all 7 subsequent entities (their pumpe calls
were enqueued but hadn't run). This caused a 14s visible freeze at battle start.

### Fix
Per-gsvc Lua AI budget: `PUMPE_BUDGET_MS = 33.0` ms. `g_pumpe_frame_used_ms` is
reset to 0 at the start of each `game_service_hook` call. Each `pumpe_hook` call
increments the accumulator; once exhausted, subsequent pumpe calls return early
(`InterlockedIncrement(&g_pumpe_skip)`).

**Validation (from log):**
- W1: pumpe max=~14 000ms, skip=7 — the 7 entities after the slow one are skipped,
  preventing cascading stalls. The first entity (slow one) still runs because the
  budget is 0.0ms at its entry.
- W2+: pumpe max=2–9ms, skip=0 — steady-state is stall-free.

**Limitation:** The first entity's 14s run is unaffected. The budget only prevents
new pumpe calls from starting once the cap is hit; it cannot interrupt a running
`lua_resume` call.

---

## Phase 5 Step 27 — Lua AI 14s Stall Root Cause Investigation — 2026-04-29

### Stall profile
- Pattern: W1 pumpe max ~14 000ms, skip=7. Deterministic, once per battle process.
- Trigger: Lua AI initialization for the first entity in the thread table.

### Pump_Threads coroutine lifecycle (FUN_140247a90, RVA 0x247a90)

The outer loop iterates all slots in the entity's thread table (`param_1 + 0x40`,
64-byte slots). For each non-null slot:

```
iVar3 = thunk_FUN_1407bc310(coroutine_state, ...);  // lua_resume — runs to completion
if (iVar3 == 0) {               // success
    FUN_140248d10(…);           // read results from LuaThreadTable global
    *puVar9 = 0;                // CLEAR coroutine slot
} else {                        // error
    FUN_140246940();            // log error ("LuaScriptClass::ERROR -- %s")
    FUN_140248d10(…);           // read results (finds nothing — error string on stack)
    *puVar9 = 0;                // CLEAR coroutine slot — same path
}
```

**Critical:** both success and error paths zero the coroutine slot. This is a
**run-once-discard** pattern. Each coroutine is resumed exactly once, runs to
completion (or errors), and is then discarded. There is no re-enqueue, no retry.

Coroutines are created by `FUN_140244740` (RVA 0x244740) via `lua_newthread`
(`FUN_1407b9190`) on the entity's own root `lua_State` (`entity + 0x58`).
Each entity has an independent Lua universe.

### Why lua_sethook + lua_error is not viable

If the debug hook fires `lua_error` (the only available abort in Lua 5.1):
- `lua_resume` returns `LUA_ERRRUN` (non-zero)
- The `else` branch runs: logs error, clears slot
- **The initialization coroutine is permanently dead for this battle**
- Entity AI is partially initialized → incorrect behavior for the entire battle

`lua_yield` from a debug hook in Lua 5.1 raises "attempt to yield across C-call
boundary" when `nCcalls > 0` (always the case during the VM execution path from
`lua_resume`). So there is no way to pause-and-resume a running coroutine from
a debug hook in Lua 5.1.

### Game's existing lua_sethook usage (FUN_1402531f0, RVA 0x2531f0)

The game installs `FUN_1402531f0` as a debug hook with mask=4 (`LUA_MASKLINE` —
fires on every Lua source line transition) on all entity states and their coroutines.
This is EaW's **in-game Lua script debugger**:
- When no debugger is attached (`FUN_140245790()` returns 0): hook exits immediately
- When a breakpoint condition is met: sets `DAT_140b09e71 = 1`, enters a Windows
  message pump loop (`PeekMessageA / TranslateMessage / DispatchMessageA`) until the
  flag is cleared externally

This mechanism cannot be repurposed for frame-level time-slicing because:
1. While paused, only the Win32 message loop runs — game logic does not advance
2. The "resume" requires an external trigger from the debugger UI

### Entity Lua state isolation

Evidence that entities have independent lua_States (safe for potential threading):
- Each entity object has its own `lua_State*` at `entity + 0x58`
- `FUN_140244740` creates child coroutines via `lua_newthread` on that entity's state
- `FUN_140251360` ("pause all") iterates the global entity list and calls
  `lua_sethook(entity->lua_state, …)` on each entity separately — no shared state

Cross-entity Lua communication has not been confirmed or ruled out.

### Remaining approaches for the 14s stall

| Approach | Assessment |
|---|---|
| lua_sethook + lua_error | Not viable — kills AI initialization permanently |
| lua_sethook + lua_yield | Not viable — forbidden across C-call boundary in Lua 5.1 |
| Game's debugger pause | Not viable — message-pump-only loop, no frame advance |
| Worker thread offload | Viable: entities have independent lua_States. Risk: Lua-callable C APIs may access shared game state. Requires thread-safety audit. |
| Pre-warm during loading | Viable: if coroutines are enqueued before the battle is visible, the 14s moves to the loading screen. Requires identifying when `FUN_140244740` first runs relative to the loading→battle transition. |

**Current state as of Step 27:** 14s stall is a known limitation. The pumpe_budget fix (Step 26)
prevents cascading stalls for all subsequent entities. Pre-warm during loading is the
recommended next investigation path — lower risk than worker threads.

## Phase 5 Step 28 — Pump_Threads Background Thread Offload — 2026-04-30

`pumpe_hook` (hooks all calls to `FUN_140247a90` within `FUN_1403a6b80`) was extended to
dispatch to a background thread when `g_space_mode_seen >= 3` (confirmed space battle).
The BG thread calls `g_pumpe_orig(lua_state)` and clears `g_pumpe_bg_running` when done.
The main thread returns immediately — the 14s stall no longer blocks the frame loop.

**Limitation / regression (Step 30):** EaW's Lua C closures access shared game state
(entity lists, resource counts, vtable objects). Running `lua_resume` on a BG thread
while the main thread reads the same state causes main-thread vtable corruption → crash
(`av_read` at heap address, confirmed on main-menu battle scene). BG dispatch reverted
in Step 30; replaced by loading-screen prewarm (Step 29).

## Phase 5 Step 29 — Loading Screen Pre-warm — 2026-05-04

### Investigation: does coroutine creation happen during the loading screen?

**Call chain traced (8 Ghidra scripts):**

```
WinMain (rva=0x5d990)
  callsite 0x5e627: FUN_140052d10(...)             ← level-load, PRE-MAIN-LOOP
    tag 0x3e9: FUN_14028b080(&DAT_140b153e0, ...)  ← GameModeManager deserializer
      *(param_1 + 0x38) = mode_ptr                 ← SETS DAT_140b15418
    FUN_1402a8730(DAT_140b15418[3])                ← entity/AI manager init
      for each node in linked list (ai_manager+0x48, node+8 next):
        FUN_140394510(entity)                      ← per-entity init
          for each service (entity+0x278, count at +0x288):
            (*vtable[+0x68])(service, entity)      ← dispatches FUN_1406cacd0
              dirty flag set → FUN_1406c8710
                → FUN_1406c8970
                  → FUN_1406bad60                 ← CREATE coroutines
  return → WinMain main game loop starts
```

**Confirmed:** `DAT_140b15418` (battle context pointer) is set, and entity AI
coroutines are **created** inside `FUN_140052d10` before it returns — before the first
battle frame. The main game loop begins ~4 lines after `FUN_140052d10` returns in WinMain.

### Key structures

| Symbol | Address / RVA | Role |
|---|---|---|
| `DAT_140b15418` | RVA 0xb15418 | GameModeManager+0x38 — current battle mode pointer |
| `FUN_140052d10` | RVA 0x52d10 | Level-load function (size=3088) |
| `FUN_14028b080` | RVA 0x28b080 | GameModeManager deserializer — writes `*(param_1+0x38)` |
| `FUN_1402a8730` | RVA 0x2a8730 | Entity/AI manager init — iterates linked list |
| `FUN_140394510` | RVA 0x394510 | Per-entity service dispatch — calls vtable+0x68 per service |
| `FUN_1406cacd0` | RVA 0x6cacd0 | Entity AI service — creates coroutines when dirty flag set |
| AI manager ptr  | `*(DAT_140b15418 + 0x18)` = `DAT_140b15418[3]` | Top of entity list |
| Entity list     | `ai_manager+0x40` sentinel, `ai_manager+0x48` head, `node+0x8` next | Linked list |
| Entity ptr      | `*(node + 0x18) - 0x18` | Recover entity struct base |
| Lua state arg   | `*(entity + 0x2d8)` = `entity[0x5b]` | Argument to `FUN_140247a90` |

The last two rows were confirmed from `phase5_3a6b80.c` line 304:
```c
FUN_140247a90(param_1[0x5b]);   /* param_1 = entity ptr, [0x5b] = +0x2d8 */
```

### Fix implemented — `install_prewarm_hook()`

Hooks the `E8 CALL` to `FUN_140052d10` in WinMain (scan within 4096 bytes of
`WINMAIN_RVA=0x5d990`). After the original returns, `prewarm_pump_all_entities()`
enumerates every entity in the AI manager linked list and calls `g_pumpe_orig(lua_state)`
for each entity that has a non-null Lua state. This fires `lua_resume` synchronously
during the loading screen — the 21s Lua init cost runs before the first battle frame.

Expected log output:
```
[eaw-mt] prewarm: FUN_140052d10 callsite in WinMain patched
[eaw-mt] PREWARM: <N> entities walked, <M> pumped, <T>ms total
```

**Safety:** At the hook point, the main thread is not in service dispatch. All entity
Lua states are independent (one per entity). No render thread or BG thread is running.
The pre-warm is synchronous and completes before WinMain's main loop begins.

## Phase 5 Step 30 — Revert BG Thread Dispatch (Use-After-Free Fix) — 2026-05-04

BG thread dispatch (Step 28) caused a main-thread crash via vtable corruption.

**Root cause:** EaW's Lua C closures (called from inside `lua_resume`) access shared
game state — entity lists, resource counts, object vtables. Running them on a BG thread
while the main thread reads the same state is a data race. Confirmed: `rip=0x39424190`
(`av_read` at heap address, main thread, tid=388) after BG dispatches `bg=9`.

The `g_space_mode_seen >= 3` guard was intended to prevent false triggers on the galactic
map, but the main-menu animated space battle also increments the counter, enabling BG
dispatch for a scene where ships regularly die and free entity memory mid-resume.

**Fix:** `pumpe_hook` always runs `lua_resume` synchronously (BG dispatch block removed).
Per-gsvc budget cap (`PUMPE_BUDGET_MS = 33ms`) is retained to bound frame cost.

**Architecture going forward:** The prewarm hook (Step 29) absorbs the first-call 14s
Lua-init cost during the loading screen. Post-prewarm, each `lua_resume` takes ~1ms;
synchronous dispatch is sufficient. BG dispatch would require a full audit of every EaW
Lua C binding to confirm no shared-state writes before it can safely be re-enabled.
