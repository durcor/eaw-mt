# Engine-Source Integration Contract — the three boundary interfaces (2026-06-05)

**Status: build-ready interface spec, source-only to wire.** This document closes the gap between the
**host-validated** parallel-command system (`sim/sim_parallel.{h,cpp}`, `sim/command_sink.h`, `sim/sim_rng.{h,cpp}`)
and the **engine source** an actual Phase-A/Phase-B retrofit would touch. It does not introduce new
RE — every binary fact it binds to is already confirmed (`sim_parallel_command_schema.md` §10,
`threading_model.md` boundary scope) and, for the apply path, **validated in-game** by the DTWA /
DTWASIG oracles (`hooks/winmm_proxy.c`, evidence `eaw-mt.log.dtwa-obj50-pass` + `eaw-mt.log.dtwasig-pass`).

The schema doc's §10 left three items "still requires engine source": the **real `WorldApply` adapter**,
the **frozen snapshot** (boundary work-item #2), and the **shard scheduler** (boundary work-item #1).
This contract turns each into a precise interface with named binary bindings, exact signatures/offsets,
and a single remaining open-seam fix (manager-resolution), so that when source is available the work is
**filling in three named functions against a tested interface**, not re-deriving semantics.

### The "source" premise (read this first)

There is **no engine source** and none is obtainable — "source-only to build" throughout this doc means
**source we reconstruct ourselves by matching decompilation**, not source handed over by Petroglyph. That
reconstruction is *the same methodology the project's `sim/` lift already uses* (lift a function →
host-compilable C++ → bit-exact oracle), simply applied to the rest of the sim-tick closure. So the three
interfaces below are not "one increment away once source arrives" — they are the **integration points
inside a matching decompilation that does not yet exist** for the orchestration layer. The genuinely
unbuilt prerequisite is decompiling the **tick driver / orchestration glue** (`FUN_14028d400` →
`FUN_1403a76b0` → `FUN_140387010`) and the GameObjectManager internals to rebuildable form — the
cross-cutting part the project has deliberately deferred as "the Phase-B seam," which is precisely the
hard, high-volume, non-leaf work. The per-object compute bodies (the lifted behaviors/locomotors) are the
easy, largely-done part. This doc is the *target shape* of that reconstruction, not a near-term build step.

What is already built and host-tested (do not re-spec): the op types (`Command`/`SpawnCommand`/`SfxCommand`),
the `ShardBuffer` recording sink, the per-entity RNG substreams (`SimRng::substream`, removing invariant I2),
and the canonical-order `drain_parallel` merge — all passing the §9 determinism gate for N∈{1,2,4,8} ×
{round-robin, contiguous} × shuffled completion order. This contract is everything *downstream* of those.

---

## 0. The boundary in one diagram

```
  tick start
     │
     ▼
  ┌──────────────────────────────────────────────────────────┐
  │ FrozenSnapshot::acquire(world)          ── INTERFACE #2 ──│   immutable last-tick view
  └──────────────────────────────────────────────────────────┘
     │
     ▼
  ┌──────────────────────────────────────────────────────────┐
  │ ShardScheduler::run(snapshot, shards)   ── INTERFACE #3 ──│   Phase A, parallel
  │   for each owned GameObject g:                            │
  │     buffer.begin_entity(g.id)                             │   (ShardBuffer — BUILT)
  │     update(g, snapshot, /*CommandSink&*/ buffer)          │   reads snapshot, writes own-state,
  │                                                           │   EMITS cross-boundary ops
  └──────────────────────────────────────────────────────────┘
     │   barrier (all shards done; registry/grid still frozen)
     ▼
  ┌──────────────────────────────────────────────────────────┐
  │ drain_parallel(shards, WorldApply&)     ── BUILT ────────│   Phase B, serial, canonical order
  │     → WorldApply::create_object / apply_signal / flush_sfx│
  └──────────────────────────────────────────────────────────┘
                                  │
                                  ▼  EngineWorldApply  ── INTERFACE #1 ──   binds the live engine fns
  ┌──────────────────────────────────────────────────────────┐
  │ FrozenSnapshot::release()                                │   tick end → next acquire rebuilds
  └──────────────────────────────────────────────────────────┘
```

The middle box (`drain_parallel` + `ShardBuffer`) is **already built and tested**. The three labelled
interfaces are this contract.

---

## 1. INTERFACE #1 — `EngineWorldApply` : the real `WorldApply`

`sim/sim_parallel.h:109` already declares the abstract `WorldApply` (`create_object` / `apply_signal` /
`flush_sfx`); host tests implement a recording one. The engine build supplies a concrete subclass that
calls the live functions. **The binding is already in `hooks/winmm_proxy.c` (`eng_*`) and is the exact
pointer-resolution + arg layout the DTWA/DTWASIG oracles validated against the engine's own calls** —
this section promotes it from "built but not invoked" to the integration spec.

