# EaW Engine Multithreading Project — Claude Code Blueprint
**Project:** Reverse engineering and multithreading retrofit for Star Wars: Empire at War (Forces of Corruption engine)
**Target Mod:** Thrawn's Revenge
**Host OS:** NixOS (Linux)
**Runtime:** Steam via Proton/Wine
**Approach:** Binary reverse engineering → OpenSpec documentation → targeted threading patches

---

## 0. Agent Operational Rules

Before doing anything else, read and internalize these rules. They govern every session.

1. **Spec-first discipline.** Every confirmed finding must be committed to `openspec/` before the session ends. Unwritten findings are lost findings. Integration with openspec tooling is vital for ensuring quality and clarity.
2. **Never assume, always verify.** Form a hypothesis, write a test or use the debugger to confirm it, then document it. Do not document hypotheses as facts.
3. **Preserve the baseline.** Never patch the original binaries in-place. Always work on copies. Keep `binaries/original/` read-only and checksummed.
4. **Fail loudly.** If a task requires information that is missing from the OpenSpec and cannot be determined this session, write a `TODO` entry in the spec and halt that branch. Do not proceed on assumptions.
5. **Checkpoint often.** After completing any major discovery or patch, commit everything to version control with a descriptive message before continuing.
6. **Human validation gates.** The following require a human sign-off before proceeding: architectural decisions, any patch that modifies more than one subsystem, and any finding that contradicts an existing spec entry.

---

## 1. Platform Notes — NixOS + Wine

This project runs on NixOS with the game executing under Proton/Wine. This has meaningful implications for the toolchain and the patching strategy.

### The Good News

The **analysis** side of the project is entirely native Linux. Ghidra, Python, radare2, pefile, and frida all run natively. You are analyzing a Windows PE binary, but the tools that do so are Linux-native. No Wine is needed to reverse engineer the binary. This is actually a cleaner environment than Windows for static analysis.

### Where Wine Enters the Picture

Wine/Proton only matters when:
- **Running the game** to observe live behavior or validate patches
- **Attaching a debugger** to the running process
- **Loading hook DLLs** — these must be Windows PE DLLs even though they're built on Linux (use MinGW cross-compiler)

### Debugger Strategy on Wine

`x64dbg` is Windows-only and does not run natively on Linux. Use one of the following instead:

**Option A — winedbg (built into Wine):**
Wine ships its own GDB-compatible debugger. Run the game via:
```bash
winedbg --gdb swfoc.exe
```
This gives a GDB-like interface attached to the Wine process. Supports breakpoints, memory inspection, and register reads. Sufficient for most tasks.

**Option B — Frida (recommended for automation):**
Frida runs natively on Linux and can attach to Wine processes just like any other Linux process, injecting JavaScript instrumentation. This is the preferred approach for Claude Code's autonomous iteration loops because it is scriptable and doesn't require a GUI.
```bash
frida -n swfoc.exe  # attaches to running Wine process by name
```

**Option C — GDB with Wine stubs:**
`gdb --args wine swfoc.exe` works but Wine's symbol handling makes this messy. Use only as a fallback.

### Hook DLL Strategy on Wine

DLL injection into a Wine process requires building a standard Windows PE DLL using the MinGW cross-compiler on Linux. Wine loads these exactly as it would on Windows.

```bash
# NixOS: these are provided by flake.nix at the project root (see Section 1, NixOS Package Environment)
# Enter the dev shell with: nix develop
```

Build hook DLL with:
```bash
x86_64-w64-mingw32-gcc -shared -o eaw-mt-hook.dll hook.c -lkernel32
# For 32-bit (pre-2023 binary):
i686-w64-mingw32-gcc -shared -o eaw-mt-hook.dll hook.c -lkernel32
```

Inject by placing the DLL in the game directory and adding it to Wine's DLL override:
```bash
WINEDLLOVERRIDES="eaw-mt-hook=n,b" wine swfoc.exe
```
Or via Steam launch options:
```
WINEDLLOVERRIDES="eaw-mt-hook=n,b" %command%
```

### NixOS Package Environment

