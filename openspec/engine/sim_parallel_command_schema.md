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
- **(I1) object-id append order** — `GameObjectManager::CreateObject` assigns the next id by append;
  spawns must commit in canonical order.
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
    // NO pre-assigned id — the drain calls CreateObject, which assigns the next id by append (I1).
};
```
- **Drain** = `FUN_14029f810(mgr, template, flags, &pos, ...)` in **canonical requester order**, so the
  append sequence (and thus every new object id) is identical to the serial tick. This is the only place
  (I1) is honoured.
- `CreateObject` also inserts into ~12 global spatial/category buckets and runs the observer-notify
  sweep — all of that is inside the serial drain, so it never races the Phase-A read.
- **Volume:** low (spawns + fx per tick, "a few hundred"), so the serial cost is small (matches the
  Amdahl tail estimate). Projectile spawn from the firing path (`381ff0`) routes here too.

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
| firing `381ff0` | `SpawnCommand` (projectile) | 2 |

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
- **Specified here, not built:** `SpawnCommand`, the per-entity substream `SimRng::substream`, the
  Phase-B canonical-order drain, the double-buffered frozen snapshot (boundary-scope work-item #2), and
  the object-granular shard scheduler. All require engine source.
- **Out of scope (deliberately serial):** the AI Lua pump (≈0.02ms/tick — left serial sidesteps the
  shared-`global_State` problem, boundary-scope work-item #5).

This closes the RE side of the parallelization question: the write set is enumerated, classified, and
given concrete op types + a drain that provably preserves lockstep. What remains is implementation,
which is source-only.