### 1.1 Bindings (RVAs relative to image base; signatures from confirmed call sites)

| Op (Class) | Live function | Signature (as called) | Result |
|---|---|---|---|
| `create_object` (2) | `FUN_14029f810` `GameObjectManager::CreateObject` | `int64_t(int64_t mgr, int64_t template_id, int32_t flags, const float pos[3], int64_t p5=0, int8_t p6=1, int8_t p7=0)` | new `GameObject*`; **id = `*(int32_t*)(obj+0x50)`** |
| `apply_signal` (2b) | `FUN_140240940` signal fan-out | `void(int64_t dispatcher, uint32_t sig_id, const void* payload)` where `dispatcher = emitter+0x38` | — |
| `flush_sfx` (3) | `FUN_1402d5290` `SFXEventManager::Start` | `int32_t(int64_t sfxmgr=imgbase+0xb27e60, int64_t sfx_id, int32_t=0, int8_t=0, uint32_t=1)` | — (off-lockstep) |

Notes that are **load-bearing for correctness**, all confirmed:
- `create_object` returns the object id via **`obj+0x50`** (the dense monotonic own-id), **never `obj+0x58`**
  (that is the owner/parent ref, shared by siblings). This is invariant **I1**; reading the wrong field
  is the exact bug that produced the retired "free-list" reading. DTWA proof: 9756/9756 `id_eq_ctr`,
  `grew1`, `mono`; `idfail=ctrfail=0`.
- `apply_signal` fires on `emitter+0x38` (the per-object `SignalDispatcherClass`), matching the live
  `FUN_140220ed0(_, emitter+0x38, …)` thunk; the global singleton (`FUN_140058570`) is **not** the
  dispatch target (it is a registry whose return is ignored by emit — `command_sink.h:34`). `payload`
  is **nullable** — DTWASIG: `pl0≈82%` of emits are bare notifications; do not assume a payload blob.
- `flush_sfx` runs **after** the lockstep merge; ordering carries no determinism weight (separate LCG
  `DAT_140a13e20`, wall-clock-sorted queue). A headless/dedicated-server `WorldApply` may drop it.

### 1.2 THE ONE OPEN SEAM — manager resolution (now specified, closes §10's ⚠️)

`eng_world_manager()` resolves a **single** global manager — the combat singleton
`mgr = *(int64_t*)( *(int64_t*)(imgbase+0xb15418) + 0x18 )`. DTWA measured this matches only **~82%** of
spawns; the other **~18%** (`mgr_fail=3422`) use a **sibling manager**, and that population correlates
**exactly** with the flag-clear (`mgr+0x63e==0`, per-manager-counter) path.

**Fix (contract requirement):** `EngineWorldApply::create_object` MUST resolve the manager **from the
requester**, not from the global singleton. Every `GameObject` carries its owning manager at **`obj+0x2b8`**
(confirmed: the constructor `FUN_140388b60:162` does `mov rcx,[obj+0x2b8]` = manager → `call FUN_1402ac980`
to allocate its id; `winmm_proxy.c:8052` independently reads `requester+0x2b8` for the same manager). So:

```cpp
uint32_t EngineWorldApply::create_object(const sim::SpawnCommand& cmd) override {
    // Resolve the manager from the REQUESTER, not a global — sibling managers exist (DTWA: ~18%).
    int64_t mgr = *(int64_t*)((int64_t)cmd.requester + 0x2b8);
    if (!mgr) mgr = eng_world_manager();          // fallback: combat singleton
    GameObject* obj = (GameObject*)CreateObject(mgr, cmd.template_id, cmd.flags, cmd.pos, 0, 1, 0);
    return obj ? *(uint32_t*)((char*)obj + 0x50)  // I1: the dense monotonic own-id
               : kInvalidObjectId;
}
```

The global-vs-per-manager **counter** selection (`mgr+0x63e` → `DAT_140b153e0+0x80` global else `mgr+0x620`
per-manager) is then handled *inside* `CreateObject` and needs no special-casing in the adapter — once the
right manager is the `this`, the right counter is selected. DTWA confirmed **both** counter paths assign
ids bit-exactly (global=6334, per-manager=3422, both pass). With requester-resolution, `mgr_fail` →
expected 0.

> **Validation hook to re-run after wiring:** the DTWA oracle's `eng_world_manager()==mgr` check
> (`winmm_proxy.c:3880`) becomes `(*(int64_t*)(requester+0x2b8))==mgr`; expect `mgr_fail=0` across a
> multi-battle capture. This is the single concrete regression test that proves the open seam closed.

