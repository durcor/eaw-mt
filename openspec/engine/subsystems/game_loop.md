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
                              └─ FUN_14029f810 (nav order + pathfinding) [Source B — 1,094ms]
```

TODO: Decompile `FUN_14029f810` (RVA `0x29f810`) to identify what pathfinding
operation causes the 1,094ms stall and design a targeted fix.
