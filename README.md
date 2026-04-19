# EaW Engine Reverse Engineering

Reverse engineering and multithreading retrofit project for *Star Wars: Empire at War — Forces of Corruption* (`StarWarsG.exe`, post-2023 Steam patch, x86-64).

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
