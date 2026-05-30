# Data Structure Registry

**Target binary:** `StarWarsG.exe` (x86-64, ImageBase `0x0000000140000000`)

---

## Movement / sim-core structures (decomp Phase 2)

See [movement_structs.md](movement_structs.md) — partial empirical layouts for the sim-parallel
rewrite + differential harness:

- **GameObjectClass** (entity) — vtables `0x8661b8`/`d8`; dtor `FUN_1403893e0`; 183-offset empirical
  map; movement-relevant fields labeled (`+0x48` motion-state, `+0x4d` active, `+0x38` listener list,
  `+0x2d8` lua ctx). Transform offset still TBD.
- **MovementComponent** (per-tick serviced element; RTTI class TBD) — `+0x20` entity, `+0x28` speed,
  `+0x48` target, `+0x58` countdown, `+0x6c–6e` flags.
- **LocomotorCommonClass** (behavior base, vtable `0x869ea0`) and **GameObjectManagerClass** (GOM,
  vtable `0x85b9a8`, ctor `FUN_14029aaa0`).

Recovery tooling: `tools/ghidra_scripts/Phase2StructFields.java` (empirical field maps) +
`Phase2VtableXrefs.java` (ctor/dtor locator). Regenerate via `just gs Phase2StructFields <Class>`.
