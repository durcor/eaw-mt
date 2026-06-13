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
| **a2.3** | **Route cheap-mass cross-reads through `FrozenSnapshot`** + assert read-discipline (§5.3). | snapshot consistency | debug assert: no live cross-read |
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
the sim tick) is in-engine and behavior-neutral. Next: **a2.2.1** — acquire/populate the `FrozenSnapshot`
around block 2 (reads still live, 1-shard), then a2.2.2+ swaps block-2 cheap-mass sub-calls for the lifted
`sim/` bodies + two-phase fire. (DTWORLD coverage caveat: it hashes only fire-control objects; the gate
relies on it + crash-free longevity, the same posture as the spatial-query and pfire validations.)

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
