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
| **b3** | **`SpawnCommand` payload + Phase-B applier** (§3) + the two Class-2b `Command` ops (`220e90`, `3a06a0`). Host-assemble: Phase-A computes payload, Phase-B drain creates+applies. **✅ HOST DONE 2026-06-06** — see §8.8. | (A/B) the create restructure | host §9 gate (drain N≡serial) **+** in-game DTWA structural (projectile carries identical init fields) | Med |
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
5. ~~**UNVERIFIED: the query vfuncs** (`0x11`, `0x118`) must be READ-ONLY~~ **✅ VERIFIED read-only
   2026-06-06 — `vfunc_2` `0x395ac0` + `vfunc_35` `0x566ba0`, full callee audit; see §8.7.**
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

### 8.6 SPATIAL-QUERY THREAD-SAFETY ASSESSMENT (2026-06-06) — MANAGEABLE: short-lock + copy-out
The b4-revisit flagged the spatial query as the one hard hazard. Decoded:
- **The grid is read-only during the per-object walk.** `2acb60` returns a per-team index
  (`manager+0xd0 + team·0x38`). `2be640` (the tick driver) rebuilds every team's grid ONCE at tick-start
  via the `20ed70` preamble loop, *before* the per-object walk; the locomotor/transform/body
  (`625990`/`3ac530`/`3a6b80`) never touch it. ⇒ the grid STRUCTURE is effectively **frozen** during the
  walk (tick-start view — the same semantics the FrozenSnapshot gives, for free). Concurrent **reads** of
  it are safe; no snapshot needed.
- **The query's OUTPUT is the thread-hostile part — and worse than a shared buffer.** `20e780` builds an
  **INTRUSIVE result list threaded through each result object's `+0x18` field** (overwritten per query),
  head at the shared `index+8`, plus a benign global query counter `DAT_140a7c66c++`. Two concurrent
  queries that share *any* candidate clobber that candidate's `+0x18` → corruption. A per-thread *index*
  would NOT fix it (the `+0x18` lives on the shared objects).
- **Mitigation (clean, low-effort, deterministic): a short lock around `{ query + copy result pointers to
  a thread-local buffer }`, then evaluate candidates from the local copy in parallel.** The lock covers
  only the fast tree-walk + a bounded pointer copy (consumption is already capped at **25** by Fix B2), so
  the EXPENSIVE per-candidate eval (fog/sensor/score) stays parallel — the win is preserved. Determinism
  holds: each query reads the same frozen grid ⇒ returns the same candidate set regardless of lock order;
  the shared `+0x18`/`index+8`/counter are written-then-copied under the lock and never read across units.
  No `20e780` rewrite needed. (A full intrusive-list rewrite to a non-intrusive buffer is the alternative
  if the short lock ever shows contention, but the 25-cap makes the critical section tiny.)
- **Residual (a read, lower risk):** confirm the query vfuncs `0x11` (target-query) / `0x118` (score) are
  read-only on the dominant unit/ship impls (if any caches into shared state under the eval, it races).
  **✅ CLEARED 2026-06-06 — see §8.7.**

**Verdict: MANAGEABLE — GO.** The grid is free (frozen by tick-start rebuild), and the query's intrusive
list is contained by a short lock + copy-out that preserves parallelism. The b4-revisit's "one hard
hazard" is downgraded to a small critical section. ⇒ the parallel-fire-pass to **~3.5–6×** stands; the
build prerequisites are now: **b3 (create + Class-2b + UI-queue deferral)**, the **short-lock+copy** around
the spatial query, global-scratch localization + RNG substreams, and the query-vfunc read-only spot-check.

