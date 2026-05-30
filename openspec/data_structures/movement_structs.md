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
| `+0x50` | char | state flag (gates path branches in `387400`) | ✓ med |
| `+0x2b0` | ptr | sub-object ptr (path/order context; deref'd in `387400`) | med |
| `+0x2d8` | ptr | Lua context (`ctx`; `*ctx+0x58` = lua_State*) — prewarm chain | ✓ high |
| `+0x148`,`+0x1f0`,`+0x360` | ptr | sub-object pointers used by the movement path | low |

- **Transform/position: NOT YET LOCATED.** It is *not* a typed-float cluster in the aggregate map
  (only `+0x4f` shows a stray float), so position likely lives in a **sub-object** (one of the
  member pointers above) or is accessed untyped (`undefined4`/matrix copy). Pinning it needs a
  targeted decompile of a position getter / the integrator that writes it. **This is the next
  Phase-2 unit** — it's what unlocks transform-value coverage in the differential harness.

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
1. **Entity transform/position offset** (sub-object hunt) — unblocks harness transform coverage.
2. **MovementComponent RTTI class** — allocation site / live-RTTI read.
3. **GOM entity list** layout (the double-buffer set).
4. RNG + tick-clock globals already known (`DAT_140b0a320` counter, `0x9cf314` FF flag); fold into a
   determinism-surface struct doc in Phase 4.
