# Sim-Parallel Command Schema — build-ready design (2026-06-03)

**Status: design spec, source-only to build.** This is the concrete realization of the
boundary-scope's work-items #3 (command schema + buffers) and #4 (determinism preservation) from
`threading_model.md`, turned into op/buffer types per write-target class. It consumes two prior
deliverables: the **Phase-B Seam Characterization** (the 3 write-target classes, both IN and movement
slices) in `threading_model.md`, and the already-lifted **`sim/command_sink.h`** (the per-object
signal-emission seam framing). Nothing here can be built by patching the closed binary — it specifies
what an engine-source Phase-A/Phase-B retrofit would implement. It is, however, fully **host-testable**
in the existing `sim/` harness: every Phase-A unit already emits into a `CommandSink`-style interface,
so a recording sink + a canonical-order replay validates the schema offline.

---

## 1. Model recap (the contract this schema serves)

Per-tick sim is restructured into:
- **Phase A (parallel):** each **GameObject** (not each hardpoint/component — see §2) reads a *frozen*
  snapshot of last tick's world, computes its new own-state, writes **only its own fields** directly,
  and **emits** every cross-boundary side effect as a typed op into its shard-local buffers.
- **Phase B (serial):** a single thread drains all shard buffers in **canonical entity order** and
  applies them to the live world.