---

## 2. INTERFACE #2 — `FrozenSnapshot` : the double-buffered read view (work-item #2)

Phase A workers run concurrently and must each see a **consistent last-tick world** — never another
object's mid-tick mutation (the hard RAW hazard, `threading_model.md` wall #1/#2). This is the only
piece with **no existing host stand-in** (the host tests drive units directly), so it is the largest
genuine source-side build. The contract specifies exactly *what must be frozen* and the *read discipline*.

### 2.1 The cross-object read set (what the snapshot must cover)

Phase A reads, all of which must reflect **tick-start** values:
- **Global object registry `DAT_140a16fd0`** — id-indexed `Object*` vector (`+0x20` ptr array, `+0x28`
  count; lookup `FUN_140294bc0(reg,id)`). Its *contents* (which ids exist) must not change during Phase A
  — guaranteed by the **sweep barrier** (all mutation is Class-2 `CreateObject`/destroy, deferred to
  Phase B), so the registry array itself needs no copy, only the no-mutation invariant.
- **Per-object cross-read fields** — transform (`obj+0x78/+0x7c/+0x80`), health, target ptrs, team/faction,
  visibility state. These ARE written by their owner during Phase A, so a reader of *another* object must
  see the pre-tick value ⇒ **these are the fields the double-buffer covers.**
- **Sensor grid geometry `DAT_140b31900`** (`+0xb0/+0xb4` dims, `+0xb8/+0xbc` cell sizes, `+0x40` per-observer
  bucket grids) — Class-1 writes are observer-slot-disjoint (`command_sink`/§3), so the *geometry* is read-only
  during the pass; only the no-resize invariant is needed (again the sweep barrier).
