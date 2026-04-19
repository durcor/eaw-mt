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

### Finding Your Proton Prefix

Steam game files and the Wine prefix for EaW are separate locations:

```
# Binary (what we analyze and patch):
~/.steam/steam/steamapps/common/Star Wars Empire at War/corruption/swfoc.exe

# Proton prefix (Wine C: drive, config, etc.):
~/.steam/steam/steamapps/compatdata/32470/pfx/

# EaW Gold Pack Steam App ID: 32470
```

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
│   │   ├── swfoc.exe           # Forces of Corruption main executable
│   │   ├── sweaw.exe           # Base game executable
│   │   └── *.dll               # All DLLs from the corruption/ directory
│   └── patched/                # Working copies for modification
├── openspec/
│   ├── README.md               # Spec conventions and glossary
│   ├── engine/
│   │   ├── game_loop.md
│   │   ├── memory_layout.md
│   │   ├── binary_metadata.md  # Architecture, imports, section layout
│   │   ├── threading_model.md  # Initially: "single-threaded, main thread only"
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
│   ├── ghidra_scripts/         # Custom Ghidra analysis scripts (Python/Java)
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

### 1.1 — PE Header Analysis

Using `pefile`, extract and document in `openspec/engine/binary_metadata.md`:

- Exact binary architecture — **critical question: is this the pre-2023 32-bit build or the post-November 2023 64-bit Steam build?** The 2023 patch converted the engine to 64-bit. This determines the entire toolchain going forward (MinGW target, Frida arch, Ghidra processor selection).
- Whether the Large Address Aware (LAA) flag is set (relevant mainly if it's still a 32-bit build)
- All imported DLLs and key imported functions — pay close attention to:
  - Any threading primitives: `CreateThread`, `InitializeCriticalSection`, `WaitForSingleObject`, `SetThreadAffinityMask`, `_beginthreadex`
  - D3D version: `d3d9.dll` vs `d3d11.dll` vs `d3d12.dll` (indicates renderer generation)
  - Lua DLL if dynamically linked (`lua51.dll`, `lua52.dll`, etc.)
- Section layout (`.text`, `.data`, `.rdata`, `.bss` sizes and RVAs)
- Any TLS (Thread Local Storage) directory — its presence means the game already uses thread-local state somewhere

Sample script skeleton:

```python
import pefile
import hashlib

pe = pefile.PE("binaries/original/swfoc.exe")

# Architecture
arch = "x86-64" if pe.FILE_HEADER.Machine == 0x8664 else "x86-32"

# LAA flag
laa = bool(pe.OPTIONAL_HEADER.DllCharacteristics & 0x0020)

# Imports
for entry in pe.DIRECTORY_ENTRY_IMPORT:
    dll = entry.dll.decode()
    funcs = [imp.name.decode() for imp in entry.imports if imp.name]
    # write to spec...

# SHA-256
with open("binaries/original/swfoc.exe", "rb") as f:
    sha256 = hashlib.sha256(f.read()).hexdigest()
```

**Key question to answer and document explicitly:** Does `swfoc.exe` import *any* threading primitives? Even if unused, this tells us whether Petroglyph's 2023 patches introduced any threading scaffolding.

### 1.2 — Ghidra Initial Import

Run headless Ghidra analysis. Select the correct processor based on the architecture found in 1.1:

```bash
analyzeHeadless /tmp/eaw-ghidra EawProject \
  -import binaries/original/swfoc.exe \
  -processor x86:LE:64:default \
  -postScript ExportFunctions.py \
  -postScript ExportStrings.py \
  -log logs/ghidra_analysis.log
# Use x86:LE:32:default for a 32-bit binary
```

Write `tools/ghidra_scripts/ExportFunctions.py` and `ExportStrings.py` to dump function lists and strings to `logs/`.

From the strings output, search for and document:
- Thread/job/fiber/queue/worker keyword hits
- Lua-related strings (`.lua`, `lua_`, error messages)
- Source file path fragments in assert strings (e.g. `../src/AI/GalacticAI.cpp` — these are goldmines)
- Known EaW asset names (confirms we have the right binary)

**Any recovered source file path fragments must be immediately added to `openspec/engine/` as they directly anchor the subsystem map.**

### 1.3 — Game Loop Identification

Locate the main loop via calls to timing/sleep functions. In the Ghidra decompiler, search for cross-references to:
- `Sleep` / `SleepEx`
- `QueryPerformanceCounter`
- `WaitForSingleObject` with a non-zero timeout

The function containing one of these in a tight conditional loop, branching on a "game running" boolean, and calling many subsystem-sized functions is the main loop.

Document in `openspec/engine/game_loop.md`:
- RVA of the main loop
- Approximate pseudocode structure
- Each direct callee with RVA and preliminary purpose guess

### 1.4 — Script Engine Identification

Search for Lua presence:
1. Check imports for `lua*.dll` (dynamic link)
2. If statically linked, search strings for `lua_` function name patterns, `.lua` extensions, Lua error message templates
3. Search strings for known Thrawn's Revenge script names or paths

Document in `openspec/engine/subsystems/script_engine.md`:
- Dynamic vs static Lua linkage
- Lua version if determinable
- RVA of the script tick function
- Whether execution is synchronous on the main thread

---

## 5. Phase 2 — Subsystem Mapping

**Goal:** Map each major subsystem's data dependencies to understand what can be safely parallelized.

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

## 10. Immediate First Task

Execute these steps in order and do not skip ahead:

1. Enter the dev environment (`nix develop`) and verify all tools from Section 3. Write results to `logs/toolchain_check.txt`. Commit `flake.lock` if it was just generated.
2. Locate `swfoc.exe` on disk. Search in this order:
   - `~/gam/steam/steamapps/common/Star Wars Empire at War/corruption/swfoc.exe`
   - `~/.steam/steam/steamapps/common/Star Wars Empire at War/corruption/swfoc.exe`
   - `~/.local/share/Steam/steamapps/common/Star Wars Empire at War/corruption/swfoc.exe`
   - Run `find ~ -name "swfoc.exe" 2>/dev/null` as a fallback
   Report the exact path found.
3. Create the full repository structure from Section 2.
4. Copy `swfoc.exe`, `sweaw.exe`, and all DLLs from the game's `corruption/` directory into `binaries/original/`. Compute SHA-256 checksums for every file and write them to `binaries/original/CHECKSUMS.txt`. Set the directory read-only: `chmod -R a-w binaries/original/`.
5. Initialize git and make an initial commit with message `"init: project structure and original binary checksums"`.
6. Run Phase 1.1 (PE Header Analysis) and report back:
   - **32-bit or 64-bit?** (Most critical finding — determines the entire toolchain)
   - LAA flag status
   - Any threading primitives imported?
   - Which Direct3D version is imported?
   - Full SHA-256 of `swfoc.exe`

**Pause after step 6 and wait for human review before continuing to Phase 1.2.**
