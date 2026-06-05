# In-Process Integration — the (a)-minimal milestone scope (2026-06-05)

**Status: milestone scope.** The host parallel pipeline is complete and §9-gate-green (Increment I5:
`sim/frozen_snapshot.h` + `sim/shard_scheduler.h` + the assembled two-GOM tick + the `(gom_index,
emitter_iteration_rank, seq)` drain). The remaining question is the only one a host test cannot answer:
**does the restructure hold against the LIVE engine?** This doc scopes the cheapest step that begins to
answer it — and surfaces a blocker found during scoping that reshapes what "minimal" means.

---

## 0. The blocker found while scoping (why "emit-redirection with binary bodies" is not the first step)

The tempting first step was: keep the binary's per-object body (`FUN_1403a6b80`), flip the existing
`29f810`/`240940`/`2d5290` hooks from *observe* to *buffer-and-defer*, run 2 shards, drain canonically.
Two facts kill that as a *first* step:

1. **Inline-consumed creates.** The firing path consumes the create's result immediately:
   `plVar12 = 29f810(...); plVar12[0x1d]+0x58 = firer_id; …damage…speed…lifetime` (`3825b0:266-324`).
   If the hook *defers* the create, there is no object to initialize — the binary's own follow-on writes
   corrupt memory / crash. Deferral is only safe for **fire-and-forget** emits (signals, SFX) whose
   return value the caller ignores. So "buffer the creates" requires the firing body to be **lifted and
   restructured** (Strategy 2), not just intercepted.
2. **Live cross-object reads on threads.** The binary body reads other objects' *live* fields
   (`target+0x78` etc.), not a snapshot. Running it on ≥2 threads re-introduces exactly the RAW race the
   `FrozenSnapshot` exists to prevent — and you cannot retrofit the snapshot into compiled binary code.

⇒ **True in-game multi-shard needs lifted Phase-A bodies.** That is a large, later effort. The *minimal*
in-game step must therefore be something that (i) validates the restructure's correctness claim against
live data, (ii) carries **no** control-flow risk, and (iii) does not need any new lift. That is a
**passive drain-order oracle**, not a control-flow takeover.

---

## 1. Milestone a0 — `DT-DRAIN`: confirm the canonical key reproduces live serial order (DO THIS)

The host gate proved, with a toy, that the `(gom_index, rank, seq)` key reproduces the serial tick and
that an `object_id` key does **not** (the negative control). a0 is the **in-game** version of that exact
test — observe-only, the same risk class as every shipped DT* oracle.

### 1.1 What it measures
For every Class-2 spawn (`29f810`) and Class-2b signal (`240940`) emitted during a real tick, record:
- **`emission_ordinal`** — a global monotonic counter: the order the binary *actually* emitted (ground truth).
- **`gom_index`** — which GOM is being serviced (0 = `mgr[3]`, 1 = `mgr[4]`).
- **`emitter_rank`** — the emitter's visitation rank in this tick's Pass-A walk.
- **`emitter_object_id`** — the emitter's `obj+0x50` (the field a naive drain would sort on).
- **`seq`** — the per-emitter emission counter within the tick.

### 1.2 The headline checks (offline, over a multi-battle capture)
1. **Key-correct:** sorting the recorded emits by `(gom_index, emitter_rank, seq)` reproduces
   `emission_ordinal` order **exactly** (zero inversions). ⇒ the §7 drain key reproduces the live serial
   emission order.
2. **Negative control (the load-bearing one):** sorting by `(emitter_object_id, seq)` does **NOT** match
   `emission_ordinal` — there are inversions, because the master list is head-inserted so visitation ≈
   descending id (I4). ⇒ confirms in-game that a drain keyed by `object_id` would desync. This is the
   in-game analogue of `shard_scheduler_test`'s negative control.
3. **GOM order:** all GOM-0 emits' ordinals precede all GOM-1 emits' (within a tick) — confirms the
   fixed GOM service order the global id counter depends on.

