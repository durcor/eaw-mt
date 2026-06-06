# (b) Phase-A Body Lift — scope for the firing body + Pass-C (2026-06-05)

**Status: sub-program scope.** Milestones a0 (✅ drain key) and a1 (✅ gated SFX takeover) validated the
Phase-B *plumbing* against the live engine. The §0 blocker of `inproc_integration_milestone.md` showed why
a real ≥2-shard tick (a2) cannot reuse the **binary** per-object bodies: (1) the firing path consumes its
`CreateObject` result **inline** (`3825b0:289-401`), so a deferred create has nothing to initialize; (2)
the binary body reads **live** cross-object fields, re-introducing the RAW race `FrozenSnapshot` exists to
prevent — and you cannot retrofit a snapshot into compiled code. ⇒ **a2 needs LIFTED Phase-A bodies that
read a snapshot and emit deferred commands.** That batch is `(b)`. This doc scopes it.

I1–I5 **characterized** these bodies (write-classes, the drain key, the apply targets); `(b)` is the
**lift** — rebuildable `sim/` source, oracle-validated, that I1–I5 deliberately left as "optional in-game
confirmation." It is materially larger than any single behavior lift, so it is itself an increment series
(b1–b5) with a reassessment gate, mirroring I1–I5.

---

## 0. The two targets, and the asymmetry that shapes the plan

The Pass-A parallel set runs two body kinds per the I4 iteration structure
(`(mgr[3],mgr[4]) × {Pass-A master-list 3a6b80 ∥ → barrier → Pass-C second-list 3ac530 ∥ → Pass-D drain}`):

| Target | Size | Verdict | Why |
|---|---|---|---|
| **Pass-C `3ac530`** `DynamicTransformBehavior` | 812 B | **EASY — self body** | A self-frame angular integrator: rotates the object's OWN orientation (`param_1+0x49..+0x4e`, the quaternion/axes) by its OWN angular velocity (`param_1+0x12/0x13/0x94`), **game-speed-weighted** (`(DAT_1408007dc·DAT_1408007d4)/DAT_1408007f4` — the same factor the firing body uses). NO cross-object read or write of sim state; the only cross-boundary outputs are **render-node** updates (`param_1[0x54]` → `265d80`/`266340`, Class-3 presentation, off-lockstep) and an own-child-list notify (`param_1[6]`). Numeric core = the weighted axis-rotations (`480f0`/`2cf8d0`), liftable bit-exact like the locomotors. |
| **Firing body `3825b0`** (reached `3a76b0→387010→387400→3825b0`) | 4034 B | **HARD — the real (b)** | A 4034-byte fire-control orchestrator, ~55 callees, with the inline-consumed create and the live cross-object reads/writes. This is the whole reason a2 is blocked. The rest of §2–§5 is about this body. |

**Consequence:** Pass-C is a quick win (one self-body lift, defer the render outputs). The firing body is
the load-bearing effort and the source of all the risk. Plan accordingly.

---

## 1. Firing body — the Phase-A / Phase-B cut line

The body's work splits cleanly along the write-class boundary already characterized:

```
  PHASE-A  (per-firer, PARALLEL, reads ONLY snapshot + own state)        ── lift to sim/
    ├─ fire-decision GATE        3825b0:62-225   self+weapon reads + a few snapshot reads (fog/LOS/target pos)
    ├─ aim / intercept GEOMETRY  385e70,399450,381dc0,20acd0,383f70,385c70   pure-numeric, read target motion (snapshot)
    ├─ self COOLDOWN/RELOAD      3825b0:403-498   writes own weapon component (param_1) — Phase-A-safe
    │                                             LCG draws (1ffb40/1ffbb0/1ffdb0) → per-entity RNG SUBSTREAM (I2 removed)
    └─ EMIT (buffered, not executed):
         SpawnCommand   projectile create + its FULL init payload (§3)         Class 2
         Command(0x28)  220e90 → target+7 listener  (registers shot on target) Class 2b
         Command(shot)  3a06a0 → firer combat record + GLOBAL sensor mgr b15418 Class 2b/2
         SfxCommand     2d5240 muzzle cue                                        Class 3  (a1 pattern, proven)

  PHASE-B  (serial drain, CANONICAL (gom,rank,seq) order — a0-confirmed key)  ── reuse a1 plumbing
    ├─ SpawnCommand → 29f810 create + apply the §3 payload   (REPLACES the inline 3825b0:289-401)
    ├─ Command(0x28) → 220e90(target+7, …)
    ├─ Command(shot) → 3a06a0(firer, target, projectile)     (touches global sensor mgr → serial anyway)
    └─ SfxCommand    → 2d5240(...)
```

