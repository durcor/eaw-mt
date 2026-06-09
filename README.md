Make sure to show Petroglyph your support by buying Empire at War at <https://store.steampowered.com/app/32470/STAR_WARS_Empire_at_War__Gold_Pack/>!

They're the reason any of this is possible.

# Multi-threaded EaW FoC

Multithreading retrofit project for *Star Wars: Empire at War — Forces of Corruption* (`StarWarsG.exe`, post-2023 Steam patch, x86-64).

The primary goal of this is to drastically improve the performance on modern multicore machines.

**NOTE:** All of this work was done on Linux (specifically NixOS).
Reverse-engineering and debug commands use `just` as the command runner and `nix` as the package/environment manager.

## Build (and deploy)
```sh
# Build the winmm.dll hook DLL that we'll use to hook into the engine
just build-winmm
```

## Usage
These are the steam launch options that I use:

```
# NOTE(IMPORTANT!): Linux-only (requires proton)
WINEDLLOVERRIDES="winmm=n,b" WINEDEBUG=+debugstr PROTON_USE_NTSYNC=1 %command% STEAMMOD=1125571106
```

## Dev Shell

All tools are pinned in `flake.nix`. Enter the shell with:

```sh
nix develop
```

This puts `analyzeHeadless` on `$PATH` via the Ghidra package in the Nix store (see the `shellHook` in `flake.nix`). Do not hardcode the Nix store path — it changes on Ghidra upgrades.

## Running Ghidra Scripts

Headless analysis against the existing project (no re-analysis):

```sh
nix develop --command analyzeHeadless ghidra_projects EawProject \
  -process 'StarWarsG.exe' \
  -noanalysis \
  -scriptPath tools/ghidra_scripts \
  -postScript <ScriptName>.java

# or if you've already run `nix develop` and are in its subshell
analyzeHeadless ghidra_projects EawProject \
  -process 'StarWarsG.exe' \
  -noanalysis \
  -scriptPath tools/ghidra_scripts \
  -postScript <ScriptName>.java
```

Output is written to `logs/`.

## Layout

| Path | Contents |
|------|----------|
| `binaries/` | Target executables |
| `ghidra_projects/` | Ghidra project files |
| `tools/ghidra_scripts/` | Java decompile/analysis scripts |
| `logs/` | Script output (decompiled C, findings) |
| `sessions/` | Session notes |
| `EaW_Multithread_Blueprint.md` | Architecture spec |