The determinism gate (lockstep MP + replays + save/load) reduces — per the characterization — to two
invariants, both satisfied by the canonical-order drain:
- **(I1) object-id append order** — `GameObjectManager::CreateObject` assigns each new object its own id
  (`obj+0x50`) by **post-incrementing the active monotonic id counter** — the global counter
  `DAT_140b153e0+0x80` when the manager flag `mgr+0x63e` is set, else the per-manager counter `mgr+0x620`
  (`FUN_1402ac980`/`FUN_14028a9e0`; saturating at 0x3fffff, never reused). Ids are therefore dense,
  monotonic, and gap-free; spawns must commit in canonical order so the id↔object mapping is reproducible.
  (Confirmed in-game: §10, `eaw-mt.log.dtwa-obj50-pass` — 9756/9756 bit-exact. `obj+0x58` is a separate
  owner/parent ref, **not** the object's id.)
- **(I2) RNG draw order** — replaced outright by **per-entity substreams** (§4), so no global draw
  ordering is needed.

---

## 2. Unit of work = the whole GameObject (granularity constraint)

A shard owns whole objects. **Never split one object's hardpoints/components across threads.** This is
load-bearing for determinism, not just convenience:
- The FF energy/recharge force-sum (`FUN_1403a76b0:44` `fVar11 += fVar9`, then the proportional
  redistribute) accumulates floats across an object's *own* hardpoints. Single-threaded per object →
  bit-identical to serial. (Float add is non-associative; this is the same hazard the bit-exact oracles
  caught in #8.)
- The per-object signal dispatcher (`obj+0x38`) and own-state writes are then trivially race-free.

Sharding is by a stable partition of object id (e.g. id % N or contiguous id ranges) so the assignment
is itself deterministic and reproducible across machines.

---

## 3. Read contract & the Class-1 sweep barrier

Phase A reads, all **read-only** during the parallel pass:
- the global object registry `DAT_140a16fd0` (id-indexed `Object*` vector, `+0x20` ptr / `+0x28`
  count; lookup `FUN_140294bc0`),
- other objects' last-tick transform/health/target fields,
- the LOS/team/visibility predicates (`FUN_14039a2c0`, `FUN_14035f470`, `FUN_1402823e0`).

**Class 1 (sensor/fog — #4 UnitAI, #10 Reveal) needs NO command buffer.** Every write is keyed by the
writing observer's own unique slot (`*(int*)(observer+0x48)`) into either that observer's grid column
(`DAT_140b31900 +0x40 + slot*0x18`) or an object's per-observer array column or the behavior's own
`this[0x33]`. No two observers touch the same word. The only requirement is a **sweep barrier**: the
registry and grid geometry must not be mutated during the parallel observer pass. Since all mutation of
those globals is Class-2 (`CreateObject`/destroy), which is deferred to Phase B, the barrier is free —
Phase A is pure-read of the registry, Phase B grows it. **No op type; document the barrier and move on.**

---

## 4. RNG: per-entity substreams (replaces the global LCG)

The lockstep RNG is a single global LCG word `DAT_140a13e24`
(`s' = s*0x41c64e6d + 0xbdf`, lifted as `eaw::SimRng`). It is drawn:
- **hot**, on the per-component opportunity-target path (`FUN_140387400:250`, `FUN_1401ffb40` int-range,
  ~one draw per component per acquisition), and
- **cold**, on spawn rolls (#6 asteroid damage, one per in-range asteroid).

Serializing every global draw would put a hot dependency on the serial tail. Instead, **give each
object a deterministic substream**: `SimRng substream(entity_id, channel)` — a counter-based / split
seeding derived from `(base_seed, entity_id, channel, tick)` so draws are reproducible **without any
cross-thread ordering**. Each Phase-A object draws from its own substream; Phase B never serializes RNG.
This is a standard splittable-RNG retrofit and is the single biggest simplifier — it removes (I2) from
the drain entirely.

- **Build requirement:** the substream function must be bit-reproducible across platforms (the lockstep
  contract). A counter-based generator (e.g. an LCG/Weyl hash of the tuple, or Philox) is preferred over
  re-seeding the Park-Miller LCG, to avoid correlation between adjacent entity ids.
- **Migration check:** every `&DAT_140a13e24` call site (the `1ffb40`/`1ffbb0` family, ~the 202 sites in
  the corpus) must be rebound to `entity.rng(channel)`. The **presentation** SFX LCG `DAT_140a13e20` is a
  *separate* word and stays global/untouched (it is off-lockstep).

---

## 5. Write-class → mechanism summary

| Class | Write target | Behaviors / slice | Mechanism |
|---|---|---|---|
| 1 | sensor grid / fog arrays (observer-slot-disjoint) | #4, #10 | direct sharded write under sweep barrier — **no command** |
| 2 | `GameObjectManager::CreateObject` (registry append = id order) | #6 fx, #7 projectile, any spawn | `SpawnCommand` → serial drain in canonical order |
| 2b | per-object `+0x38` signal dispatcher; cross-entity order/target/listener edits | #4/#7/#8/#9 + movement `387400` | `Command` (extends `CommandSink::emit_signal`) → ordered replay |
| 3 | SFX event queues (`b27e60` via `2d5290`/`2d5320`→`2d5440`, LCG `a13e20`) | #8, movement | `SfxCommand` → async flush, **off lockstep** |
| — (safe) | own-object state; energy redistribute to own hardpoints (`387f50`); self-damage (`436920`/`3a9e30`) | all | direct write, no buffer (object-granular) |

Note self-damage is own-object (safe); what makes #6/#8 cross-boundary is the Class-2 fx spawn it
triggers, not the HP write.

---

## 6. Op schemas (per-shard, thread-local buffers)

Three buffers per shard. `Command`/`SfxCommand` already exist in `sim/command_sink.h`; this section
fixes their schema and adds `SpawnCommand`.

### 6.1 Class 2b — `Command` (per-object signal fan-out) — EXISTS
```
struct Command {            // sim/command_sink.h
    void*    emitter;       // GameObject* whose +0x38 dispatcher fires (drain resolves +0x38)
    uint32_t sig_id;        // SignalId: 0x20 fire-order, 0x21 opp-target, 0x29/0x2c/0x2d ability/dmg
    const void* payload;    // borrowed; nullptr for parameterless
    size_t   payload_size;
};
```
- **Drain** = `FUN_140220ed0(_, emitter+0x38, sig_id, payload)` → `FUN_140240940` fan-out.
- **Determinism (from command_sink.h):** within one emission, slots fire in `SignalListClass`
  **insertion order**; the `+0x08`/`+0x0c` reentrancy guard means a slot may emit during dispatch and
  that nesting order is part of the contract (replay, don't reorder). Across entities, emissions replay
  in **GOM/canonical entity order**.
- **Listener edits** (`FUN_1403846c0` detach = sig 0x28, connect `220e90`) are the same channel — model
  as `Command`s with the detach/connect sig_id; they mutate the target's `+0x38` list and must replay in
  emission order so subscribe/unsubscribe pairs stay consistent.
- **Target-assign** (#9 Lure `38d730`, #7 selection) writes the *victim's* order/AI state: model as a
  `Command` (or a dedicated `OrderCommand` payload) addressed to the victim; ordered replay resolves the
  aliasing case (two emitters, same victim) deterministically by emitter canonical order.

### 6.2 Class 2 — `SpawnCommand` (object creation) — NEW
```
struct SpawnCommand {
    void*    requester;     // emitting GameObject* (for ownership/team/parenting)
    uint32_t template_id;   // object-type/template (param_2 of FUN_14029f810)
    float    pos[3];        // world spawn position (param_4)
    uint32_t flags;         // param_3/param_6/param_7 packed (spawn kind: projectile, fx, ...)
    // NO pre-assigned id — the drain calls CreateObject, which post-increments the active monotonic
    // id counter (global DAT_140b153e0+0x80 if mgr+0x63e set, else per-manager mgr+0x620) to stamp
    // obj+0x50 (I1). requester also fixes WHICH manager is used (sibling managers exist — §10).
};
```
- **Drain** = `FUN_14029f810(mgr, template, flags, &pos, ...)` in **canonical requester order**, so the
  id-counter increment sequence (and thus every new object id `obj+0x50`) is identical to the serial tick.
  This is the only place (I1) is honoured.
- `CreateObject` also inserts into ~12 global spatial/category buckets and runs the observer-notify
  sweep — all of that is inside the serial drain, so it never races the Phase-A read.
- **Volume:** low (spawns + fx per tick, "a few hundred"), so the serial cost is small (matches the
  Amdahl tail estimate). Projectile spawn from the firing path (`381ff0`) routes here too.

> #### ✅ I2 — `CreateObject` apply internals VERIFIED (2026-06-05, decomp-program Increment I2)
> The "inserts into ~12 buckets + observer sweep, all inside the serial drain" claim above was
> **asserted**; I2 lifted the internals (`decomp/20a9b0.c`/`241df0.c`/`294bc0.c`/`2823e0.c`/`289010.c`/
> `769c58.c`) and confirms the apply target adds **NO determinism invariant beyond I1**:
> - **Buckets** (`20a9b0` = `ReferenceListClass::insert(ref, dedup)`): every bucket is one container
>   type — an **intrusive doubly-linked SET** (dedup-walks the ref's own membership list `ref+0x8`;
>   head-inserts a 0x28 node; `bucket+0x30` = count, `bucket+0x38` = iteration-cursor cache reset to
>   `0xfffffc19`). Membership order = insertion order = **call order**. Since the drain calls
>   `CreateObject` in canonical requester order, bucket membership is deterministic **by the serial
>   drain alone** — no extra invariant, no per-bucket sort.
> - **Id-index map** (`241df0` at `GOM+0x80` = `std::unordered_map<uint32_t id, GameObject*>::insert`,
>   Park-Miller/MINSTD hash): **keyed by `obj+0x50`**, so order-INDEPENDENT — deterministic iff the id
>   is (I1, already in-game-confirmed). The 16 conditional buckets are each gated by a template/object
>   flag (predicate table in `data_structures/movement_structs.md` §I1), not by order.
> - **Observer/fog sweep** (`294bc0` registry `at()` over `DAT_140a16fd0` {+0x20 begin,+0x28 count} →
>   `2823e0` notify-predicate → `289010`): recomputes each qualifying observer's shroud state at
>   `obs+0x404`, keyed by the observer's **own** slot `obs+0x4c` ⇒ a **Class-1 observer-disjoint** write,
>   and it runs inside the serial create so it never races Phase-A regardless.
> - **Manager = `requester+0x2b8`, CONFIRMED at the live spawn call site:** `3825b0:266` calls
>   `CreateObject(*(requester+0x2b8), …)`. This proves the contract's manager-resolution fix
>   (`engine_integration_contract.md` Int. #1 §1.2) against the real firing path and explains the
>   Increment-1 `mgr_fail` split (the manager is **per-requester**, not a global singleton — sibling
>   managers are simply different requesters' `+0x2b8`). ⇒ `EngineWorldApply::create_object` faithfully
>   **delegates** to `29f810` with `mgr = *(requester+0x2b8)`; it re-implements nothing.
>
> **Reassessment-gate verdict (program doc §5): PASS.** The GOM apply internals are *shallower* than the
> "12 buckets" feared — one list container, one keyed map, one observer-disjoint sweep — and introduce
> no new lockstep invariant. I3/I4/I5 proceed as mechanical-to-medium.

### 6.3 Class 3 — `SfxCommand` (presentation) — EXISTS, off-lockstep
```
struct SfxCommand { void* emitter; uint32_t sfx_id; };   // sim/command_sink.h
```
- **Drain** = `FUN_1402d5290(&SFXEventManager, sfx, ...)` (or `2d5320` for the 2D/3D variant). Flushed
  **after** the sim drain, or handed to the audio thread. **Not part of lockstep state**: separate LCG
  `DAT_140a13e20`, wall-clock-ordered queue `DAT_140b27e60`. Ordering affects only which sound plays,
  never sim determinism — so this buffer needs no canonical sort.

### 6.4 (framed, no tactical emitter) — `schedule_event`
The galactic/strategic `OutgoingEventQueueClass` (`DAT_140b2ed18`, ordered by (fire_time, insertion))
is a different layer; no tactical sim unit emits there. `CommandSink::schedule_event` exists for
completeness only.

---

## 7. Phase-B drain algorithm

```
barrier();                                  // all Phase-A shards done; registry/grid still frozen
for each shard in fixed shard order:        // deterministic partition order
    drain that shard's buffers, but interleave by GLOBAL canonical entity order, i.e.:
sort/merge all (Command, SpawnCommand) by (emitter_entity_id, per-entity emission seq);
for cmd in that global order:
    if SpawnCommand: CreateObject(...)       // (I1) id append order
    if Command:      emit_signal(...)         // (I2-free; insertion order within emission preserved)
flush SfxCommands (any order / audio thread); // Class 3, off lockstep
// Class 1 sensor/fog writes already applied in Phase A (sharded, no drain)
```
The merge key is `(canonical entity id, emission sequence index)` — emission seq is the per-object
monotonic counter assigned as the object emits during Phase A, making the global order equal to the
serial-tick order. No float accumulation happens in the drain (the force-sums were intra-object in
Phase A), so the drain itself introduces no FP-order divergence.

---

## 8. Per-behavior / per-slice op mapping

| Source | Emits | Class |
|---|---|---|
| #4 UnitAI sensor grid (`657ce0`/`657db0`/`659760`) | direct sharded write | 1 |
| #10 Reveal fog arrays (`365760`/`35d1b0`) | direct sharded write (observer-private) | 1 |
| #5 Nebula | nothing (read-only coupling) | — |
| #6 Asteroid | `SpawnCommand` (impact fx) + own self-damage; RNG via substream | 2 |
| #7 Targeting | `SpawnCommand` (projectile) + `Command` (target-assign/fire signals) | 2 / 2b |
| #8 Telekinesis | `SfxCommand` (grip dmg cue); own-entity pose/damage | 3 (+ safe) |
| #9 Lure | `Command` (order lured unit + AI-group edit) | 2b |
| movement `387400` | `Command` 0x20/0x21 fire-order/opp-target + `SfxCommand`; RNG via substream | 2b / 3 |
| firing `3825b0` (from `387400` fire-control) | `SpawnCommand` (projectile) + `SfxCommand` (muzzle) | 2 / 3 |
| turret-aim `381ff0` (render bone) | nothing — Class-3 presentation, `render_node_gate` only | 3 |

> #### ✅ I3 — firing → projectile-spawn emit site VERIFIED + a wrong attribution CORRECTED (2026-06-05)
> Decomp-program Increment I3. The program doc's gap (A) named `381ff0` as "firing → `3825b0` →
> Class-2 spawn". **That chain is wrong.** Ground truth:
> - **`381ff0` is the turret/hardpoint BONE-AIM** (rotates the weapon's render bone toward the target via
>   `HolsterWeaponBehaviorClass::12d480`, rate-limited with `wrap_180`); it is gated on
>   `owner_record.render_node_gate (+0x4e) == 1`, writes only render-bone matrices, and **does NOT call
>   `3825b0`**. ⇒ **Class-3 presentation**, off-lockstep; the bone-cache leaves
>   (`385cf0`/`12d2a0`/`12d430`/`12d2c0`/`12d480`) are render-path and **out of the sim Phase-A**.
> - **The real projectile spawn is `387400` (fire-control) → `3825b0` → `29f810`** (`3825b0:266`). Across
>   its whole 4034-byte body there is **exactly one** `29f810` (the projectile) and **exactly one**
>   `2d5240`→`DAT_140b27e60` (the muzzle SFX queue); the target (`param_2`) is only ever **read** (sim pos
>   `+0x78/+0x80`). **No cross-entity write, no new invariant** — the firing emit is precisely
>   **`SpawnCommand` (Class 2) + `SfxCommand` (Class 3)**, both already in the schema.
> - **SpawnCommand payload:** `requester` = firer hardpoint-ctx (manager `+0x2b8`, id `+0x50`); `template`
>   = weapon/projectile def; `flags` = `registry[firer.type_index]+0x4c`; `pos`/orientation from the
>   firing solution (`399450` intercept → `381dc0` lead → `20acd0` direction→Euler — **`20acd0` is the
>   already-lifted #7 aim-geometry**). Post-create self-init (owner-id = firer `object_id`, damage, speed,
>   lifetime) writes only the NEW object → part of the same serial Class-2 apply.
> - **Fire decision is per-firer/shardable:** reads the firer's own state + the **target's frozen** state
>   (pos, flags, team). The render-node firing-ARC gate (`3825b0:112-146`, `385cf0`+`12d2c0` bone matrix)
>   is dead in the tactical sim (`+0x4e==0` for all serviced components per Phase-2) → uniformly skipped,
>   deterministic. The liftable numeric core of the firing body is the intercept solution `399450`/`381dc0`
>   (same category as the lifted #7 `20acd0`, which it reuses); `3825b0`'s outer range/LOS/ammo logic is
>   per-firer decision orchestration (the deferred emit-FSM seam, shards cleanly by firer).
>
> Net: gap (A) closes as **two existing command types, no new invariant**; the read-set gains the
> target's frozen fields (feeds Int. #2 / D-gap). In-game confirmation = DIFFTRACE fire-cadence + DTWA on
> the projectile spawn (optional — the emit reduces to validated Class-2/Class-3 paths).

---

## 9. Determinism test gate (the acceptance bar)

A correct implementation must pass, in addition to the existing per-unit bit-exact oracles:
1. **Serial-equivalence:** N-shard Phase-A/B output is bit-identical to the single-threaded tick, over a
   long replay, for N ∈ {1,2,4,8}. (The recording-sink + canonical replay already lets this run on host.)
2. **Replay/MP determinism:** a recorded match replays bit-identically; two machines stay in lockstep
   (the existing engine desync check is the ground truth).
3. **Substream reproducibility:** `substream(id, channel)` yields identical sequences cross-platform.
4. **Spawn-id stability:** object ids assigned across a parallel tick equal the serial-tick ids.

---

## 10. Build status

- **Lifted / host-testable now:** the Phase-A compute units (#1–#10 numeric cores, locomotors, spine,
  hardpoint, RNG `eaw::SimRng`) and the `CommandSink` emission seam (`Command`/`SfxCommand`).
- **✅ BUILT (2026-06-04, host-validated — this doc's command system):**
  - `SpawnCommand` op + `CommandSink::emit_spawn` (`sim/command_sink.h`) — §6.2.
  - `SimRng::substream(base_seed, entity_id, channel, tick)` per-entity substream (`sim/sim_rng.{h,cpp}`),
    SplitMix64-seeded, removing invariant I2 — §4. NB: a determinism *retrofit* (changes the numbers
    vs the global-LCG binary), so its contract is reproducibility, not bit-equivalence to the original.
  - `ShardBuffer` (a `CommandSink` that tags each op `(entity_id, seq)`) + the Phase-B
    `drain_parallel` canonical-order merge/apply (`sim/sim_parallel.{h,cpp}`) — §7.
  - The §9 determinism gate as a host test (`sim/tests/sim_parallel_test.cpp`, in `just sim-test`):
    N-shard ≡ serial bit-identical for N∈{1,2,4,8} × {round-robin, contiguous} × shuffled order;
    spawn-id stability; replay-determinism proxy; substream reproducibility/order-independence.
- **🟡 Real `WorldApply` adapter + in-game oracle — BUILT & RUN (2026-06-05, Increment 1, hook-side,
  no live mutation; `hooks/winmm_proxy.c` DTWA, evidence `eaw-mt.log.dtwa-obj50-pass` [spawn/I1] +
  `eaw-mt.log.dtwasig-pass` [signal/schema]):** the adapter
  (`eng_world_manager`/`eng_create_object`/`eng_apply_signal`/`eng_flush_sfx`) resolves the live
  manager and drives `FUN_14029f810` / `FUN_140240940` / `FUN_1402d5290`; the DTWA-SPAWN/DTWASIG entry
  detours validate it against the engine's own calls. Findings (multi-tick, ~9.7k spawns / ~118k signals):
  - ✅ **Schema fidelity** — `schemafit=100%` (every `CreateObject` carries a readable `pos`).
    DTWASIG (clean isolated re-capture 2026-06-05, `eaw-mt.log.dtwasig-pass`, ~118k emits over a
    multi-battle run): every Class-2b `Command` field is **losslessly recoverable** — `emok=100%`
    (dispatcher always a real ptr ⇒ `emitter = dispatcher-0x38` always computable), `sigok=100%`
    (`sig_id` always a small enum), `disp0=0`. `sigover=0` (≤17 distinct ids, well under the table).
    **Two refinements vs the model:**
    - **`payload` is NULLABLE** — `pl0≈82%` of emits carry no payload blob; the dominant ids are bare
      notifications (`0x0`, `0x10`, `0x28`, `0x18`). `Command.payload` (a nullable pointer) represents
      this faithfully; an emit is not required to carry a payload.
    - **`240940` is a *general* signal bus, not just the Class-2b gameplay channel** — the modeled
      gameplay-order ids are a **subset** of live traffic. Observed histogram (per-1024-tick):
      `{0:33812, 10:29084, 28:29079, 18:20905, 20:4042, 21:428, 2f:232, 1e:161, 9:121, b:96, a:77,
      1c:15, 19:11, 16:1, 29:1, 25:1, 24:1}` — the modeled set `{1c,20,21,28,29}` is present (fire-order
      `0x20` and opp-target `0x21` carry payloads and grow with combat), interleaved with engine-internal
      notifications. The schema models the gameplay-order subset; the bus carries more, and the three
      fields are recoverable for **all** of them. Canonical emission order = the live fan-out's
      insertion-order walk (established by the `240940` lift itself, `sim/signal_dispatch.{h,cpp}`); not
      separately re-measured here, so the order-fidelity check is satisfied by the lift, not by DTWASIG.
  - ⚠️ **Manager is multi-instance** — `eng_world_manager()` (=`*(combat+0x18)`) matches only ~82% of
    battle spawns; ~18% use a sibling manager (`DTWAMGR`: engine `0x35878ff0` vs resolved `0x358705e0`,
    ~0x89a0 apart). ⇒ the real `WorldApply::create_object` must resolve the manager from the
    requester/context, not a single global.
  - ✅ **I1 confirmed — object ids are a dense, monotonic, gap-free allocation** (`obj+0x50`). The
    constructor `FUN_140388b60` stamps two distinct identities, and an earlier DTWA round read the wrong
    one (`obj+0x58`), which is why it falsely reported `i1_c20=0 / reused≈100% / grew1=0` and prompted a
    now-retired "free-list pop order" reading. The two fields:
    - **`obj+0x50`** (`plVar4[10]`) = the object's *own* unique id. Allocated by `FUN_1402ac980(manager)`
      (388b60:162; objdump `388f67`: `mov rcx,[obj+0x2b8]`=manager → `call 2ac980`) — a **post-increment
      monotonic counter** selected by the manager flag `mgr+0x63e`: flag SET → delegate to
      `FUN_14028a9e0(&DAT_140b153e0)`, post-incrementing the **global** counter `DAT_140b153e0+0x80`
      (RVA 0xb15460; decomp/28a9e0.c, process-wide-unique ids); flag CLEAR → post-increment the
      **per-manager** counter `mgr+0x620` (decomp/2ac980.c). Both saturate at `0x3fffff` and never reuse.
      It is the key of the canonical id→object `unordered_map` at `manager+0x80` (insert `FUN_140241df0`,
      MINSTD-hashed key=`obj+0x50`, value=obj; 29f810.c:54-56, decomp/241df0.c).
    - **`obj+0x58`** (`plVar4[0xb]`) = CreateObject's **param_3**, set verbatim from the caller (388b60:44);
      the hot projectile spawner sources it from a *parent* object's `+0x4c` (386660.c:93,111) — an
      **owner/parent ref**, shared by all siblings (→ `reused≈100%`), indexing the per-owner tables at
      `manager+0xc8` (0x48 stride, 29f810.c:61) / `manager+0xd0` (0x38 stride, :72). **Not** the object's id.
    - **In-game proof (2026-06-05, DTWA re-pointed to `obj+0x50`, evidence `eaw-mt.log.dtwa-obj50-pass`):**
      snapshotting the active counter before/after each real `CreateObject`, over **9,756 spawns @ tick 1024**:
      `id_eq_ctr=9756 grew1=9756 mono=9756 idfail=0 ctrfail=0 null=0` — every own id equals the counter's
      pre-increment value, the counter advances by exactly one, `own_id` runs strictly +1 gap-free
      (162169→…). Both counter paths fired (global=6334, per-manager=3422) and **both passed**. (§4/§6.2
      updated to this mechanism; this resolves the prior `❌`/free-list TODO.)
    - **Manager-resolution caveat sharpened:** `mgr_fail=3422` correlated **exactly** with the per-manager
      (flag-clear) population, while every global-path spawn resolved the combat singleton cleanly. ⇒ the
      sibling-manager mismatch (bullet above) is precisely the flag-clear spawns; `WorldApply` must resolve
      the manager from the requester/context for those.
- **Still requires engine source (not built):** the double-buffered frozen snapshot (boundary-scope
  work-item #2) and the object-granular shard scheduler. (The `WorldApply` real adapter now exists
  hook-side and is in-game-validated for schema fidelity; manager-resolution + the I1 restatement are
  the open items above.) **→ These three items are now turned into a build-ready drop-in interface spec
  with named binary bindings, exact signatures/offsets, and the manager-resolution fix closed on paper
  (resolve from `requester+0x2b8`) in `engine_integration_contract.md`.**
- **Out of scope (deliberately serial):** the AI Lua pump (≈0.02ms/tick — left serial sidesteps the
  shared-`global_State` problem, boundary-scope work-item #5).

This closes the RE side of the parallelization question: the write set is enumerated, classified, and
given concrete op types + a drain that provably preserves lockstep. The op types, the per-entity RNG,
and the canonical-order drain are now built and host-validated; what remains (snapshot, scheduler, the
real `WorldApply`) is engine-source integration.