The **self** reads (`param_1` tree: `param_1+0x10` firer object, `param_1+0x20` firer type/owner,
`param_1+0x58/0x5c/0x94/0xa8` weapon state) are Phase-A-safe — one firer owns its weapon, so 1 object = 1
work unit. The **hazard surface is exactly the target** (`param_2`/`param_3`): read for the aim, written by
the two Class-2b emits. That is the classic Class-2b pattern (read snapshot, buffer the write).

---

## 2. Sub-call classification (the firing body's ~55 callees)

| Class | Functions | Lift action |
|---|---|---|
| **Already lifted** | `20acd0` (#7 Euler aim), `139800` (normalize), `22d410` (matrix), `776d48` (sqrt), `776150`/`776650` (fp helpers) | reuse `sim/` |
| **Pure-numeric leaf → lift bit-exact** | `399450` (intercept lead — ⚠ localize global scratch `DAT_140b2c380..394`), `381dc0` (launch spread — RNG→substream), `399e20` (direct-aim fallback), `38ee10` (relative-velocity frame), `383f70`/`385c70` (aim point), `385e70` (muzzle/LOS feasibility) | numeric oracle |
| **Self-state read/write (Phase-A-safe)** | the weapon/owner reads + cooldown/reload writes (`param_1+0x58/0x5c/0x68/0xa8`), `3857d0`,`397780`,`398010`,`540140`,`5400f0`,`540070`,`370f00`,`374b50`,`395c70`,`535cb0/fb0`,`33fb70`,`3952a0` | keep inline (read own component) |
| **Fire-gate predicates reading TARGET/manager** ⚠ | `39b140`,`39a540`,`39b950`,`39b1a0`,`397e00`,`405870`,`372210`,`35f470` (fog) | **fan-out risk** — lift to read snapshot, or prove snapshot-pure (b4) |
| **Class-2 create** | `29f810` (CreateObject) | → `SpawnCommand` (§3) |
| **Class-2b cross-entity write** | `220e90` (25 B signal thunk, order 0x28 → target+7), `3a06a0` (shot-register → firer record + global sensor mgr `b15418` vfunc+0x260), `3846c0`/`382510` (weapon current-target record — actually SELF `param_1+0xa8`) | → `Command` ops |
| **Class-3 presentation** | `2d5240` (muzzle SFX) | → `SfxCommand` (a1) |
| **RNG (shared LCG `a13e24`)** | `1ffb40`,`1ffbb0`,`1ffdb0` | → per-entity substream (removes I2) |
| **Registry/util read** | `294bc0` (team entry by id), `398440`,`394a80`,`397e00`,`285d70`,`058570` | snapshot/registry read |

**The one genuinely hard sub-problem = the fan-out row.** The gate is a cascade of `return 0` predicates
each reading target/manager state internally; lifting it faithfully means lifting (or proving pure) each
predicate against the snapshot. This is the I4-dispatch-fan-out analogue and the reason for the gate in §5.

---

## 3. The `SpawnCommand` projectile payload (replaces the inline `3825b0:289-401`)

The blocker is that the binary writes the new projectile's motion record `plVar12[0x1d]` (= `proj+0xe8`)
immediately after the create. To defer the create these become a **payload** the Phase-B applier writes:

| proj+0xe8 off | Source in `3825b0` | Meaning |
|---|---|---|
| `+0x08` | `param_2` | target object ptr → **target_id** in the command |
| `+0x10` | `param_3+0x18` or `-1` | target sub-id (hardpoint) |
| `+0x58` | `firer+0x50` | **firer object id** (I1-subject linkage) |
| `+0x5c` | `0` | (reset) |
| `+0x60` | `b15418+0x10 + owner+0x4a4` | visibility/team frame |
| `+0x64` | `owner+0x478` or `tmpl+0x474`, ×charge-scale `local_238+0x394` | **damage** |
| `+0x68` | `5400f0(owner)` or `tmpl+0x440` | lifetime/range |
| `+0x6c` | `3857d0 + 397780 accumulations + \|Δz\|` | **muzzle velocity** |
| `+0x74..0x90` | `plVar12` transform fields (ballistic branch `tmpl+0x1fe8≠1`) | ballistic launch transform |
| `+0x24/0x28/0x2c` | target transform rows × Δ (guided branch `tmpl+0x1fe8==1`) | lead/intercept offset |
| `+0x3c/0x40/0x44` | `local_2d8..` launch dir | launch direction |
| `+0xb0` | `local_res20` | shroud/visibility ptr |

Create args (unchanged from I3): `mgr=*(firer.owner+0x2b8)`, `template=lVar7`(projectile type),
`team=*(team_entry+0x4c)`, `pos=&local_2b8` (Euler-rotated dir from `20acd0`), `extra=&local_230`,
flags `(1,0)`. The applier also runs `39d6a0(proj)` + the `vfunc+8`→`220e90(target+7,…,0x28)` listener
edit + `3a06a0(firer,target,proj)`. **No new determinism invariant** beyond I1 (I3 finding) — the payload
is pure data computed in Phase-A.

---

## 4. The `FrozenSnapshot` read-set the firing body needs off the target

`FrozenSnapshot` must double-buffer, per object, at least:
- **transform** `+0x78/+0x7c/+0x80` (position), `+0x248..+0x270` (rotation rows — guided lead)
- **velocity** `+0x254/+0x264/+0x274`
- **flags** `+0x74` (&0x40 dead/no-target), `+0x3b4`
- **identity/team** `+0x50` (id), team via registry `294bc0`
- **predicate sub-records** `+0x53/+0x56/+0x57/+0x69/+7` (read by the gate predicates — exact set pinned in b1)
- **fog/visibility** the `35f470(shroud b15418, firer_team, target)` result — a per-`(observer_team,target)`
  visibility bit; Class-1 (observer-keyed) so it can be a sharded read like #4/#10.

This extends the D-gap struct work (I1) with the *exact* firing read-set — b1's deliverable.

---

## 5. Increment plan (b1–b5) + the reassessment gate

| # | Increment | Closes | Oracle | Risk |
|---|---|---|---|---|
| **b1** | **Decompile + classify** the firing leaves (`381dc0`✅,`385e70`✅,`383f70`,`385c70`,`399e20`,`38ee10`,`3a06a0`✅) and pin the **exact** target read-set (§4) + gate-predicate read-set. Produces the per-call class table with NO gaps. | (D) firing read-set; readability | n/a (enables) | Low |
| **b2** | **Lift the pure-numeric aim/intercept cluster** bit-exact, reading target motion from **explicit params** (snapshot-ready): `399450` (⚠ localize `DAT_140b2c380..394` scratch → thread-safe), `381dc0` (RNG→substream), `399e20`, `38ee10`, `385e70`. **✅ piece 1 DONE 2026-06-05 (host + in-game):** `399e20` + `393b70` linear mode → `sim/firing_intercept.{h,cpp}` (the matrix-free quadratic intercept; global scratch `DAT_140b2ec18..30` localized to returns; host test 5 cases PASS). **DTFINT in-game oracle = BIT-EXACT 473,306/473,306** solved linear-mode calls (`eaw-mt.log.dtfint-pass`), 11 no-solution sentinels matched, **`complex=0`** (the curved predictor `393b70` `DAT_140b2c37c!=0` is DORMANT in practice → the linear lift covers 100% of live calls). **✅ piece 2 (`381dc0` weapon dispersion) DONE 2026-06-06 (host):** `firing_apply_spread` → `sim/firing_intercept.cpp` (branch ladder + 3× `range_f(-m,m)` reusing the lifted `SimRng`; magnitudes hoisted to inputs). RNG-retrofit (per-entity substream) so output differs from the global-LCG binary BY DESIGN — validated by sim_rng.h's reproducibility contract (host: no-draw paths leave rng untouched, perturbing paths consume exactly 3 steps within [-m,m], same seed ⇒ same output). **Remaining:** curved-lead (`393b70` complex + `38ee10` circumcenter, both need matrix leaf `22d390`) — known dormant edge (DTFINT `complex=0`), low priority; `399450` full lead solver (orchestrates 399e20/38ee10/393b70); `385e70` LOS/muzzle feasibility. | aim geometry | **numeric bit-exact** (trampoline vs lifted over live input stream — the project's proven loop) | Med (global-scratch localization) |
| **b3** | **`SpawnCommand` payload + Phase-B applier** (§3) + the two Class-2b `Command` ops (`220e90`, `3a06a0`). Host-assemble: Phase-A computes payload, Phase-B drain creates+applies. | (A/B) the create restructure | host §9 gate (drain N≡serial) **+** in-game DTWA structural (projectile carries identical init fields) | Med |
| **b4** | **The fire-decision gate** reading snapshot — lift/prove-pure the predicate cascade (the §2 fan-out row). | the gate | **structural** (same fire decisions + cadence under same snapshot, long multi-battle capture) | **High (fan-out)** |
| **b5** | **1-shard in-game firing takeover** (the a1 successor): drive the lifted firing body from `sim/` on a snapshot, buffer all emits, drain canonically; DIFFTRACE structural-equivalent to the binary. Then a2 = run b5 on ≥2 shards. | the contract's 3 interfaces, live | structural DIFFTRACE vs serial binary + a1's DTWA/DTDRAIN invariants hold | High (integration) |

