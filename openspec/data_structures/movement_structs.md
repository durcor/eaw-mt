# Movement / Sim-Core Structures

**Status:** Phase 2 (struct recovery) — IN PROGRESS, started 2026-05-30. Layouts here are *partial*,
recovered empirically. Confidence is per-field. These are the structures the sim-parallel rewrite's
Phase-A movement body reads/writes, and the targets the differential-test harness must fingerprint.

**Recovery method.** RTTI (Phase 0) named the classes; Phase-1 attribution grouped methods under
them. `tools/ghidra_scripts/Phase2StructFields.java` then decompiles every method of a class and
unions all `this`-relative accesses into an empirical field map (offset → access count → observed
widths). Regenerate: `just gs Phase2StructFields <ClassName> [cap]` → `logs/Phase2Fields_<Class>.tsv`.
Constructors/destructors located via `just gs Phase2VtableXrefs <vtableRVA…>` (the ctor sets the
vtable ptr at `this+0` and initializes fields).

---

## GameObjectClass — the entity

- **Vtables (multiple inheritance):** primary `0x8661b8` + `0x8661d8`, plus `0x866200/210/228`.
- **Ctor/dtor:** `FUN_1403893e0` is the **destructor** (writes the base vtables, then frees/zeros
  members via virtual calls — `[0x54]/[0x55]/[0x56]`). The constructor is among the other vtable
  referrers `FUN_140388b60` / `FUN_140388720` (Phase2VtableXrefs). Size: large (dtor body 3168 B;
  object spans ≥0x2e0, members reference offsets up to ~0x370+ in callers).
- **Empirical field map:** `logs/Phase2Fields_GameObjectClass.tsv` — 183 distinct offsets across 62
  methods. Confirmed/labeled subset (✓ = corroborated by hooks/openspec movement analysis):

