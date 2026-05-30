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
  - **Corollary — the sim entity carries no plain-float world position.** Consistent with the
    OFFWATCH result (entity's own `0x40..0x4c8` float window static during movement). If absolute
    position coverage is ever needed, the remaining candidate is **path-spline progress** (the
    path/order object at `component+0xc0` + which spline node the unit occupies; the spline node
    matrices are the `FUN_14012d2c0` source) — not pursued now, since the state-machine fingerprint
    already covers the deterministic movement surface.

---

## MovementComponent — the per-tick serviced element (RTTI class TBD)

The element `e` in the movement coordinator's list (`coordinator+0x2d0` → array; see
`decomp/3a76b0.c`) and the `param_1` of the path-follow `FUN_140387400`. A **compact movement-state
record**, distinct from `LocomotorCommonClass` (below).

| Offset | Width | Meaning | Conf |
|---|---|---|---|
| `+0x10` | ptr | context/owner ptr (deref'd → `+0x298`,`+0x2b0` in `387400`) | med |
| `+0x20` | ptr | **owning entity** (`GameObjectClass*`) | ✓ high |
| `+0x28` | float | speed / weight (force-distribution numerator in `3a76b0`) | ✓ high |
| `+0x48` | ptr | **current target** (target entity; `target+0x10` compared) | ✓ high |
| `+0x58` | uint | rate-limit **countdown** (`-= delta`; gates per-component service cadence) | ✓ high |
| `+0x60` | int | last-serviced tick stamp (`387010`: `delta = param_2 - *(comp+0x60)`) | ✓ high |
| `+0x90` | int | **cached node id** (name→id via `12d520`; re-resolved when `< 0`) | ✓ high |
| `+0x94` | int | **cached bone index** (primary; name→idx via `12d2a0`) | ✓ high |
| `+0x98` | int | cached secondary bone index | ✓ med |
| `+0x6c` | char | enable flag | ✓ med |
| `+0x6d` | char | in-progress flag (set/cleared around the path step in `387400`) | ✓ med |
| `+0x6e` | char | flag | ✓ med |
| `+0xc0` | ptr | path/order object (alloc'd lazily; `+0xcc` reset to 0) | med |

- **RTTI class: unresolved.** Confirmed it is **not** `LocomotorCommonClass` (that class's `+0x28`
  is a pointer, not the speed float). Next step: resolve via what allocates the `+0x2d0` list entries
  (or read RTTI off a live `e` from the harness). These offsets are the harness's current fingerprint
  field set (index, entity `+0x48`, component `+0x28`, `+0x58`).

---

## LocomotorCommonClass — the locomotor behavior (base)

- **Vtable:** `0x869ea0` (97 methods — the base; Walk/Fighter/Fleet/JetPack/… derive it). Base vtable
  written by the derived behavior ctors (`TeamLocomotorBehaviorClass` `FUN_14062f680`,
  `FighterLocomotorBehaviorClass` `FUN_1405c8a80`).
- **Empirical map:** `logs/Phase2Fields_LocomotorCommonClass.tsv` — 43 offsets / 57 methods.
  Dominant member `+0x28` (ptr, 78×). Distinct from the MovementComponent above (a behavior attached
  to an entity, not the per-tick coordinator element). Detailed labeling deferred — not on the
  rewrite critical path until the MovementComponent↔behavior relationship is mapped.

---

## GameObjectManagerClass — the GOM

- **Vtable:** `0x85b9a8`. **Ctor:** `FUN_14029aaa0` (constructed by `GameModeClass`'s ctor
  `FUN_14029a0c0` — the GOM is a GameMode member). Entity-list layout TBD (the world-state set the
  rewrite must double-buffer); next after the entity transform.

---

## Phase-2 TODO (priority order)
1. ~~Entity transform/position offset~~ — **RESOLVED** (node-manager chain above). Remaining:
   locate the `manager` singleton once for the harness fast path; validate transform determinism.
2. **MovementComponent RTTI class** — allocation site / live-RTTI read.
3. **GOM entity list** layout (the double-buffer set).
4. RNG + tick-clock globals already known (`DAT_140b0a320` counter, `0x9cf314` FF flag); fold into a
   determinism-surface struct doc in Phase 4.