Pass-C `3ac530` is an **independent quick win** (self-body, render outputs deferred) — slot it as **b2′**
alongside b2 (same numeric-oracle method), not on the firing critical path. **✅ b2′ DONE 2026-06-06
(host):** `sim/dynamic_transform.{h,cpp}` — the 3×4 transform rebuild from Euler pose + position via the
three Givens leaves (`480f0`/`cf8d0`/inline) + fixed +90° roll, pure self body, host test PASS.
**DTDYN in-game oracle = BIT-EXACT 1,729,364/1,729,364** (`eaw-mt.log.dtdyn-pass`, real rotations incl.
357° poses) — Pass-C closed host + in-game; the cheap-mass self-body the fallback parallelizes is validated.

**Dependency:** b1 → b2 → b3 → b4 → b5. b2/b2′/b3 are de-riskable in parallel; b4 is the long pole.

> **Reassessment gate — after b2 (mirrors the I2 gate).** b2 proves whether the aim/intercept cluster
> lifts clean (global-scratch localization + bit-exact). The harder question is b4's predicate fan-out: if
> b1's classification shows the gate predicates each pull deep target/manager state that itself fans out,
> the firing-body lift could balloon. **Decision point: after b1+b2, judge the b4 fan-out width against the
> 3–4× payoff before sinking b3–b5 effort.** If b4 is intractable, the fallback is to keep firing **serial**
> (a Pass between the parallel sweeps) and parallelize only the cheap mass (movement/sensor/Pass-C) — still
> a real win, and a1+Pass-C already prove that path.
>
> **✅ GATE RESOLVED 2026-06-06 (Rule-6 sign-off): take the SERIAL-GATE FALLBACK.** Evidence: b1 proved the
> fire-DECISION gate is a deep vfunc/sub-object/sensor graph **not servable by a field-copy snapshot**;
> DTFINT proved the GEOMETRY lifts **bit-exact** (473,306/473,306) and the curved-lead is **dormant**
> (`complex=0`); b2′ proved the cheap-mass (Pass-C) parallelizes cleanly. ⇒ **b3/b4/b5 (full gate lift +
> create-deferral) are NOT pursued.** The a2 architecture: keep the per-firer fire DECISION on the
> ordered/serial path (the cheap early-return predicate cascade, creates stay immediate there → the §0
> blocker is sidestepped), and parallelize the **GEOMETRY compute + the cheap per-tick mass**
> (movement/sensor/Pass-C — all lifted + snapshot-ready). Remaining work is ENGINE INTEGRATION (wire the
> built `FrozenSnapshot`/`ShardScheduler` into the live tick: cheap-mass sweep ∥ → serial fire pass), plus
> the DTDYN oracle to close Pass-C bit-exact. `399450`/`385e70` become low-value (the serial pass runs the
> binary body), so they are dropped from the critical path.

