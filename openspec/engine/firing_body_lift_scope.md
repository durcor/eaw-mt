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
| **b2** | **Lift the pure-numeric aim/intercept cluster** bit-exact, reading target motion from **explicit params** (snapshot-ready): `399450` (⚠ localize `DAT_140b2c380..394` scratch → thread-safe), `381dc0` (RNG→substream), `399e20`, `38ee10`, `385e70`. | aim geometry | **numeric bit-exact** (trampoline vs lifted over live input stream — the project's proven loop) | Med (global-scratch localization) |
| **b3** | **`SpawnCommand` payload + Phase-B applier** (§3) + the two Class-2b `Command` ops (`220e90`, `3a06a0`). Host-assemble: Phase-A computes payload, Phase-B drain creates+applies. | (A/B) the create restructure | host §9 gate (drain N≡serial) **+** in-game DTWA structural (projectile carries identical init fields) | Med |
| **b4** | **The fire-decision gate** reading snapshot — lift/prove-pure the predicate cascade (the §2 fan-out row). | the gate | **structural** (same fire decisions + cadence under same snapshot, long multi-battle capture) | **High (fan-out)** |
| **b5** | **1-shard in-game firing takeover** (the a1 successor): drive the lifted firing body from `sim/` on a snapshot, buffer all emits, drain canonically; DIFFTRACE structural-equivalent to the binary. Then a2 = run b5 on ≥2 shards. | the contract's 3 interfaces, live | structural DIFFTRACE vs serial binary + a1's DTWA/DTDRAIN invariants hold | High (integration) |

Pass-C `3ac530` is an **independent quick win** (self-body, render outputs deferred) — slot it as **b2′**
alongside b2 (same numeric-oracle method), not on the firing critical path.

**Dependency:** b1 → b2 → b3 → b4 → b5. b2/b2′/b3 are de-riskable in parallel; b4 is the long pole.

> **Reassessment gate — after b2 (mirrors the I2 gate).** b2 proves whether the aim/intercept cluster
> lifts clean (global-scratch localization + bit-exact). The harder question is b4's predicate fan-out: if
> b1's classification shows the gate predicates each pull deep target/manager state that itself fans out,
> the firing-body lift could balloon. **Decision point: after b1+b2, judge the b4 fan-out width against the
> 3–4× payoff before sinking b3–b5 effort.** If b4 is intractable, the fallback is to keep firing **serial**
> (a Pass between the parallel sweeps) and parallelize only the cheap mass (movement/sensor/Pass-C) — still
> a real win, and a1+Pass-C already prove that path.

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

## 7. Cross-refs
- The blocker this answers: `inproc_integration_milestone.md` §0 + §2 (a1 PASS).
- The increment discipline this mirrors: `sim_tick_decomp_program.md` I1–I5 + the I2 gate.
- Command ops / drain key: `sim_parallel_command_schema.md` §7 (I4 box, the `(gom,rank,seq)` key a0 confirmed).
- Interfaces b5 wires: `engine_integration_contract.md` Int #1/#2/#3; host gate `sim/tests/shard_scheduler_test.cpp`.
- Already-lifted leaves to reuse: `sim/targeting_aim.*` (`20acd0`), `sim/geom_distance.*`, `sim/damped_spring.*`.