A `flake.nix` exists at the project root. This pins all tool versions in `flake.lock` so every session — and any future contributor — uses identical tooling regardless of when they run `nix develop`.

Enter the environment with `nix develop`. The `flake.lock` file generated on first run should be committed to git alongside `flake.nix` — it's what guarantees reproducibility.

If `python3Packages.frida-python` is missing or the wrong version in the pinned nixpkgs, enter the shell and run `pip install frida-tools` into a local venv. Note that in the flake version you can also add a `frida` overlay if you need a specific version pinned.

**`analyzeHeadless` path:** The Ghidra support scripts are not on the default PATH. The flake shellHook adds `${pkgs.ghidra}/lib/ghidra/support` to PATH so `analyzeHeadless` is available inside `nix develop`. Do not call the `ghidra` binary directly — it launches the GUI.

**Ghidra project location:** Store Ghidra projects under `ghidra_projects/` in the repo root, not `/tmp`. This keeps the analyzed database in version control range and survives reboots.

**Ghidra scripts must be Java, not Python.** Headless Ghidra does not include PyGhidra. All scripts in `tools/ghidra_scripts/` use the GhidraScript Java API. See `ExportFunctions.java`, `ExportStrings.java`, `FindGameLoop.java`, `DecompileCandidates.java` for examples.

### Finding Your Proton Prefix

Steam game files and the Wine prefix for EaW are separate locations:

```
# Binaries (what we analyze and patch):
~/.steam/steam/steamapps/common/Star Wars Empire at War/corruption/StarWarsG.exe  # PRIMARY
~/.steam/steam/steamapps/common/Star Wars Empire at War/corruption/swfoc.exe      # launcher

# Proton prefix (Wine C: drive, config, etc.):
~/.steam/steam/steamapps/compatdata/32470/pfx/

# EaW Gold Pack Steam App ID: 32470
```

**Critical:** `swfoc.exe` is a 1.7MB launcher stub. The actual game engine is `StarWarsG.exe` (12MB). All Ghidra analysis, Frida scripts, and patches target `StarWarsG.exe`.

---

## 2. Repository Structure

Initialize/confirm this layout before any analysis begins:

```
$PWD
├── flake.nix                   # Reproducible nix dev environment
├── flake.lock                  # Pinned nixpkgs — commit this to git
├── python/
│   ├── pyproject.toml          # declares frida-tools, pefile, capstone
│   └── uv.lock                 # pinned by uv, consumed by uv2nix
├── binaries/
│   ├── original/               # Read-only copies. Never touch.
│   │   ├── StarWarsG.exe       # PRIMARY TARGET — Forces of Corruption engine (12MB, x86-64)
│   │   ├── swfoc.exe           # Launcher stub only (1.7MB) — NOT the engine
│   │   ├── sweaw.exe           # Base game executable
│   │   └── *.dll               # All DLLs from the corruption/ directory
│   └── patched/                # Working copies for modification
├── ghidra_projects/
│   └── EawProject/             # Ghidra project with analyzed StarWarsG.exe
├── openspec/
│   ├── README.md               # Spec conventions and glossary
│   ├── engine/
│   │   ├── game_loop.md
│   │   ├── memory_layout.md
│   │   ├── binary_metadata.md  # Architecture, imports, section layout
│   │   ├── threading_model.md  # Engine is multi-threaded at startup; main loop is single-threaded
│   │   └── subsystems/
│   │       ├── renderer.md
│   │       ├── ai.md
│   │       ├── script_engine.md
│   │       ├── galactic_map.md
│   │       ├── pathfinding.md
│   │       └── entity_system.md
│   ├── functions/
│   │   └── index.md            # Master function registry
│   └── data_structures/
│       └── index.md            # Master struct registry
├── tools/
│   ├── ghidra_scripts/         # Ghidra analysis scripts — use Java, NOT Python
│   │                           # (headless Ghidra does not support PyGhidra)
│   ├── frida_scripts/          # Frida instrumentation scripts
│   ├── hooks/                  # Hook DLL source (compiled with MinGW)
│   └── harness/                # Headless test harness (built later)
├── patches/
│   ├── experimental/
│   └── validated/
├── logs/
└── sessions/
    └── YYYY-MM-DD_topic.md
```