---

## 6. Caps (do NOT let the closure expand)
- **Firing up to the projectile spawn-emit ONLY** (program §5 cap). The projectile's own subsequent tick
  recurses through the same machinery (it is just another GameObject); collision broad-phase + damage
  application are **separate later closures**, out of (b).
- **Render-node / bone updates (`265d80`/`266340`/`12d480`) are Class-3, off-lockstep** — defer or ignore
  for lockstep; do NOT faithfully reconstruct the render path.
- **Byte-matching decomp is out** — functional equivalence under the determinism gate (program §3).
- **Pass-D native deferred-create** (`GOM+0x5f0`) is already the binary's own buffer; the lift emits into it
  via `SpawnCommand`, it is not separately reconstructed.

## 7. b1 RESULTS (2026-06-05) — the read-set is a graph, not a slice; the body splits at a geometry/gate seam

Decompiled + classified the remaining target/manager-reading callees (`383f70`,`385c70`,`399e20`,`38ee10`,
`39b140`,`39a540`,`35f470`,`397e00`,`397780`,`39b1a0`,`383ba0`,`540140`). The per-call read-set is now
pinned, and it **reshapes the plan**: the firing body cleanly splits into two halves with very different
liftability.

### 8.1 GEOMETRY half — cleanly snapshot-able (b2 confirmed sound)
| Fn | Reads off the target | Indirection | Hazard |
|---|---|---|---|
| `399450` intercept | pos `+0x78/7c/80`, vel `+0x254/264/274`, rot rows `+0x248..` (flat) | **none (flat offsets)** | ⚠ global scratch `DAT_140b2c380..394` → localize |
| `38ee10` rel-vel frame | — (operates on caller arrays only) | **none — perfectly pure** | none |
| `399e20` direct-aim | flat motion via args | none | none |
| `381dc0` launch spread | flat dir via args | none | RNG `a13e24` → substream |
| `383f70` aim-point | target motion (flat) | **no vfunc** | RNG `a13e24` → substream |
| `385c70` hardpoint pos | `param_3+0x2c/0x38/0x10/0x2a0` → bone cache `12d2c0` | one bone lookup | bone cache (Class-3 presentation read) |
| `385e70` LOS/muzzle | self + bone via `12d2c0`/`3858b0` | one bone lookup | none |

