# a2 Engine-Integration Scope — wiring the parallel tick into the live engine (2026-06-06)

**Status: build scope.** The §5 reassessment gate resolved (Rule-6 sign-off, `firing_body_lift_scope.md`
§5) to the **serial-gate fallback**: keep the per-firer fire DECISION on the ordered/serial path,
parallelize the cheap per-tick mass. a2 is the build that realizes this against the live engine — the
successor to a1 (which proved the control-flow-takeover + buffer+drain plumbing for SFX). This doc scopes
it, and **leads with the two honest questions a2 must answer before any takeover code is written**: is the
payoff worth it (Amdahl), and is the determinism *retrofit* (not stock-bit-identical) acceptable.

---

## 0. What a2 IS, and three things it is NOT

**IS:** a two-phase deterministic tick — a parallel sweep of the **cheap-mass self-bodies** (movement,
transform, sensor/fog — all already lifted + in-game-validated) across N shards reading a `FrozenSnapshot`,
a barrier, then a **serial fire pass** (the binary fire-control body, creates immediate), then the
canonical-order drain of the buffered Class-1/Class-3 ops.

**NOT (1) — not bit-identical to stock.** Stock interleaves movement+firing per object in visitation order,
so a unit's firing reads each target at whatever update-state it had reached at that rank (a mix of pre/
post-move). The two-phase split makes every cross-object read come from the tick-start `FrozenSnapshot`
(+ RNG from per-entity substreams, already a retrofit per `sim_rng.h`). That is a **determinism RETROFIT
with new read semantics** — internally deterministic + lockstep-safe, but it CHANGES gameplay vs stock by
a sub-tick of target-position latency. DIFFTRACE-vs-stock **will** diverge by design; the validation is
replay-determinism + lockstep (contract §5.2), **not** stock-equivalence. (§3 assesses the delta.)

**NOT (2) — it does NOT use the firing geometry lifts.** The intercept (`399e20`) and spread (`381dc0`)
lifts live *inside* the fire-control body, which stays **serial** (binary). So a2's parallel sweep does
not call them. Those lifts were **diagnostic** (they proved the geometry/gate asymmetry that decided the
§5 gate) and **future-proofing** (a full-parallel path could use them) — be honest that a2's speedup comes
from the cheap-mass bodies, not the firing geometry. The bodies a2 DOES drive in parallel are the
locomotor (DTSPL2/DTSTEER/DTYAW ✅), Pass-C transform (DTDYN ✅), and sensor/fog (DTUAI/DTREVEAL ✅).

**NOT (3) — not "just wire 3 interfaces."** The cheap-mass is currently *inside* the monolithic per-object
body `3a6b80` (spine + locomotor + behaviors + transform + fire-control, all in one call). a2 must drive
the lifted cheap-mass bodies from OUR OWN loop and call only the binary's fire-control serially — i.e.
**assemble the lifted per-object update (minus firing) into a Phase-A driver.** That assembly, not the
`FrozenSnapshot`/`ShardScheduler` glue, is the real work (§5).

---

## 1. The two-phase tick (the architecture)

```
  for gom in (mgr[3], mgr[4]):                       ← fixed order (I4); global id counter depends on it
     ┌───────────────────────────────────────────────────────────────┐
     │ snapshot.acquire(cross-read fields of this GOM, in rank order) │  Interface #2
     ├───────────────────────────────────────────────────────────────┤
     │ PHASE A (∥ across N shards, shard_of(obj+0x50)):              │  Interface #3
     │   per owned object, in visitation-rank order:                 │
     │     begin_entity(rank, gom_index)                             │
     │     LIFTED cheap-mass update — locomotor + Pass-C transform + │  reads snapshot for cross-obj,
     │       sensor/fog + the self-contained IN behaviors            │  writes OWN state live,
     │       (damage/energy/ability/nebula/…)                        │  buffers Class-1 sensor + Class-3 SFX
     ├───────────────────────────────────────────────────────────────┤
     │ barrier (all shards done; registry + sensor grid still frozen)│
     ├───────────────────────────────────────────────────────────────┤
     │ SERIAL fire pass — per object in rank order, call the binary  │  creates IMMEDIATE (no §0 blocker),
     │   fire-control 3a76b0/387400/3825b0 unchanged                 │  gate reads live (serial ⇒ no race)
     ├───────────────────────────────────────────────────────────────┤
     │ drain_parallel(shard buffers, WorldApply)  — Class-1/Class-3  │  BUILT; canonical (gom,rank,seq)
     │ snapshot.release()                                            │
     └───────────────────────────────────────────────────────────────┘
```

The serial fire pass keeps creates immediate, so the §0 inline-consume blocker is sidestepped and no
`SpawnCommand`/gate-lift is needed. Phase A buffers only the genuinely-bufferable classes (Class-1 sensor
grid notifies — observer-sharded; Class-3 SFX — off-lockstep), which the built `drain_parallel` already
handles and a1 already proved in-game.

---

## 2. THE FIRST QUESTION — Amdahl: is the payoff worth it? (measure BEFORE building)

The fire-control stays serial, and the perf history says fire-control is **expensive**: `387400`
(path-following + opportunity-target scan `385190`) was *the* per-tick stall (Fix B1/B2 capped its search),
and `t2be640` (the GOM iterator) was the headline stall the whole perf arc chased. If fire-control is, say,
60% of tick cost and stays serial, Amdahl caps the speedup at **~1.6×** no matter how many cores service
the cheap-mass — possibly not worth the integration risk + the gameplay delta (§3).

**⇒ a2.0 (mandatory first increment) = MEASURE the split.** Instrument the existing `EAW_PROFILE` timers
(or a lean oracle-build timer) to attribute per-tick wall-time to: (a) cheap-mass (locomotor + transform +
sensor + self behaviors) vs (b) fire-control (`3a76b0`/`387400`/`3825b0`) vs (c) the rest. Compute the
parallelizable fraction `p` and the Amdahl ceiling `1/((1-p)+p/N)`. **Gate: if `p` is small (fire-control
dominates), STOP and reconsider** — either (i) accept a modest win, (ii) revisit the §5 gate to also lift
the opportunity-scan (the expensive read-only part of fire-control — note it has the b1 fan-out), or (iii)
shelve a2. This measurement is cheap, decisive, and must precede the takeover.