---

## 3. Toolchain Setup and Verification

Before beginning analysis, run a verification script that checks each tool and reports status. Do not proceed to Phase 1 until all critical tools are confirmed.

| Tool | Critical? | Purpose | How to Check |
|------|-----------|---------|--------------|
| Ghidra (headless) | Yes | Disassembly / decompilation | `ghidra --version` or run analyzeHeadless |
| Python 3 + pefile | Yes | PE header inspection | `python3 -c "import pefile"` |
| Python 3 + capstone | Recommended | Disassembly in Python | `python3 -c "import capstone"` |
| frida-tools | Yes | Live instrumentation / debugging | `frida --version` |
| MinGW cross-compiler | Yes | Build Windows hook DLLs on Linux | `x86_64-w64-mingw32-gcc --version` |
| radare2 | Optional | Secondary analysis | `r2 --version` |
| git | Yes | Version control | `git --version` |
| wine / proton | Yes | Run the game for validation | `wine --version` |

Write the verification results to `logs/toolchain_check.txt`.

---

## 4. Phase 1 — Binary Survey (No Modifications)

**Goal:** Establish ground truth. Populate top-level spec entries. No patching.

### 1.1 — PE Header Analysis ✅ COMPLETE

**Findings (see `openspec/engine/binary_metadata.md`):**
- **x86-64** (post-November 2023 Steam patch) — 64-bit toolchain throughout
- LAA flag: not applicable (64-bit binary)
- Threading primitives: `CreateThread`, `TlsAlloc/Get/Set/Free`, `InitializeCriticalSection`, `EnterCriticalSection` — **engine already uses threads**; `_beginthreadex` absent (uses `CreateThread` directly)
- **Direct3D 9** via `d3d9.dll` + `d3dx9_43.dll` — legacy renderer, NOT D3D11/12
- Lua: **statically linked** — no `lua*.dll` in import table
- TLS directory present — thread-local state confirmed

**Important clarification:** `swfoc.exe` is a 1.7MB launcher stub. The engine is `StarWarsG.exe` (12MB). All analysis targets `StarWarsG.exe`.

SHA-256 of `StarWarsG.exe`: `07daeeaec9d1e751383b1db3f5847b6c66f68785440a6c52c87b85f723d018a4`

### 1.2 — Ghidra Initial Import + String Survey ✅ COMPLETE

**How to run (for future sessions):**
```bash
# From inside nix develop:
analyzeHeadless ghidra_projects EawProject \
  -import binaries/original/StarWarsG.exe \
  -processor x86:LE:64:default \
  -postScript ExportFunctions.java \
  -postScript ExportStrings.java \
  -log logs/ghidra_analysis.log
# NOTE: Use Java scripts, not Python. Headless Ghidra does not support PyGhidra.
# NOTE: Project goes in ghidra_projects/, not /tmp.
```

**Findings:**
- 21,744 functions exported to `logs/functions.tsv`
- 17,758 strings exported to `logs/ghidra_strings.tsv`
- Named thread roster confirmed: `Main Thread`, `LoadThread`, `PacketHandler Thread`, `NATUtilsThread`, `LuaScriptThread: Main State`, `LuaScriptThread: <name>`
- `ThreadLockMutexClass` — custom Petroglyph mutex wrapper with 10-second deadlock detection
- Render task system: `alRenderTask` base + 14 concrete task types, `MultiLinkedListClass<alRenderTask>` storage
- Flush variants (`ValLaserBeamFlushTask`, `ValBlobFlushTask`, etc.) confirm two-phase accumulate+flush pattern

See `openspec/engine/threading_model.md` and `openspec/engine/subsystems/renderer.md`.

### 1.3 — Game Loop Identification ✅ COMPLETE

**Findings (see `openspec/engine/game_loop.md`):**
- **WinMain** = `FUN_0x5d990` (RVA `0x5d990`, 7440 bytes)
- Frame timing: `timeGetTime()` based, not QPC. `timeBeginPeriod()` at startup for 1ms resolution.
- Active/idle gating: `DAT_1409cf314` — `WaitMessage()` when no game loaded, tight spin loop in-game
- Primary game-state update: `FUN_14021caf0` (takes abs time + camera params) — likely the GOM tick
- No Sleep in the active game path — fully single-threaded main loop
- QPC-based timer cluster at `0x222e20–0x222fa0` is a **separate subsystem** (animation/particle timing)