⇒ The aim/intercept geometry reads a **small flat target slice (pos/vel/rot)** plus the bone cache. The
`FrozenSnapshot` field set of §4 **does** serve it. b2 stands.

### 8.2 GATE half — a deep vfunc/sub-object/sensor graph (the real fan-out, surfaced early)
| Predicate | What it actually reads | Why a field-copy snapshot can't serve it |
|---|---|---|
| `39b140` (target timed-state) | `tgt+0x220→+0x20`, `tgt+0x100→+0x2a4/+0x2bc`, frame ctr | **sub-object pointer chains** (chases live ptrs) |
| `39a540` (sensor/stealth) | `tgt+0x3b4` + **sensor grid** `b15418` vfunc+0x210 → `56c720(grid,firer)` → vfunc+0x18(entry,tgt) | **global sensor grid + vfunc dispatch** |
| `35f470` (fog reveal) | `tgt+0x348/34a/34b`, `firer[0x33][team]`, iterate via `35dec0` | **global mutable heap scratch `DAT_140a28538/40/44`** (thread-init-guarded but GLOBAL) + sensor array |
| `397e00` (assigned-target) | `tgt` identity + `tgt+0x2b0`, firer order state `+0x100→+0x270/278` | identity compare across the live graph |
| `397780` (target extent) | `(**tgt[0x10])(tgt,0x16/1)`, vfunc+0x230, `396850` | **vfunc dispatch** |
| `39b1a0` (target behavior) | `tgt+0x2b0→+0x348` | sub-object chain |
| `540140` (category mask) | `3973b0(tgt)+0x1648` vs `owner+0x408` | sub-object chain |
| `3825b0` inline | `(**tgt[0x10])(tgt, 0x11/0x16)` | **vfunc dispatch** |

