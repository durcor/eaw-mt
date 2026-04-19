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

### 1.4 — Focused Decompilation ✅ COMPLETE

**A. Render flush RVA** — `FUN_0x180dc0` (2078 bytes). 12-pass render task list iterator; per-task `vftable[0x10]` Execute; batch geometry flush via `FUN_140183e80`; post-process scene/distortion texture handling. Takes `(alGraphicsDriver*, uint pass_mask)`. See `openspec/engine/subsystems/renderer.md`.

**B. Audio system, not GOM** — `FUN_14021caf0` is the **Miles Sound System audio tick** (calls `AIL_set_listener_3D_position`, `AIL_serve`, etc.), not the GOM tick. The GOM tick is unidentified; finding it is a Phase 2 task.

**C. Lua confirmed as coroutines** — Only **4 OS threads** in the entire binary (main + LoadThread + PacketHandler + NATUtils). All `_beginthreadex` calls go through one wrapper (`FUN_0x22e490`), which has exactly 5 call sites. No Lua OS thread creation anywhere. `LuaScriptThread` labels are Lua coroutine names. This removes the Model B gate blocker — moving Lua to a dedicated OS thread is now confirmed viable (pending shared-state audit in Phase 2). See `openspec/engine/threading_model.md`.

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

Phase 2 is complete. Phase 3 reverse engineering (GOM/simulation tick drill-down) is complete. Architecture design is now the active work.

Evaluate these models in order of risk. Choose the lowest-risk viable option first:

### Model A — Render Thread Separation (Lowest Risk)
Separate render submission from game logic. Game logic thread writes a command buffer each frame; render thread consumes it. One-way queue, minimal shared state risk.

**Gate:** Renderer must read game state in a way that can be snapshotted or double-buffered.

### Model B — Lua Coroutine Pump Offload (Medium Risk)
Move `FUN_140247a90` (Pump_Threads) to a worker thread. The coroutine array (`this+0x40`) appears independently iterable — each slot holds one unit's Lua coroutine state. One frame of script latency is acceptable for Thrawn's Revenge AI.

**Gate (open — partially cleared):**
- ✅ `thunk_FUN_1407bc310` confirmed as Lua 5.1 `lua_resume` — coroutines are independent lua_State objects.
- ✅ `ServiceRate`/`LastService`: Lua globals read at start of `FUN_140488660` before entering Pump_Threads — snapshot before dispatch is straightforward.
- ✅ Signal listener list: read-only during Pump_Threads execution; only written during subscribe/unsubscribe (init/shutdown).
- ⚠️ **OPEN HAZARD**: `FUN_140247700` case 1 traverses C++ entity hierarchy (reads `entity->list[4]`, `entity->list[5]`). Main-thread command queue processor `FUN_1403b08c0` writes game-object state. These overlap and must be synchronized (or coroutine execution must be guaranteed to run before command queue processes writes, which is already the case given current call order: mode vtable[0x158] fires before `FUN_1403b08c0` in WinMain). If offloaded to a separate thread, a snapshot or read-lock of game-object state is required for yield type 1.

**Specific insertion point:** After rate-check passes in `FUN_140488660` (TheGameScoringManagerClass::Service), before the `FUN_140247a90` call. Snapshot the coroutine array pointer + count, dispatch to thread pool, join before next frame's rate-check.

### Model C — AI Faction Parallelism (Higher Risk)
Fork-join per faction AI update. Snapshot world state, dispatch worker per faction, join before write-back.

**Gate:** Faction AI updates must be sufficiently isolated during the tick. Phase 3 confirms AI is entirely Lua-based; faction isolation depends on whether Lua coroutines share any mutable C++ game-object state during a single pump cycle.

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

### Completed (Phase 2) ✅

Phase 2 subsystem mapping complete. All per-frame callees identified:

- ✅ **Renderer** — call chain confirmed: `FUN_0x180d90` wraps `FUN_0x180dc0`. Accumulate + flush are serialized. Model A insertion point confirmed.
- ✅ **Audio** — 4 game-active callees are all audio: MSS core tick, music/speech event managers, camera listener update. GOM tick is **not** in the game-active list.
- ✅ **Networking** — BroadcasterClass, multiplayer sync (59 objects/player/frame), dialog voting, session timeouts, event queue cleaner all mapped.
- ✅ **UI** — Tooltip system (33ms throttle), timed callback queue + tweening, command queue processor.
- ✅ **Game mode state machine** — `FUN_0x30c3b0` dispatches to 10+ galactic/space/land subsystem ticks per mode. GOM tick is likely inside the mode-1 galactic map dispatch.
- ✅ **Script engine** — Lua pool tick at `0x24bb80`. Coroutine scheduler tick location still unknown.