**Key architectural finding:** The main loop is single-threaded — all subsystem ticks run sequentially on the main thread. Worker threads (`LoadThread`, etc.) are started at startup and run independently. This confirms the baseline that Model A assumes.

### 1.4 — Focused Decompilation (Remaining) 🔲 IN PROGRESS

Three open questions from 1.3 that block Phase 2 spec entries:

**A. Render task flush RVA** — Find the function that drains `MultiLinkedListClass<alRenderTask>` and issues D3D draw calls. This is the exact split point for Model A. Without it, the renderer Phase 2 entry cannot be completed.
- Strategy: xref from `alRenderTask` vftable or find `IDirect3DDevice9::DrawIndexedPrimitive` callers

**B. GOM tick confirmation** — Confirm `FUN_14021caf0` is the game object manager tick (decompile + cross-reference with GOM-related strings like `GameObjectManager`)

**C. Lua threading** — Confirm whether `LuaScriptThread` entries are OS-level threads (`CreateThread`) or Lua coroutines on one OS thread. This is the critical gate for Phase 3 Model B.
- Strategy: xref `LuaScriptThread` string to the function that creates it; check if it calls `CreateThread`

Document results in `openspec/engine/subsystems/script_engine.md` (Lua) and `openspec/engine/subsystems/renderer.md` (flush RVA).

---

## 5. Phase 2 — Subsystem Mapping

**Goal:** Map each major subsystem's data dependencies to understand what can be safely parallelized.

**Dependency on Phase 1.4:** The renderer entry (priority 1) requires the flush RVA from 1.4A. The script engine entry (priority 2) requires the Lua threading answer from 1.4C. Complete 1.4 before finalizing those two entries.

For each file in `openspec/engine/subsystems/`, produce this minimum spec entry:

```markdown
## [Subsystem Name]

### Entry Point
- RVA: 0xXXXXXXXX
- Called from: [caller RVA]
- Call frequency: [every frame / every N ticks / event-driven]

### Reads from
- [global data structures or memory regions this subsystem reads]

### Writes to
- [global data structures or memory regions this subsystem writes]

### Thread Safety Assessment
- [ ] Reads shared mutable state
- [ ] Writes shared state
- [ ] Contains blocking calls
- [ ] Contains timing-sensitive operations
- Notes:

### Parallelization Candidate?
- Rating: High / Medium / Low / No
- Rationale:
```

**Priority order:**
1. Renderer (highest value — render thread split is the first real win)
2. Script engine (highest pain point — galactic map stalls)
3. AI update loop
4. Pathfinding
5. Entity/unit state update
6. Galactic map tick

---

## 6. Phase 3 — Threading Architecture Design

**Do not begin until Phase 2 is at least 60% complete and has been human-reviewed.**

Evaluate these models in order of risk. Choose the lowest-risk viable option first:

### Model A — Render Thread Separation (Lowest Risk)
Separate render submission from game logic. Game logic thread writes a command buffer each frame; render thread consumes it. One-way queue, minimal shared state risk.

**Gate:** Renderer must read game state in a way that can be snapshotted or double-buffered.

### Model B — Script Engine Offload (Medium Risk)
Move Lua tick to a worker thread running one frame ahead or behind. Removes stall from main thread at the cost of one frame of script latency.

**Gate:** Scripts must not require synchronous same-frame reads/writes to shared state.

### Model C — AI Faction Parallelism (Higher Risk)
Fork-join per faction AI update. Snapshot world state, dispatch worker per faction, join before write-back.

**Gate:** Faction AI updates must be sufficiently isolated during the tick.

Document chosen architecture in `openspec/engine/threading_model.md` with full rationale. **Human approval required before Phase 4.**

---

## 7. Phase 4 — Patch Implementation

**Do not begin until threading architecture is human-approved.**

### Hook DLL on Wine/Linux