⇒ **Three indirection mechanisms defeat a per-object field-copy snapshot:** (1) **vfunc dispatch**
`(**(tgt+0x10))(tgt,sel)` reads live state through virtual calls; (2) **sub-object pointer chains**
(`tgt+0x100→…`, `tgt+0x2b0→…`) chase live pointers into other allocations; (3) the **global sensor
grid + global mutable scratch**. The contract's `FrozenSnapshot` is a per-object *state* double-buffer —
it cannot redirect a vfunc or a chased pointer to the buffered copy without recompiling the callee. So the
gate cannot be made snapshot-safe by *snapshotting* — only by **lifting** each predicate to replace its
vfunc/chain reads with explicit snapshot lookups (large), or by keeping the decision **serial**.

### 8.3 Two more global-scratch thread hazards (any parallel firing must localize)
Beyond `399450`'s `DAT_140b2c380..394`: **`35f470` uses `DAT_140a28538/40/44`** (a process-heap scratch
buffer, `_Init_thread_header`-guarded init but a *single global* allocation reused per call). Concurrent
firing workers would race it. Inventory of firing-path global mutable scratch to localize: `DAT_140b2c380`,
`DAT_140b2c384`, `DAT_140b2c388`, `DAT_140b2c38c`, `DAT_140b2c390`, `DAT_140b2c394`, `DAT_140a28538`,
`DAT_140a28540`, `DAT_140a28544`.

### 8.4 What this does to the plan
- **b2 (geometry) is GREEN** and the highest-value clean win — lift the flat-slice aim/intercept cluster
  (localize the 6 `DAT_140b2c3xx` scratch words). Pair with **b2′ Pass-C `3ac530`** (also clean).
- **b4 (gate) fan-out is CONFIRMED, and surfaced at b1** rather than after b2. The gate is not
  snapshot-servable; faithfully lifting it = lifting ~8 predicate subtrees (with their own vfunc fan-out)
  + localizing global scratch. **The reassessment-gate decision is now informed early** (see §5): the
  evidence leans toward the **fallback** — keep the per-firer fire *decision* effectively serial (it is
  cheap: a cascade of early-return predicates) and parallelize the **geometry + spawn-payload compute +
  the cheap mass (movement/sensor/Pass-C)**. That still threads the expensive per-tick math; only the
  branch-heavy, graph-chasing decision stays on the ordered path. **Recommend taking the §5 gate decision
  after b2 lands, with this b1 evidence on the table.**

## 8. b4 REVISIT (2026-06-06) — can the expensive fire-control parallelize? A reframe + its hazards

a2.0 measured the cheap-mass-only fallback at a **~2× ceiling** (p≈0.65), weakest in big battles because
the serial **fire-control** carries ~35% of per-object time on a few units (each fire-call ~28× an average
cheap body). The user asked to revisit b4 — can the expensive part parallelize to lift the ceiling? This
section is that investigation. **Finding: b1's "the gate isn't snapshot-servable" was the wrong frame; a
parallel fire pass is PLAUSIBLE — but gated on resolving several shared-scratch hazards, one of them hard.**