### Completed (Phase 3) ✅

Phase 3 GOM / simulation tick drill-down complete.

**Key finding: There is no native C++ per-unit iterate-and-call loop.** All per-unit AI is Lua-based, driven by a coroutine pump.

#### WinMain loop order (corrected)
Simulation tick runs **before** the Win32 message pump each frame:
```
mode->vtable[0x2B]  (offset 0x158)  ← simulation tick, runs FIRST
Win32 message pump
Render flush
```

#### Complete per-unit AI call chain
```
WinMain
  → (*DAT_140b15418 + 0x158)()  [mode vtable slot 43]
  → FUN_14035cc10               [combat mode per-frame tick]
      fires event 0x23 via FUN_140220ed0 → FUN_140240940 (signal dispatcher)
  → FUN_140488660               [TheGameScoringManagerClass::Service]
      reads Lua globals "ServiceRate" / "LastService" to rate-limit
  → FUN_140247a90               [Pump_Threads — 953 bytes]
      iterates DVC at this+0x40..+0x48, stride 0x40 per slot
  → thunk_FUN_1407bc310         [lua_resume equivalent, per coroutine]
```

#### TheGameScoringManagerClass layout (0x20 bytes)
| Offset | Field | Notes |
|--------|-------|-------|
| +0x00 | vftable | RVA 0x8742b8 |
| +0x08 | (padding) | — |
| +0x10 | lua_script | pointer to Lua script object |
| +0x18 | flags | 2-byte field (shutdown flag at +0x121 in manager) |

Singleton getter: `FUN_14004fd50`; constructor: `FUN_140485340` (28 bytes); shutdown: `FUN_1402488e0` (754 bytes).

#### Three-layer AI architecture
- **Layer 1 — Lua coroutines** (rate-limited by `ServiceRate` global): high-level AI decisions
- **Layer 2 — Command queue** `FUN_1403b08c0` (every frame): low-level command execution
- **Layer 3 — Combat schedulers** (bombing/bombardment, called directly from `FUN_14035cc10`): combat-specific timers

#### Parallelization implications for Model B / Model C
- `FUN_140247a90` (Pump_Threads) is the **primary candidate** for Model B offload — it iterates an array of Lua coroutines with no obvious inter-coroutine dependencies during a single pump.
- `FUN_1403b08c0` (command queue) is a secondary candidate; needs shared-state audit.
- Signal dispatch (`FUN_140240940`) is single-threaded; listener list is shared mutable state — must be read-locked before any threading patch.
- `ServiceRate` / `LastService` Lua globals are written by the main thread each frame; offloading the pump requires snapshotting these before dispatch.

### Current Tasks — Phase 3 Loose Ends + Phase 4 Prep

1. ⚠️ **Map LoadThread scope** — thread creation mechanism fully traced (`FUN_14022e490` → `_beginthreadex(FUN_14022e400)` → `LoadingThreadClass::vftable[1]`). Front-end trigger: `FUN_1400c5250` (button handler). LoadThread sync with main thread: `ThreadLockMutexClass` (Win32 mutex, 10-second deadlock detection), concentrated in RVA `0x219xxx–0x21exxx`. **Remaining**: decompile `LoadingThreadClass::vftable[1]` to confirm asset types and main-thread sync points.
2. ✅ **Confirm `FUN_140215b90`** — frame profiler (writes `Slow_Frame_Profile_NNN.xml` on slow frames, only called from WinMain). Not a timer subsystem.
3. ✅ **Identify mode vtable[43] for non-combat modes** — all three mode classes (`GalacticModeClass`, `LandModeClass`, `SpaceModeClass`) have `FUN_14035cc10` at vtable slot 43. Signal 0x23 only has registered listeners in combat (when `DAT_140b15b10 != 0`). Mode IDs: 0=Galactic, 1=Land, 2=Space.
4. ✅ **Trace `thunk_FUN_1407bc310`** — confirmed Lua 5.1 `lua_resume`. Coroutines are independent `lua_State*` objects; each slot in the DVC is a separate coroutine.
5. ✅ **Shared-state audit for Pump_Threads** — primary hazard: `FUN_140247700` case 1 traverses C++ entity hierarchy (reads game objects). Main thread `FUN_1403b08c0` writes game-object state. Must synchronize for Model B. See §6 Model B gate conditions.