> **✅ a2.0 MEASURED 2026-06-06 (commit f58da7c, `EAW_A2MEASURE=1`, evidence `eaw-mt.log.a2measure`).**
> Lean QPC entry-detour timers on `3a6b80` (per-object body) vs `3a76b0` (serial fire-control subtree),
> DT hooks skipped so nothing confounds the timing. **Result (multi-loop menu-demo battle):**
> - **`p ≈ 0.65`** cumulative (cheap-mass = ~65 % of per-object body time; recent-interval ~0.64), swinging
>   **~0.52 in peak-combat bursts** to ~0.70 in lulls. Only ~8 of ~520 objects/tick run fire-control, but
>   each fire-call is **~28× an average cheap body**, so the few firing units carry the ~35 % serial cost
>   (matches the `387400` opportunity-scan stall history).
> - **Amdahl ceiling at p=0.65:** 2-core **1.48×**, 4-core **1.95×**, 8-core **2.32×**, ∞ **2.86×**. At
>   p=0.52 (peak combat): 4-core 1.6×, 8-core 1.8×, ∞ 2.1×. (Body-only; total-tick adds serial
>   iteration/drain → real-world lower.)
> - **The win is WEAKEST in the biggest battles** (serial fire-control grows with combat) — exactly where
>   speedup is most wanted. Getting past ~2× would need the deferred **b4 gate lift** (the opportunity-scan
>   is the expensive read-only part, but it has the b1 fan-out).
>
> **GATE VERDICT: the parallel fraction is REAL (~65 %) but the ceiling is MODEST (~2× at 4 cores), not the
> 3–4× the program hoped.** Whether ~1.6–2× justifies the a2 build (engine integration + the retrofit
> gameplay-delta §3 + the takeover risk) is a judgment call — **surfaced to the user.** A ~2× sim-tick
> speedup is a genuine win for a CPU-bound game (TR big-battle fast-forward is CPU-bound), but it is not
> dramatic and degrades under load. Options if NOT worth it: revisit b4 (parallelize the opportunity-scan
> despite the fan-out) for a higher ceiling, or shelve a2 and bank the lifts.
>
> **➡ b4 REVISITED 2026-06-06 (`firing_body_lift_scope.md` §8):** the opportunity-scan IS the b1 fan-out,
> but a **parallel FIRE PASS** (a read-only phase *after* movement, with WRITES deferred) avoids the
> intractable predicate-read lift — the gate predicates read live-but-STABLE state, needing only write-
> deferral (b3) + scratch localization, not a snapshot. That raises the ceiling to **~3.5–6×**. Gated on
> real hazards (chiefly the spatial-query's shared result buffer + a fire-path write-audit). ⇒ if a higher
> ceiling is wanted, the next step is the **fire-path write-audit + spatial-query thread-safety assessment**
> (cheap, decisive, a2.0-spirit) BEFORE committing to b3 + the parallel-fire takeover.

### ✅ CEILING-RAISE REASSESSMENT (2026-06-12) — the higher ceiling is GO *and* most of its machinery now exists

The user asked to raise the ceiling before committing to a2.2. The assessment is **already complete** and is
**GO** (`firing_body_lift_scope.md` §8.5–8.8): the write-audit found **no un-deferrable live cross-entity
write** (every cross-object write is in the deferrable Class-2/2b/3 channels); the spatial-query is
**MANAGEABLE** (its grid is frozen by the tick-start rebuild `2be640`/`20ed70` → concurrent reads safe for
free; the only hazard is the intrusive result-list `+0x18`/`index+8`, contained by a **short-lock + copy-out**
to a thread-local buffer, with the per-candidate eval still parallel — consumption capped at 25 by Fix B2);
both query vfuncs (`vfunc_2` `0x395ac0`, score `vfunc_35` `0x566ba0`) are **read-only**. **Parallel-fire-pass
ceiling: ~3.5–6×.**

**Decisive update since June:** the **B3.7–B3.9 firing takeover is COMPLETE** (§8.46–8.73). `sim::fire_control_
decide` (`sim/fire_control.{h,cpp}` + `firing_aimpoint`/`intercept`/`spawn`) now reads marshalled firer/target
state, decides the geometry, and **defers every cross-entity write**: all projectile creates
(ballistic/guided/charge) → `SpawnCommand`; the opportunity-target rebind (stage K, Class-2b) →
`Command`/`listener_edits`; SFX → `SfxCommand` — leaving **only stage-J cooldown** live, which is **own-state
/ Phase-A-safe**. It was driven LIVE in-engine (drive_n≈80k creates + 80k opp-rebinds, zero crashes,
deterministic). **This is precisely the read-mostly + deferred-write fire decision a parallel fire pass
needs — already built and in-engine-validated, just run serially today.**

**⇒ The §5 serial-fire fallback is effectively SUPERSEDED.** That gate was decided (June) *because* the fire
body was an un-lifted 4034 B binary blob; B3.7–B3.9 then lifted it. So a2 should target the **parallel-fire
higher ceiling (~3.5–6×)**, not the ~2× serial-fire fallback — keeping the now-lifted `decide` serial would
waste the lift. Remaining work to realize it (delta over the serial-fire a2):
1. Feed `fire_control_decide`'s cross-object inputs from the `FrozenSnapshot` (target pos/health/team) + the
   frozen team-grid (spatial query) — both already tick-start-stable.
2. ~~The spatial-query **short-lock + copy-out** (§8.6) — the one piece not yet built.~~ ✅ **DONE
   2026-06-12** (commit a157da6): engine binding of `sim/spatial_query.h` wired into the live opp-scan
   reimpl — `20e780` query + copy candidate pointers to a per-thread Win32-TLS buffer under
   `g_spatial_query_cs`, gate/score/reach eval runs lock-free from the copy. Validated **behavior-neutral**
   (`EAW_PFIRE_SCAN=1` self-validation `wmiss=0/smiss=0`, DTWORLD bit-identical to baseline, zero crashes);
   inert today (serial 1-shard), correct for the N-shard parallel fire pass.
3. Drain the deferred `SpawnCommand`/`Command`/`SfxCommand` in canonical order after the parallel phase
   (`drain_parallel`, already built + host-green).
Then `fire_control_decide` runs **inside Phase-A** (deferred writes) instead of a serial pass.

**Verdict: pursue the parallel-fire a2 (higher ceiling), not the serial-fire ~2× variant — the lift that
makes it possible is done.** a2.1 still holds: the cheap-mass is lifted; the per-object AI-pump +
scheduled-events + DoT still go serial (cheap), but fire-control now joins Phase-A. (a2.0b retrofit-delta
sign-off still required before the takeover.)

---

## 3. THE SECOND QUESTION — is the determinism retrofit's gameplay delta acceptable?

The two-phase reorder + frozen reads change behavior vs stock (NOT (1) above). The delta is bounded:
cross-object reads (target positions for aim, sensor inputs) become tick-start instead of mixed-update —
a uniform **one-sub-tick latency**, ≈ the distance a unit moves in 1/30 s (sub-unit at combat speeds). RNG
substreams already changed which numbers come out (accepted in `sim_rng.h`). For a mod whose GOAL is
MP-deterministic multithreading this is very likely fine, but it must be **stated and signed off**, because
it means a2 can NOT be validated by "matches stock" — only by replay-determinism + lockstep. Assess: does
TR care about frame-exact parity with stock single-thread (it does not — stock isn't even deterministic
across its own RNG-reordering), or only internal lockstep (it does)? Expected verdict: **acceptable**, same
class as the substream retrofit already shipped.

---

## 4. The control-flow takeover point (building on a1)

a1 call-site-patched one SFX emit. a2 takes over the **per-object walk**. Interception candidates:
- **`t2be640`** — the GOM-iterator wrapper already hooked for timing (the tail22 stall). This is the
  natural seam: replace its body with `acquire → ShardScheduler::run(cheap-mass) → barrier → serial fire
  pass → drain → release`. But it must drive the master-list walk itself (I4: `GOM+0xe8` head→sentinel).
- **`a76b0`/`b3a76b0`** — the per-object service already detoured (DTFIRE/DT-DRAIN). a2 could intercept the
  walk driver one level up so it controls iteration.

The takeover replaces the binary's *inline* per-object dispatch with our two-phase driver. Gated behind a
default-OFF kill-switch (`EAW_A2=1`, like `EAW_A1`); default OFF = the binary walk runs untouched. Single
writer of the tick while engaged.

---

## 5. THE REAL WORK — assembling the lifted Phase-A body (Interface #3 body)

The `FrozenSnapshot`/`ShardScheduler`/`drain_parallel` glue is built (host §9-gate-green). The genuine a2
work is the **Phase-A body** = drive the lifted cheap-mass per object:
| Cheap-mass body | Lifted? | In-game oracle |
|---|---|---|
| Locomotor (SimpleSpace cubic-Hermite + Fighter steer/yaw) | ✅ `sim/spline_mover`,`sim/locomotor` | DTSPL2/DTSTEER/DTYAW |
| Pass-C DynamicTransform | ✅ `sim/dynamic_transform` | DTDYN (1.7M/1.7M) |
| Sensor/fog (UnitAI #4, Reveal #10) | ✅ `sim/unit_ai`,`sim/reveal_gate` | DTUAI/DTREVEAL |
| Self behaviors (Damage/Energy/Ability/Nebula/Asteroid) | ✅ `sim/*` | DTDMG/DTNRG/DTABIL/DTNEB/DTAST |
| Entity spine | ✅ `sim/entity_spine` | (host) |

**Open: does the cheap-mass = "everything in `3a6b80` except fire-control", and is that set fully lifted?**
Mostly yes (above), but a2.1 must **enumerate `3a6b80`'s per-object sub-calls** and confirm each is either
(a) a lifted cheap-mass body, (b) fire-control (→ serial pass), or (c) Class-1/3 (→ buffer). Any
un-lifted, non-firing sub-call is a gap that blocks driving the body from our loop (it would have to call
back into the binary mid-Phase-A, which re-introduces live cross-reads). This enumeration is the I1-analogue
for a2 and may surface residual lifts.

The `FrozenSnapshot` payload is the §2.1 / `firing_body_lift_scope.md` §4 cross-read set already pinned:
`object_id`, pos `+0x78/7c/80`, health, target_id, team, visible — populated in rank order at `acquire()`.

---

### ✅ a2.1 RESULT — `3a6b80` per-object sub-call enumeration & classification (2026-06-12)

Decoded `FUN_1403a6b80` (2862 B, `decomp/3a6b80.c`) — the monolithic per-object body. Every per-object
sub-call classified:

| Sub-call (3a6b80 line) | What it is | Class | a2 routing |
|---|---|---|---|
| locomotor field-copy +0x164.. + `557ba0` (40-67) | locomotor pose double-buffer + advance | cheap-mass ✅lifted (`sim/locomotor`) | **Phase-A** |
| `220ed0(disp, listener_list, 0x17)` (76) | listener emit (locomotor-arrived) | Class-2b cross-entity | **buffer** (drain_parallel) |
| `5369e0` (80) | field_0xf0 update; no RNG/create/emit | self-state | **Phase-A** (read-discipline check) |
| `3a9e30` timer-expiry loop (82-131) | scheduled-event/ability fire — **6165 B, emits a signal**, un-lifted | un-lifted Class-2b | **GAP → serial pass** (low-freq: timer-gated) |
| behavior loop `vtable[0x30]` (132-138) | self-behaviors (Damage/Energy/Ability/Nebula/Asteroid/UnitAI/Reveal) | cheap-mass ✅lifted (`sim/*`) | **Phase-A** (the core) |
| `DynamicTransformBehaviorClass::3ac530` (140) | Pass-C transform | cheap-mass ✅lifted (`sim/dynamic_transform`) | **Phase-A** |
| `SelectBehaviorClass::3c2710` (144) | select marker pulse | Class-3 ✅lifted (`sim/select_marker`) | buffer/presentation |
| `266340` + the +0x2a0/2e0/298/2b8 render block incl. pose-eval `12d2c0` (147-261) | scene/anim/alHModel pose-eval | Class-3 presentation | **serial or scratch-lock** (the known Fork-B pose-eval race) |
| `37da80` (265) | field_0x100 update; no RNG/create/emit | self-state | **Phase-A** (read-discipline check) |
| **`247a90` = Pump_Threads (267-346)** | **per-object Lua AI pump** (ServiceRate-gated; writes the Model B C-closure surface; draws global RNG line 326) | un-lifted, writes shared sim state | **GAP → serial pass** |
| hardpoint pre-calc `396070`/`396df0`/`3a8ad0` (361-369) + **`3a76b0` (372)** | fire-energy + the **fire-control subtree** | fire-control | **serial pass** (the planned one) |
| `3727a0`+`3ab890`+RNG (374-386) | DoT/effect roll; **draws global RNG `a13e24` line 382** | Class-2 (RNG) + self-effect | **per-entity substream** (`sim_rng`) + serial-apply |

**Verdict: the Phase-A body is buildable — the dominant cheap-mass (locomotor + self-behavior loop +
DynamicTransform) is fully lifted.** The gaps surfaced are NOT blockers, but they REFINE the architecture:

1. **The serial pass is bigger than just fire-control.** It must also absorb (a) the per-object **Lua AI
   pump** `247a90` — it writes the whole reflected C-closure surface (Phase 6 / Model B), so it cannot run
   in a Phase-A worker; cheap (PUMPPROF: ~0.1 % steady-state) so serializing it is free; and (b) the
   **scheduled-event handler** `3a9e30` (un-lifted, emits a cross-entity signal, timer-gated/low-freq).
   ⇒ **serial pass = {3a9e30 events, AI pump, fire-control, DoT-apply}** — all low-frequency. (The a2.0
   Amdahl split measured `3a6b80` minus `3a76b0` as "cheap-mass"; the AI-pump+events were in that bucket,
   but PUMPPROF shows they're ~0.1 %, so the p≈0.65 ceiling is unaffected — they just move to the serial
   side at negligible cost.)
2. **Two direct global-RNG draws inside `3a6b80`** (line 326 AI-pump LastService seed; line 382 DoT roll),
   both on `&DAT_140a13e24` → the I2 draw-order hazard → **per-entity substreams** (`sim_rng.h`, the retrofit
   already built). Not a blocker; route both through the substream.
3. **The pose-eval presentation race** (`12d2c0`→alHModel, Fork-B) stays — Class-3, handled by serial or a
   scratch-lock, not a Phase-A parallel write.

No sub-call forces an un-liftable binary callback that reads live cross-object state mid-Phase-A: every
non-cheap-mass body is either serial-pass (fire/AI/events/DoT) or buffered (Class-2b/3). **a2.1 clears its
gate — proceed to a2.2 (the 1-shard takeover) after the a2.0b retrofit-delta sign-off.**

## 6. Increment plan

| # | Increment | Validates | Kill-switch / gate |
|---|---|---|---|
| **a2.0** | **Measure the parallel fraction** (§2) — profile cheap-mass vs fire-control vs rest; compute Amdahl ceiling. | the payoff is real | **HARD GATE: proceed only if `p` worthwhile.** No code-change risk. |
| **a2.0b** | **Sign off the retrofit delta** (§3) — confirm stock-divergence is acceptable (lockstep-only goal). | the approach is allowed | user sign-off (Rule 6) |
| **a2.1** | **Enumerate `3a6b80`'s per-object sub-calls** (§5) — classify cheap-mass / fire-control / Class-1-3; find any un-lifted non-firing gap. | the Phase-A body is buildable with no binary callback | static; gaps → residual lifts |
| **a2.2** | **1-shard two-phase driver** — take over the walk (`t2be640`), drive lifted cheap-mass serially (N=1) + binary serial fire pass + drain. `FrozenSnapshot` populated but reads still live (1-shard ⇒ no race) to isolate control-flow from snapshot. | tick takeover + iteration-order + drain hold in-game | `EAW_A2=1` OFF default; stability + replay-determinism (NOT stock) |
| **a2.3** | **Route cross-reads through `FrozenSnapshot`** + assert read-discipline (§5.3). ✅ **a2.2.3 DONE** — the fire-control aim read is flipped onto the snapshot (whole-world freeze-swap) via the a2↔pfire co-arm; `EAW_A2=4` neutral checkpoint + `EAW_A2=5` drive-flip, replay-deterministic. | snapshot consistency | debug assert: no live cross-read |
| **a2.4** | **N-shard** — partition by `shard_of(obj+0x50)`, real worker threads, canonical drain. | N≡1 replay-determinism + lockstep + the measured speedup | per-N kill-switch; contract §5 gate |

a2.2 is the a1-successor (first sim-state control-flow takeover). a2.0 must clear its gate first.

### ✅ a2.2.0 RESULT — the sim-tick CONTROL-FLOW TAKEOVER is live (2026-06-12)

**Our code now drives the GOM tick.** `EAW_A2=1` replaces the binary `FUN_1402be640` (the per-tick
GameObjectManager update, called ×2 from the tail22 body) with `a2_tick_2be640` in `hooks/winmm_proxy.c`
— a **faithful transcription** of all 8 blocks of `decomp/2be640.c`, verified field-by-field against an
`objdump` of the baseline binary (`binaries/original/StarWarsG.exe` @ `0x1402be640..0x1402bec69`). Per
the two Rule-6 sign-offs taken this session: **full walk-driver** (spec-literal — drive the
`master_update_list` head→sentinel walk ourselves, not the lighter `3a6b80` call-site repoint) +
**flip-behind-kill-switch** (validate by DTWORLD identity + crash-free longevity, not a shadow compare).

**a2.2.0 is IDENTITY ONLY:** block 2 (the walk) is driven by OUR loop but still calls the binary
per-object body `3a6b80`; every other block (1: `20ed70` preamble; 3: 2nd master sweep `3fc750`;
4: `second_update_list` `3ac530`; 5: `creation_params` spawn-drain `29f810`; 6: player-roster `2aca60`;
7: mode/bridge FSM `28c720`/`3de570`/…; 8: `DynamicVectorClass` collect/apply `2ad100`/`2b92e0` + RAII +
final flush `2a62d0`) calls its binary helper. Zero intended behavior change — this isolates the
control-flow takeover from the lifted bodies (which enter at a2.2.1+).

**Wiring:** a new `install_a2_hooks()` (gated by `a2_on()`, in the always-installed section) repoints the
2 tail22→`2be640` call sites to our dispatcher — so it works in the **lean `EAW_ORACLE` build** where
DTWORLD is emitted (the profile-only `install_tail22_body_hooks` timing detour is left untouched).
**Mutually exclusive with `EAW_PFIRE`** (both own the `2be640` seam); `a2_on()` refuses to co-arm.
Ground-truth offsets banked in the code header (master_update_list head@+0xf8/sent@+0xf0,
second_update_list +0x140/+0x138, node{next@+8,objptr@+0x18}, creation_params@+0x5f8 cnt@+0x600
stride 0x30, param_1[1]@+0x5f0, mode FSM@+0x618, rosters +0x470/+0x478 & +0x488/+0x490 stride 0x58,
DynVec vftable@imgbase+0x800488, inline-free thunk@0x769c94).

**Validation (oracle build, `just a2=1 difftrace=1 launch-foc-desktop`, menu-demo battle):**
- **DTWORLD identity GATE = PASS.** Battle-1 (the deterministic demo intro, identical initial state from
  launch) is **bit-identical** between `EAW_A2=0` and `EAW_A2=1`, hash AND global-LCG seed:
  `tick=1024 h=ea5f27913390cce2 seed=8f00da8b`, `tick=2048 h=7f7fdaf6ca0fcbaa seed=cdb39e0c` (these also
  match the prior spatial-query baseline → run-to-run reproducible). The matching seed proves the RNG
  draw sequence is preserved, not just object positions. (Battles 2–4 of the attract loop diverge across
  separate launches because the demo sequence is not frame-aligned launch-to-launch — different scenarios,
  not an a2 regression; battle-1 is the controlled comparison, the established project methodology.)
- **Stability:** the driver ran **21,505 ticks / 6.3M object-walk calls, ZERO crashes** (vs the baseline's
  shorter run) — far past the a2.0 measure's reach. Live evidence + the `repointed at 2 tail22 call site(s)`
  line confirm our driver, not the binary, walked every object.

**⇒ a2.2.0 clears its gate.** The riskiest control-flow change in the project (first time our code drives
the sim tick) is in-engine and behavior-neutral. (DTWORLD coverage caveat: it hashes only fire-control
objects; the gate relies on it + crash-free longevity, the same posture as the spatial-query and pfire
validations.)

### ✅ a2.2.1 RESULT — the tick-start FrozenSnapshot is populated (2026-06-12, same commit as a2.2.2-prep)

**`EAW_A2>=2`** adds `a2_snapshot_acquire(gom)` — a pre-pass over the `master_update_list` in **visitation
rank order** (== block-2's order) that captures each object's cross-read fields into a grow-only frozen
buffer (`g_a2_snap`) BEFORE the per-object bodies mutate anything, with `a2_snapshot_release()` at tick end.
This is Interface #2 (`sim/frozen_snapshot.h`) realized on the engine: the eventual N-shard Phase-A will
read OTHER objects only from this tick-start view (no read-after-write race). **a2.2.1 only POPULATES it —
block 2's reads stay LIVE (1-shard ⇒ no race) — so it is behavior-neutral.** Gated at `>=2` so the proven
a2.2.0 takeover (`=1`) stays a clean A/B baseline.

**Confirmed offsets only (Rule 2 — no guessed offsets):** `object_id @ obj+0x50`, `pos @ obj+0x78/+0x7c/
+0x80` (the dominant cross-read for the sensor/fog/aim cheap-mass). `health/target_id/team/visible` stay 0
in the `A2ObjState` POD until **a2.2.2** confirms their offsets by validated use (they are not cleanly
documented today; the lifted sensor bodies abstract them and fire-marshalling uses geometry).

**Validation (`just a2=2 difftrace=1 launch-foc-desktop`):** DTWORLD battle-1 **bit-identical** to the
`EAW_A2=0` baseline (hash + seed: 1024=ea5f27913390cce2/8f00da8b, 2048=7f7fdaf6ca0fcbaa/cdb39e0c) ⇒ the
populate is behavior-neutral; snapshot populated **max_n=1010 objects/tick, overflow=0** (grow-only buffer,
no per-tick alloc at steady state); **zero crashes** over 5121+ ticks. **⇒ a2.2.1 clears its gate.**

### ✅ a2.2.2 RESULT — the FrozenSnapshot is PROVEN a correct read source (2026-06-12)

**Roadmap-reshaping finding (Rule 6 — contradicts the planned "flip cheap-mass cross-reads"):** the lifted
**cheap-mass bodies have NO snapshot-resolvable cross-read.** Locomotor/transform/reveal read only OWN state
(waypoints / own transform / own 2D move-threshold). Sensor/fog (`unit_ai`, `sim/unit_ai.cpp` BLOCK C)
cross-reads only via `visible(owner,obj)` → the **un-lifted binary LOS/faction predicate `FUN_14035f470`**
(reads the other object's live pos for LOS). So the snapshot's real first consumer is **not** the cheap-mass
— it is the lifted `fire_control_decide` **target-pos feed** (`sim/fire_control.h:144` already declares
`target_pos` as "snapshot target world pos"; the marshalling `winmm_proxy.c:5354` currently feeds it LIVE
`tgt+0x78`), which lives in the `EAW_PFIRE` path — **mutually exclusive with `EAW_A2`**. ⇒ the real read-
flip (a2.2.3) requires an **a2↔pfire composition**, OR lifting the sensor LOS predicate `35f470` for the
"true" cheap-mass flip.

**Given that, a2.2.2 = snapshot READ-BACK correctness validation** (user-chosen — prove the populated view
is usable before any flip). `EAW_A2>=3` adds three read-only checks (behavior-neutral) exploiting the
object-granular invariant (object r is untouched until its OWN body runs):
1. **Pre-call invariant** — before each `f3a6b80(r)`, live `{id@+0x50, pos@+0x78}` must == `snap[r]` exactly
   (rank-alignment + capture). 2. **Freeze (anti-alias)** — after the walk, moved objects' live pos has
   diverged from the still-frozen `snap[r]` (proves a real copy, not a live view). 3. **find_by_id** — the
   id→rank resolution path a consumer will use.

**Validation (`just a2=3 difftrace=1`):** DTWORLD battle-1 **bit-identical** to baseline (still behavior-
neutral). Over millions of object-visits: **pre-call `ok=1,733,033 idbad=0 posbad=0`** (invariant holds for
EVERY object), **freeze `moved=1,526,177 still=206,856`** (moved≫0 ⇒ snapshot genuinely frozen while the
world advanced), **find_by_id `ok=9,107 bad=0`**, **zero crashes** over 4097+ ticks. **⇒ the FrozenSnapshot
is a correct, stably rank/id-indexed, genuinely-frozen tick-start read source — a2.2.2 clears its gate.**

### ✅ a2.2.3 RESULT — the first read-flip is LIVE: fire aims at the FrozenSnapshot (2026-06-13)

**The first increment that CONSUMES the snapshot and the first that diverges DTWORLD by design** (the
a2.0b retrofit, signed off). User chose the **spec-literal drive-flip** (make the *actual* fire consume
frozen positions, not just an observe-measure).

**The composition (a2↔pfire), code-grounded.** The two takeovers compose cleanly with **one new wire**.
`EAW_A2` replaces `2be640` and drives the walk, calling binary `3a6b80(obj)` per object; pfire patches the
`a76b0` call-site *inside* `3a6b80`, so under co-arm **the fire defer happens for free** (a2's `3a6b80`
call hits pfire's intercept). The only gap was the flush: pfire's drain lives at the `a62d0` call-site
*inside* `2be640` (which a2 bypasses), so **a2's block 8b now calls `pfire_a62d0_intercept(gom)` instead of
the direct `2a62d0`** — it replays the deferred fires then calls the real `2a62d0` (no double-flush). pfire's
`2be640`-internal repoints are inert under a2. The mutual-exclusion guard is relaxed for `EAW_A2∈{4,5}`,
which **require `EAW_PFIRE>=2`**.

**The drive mechanism — whole-world freeze-swap.** Per-target spoofing is blocked (the ship→target offset
is unconfirmed; Rule 2). Instead, at the block-8b flush `a2_fire_freeze_in` walks the current master list and
for each object still in the tick-start view (resolved by `find_by_id`, robust to mid-tick spawns) **saves
its live pos and overwrites `+0x78/7c/80` with the frozen `snap` value**; the binary fire replays aiming at
the tick-start world; `a2_fire_freeze_out` restores every saved pos by object-pointer **before** the real
`2a62d0`. Uses only the a2.2.2-validated `+0x50`/`+0x78` offsets + rank/id alignment. **Known bounded delta:**
this also shifts the shooter's OWN muzzle-origin read to tick-start (slightly beyond the spec's "cross-object
reads" wording) — same ≤1-sub-tick class, deterministic; a finer target-only flip is offset-gated for later.

**Gate ladder:** `EAW_A2=4` = co-arm, **flip OFF** (behavior-neutral checkpoint); `EAW_A2=5` = co-arm +
**drive-flip ON**.

**Validation (oracle build, `just a2=N pfire=2 difftrace=1`, demo battle-1):**
- **Co-arm neutrality (a2=4):** DTWORLD **bit-identical to the `pfire=2`-alone baseline** —
  `1024 h=4a4885ea924b35e4/seed=68cf9c25`, `2048 h=8011fcf6ca8bf38d/seed=19a45820` (both runs equal). ⇒ the
  co-arm + drain-wire composition is **behavior-neutral**. (Both diverge from the *a2=0* baseline by exactly
  pfire=2's inherent two-phase RNG reorder — not the wire.) Flip OFF: `fires=0 swapped=0 in==out=0`.
- **Drive-flip (a2=5):** DTWORLD **diverges by design** — `1024 h=9d1ceaecadbd274c`, `2048 h=e571805005a2d429`.
  Flip active: `fires=2047 swapped=1,537,658 in=2047 out=2047` (**in==out ⇒ no leaked frozen state**); zero
  crashes over 4097+ ticks, walk≈1.8M.
- **Replay-determinism (THE gate, not baseline-match):** two independent `a2=5` runs are **bit-identical** —
  same hashes AND same `swapped=1,537,658`/`in==out=2047`. **⇒ the flip is fully deterministic.**

**⇒ a2.2.3 clears its gate.** The FrozenSnapshot is now a live read source feeding fire-control aim, the
a2↔pfire composition is proven neutral, and the first by-design DTWORLD divergence is deterministic.
Evidence: `eaw-mt.log.a223-{a4,pf2base,a5runA,a5runB}`. (Hook `winmm_proxy.c`: `a2_on()` levels 4/5,
`a2_fire_freeze_in/out`, the block-8b wire, the freeze-swap in `pfire_a62d0_intercept`.)

**Next: a2.4** (N-shard) — partition the walk by `shard_of(obj+0x50)` on real workers with canonical drain;
gate = N≡1 replay-determinism + lockstep + the measured speedup. (Optional refinement: confirm the
ship→target offset to narrow the flip from whole-world to target-cross-read-only.)

### 🔧 a2.4 START — `3a6b80` per-object body, RE WORKSHEET + buildability findings (2026-06-13, IN PROGRESS)

a2.4 (N-shard real threads) requires self-driving the per-object body so cheap-mass can move to workers.
The walk still calls the monolithic binary `FUN_1403a6b80`. Fully decoding it (`decomp/3a6b80.c` +
objdump of `0x1403a6b80`) surfaced **three structural findings that reshape the approach** — banked here so
the transcription is mechanical next session:

**FINDING 1 — risk and sharding-payoff are in DISJOINT spans.** The hard-to-transcribe spans are all
*serial-pass gaps that never parallelize*: block B render/scene vtable-getter chain (decode 159–261, ~26
lost-arg vtable calls), the Lua AI-pump scheduling + RNG seed (267–346, needs `r14` control-flow tracing),
the DoT roll (376–386). The only spans that ever move to a worker are the cheap-mass: locomotor copy
(40–66) + `557ba0`, behavior loop `vtable[0x30]` (132–138), `3ac530` (140), `3c2710` (144). ⇒ a verbatim
transcription spends most of its risk on code that stays serial forever.

**FINDING 2 — cheap-mass and serial are INTERLEAVED**, no clean prefix/suffix cut: order is loco-copy
(cheap) → event-loop `3a9e30` (serial) → behavior loop (cheap) → `3ac530`/`3c2710` (cheap) → `266340`+block
B (serial) → tail (serial). So "self-drive cheap-mass, binary-call the serial tail" can't be a single split;
the serial spans are inline (no callable mid-function entry). User chose the **hybrid** (self-drive
cheap-mass, leave serial as binary) — realizable as transcribe-outer-flow + per-span binary helpers, OR as
call-site repointing of the cheap-mass sub-calls inside binary `3a6b80` (the proven `pfire_repoint_calls`
pattern — leaves ALL serial code untouched binary).

**FINDING 3 — `sim/` is NOT linked into the hook** (`winmm_proxy.c:4263`: "the hook is a single C TU; sim/
is C++"). The in-game path **re-transcribes each lifted body in C** inside the hook (as DTSPL2's mover is).
So a2.4.1's "swap in `sim::dynamic_transform`" means a fresh C re-transcription in the hook, not a call into
`sim/`. The DTDYN/etc. oracles validated the C++ `sim/` offline; the in-game C body is a separate (small)
transcription, gated + DTWORLD-identity-validated.

**VERIFIED RE (objdump of `0x1403a6b80`, params `(GameObjectClass* obj, uint p2/tick, char p3/sil)`):**
- *Field offsets:* `vtable`@+0x0, `pos`@+0x78/7c/80, `field_0x98`, `locomotor_state`@+0xa8, `+0xb0`,
  `+0xf0`, `+0x100`, event-vec @+0x60/+0x68, `listener_list`@+0x38, `behaviors`@+0x278,
  `behavior_count`@+0x290, `+0x298`/`+0x2a0`/`+0x2b8`/`+0x2e0`, `hardpoints`@+0x2d0, `lua_ctx`@+0x2d8,
  `+0x2f0`/`+0x2f8`/`+0x320`/`+0x3a7`, **`flags` = dword @+0x3a0** (flags&8→[0x3a0]&8, &0x100→[0x3a1]&1).
- *Cheap-mass call targets/args:* `557ba0(loco)`→char; behavior `vtable[0x30](behavior,obj,p2)` after
  `4c3700(behavior)==1` && `behavior[0x30]<=p2`; `3ac530(obj,0)` (existing `A2_f3ac530`, 2-arg — decode
  drops edx=0); `vtable[0x10](obj,2)`→sel ptr then `3c2710(sel)`.
- *Serial helpers (stay binary):* `5369e0(*(obj+0xf0)+0x138, p2)`; `3a9e30(obj, …14 args…)`;
  `266340(*(obj+0x2a0), obj+0x78, (DAT_8007dc*DAT_8007d4/DAT_8007f4)*field_0x98, DAT_b0a320, 0)`;
  `37da80(*(obj+0x100)+8, p2)`; `247a90(lua_ctx)`; `246fb0(lua_ctx, "ServiceRate"@RVA0x7ff960, 0)`;
  fire `396070`/`396df0(obj)`→float, `3a8ad0(obj,clamp)`, `3a76b0(obj,p2)`; DoT `3727a0(*(obj+0x298),obj)`,
  `3ab890(obj,0x11)`.
- *The two determinism-critical RNG draws* — `1ffbb0 = float(uint32_t* lcg, float lo, float hi)` (lo/hi in
  XMM1/XMM2, Ghidra shows them as ints): constants `xmm8=0.0`, `xmm7=DAT_140800860`(=uVar3); LCG =
  `&DAT_140a13e24`. DoT site (382): `ffbb0(&LCG, lo=ratio^uVar3bits, hi=ratio)`, `ratio=xmm6/DAT_805898`.
  AI-pump LastService site (326): `ffbb0(&LCG, lo=0.0, hi=(float)*(double*)(r14+0x10))`, r14=LastService
  node, result bit-XOR `xmm7`(uVar3). 26 indirect/vtable calls total; `2647a0`→float (xmm0).

**RECOMMENDED NEXT (de-risked):** implement the hybrid via **call-site repointing** — `pfire_repoint_calls`
the cheap-mass E8 sites inside binary `3a6b80` to gated intercepts (`EAW_A2_BODY`): =1 pass-through identity
foothold (DTWORLD == `ea5f…`/`7f7f…`), =2 redirect `3ac530`→a C re-transcription of DynamicTransform. Leaves
ALL serial spans untouched binary (honors the hybrid + Findings 1–3), zero transcription of block B/lua.
The fully self-driven outer-flow transcription (for worker extraction) can follow once each cheap-mass body
is repoint-validated in-situ.

### ✅ a2.4.0 RESULT — the cheap-mass call-site repoint mechanism is LIVE (2026-06-13)

`EAW_A2_BODY=1` installs `install_a2body_hooks` (`winmm_proxy.c`): `alloc_near` a stub →
`pfire_repoint_calls(3a6b80, A6B80_BODY_SIZE, 3ac530, stub)` repoints the one `3ac530` E8 site inside binary
`3a6b80` to `a2_body_3ac530_intercept`, which (level 1) is **pure pass-through** to the real `3ac530`.
Self-contained install (own imgbase + real pointer), orthogonal to `EAW_A2`/`EAW_PFIRE` (distinct E8 site
from pfire's `a76b0`), works with the stock walk.

**Validation (`just a2body=1 difftrace=1`, demo battle-1):** repoint `site(s)=1` (exactly one DynamicTransform
call site); intercept genuinely in-path — **1.31M+ calls** routed through it; DTWORLD **bit-identical to the
`EAW_A2_BODY=0` baseline** (`1024 h=ea5f27913390cce2/seed=8f00da8b`, `2048 h=7f7fdaf6ca0fcbaa/seed=cdb39e0c`);
zero crashes. **⇒ the redirect mechanism is proven inert** — the foundation a2.4.1 uses to swap a lifted body
in-situ. Evidence: `eaw-mt.log.a240-body1`.

### ✅ a2.4.1 RESULT — the C DynamicTransform numeric core is bit-exact IN-SITU (2026-06-13)

⚠️Decoding `3ac530` showed it is **not pure numeric** — `sim/dynamic_transform.cpp` lifted only the
matrix-rebuild core (identity + 3 Givens + 90°); the full `3ac530` also has a **Class-3 render/scene tail**
(`266340`/`265d80`/`vtable[0x28]`/listener loop) that never parallelizes. A full C *replace* would force
transcribing that tail (risk, zero shard payoff — the §a2.4 Finding-1 pattern one level down). So a2.4.1 is
the project-standard **in-situ SHADOW ORACLE**, not a replace.

`EAW_A2_BODY=2` (`winmm_proxy.c`, `a2_dt_core` + `a2_body_3ac530_intercept`): a literal C port of
`sim/dynamic_transform.cpp` (the guarded sin/cos via binary `776650`/`776150`, three Givens with the
decompile's operand order). The intercept captures the inputs (euler `+0x90/94/98`, pos `+0x78/7c/80`), lets
the **binary drive** (numeric + render tail), then runs the C core into a scratch matrix and compares it
bit-exact against the binary's written matrix at `obj+0x248`. Constants read live from the binary (no assumed
1.0/2.0/360.0 — Rule 2): `d2r=(DAT_8007dc·DAT_8007d4)/DAT_8007f4`, 90°=`d2r·DAT_8007ec`, small-angle
cos=`DAT_1407ffaf8`.

**Validation (`just a2body=2 difftrace=1`, demo battle-1):** repoint `site(s)=1`; **shadow `ok=1,310,721
bad=0`** — the C core matched the binary's matrix bit-for-bit on every one of 1.31M real calls (no FIRST
MISMATCH ever fired); DTWORLD **bit-identical to baseline** (`ea5f…`/`7f7f…`) — behavior-neutral (binary
drives); zero crashes. **⇒ the lifted DynamicTransform numeric core is proven correct in-situ on real game
data** — the prerequisite for running it on a worker. Evidence: `eaw-mt.log.a241-body2`.

### ✅ a2.4.2 RESULT — the proven DT core runs on a WORKER thread concurrently (2026-06-13)

The threading foothold — the first time a2's cheap-mass actually runs off the main thread. `EAW_A2_BODY=3`
(`winmm_proxy.c`, `a2body_worker` + `a2body_dt_offload`): the `3ac530` intercept snapshots `{inputs,
binary-matrix}` per call and appends to a **double-buffer**; a single persistent worker (`CreateThread`,
auto-reset events) recomputes `a2_dt_core` for each item and compares bit-exact — processing batch A while
the main thread fills batch B (genuine **overlap**, threshold `K=2048`). The worker only ever reads recorded
copies (no live game object) ⇒ no race on sim state; the binary still drives ⇒ behavior-neutral. N=1
("1-shard first", per §7 gate).

**The key unknown this resolves:** can `a2_dt_core` — including its binary `sin`/`cos` leaves (`776650`/
`776150`) — run on a worker **concurrently** with the main walk (which also calls those leaves) without a
race? **Yes.**

**Validation (`just a2body=3 difftrace=1` ×2, demo battle-1):** worker thread started; **WORKER-OFFLOAD
ok=1,308,672 bad=0, dispatches=640** (640 overlapped batch hand-offs); no `FIRST MISMATCH`; DTWORLD
**bit-identical to baseline** (`ea5f…`/`7f7f…`) — behavior-neutral; **zero crashes**, reproduced across both
runs. **⇒ the binary trig leaves are reentrant and the proven cheap-mass core runs correctly off-thread,
concurrently** — the core threading premise of a2.4 is validated. Evidence:
`eaw-mt.log.a242-body3run{A,B}`.

### ✅ a2.4.3 RESULT — the DT shadow runs on an N-SHARD WORKER POOL, bit-exact for every N (2026-06-13)

N-shard the a2.4.2 offload. `EAW_A2_BODY=4` (`winmm_proxy.c`): each `3ac530` work-item routes to
`shard = shard_of(obj+0x50) = object_id %% N` — the `sim/shard_scheduler.h` partition contract transcribed
into the C hook (sim/ is not linked; the contract honoured = a STABLE pure fn of the object's stable id,
whole object → exactly one shard, so its hardpoints never split). N from `EAW_A2_SHARDS` (default 4,
clamp [1,8]); `EAW_A2_BODY=3` stays the proven N=1 rung. Each shard owns its own double-buffer + worker +
`ok`/`bad` counters (sole writer per shard ⇒ no per-shard atomics), so **N workers + the main thread all
call the binary `sin`/`cos` leaves concurrently**. Determinism argument: the per-item compare is a pure fn of
the recorded inputs, so its result is INDEPENDENT of which shard ran it ⇒ **`bad=0` for any N** is the gate
(and `ok≈calls` minus the per-shard sub-K tails).

**Validation (`just a2body=4 a2shards=N difftrace=1`, demo battle-1, N∈{1,4,8}):**

| N | pool started | `ok` | `bad` | dispatches | DTWORLD 1024/2048 | mismatch |
|---|---|---|---|---|---|---|
| 1 | N=1 of 1 | 1,570,816 | 0 | 768 | `ea5f…`/`7f7f…` ✓ | 0 |
| 4 | N=4 of 4 | 1,568,768 | 0 | 766 | `ea5f…`/`7f7f…` ✓ | 0 |
| 8 | N=8 of 8 | 1,564,672 | 0 | 764 | `ea5f…`/`7f7f…` ✓ | 0 |

**`bad=0` across all N** (≈1.57M comparisons each); DTWORLD **bit-identical to baseline** (behavior-neutral —
binary still drives); **zero crashes**. `ok` shrinks slightly as N rises (more sub-K tails — `calls−ok` =
2049/4097/8193 ≈ N·K), exactly as predicted. **N=1 reproduces the a2.4.2 single-worker artifact byte-for-byte
(`ok=1,570,816 dispatches=768`)** ⇒ the pool refactor is behavior-identical at N=1. **⇒ the proven cheap-mass
core shards correctly across N concurrent workers, result-independent of the partition.** Evidence:
`eaw-mt.log.a243-n{1,4,8}`.

⚠️Build lesson banked: the level-4 ARMED log string (~520 B with multibyte `—`/`⇒`) overflowed a `char[400]`
sprintf buffer → stack smash → `c0000005` av_read @-1 on `a2_body_on()` return (caught the first launch).
Fix = `snprintf` into `char[768]`. Re-check every new long log line's buffer.

### ✅ a2.4.4 RESULT — the SHADOW→APPLY: our matrix is live, binary `3ac530` SKIPPED, world bit-identical (2026-06-13)

The first time a2 cheap-mass actually DRIVES (not shadows). `EAW_A2_BODY=5` (`winmm_proxy.c`): the intercept
stops calling binary `3ac530` entirely and instead (1) runs `a2_dt_core` and `memcpy`s its 12-float matrix
straight into `obj+0x248` — the live one — then (2) runs the `3ac530` **render tail itself**
(`a2_dt_render_tail`), the Class-3 scene/anim/listener block (`vtable[0x28]`→`265d80`→`266340`→`vtable[0x10]`
→`3982b0`→listener loop, then clear `obj+0x3a1 & 0xfe`), transcribed from `decomp/3ac530.c` + the
`0x1403ac746` objdump. The seam: the numeric prologue's ONLY object writes are the 12-float matrix at
`obj+0x248..0x277` (incl. the translation column M[3]/M[7]/M[11]=pos), which `a2_dt_core` already reproduces
bit-exact (a2.4.1–3); everything the tail reads (`+0x98` euler_z, `+0x78` pos, `+0x2a0/2e0/298/240/335/30/
3a1`) is untouched by the prologue ⇒ injecting our matrix and running the tail is behavior-identical.

⚠️ABI nailed from the objdump (the decompile under-typed these): `265d80`'s 2nd arg is a **float in xmm1**
(not int); `266340` = `(renderobj /*rcx*/, &pos=obj+0x78 /*rdx*/, d2r·euler_z /*xmm2*/, DAT_140b0a320 as
u32 /*r9d*/, mode byte /*[rsp+0x20]*/)`; the listener loop calls `vtable[0x20](o=*node, node, 3982b0(obj))`.
The CFG `_guard_check_icall` (`01fa90`) is intentionally omitted (validates the indirect target only, no
behavior). Main-thread first (no worker) to isolate the apply mechanism from threading.

**Validation (`just a2body=5 difftrace=1` ×2, demo battle-1):** **SELF-APPLY** path took **every** call
(applied = calls = 1,572,865 / 1,310,721; binary `3ac530` never entered); DTWORLD **bit-identical to baseline**
(`1024=ea5f27913390cce2`, `2048=7f7fdaf6ca0fcbaa`) on **both** runs; **zero crashes**. **⇒ the full self-drive
of `3ac530` — our C numeric written live + the C-transcribed render tail calling the binary leaves — is
bit-faithful.** The matrix rebuild is now genuinely OURS (the binary no longer computes it), and the render
tail is proven faithfully callable from C. Evidence: `eaw-mt.log.a244-apply{A,B}`.

LESSON: an "apply" past a monolithic binary fn doesn't need a fragile mid-function entry — find the seam by
what the prologue WRITES (here: only `obj+0x248`), inject the proven value, and transcribe the *tail's
orchestration* (calling binary leaves), reading each leaf's real ABI from the objdump (Ghidra under-types
float-in-xmm args). DTWORLD-vs-known-baseline is a stronger gate than self-reproduction (it pins to the stock
world, not just determinism).

### ✅ a2.4.5 RESULT — MEASURED the per-slice Amdahl: `3ac530` is ~35% numeric / ~65% render tail (2026-06-13)

Before building an off-thread numeric (which from the per-call hook forces a render-tail-reordering pipeline —
the tail is serial Class-3 and `numeric(i)` needs obj_i's *post-locomotor* pose so it can't be hoisted to a
tick-start batch), MEASURE the split (§8 "measure first", the highest-risk-Amdahl mitigation). `EAW_A2_BODY=5
EAW_A2_BODY_PROF=1` (`winmm_proxy.c`): `rdtsc` brackets the apply — `t0`→`a2_dt_core`+matrix-write→`t1`
→`a2_dt_render_tail`→`t2` — accumulating cycle sums + count, subtracting a once-measured back-to-back rdtsc
overhead floor. Timing-only ⇒ behavior-neutral.

**Validation (`just a2body=5 a2bodyprof=1 difftrace=1` ×2, demo battle-1, n≈1.57M each):**

| run | numeric | render tail | numeric frac | per-slice off-thread ceiling | DTWORLD |
|---|---|---|---|---|---|
| A | 627 cyc | 1167 cyc | **35.0%** | **1.538×** | `ea5f…`/`7f7f…` ✓ |
| B | 636 cyc | 1214 cyc | **34.4%** | **1.524×** | `ea5f…`/`7f7f…` ✓ |

Rock-stable in-run (35.3→35.0% / 34.2→34.4% across 786k→1.57M calls) and cross-run (~34–35%). DTWORLD
bit-identical to baseline both runs; **zero crashes**. Evidence: `eaw-mt.log.a245-prof{A,B}`.

**⇒ FINDING: the `3ac530` transform slice is ~35% parallelizable-numeric, ~65% serial-render-tail.** The
per-slice pipeline ceiling (overlap `numeric(i)` with `tail(i−1)`, per-object wall = `max(num,tail)` = tail) is
only **~1.53×**, and getting it would cost the render-tail-by-one reorder (DTWORLD risk) + a last-object
cross-frame flush (UAF guard). 35% is **more than feared** (not marginal) but still tail-bound. This also
refines the a2.0 Amdahl model for the transform slice: of `3ac530`'s cost, only ~35% ever parallelizes; the
render tail is serial-on-main under EVERY a2 architecture (per-call pipeline OR object-level shard).

**Next: a2.4.6 (user decision)** — the data favors **object-level sharding** (shard whole cheap-mass object
bodies across N workers via the `3a6b80` hook; numeric of `3ac530`/`557ba0`/loco/behavior all parallelize
together, render/fire stay serial) over optimizing this one slice's 1.53× pipeline. That is the real
Phase-A/B split + the true a2 end-goal, but the bigger build (the interleaved cheap-mass/serial cut a2.4-START
flagged). Alternatively build the 1.53× pipeline as a self-contained win first. `557ba0`/`3c2710` get the same
apply treatment; behavior-loop `vtable[0x30]` (indirect, not E8) is handled when self-behaviors drive workers.

### 🔧 a2.4.6 START — object-shard pivot CHOSEN (user, 2026-06-13): it forces a full `3a6b80` body transcription

User chose object-level sharding over the 1.53× pipeline. Deep RE of `decomp/3a6b80.c` (390 lines) + objdump
`0x1403a6b80` settles the buildability and reshapes the increment:

**FINDING A — no incremental/shortcut path; the body transcription is ALL-OR-NOTHING.** The cheap-mass that
must move to a Phase-A worker (the inline locomotor field-copy 42–66, the behavior loop 132–138) is **inline
code, not E8 call sites** — so unlike `3ac530`/`3c2710`/`557ba0` it CANNOT be owned by `pfire_repoint_calls`.
And there is **no flag that splits cheap-mass from serial**: `param_3`(sil) gates the whole head+Lua+fire
(skips far too much — incl. fire); block-B render is gated only by `obj+0x2a0`. There is **no safe
mid-function re-entry** into the binary, so a partial transcription that "calls binary for the rest" is
impossible. ⇒ owning the cheap-mass requires transcribing the ENTIRE per-object body (the a2.2.0-for-the-body
step), and its first validatable milestone is the COMPLETE body driving DTWORLD-identical.

**FINDING B — it IS feasible (the block-B blocker is dissolved).** The a2.4-START worry ("~26 lost-arg vtable
calls" in block B 159–261) is resolved by objdump: those are simple **`this`-only getters** — e.g.
`mov rcx,[rax+0x20]; call [rax+0x108]` = `(*(fn**)(*(i64*)X+0x108))(X)`. Ghidra just dropped the `rcx`
attribution. Every serial leaf is identified (event `3a9e30`+vector-erase `582a0`/`38a1f0`/`3ac8a0`; block-B
pose-eval `12d2c0`, scene `264cb0`/`264c80`/`2647a0`/`265280`/`2648b0`/`4775d0`/`294a40`/`2824d0`/`264840`/
`278fa0`/`266340`, `this`-only vtable getters at `+0x100/108/110/118` + `vtable[0x28]`; Lua `246fb0`/`247a90`/
`769c58`/`247000`/`LuaValue::vftable`; fire `396070`/`396df0`/`3a8ad0`/`3a76b0`; DoT `3727a0`/`3ab890`; RNG
`1ffbb0`). So the transcription is mechanical but LARGE — and ~250 of 390 lines are the high-risk serial spans
(event-loop vector-erase 82–130, block-B render 159–261, Lua pump 264–346) FINDING 1 flagged.

**SPAN MAP (the transcription unit list, each = own-C or call-binary-leaf, top-level guards orchestrate):**
1. loco 37–78: 24 field-copies (own) + `557ba0`(loco)→char + completion tail (`41aeb0`/vt[0x20]/vt[0x18]/
   `58570`/`220ed0`). 2. preamble 79–81: `5369e0(*(obj+0xf0)+0x138,p2)`. 3. event loop 82–130: per-event
   `3a9e30(obj,…14 args…)` + std::vector-erase of 0x38-stride elems (`582a0` move, `38a1f0` destroy, `3ac8a0`
   clear-if-≤1). 4. behaviors 132–138: `for i=count-1..0: b=behaviors[(char)i*8]; if b[6]<=p2 && 4c3700(b):
   vt[0x30](b,obj,p2)`. 5. transform/select 139–146: `if flags&0x100 { OUR 3ac530-apply(obj); if obj+0xb0 &&
   *(obj+0xb0)+0x38 && (sel=vt[0x10](obj)): 3c2710(sel) }`. 6. render266340 147–157: gate `264840`/`278fa0`
   then `266340(...)`. 7. block B 159–261: vtable-getter pair compare → pose-eval `12d2c0`+vt[0x28]; scene
   visibility `264cb0`/`264c80`; alpha `2647a0`/`265280`/`4775d0`; flag-clear (all `this`-only calls).
   8. Lua 264–346: `37da80`; ServiceRate/LastService node-walk via `246fb0`+Lua-value list, gate then
   `247a90(lua_ctx)` pump + `1ffbb0` RNG seed + `769c58`/`247000` LuaValue alloc/set. 9. fire 347–373:
   hardpoint scan (count `+0x4d`==1) → `396070`/clamp/`396df0`/`3a8ad0`; `3a76b0(obj,p2)`. 10. `obj+0x320 *=
   DAT_b26fa4`. 11. DoT 376–386: `3727a0`+`1ffbb0` RNG+`3ab890(obj,0x11)`. The two determinism-critical RNG
   draws (AI-pump :326, DoT :382) use `1ffbb0=float(u32* lcg,float lo,float hi)` (XMM1/2; `xmm7=DAT_800860`
   bit-XOR), LCG=`&DAT_140a13e24` — must hit the global stream in the exact order/values (DTWORLD gate).

**Build plan:** transcribe all spans into `a2_a6b80_body` (per-span helpers) calling binary leaves; gate
`EAW_A2_BODY=6` under the **EAW_A2 walk-driver** (block-2's `g_a2.f3a6b80(obj,p2,sil)` → `a2_a6b80_body`);
identity-first, DTWORLD-gated (`just a2=1 a2body=6` must == `ea5f…`/`7f7f…`, 0 crashes). Default OFF ⇒
release-safe. THEN a2.4.7 reorders the transcribed body into Phase-A(cheap-mass: spans 1,4,5)/Phase-B(serial),
a2.4.8 shards Phase-A across N workers by `shard_of(obj+0x50)` with the serial Phase-B drained on main in walk
order. Multi-turn; this is the riskiest control-flow change since a2.2.0 (but gated + identity-validated,
same playbook).

### ✅ a2.4.6.1 RESULT — body-transcription seam + PER-OBJECT-DELEGATION harness LIVE; 6 mechanical spans proven (2026-06-13)

The "all-or-nothing" transcription is dissolved by a **per-object delegation** harness: `a2_a6b80_body`
(`winmm_proxy.c`, `EAW_A2_BODY=6`, driven by the walk-driver's block-2) runs OUR transcription for objects
whose path hits only transcribed spans, and **delegates the WHOLE object to binary `3a6b80`** for anything
that would hit a not-yet-done span (`a2_a6b80_delegate`: events present `+0x68!=+0x60` / scene obj `+0x2a0` /
`lua_ctx +0x2d8` / hardpoints `+0x2d0` / DoT flag `+0x3a0&0x40000` / sil). Delegated objects are always
correct ⇒ **DTWORLD stays the gate at every increment** while we move spans from delegate→transcribed. No
`3ac530` repoint at level 6 (the walk-driver routes the whole body to us); leaves lazy-bound from
`g_a2.imgbase`.

Increment 1 transcribes the mechanical spans — loco copy 40–66 (24 dword copies, offsets vs objdump) +
`557ba0`+completion-tail 67–77, preamble `5369e0` 79–81, behaviors 132–138, transform/select 139–146 (binary
`3ac530(obj,0)` numeric-only since accepted objs have no scene obj, + `vt[0x10]`/`3c2710`), `37da80` 264–266,
scale `obj+0x320 *= DAT_b26fa4` 374.

**Validation (`just a2=1 a2body=6 difftrace=1` ×2, demo battle-1):** **accepted=2110/2111**,
**delegated≈1.83M**; DTWORLD **bit-identical to baseline** (`ea5f…`/`7f7f…`) **both runs**; **zero crashes**.
⇒ the body-takeover seam under the walk-driver, the delegation harness, and the 6 mechanical spans are proven
bit-correct on the accepted objects. Evidence: `eaw-mt.log.a246-body1run{A,B}`. (Accept count is small because
nearly every object has a scene object `+0x2a0` ⇒ delegates; accepting more requires the render spans 6/7,
the next increments.)

**Next: a2.4.6.2+** — move spans delegate→transcribed, each DTWORLD-gated: head-`266340` (6) + block-B render
(7) [the big one — unlocks the ~all objects with a scene obj], then event-loop vector-erase (3), Lua pump (8),
fire (9), DoT (11). When all spans are transcribed (delegation predicate → never), a2.4.7 reorders into
Phase-A/Phase-B, a2.4.8 shards Phase-A across workers. LESSON: a per-object delegate-to-binary fallback turns
an "all-or-nothing" body transcription into an INCREMENTALLY DTWORLD-validatable ladder — each span proven on
the objects that hit it before the next is added.

---

## 7. Acceptance gate (contract §5, adapted for the fallback)
1. **DTWA `mgr_fail=0`** with requester-`+0x2b8` resolution (the apply path) — already specified.
2. **Replay-determinism + lockstep** (NOT stock-equivalence): a recorded match replays bit-identically on
   the a2 build, and two a2 machines hold lockstep (engine desync check = ground truth). 1-shard first.
3. **Snapshot read-discipline** (debug): no Phase-A worker reads a live cross-object field outside
   `FrozenSnapshot::read_*`.
4. **Measured speedup** ≥ the a2.0 Amdahl ceiling within scheduling overhead, with no stability regression.

## 8. Risks (honest)
- **Amdahl (highest)** — serial fire-control may cap the win; §2/a2.0 is the mitigation (measure first).
- **`3a6b80` sub-call gaps** — an un-lifted non-firing body forces a binary callback mid-Phase-A (live
  cross-read) → either lift it or pull it into the serial pass; a2.1 surfaces these.
- **Gameplay delta** — the retrofit isn't stock-identical; a2.0b signs it off.
- **Takeover stability** — driving the walk ourselves is the riskiest control-flow change yet; kill-switch
  + 1-shard-first + a1's proven plumbing mitigate.

## 9. Cross-refs
- Interfaces + gate: `engine_integration_contract.md` §0/§1/§2/§3/§5; host glue `sim/frozen_snapshot.h`,
  `sim/shard_scheduler.h`, `sim/sim_parallel.{h,cpp}` (§9-gate-green `shard_scheduler_test`).
- The fallback decision: `firing_body_lift_scope.md` §5 gate box.
- The control-flow-takeover precedent: `inproc_integration_milestone.md` (a1 SFX takeover, PASS).
- Tick structure (I4) + drain key: `sim_parallel_command_schema.md` §7.