- **Predicate inputs** — `FUN_14039a2c0` (LOS/vis #10), `FUN_14035f470` (vis #4), `FUN_1402823e0` (team) read
  the above; if they read only snapshot-covered fields they are pure during Phase A.

### 2.2 Interface

```cpp
struct FrozenSnapshot {
    virtual ~FrozenSnapshot() = default;

    // Tick start: capture the double-buffer of cross-object read fields from the live world.
    // After this returns, Phase A reads cross-object data ONLY through read_*().
    virtual void acquire(World& live) = 0;

    // Cross-object reads — return the FROZEN (tick-start) value, never the live mutable field.
    virtual const ObjectState& read_object(uint32_t id) const = 0;   // transform/health/target/team
    virtual uint32_t object_count() const = 0;
    virtual const GridGeometry& read_grid() const = 0;

    // Tick end: release the read buffer; next acquire() rebuilds from the now-updated live world.
    virtual void release() = 0;
};
```

### 2.3 Read discipline (the invariant the build MUST uphold)

- A Phase-A worker reads **its own** object's fields directly from the live object (it owns them; no race).
- A Phase-A worker reads **any other** object's fields **only** via `snapshot.read_object(other_id)`.
  Reading a live cross-object field is the one defect this interface exists to prevent.
- Own-state **writes** go to the live object directly (own-granular ⇒ race-free, given object-granular
  sharding — §3).
- The registry/grid are **not mutated** during Phase A (sweep barrier; all growth is the Phase-B
  `create_object` drain). The snapshot therefore copies only the *mutable cross-read fields* (§2.1
  bullet 2), not the whole registry — keeping the double-buffer cost to the live entity working set.

### 2.4 Build cost / placement

This is boundary work-item #2 ("double-buffer all sim state read during a tick"), rated **High**. The
contract narrows it: only the §2.1 cross-read fields need double-buffering (own-state and registry
membership do not), and the source insertion point is the start of the existing per-tick sim
(`FUN_14028d400` movement-service dispatcher region) where Phase A would replace the inline per-object
update.

---

## 3. INTERFACE #3 — `ShardScheduler` : Phase-A partition + run (work-item #1)

Partitions the live objects into shards (each owning **whole** GameObjects — never splitting one object's
hardpoints, §2 of the schema doc, load-bearing for the intra-object float-sum determinism), runs each
shard's Phase-A body on a worker, and produces the per-shard `ShardBuffer`s the **already-built**
`drain_parallel` consumes.

```cpp
struct ShardScheduler {
    virtual ~ShardScheduler() = default;

    // Deterministic partition of object ids → N shards. MUST be a stable pure function of id
    // (e.g. id % N, or contiguous id ranges) so the assignment reproduces across machines (lockstep).
    virtual int shard_of(uint32_t object_id) const = 0;

    // Run Phase A: for each shard in parallel, for each owned object in registry order,
    //   buffer.begin_entity(id); update(obj, snapshot, /*CommandSink&*/ buffer);
    // Returns the per-shard buffers (borrowed by drain_parallel). No shard touches another's buffer
    // ⇒ no locks in Phase A.
    virtual std::vector<sim::ShardBuffer*> run(const FrozenSnapshot& snapshot) = 0;
};
```

The per-tick driver is then exactly the §0 diagram:

```cpp
snapshot.acquire(live);
auto shards = scheduler.run(snapshot);     // Phase A (parallel) — emits into ShardBuffers
barrier();                                  // registry/grid still frozen
sim::drain_parallel(shards, world_apply);   // Phase B (serial, canonical order) — BUILT
snapshot.release();
```

Determinism obligations the scheduler must honour (all already discharged by the built pieces, listed
here so the integrator does not re-introduce them):
- **Object-granular**: one object = one work unit; its hardpoints never split (intra-object force-sum
  `FUN_1403a76b0:44` stays serial-order-identical).
- **`begin_entity(id)` before each object** so emitted ops carry `(entity_id, seq)` — the canonical merge
  key `drain_parallel` sorts on. Visiting owned objects in **registry (id) order** within a shard makes
  the per-entity seq match the serial tick.
- **RNG via substream**: each object draws from `SimRng::substream(base_seed, id, channel, tick)` (already
  built), never the global LCG — so Phase A has no cross-thread RNG ordering (invariant I2 gone).

---

## 4. What this contract does and does NOT make drop-in

| Item | Before this contract | After |
|---|---|---|
| `WorldApply` real impl (Int. #1) | "built but not invoked", manager-resolution open | **drop-in**: exact bindings + the requester-`+0x2b8` fix specified; one regression check named |
| Manager resolution (§10 ⚠️) | open question | **resolved**: `requester+0x2b8`, with the DTWA check to confirm `mgr_fail=0` |
| `FrozenSnapshot` (Int. #2) | "requires engine source" | **scoped to a precise field set + read discipline**; still a real source build (double-buffer the §2.1 fields) — the genuine remaining work |
| `ShardScheduler` (Int. #3) | "requires engine source" | **interface fixed**; body is the existing per-object update re-pointed to read snapshot + emit into `ShardBuffer` (built) |
| Op types / RNG / drain | built + host-tested | unchanged (the middle box) |

So the residual genuine source work is **Interface #2's double-buffer** (the only piece with no host
analogue) plus **re-pointing the existing per-object update** into the Interface #3 body. Interface #1
is fully specified and in-game-validated; its only open item is now closed on paper and has a named test.

---

## 5. Acceptance gate (extends the schema doc §9)

A correct integration passes, in addition to the existing host `§9` gate (N-shard ≡ serial, substream
reproducibility, spawn-id stability — all already green):
1. **Manager-resolution regression** — DTWA capture with requester-`+0x2b8` resolution: `mgr_fail=0`
   over a multi-battle run (vs the current ~18%).
2. **In-game serial-equivalence** — with `EngineWorldApply` wired into a real (initially 1-shard)
   Phase-A/B tick, a recorded match replays bit-identically and two machines hold lockstep (the engine's
   own desync check is ground truth). 1-shard first isolates the apply-path transcription from the
   parallel partition.
3. **Snapshot consistency** — assert (debug build) that no Phase-A worker reads a live cross-object field
   outside `FrozenSnapshot::read_*` (the §2.3 invariant); a single violation is a latent desync.

---

## 6. Provenance / cross-refs

- Apply-path bindings + I1 + manager finding: `sim_parallel_command_schema.md` §6.2/§10, in-game
  evidence `eaw-mt.log.dtwa-obj50-pass` (I1, 9756/9756) + `eaw-mt.log.dtwasig-pass` (Class-2b schema
  fidelity, ~118k emits).
- Write-class characterization (the 3 classes + the snapshot/scheduler work-items #1–#5): `threading_model.md`
  "Phase-B Seam Characterization" + "Engine-Source Sim-Parallel Rewrite — Boundary Scope".
- Built host pieces this contract sits on: `sim/command_sink.h`, `sim/sim_parallel.{h,cpp}`,
  `sim/sim_rng.{h,cpp}`, `sim/tests/sim_parallel_test.cpp`.
- Live adapter prototype (the `eng_*` functions this promotes): `hooks/winmm_proxy.c` §"ENGINE-SOURCE
  INTEGRATION — Increment 1".

This is a design/interface spec — **source-only to build**, like `sim_parallel_command_schema.md`. It
introduces no new runtime claim; it formalizes the integration boundary so the remaining source work is
mechanical against a tested contract.
