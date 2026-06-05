# Sim-Tick Decompilation Program — scope for a rebuildable parallel sim (2026-06-05)

**Status: program scope / increment plan.** The engine-integration contract (`engine_integration_contract.md`)
established that there is **no engine source** — "source" can only mean a **matching decompilation we
produce ourselves**, and the genuine prerequisite for the parallel sim is reconstructing the sim-tick
closure to rebuildable form. The user's framing — "we'd have to decompile everything" — is the right
instinct but **overshoots**. This document scopes what actually has to be reconstructed, what is already
in hand, and an ordered, individually-validated increment plan.

---

## 0. The reframe — "decompile everything" overshoots

The parallel rewrite **replaces** the tick driver; it does not preserve it. So the orchestration loop
(`FUN_14028d400` gsvc → the indirect service dispatch → the per-object iteration) is **thrown away**, not
faithfully decompiled. What replaces it is `ShardScheduler::run` + `drain_parallel` — both already designed
(contract Int. #3) and the drain already built. You therefore do **not** need a rebuildable `28d400`.

```
  WHAT GETS THROWN AWAY (don't faithfully reconstruct):
    FUN_14028d400  gsvc driver  ── the service-rate decrement, the network-vote/PENDING_TACTICAL_BATTLE_VOTE
                                    branch, the OutgoingEventQueue emit, the inline per-object iteration
    the indirect service-dispatch vfunc[+0xb0]  ── replaced by ShardScheduler's explicit owned-object loop

  WHAT MUST BE RECONSTRUCTED (rebuildable, oracle-validated):
    (A) the per-object Phase-A BODY            ── compute new own-state + emit cross-boundary ops
    (B) the Class-2/2b APPLY targets           ── what WorldApply::create_object / apply_signal actually do
    (C) the ITERATION SET + ORDER the driver visited  ── as data for ShardScheduler (NOT the loop code)
    (D) the cross-object READ-SET struct layout ── what FrozenSnapshot must double-buffer
```

This is the whole reason the rewrite is *bounded*: you reconstruct the leaves and the apply targets and
the data layout, and you **rewrite** the glue rather than decompile it. The expensive per-tick mass (the
per-object math) is exactly the part that is already largely lifted.

---

## 1. What is already in hand (do NOT redo)

| Asset | State | Source |
|---|---|---|
| Per-object compute bodies — the 10 IN/OUT behaviors, locomotors, entity spine, **hardpoint fire-budget `FUN_1403a76b0` incl. the order-sensitive force-sum** + `387f50` consume/fire + `540070` weight + `387170` | **lifted, bit-exact** | `sim/hardpoint.{h,cpp}`, `sim/*.cpp`, schema §10 |
| Targeting / opportunity body `FUN_140387400` | **lifted** (dispatch + aim) | `sim/targeting_*.{h,cpp}` |
| Per-entity RNG substreams (removes invariant I2), command ops, `ShardBuffer`, `drain_parallel` | **built, host-tested** (§9 gate green) | `sim/sim_rng.*`, `sim/sim_parallel.*`, `sim/command_sink.h` |
| **DIFFTRACE structural oracle** — per-tick FNV-1a fingerprint over the serviced component set + order + cadence; enumeration mirrors `3a76b0` exactly | **built (v1)**; needs the transform-value field set added once struct layout is complete | `hooks/winmm_proxy.c` `a76b0_hook`, `decomp_plan.md` Phase-1 |
| GOM id allocator + invariant **I1** (`obj+0x50`, dual counter `mgr+0x620`/global) | **decoded + in-game-validated** (DTWA 9756/9756) | schema §10, `decomp/2ac980.c`/`28a9e0.c` |
| RTTI symbols (523 classes, 1992 vtables, 5207 `vfunc_<slot>`) + struct-DB tooling + batch decompiler | **built** | `decomp_plan.md` |

The headline: **the per-object Phase-A body — items (A) and the force-sum determinism crux — is already
reconstructed.** The fire-budget note even found the game-speed weight **cancels out of every share**
(`sim/hardpoint.h:30`), so the flagged float-accumulation order hazard is already neutralized at the
math level, not just by object-granular sharding.

---

## 2. The genuine remaining closure (the gap list)

Four gaps, all bounded:

- **(A-gap) The firing → projectile-spawn sub-path** — ✅ **I3 RESOLVED 2026-06-05, and the original
  chain here was WRONG.** `FUN_140381ff0` is NOT the spawn — it is the **turret BONE-AIM** (Class-3 render
  presentation, gated on `render_node_gate +0x4e==1`, writes render-bone matrices via `12d480`; the
  bone-cache leaves `385cf0`/`12d2a0`/`12d430`/`12d2c0` are render-path, **out of the sim Phase-A**). The
  real projectile spawn is **`FUN_140387400` (fire-control) → `FUN_1403825b0` → `FUN_14029f810`**
  (`3825b0:266`). Its entire 4034-byte body makes **exactly one** Class-2 spawn + **one** Class-3 muzzle
  SFX, with the target read-only ⇒ **no cross-entity write, no new invariant** (= existing `SpawnCommand`
  + `SfxCommand`). Liftable numeric core = the intercept solution `399450`/`381dc0` (reuses the lifted #7
  `20acd0`). See `sim_parallel_command_schema.md` §8 I3 box.
- **(B-gap) The Class-2 apply internals** — `FUN_14029f810` `CreateObject` beyond the id allocator:
  `FUN_14020a9b0` (the bucket-insert primitive, **not yet in `decomp/`**, called ~12× into
  `mgr+0x38/+0x408/+0xc8[slot]/+0x1c8/+0x210/+0x258/+0x2a0/+0x2e8/...`), `FUN_140769c58` (alloc),
  finish `FUN_140388b60` (construct), and the observer-notify sweep `294bc0`/`2823e0`/`289010`. This is
  what `EngineWorldApply::create_object` must faithfully be.
- **(C-gap) The iteration set + order as data** — ✅ **I4 RESOLVED 2026-06-05.** The tick visits
  `for gom in (mgr[3], mgr[4])` (`3639d0`, fixed order) → **Pass A** master_update_list (`GOM+0xe8`,
  HEAD→sentinel walk) `3a6b80` per entity [the Phase-A parallel set] → barrier → **Pass C**
  second_update_list (`GOM+0x130`) `3ac530` DynamicTransform → **Pass D** drain `creation_params`
  (`GOM+0x5f0`, reverse-vector, native deferred-create). **Key finding: the canonical drain key is the
  Pass-A iteration RANK, not `object_id`** (master list is head-inserted → walk ≈ descending id; sorting
  the drain by ascending id would reverse the create order). The `28d400` indirect-dispatch is a single
  mode-service call (no fan-out). See `sim_parallel_command_schema.md` §7 I4 box.
- **(D-gap) The cross-object read-set struct layout** — the fields a Phase-A worker reads off *other*
  objects (transform `+0x78/+0x7c/+0x80`, health, target ptrs, team, `HardPointOwnerRecord.motion_state`/
  `render_node_gate`) must be laid out as named struct fields so `FrozenSnapshot` can double-buffer them.
  Partially done (Phase-2 structs); needs completion for the exact snapshot field set.

Everything else in the chain is either already lifted (item 1) or deliberately replaced (§0).

---

## 3. Methodology — two oracle types, and the byte-matching decision

**Decision: target functional-equivalence rebuildable source, NOT a byte-identical matching decomp.**
A re3/OpenRCT2-style decomp that recompiles to the same bytes is ~10× the effort and buys nothing for the
parallel-sim goal — we need source that *behaves* identically under the determinism gate, not source that
*assembles* identically. This single scoping choice is what keeps the program tractable.

Validation uses **two oracle types**, both already established here:
1. **Numeric bit-exact** (leaves / arithmetic) — feed captured inputs, compare float/int outputs bit-for-bit
   vs the trampolined original. Used for every lifted behavior; applies to any arithmetic sub-leaf in the gaps.
2. **Structural / side-effect trace** (orchestration / side-effecting functions) — you cannot compare a
   return value, so compare the **sequence of observable effects**: which sub-calls fire, in what order,
   with what args, and the resulting state fingerprint. **This is exactly what DIFFTRACE already does** for
   the serviced set; it generalizes to "does the reconstruction emit the same `(entity_id, seq, op)` stream
   the binary did." A reconstruction passes when its trace == the binary's over a long multi-battle capture.
   ⚠️ Honest caveat: this proves *observable* equivalence, not identical internal computation — weaker than
   numeric bit-exact. For the lockstep contract that is sufficient (lockstep only observes the op stream +
   id allocation), but it must be stated, not assumed.

---

## 4. Increment plan

Ordered by dependency and by de-risking the hardest apply piece early. Each increment is independently
host-compilable and oracle-validated — the project's proven loop.

| # | Increment | Closes | Oracle | Risk | Done-criterion |
|---|---|---|---|---|---|
| **I1** ✅ | **Struct completion for the closure** — lay out `GameObjectClass` (the `+0x38` service ptr, `+0x2b8` mgr, hardpoints, `+0x3a0` flags, transform `+0x78..`), `HardPointClass`, `HardPointOwnerRecord`, `GameObjectManager` (the `29f810` bucket offsets + `+0x620`/`+0x63e`/`+0x80`) into the struct DB. Unlocks readable decompiles for all gaps **and** DIFFTRACE transform-value coverage (D-gap). | (D) + readability | n/a (enables) | Low (mechanical, tooling exists) | **DONE 2026-06-05** — +20 named fields applied (`Phase2ApplyStructs.java`); GOM-create / id-allocator cluster ground-truthed (`obj+0x50` id, `obj+0x2b8` mgr, GOM bucket offsets, `+0x620`/`+0x63e` counters) and verified in `decomp/29f810.c`/`388b60.c`/`3825b0.c`. See `data_structures/movement_structs.md` §I1. |
| **I2** ✅ | **GOM apply internals** — lift `FUN_14020a9b0` (bucket-insert), the ~12 bucket layouts, `769c58` alloc, finish `388b60` construct, observer-sweep `294bc0`/`2823e0`/`289010`. Realize `EngineWorldApply::create_object` faithfully (incl. the contract's `requester+0x2b8` manager resolution). | (B) | DTWA extended to **bucket membership** (new object appears in the same ~12 buckets the binary inserted it into) + existing I1 | **Med** (12 bucket types) | **DONE 2026-06-05 (static)** — all buckets = one type (`ReferenceListClass::insert`, head-insert SET); id-map keyed by `obj+0x50` (order-independent); observer sweep observer-slot-disjoint; manager = `requester+0x2b8` confirmed at `3825b0:266`. **NO invariant beyond I1; create_object DELEGATES to `29f810`.** `schema §6.2` verification box. In-game bucket-membership oracle = OPTIONAL belt-and-suspenders (membership is reproduced by construction since we call the same fn). |
| **I3** ✅ | **Firing / projectile-spawn sub-path** — ~~lift `381ff0`~~ (corrected: `381ff0` = render turret-aim, out of scope) — characterize `3825b0` (the real spawn, from `387400`); route projectile creation through `emit_spawn` (Class 2). | (A) | DIFFTRACE structural (same fire decisions/cadence under same state) + DTWA spawn for the projectile | Med | **DONE 2026-06-05 (static)** — firing emit = `SpawnCommand` (Class 2) + `SfxCommand` (Class 3), target read-only, **no cross-entity write / no new invariant**; manager `requester+0x2b8` (reconfirms I2). Numeric core = `399450`/`381dc0` (reuses #7 `20acd0`). In-game DTWA/DIFFTRACE = optional confirmation. `schema §8` I3 box. |
| **I4** ✅ | **Iteration-set lift** — turn the driver's serviced-set + order into an explicit ordered owned-object list for `ShardScheduler::run` (characterize, don't reconstruct `28d400`). Resolve the indirect service-dispatch only far enough to know *what* is iterated and *in what order*. | (C) | DIFFTRACE serviced-set + order match | **Med-High** (indirect-dispatch fan-out — but DIFFTRACE already pins the empirical set) | **DONE 2026-06-05** — explicit structure: `(mgr[3],mgr[4])` × {Pass-A master-list `3a6b80` ∥ → barrier → Pass-C second-list `3ac530` ∥ → Pass-D deferred-create drain}. Fan-out risk NOT realized (single mode-service `vfunc+0xb0`). **Surfaced + FIXED: canonical drain key = iteration RANK not `object_id`** (head-insert ⇒ walk ≈ descending id); §7 restated (sign-off 2026-06-05) to `(gom_index, emitter_iteration_rank, seq)`. `schema §7` I4 box. |
| **I5** ✅ host | **Assemble the host Phase-A/B tick** on the reconstruction + realize `FrozenSnapshot` (Int. #2) and `ShardScheduler` (Int. #3) from the contract; wire `EngineWorldApply` (Int. #1). | the contract's 3 interfaces | §9 determinism gate (N∈{1,2,4,8} ≡ serial) **+** in-game DIFFTRACE equivalence vs the serial binary; contract §5 gate | High (integration) | **HOST DONE 2026-06-05** — `sim/frozen_snapshot.h` (Int #2) + `sim/shard_scheduler.h` (Int #3) + assembled two-GOM tick; `shard_scheduler_test` ALL PASS: N∈{1,2,4,8}≡serial under shuffled processing, rank≠id negative control, shared-counter GOM ordering, snapshot RAW-freedom; drain key now `(gom_index, rank, seq)`. **Still open: wire the REAL `EngineWorldApply` into a live 1-shard in-game tick + in-game DIFFTRACE-vs-serial (needs hook integration).** |

**Dependency:** I1 → {I2, I3, I4} (all need the structs) → I5. I2/I3/I4 are independent of each other and
can proceed in any order (or in parallel sessions).

---

## 5. Risks, caps, and the decision gate

**Caps (do NOT let the closure expand):**
- **Firing → projectile → collision → damage is a deep sub-closure — cap it.** I3 reconstructs firing up
  to the projectile **spawn emit** only. The projectile's *own* subsequent tick recurses through the same
  per-object machinery (it is just another GameObject), and collision broad-phase + damage application are
  **separate later closures**, not part of this program.
- **Particle/FX sim and audio are out** (Class-3, off-lockstep — already peeled off).
- **AI Lua pump is out** (kept serial per the boundary scope; the reflection-dispatcher write surface is
  intractable and the measured cost ~0.02ms/tick makes serial painless).
- **Byte-matching decomp is out** (§3) — functional equivalence only.

**Risks:**
1. **I4 indirect-dispatch fan-out** (highest) — if the service dispatch resolves to many per-entity-type
   concrete vfuncs, characterizing the iteration could fan out. *Mitigant:* DIFFTRACE already enumerates the
   serviced set empirically, so the iteration is known as *data* even where the static call graph has no
   edge — I4 is "lift the observed set to an explicit list," not "trace every vtable."
2. **Structural-oracle weakness** (§3 caveat) — observable-equivalence, not internal-equivalence. Acceptable
   for lockstep, but a reconstruction could pass DIFFTRACE while computing internals differently; guard with
   a *long, varied* capture (multi-battle, multi-faction) so the observable surface is wide.
3. **I2 bucket volume** — 12 bucket types, each a container whose layout must be recovered; mechanical but
   not trivial. The id-allocator (the hard determinism part) is already done.

**Decision gate — reassess after I2.** I2 reconstructs the `WorldApply::create_object` apply target — the
single hardest piece and the one the whole drain depends on. If I2 lands clean (bucket membership validates),
I3–I5 are mechanical-to-medium and the program is worth finishing. If I2 reveals the GOM internals are far
deeper than the 12 buckets suggest, that is the point to stop and re-judge the 3–4× fast-forward payoff
against the remaining cost — **before** sinking I3–I5 effort.

> **✅ GATE RESULT (2026-06-05): PASS — proceed.** I2 (static lift) found the GOM apply internals
> *shallower* than the "12 bucket types" feared: there is **one** bucket container type
> (`ReferenceListClass::insert`, an intrusive head-insert SET), **one** keyed map (`unordered_map` on
> `obj+0x50`, order-independent), and **one** observer-disjoint fog sweep — and together they add **no
> determinism invariant beyond I1** (already in-game-confirmed). The hardest apply piece is done and the
> `create_object` adapter merely **delegates** to `29f810` with `mgr = *(requester+0x2b8)`. I3/I4/I5 stand
> as mechanical-to-medium; the 3–4× fast-forward payoff is intact. The optional in-game bucket-membership
> oracle would only re-confirm a delegation that reproduces membership by construction.

---

## 6. Cross-refs

- Target architecture the reconstruction plugs into: `engine_integration_contract.md` (Int. #1/#2/#3).
- Write-class characterization + the apply bindings: `sim_parallel_command_schema.md` §6/§10, `threading_model.md`.
- Decompilation tooling (RTTI, struct DB, batch decompiler, DIFFTRACE): `decomp_plan.md`.
- Already-lifted Phase-A bodies: `sim/` (`hardpoint`, `targeting_*`, locomotors, behaviors).

This scopes the parallel-sim decompilation as a **bounded, 5-increment program** — not "decompile
everything." The expensive per-object mass is already reconstructed; the remaining work is the apply
targets (I2), the one unlifted Phase-A emit path (I3), the iteration set as data (I4), and the assembly
(I5) — each validated by an oracle type the project already runs, with a hard reassessment gate after I2.