### 9.1 The expensive part decoded — it IS the b1 fan-out
The opportunity-scan (`387400:250-281`) is a random-start linear scan (one LCG draw spreads targeting load)
that picks the FIRST valid candidate; its per-candidate evaluator **`385190`** does a **spatial query**
(`2acb60`→`20e780`) then the **full b1 gate-predicate evaluation** per candidate — `35f470` (global-scratch
fog), `39a540` (sensor grid), the `0x11` target-query + `0x118` score **vfuncs**, `39b140`/`383ba0`/`540140`
/`383f70`. So the scan is exactly the deep vfunc/sensor/global-scratch graph b1 flagged. A field-copy
`FrozenSnapshot` cannot serve it (b1 §7.2) — confirmed.

### 9.2 The reframe — fire as a READ-ONLY PHASE needs no snapshot and no predicate lift
b1 implicitly assumed firing must run *concurrently* with movement (mid-tick mutation) ⇒ needs a snapshot
⇒ needs the predicates lifted to read it (intractable). **But if the fire pass is a SEPARATE phase AFTER
the cheap-mass movement/sensor settle, with all its WRITES deferred, then every cross-object read the gate
predicates make is live-but-STABLE** — nothing mutates it during the phase. No snapshot, no predicate lift.
The work shifts from lifting the READS (the intractable b4) to **deferring the WRITES** (the already-scoped
b3 §3): creates → `SpawnCommand`, the two Class-2b edits (`220e90`/`3a06a0`) → `Command`, SFX → `SfxCommand`
(a1). Architecture:
```
  Phase A ∥   cheap-mass (movement + transform + sensor/fog)   → positions + sensor grid settle
  barrier
  Phase B ∥   FIRE PASS — binary fire-control per firing unit, reads stable live state, writes OWN state,
              BUFFERS creates + Class-2b + SFX (no live mutation of what other units read)
  barrier
  Phase C ·   drain buffers in canonical (gom,rank,seq) order   (BUILT)
```
This parallelizes the **expensive 35%** too → p → ~0.9+ → **4-core ~3.5×, 8-core ~6×** (vs the fallback's ~2×).

### 9.3 The hazards that gate it (why it is not free)
1. **Create-deferral (§0 blocker)** — `3825b0` consumes the create result inline; deferral needs its
   create+init section restructured to emit the `SpawnCommand` payload (§3). **Bounded** (the payload is
   mapped) but real; the predicates around it stay binary.
2. **The spatial-query shared result buffer (the HARD one)** — `2acb60` returns a *persistent shared*
   spatial index; `20e780` writes the query result list into it (`index+8`), which `385190` then reads.
   Concurrent queries from different firing units **race on that shared buffer**. Localizing it means
   per-thread query buffers (refactor the query API) or a lock (serializes the query — part of the
   expensive scan, eroding the win). This is statefuller than a global scalar and is the main risk.
3. **Global mutable scratch** — already inventoried (§8.3): `DAT_140b2c380..394` (399450),
   `DAT_140b2ec18..30` (393b70), `DAT_140a28538/40/44` (35f470 fog). All thread-local-izable.
4. **RNG** → per-entity substream (the scan's start-index draw; already the I2 design).
5. **UNVERIFIED: the query vfuncs** (`0x11`, `0x118`) must be READ-ONLY — if any caches into shared state
   it races. Needs a write-audit (can't assume).
6. **Full write-audit** of the fire path (`3a76b0`/`387400`/`3825b0`) to confirm the ONLY live writes are
   own-state + the (deferred) creates/Class-2b/SFX. A single un-audited live cross-write is a latent desync.

### 9.4 Verdict
**The parallel-fire-pass is a real path to a much higher ceiling (~3.5–6× vs ~2×), and it reframes b1's
pessimism — read-only phasing replaces the intractable predicate-read lift with the scoped write-deferral.**
But it is **moderate-to-high effort with real risk**, chiefly the spatial-query shared buffer (#2) and the
write-audit (#5/#6). It is NOT the "shelve / cheap-2×" option; it reopens b3 (now well-motivated) plus a
**fire-path write-audit increment** (the new prerequisite) before any parallel fire takeover. Recommend:
if a higher ceiling is wanted, do the **write-audit + spatial-query thread-safety assessment FIRST** (cheap,
decisive — same spirit as a2.0) to confirm #2/#5/#6 are tractable before committing to b3 + the takeover.

### 8.5 WRITE-AUDIT RESULT (2026-06-06) — GO: every cross-entity write is deferrable
Audited every write in the fire path (`3a76b0`→`387f50`/`387010`→`387400`→`3825b0`/`385190`/`386660`, and
the target-record updaters `3846c0`/`382510`). Disposition:

| Write target | where | class | disposition |
|---|---|---|---|
| own weapon/hardpoint (budget `+0x28`, target records, cooldown, bone-idx, `last_serviced_tick`) | all | OWN | **safe** (object-granular: 1 object = 1 work item, its hardpoints serial within it) |
| new projectile init (`plVar12[0x1d]…`) | 3825b0 | create payload | **SpawnCommand** (§3) |
| `CreateObject` projectile/debris (`29f810`) | 3825b0, 386660 | Class-2 | **SpawnCommand** → serial drain |
| target `+0x38` listener set/clear self (`220e90`/`220eb0`) | 382510, 3846c0, 3825b0:350 | Class-2b | **Command** (deferred) |
| shot-register → firer rec + shroud-mgr `b15418` vf+0x260 + order emit `38a350` | 3a06a0 | Class-2b | **Command** (deferred — incl. the sensor-mgr write) |
| SFX (`2d5240`/`2d5290`/`2d5320`→`b27e60`) | 3825b0, 386660 | Class-3 | **SfxCommand** (a1) |
| render bone cache (`12d430`/`12d480`, turret-aim `381ff0`) | 387010 | Class-3 render | **safe** — own-object, off-lockstep, no cross-object race under object-granular |
| ⚠ UI message queue (`b27f60` via `2dd630`, "STATION_LEVEL_LOST") | 387f50 | Class-3 presentation (shared queue) | **NEW low-volume channel** — defer or serialize (rare: hardpoint/station-level destroyed) |

**Verdict: NO un-deferrable live cross-entity write exists in the fire path.** Every cross-object write is
already in (or trivially extends) the deferrable Class-2/2b/3 channels the command system handles; render
writes are own-object (safe). The audit surfaced exactly one new channel — a rare UI-queue presentation
write (`b27f60`) — which is low-volume and handled like SFX. ⇒ **the read-only-phase assumption holds on
the WRITE side.**

The remaining blockers are therefore **concurrency-scratch, not writes**: (1) the spatial-query shared
result buffer `index+8` (§9.3#2 — the one hard engineering problem; per-thread query buffers or a lock);
(2) the localizable global scratch (`b2c380..`/`b2ec18..`/`a28538..`); (3) RNG→substream. **Residual open**
(not a write, a read): confirm the query vfuncs `0x11`/`0x118` are read-only (lower risk; if any caches into
shared state it races).

**GO/NO-GO: GO**, conditional on (a) making the spatial query thread-safe (a bounded refactor — per-thread
result buffers) and (b) the query-vfunc read-only check. The parallel-fire-pass path to **~3.5–6×** is
feasible; the next build step is **b3 (create + Class-2b + UI-queue deferral)** + the spatial-query
thread-safety, then the parallel-fire takeover. No intractable predicate-read lift is needed.

## 9. Cross-refs
- The blocker this answers: `inproc_integration_milestone.md` §0 + §2 (a1 PASS).
- The increment discipline this mirrors: `sim_tick_decomp_program.md` I1–I5 + the I2 gate.
- Command ops / drain key: `sim_parallel_command_schema.md` §7 (I4 box, the `(gom,rank,seq)` key a0 confirmed).
- Interfaces b5 wires: `engine_integration_contract.md` Int #1/#2/#3; host gate `sim/tests/shard_scheduler_test.cpp`.
- Already-lifted leaves to reuse: `sim/targeting_aim.*` (`20acd0`), `sim/geom_distance.*`, `sim/damped_spring.*`.