### 1.3 Mechanism (extends existing hooks; no new control flow)
- **Rank + gom counter:** the per-entity hook `b3a76b0_hook` (already installed) fires once per serviced
  ship in visitation order. On each new tick (detected exactly as `a76b0_hook` does, `g != g_dt_tick`),
  reset `rank=0`; per entity, stamp a thread-local `g_cur_rank++`, `g_cur_emitter=ship`,
  `g_cur_object_id=*(int*)(ship+0x50)`, and set `g_cur_gom` from the GOM being walked (resolve via the
  `2be640` hook / `ship+0x2b8`). (Caveat: `3a6b80` is the *behaviour+hardpoint* per-entity body; confirm
  it is the same visitation as the master-list walk — it is, per I4/`2be640:63-72`.)
- **Capture at emit:** the existing DTWA-SPAWN (`29f810`) and DTWASIG (`240940`) detours already fire on
  each emit. Add: append `{emission_ordinal++, g_cur_gom, g_cur_rank, g_cur_object_id, seq}` to a
  per-tick log line (reuse the DIFFTRACE log channel + the lean `build-winmm-oracle`).
- **Offline checker:** a tiny host script reads the capture and runs the three §1.2 checks. (Could even
  reuse the `shard_scheduler_test` comparator structure.)

### 1.4 Why a0 is the right minimal step
- **Zero control-flow risk** — observe-only, identical risk class to DTWA/DTWASIG (already shipped).
- **Validates the I4/I5 central claim on live data** — the one thing the host gate could only assert with
  a toy: that the canonical key, *and only the rank key*, reproduces the engine's real emission order.
- **No new lift** — reuses `b3a76b0_hook`, the DTWA/DTWASIG detours, the DIFFTRACE tick-boundary logic.
- **De-risks a1** — if even the *passive* key check failed in-game, the whole drain design is wrong, and
  you learn it before any risky takeover or any body lift.

---

## 2. Milestone a1 — 1-shard control-flow restructure (LATER, gated behind a0)

Once a0 confirms the key, the next step takes over the tick loop **serially** (1 shard) to prove the
restructure's *control flow* is sound in-game, then becomes the substrate for real sharding.

- **Interception point:** `t2be640_hook` (already wrapping the GOM iterator for timing). Replace its
  body with: `snapshot.acquire()` → drive the master-list walk ourselves calling the binary `3a6b80` per
  entity in list order → buffer the **fire-and-forget** emits (signals/SFX) and replay them via
  `drain_parallel` → `snapshot.release()`. DIFFTRACE must stay bit-identical to the un-restructured tick.
- **The create-deferral decision (a1's gate):** at 1 shard, creates can stay **immediate** (serial order
  is preserved by construction), so a1 does *not* need to defer creates and does *not* hit the §0 blocker.
  a1 therefore validates: tick takeover, iteration-order reproduction, snapshot lifecycle, and the
  signal/SFX buffer+drain against the live engine — everything except create-deferral and threading.
- **What a1 still does NOT prove** (and must not be claimed to): multi-thread RAW-safety and
  create-deferral. Those require the lifted Phase-A bodies (Strategy 2) — the firing intercept core
  `399450`/`381dc0`, Pass-C `3ac530`, etc. That batch is the natural payoff of `(b)` *after* a1 holds.

---

## 3. Sequencing (the recommendation)

```
a0  DT-DRAIN passive key oracle      ← DO NOW (no risk, no lift, validates the core claim in-game)
 └─ a1  1-shard tick restructure     ← then (control-flow takeover, immediate creates, signal/sfx drain)
     └─ (b)  lift remaining Phase-A bodies (399450/381dc0, 3ac530, …)   ← only once a1 holds
         └─ a2  ≥2-shard with snapshot + deferred creates              ← the real parallel finish line
```

a0 is the increment to build next. It is cheap, reversible, and answers the highest-information question
(does the §7 key reproduce live serial order?) before any control-flow or lifting effort is spent.

---

## 4. Cross-refs
- The key being validated: `sim_parallel_command_schema.md` §7 (I4 box) + the host gate
  `sim/tests/shard_scheduler_test.cpp`.
- The interfaces a1/a2 wire in: `engine_integration_contract.md` Int #1/#2/#3.
- The increment program this closes the tail of: `sim_tick_decomp_program.md` I5.
- Hook substrate: `hooks/winmm_proxy.c` (`b3a76b0_hook`, `a76b0_hook`, `t2be640_hook`, DTWA/DTWASIG).
