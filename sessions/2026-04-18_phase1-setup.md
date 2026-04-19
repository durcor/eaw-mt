# Session: 2026-04-18 — Environment Setup & Phase 1.1

**Phase:** 1 (Binary Survey)
**Objective:** Environment setup, toolchain verification, PE header analysis
**Outcome:** Phase 1.1 complete — pausing for human review

---

## Completed

- [x] Initialized project git repository
- [x] Created full directory structure per blueprint Section 2
- [x] Generated `flake.lock` (pinned nixpkgs nixos-unstable `4bd9165`, 2026-04-14)
- [x] Verified toolchain (all critical tools confirmed — see `logs/toolchain_check.txt`)
- [x] Copied all EXE/DLL from game dir to `binaries/original/`, computed SHA-256 checksums
- [x] Set `binaries/original/` read-only (`chmod -R a-w`)
- [x] Phase 1.1: PE header analysis of `swfoc.exe` and `StarWarsG.exe`
- [x] Wrote `openspec/engine/binary_metadata.md` with Phase 1.1 findings
- [x] Wrote stub openspec files for game_loop, threading_model, functions/index, data_structures/index

## Key Findings (Phase 1.1)

1. **Both binaries are x86-64 (64-bit)** — post-2023 Steam remaster confirmed
2. **Primary analysis target: `StarWarsG.exe` (12 MB)** — not swfoc.exe (1.7 MB launcher)
3. **Renderer: Direct3D 9** — `d3d9.dll` + `d3dx9_43.dll`
4. **Threading: Win32 CreateThread + TLS + CriticalSections** — engine already has threading infrastructure
5. **TLS directory present** in both binaries
6. **Lua: statically linked** — no lua*.dll import; version TBD via string analysis
7. **tbbR.dll (Intel TBB)** imported by `swfoc.exe` launcher only — engine doesn't use TBB directly
8. `.bind` section in `swfoc.exe` — purpose unclear, TODO for later

## Surprises / Deviations from Blueprint Assumptions

- Blueprint assumed engine might be single-threaded. It's NOT — `CreateThread` is actively imported.
- `tbbR.dll` is in the launcher, not the engine — TBB headroom exists but isn't wired to the game logic yet.
- ASLR is enabled on `StarWarsG.exe` (YES) but disabled on `swfoc.exe` (NO).

## Pending (Phase 1.2+)

- Ghidra headless analysis of `StarWarsG.exe`
- String extraction to find: source file paths, Lua version strings, thread/worker keywords
- Game loop identification (Phase 1.3)
- Human review of Phase 1.1 findings required before Phase 1.2

---

**Pausing here for human review per blueprint Section 10, step 6.**