### 8.7 QUERY-VFUNC READ-ONLY SPOT-CHECK (2026-06-06) — CLEARED (b3 opening sub-step)
The last residual READ hazard for a parallel fire pass (§8.6 / §9.3 #5): the two vfuncs the
opportunity-scan evaluator `385190` (and the spawn body `3825b0:74`) dispatch on live target/firer state
must not cache into shared state. Both decoded; **both are pure reads.**

- **`0x11`/`9`/`0x16` selector query = `GameObjectClass::vfunc_2` `0x395ac0`** (vtable slot 2, +0x10;
  the GameObjectClass *base* method — ships/units do not override this generic accessor). Body (43 B):
  `sel==0 → return self`; else `slot = *(char*)(obj+0x332+sel)` (a per-object selector→behavior-slot byte
  table fixed at construction); `slot==-1 → return 0`; else `return *(obj+0x278 + slot*8)` (the object's
  behavior-array). **Pure read + pointer return, no writes.** Used as a gate in `385190:68`/`3825b0:74`
  (`==0`?) and to fetch the targeting behavior in `385190:83`/`387400:136`/`5c9ca0:23`.
- **`+0x118` score = `TeamTargetingBehaviorClass::vfunc_35` `0x566ba0`** (slot 35; `TargetingBehaviorClass`
  and `TeamTargetingBehaviorClass` — the two dominant targeting behaviors — SHARE this exact rva, so the
  check covers both; the score object is what selector-9 returns, gated `firer+0x33b != -1` at `385190:82`).
  Body reads the behavior's owner context (`+0x28`) + the candidate category (`param_2+0x1648`), chases
  read-only ptr chains, and returns the priority float via `FUN_14054dc00`. **Every callee audited pure:**
  `54dc00` (the score lookup — iterates the behavior's OWN priority tables `+0x20/0x30/0x38/0x48/0x50`,
  returns a min-float, no stores), `4ec820`/`3751e0` (1-line mask/deref getters), and the 5 priority-set
  accessors `68dca0`/`68dbb0`/`68dc80`/`68db60`/`68db80` (10-line index getters, no stores). **No writes
  anywhere on the score path.**

**⇒ Both query vfuncs are READ-ONLY on the dominant impls; the parallel-fire-pass's last read hazard is
cleared.** Combined with §8.5 (write-audit GO) and §8.6 (spatial query MANAGEABLE), the *analysis*
prerequisites for the parallel fire pass are complete; the remaining work is the b3 *build* (create +
Class-2b + UI-queue deferral) + scratch localization + the short-lock+copy. (Caveat per Rule 2: other rare
targeting-behavior subclasses are not individually audited; the two majority classes share the method, and
`vfunc_2` is the universal base accessor.)

### 8.8 b3 HOST DONE (2026-06-06) — the create+init restructure, drain-gate-green
Built the §3 keystone as source-only, host-validated code (the parallel-fire-pass build prerequisite from
§8.6). **Files:** `sim/firing_spawn.{h,cpp}` + `sim/tests/firing_spawn_test.cpp` (wired into `just sim-test`),
plus the `ProjectileInit` payload embedded by value in `SpawnCommand` (`sim/command_sink.h`). Full sim suite
green; existing `sim_parallel_test`/`shard_scheduler_test` unaffected by the `SpawnCommand` growth.

**The load-bearing design finding (recorded for the in-game build): the WHOLE inline-consumed block
`3825b0:261-401` becomes the SpawnCommand APPLIER — b3 needs NO new command op kind.** Both Class-2b edits
DEPEND on the freshly-created projectile (`220e90` listener needs `vfunc_2(proj,8)`; `3a06a0` shot-register
needs `proj`), so they cannot be separately-buffered Phase-A signals — they belong in the post-create applier
sequence, which already runs in canonical `(gom,rank,seq)` order. So the cut is:
- **Phase A (parallel, pure):** compute the `ProjectileInit` payload (firer id, target+sub-id, damage,
  lifetime, muzzle speed, vis frame, shroud, guided/ballistic launch geometry) from firer own-state + the
  target snapshot — no cross-entity write — and emit ONE `SpawnCommand`. (`firing_build_projectile_init` /
  `firing_make_spawn`, faithful to the 3825b0 branch logic + FP grouping: charge scale `mod*dmg`, muzzle
  accumulations `addend+acc`.)
- **Phase B (serial drain):** `create_object` runs 29f810 → writes proj+0xe8 from the payload
  (`firing_apply_projectile_record`) → the post-create transform copy (ballistic, reads the NEW proj's own
  transform) → `220e90` → `3a06a0`. This dissolves the §0 inline-consume blocker structurally.

**Gate (host §9, the in-host twin of the later in-game DTWA-structural oracle):** M=240 firers each emit one
projectile spawn; for every N ∈ {1,2,4,8} × {round-robin, contiguous} partition × shuffled within-shard
processing, the drained `(assigned-id, applied ProjectileRecord)` sequence is **BIT-IDENTICAL** to the serial
drain — ids are the contiguous append run (I1), each record matches the applier output for its rank. Plus a
**negative control** (mis-tagging rank by emit order ⇒ divergent id→record assignment, proving the canonical
key is load-bearing here too) and three branch-fidelity unit cases (owner-vs-template damage/lifetime/speed,
charge scale, vis frame, guided lead = rot·delta vs ballistic launch_dir).

**Caps honored (§6):** payload values are computed from RESOLVED ENV inputs (the firer-stat getters
`3857d0`/`397780`/`5400f0`/`3952a0`/… stay ENV, same pattern as `firing_intercept`); per-field bit-exactness
vs the binary is the in-game DTWA-structural oracle below (the host gate proves the RESTRUCTURE preserves
order + the id→init mapping, not the binary's exact arithmetic).

### 8.9 DTWA-B3 in-game STRUCTURAL oracle = PASS (2026-06-06) — §3 source-map confirmed bit-exact
Built the in-game twin (`winmm_proxy.c`, EAW_DIFFTRACE=1, observe-only): entry-detour the firing body
`0x3825b0` (18-byte position-independent prologue `mov rax,rsp`+6 pushes+`lea`), capture the created
projectile via a piggyback on the existing DTWA-SPAWN `29f810` detour (the FIRST create inside the firing
window is the projectile), and at exit compare the binary's `proj+0xe8` motion/combat record against a
recompute of each §3 field from **PURE memory reads** (no engine calls — `5400f0`'s lifetime is read from
its memoized cache field `owner_type+0x4a0` that the binary just filled, and the charge-damage path is
counted not asserted, avoiding the stateful `3952a0`). Two rounds, menu-demo space battle; **evidence
`eaw-mt.log.dtb3-pass`.**

**RESULT @tick=2048 over 362 firing-path projectile spawns — every measurable §3 field BIT-EXACT, zero
mismatches:** `firer_id` (rec+0x58 == owner+0x50) **362/0**; `target` (rec+0x08 == param_2) **362/0**;
`damage` (rec+0x64 = owner+0x478 override else template+0x474) **362/0**; `lifetime` (rec+0x68 = the
`5400f0` result else template+0x440) **362/0**; `target_sub_id` (rec+0x10) **136/0** on the measurable
cases. No regression to the co-installed oracles (DTWA `idfail=0/ctrfail=0` over 39,667 creates, DTDRAIN
`rank_down=0`) — the new entry-detour does not perturb the create/order path.

**One §3 REFINEMENT the oracle forced (measure-don't-assert paid off again):** `target_sub_id`'s source is
the **RESOLVED** `param_3`, not the caller's raw arg. When the caller passes a non-null `param_3` it is used
unchanged (gated at `3825b0:68`) and rec+0x10 == `*(param_3+0x18)` bit-exact (the 136/0). When the caller
passes **NULL**, `3825b0:107-110` REASSIGNS `param_3` via `398440`/`394a80` to the resolved target-hardpoint
context — an internal local invisible to an entry/exit detour, so those 226 spawns are counted `sub_reasn`
(not mis-asserted against the stale entry NULL). ⇒ the lift's `target_sub_id` input must be the resolved
context (the host `firing_spawn.h` comment now says so); this is the recurring **mutated-mid-body-arg**
provenance lesson (DTSPL2/DTWA `obj+0x58`), caught a third time by an oracle that MEASURES rather than
asserts. **Not exercised in the menu demo (documented, unconfirmed in-game):** `vis_frame` (no projectile
had `owner_type+0x4a4 > 0`) and the charged-damage scale (no charged shots). **⇒ b3's §3 field→source map
is confirmed against the live binary for every substantive payload field; the restructure's Phase-A payload
would reproduce the binary's `proj+0xe8` record.**

**Remaining for the parallel-fire build:** the global-scratch localization + RNG substreams + the
short-lock+copy around the spatial query (§8.6) → §8.10.

### 8.10 PARALLEL-FIRE BUILD — host pipeline + concurrency primitive DONE; in-game takeover SCOPED (2026-06-06)
With b3 (host §8.8 + in-game §8.9) closing the create-deferral, the two HOST-buildable pieces of the
parallel-fire path are built + determinism-gate-green:

- **The assembled two-parallel-phase tick** (`sim/tests/parallel_fire_test.cpp`) realizes the §8.2 b4-revisit
  architecture from the built pieces (ShardBuffer + drain_parallel + the b3 `firing_spawn` payload):
  `PhaseA∥ cheap-mass (settle own-state) → barrier → PhaseB∥ fire pass (read SETTLED state, buffer b3
  SpawnCommand + Class-2b signal + SFX) → barrier → PhaseC drain`. **Gate PASS:** bit-identical N-shard ≡
  1-shard for N∈{1,2,4,8} × {round-robin, contiguous} × SHUFFLED processing in BOTH phases — the proof that
  the read-only fire phase needs no snapshot and no predicate lift (PhaseB reads live-but-stable state,
  writes only its own shard buffer ⇒ output depends only on (id, settled world), never on shard/order).
  Also demonstrates the **retrofit delta** (CLAIM 2): the two-phase tick DIFFERS from a stock-interleaved
  tick by design (every fire reads fully-settled positions vs a pre/post-move mix) — validated by
  determinism + lockstep, NOT stock-equivalence (a2_integration §3 / a2.0b).
- **The spatial-query short-lock+copy primitive** (`sim/spatial_query.h` + `sim/tests/spatial_query_test.cpp`):
  `SpatialQueryGuard::query_copy` makes {run query + copy the intrusive `+0x18` result list into a
  thread-local snapshot} atomic, then releases — the expensive per-candidate eval runs lock-free. Host gate:
  the copy is independent of later shared-buffer mutation (contract); 8 threads × 4000 guarded queries →
  corruption=0; a DETERMINISTIC interleaving negative control shows the unguarded shared `+0x18` walk tears
  (7/25 ids) — the guard is load-bearing. (Grid-frozen invariant is the caller's, upheld by the sweep barrier.)

**STILL REMAINING = the IN-GAME THREADED TAKEOVER (the next gated milestone, high-risk, Rule-6 sign-off).**
This is the irreducibly-in-game part — a live-tick control-flow takeover driving real worker threads — that
cannot be host-validated and per a2_integration §8 is the riskiest control-flow change in the project. The
precise plan (each piece, and why it's now tractable):
1. **Global-scratch localization** — the parallel fire pass runs the BINARY fire body per firer on N
   threads; the binary leaves' fixed-address global scratch (`399450`→`DAT_140b2c380..394`,
   `393b70`→`DAT_140b2ec18..30`, `35f470` fog→`DAT_140a28538/40/44`, §8.3) would race. Mechanism:
   entry-detour each scratch-using leaf to a per-thread implementation. `399e20`/`393b70` are ALREADY lifted
   thread-safe + bit-exact (DTFINT 473k/473k, return-by-value, `sim/firing_intercept`) so they drop in;
   `35f470` fog needs a thread-local-scratch detour (lift or per-thread arena).
2. **RNG** — the fire path's LCG draws (`1ffb40`/`1ffbb0`/`1ffdb0`) → per-entity `SimRng::substream` (built,
   invariant I2 gone). A determinism retrofit, already accepted (sim_rng.h).
3. **Spatial query** — wrap `2acb60`→`20e780` with `SpatialQueryGuard` (built, §8.10).
4. **The takeover seam** — at `t2be640` / the per-object walk, drive `PhaseA∥ lifted cheap-mass → barrier →
   PhaseB∥ binary fire body per firer (emits buffered via the b3 SpawnCommand + Class-2b + SfxCommand +
   the rare UI-queue channel §8.5) → barrier → PhaseC drain_parallel`. Kill-switch `EAW_PFIRE=1`
   default-OFF; **1-SHARD FIRST** (control-flow takeover with no real threads) before N-shard, mirroring a1.
5. **The gate** — replay-determinism + lockstep (NOT stock-equivalence — the retrofit delta §8.10/CLAIM 2);
   the structural invariants must hold under takeover (DTWA `idfail=0`, DTDRAIN `rank_down=0`, DTWA-B3
   source-map); the host `parallel_fire_test` determinism property is the in-host proxy the in-game run must
   reproduce. Expected ceiling ~3.5–6× (§8.2) vs the serial-fallback ~2× (a2.0).

⇒ **The RE + host-build side of the parallel-fire path is COMPLETE and determinism-gate-green; what
remains is the gated, sign-off-required live-tick threading integration (steps 1–5), which is its own
milestone and should roll out 1-shard-first behind `EAW_PFIRE`.**

### 8.11 IN-GAME TAKEOVER — STAGE A (identity scaffold) = PASS (2026-06-06)
First live-tick step of the step-4 takeover, mirroring milestone a1 ("control-flow takeover with no real
threads first"). Rule-6 sign-off taken before touching the tick. **Seam confirmed:** the per-object walk
is `FUN_1402be640`'s `master_update_list` sweep (decomp/2be640.c:64-71) calling `FUN_1403a6b80(obj,
param_2, cVar4)` once per object — one monolithic update that *interleaves* settle and fire. Inside
`3a6b80` the split point is: **settle half** = locomotor state-shift + behavior-update loop (`vfunc+0x30`)
+ transform + scene (2be640.c:37-261); **fire half** = `FUN_1403a76b0` (hardpoint fire body, 3a6b80.c:372)
+ the `flags&0x40000` effect-probability gate (`FUN_1403ab890`, 3a6b80.c:384). The future two-phase split
is: PhaseA = `3a6b80`-minus-fire ∥ → barrier → PhaseB = fire-body-only per firer ∥ → barrier → PhaseC
drain. NB the recharge block (3a6b80.c:360-370) reads only self → stays in PhaseA; only `a76b0`+`ab890`
(which read other objects' settled positions) move to PhaseB.

**Build (hooks/winmm_proxy.c, mirroring a1's call-site-repoint idiom):** `EAW_PFIRE` kill-switch
(default OFF; with it unset `install_pfire_hooks()` is never called and the image is byte-for-byte stock).
When armed, two E8 call sites inside `FUN_1402be640` are repointed (bodies intact, replay via saved real
pointers): the per-object `3a6b80` call (the walk; B's PhaseA collect/settle) and the final `2a62d0`
call (2be640.c:249; B's PhaseB/PhaseC flush point). **STAGE A interceptors are pure passthrough** — the
identity case, where bit-identity is still a valid oracle (it stops being one once the PhaseA/B split
introduces the intended retrofit delta, §8.10/CLAIM 2). justfile: `just pfire=1 launch-foc-desktop`.

**Gate result (in-game, EAW_PFIRE=1):**
- Seam interception assumption CONFIRMED: `a6b80(walk)=1 a62d0(flush)=1` site(s) repointed — exactly one
  per-object walk call and one flush site, as the restructure requires (not 0, not many).
- Release build: interceptors fire at full battle scale (walk=2.57M over 8193 flushes, ~397 obj/tick),
  no crash / c0000005 / VEH / FATAL. (`eaw-mt.log.pfireA-release`)
- Oracle build (`build-winmm-oracle`, EAW_DIFFTRACE=1 EAW_PFIRE=1): structural sim-ordering oracles stay
  bit-exact GREEN across multiple battles — `DTWA idfail=0 ctrfail=0` (I1 create-id allocator),
  `DTDRAIN rank_down=0` (canonical drain order), `DTB3SUM` all `N/0` (firer/tgt/sub/dmg/life — §3
  firing-body source-map). 0 crashes. (`eaw-mt.log.pfireA-oracle-pass`)
⇒ The call-site repoint of the `t2be640` seam does NOT perturb the create/order sim path — proven
in-game, exactly as a passthrough must. The seam STAGE B will own is plumbed + live-validated.

#### STAGE B (two-phase split, 1-shard) = PASS (EAW_PFIRE=2)
The retrofit. The fire half of the per-object update is `FUN_1403a76b0` (the hardpoint fire body,
3a6b80.c:372). A THIRD call-site repoint — the `a76b0` E8 **inside `3a6b80`'s body** → `pfire_a76b0_
intercept` — lets the settle walk run fire-suppressed: at level 2 the interceptor RECORDS `(firer, tick)`
and returns (no fire) ⇒ `3a6b80` settles only (**PhaseA**). The deferred fires are replayed IN WALK
ORDER at the `2a62d0` flush (**PhaseB**), after every object in that manager's walk has settled — the
barrier — so every fire reads the SETTLED world. The walk→flush pairing is guaranteed (2be640 has no
return between the walk and 2a62d0). PhaseB calls `a76b0` at its ENTRY, so it still passes through the
DT oracle entry-detours; DTDRAIN's rank is an incrementing per-tick counter stamped at each `a76b0`
entry, so walk-order replay keeps `rank_down=0`. 1-SHARD ⇒ no command buffer: the binary fire creates
projectiles inline in canonical (walk) order (= PhaseC for 1-shard); the b3 SpawnCommand buffer is the
later N-shard step. `g_pfire_a76b0_real` keeps the ENTRY (not a trampoline) so the b3a76b0 / DTWA-B3
oracles observe the deferred fires. justfile: `just pfire=2 launch-foc-desktop`.

**Gate result (in-game, EAW_DIFFTRACE=1 EAW_PFIRE=2):** the structural invariants HOLD under the
two-phase delta (validated by invariants, NOT stock-equivalence — fire now reads fully-settled positions
by design):
- Seam: `a6b80(walk)=1 a62d0(flush)=1 a76b0(fire)=1` repointed (the fire-body call site at 3a6b80.c:372
  found, exactly one).
- Two-phase balance PERFECT: `deferred=16272 fired=16272` (every suppressed fire replayed), `overflow=0`,
  `maxfill=8` (≤8 firers buffered per manager-walk, far under the 8192 cap), over walk=1.64M / 4097
  flushes — a full battle.
- Structural oracles bit-exact GREEN across multiple battles: `DTWA idfail=0 ctrfail=0`,
  `DTDRAIN rank_down=0`, `DTB3SUM` all `N/0`. 0 crashes (the deferred-fire use-after-free risk did not
  materialize — death-free is deferred to a later pass, so recorded firer pointers stay valid through the
  flush). (`eaw-mt.log.pfireB-oracle-pass`)
⇒ **The two-phase settle→barrier→fire restructure is live + correct at 1-shard** — the in-game analogue
of the host `parallel_fire_test` determinism property, now reproduced against the real engine. The
read-only fire phase needs no snapshot and no predicate lift; every fire reads the settled world and the
create/order invariants are preserved.

**NEXT = N-SHARD prerequisites then N-shard** (§8.10 steps 1-3 + the threaded takeover): (1) global-scratch
localization (scope refined in §8.12); (2) RNG → `SimRng::substream`; (3) the b3 SpawnCommand buffer +
canonical drain replaces inline create so PhaseB workers don't race the GOM; THEN run PhaseB on real
worker threads (N>1), re-running this same invariant gate + the host determinism property.

### 8.12 GLOBAL-SCRATCH LOCALIZATION — call graph confirmed + scope refined (2026-06-06)
Surveying the deferred-fire path (PhaseB: `a76b0`→…→`3825b0`) for the racy fixed-address scratch §8.3
listed. The firing body `3825b0` reaches exactly two scratch-using leaves:
- **`3825b0:97` → `FUN_14035f470` (FOG/detection, 567B):** rebuilds a SINGLE global heap buffer
  `DAT_140a28538` (ptr) / `DAT_140a28540` (count) / `DAT_140a28544` (flags) PER CALL — frees the prior
  buffer (35f470:31-41), repopulates via the sensor-grid query `FUN_14035dec0` (which appends through the
  result vtable `PTR_vftable_140a28530`), then iterates it (44-78). Two PhaseB workers would free/rebuild
  each other's buffer ⇒ RACE. Localize = per-thread result buffer (lift, or a TLS arena swapped around
  the 35dec0 query). Surface = 567B + the 35dec0 callee + the result vtable.
- **`3825b0:231` → `FUN_140399450` (FULL lead solver, 2512B):** its MAIN path writes+reads
  `DAT_140b2c380..394` as a within-call temp (399450:198-235); it delegates to the thread-safe `399e20`
  ONLY in fallback branches (209/252/265). ⇒ **REFINEMENT of §8.10 step 1: "399e20/393b70 drop in" does
  NOT cover 399450** — the main lead-math path uses `b2c380` directly and needs a FULL lift. **FURTHER
  finding (the lift is harder than "reuse firing_intercept"):** `b2c380..394` is not a plain temp — it is
  the **closure environment for a callback root-finder**. 399450:213-217 writes the scratch, then
  `FUN_1407d68e0(…, FUN_1403aba90, [DAT_1408007b4..DAT_1408007f8], …, 0xf)` (399450:219) runs a numeric
  solver that calls the objective `FUN_1403aba90`, which READS `b2c380/384/388/38c/390/394` back; 399450
  then reads `b2c380/384` again (222/234/235) to form the lead point. So localizing 399450 means
  reimplementing the solver objective `FUN_1403aba90` + driving `FUN_1407d68e0` from per-thread/local
  state (or TLS-backing the six globals, which the callback hardcodes) — NOT a thin wrapper over the
  lifted `399e20` (399e20 is only the close-range / degenerate fallback).
- **`393b70` (`b2ec18..30` scratch)** is reached ONLY via `399e20` (393b70 callers = 399e20 + itself), not
  directly by 3825b0 ⇒ localized for free once 399450's lift uses lifted `firing_intercept`. `399e20`
  itself is already scratch-free.

⇒ Global-scratch localization on the PhaseB path = **TWO per-function lifts** — `399450` (full lead solver
+ its `FUN_1403aba90`/`FUN_1407d68e0` root-finder) + `35f470` (fog, per-thread result buffer) — each a
decode → host bit-exact → in-game DTFINT-style oracle. Both are session-scale (399450 the larger, given
the callback-closure). **Two viable mechanisms (the §8.10 "lift OR arena" choice, now sharper):**
(a) **LIFT** each leaf to per-thread/return-by-value — speedup-preserving, but 399450 needs the objective +
solver reimplemented; (b) **LOCK** each scratch group (one mutex per group, entry-detour `lock();real();
unlock();`) — correct + cheap + unblocks N-shard immediately, but serializes those leaves. Which is right
depends on how much of the fire body's cost lives in 399450+35f470: if small, (b) costs almost nothing; if
large, (a) is needed to keep the ~p≈0.65 parallel fraction (a2.0). ⇒ **An a2.0-style per-leaf timing
measurement should gate the lift-vs-lock decision before either is built.**

**MEASUREMENT DONE → DECISION = LOCK (2026-06-06, `eaw-mt.log.a2scratch-pass`).** Extended the A2MEASURE
harness (EAW_A2MEASURE=1) with entry-detour QPC timers on `399450` + `35f470`, reported as a fraction of
the fire-control subtree `a76b0` (both are sub-calls of it, so locking them serializes that fraction of
PhaseB). Stable over 5×512-tick windows of a real space battle:
`scratch/fire = 0.069, 0.067, 0.064, 0.063, 0.062` (→ **~6.2%**), split `399450 ≈ 1.4%` + `35f470 ≈ 5.0%`.
⇒ The racy scratch is only ~6.2% of the parallelizable fire-control. **LOCK wins decisively:** two mutexes
(one per scratch group) serialize ≤6.2% of PhaseB ⇒ PhaseB stays at p≈0.94 → ~3.3× (N=4) / ~5.4× (N=8),
within the 3.5–6× target. Lifting would recover only that ~6.2%, and the most expensive piece — `399450`'s
solver+callback reimplementation (~2 sessions) — buys back a mere **1.4%**. (Had the leaves been a large
fraction the lifts would be mandatory; measuring first turned a ~2-session lift into a ~30-min lock with no
speedup loss.) The two timer entry-detours ran 5 battles with 0 crashes, confirming the 15-byte prologue
steals for 399450/35f470 are clean — a free pre-validation of where the lock detours will attach.
⇒ **Localization mechanism = per-scratch-group mutex; implementation folds into the N-shard build (a lock
is a no-op at 1-shard, only exercised/gated once PhaseB runs on N>1 threads).**

### 8.13 ⚠️ N-SHARD PRE-BUILD AUDIT — running the BINARY bodies on threads is BLOCKED on BOTH halves (2026-06-06)
**Rule-6 / Rule-4 FLAG (does NOT rewrite §8.10's step-4 plan — surfaces a verified blocker against it; the
N-shard branch HALTS here pending the architectural sign-off below).** Before sinking the N-shard build
(§8.10 steps 1-3 + the threaded takeover, the prior-session plan = "locks on 399450/35f470 → RNG substream →
swap inline create→SpawnCommand buffer → run PhaseB binary fire body on N>1 threads"), I audited whether the
STOCK binary per-object bodies can be executed concurrently at all. **They cannot — each half has an
independent showstopper, both confirmed in the current decompiles (provenance discipline, not memory):**

- **PhaseB (fire) `3a76b0`→`3825b0` — the create is INLINE-CONSUMED + the LCG is drawn INLINE.** The
  projectile is created at `3825b0:266` (`plVar12 = FUN_14029f810(*(mgr+0x2b8), …)`) and **immediately
  consumed in the SAME stack frame** using locals computed earlier this invocation: `3825b0:339`
  `lVar9 = plVar12[0x1d]` then `:340-455` write firer-id (`lVar9+0x58` ← `param_1.10+0x50`), damage
  (`+0x64`), lifetime (`+0x68`), velocity (`+0x6c`), target (`+0x08`←param_2), shroud (`+0x60`) etc. into the
  new object; plus `3825b0:285` SFX (`2d5240(…,plVar12+0xf,…)`), `:402` shot-register `3a06a0(…,plVar12)`,
  `39d6a0(plVar12)`. ⇒ the §8.10-step-3 "swap inline create → b3 SpawnCommand buffer + canonical drain"
  is **NOT applicable to the binary body** — you can't defer a create whose result the very next binary
  instructions write into (this is exactly the `inproc_integration_milestone.md` §0 inline-consume blocker;
  b3's resolution — "the whole consumed block becomes the SpawnCommand APPLIER", methodology #18 — lives in
  the LIFTED `sim/firing_spawn`, where WE own the structure; it does not retrofit onto the opaque binary
  frame). Additionally the spread RNG is drawn from the **shared global LCG** inline at `3825b0:211-216`
  (`FUN_1401ffbb0(&DAT_140a13e24)` ×3 + `1ffdb0`) and `:410` (`1ffb40`) — N concurrent fire bodies race the
  LCG, and even serialized-by-lock the draw ORDER is thread-nondeterministic (the §8.10-step-2 substream fix
  is a property of the LIFTED path; it can't be injected into binary draw sites). The §8.12 scratch locks
  (399450/35f470) are necessary but **address only ~6.2%** of the hazard surface — they do NOT touch the
  create-consume, the LCG, or the inline Class-2b writes (`220e90` `:350` / `3a06a0` `:402` → target `+0x38`
  listener), all of which also race/disorder under concurrency.
- **PhaseA (settle) `3a6b80` — runs the Lua AI coroutine pump.** `3a6b80:315` `FUN_140247a90(param_1->
  lua_ctx)` = `Pump_Threads` (the per-entity Lua resume). Threading the binary settle body therefore pumps
  Lua on multiple threads, which is the **already-proven-fatal Model B path** (`threading_model.md` Model B
  verdict: EaW's Lua C closures touch shared sim state during `lua_resume` → main-thread vtable corruption).

⇒ **Neither stock binary half is concurrently executable.** Real in-game N-shard therefore requires LIFTED
per-object bodies for whichever phase is parallelized — exactly the "needs lifted Phase-A bodies (Strategy 2,
large later effort)" the project named at `inproc_integration_milestone.md` §0 and `a2_integration_scope.md`.
The §8.12 measure-first→LOCK decision is correct AND sufficient for the scratch leaves but is **not the whole
N-shard cost**; the prior-session one-liner under-counted the create-consume / LCG / Lua blockers. **This is
an architectural fork for the user (Rule 6); the candidate paths:**
- **(A) Lift the fire-emit path** so PhaseB runs LIFTED on threads (drive a lifted fire DECISION feeding the
  already-host-built b3 `firing_spawn` applier + `firing_intercept` geometry; emit SpawnCommand/Command/Sfx
  to per-thread buffers; canonical `(gom,rank,seq)` drain). The genuine ~3.5-6× path — but it is the large
  lift the §5 gate's "serial-fire fallback" specifically deferred (the fire DECISION is the b1 vfunc/sensor
  graph; under the read-only-phase reframe §8 it needs no snapshot, but the inline writes still must be
  extracted to the lifted applier).
- **(B) Lift the cheap-mass settle bodies** so PhaseA runs LIFTED on threads (locomotor/Pass-C/sensor are
  already lifted), keep fire serial (the a2.0 fallback ~2×). Requires splitting `3a6b80` into {lifted-parallel
  cheap-mass} + {serial Lua pump + any un-lifted behavior} — the never-run a2.1 "enumerate `3a6b80` sub-calls"
  step, gated by the Lua-must-stay-serial constraint.
- **(C) Treat STAGE B (1-shard two-phase, §8.11) as the shipping integration result** — it is live, correct,
  invariant-validated, and the host `parallel_fire_test`/`shard_scheduler_test` pipeline proves the full
  parallel design; declare in-game multi-thread blocked-by-design on binary-body non-threadability and pivot.

Methodology banked (23): before a "wire up the threads" build, audit whether the code you intend to run
concurrently can run concurrently AT ALL — here a 10-minute decompile read of the two bodies (`3825b0`
inline-consume + LCG, `3a6b80` Lua pump) showed both stock halves are non-threadable, converting a planned
multi-session threaded-takeover build into a one-decision architectural fork BEFORE any risky live-tick code.

### 8.14 PATH A (lift the fire-emit path) — A1 decode/catalog + increment plan (2026-06-06)
**User picked path A (§8.13 fork): the genuine ~3.5-6× ceiling — lift the fire-control body so PhaseB runs
LIFTED on N threads.** A1 is the b1-style risk-free decode: the full `3825b0` body (`decomp/3825b0.c`,
re-read end-to-end) cataloged into three regions by the §8.13 threading verdict, each leaf classified
{binary-read-safe-on-stable | lifted-sim (already built) | deferred-write (command buffer) | self-write
(object-granular, on-thread)}. **Strategy = A-REIMPL** (faithful hook reimplementation of `3825b0`'s control
flow, delegating heavy READS to the binary leaves — pure on stable post-barrier state per the §8.5 write-audit
+ §8.7 query-vfunc spot-check — math to lifted `sim/`, and routing the 3 cross-entity WRITES + SFX to the
command buffers). A-reimpl is the only strategy that yields cleanly deferrable writes for true N-shard; the
alternative "run the binary + intercept its writes" (A-provisional) fails because the binary inline-consumes
the create result and emits the Class-2b writes mid-frame on it (§8.13). The b3 host build already lifted
Region 2 (the applier); DTWA-B3 validated it bit-exact in-game.

**REGION 1 — DECISION + GEOMETRY (`3825b0:60-260`): all READS → run on-thread on stable state.**
- Gate cascade: `:71` self-alive `param_2+0x74&0x40`; `:74` `vfunc_2(tgt,0x11)` (=`395ac0`, pure, §8.7);
  `:78` `39b140`; `:93` `540140`; `:97` `35f470` FOG (⚠ scratch `a28538..` → §8.12 LOCK); `:104` `39a540`;
  `:108-109` `398440`+`394a80` target sub-resolve; `:112-145` firing-ARC gate (`385cf0` bone/`12d2c0`/
  `139800` normalize/dot — DEAD in tactical, `+0x4e==0`, per I3, but transcribe the branch); `:158` `397e00`
  aim-point resolver; `:161-162` `39b950` flags; `:165-173` `398440`+`vfunc_2(0x16)`+`405870` (UnitAI
  target redirect); `:174` `383f70` / `:183` `385c70` aim point; `:201` `385e70` LOS; `:203-209` `776d48`
  sqrt range-check + `3857d0` + `397780` range gate; `:230` `370f00`; `:231` `399450` FULL LEAD SOLVER
  (⚠ scratch `b2c380..` → §8.12 LOCK; reaches lifted `399e20`/`393b70`-linear in fallback, DTFINT-clean);
  `:238` `383ba0` aim-valid; `:211-225`+`:253` `381dc0` SPREAD (⚠ LCG `a13e24` → `SimRng::substream`);
  `:258` `20acd0` dir→Euler (✅ lifted `sim/targeting_aim`).
- **Lifted-sim available:** intercept lead `firing_intercept` (399e20/393b70-linear, DTFINT 473k/473k),
  spread `firing_apply_spread` (381dc0, RNG-retrofit), dir→Euler `targeting_aim` (20acd0).

**REGION 2 — CREATE + INIT + Class-2b (`3825b0:261-402`) = the b3 APPLIER (✅ already lifted = `sim/firing_spawn`).**
- `:266` `29f810` create (Class-2 SpawnCommand, I1 id-order); `:285` `2d5240` muzzle SFX (Class-3 SfxCommand,
  a1 pattern); `:339-400` `proj+0xe8` init — firer `+0x58`/dmg `+0x64`/life `+0x68`/vel `+0x6c`/target `+0x08`/
  guided-vs-ballistic transform `+0x24..+0x90` (✅ `ProjectileInit`, DTWA-B3 bit-exact 362/0); `:350` `220e90`
  target `+0x38` listener (Class-2b Command — depends on the created proj's `vfunc_2(proj,8)` → folds into the
  applier, methodology #18); `:402` `3a06a0` shot-register (Class-2b Command — firer record + global shroud
  mgr `b15418`). All three cross-writes depend on `plVar12` ⇒ they ARE the applier, drained in `(gom,rank,seq)`.

**REGION 3 — FIRER COOLDOWN/AMMO SELF-UPDATE (`3825b0:403-465+`): SELF-WRITES (object-granular → on-thread).**
- ⚠ **NEW (not in b3's scope, which was create+init only):** `:403-406` ammo counter `param_1+0x5c`;
  `:410` LCG draw for recharge jitter (⚠ `a13e24` → `SimRng::substream`); `:412/439/450/455/459` cooldown
  timer `param_1+0x58`; `:417` shroud frame `param_1+0x68`; `:418-421` `285d70` deregister + `param_1+0xc8`;
  `:423-456` cooldown scaling (`374b50`/`39b950`/`398010`/`535cb0`/`395c70`/`33fb70`). All writes are to
  `param_1` (the firer's own hardpoint fire-control component) ⇒ Phase-A-safe by object-granularity (1 firer =
  1 shard); only the LCG draw needs the substream retrofit. Needs its OWN in-game oracle (cooldown/ammo
  bit-exact) since b3's DTWA-B3 only covered Region 2.

**INCREMENT PLAN (mirrors I1-I5 / b1-b3 discipline; reassessment gate after A3):**
- **A1 = this decode/catalog/strategy.** ✅
- **A2 (host) = `sim/firing_control.{h,cpp}`** — the Region-1 decision/geometry skeleton (providers injected
  for the binary read-leaves) producing a `SpawnCommand` via the built `firing_spawn`, + the Region-3 cooldown
  self-update (RNG via `SimRng::substream`). Host gate: recorded inputs reproduce the b3 payload + a
  cooldown-determinism check; reuses firing_spawn/firing_intercept/targeting_aim. No launch risk.
- **A3 (in-game, 1-shard, `EAW_PFIRE=3`) = emit-redirection takeover** — in PhaseB, replace the binary
  `3825b0` call with `firing_control` (still no real threads): compute on-thread, buffer SpawnCommand +
  Class-2b + SFX, drain at the `2a62d0` flush in walk order via the lifted applier (which calls binary
  `29f810` then replays the init). Gate (the STAGE-A-analogue for path A — control-flow takeover, no threads):
  DTWA-B3 bit-exact (projectiles field-identical to binary), DTWA `idfail=0`, DTDRAIN `rank_down=0`, a NEW
  cooldown self-write oracle (`param_1+0x58/0x5c` identical), buffer balance, 0 crashes. **REASSESSMENT GATE:
  if the reimpl reproduces the binary bit-exactly, A4 is mechanical; if a branch diverges, fix/reassess.**
- **A4 (in-game, N-shard, `EAW_PFIRE=4`) = thread PhaseB** — run `firing_control` on N workers (per-thread
  `ShardBuffer`), §8.12 scratch LOCKS on `399450`/`35f470`, RNG substreams, `SpatialQueryGuard` on the
  opp-scan (§8.6), canonical drain. Gate: replay-determinism + lockstep (NOT stock-equivalence, the §8.10
  retrofit delta) + all structural invariants hold + the host `parallel_fire_test` property reproduced;
  measure the actual speedup (target ~3.5-6×, a2.0 `p≈0.65`).

**A3 BUILD FOUNDATIONS (decided 2026-06-06, user picked "straight to A3"):**
- ⚠️ **The hook is C, not C++** (`hooks/winmm_proxy.c` built by `x86_64-w64-mingw32-gcc`, NOT g++; the
  `sim/` lifts are host-only g++ for `just sim-test`). So the A3 reimpl **cannot call `firing_spawn.cpp` /
  `firing_intercept.cpp` directly** — their logic is transcribed into C in the hook. The existing **DTWA-B3
  oracle (winmm_proxy.c ~4048-4400) already C-transcribes the §3 `proj+0xe8` field computation** (recompute-
  and-compare) ⇒ that is the seed for the create+init applier; promote it from "recompute for comparison" to
  "compute for the buffered create".
- **A-reimpl = ORCHESTRATION, not full leaf reimplementation:** the reimpl reproduces `3825b0`'s control flow
  + the glue FP arithmetic (faithful grouping), but CALLS the ~40 binary leaves (resolved by RVA) for the
  heavy reads — we do NOT reimplement `374b50`/`39b950`/`398010`/`535cb0`/`395c70`/`33fb70`/`397e00`/`383f70`
  /`385e70`/`399450`/`381dc0`/etc. internally.
- ⚠️ **OBSERVE-FIRST sub-staging (the STAGE-A-analogue, to isolate transcription risk from takeover risk):**
  before flipping the control-flow takeover, run the reimpl ALONGSIDE the binary (entry-detour `3825b0`, let
  the binary run, then recompute the reimpl's outputs on a pre-call snapshot and compare bit-for-bit, like
  DTFINT/DTREVEAL/DTWA-B3) until it matches across a full battle. **CAVEAT — orchestration idempotency:**
  re-calling a binary leaf in observe-mode is only safe if the leaf is side-effect-free on shared state
  (pure read); leaves that mutate (`535cb0`/`535fb0` build/destroy a stack temp = local, OK; but verify each
  before re-calling). Where a leaf is not safely re-callable, snapshot its result during the binary's own
  call (piggyback) rather than re-invoking — the DTWA-B3 `5400f0`-cache-read trick (methodology #18).
- **Build chunks (compile-gated by `build-winmm-oracle` with NO launch; then launch-validated):** (A3.0)
  binary-leaf scaffolding (RVA `#define`s + fn-ptr typedefs + resolution); (A3.1) R3a cooldown reimpl +
  observe oracle [self-contained, own-object writes, the "new cooldown oracle" the gate needs]; (A3.2) R1
  decision/geometry skeleton + create-args + R2 init (promote DTWA-B3 transcription) + observe oracle for the
  create args/init; (A3.3) flip to takeover `EAW_PFIRE=3` (buffer + drain at the `2a62d0` flush via the
  applier) + the full A3 gate. Each chunk compiles green before the next; launch cycles validate (user drives
  a space battle — fire-path data needs combat, per §8.12 launch note).

### 8.15 PATH A — R2 create-args + R1 gate-1/gate-2a OBSERVE oracle = IN-GAME PASS (2026-06-06)
Built the first three A3 chunks in `winmm_proxy.c` (observe-only, EAW_ORACLE, wired into the existing
DTWA-B3 `3825b0` entry-detour) and launch-validated them in two live space battles. **The verify-first
checkpoint before the buffer-passing gate-2b.**
- **R2 create-args capture** (the §8.14 applier's create inputs): `PfireSpawnArgs` filled from the existing
  `29f810` detour params on the firing-window piggyback; validated `mgr == *(owner+0x2b8)` (re-confirms the
  I2/Int-#1 manager-resolution) + `template == proj+0x298`.
- **R1 gate-1** (`pfire_r1_gate1`, `3825b0:60-105`) + **gate-2a** (`pfire_r1_gate2a`, `:107-163`): the
  RNG-free decision-gate prefix, transcribed by CALLING the binary gate leaves (`39b140`/`540140`/`35f470`/
  `39a540`/`vfunc_2`; `398440`/`394a80`/`397e00`/`39b950`) on the PRE-call state; types mirror the
  decompile (`p2`=`longlong*`→`+N` is ×8). One-directional oracle: a fired shot (binary `r==1`) MUST pass
  every gate ⇒ `bug` counts the impossible `r==1 & verdict==0`.
- **RESULT (multi-battle, `eaw-mt.log` DTR1SUM/DTB3SUM, ~1199 fires / 413k fire-body calls):**
  `g1_bug=0`, `g2_bug=0`, `arg_mgr=1199/0`, `arg_tmpl=1199/0`; all prior DTWA-B3 fields still `N/0`
  (firer/tgt/sub/dmg/life); **0 crashes**, game stable across two battles. ⇒ the transcription FIDELITY —
  pointer types, byte offsets, leaf arities (incl. the resolved `398440` 2-arg, the omitted side-effect-only
  `:165` call), and the pre-call re-invocation of the binary leaves (incl. `35f470`'s idempotent global fog
  rebuild) — is **correct on live data and does not perturb the sim**.
- **Honest coverage limitation:** the gate REJECT paths were barely exercised — `gate1` returned 0 only ~2×
  (the caller `387400` pre-selects a valid target, so `3825b0` rarely sees an invalid one), and `gate2a`'s
  reject (the targeting compound) fired 0× (`397e00` resolved for every gate-1 passer). So the oracle proves
  "no false-rejection of a fired shot" but only lightly exercises the rejection logic. The dominant per-tick
  "don't fire" filter is the range/LOS gate in the UNtranscribed gate-2b (`164-209`) — `g1_nofire≈g2_nofire`
  ≈412k confirms nearly every call passes the early gates and is blocked downstream. Reject-path coverage
  improves once gate-2b (range/LOS) lands and with target-death/fog scenarios. Methodology banked (24): a
  one-directional `r==1 ⇒ verdict==1` observe oracle cleanly proves transcription FIDELITY + crash-safety on
  the fire path, but does NOT exercise the reject branches when an upstream caller pre-filters — count the
  `(r==0 & verdict==0)` rejections to see whether the reject logic was actually tested (here ≈2, i.e. barely).

**✅ gate-2b ADDED + IN-GAME PASS (2026-06-07, evidence `eaw-mt.log.gate2b-validate`).** `pfire_r1_gate2b`
transcribes `3825b0:164-209` (aim-point + LOS + range, the dominant per-tick fire/no-fire filter), the first
chunk passing scratch BUFFERS to binary fns. **Leaf-safety vetted FIRST (caught two RNG perturbation traps
before launch): `405870` (UnitAI target redirect) AND `383f70` (aim, `:121`) BOTH draw the global LCG
(`1ffb40`) → re-invoking them observe-only would shift the sim's RNG stream; so the `param_3==0` aim path is
SKIPPED (returns -1, validated at the A3.3 takeover instead). The `param_3!=0` path (`385c70` aim + `385e70`
LOS + `3857d0`/`397780` range) is RNG-free + shared-write-free (verified) ⇒ safe to re-invoke.** `385e70`
writes `param_2[0..2]` (vec3) + `param_3[0..0xb]` (3×4) per `decomp/385e70.c` → buffers over-sized to 64B;
matrices init from `DAT_140800820` (48B). **RESULT (2 battles, ~362 fires / 264k calls): `g3_ok=362`,
`g3_bug=0`, `g3_skip=0` (param_3 always resolved → full coverage, no RNG path hit), `g3_nofire=115680`
(gate-2b correctly REJECTED ~148k range/LOS-failing calls — the reject path now HEAVILY exercised, the
coverage gate1/gate2a lacked); sim-invariant (DTB3 fields N/0, DTWA `idfail=0/ctrfail=0`, DTDRAIN
`rank_down=0` ⇒ the buffer re-invocations did NOT perturb the sim); 0 crashes (64B buffers correctly
sized).** ⇒ **the full R1 DECISION transcription (`60-209`, gate1+gate2a+gate2b) is COMPLETE + in-game
validated** — the RNG-free fire/no-fire verdict is reproduced with zero false accept/reject of any fired
shot and the dominant reject filter proven discriminating + correct. Methodology banked (25): for a
buffer-passing observe chunk, vet EACH re-invoked leaf for (a) RNG draws and (b) shared-state writes BEFORE
launching — `383f70`/`405870` hid `1ffb40` draws that a launch-without-vetting would have silently turned
into sim perturbation; skip the RNG-tainted branch (validate it at takeover) and size output buffers from
the leaf's decompiled writes (385e70 = vec3+3×4 → 64B).

**NEXT = R1c geometry (`3825b0:211-260`: spread `381dc0` [LCG→substream] + lead solver `399450` [locked
scratch] + `20acd0` dir→Euler — the RNG-tainted geometry, validated at takeover not observe) → R2 applier
`pfire_apply_spawn` (`261-402`) → R3 cooldown (`403-490`) → A3.3 takeover flip (`EAW_PFIRE=3`).**

**✅ R1c + R2 applier + R3 cooldown TRANSCRIBED + unified into `pfire_fire_reimpl` (commits 4403307→de771bd,
compile-green; R1c/R2a takeover-validated note carried from the gate-2b observe pass).** The reimpl mirrors
the whole firing body as one function — decision inline (gate1+gate2a+gate2b-common) → R1c geometry → R2a/R2b
applier → R3 cooldown — returning 0 (no-fire) / 1 (fired) / `PFIRE_FALLBACK` (2 = run the stock binary).
Safe-rollout design: handles only the common tactical path; falls back to the binary for the rare/dead edges
(the `owner_type+0x4e==1` arc gate, and the `param_3==0` RNG aim path that in-game data showed never occurs
tactically, `g3_skip=0`) — and it bails BEFORE any RNG draw on those, so the binary fallback re-runs with no
LCG perturbation. Each binary leaf called exactly once (binary parity, no double-call).

### 8.16 PATH A — A3.3 TAKEOVER WIRED (compile-green, EAW_PFIRE=3) — 2026-06-07
The takeover flip is wired into the existing DTWA-B3 `3825b0` entry-detour (`dtwa_b3_3825b0_hook`): when
`pfire_on() >= 3`, the hook calls `pfire_fire_reimpl(p1,p2,p3)` instead of the binary trampoline; on
`PFIRE_FALLBACK` it calls the binary. Default-OFF, 1-shard, gated behind `EAW_PFIRE=3` (mirrors STAGE A/B).
Because level≥2 already DEFERS the fire to the `2a62d0` flush (`pfire_a76b0_intercept`), every `3825b0` call
under takeover runs in **PhaseB on the settled world** — A3.3 is STAGE-B-two-phase + the reimpl body, exactly
the seam A4 threads. **Validation comes for free:** at takeover the reimpl's R2a creates the projectile via
the binary `29f810`, so the DTWA-SPAWN piggyback captures it and `dtwa_b3_check` auto-validates its §3 fields
(firer/tgt/dmg/life/sub/vis — all RNG-free, so still a bit-exact oracle even though R1c-onward geometry is
RNG-tainted). **Two oracle-interactions handled:** (1) the R1 gate-verdict observe (g* vs binary r) is gated
OFF at `pf>=3` — r is then the reimpl's own output so the compare is tautological, and the reimpl already
calls those leaves once (re-invoking would double-call, incl. the global `35f470` fog rebuild); (2) DTR1SUM
prints a takeover marker instead of the now-frozen gate counters. Compile-green via `build-winmm-oracle`.
**LAUNCH GATE (the A3 reassessment gate, §8.14):** DTB3SUM all `N/0` (reimpl projectile field-identical to
binary) + DTWA `idfail=0/ctrfail=0` + DTDRAIN `rank_down=0` + sane fire cadence + low/zero fallback rate +
0 crashes ⇒ A4 is mechanical; if a field diverges, fix/reassess. Needs a live space battle (fire-path data
needs combat) — `just pfire=3 difftrace=1 launch-foc-desktop` (or the oracle-launch recipe).

**⚠️ SUPERSEDED — this "PASS" was a FALSE PASS; see §8.19. The field oracle could not tell a reimpl-create
from a binary-fallback-create, so it passed while the reimpl was silently no-op'ing/falling back and player
ships under-fired. Treat the claim below as NOT validated.**

**✅ A3.3 TAKEOVER = IN-GAME PASS (2026-06-07, `EAW_PFIRE=3` oracle build, multi-battle).** The reimpl drove
the firing body live and produced projectiles field-identical to the binary's §3 source-map — the A3
reassessment gate is MET. Evidence over the full run (`walk=2,465,749 / flush=45,057` across multiple
battle loads):
- **DTB3SUM every window all `N/0`, zero `DTB3MISS`:** `firer`, `tgt`, `dmg`, `life`, `arg_mgr`, `arg_tmpl`
  all bit-exact (e.g. per-battle `n=3225 → 3225/0` on each); `sub=11/0` on the measurable sub-id cases,
  `sub_reasn`=majority (the null→resolved param_3 path, counted not asserted — same as the §8.9 baseline).
- **Sim-ordering invariants intact under takeover:** DTWA `idfail=0 ctrfail=0` over 48,164 creates (I1
  id-allocator), DTDRAIN `rank_down=0` over 24,790 transitions (canonical drain order). The reimpl's inline
  R2a create (via binary `29f810`) lands in walk order = PhaseC for 1-shard, so the create/order path is
  unperturbed.
- **Two-phase balance perfect:** `deferred=854,184 fired=854,184`, `overflow=0`, `maxfill=79` — every
  deferred fire replayed at the flush, far under the 8192 cap. **0 crashes/faults** in the whole log.
- **Fallback effectively zero:** `skip=0` (the `param_3==0` RNG-aim fallback never triggered tactically,
  confirming §8.15's `g3_skip=0`); the only other fallback (`owner_type+0x4e==1` arc gate) is I3-dead. ⇒ the
  reimpl handled the tactical path ITSELF for all ~854K fire calls, not via binary fallback.
- **Coverage gap (documented, same as §8.9 DTWA-B3):** `vis=0/0` and `charge=0` — no vis-frame
  (`owner_type+0x4a4 > 0`) or charged shots occurred in these battles, so those two §3 branches stay
  in-game-unconfirmed (validated in host `firing_spawn_test` only). No direct reimpl-vs-fallback counter
  exists (a DTB3 projectile passes the check whether reimpl or binary created it) — `skip=0` + the I3-dead
  arc gate are the evidence fallback≈0; a future build could add an explicit fallback tally for rigor.

⇒ **The full firing body now runs from the lifted C reimpl in-game (1-shard, two-phase), bit-exact on every
measurable §3 field with all structural invariants preserved.** A4 = thread PhaseB: run `pfire_fire_reimpl`
on N workers with per-thread `ShardBuffer` (b3 `SpawnCommand` replacing the inline create), §8.12 scratch
LOCKS on `399450`/`35f470`, RNG→`SimRng::substream`, `SpatialQueryGuard` on the opp-scan, canonical drain;
gate = replay-determinism + lockstep + these same invariants + measured speedup (target ~3.5–6×).

### 8.17 A4.0 — THREADED-PATH PRE-BUILD AUDIT (2026-06-07): the "thread the reimpl" plan UNDER-COUNTS — the binary opp-scan `387400` runs on the threads too
**Methodology #23 applied again (the §8.13 discipline): before writing any thread code, audit whether
everything that runs in PhaseB can run concurrently.** At A3.3 the reimpl replaced ONLY `3825b0`, but the
PhaseB unit deferred at STAGE B is the whole per-ship fire body `a76b0` → `387010` → `387400` → {opp-scan;
`3825b0`}. So threading PhaseB runs the **binary `387400` opportunity-scan** on N threads — and it carries
hazards the §8.14 A4 one-liner did not list (all confirmed against `decomp/387400.c`, not memory):

| Hazard | site | class | why it bites on N threads |
|---|---|---|---|
| global-LCG random-start draw `1ffb40(&a13e24,0,iVar4-1)` | `387400:250` | RNG | races the shared LCG AND is draw-order-nondeterministic across threads → breaks DETERMINISM (a lock serializes but does NOT make it reproducible). Inside **binary** `387400` ⇒ a substream cannot be injected without LIFTING the scan. |
| spatial query / per-candidate eval `385190` | `387400:273` | shared scratch | the §8.6 intrusive `+0x18` result-list hazard → needs `SpatialQueryGuard` (built). |
| target-listener set `382510`/`3846c0` on `param_1->opp_target_slot` | `387400:285-294` | Class-2b | cross-entity listener edit on the target → must buffer. |
| post-fire signal `220ed0(…,0x21,…)` OpportunityTargetAcquired | `387400:314` | Class-2b | depends on the fire RESULT `cVar6` (`387400:292-293`) ⇒ entangles the scan's post-logic with the fire's return. |

**The opp-scan is THROTTLED** (`387400:239` — re-acquire only every `b0a340·8007c0` ticks, or when no
current target), so it is a periodic re-acquisition, not per-fire; the per-fire body `3825b0` runs far more
often. **This reshapes A4 into a fork (Rule-6, mirroring §8.13):**
- **(A) Lift the opp-scan too** (`387400` scan + `385190`) into the reimpl alongside `3825b0`: substream the
  start draw, `SpatialQueryGuard` the query, buffer the Class-2b target-set. Full ceiling (~3.5–6× on the
  whole fire-control), but `385190` is the §8.9 "expensive part" (spatial query + full gate-predicate eval)
  — a materially larger lift, and the scan/fire post-logic entanglement (`387400:292-315`) must be untangled.
- **(B) Keep the throttled opp-scan SERIAL** (run it in the PhaseA walk), thread ONLY the per-fire body
  `3825b0`/reimpl in PhaseB: sidesteps ALL four `387400` hazards. Smaller, safer. ⚠️ Mechanism is non-trivial
  — `387400:292` consumes `3825b0`'s return `cVar6` for its post-fire branch (`:293-315`), so deferring the
  fire out of `387400` strands that logic; needs a finer split than the current `a76b0`-level deferral.
  Ceiling is gated by how much PhaseB cost is the per-fire body vs the throttled scan.
- **(measure-first, recommended)** an a2.0-style per-leaf timing split of the throttled opp-scan
  (`387400:238-289`) vs the per-fire body (`3825b0`) over a real battle — the SAME measure-first that turned
  §8.12's 2-session lift into a 30-min lock. If the per-fire body dominates aggregate PhaseB time, (B) is the
  clear win (most of the speedup for a fraction of the build + risk); if the throttled scan dominates, (A) is
  mandatory. Decide A-vs-B on that number before sinking either build.

⇒ **A4.0 = this audit. A4.1+ awaits the A-vs-B fork decision.** Independent of the fork, the structural
prerequisite is the same first build step: **create-deferral + canonical drain at 1-shard** (the reimpl emits
a C-transcribed `SpawnCommand` to a buffer instead of the inline `29f810`; the flush drains create+init+
Class-2b in `(gom,rank,seq)` order). That removes the §0 inline-consume blocker on the threaded path and is
fully 1-shard-validatable (DTWA-B3 `N/0` + DTDRAIN `rank_down=0`) before any real threads — so it can proceed
while the A-vs-B measurement is taken.

**USER PICKED "prereq + measure together" (2026-06-07): build the 1-shard create-deferral AND the
scan-vs-fire timer in one cycle.**

### 8.18 A4.1 — create-deferral (1-shard) + scan-vs-fire timer BUILT, compile-green (2026-06-07)
Two pieces in `hooks/winmm_proxy.c`, gated `EAW_PFIRE=4` (default-OFF; release ⇒ level-4 == STAGE B since the
buffering reimpl needs the oracle-only DTWA-B3 hook):
- **Scan-vs-fire timer (the A4.0 fork measurement, no new prologue):** QPC the whole fire subtree `a76b0`
  (inclusive) in the flush-replay loop + the fire body (`3825b0`/reimpl) inside `dtwa_b3_3825b0_hook` (inner
  reimpl call AND whole-hook span, so the oracle-validation overhead `val` is reported separately, not
  confounding the split). `PFIRESPLIT` line reports `fire/fc` (B-relevant — fraction already threaded) and
  `scan/fc` (A-relevant — fraction fork B leaves serial as the throttled opp-scan).
- **Create-deferral (the structural prereq):** at level>=4 the reimpl computes payload+geometry and BUFFERS a
  `PfireSpawnRec {p1,p2,p3,lVar7,S[48],rank,objid}` (capturing the firing `a76b0`'s canonical DTDRAIN key)
  instead of calling R2a/R2b inline; buffer-full falls back to inline (never drop a shot). PhaseC drain
  (`pfire_drain_spawns`, at the `2a62d0` flush AFTER the walk replay) applies create+init+Class-2b (R2a+R2b)
  in insertion order (== ascending rank == canonical at 1-shard, no sort yet). **DTDRAIN coupling handled:**
  the creates move out of the `a76b0` window into the post-replay drain, so the drain re-stamps `g_drain_cur_*`
  from each record + resets `have_prev` (a fresh canonical sequence distinct from the replay-phase non-firing
  creates) so `rank_down` is evaluated on the drain order; `g_b3_in_fire` is set around R2a so the DTWA-SPAWN
  piggyback captures the deferred proj+args and `dtwa_b3_check` validates each drained projectile's §3 fields
  (the A3.3 free-validation, relocated into the drain). R3 cooldown (self-write + Class-2b rebind) stays inline
  in Phase-A at 1-shard (A4.3 buffers the rebind for N-shard). Compiles clean oracle+release+profile.
**LAUNCH GATE (1-shard, the deferral must be a structural no-op):** DTB3SUM all `N/0` + DTB3MISS=0 (drained
projectiles field-identical) + DTWA `idfail=0/ctrfail=0` + DTDRAIN `rank_down=0` (canonical drain order
preserved through the buffer) + spawn-defer balance (drained≈deferred-fires, overflow=0) + 0 crashes; plus the
`PFIRESPLIT` numbers to decide the A-vs-B fork. Recipe `just pfire=4 difftrace=1 launch-foc-desktop`.

### 8.19 ⛔ A4.1 IN-GAME = FAIL → exposed a PRE-EXISTING reimpl firing bug; A3.3 PASS was a FALSE PASS (2026-06-07)
**User-reported regression at `pfire=4`: capital ships/frigates dealt/took no damage; on inspection
player-controlled (New Republic) ships barely fire while AI (Imperial) ships fire fine.** Root-caused with
added diagnostic counters (`g_pfdbg_*`: reimpl entry / fallback-arc / fallback-p3 / r1c-fail / reachedR2,
+ the spawn-defer balance):
- **The PFIRESPLIT fork measurement DID land (valid):** `fire/fc ≈ 0.17–0.25`, `scan/fc ≈ 0.74–0.84` over a
  full battle ⇒ the per-fire body `3825b0` is only ~20% of the fire-control subtree; the throttled opp-scan +
  dispatch dominate (~80%). **Fork implication: Path B (thread only the fire body) ⇒ low ceiling; Path A
  (lift the opp-scan too) is required for the real speedup.** (Recorded for when the firing bug is fixed.)
- **The create-deferral was DORMANT** — `buf=0`, `drain_maxn=0`, `drainenter>0` (drain always empty): the
  reimpl **never reached R2 (the create step)** in the broken scenario, so the buffer was never filled. ⇒ the
  create-deferral CODE is not the bug; it is a red herring (reverted to A3.3 inline create, diagnostics kept).
- **The real bug: the reimpl REJECTS ~all fires upstream of R2.** Exit distribution @pfire=4 (725,341 reimpl
  calls): `r1cfail=475,661 (66%)` + gate-0 `≈249,680 (34%)`, `fb_arc=0`, `fb_p3=0`, **`reachedR2=0`**. Nothing
  fired via the reimpl; the validated `n` came entirely from the binary fallback path or (when fallback was 0)
  nothing fired at all. The asymmetry (AI fires, player barely fires) points at a **state the reimpl reads at
  the deferred-flush replay that differs for player-controlled firers** — most likely the STAGE-B two-phase
  delta (the reimpl reads SETTLED order/target state at the flush, not the mid-walk state the binary read),
  hitting an order/aim gate or the unvalidated R1c geometry (lead solver `399450` / fire-gate `383ba0`).
- **⚠️ A3.3 (§8.16) was a FALSE PASS.** Its gate was field-correctness (DTB3SUM `N/0`) + invariants, which
  CANNOT distinguish a reimpl-create from a binary-fallback-create — exactly the "no reimpl-vs-fallback
  counter exists" gap §8.16 itself flagged. The reimpl likely **never fired its own projectile**; A3.3's `n`
  was the binary fallback all along, and the reduced player-ship fire rate was invisible to a field oracle.
  **Methodology #26 (the hard lesson): a takeover oracle MUST count "did the LIFTED path actually drive the
  effect" (reimpl-reachedR2 vs fallback) AND compare the takeover's FIRE RATE / decision distribution against
  the binary baseline — field-correctness of whatever DID fire is necessary but NOT sufficient; a lifted body
  that silently no-ops (falls back / rejects) passes a field oracle while breaking the game.**

**Status: reverted to A3.3 inline create (create-deferral disabled), diagnostic counters retained, builds
green (oracle+release). The reimpl takeover (pfire>=3) is KNOWN-BROKEN for player firers — do NOT treat A3.3
as validated. Stock play (pfire unset) is unaffected.** NEXT (the real fix, next focused effort): localize
the rejecting gate/R1c via the ungated `PFIRE-C` counters at pfire=3 (now visible at level>=3) + an AI-vs-player
firer split; the prime suspect is the STAGE-B settled-state read or the never-observe-validated R1c geometry.
Re-validate with a PROPER oracle: reimpl fire-rate ≈ binary fire-rate per firer-faction, not just field N/0.

### 8.20 ROOT CAUSE FOUND + 2 fixes; create-geometry has a CLASS of Ghidra-args traps (2026-06-07, 9f91d65, 909e4b8)
Localized with in-game exit-stage counters + value dumps (PFLEAD lead I/O, PFPOS proj-vs-firer position).
The reimpl rejected 100% of fires at the **R1c lead-zero check** (`r1cfail=80,199=100%`, `reachedR2=0`). Cause
+ fixes, both in the never-observe-validated R1c geometry:
- **(1) `370f00` is Ghidra-`void`/undefined4 but RETURNS A FLOAT (projectile speed) in XMM0**, and `399450`'s
  `param_6` is a **float** lead-multiplier (`399450:214 local_104 * param_6`). Transcribed as int ⇒ leftover-RAX
  garbage into a float arg ⇒ `lead = x*0 = 0` ⇒ never fires. **FIX: `370f00`→float, `399450` param_6→float,
  `rec+0x90` stores float bits.** In-game: `reachedR2 0→16,364`, `lead0 100%→~0`, DTB3SUM all `N/0`, ships fire.
- **(2) `ffbb0` (spread draw) is really `ffbb0(LCG, lo, hi)`→uniform random in `[lo,hi]`** (ret RAX low32; lo/hi
  = float-bits in GP regs RDX/R8). Ghidra **dropped lo/hi** (showed `ffbb0(&LCG)`); the binary's leftover regs
  hold the two muzzle matrices' translation columns (`mat1[k]`,`mat2[k]` = the spread cone). My 1-arg call ⇒
  garbage bounds ⇒ spread-weapon projectiles at the origin. **FIX: spread branch = `ffbb0_range(LCG, mat1[k],
  mat2[k])`, k∈{3,7,11}.** First ship group now spawns at the muzzle.
- **REMAINING (unfixed): a 2nd firer population still spawns at EXACT `(0,0,0)`** ⇒ their `mat1`/`mat2`
  translation is 0 (`385e70` not filling the muzzle for them) = a further create-geometry trap.

**Methodology #27:** a Ghidra-`void`/undefined4 function may actually return a FLOAT in XMM0, and Ghidra DROPS
trailing args passed in leftover registers — both invisible at the call site, both bit-wrong if transcribed
literally. **The field oracle (DTB3SUM `N/0`) is necessary-not-sufficient** — it validated firer/tgt/dmg/life
but NOT the create POSITION/GEOMETRY, which is exactly where these bugs live. **The right fix is a create-pos/
orient OBSERVE oracle at pfire=2** (binary fires; recompute the reimpl's geometry on the same pre-call state and
compare to the binary's actual projectile transform) to surface ALL the remaining traps in one battle, instead
of whack-a-mole one-per-launch. Then re-validate fire-rate≈binary per faction (methodology #26).

### 8.21 BUILT the create-position OBSERVE oracle (pf==2, methodology #27) (2026-06-07)
Refactored the takeover into a SHARED geometry core so the observe tests the EXACT code the takeover runs
(no transcription drift between the two):
- `pfire_compute_geom(p1,p2,p3, S, &lVar7,&local238,&p3, mat1col,mat2col)` = 3825b0:60-260 (R1 gates +
  gate2a + gate2b aim/LOS/range + R1c spread/lead/dispersion/Euler). Returns 1=fire (S filled, create
  pos=`&S[8]`=（S8,S9,S10)) / 0 / PFIRE_FALLBACK. Optional out: the two muzzle-matrix translation columns.
- `pfire_fire_reimpl` = thin wrapper: compute_geom → R2a/R2b/R3 (takeover, unchanged behavior).
- `pfire_observe_geom` = compute_geom → reports create-pos + mat cols + flags (bit0 spread, bit1 `p1+0x3c<0`
  single-bone ⇒ 385e70 leaves mat2 UNFILLED, branch A).

**Mechanism (RNG-faithful diff):** at **pf==2** the binary fires normally (STAGE B two-phase, gameplay-safe).
The b3 hook snapshots the weapon LCG (`0xa13e24`, a single u32 — `ffbb0`/`ffdb0`/`381dc0` all step it)
BEFORE the trampoline; after the binary fires (and the existing 29f810 piggyback captures the binary's
actual create-pos in `g_b3_args.pos`), it **rewinds the LCG to the pre-state, runs the observe (replaying the
binary's exact spread/dispersion draw sequence), then restores the LCG** so the live stream is untouched. A
correct transcription ⇒ `obs==bin` bit-exact. Tally: `PFOBS-SUM n/match/mismatch/origin/nofire`; first 32
mismatches dump `PFOBS obs=… bin=… mat1=… mat2=… spread=… p3c_neg=…`.

**Root-cause confirmed in 385e70:** `decomp/385e70.c` has two branches on `*(int*)(param_1+0x3c)`: `<0`
(single bone) fills param_2(pos)+param_3(mat1) and **leaves param_4(mat2) = the 0x800820 identity init
(translation 0)**; `>=0` (two bones) fills both. The binary's spread-selection is internally consistent (it
selects mat2 only when `+0x3c>=0`). The pf==2 observe will show, per origin-spawn, whether the reimpl
selected/blended a zeroed mat2 the binary wouldn't — the precise create-geometry trap. **Built + oracle-
deployed; NEEDS A BATTLE RUN to collect PFOBS data (`just pfire=2 difftrace=1 launch-foc-desktop`).**

**RESULT (battle run) — the observe NAILED it in one pass, and it was NOT the mat2-unfilled theory:** PFOBS
`n=188035 match=54361 mismatch=133674 origin=133674 nofire=1.8M`. EVERY mismatch had **`mat1==mat2==bin`**
(both matrices correctly = the real muzzle), **`spread=1`, `p3c_neg=0`**, `obs=0,0,0`. So non-spread weapons
matched (deterministic mat-column select) and 100% of the origin-spawns were SPREAD weapons whose `ffbb0`
cone-draw returned 0 from CORRECT, equal bounds. `ffbb0_range(LCG,408,408)` must = 408 yet gave 0 ⇒
calling-convention bug, confirmed by `objdump 0x1ffbb0`:
```
comiss xmm1,xmm2     ; lo=XMM1, hi=XMM2  (FLOAT args, not RDX/R8)
imul eax,[rcx],...   ; lcg ptr = RCX
... movaps xmm0,xmm2 ; ret in XMM0  (not RAX)
```
**`ffbb0` is `float ffbb0(uint *lcg, float lo, float hi)`.** The prior "dropped-args" fix (909e4b8) was
HALF-right (the args WERE dropped) but WRONG on the convention — it passed bounds in GP regs (RDX/R8,
ignored) and read RAX (garbage); XMM1/XMM2≈0 ⇒ `(0-0)*r+0=0`. **FIX: typedef → `float(*)(uint32_t*,float,
float)`, call directly (no bit-packing).** This is the THIRD trap of the §8.20 class (void-float-return,
dropped-args, AND now **float-args-in-XMM mistyped as GP**) — all invisible at the Ghidra call site.

**Methodology #28:** when a leaf's args are FLOATS, Ghidra may mistype them as `undefined8` GP-register
params (and the return as RAX). On Win64 floats ride XMM0-3 by position and float returns come back in XMM0.
A GP-typed function pointer then passes/reads the wrong registers — and the symptom is a CLEAN ZERO (the
unset XMM reg), not garbage, so it can masquerade as a logic bug. **Disassemble any RNG/math leaf before
transcribing its signature** (`comiss/movss/cvtsi2ss xmmN` on the arg = it's an XMM float param).

### 8.22 A3.3 TAKEOVER — REAL PASS (in-game, user-confirmed) (2026-06-07)
After the ffbb0 XMM fix, re-validated in two stages:
- **pf==2 position observe — BIT-EXACT:** `PFOBS-SUM n=25296 match=25296 mismatch=0 origin=0`. Every
  spread+non-spread create-pos matches the binary's actual 29f810 pos arg on the replayed-LCG pre-state.
- **pf==2 fire-rate parity — ZERO genuine under-fire:** the metric initially read `underfire=31873` but that
  EXACTLY equalled `fb_p3=31873` (the deliberate `param_3==0` defer-to-binary). Split the counter
  (`GENUINE_underfire` vs `fallback`) ⇒ genuine under-fire = 0. (Also fixed a stale `g_b3_args.have` that had
  inflated the raw nofire — reset it per 3825b0 call.)
- **pf==3 takeover — IN-GAME PASS:** `DTB3SUM TAKEOVER=reimpl firer/dmg/life all N/0`; `PFIREDBG
  reachedR2=8278↑ lvl=3` (reimpl creates inline, not just falling back); `PFPOS` shows projectiles at the
  firer's hull (the ex-origin firer `-1350,3449` now spawns `proj@408,3253`); no crashes; two-phase intact
  (`deferred==fired`). **USER CONFIRMED on screen: capital ships deal/take damage, NR fires symmetrically,
  turbolaser/ion fire from muzzles.** The earlier §8.16 "A3.3 PASS" was the FALSE pass; THIS is the real one
  — the position observe (methodology #27) caught the ffbb0 bug the field oracle (DTB3SUM N/0) structurally
  could not see. `tgt=N/35` (~0.2%) is the pre-existing target-died-mid-tick race (binary shows the same rate
  at pf==2), NOT a takeover regression.

**A4 RE-AIM (the threading ceiling):** at pf==3 `fb_p3≈298048` vs `reachedR2≈8278` — **~97% of fires take the
`param_3==0` binary fallback**, so the lifted reimpl currently drives only ~3% of the create path. The A4
speedup therefore lives in **lifting the `param_3==0` aim path** (`383f70` direct-aim + `405870` target
redirect — the RNG-aim branch the reimpl defers), not in threading the already-lifted body. This supersedes
the A4.0 "fire body is ~20% of fire-control" framing: of the body itself, the lifted slice is a further ~3%.

### 8.23 A4 INCREMENT 1 — `param_3==0` RNG-aim path LIFTED, fire body CLOSED (pf==2 bit-exact) (2026-06-07, 30e83cf)
**User picked "param_3==0 first" (the bounded, lower-risk A4 entry over the bigger opp-scan lift).** The reimpl
deferred ~97% of fires because `pfire_compute_geom` fell back at `param_3==0`. Lifted `3825b0:164-181` as
ORCHESTRATION (A-reimpl — call the binary leaves, don't reimplement): `398440(p2,&scratch)` dead read (:165,
pure, output unused) + optional `405870` UnitAI target-redirect (`vfunc_2(p2,0x16)`→`405870`, :167-170) +
`383f70(p1,&aim,target,&flag)` aim (:174) + aim-fail gate (:178). **Decoded all 3 leaves: none write shared
state; `405870`/`383f70` draw the LCG (0-1× each) ⇒ observe-safe under the pf==2 LCG save/restore.** The
redirect REASSIGNS the target for the rest of the body (lead solver `399450` + Class-2b `+0x38` listener +
`3a06a0` shot-register + R3) — propagated via a new `out_p2` from `compute_geom`, mirroring the binary's
in-place `param_2` reassignment.
**VALIDATION (pf==2 observe, battle run) = BIT-EXACT:** `PFOBS-SUM n=139878 match=139878 mismatch=0 origin=0
GENUINE_underfire=0 fallback=0`. **`fallback` dropped 31873→0**; `n` rose ~25k→140k (the ~101k `param_3==0`
fires now compared, not deferred); every create-pos matches the binary on the replayed LCG. ⇒ **the fire body
is now ~100% reimpl-driven** (only the dead `+0x4e==1` arc gate would fall back, `fb_arc=0` in tactical).
Fixed a cosmetic `PFIREDBG` funnel underflow (`fb_p3` is now a LIFTED sub-path, not a reject → relabeled
`p3eq0(lifted)`, removed from the reject math). **NEXT A4 increment = the opp-scan `387400`+`385190` lift
(~80% of PhaseB time, the real threading ceiling), then create-deferral on full coverage, then N-shard.**

### 8.24 A4 B2 — DTSCAN: the opp-scan cost IS `385190` (2026-06-07, 5ddd6a3)
Entry-detour on `385190` (scan-exclusive — only `387400` calls it). Battle result (cumulative, ~tick 110k):
`evals=2,810,132 hit=12,900 winner=5,952 eval_ms≈13,529 avg_us=1.1→4.8`. **`385190`'s `eval_ms` (~13.5s) is on
par with PFIRESPLIT's whole `scan_ms` (~10s) ⇒ `385190` is essentially the ENTIRE opp-scan cost; the `387400`
orchestration is cheap.** The scan does ~2.8M per-candidate spatial-evals, only ~0.46% return a candidate and
~0.2% a winner (the random-start linear walk burns most evals on non-qualifying candidates). Each eval =
spatial query `20e780` + full gate-predicate + a `383f70` aim — near-read-only and independent ⇒ the
embarrassingly-parallel profile A4 targets. (Caveat: 2×2.8M `QueryPerformanceCounter` calls inflate `eval_ms`;
the COUNTS are solid, timing approximate.) Takeover stayed clean alongside (`DTB3SUM 58312/0`, no crash).
**Reframe confirmed:** the scan is RNG-retrofit (substream the `:250` start + per-candidate `383f70`) ⇒ the
lift is NOT bit-exact vs the global-LCG binary; the gate is this structural load/cost/selection distribution.
**⇒ B3 = LIFT `385190`** (the dominant cost), not the dispatcher. Hazards to inject (all buried inside the
binary leaf ⇒ require transcription): `20e780` spatial query → `SpatialQueryGuard` (§8.6/§8.10 built); the
`:89` `383f70` per-candidate draw → `SimRng::substream`; `35f470` FOG → §8.12 scratch LOCK; the rest of the
gate-predicate (`39b140`/`39a540`/`540140`/`383ba0`/`vfunc_2(0x11)`) are the already-validated read leaves.

### 8.25 B3 INCREMENT 0 — `385190` DECODE + the SELECTION-DETERMINISM refinement (2026-06-08)
Full read of `385190` (the per-team opp-scan, decode against `decomp/385190.c`) + its caller `387400:236-289` and
its aim callee `383f70`. **Structure of `385190(param_1=fire-ctx, param_2=ENEMY TEAM, param_3=&best_score)`:**
1. **early-out** `:29`: `*param_3==1.0` (a perfect winner already found by a prior team) **or** `param_2==0` → return 0.
2. **query box** `:32-50`: `2acb60(index, team+0x4c)` gets the team's frozen spatial grid; build an AABB around the
   owner pos (`owner+0x78/+0x7c/+0x80`) ± radius `3857d0(p1)*DAT_140819b38`, center via `*DAT_1408007c0`(=0.5);
   `20e780(grid, &box)` runs the query into the grid's INTRUSIVE result list (head `*(grid+8)`, threaded through
   each result's `+0x18`, plus the benign global counter `DAT_140a7c66c++`).
3. **walk + gate** `:53-99`: for each result `plVar6`, a 12-clause gate predicate (alive `[0x54]!=0`; type flag
   `[0x53]+0x6b`; `+0x3a7!=1`; self-flag `+0x3a0&0x40`; `39b140` cloak OR owner+0x298+0xa4; `vfunc_2(0x11)`==0;
   the `+0x3b4` pair OR `39a540`; `397060`==0; `35f470` FOG !=1; `owner_type+0x4e!=1` OR `383ba0(p1,plVar6+0xf)`;
   `[0x53]+0x41==1`; `540140(owner_type,plVar6)`==0). These are the SAME read leaves pfire gate1 already validated.
4. **score** `:79-86`: `id=3973b0(plVar6)`; if `owner+0x33b!=-1` and `vfunc(owner,9)!=0` → `score=vfunc_0x118(that,id)`
   (the weapon-vs-target priority), else `score=1.0`. Lower = better; `1.0` = perfect; `DAT_140899788` = skip sentinel.
5. **reach + select** `:87-96`: if `score!=skip` → `383f70(p1, &aim, plVar6, &hit)`; if `hit && (score<*best ||
   *best==skip) && !(plVar6+0x3a1&0x10)` → `*best=score; winner=plVar6`; if `score==1.0` **return winner immediately**.

**THE REFINEMENT (stronger than §8.24's "distribution-only" gate):** `385190`'s returned winner+score is
**RNG-INDEPENDENT**, so the B3 lift can be **SELECTION-BIT-EXACT**, not merely distribution-matching.
- `383f70`'s ONLY RNG draw is `:121` `FUN_1401ffb40(&DAT_140a13e24,…)` — a random START for its `+0x2d0` candidate-
  bone loop. That loop checks EVERY bone exactly once (modulo-wrap, `iVar10 < iVar9`) and returns on the first
  reachable one ⇒ the **hit boolean is deterministic** (≡ "any bone reaches"); the random start only changes WHICH
  bone's aim point is returned.
- `385190` **discards that aim point** (`local_98` is written at `:89`, never read again — only `local_res18[0]`,
  the hit flag, is consumed `:90`). The scan-aim is throwaway; `3825b0` re-aims at fire time anyway.
- The score (`vfunc_0x118` on a deterministic target id), the gate, and the spatial-query ORDER (frozen grid) are
  all RNG-independent; the tie-break is strict-`<` (first in query order wins). ⇒ **winner + `*best` are deterministic.**
- The genuine RNG-retrofit seam is in the **DISPATCHER** `387400:250` (`FUN_1401ffb40` random TEAM-start) + `:274`
  break-on-perfect — which team's 1.0-target is taken when several teams hold one. That is NOT in `385190`; it is
  handled at the 387400 level (fork B keeps it serial; fork A substreams the team-start). **B3 lifts `385190` only.**

**HAZARD MAP (confirmed against decomp, for the host transcription):** H1 `20e780` intrusive list + counter →
`SpatialQueryGuard` (lock query+collect, copy result ptrs out; built §8.6/§8.10). H2 `383f70:121` LCG draw →
`SimRng::substream` — but per the refinement this is OBSERVABLY INERT to selection (it only perturbs the discarded
scan-aim), so the substream is a determinism-hygiene retrofit, not a behavior change. H3 `35f470` FOG → §8.12
scratch LOCK. All other leaves (`2acb60`/`3857d0`/`39b140`/`vfunc_2(0x11)`/`39a540`/`397060`/`383ba0`/`540140`/
`3973b0`/`vfunc(9)`/`vfunc_0x118`/`383f70`'s `12d2c0`/`3858b0`/`383ba0`) are near-read-only — call via trampoline.

**ORACLE DESIGN (next increment, methodology #27 / DTSCAN-shaped):** entry-detour `385190`; per call run the binary
(trampoline) AND the host reimpl on the same `(param_1, team, &best_copy)`; assert IDENTICAL returned winner ptr +
IDENTICAL post-`*best` score. Gate = `mismatch=0` over a full battle (selection-bit-exact), STRICTER than §8.24's
load/cost distribution. The reimpl is a `pfire_opp_scan_reimpl` mirroring `385190` (box→guarded query→gate walk→
score→select), calling every leaf via its trampoline; then the takeover repoints `387400:273`'s `385190` call site.

**INCREMENT PLAN:** (B3.1) build `pfire_opp_scan_reimpl` + the observe oracle → selection-bit-exact PASS in a live
battle; (B3.2) wire the takeover at `387400:273` (1-shard, default-OFF env gate) → in-game PASS; (B3.3) wrap the
query in `SpatialQueryGuard` + per-team `SimRng::substream` and parallelize the gate walk (N-shard); (B3.4)
fork-A: thread the `387400` team-walk with the `:250` team-start substreamed. This session = B3.0 (decode) only.

### 8.26 B3.1 — `385190` host reimpl LIFTED, SELECTION-BIT-EXACT in-game PASS (2026-06-08, b69bed2)
Built `pfire_opp_scan_reimpl` (winmm_proxy.c) — a line-for-line transcription of `385190` (box → `20e780`
query → 12-clause gate walk → `vfunc_0x118` score → `383f70` reach → select), every binary leaf called via its
`g_dt_imgbase+RVA` trampoline; new typedefs `2acb60`/`20e780`/`397060`/`vfunc_0x118` join the existing R1 leaf
set. Added the **DTSCANOBS** observe (`EAW_DTSCAN_OBS=1`, default-OFF, `just dtscanobs=1`): per `385190` call it
captures the INPUT `*score`, runs the binary (trampoline), then re-runs the reimpl on that same input under an
LCG save/restore (so the reimpl's `383f70` draws don't perturb the game), and diffs the returned winner ptr +
resulting score.
**RESULT (live space battle, `eaw-mt.log` DTSCAN line):** `obs_n=4,699,072 obs_match=4,699,072 obs_wmiss=0
obs_smiss=0`, ZERO `DTSCANOBS` mismatch lines, no crash — **selection-bit-exact across 4.7M evals.** This
CONFIRMS the §8.25 refinement empirically: despite `383f70`'s `:121` LCG bone-start draw, the reimpl picks the
IDENTICAL winner + IDENTICAL score as the binary on every call ⇒ the opp-scan lift is bit-exact-on-selection, a
STRONGER gate than §8.24's load/cost distribution. (The genuine RNG seam stays in the dispatcher `387400:250`.)
**This is the SERIAL 1-shard transcription** — it calls `20e780` directly and lets `383f70` draw the global LCG
(safe under save/restore in the observe). NEXT: B3.2 wire the takeover at `387400:273` (repoint the `385190`
call to the reimpl, env-gated, 1-shard) → in-game PASS; then B3.3 wrap H1 `SpatialQueryGuard` + H2 `383f70`
`SimRng::substream` + H3 FOG scratch lock and parallelize the gate walk (N-shard); B3.4 fork-A team-walk.
**Methodology bank:** the DTSCANOBS pattern = run-binary-then-reimpl-on-captured-input-under-LCG-save/restore is
the §8.25/#27 observe applied to a SELECTION (ptr+score) rather than a geometry (pos) — works because the
captured INPUT (`*score` pre-mutation) + the frozen grid make the reimpl's verdict independent of the live RNG.

### 8.27 B3.2 — opp-scan TAKEOVER wired, in-game PASS (RNG-stream-transparent) (2026-06-08, bc7e1fe)
Flipped the DTSCANOBS machinery into a takeover at the `385190` entry detour (= the `387400:273` call site, since
`385190` is scan-exclusive): `EAW_PFIRE_SCAN=1` (`just pfirescan=1`) makes `dtscan_385190_hook` RETURN the reimpl's
winner and LEAVE the reimpl's LCG/score state, so `pfire_opp_scan_reimpl` actually DRIVES `387400`'s opportunity-
target acquisition (the `382510` assign + `OpportunityTargetAcquired` event downstream). Self-validating: the hook
still runs the binary, rewinds the LCG, runs the reimpl on the captured input, and diffs **winner + score + post-
LCG state** before returning the reimpl's result.
**NEW (stronger) claim proven: the takeover is RNG-STREAM-TRANSPARENT, not just selection-bit-exact.** Added an
`obs_lmiss` check (binary's post-scan LCG == reimpl's). Reasoning: `385190`'s only LCG-drawing call is `383f70`,
invoked on exactly the candidates that pass the gate, in frozen-grid query order — IDENTICAL set+order in the
reimpl ⇒ identical draw count ⇒ identical final LCG. **RESULT (live space battle, `eaw-mt.log` DTSCAN
`mode=TAKEOVER`):** `obs_n=3,065,365 obs_match=3,065,365 obs_wmiss=0 obs_smiss=0 obs_lmiss=0`, ZERO `DTSCANOBS`
lines, no crash — the reimpl drives target acquisition AND the global LCG ends in the binary's exact state every
call ⇒ provably indistinguishable from the binary at the `385190` boundary (downstream RNG untouched).
**This validates the SERIAL 1-shard takeover.** B3.2 still runs the binary alongside for the diff (2× cost, an
oracle-build validation, NOT a perf win). **NEXT: B3.3 = drop the binary run + wrap the three hazards (H1 `20e780`
→ `SpatialQueryGuard`, H2 `383f70` → `SimRng::substream`, H3 `35f470` FOG → scratch lock) and parallelize the
gate walk N-shard; B3.4 = fork-A thread the `387400` team-walk with the `:250` team-start substreamed.** The
RNG-transparency result simplifies B3.3: since the serial reimpl's draw-stream already matches the binary, the
substream retrofit (H2) is the ONLY thing that changes the numbers, and only when shards run concurrently.

### 8.28 B3.3.0 — PRE-BUILD AUDIT: the "drop the binary + N-shard in-game" framing OVERSHOOTS; B3.3 is a fork (2026-06-08)
Methodology #23 again (audit before any thread code, mirroring §8.17). The §8.27 NEXT bullet ("drop the binary +
N-shard the scan in-game") is NOT achievable as written. Two findings:
- **SUBSTRATE (in-game threading is blocked the SAME way the whole Phase-A tick is):** the binary drives the scan
  from its SINGLE-THREADED tick loop (`28d400`→`3a76b0`→`387010`→`387400`→`385190`). Object-granular Phase-A
  parallelism REPLACES that loop with `ShardScheduler` (`sim/shard_scheduler.h`) — which is built+host-validated
  but UNWIRED (needs the lifted Phase-A orchestrators + `FrozenSnapshot`, the long-deferred engine-source pieces).
  The hook can't inject N-way threading into the binary's loop. So the only IN-GAME threading available is
  INTRA-`385190`-call (fan the per-candidate eval across a pool inside the reimpl) — and that needs a per-leaf
  concurrent-call safety audit, which the next finding shows is expensive.
- **LEAF CONCURRENT-CALL SAFETY (the in-game intra-call hazard is worse than the H1/H2/H3 list implied):** `35f470`
  (FOG, gate clause I) `HeapFree`s + reallocs a SHARED global scratch buffer `DAT_140a28538` (+count `+0x40` /
  flags `+0x44`) on EVERY call (decomp/35f470.c:31-48). Concurrent calls = use-after-free. A lock around it
  serializes the realloc — i.e. serializes the per-candidate "expensive part" the parallelism targets — so fork B
  would have to LIFT `35f470` (per-thread scratch) too. `383f70` (reach) draws the global LCG (H2) and calls
  `12d2c0`/`3858b0`/`398440`/`373670`/`396cb0` whose static-buffer/re-entrancy safety is UNAUDITED. ⇒ in-game
  intra-call threading is a multi-leaf lift with real race/crash risk, not a wrapper job.
- **EARLY-OUT vs PARALLEL-REDUCE EQUIVALENCE (must resolve for EITHER fork's determinism gate):** serial `385190`
  is a left-fold-with-early-EXIT (`:93-94` returns at the first `score==1.0`). A parallel map+reduce reproduces it
  bit-exactly IFF `1.0` is the GLOBAL-MIN score (no candidate scores `< 1.0`); then "first 1.0 in query order" ≡
  "first occurrence of the global min". If any score `< 1.0` can appear AFTER a `1.0` in query order, serial stops
  early on the `1.0` while a plain min-reduce picks the `<1.0` ⇒ DIVERGENCE. The design (1.0 = "perfect, stop")
  strongly implies 1.0 is the min, but it is UNCONFIRMED — needs an empirical probe (count any per-candidate
  `score < 1.0`). If 1.0 is not the min, the reduction must be a segmented-scan replicating the early-exit (still
  deterministic, just not a plain min).

**THE FORK (Rule-6 gate — user decides):**
- **Fork A — HOST determinism gate (low risk, buildable now, mirrors the command-system build):** lift `385190` as
  a host `sim/opp_scan` module — guarded query (H1 `SpatialQueryGuard`, built) + substreamed reach (H2
  `SimRng::substream`, built) + serialized/per-thread FOG (H3) — driven by the existing `ShardScheduler`, with a
  `just sim-test` gate proving the winner+score is SHARDING-INVARIANT (N∈{1,2,4,8} × shuffled order ≡ serial,
  bit-identical), reusing the §8.26 selection-determinism result. In-game N-shard stays deferred to the (separate)
  scheduler wiring — same honest split as the command system. Resolves the early-out question in the host harness.
- **Fork B — IN-GAME intra-`385190` threading (high risk, real in-game speedup):** lift `35f470` (per-thread FOG
  scratch) + audit the `383f70` callee leaves for concurrent-call safety, then fan the candidate walk across a
  hook-owned thread pool inside the reimpl with H2 substreams. Validate via DTSCANOBS (winner/score must still
  match the binary) + stability. The leaf-safety audit + `35f470` lift are a multi-session sub-program.

**RECOMMENDATION: Fork A** — it is the in-scope, low-risk continuation consistent with how every prior parallel
piece was validated (host gate, in-game deferred to the unbuilt scheduler), and Fork B's payoff is capped by the
serial tick driver anyway (you'd speed one hardpoint's scan while the loop still visits hardpoints serially). This
session = B3.3.0 (audit) pending the fork decision.

### 8.29 B3.3.1 — FORK B chosen; LEAF CONCURRENT-CALL SAFETY AUDIT (2026-06-08)
User picked Fork B (in-game intra-`385190` threading). First increment per methodology #23 = audit every function
the per-candidate eval calls for concurrent-call safety (static/global/heap writes, re-entrancy) BEFORE any thread
code. Scanned `decomp/` for each leaf (write-signature grep + body read). **Verdict:**
- **CLEAN (pure reads of the frozen world — concurrent-safe):** `39b140`, `39a540`†, `540140`, `3973b0`, `398440`,
  `3858b0`, `397780`, `2648b0`, `3857d0`, `383ba0`, `776d48`(=sqrt), and `383f70`'s body MINUS its draws/transforms.
  Zero `DAT_ =`/`Heap`/`malloc`/TLS writes in each. († `39a540` calls vfunc `0x210` on the global shroud mgr
  `DAT_140b15418` — needs a read-vs-write confirm on that one vfunc, but the C body has no direct writes.)
- **H2 (LCG draw) — `1ffb40`** via `383f70:121`: writes the global LCG seed `DAT_140a13e24` through its param ptr.
  Mitigation = pass a PER-THREAD `SimRng` substream pointer (built). Per §8.27 the serial draw-stream already
  matches the binary, so the substream only changes the (discarded) scan-aim, never the selection.
- **H3 (FOG) — `35f470`: CONFIRMED HAZARD.** `GetProcessHeap`/`HeapFree`+realloc of the SHARED global scratch
  `DAT_140a28538` (count `+0x40`, flags `+0x44`) on every call, behind a `_Init_thread_header(&DAT_140b2c228)`
  magic-static guard. Concurrent calls = use-after-free. **Must LIFT `35f470` with per-thread scratch** (the buffer
  is rebuilt per call from read-only fog inputs ⇒ a faithful transcription with a thread-local vector is feasible).
- **⚠️ PIVOTAL OPEN QUESTION — `12d2c0` (per-bone matrix fetch, called heavily in `383f70`) invokes vfunc `0x78` on
  the shared shooter/candidate object BEFORE reading its bone array** (`param_1[0x1d]+0x28`, stride 0x30 = a 3×4
  matrix). If vfunc `0x78` LAZILY REBUILDS the skeleton/world-transforms (vs no-op on an already-resolved, dirty-
  flag-clean cache), concurrent candidate-evals racing on the SAME shooter's bone cache is a hazard — and the fix
  would be lifting/snapshotting the skeleton resolution (a large lift), NOT just per-thread scratch. This is the
  SAME "frozen during Pass-A?" invariant `SpatialQueryGuard` (§8.6) relies on, extended to the bone cache.
  **Fork B feasibility GATES on decoding vfunc `0x78`** (resolve the concrete vtable of `383f70`'s `3858b0`-returned
  render object) + confirming the bone cache is resolved before the fire-pass. NEXT.
  **REFINEMENT (traced `3858b0`+`2648b0`):** the object `12d2c0` operates on is a SHARED MODEL RESOURCE, not a
  per-GameObject transform — `3858b0` resolves it via the model manager (`2648b0` RTTI-walk for `DAT_140a12370`)
  → model lookup `12d6f0(mgr, name_hash)` → bone-index validate `12d2a0`; the bone table read is a static-looking
  bind-pose matrix array (stride 0x30 at `model+0x1d…+0x28`). So a race would be on a SHARED ASSET across all
  GameObjects of that type (worse than per-object), making the vfunc-`0x78` verdict more important: if it's an
  idempotent load/residency guard → read-only-once-loaded → safe; if it evaluates an animation pose INTO the shared
  model buffer per call → cross-object hazard. Decode `0x78` via Ghidra (model-resource vtable slot) = the gate.

### 8.30 B3.3.2 — vfunc 0x78 DECODED = dirty-flag-guarded SHARED pose eval ⇒ Fork B needs a serial skeleton pre-warm (2026-06-08)
Resolved the gate via Ghidra (RTTI map + `DumpVtable` + `DecompileAt`). The `3858b0`-returned model is **`alHModel`**
(vtable `0x818d90` — every method in the `0x12xxxx` cluster, `GetName` at slot 6/`+0x30` matching `12d520`); vfunc
`0x78` = slot 15 = **`FUN_14012c7c0` = `alHModel::vfunc_15`**. Decompiled (`decomp/12c7c0.c`): it is a **lazy,
dirty-flag-guarded, MUTATING pose evaluation on the shared model**:
```
if (model[+0x30] & 0x20) {            // dirty bit
    model[+0x30] &= 0xdf;             // CLEAR (non-atomic read-modify-write)
    pose = 13fe60(model);
    146310 / 1467b0 / 146d50(model+0xe8, pose, model+0x38);   // eval pose INTO the shared bone buffer
    for each sub-mesh in model[+0xf0]: vfunc_0x58(...) + toggle *(sub+6) bit 2;   // more shared writes
}                                     // (recurses into model[+0xb0] parent first)
```
**VERDICT — concurrent-call HAZARD, but idempotent once clean.** When the dirty bit is set it WRITES the model's
bone buffer (`+0x38`), its mesh (`+0xe8`), and each sub-mesh's flag byte, then clears the bit; when clear it no-ops.
In the opp-scan EVERY candidate-eval calls `3858b0(param_1)` on the SAME shooter model, and same-type candidates
share their model asset — so a threaded candidate walk = concurrent non-atomic RMW on one model's dirty flag + bone
buffer = data race / torn writes. This confirms the §8.29 worry: vfunc `0x78` is the pose-eval case, not a load guard.
**FORK B IS FEASIBLE WITH A SERIAL SKELETON PRE-WARM** (the clean mitigation, NOT a full skeleton lift): before the
parallel candidate walk, call `alHModel::vfunc_15` ONCE serially on the shooter model AND each distinct candidate
model (the dirty bit is set only by the animation pass, not re-set within a tick's fire-pass) → every worker's
later vfunc `0x78` finds the bit CLEAR → no-op → the bone reads are pure/concurrent-safe. In the existing serial
reimpl the first candidate-eval already warms the shooter for free; the parallel version must HOIST that warm-up
out of the loop + extend it to candidate models. Bounded, cheap (one eval per distinct model), confined to the
model instance (no other shared state touched). **NET — Fork B build plan now fully scoped:** (1) lift `35f470`
per-thread FOG scratch (§8.29 H3); (2) serial skeleton pre-warm pass over shooter+candidate models (this finding);
(3) per-thread `SimRng` substream for `383f70`'s draw (H2); (4) thread-pool the gate+score+reach candidate walk;
(5) deterministic reduction replicating `385190`'s early-exit-at-`score==1.0` (confirm 1.0 is the global-min score,
else segmented-scan §8.28); validate via DTSCANOBS winner/score parity + stability. Residual leaf decodes
(`397060`/`373670`/`396cb0`/`264b8b0`) still owed before step 4 but all expected clean (§8.29 grep).

### 8.31 B3.3.3 — FORK B STEP 1: `35f470` FOG made concurrent-safe via a lock-guarded primitive (2026-06-08)
Decoded `35f470`'s scratch filler `FUN_14035dec0` (`decomp/35dec0.c`): it's a target-silhouette projection that
pushes ~8+ bounding-corner points (12-byte {x,y,flag} each) into the `DynamicVectorClass` at `DAT_140a28530`
(vtable `+0`, buf `DAT_140a28538`/`+8`, count `DAT_140a28540`/`+0x10`, cap `+0x14`) via `19f6a0` grow + the
`20d1c0/210/260/2b0/310/3b0/340/380` corner projectors; `35f470` then runs a per-segment grid-occlusion loop
(`4c0e00`) over that vector. **A TRUE per-thread lift = transcribing `35dec0` + 8 projectors + `4c0e00` + `39a230`
(~10 functions) onto a thread-local vector — disproportionate, since FOG is the CHEAP gate clause (the parallel
win is `383f70`/bones) and many calls early-out before touching the scratch (fog-disabled `DAT_140a284c4==0`,
always-visible target flags).** **DECISION (Step 1): a LOCK-GUARDED primitive** — wrap the real `35f470` in a
`CRITICAL_SECTION` so the shared `DynamicVectorClass` build+consume is serialized across scan-worker threads, reusing
the engine logic BIT-EXACTLY (no transcription risk). Correct + deterministic (each call fully rebuilds+consumes its
own query; result is order-independent) + reversible. **Built** `pfire_fog_occluded(b15418, sid, cand)` (the §8.29
clause-I call now routes through it) + `g_fog_cs` (init in `install_dtscan_hook`). The lock is INERT in the current
serial reimpl (uncontended) — it is the safe primitive the step-4 thread pool will rely on. **NO-REGRESSION PASS
(live space battle, FOG routed through the wrapper under takeover): `mode=TAKEOVER evals=5,812,922
obs_match=5,812,922 obs_wmiss=0 obs_smiss=0 obs_lmiss=0`, 0 divergence lines, no crash** — the lock-guarded `35f470`
returns identical results and the takeover keeps full winner/score/LCG parity ⇒ the refactor is behavior-preserving. **Deferred optimization (if step-4
profiling shows FOG contention):** lock-free early-outs (`35f470:16-23`, pure reads — needs a `39a230` concurrent-
safety audit) + a lock-around-`35dec0`-build-then-copy-out so the per-segment `4c0e00` loop runs lock-free on a
thread-local segment copy (the SpatialQueryGuard §8.6 pattern applied to FOG).
- **RESIDUAL DECODES (not yet read, all flagged for the same write-scan):** `397060` (gate clause H), `373670`,
  `396cb0`, `264b8b0` (`383f70` callees). `776d48` confirmed = sqrt.
**NET so far:** the per-candidate hazard surface is SMALL — one definite lift (`35f470`), one substream (`1ffb40`),
one pivotal skeleton-freeze question (vfunc `0x78`), 13 leaves already clean. If vfunc `0x78` proves frozen/idempotent
during the scan, Fork B reduces to {lift `35f470` per-thread + substream `383f70` + thread-pool the candidate walk +
deterministic reduction (replicating the §8.28 early-exit semantics)}. If not, Fork B needs the skeleton snapshot
first. B3.3.1 = leaf audit; vfunc `0x78` decode is the next increment.

### 8.32 B3.3.4 — FORK B STEP 2: serial SKELETON PRE-WARM built (2026-06-08)
Built `pfire_skeleton_prewarm(p1, grid)` + `pfire_model_warm` (winmm_proxy.c), called in `pfire_opp_scan_reimpl`
right after the spatial query, before the candidate walk. It calls `alHModel::vfunc_15` (vtable+0x78, the §8.30
dirty-flag-guarded pose eval) once per model — the shooter (`3858b0(p1)`) and each candidate (`2648b0(cand+0x2a0)`)
— so the (future step-4 parallel) walk's `12d2c0`→vfunc_15 calls find the dirty bit CLEAR → no-op → concurrent bone
reads are pure. **Two lazy mutations neutralized (refinement from re-reading `383f70`):** (1) the shooter model's
pose — SHARED across all candidate iterations (`3858b0(p1)` returns the same model each time) = the actual race
target; (2) `3858b0` ALSO lazily writes a resolved bone-index cache at `p1+0x90` (only when `<0`), so warming the
shooter once resolves it → the parallel calls find it cached → no write. Candidate models are each touched by only
one worker (distinct per object), but warming them too is conservative (covers shared-asset model instancing as
well as per-instance). vfunc_15 is idempotent ⇒ eager warm ≡ the binary's lazy warm (same pose, same inputs) ⇒
behavior-preserving; it touches only the model (no LCG/FOG-scratch perturbation). **Validation note:** in the
dual-run observe/takeover the binary `385190` runs first and already warms every model it reads, so the reimpl's
pre-warm is a NO-OP there — the no-regression launch confirms the model-fetch offsets (`3858b0(p1)`,
`2648b0(cand+0x2a0)`, vtable+0x78) are correct and crash-free + no selection/LCG perturbation; the pre-warm's
actual NECESSITY only manifests once the binary is dropped (step 4). **NO-REGRESSION PASS (live battle, pre-warm
active under takeover): `mode=TAKEOVER evals=3,653,134 obs_match=3,653,134 obs_wmiss=0 obs_smiss=0 obs_lmiss=0`,
0 divergence lines, no crash** — warming every candidate model via vfunc_15 is crash-free and behavior-preserving.
NEXT = step 3 (per-thread `SimRng` substream for `383f70`'s `:121` draw).

### 8.33 B3.3.5 — FORK B STEP 3: per-candidate RNG SUBSTREAM for `383f70`'s draw (H2) (2026-06-08)
Built the H2 substream (winmm_proxy.c): `383f70`'s only shared-state write is `FUN_1401ffb40(&DAT_140a13e24,…)`
at `:121` (a random bone-START), and per §8.25/§8.27 it perturbs ONLY the discarded scan-aim, never the hit
boolean / winner. So each candidate's draw is redirected to a per-candidate substream seeded `splitmix32(object_id
@c+0x50, tick)` — stable ⇒ reproducible regardless of thread/order. **ARM `EAW_PFIRE_SCAN_SS=1` (`just
pfirescanss=1`, default OFF):** in the serial reimpl this is realized as a save/set/restore of the global LCG word
around the engine `383f70` call (serial-safe; the actual parallel walk will use a thread-local `1ffb40` redirect at
step 4 so N workers don't race the global word). **It directly VALIDATES the RNG-retrofit-safety claim:** with the
draw coming from a SUBSTREAM instead of the global LCG, the selection must be UNCHANGED (`wmiss=smiss=0`) while the
global LCG is no longer advanced by the scan (`lmiss>0` BY DESIGN — the I2 retrofit signature, distinct from the
default takeover's bit-transparent `lmiss=0`). This is stronger than §8.27's `lmiss=0` (which only showed the reimpl
matches the binary's global draws): it shows a DIFFERENT draw sequence yields the IDENTICAL winner ⇒ the scan's
RNG is genuinely selection-irrelevant, so substreaming it (the whole Fork-B/I2 retrofit) is safe. Default OFF keeps
the global draws so the dual-run takeover stays transparent. **PASS (live battle, substream armed): `evals=4,934,397
obs_match=4,860,045 obs_wmiss=0 obs_smiss=0 obs_lmiss=74,352`, no crash; EVERY DTSCANOBS detail row has matching
`bin_w/re_w` AND `bin_s/re_s` — all divergences are LCG-ONLY** (`lmiss` ≈ 1.5% of evals = the fraction where
`383f70` reaches its `:121` draw). ⇒ the per-candidate substream yields the bit-identical winner+score while the
global LCG is bypassed — the I2 retrofit is selection-safe, empirically confirmed. NEXT = step 4 (thread-pool the
candidate walk + the
thread-local `1ffb40` redirect; residual leaf decodes `397060`/`373670`/`396cb0` owed first) + step 5.

### 8.34 B3.3.6 — STEP 4 PREREQUISITE: residual leaf audit COMPLETE + a decisive GRANULARITY finding (2026-06-08)
Decoded the residual per-candidate leaves (Ghidra): `397060` (membership test — does the cand appear in its
`+0x2b0`+200 list), `373670` / `396cb0` (bounded array indexers at `+0xc40`/`+0x2d0`). **All 3 are PURE READS** —
no writes, heap, TLS, static buffers, or mutating vfuncs (`264b8b0` from the §8.29 list was a stale entry — NOT a
callee). ⇒ **the leaf audit is COMPLETE: with steps 1-3's primitives (FOG lock, skeleton pre-warm, RNG substream)
the entire per-candidate eval is concurrent-call-safe** (every other leaf — `39b140`/`39a540`/`540140`/`3973b0`/
`398440`/`397780`/`2648b0`/`3857d0`/`383ba0`/`776d48`/`12d2c0`-after-warm/`145fa0` + vfuncs `0x11`/`9`/`0x118` — was
already grep-clean reads). Methodology-#23 gate PASSED.
**⛔ BUT — DECISIVE GRANULARITY FINDING (DTSCAN avg_us): a per-`385190`-call thread pool is UNPROFITABLE.** Each
`385190` call costs **~1-5 µs** (`avg_us` 1.1→5.4 across the battle; total ≈23 s over 4.9M calls). That is BELOW
OS thread wakeup/sync latency (~5-20 µs), so fanning ONE call's handful of candidates across worker threads is a
guaranteed NET LOSS — sync overhead dwarfs the work. Fork B's literal definition (intra-`385190` candidate fan-out)
is the WRONG GRAIN; the §8.28 "payoff capped by the serial tick driver" caveat is in fact sharper than stated.
**THE PRIMITIVES ARE NOT WASTED — they retarget:** steps 1-3 + this audit prove `385190`/`387400` are safe to run
as a CONCURRENT Phase-A BODY, so the profitable realization is the OUTER OBJECT-GRANULAR parallelism — the
`ShardScheduler` (`sim/shard_scheduler.h`, built+host-validated) running `387400` per-hardpoint/object across shards
(hundreds-thousands of independent fire-control calls per tick, amortizing thread sync over MANY scans per shard).
That is the convergence of Fork A and Fork B: Fork B's concurrent-safety work is exactly what lets the object-grain
scheduler (Fork A's domain) run the fire-control body safely. **⇒ Step 4 RESHAPES: do NOT build a per-call thread
pool (knowingly slower). The remaining work is wiring `387400` as a `ShardScheduler` Phase-A body — which needs the
long-deferred `FrozenSnapshot` + iteration-set + tick-driver-replacement engine-source pieces (`sim_tick_decomp_
program.md`). USER DECISION (Rule 6) pending.**
thread-local `1ffb40` redirect; residual leaf decodes `397060`/`373670`/`396cb0`/`264b8b0` owed first) + step 5
(deterministic reduction replicating the `score==1.0` early-exit; confirm 1.0 is the global-min score).

### 8.35 B3.4.0 — OBJECT-GRAIN PATH chosen; ShardScheduler-wiring SCOPING AUDIT (2026-06-08)
User picked the object-grain `ShardScheduler` path (§8.34). Scoped what wiring `387400`/`387010` as a Phase-A body
concretely requires, grounded in the caller chain (NOT memory): the per-tick fire-control iterates **per HardPoint**
— `28d400`(coordinator) → per-object `3a76b0` → `387010`(one HardPoint) → {`387400` opp-target+fire-control,
`381ff0` fire, `387170`}. So:
- **ITERATION SET (the shard work-list) = the HardPoints serviced this tick.** `387010` is the per-HardPoint entry
  (`param_1`=HardPoint, `param_2`=tick). Per schema §2 a shard owns a WHOLE GameObject (all its hardpoints) so the
  intra-object fire-budget force-sum stays bit-exact — so the WorkItem grain is the OBJECT, each running all its
  hardpoints' `387010` serially within one shard. Source = the GOM combat-object list `3a76b0` already walks.
- **PHASE-A / PHASE-B SPLIT is INSIDE `387400`/`387010`, not between them.** Phase-A (parallel, made safe by Fork B
  steps 1-3 + the §8.34 leaf audit): the opp-scan `385190` team-walk + the target DECISION. Phase-B (buffered +
  serial drain): the `3825b0` FIRE → `29f810` projectile create (Class-2 `SpawnCommand`) + the `220ed0`/`382510`
  order/target-assign emits (Class-2b `Command`). The EAW_PFIRE A4.1 create-deferral (1-shard `ShardBuffer` +
  canonical drain, already built) is the START of this split at the `3825b0` level — it must extend to cover
  `387010`'s `381ff0`/`387170` create/emit too.
- **FrozenSnapshot read-set is SMALL for the scan:** the spatial grid is already frozen during the fire-pass (§8.6),
  candidate object fields are read-only then, and the alHModel poses are settled by the step-2 pre-warm. So the
  scan likely needs NO double-buffered snapshot — it reads the live-but-frozen world. The fire half's read-set
  (muzzle matrices, weapon stats) is per-object/local. ⇒ `FrozenSnapshot` may reduce to a sweep-barrier, not a copy.
- **This IS `sim_tick_decomp_program.md` I2-I5** (the engine-source program): I2 GOM apply internals (`29f810`
  buckets + context-mgr resolution), I3 firing/projectile-spawn sub-path (LARGELY DONE via EAW_PFIRE), I4 iteration-
  set lift (capture the per-tick HardPoint/object work-list as an explicit ordered list for the ShardScheduler), I5
  assemble host Phase-A/B tick + realize the scheduler. The tick driver `28d400` is REPLACED, not reconstructed.
**FIRST BUILD INCREMENT (lowest-risk, validatable now) = I4 ITERATION-SET CAPTURE:** an observe hook on `387010`
that records the per-tick HardPoint work-list (count, order, owning object id `+0x50`, GOM index) — the
`ShardScheduler` `WorkItem{object_id, rank}` stream, captured live (DTSCAN/DIFFTRACE-shaped, observe-only). Gates:
the set is stable/enumerable, rank order matches the serial visitation (the canonical drain key I4), and the
object→hardpoint ownership is clean (shard-by-object splits no object's hardpoints). Then I5 wires the scheduler +
the extended create/emit drain. NEXT = build the I4 iteration-set capture.

### 8.36 B3.4.1 — I4 ITERATION-SET CAPTURE built + PASS; shard-by-object structure confirmed (2026-06-08, f5692b6)
Built the entry-detour on `387010` (`i4_387010_hook`, objdump-confirmed prologue + `context=*(HP+0x10)`, `tick=edx`)
recording the per-tick HardPoint work-list: `hp` count, `obj` (distinct via context transitions), `objtrans`,
`mgrtrans`, and the visitation-order-vs-object_id direction (`idup`/`iddown`/`ideq`). Observe-only, under
`EAW_DIFFTRACE=1`. **The MAIN-MENU background battle runs the full fire-control sim** (no manual battle load needed)
— it produced clean I4 lines immediately. **RESULT (stable across ticks):** `tick=1024 hp=439 obj=8 objtrans=7
iddown=7 idup=0 ideq=0 mgrtrans=0` / `tick=2048 hp=428 obj=7 objtrans=6 iddown=6 idup=0 ideq=0 mgrtrans=0`.
**All three I4 gates PASS:**
1. **Work-list enumerable:** ~430 HardPoints across 7-8 distinct objects per tick (~55 hp/object — substantial
   per-object work, good for amortizing thread sync; a larger loaded battle scales `obj` up = more shards).
2. **Shard-by-object is CLEAN:** `objtrans == obj-1` every tick ⇒ each object's hardpoints are CONTIGUOUS in the
   visitation (no interleaving) ⇒ a shard-by-object partition splits no object's hardpoints (schema §2 satisfied).
3. **Visitation runs STRICTLY DESCENDING object_id:** `iddown == objtrans`, `idup == ideq == 0` ⇒ confirms the
   head-insert master-list order (newest object first) — so sorting the Phase-B drain by object_id would REVERSE
   the create sequence; the canonical merge key MUST be the visitation RANK (the I4 box of
   `sim_parallel_command_schema.md` §7), empirically validated here. `mgrtrans=0` = single-manager population in
   this small scene (multi-manager expected in larger battles per DTWA).
⇒ the `ShardScheduler` `WorkItem{object_id, rank}` stream is live-confirmed: enumerable, cleanly object-grouped,
rank = descending-id visitation order. **NEXT = I5: wire the `ShardScheduler` over this work-list (Phase-A = the
fire-control scan/decision, already concurrent-safe via Fork B steps 1-3) + extend the Phase-B create/emit drain
(EAW_PFIRE A4.1 deferral) to the full `387010` body; validate via the DIFFTRACE position fingerprint + DTSCANOBS
parity under N-shard.**

### 8.37 B3.5.0 — I5 SCOPING: full `387010` Phase-A/B write-set audit + the in-game blocker (2026-06-08)
Audited the NON-scan parts of `387010` (decoded `381ff0`/`387170`/`385cf0`/`12d430`) to complete the write-set for
the whole per-object fire-control body (not just the scan Fork B already covered). **Findings (all grep-clean of
global/heap writes):**
- `12d430` writes a bone-USED flag on the SHARED model (`*(*(model+0xe8)+0x10)+bone` RMW `&0xfd|flag*2`) but ONLY
  on first-time bone resolution (`cached_bone_idx<0`), idempotent ⇒ same PRE-WARM class as the skeleton (step 2) —
  extend the pre-warm to resolve bone indices serially first.
- `381ff0` (turret aim-servo) writes the HardPoint's OWN angle state (`+0xa0/+0xa4/+0x94/+0x98`) and pushes the
  turret rotation to the (per-instance) model via `12d480` — object-owned, safe under shard-by-object.
- `387170`/`385cf0` = registry READS (`294bc0`) + own-state. No cross-object writes.
- The ONLY cross-object Phase-B seam remains the FIRE create/emit: `3825b0`→`29f810` projectile (Class-2
  `SpawnCommand`) + `382510`/`220ed0` orders (Class-2b `Command`) — exactly what the command system buffers.
**⇒ FEASIBILITY PROVEN END-TO-END:** the whole per-object fire-control parallelizes with object-grain shards +
{skeleton/bone pre-warm (steps 2 + this), FOG lock (step 1), RNG substream (step 3)} for Phase-A safety + the
command-buffer drain (keyed by the I4-confirmed visitation RANK) for Phase-B. No un-mitigated hazard remains; the
host `ShardScheduler`/`drain_parallel` is already determinism-gated (`sim/tests/shard_scheduler_test.cpp`).
**⛔ BUT in-game I5 (actually threading the live tick) is BLOCKED on two things, NOT on the RE:** (1) the FAN-OUT
mechanism — the hook would have to intercept the per-tick object walk (`3a76b0`←`28d400`), batch+dispatch across
workers, and SUPPRESS the binary's serial calls (replacing the tick driver — invasive, the `sim_tick_decomp_program`
I5 "assemble host tick"); (2) the VALIDATION HARNESS — the gold-standard serial-vs-parallel gate is the per-tick
DIFFTRACE POSITION FINGERPRINT, which lives in the PROFILE build that CRASHES on battle load (perf-stability notes);
DTSCANOBS covers only the scan selection, not full tick state. **So the remaining work is ENGINEERING + a harness,
not reverse-engineering — the RE question ("can the fire-control tick be parallelized, and how") is now fully
ANSWERED.** Decision (Rule 6): (a) build a lean per-tick STATE fingerprint in the stable ORACLE build (unblock the
A/B gate) as the I5 prerequisite; (b) declare the parallelization feasibility study COMPLETE (work-list
enumerable+partitionable [I4], Phase-A concurrent-safe [Fork B 1-3], Phase-B bufferable+rank-drained [cmd system +
I4], host scheduler validated) and treat the in-game threaded tick as a separate engineering milestone gated on the
harness; (c) attempt the in-game fan-out now despite the validation gap (high risk, weak gate).

### 8.38 B3.5.1 — I5 VALIDATION HARNESS v1: per-tick world fingerprint in the ORACLE build (2026-06-08)
User picked (a) — build the harness. The existing `dt_emit`/FNV fingerprint was profile-build-only (`a76b0_hook`,
crashes on battle load); rebuilt the same idea in the STABLE oracle build, folded INSIDE the I4 `387010` detour
(no new risky entry-hook). **Insight that makes the I4 site valid:** when `387010` fires for object X, X's MOVEMENT
for this tick already ran (the locomotor precedes fire-control within X's `3a76b0`), so X's position `@+0x78` is
SETTLED — folding it there captures X's reproducible this-tick position. Each distinct fire-control object's
`(object_id, pos x/y/z)` is FNV-1a hashed and accumulated **commutatively (sum)** into a per-tick world hash —
order-independent, so the eventual parallel reordering of the walk cannot perturb it. Emits `DTWORLD tick obj h`
on the 1024-tick cadence (alongside the I4 line; sampled, since a desync persists). **The determinism gate:** a
parallel fire-control tick that stays deterministic reproduces this `DTWORLD` stream on a fixed save+replay; a
desync (different targeting → different shots → divergent downstream motion) breaks the hash and STAYS broken.
Compile-green, gated `EAW_DIFFTRACE=1`. **Coverage:** fire-control objects (ships with hardpoints) — the units the
parallelization actually moves; projectiles/non-combat objects are a later completeness refinement (would need the
full GOM/registry walk, not the fire-control work-list). NEXT = validate the harness (stock save-replay → identical
`DTWORLD` stream across two runs = the determinism BASELINE), then it gates the in-game N-shard fan-out.

## 9. Cross-refs
- The blocker this answers: `inproc_integration_milestone.md` §0 + §2 (a1 PASS).
- The increment discipline this mirrors: `sim_tick_decomp_program.md` I1–I5 + the I2 gate.
- Command ops / drain key: `sim_parallel_command_schema.md` §7 (I4 box, the `(gom,rank,seq)` key a0 confirmed).
- Interfaces b5 wires: `engine_integration_contract.md` Int #1/#2/#3; host gate `sim/tests/shard_scheduler_test.cpp`.
- Already-lifted leaves to reuse: `sim/targeting_aim.*` (`20acd0`), `sim/geom_distance.*`, `sim/damped_spring.*`.