```c
// hooks/eaw-mt-hook.c
#include <windows.h>

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
    if (fdwReason == DLL_PROCESS_ATTACH) {
        OutputDebugStringA("[eaw-mt] Hook DLL loaded successfully\n");
    }
    return TRUE;
}
```

```bash
# 64-bit build:
x86_64-w64-mingw32-gcc -shared -o patches/experimental/eaw-mt-hook.dll \
  hooks/eaw-mt-hook.c -lkernel32

# 32-bit build:
i686-w64-mingw32-gcc -shared -o patches/experimental/eaw-mt-hook.dll \
  hooks/eaw-mt-hook.c -lkernel32
```

Inject via Steam launch options:
```
WINEDLLOVERRIDES="eaw-mt-hook=n,b" %command%
```

Verify load with Frida:
```bash
frida -n swfoc.exe -e \
  "Process.enumerateModules().forEach(m => { if(m.name.includes('hook')) console.log('Hook loaded:', m.name); })"
```

### Implementation Order

Each step requires human validation before proceeding:

1. **Scaffold only:** Load DLL, confirm it prints, exit cleanly. No behavior changes.
2. **Render thread separation (Model A):** Validate over 30+ minute session. Human sign-off required.
3. **Profile:** Use Frida to measure where remaining frame time goes after Model A.
4. **Second thread:** Implement Model B or C based on profiling. One at a time.

---

## 8. OpenSpec Conventions

Every entry in `openspec/functions/index.md`:

```markdown
### fn_XXXXXXXX
- **RVA:** 0xXXXXXXXX
- **Name (inferred):** e.g., `ScriptEngine_Tick`
- **Confidence:** High / Medium / Low
- **Signature (inferred):** `void __cdecl fn_XXXXXXXX(GameState* state, float dt)`
- **Calling convention:** `__cdecl` / `__stdcall` / `__thiscall` / unknown
- **Purpose:** Description of what this function does
- **Evidence:** How we determined this
- **Called by:** [callers with RVAs]
- **Calls:** [callees with RVAs]
- **Shared state touched:** [list]
- **Threading notes:** [relevant observations]
- **Last verified:** YYYY-MM-DD
```

---

## 9. Session Startup Protocol

At the start of every session, before any work:

1. Read `openspec/README.md`
2. Read `openspec/engine/game_loop.md`
3. Read `openspec/engine/threading_model.md`
4. Scan `openspec/functions/index.md` for TODOs from prior sessions
5. Read the most recent file in `sessions/`
6. State explicitly: *"Current phase is X. Today's objective is Y. Known blockers are Z."*

---

## 10. Session Startup State

### Completed (Phase 1.1 – 1.3) ✅

1. ✅ Dev environment (`nix develop`) verified. `flake.lock` committed.
2. ✅ Binaries located at `~/gam/steam/steamapps/common/Star Wars Empire at War/corruption/`. Primary target is `StarWarsG.exe` (12MB), not `swfoc.exe` (launcher stub).
3. ✅ Repository structure created.
4. ✅ All binaries copied to `binaries/original/` with SHA-256s in `CHECKSUMS.txt`. Directory set read-only.
5. ✅ Initial commit made. Phase 1.2/1.3 committed as `b8f62e9`.
6. ✅ Phase 1.1 complete — 64-bit, D3D9, threading primitives confirmed, Lua statically linked.
7. ✅ Phase 1.2 complete — 21K functions exported, string survey done, thread roster + render task system documented.
8. ✅ Phase 1.3 complete — WinMain identified at `0x5d990`, game loop structure documented, frame timing confirmed as `timeGetTime()`-based.

### Current Task — Phase 1.4

Complete the three targeted decompiles before beginning Phase 2:

**A.** Find render task flush RVA (xref from `IDirect3DDevice9::DrawIndexedPrimitive` or `alRenderTask` vftable)
**B.** Confirm `FUN_14021caf0` is the GOM tick (decompile it)
**C.** Confirm Lua thread model — OS threads or coroutines (xref `LuaScriptThread` string → creation site → check for `CreateThread` call)

### Next Phase — Phase 2 Subsystem Mapping

Begin after Phase 1.4 is complete. Priority order per Section 5.