| Offset | Width | Meaning | Conf |
|---|---|---|---|
| `+0x00` | ptr | primary vtable | ✓ high |
| `+0x08`,`+0x18`,`+0x28` (sub-vtables) | ptr | secondary base vtables (MI) | ✓ high |
| `+0x10` | ptr (42×) | member pointer (heavily used; owner/manager back-ref candidate) | med |
| `+0x20` | ptr (24×) | member pointer | med |
| `+0x38` | ptr/list | listener/subscription list (cross-entity write target in Model C scoping) | ✓ high |
| `+0x48` | int | motion-state (read `*(int*)(entity+0x48)`, range [5,10] gate in `387400`) | ✓ high |
| `+0x4d` | char | **active flag** (==1 ⇒ serviced; the coordinator/`387400` filter) | ✓ high |
| `+0x4e` | char | **render-node gate** (==1 ⇒ entity has a live Alamo scene node → world transform resolvable via `385cf0`/`12d2c0`). Empirically ==0 for all sim-serviced movement components. | ✓ high |
| `+0x50` | char | state flag (gates path branches in `387400`) | ✓ med |
| `+0x60` | `std::string` | **node name** — keys the entity into the node manager (SSO: `+0x70` len, `+0x78` cap) | ✓ high |
| `+0x1c0` | `std::string` | **bone/hardpoint name** — resolves to a bone index for the world matrix (SSO cap `+0x1d8`) | ✓ high |
| `+0x1e0` | `std::string` | secondary bone name (used iff `+0x1f0 != 0`) | ✓ med |
| `+0x2b0` | ptr | sub-object ptr (path/order context; deref'd in `387400`) | med |
| `+0x2d8` | ptr | Lua context (`ctx`; `*ctx+0x58` = lua_State*) — prewarm chain | ✓ high |
| `+0x148`,`+0x1f0`,`+0x360` | ptr | sub-object pointers used by the movement path | low |

- **Transform/position — RESOLVED 2026-05-30. The world transform is NOT in the entity struct.**
  It lives in a global **Alamo node/skeleton manager**, keyed by the entity's *name string*, and is
  read through a name→id→node→bone-matrix chain. Confirmed end-to-end by static decompile of
  `FUN_140385cf0` / `FUN_140387010` and the `0x12dxxx` manager family. This is why five runtime
  memory-scans (OFFWATCH v1–v4) of the entity's own 0x40..0x4c8 window found **no** coordinate
  floats and **zero** moving VEC3 triples — there is nothing positional in the entity to find.

  **Full resolution chain (the authoritative world position read):**
  1. `manager = FUN_1402648b0(ctx)` — walks a subsystem list (`ctx+0x48`, vfunc-iterated `+0x20`
     chain on `node+8`) for the one tagged `&DAT_140a12370`; returns the node/skeleton manager.
  2. `entity+0x60` is a **`std::string`** (the entity's node name), NOT a handle.
     `FUN_14001e680` is just the `std::string` **copy-ctor** (SSO: `+0x10`=len `+0x18`=cap;
     heap-backed when len ≥ 0x10) — earlier "handle" reading was wrong.
  3. `id = FUN_14012d520(manager, strupr(NAME))` — linear `_stricmp` scan over the manager's
     **node vector at `manager+0xf0 .. +0xf8`** (stride 0x10, node ptr at `elem+8`, name via vfunc
     slot 6). Cached on the component at **`component+0x90`** (re-resolved only when `< 0`).
  4. `node = FUN_14012d6f0(manager, id)` = `*(manager+0xf0 + id*0x10 + 8)`.
  5. `boneIdx = FUN_14012d2a0(node, entity+0x1c0)` — a **second `std::string`** (`entity+0x1c0`,
     SSO cap at `+0x1d8`) names the bone/hardpoint; resolves to a bone index. Cached at
     **`component+0x94`** (and a second optional bone `entity+0x1e0` → `component+0x98` when
     `entity+0x1f0 != 0`). `FUN_14012d430(node, boneIdx, 1)` then marks the bone live.
  6. `FUN_14012d2c0(node, boneIdx, out)` copies the **4×3 matrix** (12 × `float`, 0x30 stride):
     `src = *(node+0xe8) + 0x28 + boneIdx*0x30`. First invokes vfunc slot 0xf (`*(node+0x78)`) to
     recompute transforms. Identity default `0x3f800000`=1.0f on the diagonal.

  **So: world matrix = `*(node+0xe8) + 0x28 + boneIdx*0x30`, 12 floats; translation = last 3
  (`out+0x24/+0x28/+0x2c`).** Format = 4×3 matrix (confirmed; 0x30 stride).

  - **Ruled out** as the position field (still valid): the Lua `Get_Position` binding (reflection
    dispatcher; `56d4c0` stores a constant); the force integrator `FUN_140387f50` (`component+0x28
    -= force`); the deep-path entity floats `+0x200` (mass·physics), `+0x23c` (speed threshold),
    `+0x250[]` (per-heading LUT) — all tuning/state, **not** position. The OFFWATCH survey also
    confirmed `+0x250..+0x380` are the all-`1.0` LUT in the live entity.
  - **This transform is RENDER-SIDE, not a sim-oracle source — proven empirically 2026-05-30
    and NOT folded into the harness.** Wired the chain into DIFFTRACE (call `385cf0`→node, then
    `12d2c0`→matrix), but it resolved **zero** transforms. Instrumented diagnostic (`DTDIAG`)
    showed the wall: **`385cf0` requires `entity+0x4e == 1`** (the render-node gate; `387010`'s
    bone-index resolution is gated on the same byte), and **across ~19,000 sim ticks (the menu
    demo + two real moving space battles with ships under move orders) every serviced movement
    component had `entity+0x4e == 0`** (per-tick tally `e4e1=0` throughout; `id`/`boneIdx` at
    `component+0x90`/`+0x94` stayed `-1`, never resolved). So the Alamo node-manager matrix is
    the scene-graph (render) transform, populated only for entities with a live model instance,
    and is **never available to the sim-serviced components**. Folding it would couple the oracle
    to rendering — exactly what the sim-parallel split must avoid — and is impossible here anyway.
    The DIFFTRACE harness correctly fingerprints the **logical** movement state (motion-state
    `+0x48`, speed `+0x28`, countdown `+0x58`), which varies per-tick with real movement; that IS
    the right sim oracle. Transform-fold code removed (documented dead end in `dt_fold_coordinator`).
  - **Corollary — SUPERSEDED 2026-05-30.** This section earlier concluded "the sim entity carries no
    plain-float world position," based on the OFFWATCH result. That was an artifact of watching the
    **hardpoint** path (`*(HardPoint+0x20)` owner record), not the moving `GameObjectClass`. The
    locomotor analysis (§LocomotorBehaviorClass) found the authoritative sim position **is** a flat
    float vector at **`GameObjectClass + 0x78/+0x7c/+0x80`** (x/y/z), written each tick by the locomotor
    `vfunc_6` and confirmed by four independent readers. The node-manager matrix above remains the
    *render* transform; `entity+0x78` is the *sim* position. The OFFWATCH `0x40..0x478` window does
    include `+0x78`, so it would have shown movement had it watched the moving GameObject directly.

---

## HardPointClass — the per-tick serviced element  ⚠ NOT a movement component

**RTTI class RESOLVED 2026-05-30 (live read, `DTRTTI`): the element `e` is `HardPointClass`**
(vtable RVA `0x865de8`; 5/5 live samples agreed). This **reframes the whole `a76b0` path** — see
the box below. `e` is the element of the list at `coord+0x2d0` (see `decomp/3a76b0.c`) and the
`param_1` of `FUN_140387400`. It is a **weapon hardpoint (fire-control) record**, not locomotion.

> ### ⚠ The `a76b0`→`387010`→`387400` path is per-ship HARDPOINT fire-control, not movement
> Live RTTI (`DTRTTI` one-shot in the harness) over the menu demo + battles established:
> - **`coord` (the `a76b0` param_1) = `GameObjectClass`** (vtable `0x8661b8`) — a *ship*, not a
>   "MovementCoordinator" class.
> - **`coord+0x2d0` = `DynamicVectorClass<HardPointClass*>`** (container vtable `0x866238` in the
>   RTTI map) — the ship's **hardpoint list**.
> - **`e` = `HardPointClass`** — one weapon mount.
> - `FUN_1403a76b0` = "for each of this ship's hardpoints, distribute a fire-rate budget (weighted
>   by `+0x58` via `FUN_140540070`, scaled by game-speed mode) and service it." `FUN_140387400` =
>   per-hardpoint **opportunity-target acquisition** (`OpportunityTargetAcquiredDataClass`, target
>   set `382510` / release `3846c0` / capped search `385190` = **Fix B2**). `HardPointClass::vfunc_1`
>   (`386a90`) = target/listener bookkeeping (on a "target destroyed" event `0x28` it nulls the
>   target slots and unregisters from the target's `+0x38` listener list).
> - **Cost reconciliation:** the per-tick audit's "≈1700 `387400` calls/tick ≈ N entities × ~55
>   components" — the **~55 "components" per capital ship are its hardpoints**. Big-battle FF
>   choppiness is **weapon target-acquisition** cost, not movement. The real locomotor system is the
>   separate `LocomotorBehaviorClass` hierarchy (below), which is *not* the `coord+0x2d0` list.
> - **Impact on prior analysis:** any earlier note calling this path "movement" / "path-following"
>   (Model C "movement parallelization", the cost audit) is mislabeled as to *subsystem* — it's
>   weapons. The structural conclusions (per-component volume; cross-object target writes block
>   parallelism) still hold; only the name was wrong. Flagged for review.

Field map of `HardPointClass` (`e`). Earlier "movement" labels corrected:

| Offset | Width | Meaning | Conf |
|---|---|---|---|
| `+0x10` | ptr | context/owner ptr (deref'd → `+0x298`,`+0x2b0` in `387400`) | med |
| `+0x20` | ptr | **owner/state record** (non-polymorphic; `*(e+0x20)` is NOT a vtable — see note) | ✓ high |
| `+0x28` | float | fire-rate / distribution weight (was mislabeled "speed"; `3a76b0` numerator) | ✓ high |
| `+0x40` | ptr | target slot (cleared on target-death event in `386a90`) | ✓ high |
| `+0x48` | ptr | **current target** (`target+0x10` compared) | ✓ high |
| `+0x50` | ptr | secondary/opportunity target slot | ✓ med |
| `+0x58` | float | **fire-rate weight** read by `540070` (was mislabeled "countdown") | ✓ high |
| `+0x60` | int | last-serviced tick stamp (`387010`: `delta = param_2 - *(e+0x60)`) | ✓ high |
| `+0x6c` | char | enable flag | ✓ med |
| `+0x6d` | char | in-progress flag (set/cleared around the target step in `387400`) | ✓ med |
| `+0x6e` | char | flag | ✓ med |
| `+0x90` | int | cached id (name→id via `12d520`; was assumed node id — **render path, unused here**) | med |
| `+0x94` | int | cached bone index (name→idx via `12d2a0`; render path, `<0` ⇒ unresolved) | med |
| `+0x98` | int | cached secondary bone index | med |
| `+0xa8` | ptr | target slot (cleared in `386a90`) | ✓ med |
| `+0xc0` | ptr | order/target object (alloc'd lazily; `+0xcc` reset to 0) | med |

- **`e+0x20` ("owner/state record") — class TBD.** Live read showed `*(e+0x20)` is a heap pointer,
  not a vtable, so the struct is **non-polymorphic** — it is NOT itself a `GameObjectClass`. Yet it
  carries the `+0x48` motion-state int, `+0x4d` active flag, `+0x4e` render-node gate, and `+0x60`
  node-name `std::string` that were earlier attributed to `GameObjectClass` (see that section — those
  offsets may actually belong to this record, or it is a non-primary base view of the owning unit).
  **Open:** dump its first 16 bytes / sub-vtables to resolve whether it's a GameObject base view or a
  distinct per-hardpoint owner struct. The DIFFTRACE harness's `*(e+0x20)+0x4d` active filter matches
  `3a76b0`'s own filter, so it is correct regardless of the record's class name.
- **NOTE on the differential harness:** DIFFTRACE folds `{index, *(e+0x20)+0x48, e+0x28, e+0x58}`.
  This is a valid deterministic *sim* fingerprint (hardpoint target/fire state IS sim state), just
  labeled "movement" — it is really a **weapon fire-control** fingerprint. Still the right oracle for
  that subsystem; no change needed beyond the naming correction.

---

## LocomotorBehaviorClass — the locomotor (THE REAL MOVER)

- **Vtables:** base `LocomotorBehaviorClass` `0x8adda0` (98 methods); base-class views `LocomotorCommonClass`
  `0x869ea0`. Derived per-movement-type: `WalkLocomotorBehaviorClass 0x8ad798`,
  `FighterLocomotorBehaviorClass 0x8a6198`, `FleetLocomotorBehaviorClass 0x899c58`,
  `StarshipLocomotorBehaviorClass 0x8ae250`, `SimpleSpaceLocomotorBehaviorClass 0x8aeaf8`,
  `LandTeam*LocomotorBehaviorClass`, etc. (~12 subclasses). Ctors e.g. `FighterLocomotorBehaviorClass`
  `FUN_1405c8a80`, `TeamLocomotorBehaviorClass` `FUN_14062f680`.
- **`behavior+0x28` = back-ref to the owning `GameObjectClass`** (the unit it moves; dominant member,
  78× in the field map). The behavior reads/writes its owner each tick.

- ### ✅ SERVICE ENTRY = vtable **slot 6** (`vfunc_6`) — the per-tick position integrator
  Every derived locomotor **overrides slot 6** (base no-op `TacticalBuildObjects…::vfunc_6` `0x42f140`
  just returns `0x650000`; real movers: Starship `0x6236b0`, Walk `0x61e930`, Fighter/Fleet/… likewise).
  Each `vfunc_6(behavior, entity)`:
  1. calls the shared `LocomotorCommonClass::vfunc_6` pre-step,
  2. reads the unit's **world position** from `entity+0x78/+0x7c/+0x80` (x/y/z floats),
  3. switches on the **locomotor state** `*(entity+0xa8)` (state record; `+0x48` = state enum,
     `+0x54/+0x58/+0x5c/+0x60` = sub-state/timers),
  4. runs the per-type movement state machine (accel/heading/turn math; `FUN_1404aaa40` = vector ops),
  5. writes the updated position back to `entity+0x78/+0x7c/+0x80`.
  This is the method the sim-parallel rewrite's movement body re-implements. Profile harness can fold
  it directly — see the position note below.

- **✅ WORLD POSITION = `GameObjectClass + 0x78/+0x7c/+0x80`** (flat `float` x/y/z), confirmed by FOUR
  independent functions: `StarshipLocomotorBehaviorClass::vfunc_6` (read→integrate→write),
  `MovementCoordinatorClass::vfunc_8` `0x4f7360` (group centroid), `SpaceMovementCoordinatorClass::vfunc_7`
  `0x5d1710` (nearest-neighbor distance), and the Walk locomotor. **This corrects the earlier
  "the sim entity carries no plain-float position" conclusion** — that conclusion came from watching the
  *hardpoint* path (OFFWATCH latched `*(HardPoint+0x20)`, the non-polymorphic owner record, never the
  moving `GameObjectClass`). The render-side node transform (entity+0x4e gated, §GameObjectClass) is a
  *separate* representation; the **authoritative sim position is `entity+0x78`**, and the differential
  harness CAN fingerprint it (the hardpoint coordinator `a76b0` param IS a `GameObjectClass`, so
  `coord+0x78/+0x7c/+0x80` is the ship's position — fold that for deterministic transform coverage).

- ### ✅ DRIVER PINNED = `FUN_1403a6b80` (the per-GameObject sim-tick update)
  Found by runtime return-address capture (vtable-hook slot 6 → `__builtin_return_address(0)`, LOCODRV):
  the caller is **RVA `0x3a6f80`, inside `FUN_1403a6b80`** (entry `0x3a6b80`; next fn is `a76b0`). This
  is the **per-entity sim update** — `FUN_1403a6b80(entity, tick, flag)`. Its behavior loop
  (`decomp/3a6b80.c:128-134`):
  ```c
  for (i = (char)entity[0x52]; --i >= 0; )                 // behavior COUNT = *(entity+0x290)
      b = *(GameObject_behaviors[i]);                      // behavior ARRAY = *(entity+0x278)
      if (*(uint*)(b+0x30) <= tick &&                      // per-behavior schedule gate
          FUN_1404c3700(b))                                // enabled? (behavior+0x3c == 0)
          (**(code**)(*b + 0x30))(b, entity, tick);        // behavior->vfunc_6(entity, tick)  ← slot 6
  ```
  So **`vfunc_6` (slot 6) is the generic `BehaviorClass::Update(entity, tick)`** — every behavior on the
  entity implements it; the *locomotor* behavior's override is the movement integrator (position at
  `entity+0x78`). `FUN_1403a6b80` ticks ALL of an entity's behaviors, then services its hardpoints
  (`FUN_1403a76b0`, `decomp/3a6b80.c:358`), and recurses into contained/child units. It is the real
  per-entity sim-tick body. **The arg order is `(behavior, entity, tick)` — 3 register args** (rcx/rdx/r8;
  the capture hook had to forward r8 to avoid dropping the tick delta).
  - **Above the driver:** `FUN_1403a6b80` is invoked per root entity by the battle/GOM tick (recursive
    for children); identifying that top-level iterator is the **GOM entity-list** task (Phase-2 #3).
  - **Not the driver:** the Land/Space `MovementCoordinatorClass` (`0x86b080`/`0x8a6520`) are
    `SignalListenerClass`-derived spatial-coherence/formation systems (registered-unit vector at
    `coordinator+0x28..+0x30`; `vfunc_3` `0x4f6b60` = add-unit, `vfunc_1` `0x4f77e0` = signal callback);
    they read positions but do not tick the locomotor.

---

## GameObjectManagerClass — the GOM

- **Vtable:** `0x85b9a8`. **Ctor:** `FUN_14029aaa0` (constructed by `GameModeClass`'s ctor
  `FUN_14029a0c0` — the GOM is a GameMode member). Entity-list layout TBD (the world-state set the
  rewrite must double-buffer); next after the entity transform.

---

## Phase-2 TODO (priority order)
1. ~~Entity transform/position offset~~ — **RESOLVED** (render-side node-manager chain; not a sim
   field — see GameObjectClass §). No harness fold.
2. ~~MovementComponent RTTI class~~ — **RESOLVED: it's `HardPointClass`** (live RTTI); `a76b0`/`387400`
   = hardpoint fire-control, not movement.
2b. ~~Locomotor service entry~~ — **RESOLVED: `LocomotorBehaviorClass` vtable slot 6 (`vfunc_6`)**, the
   per-tick position integrator; world position = `entity+0x78/+0x7c/+0x80` (see §LocomotorBehaviorClass).
   Follow-ups: (a) resolve the hardpoint `e+0x20` owner-record class; ~~(b) pin the per-frame DRIVER~~
   **DONE: driver = `FUN_1403a6b80`** (per-GameObject sim update; behavior loop at `entity+0x278`/count
   `+0x290` → `BehaviorClass::vfunc_6(entity, tick)`); (c) optionally fold `coord+0x78` into DIFFTRACE
   for real transform coverage.
3. **GOM entity list** layout (the double-buffer set).
4. RNG + tick-clock globals already known (`DAT_140b0a320` counter, `0x9cf314` FF flag); fold into a
   determinism-surface struct doc in Phase 4.
