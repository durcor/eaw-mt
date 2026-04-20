# Phase 4 Async Render Analysis — Context Handoff

**Date:** 2026-04-19  
**Goal:** Enable true flush#1 async parallelism (render thread runs flush#1 concurrently with inter-flush WinMain work).  
**Status:** Blocked — root cause identified, next investigation step defined.

---

## What's Already Implemented

`hooks/winmm_proxy.c` (winmm.dll proxy, built with `just build-winmm`, deployed via `WINEDLLOVERRIDES="winmm=n,b"`):

- Inline hook on `FUN_140180d90` (RVA `0x180d90`) — 14-byte JMP redirect to `render_flush_hook`
- Dedicated render thread receives `g_render_kick` event, calls `g_trampoline` (full `FUN_140180dc0` + `FUN_140149650`), signals `g_render_done`
- Main thread always blocks on `g_render_done` — **fully serialized, stable**
- Sim tick vtable hook at `DAT_140b15418->vtable[0x158/8]` (index 43) — pass-through in serialized mode, infrastructure for async
- Profiling: QPC timing, 300-frame window, logged via `OutputDebugString`

The serialized baseline is committed and working. The async path was attempted and reverted due to crashes described below.

---

## WinMain Frame Structure

From `logs/phase3c_winmain_full.c` (Ghidra decompile of WinMain):

```
sim_tick (line 556)       — DAT_140b15418->vtable[0x158] call
                          — writes render tasks into 12 pass lists
flush#1 (line 571)        — thunk_FUN_1401768f0() → FUN_140180d90
  [inter-flush work]      — lines 572–626 (see below)
flush#2 (line 627)        — thunk_FUN_1401768f0() → FUN_140180d90
  [post-flush cleanup]    — zeroes sentinel.next pointers
next sim_tick...
```

Two render flushes per frame. No sim_tick between them. `FUN_140149650` (drain/free all tasks) runs inside `FUN_140180d90` and is called for BOTH flush#1 and flush#2.

---

## Render Task List Layout

- 12 pass sentinel nodes at `driver + 0x6c2f0`, stride `0x48`
- Each pass is a circular doubly-linked list; `sentinel.next == sentinel` means empty
- `FUN_140180dc0` (RVA `0x180dc0`, 2078 bytes): flush body — traverses all 12 lists, issues D3D draw calls
- `FUN_140149650` (RVA `0x149650`): drain — destructs all task objects, resets all 12 sentinels

---

## Crash #1 — TOCTOU in flush body

**Symptom:** Access violation reading `0x18` at RVA `0x180e80` (FUN_140180dc0 +0xC0), or `0x5B` at RVA `0x180e89` (+0xC9).

**Disassembly (from `logs/phase4_crash_site.txt`):**
```asm
+0x0a9  CMP qword ptr [R9 + 0x8], R9   ; check: sentinel.next == sentinel?
+0x0ad  JZ  skip
+0x0b3  MOV RDI, qword ptr [RAX + 0x8] ; SECOND load of sentinel.next
+0x0b7  CMP RDI, RAX
+0x0ba  JZ  skip
+0x0c0  MOV RBX, qword ptr [RDI + 0x18]; CRASH if RDI=NULL → reads 0x18
+0x0c9  MOV R15D, dword ptr [RBX + 0x5C]; CRASH if RBX=bad → reads 0x5B
```

**Root cause:** Two loads of `sentinel.next` with no lock. Post-flush WinMain code (after flush#2, heading into the next sim_tick) zeroes `sentinel.next` between the guard check and the actual load. Classic TOCTOU.

**Was fixed by:** Serializing flush#2 (main thread blocks). But flush#1 still had a race.

---

## Crash #2 — Task object corruption during flush#1 async

**Symptom:** Game ran ~10 seconds with visual flickering (lighting/texture corruption), then access violation reading `0xFFFFFFFFFFFFFFFF` at RVA `0x180637` on the render thread.

**Significance:** RVA `0x180637` is inside the flush body at a task's draw dispatch — `(*vtable[0])(task, driver)` or similar. The task pointer itself was valid but the task object memory contained `-1`, meaning the memory was freed/overwritten while the flush was reading it.

**Concurrent activity:** While flush#1 was running async on the render thread, the main thread was executing WinMain lines 572–626 (inter-flush code).

---

## Inter-flush Code Analysis (WinMain lines 572–626)

Full decompile in `logs/phase3c_winmain_full.c` lines 572–626. Summary:

| Lines | What it does |
|-------|-------------|
| 572–579 | Periodic stat flush — `timeGetTime` gated, calls `FUN_140066980` + `FUN_14004fba0` + `FUN_140262af0` |
| 580–591 | Subsystem ticks — `thunk_FUN_14024bb80`, `FUN_1402505c0`, `FUN_1402089e0`, `FUN_1403b08c0`, `FUN_1402c2910`, conditional `FUN_1402c0c70`, `FUN_1404908c0`, `FUN_1400caa60`. All take global object pointers + `uVar5 = DAT_140b0a320`. No render-task-list pointer passed explicitly. |
| 592–614 | Timing: delta-time calc, `FUN_140215b90(float deltaT)`, two getter+consumer pairs |
| 615–624 | Game-active block (`DAT_1409cf314 == 1`): `FUN_14021caf0(&DAT_140a7d0a0, iVar14, &camera_structs...)`, then `FUN_1402d72c0`, `FUN_1402d2ab0`, `FUN_140321dc0`, CFG-guard indirect calls, `FUN_140325190` |
| 625–626 | CFG indirect dispatch on global vtable, `FUN_1401c8420` |

**Key observation:** None of these functions receive an explicit render-task-list pointer. They look like subsystem ticks (AI, camera, audio, etc.).

---

## Working Hypothesis

The crash at RVA `0x180637` (task draw call reading `-1`) is caused by the **task allocator reusing freed memory** between flush#1 and flush#2:

1. flush#1 kicks async — render thread starts reading task objects from pass lists
2. Inter-flush code (lines 572–626) calls subsystems that **submit new render tasks for flush#2**
3. These new task submissions call the task allocator (`FUN_140045580`, seen in flush body at line 84)
4. The allocator reuses memory from tasks that have already been "seen" but not yet formally freed (drain hasn't run)
5. flush#1 render thread dereferences a now-reused task pointer → reads `-1` → crash

This is **not** a list-structure double-buffering problem — it's a **task allocator pool** problem. The allocator is handing out memory that flush#1 still holds live pointers to.

**If this hypothesis is correct:**
- Double-buffering just the list sentinel nodes would NOT fix it
- You need separate allocator pools per frame (front/back), so flush#2 task allocations come from a different pool than flush#1 task objects
- This is significantly more work than list double-buffering

**If the hypothesis is wrong (inter-flush code doesn't submit new tasks):**
- The corruption source is something else — possibly a shared transform/matrix buffer that tasks reference by pointer
- Would need to decompile the functions in lines 580–624 to find what they modify

---

## Next Investigation Step

Decompile **`FUN_140045580`** (the task allocator — called in flush body when batch array is full, and presumably called by all render task submission sites):

```java
// In Ghidra script:
Address fn = getAddressFactory().getDefaultAddressSpace().getAddress(base + 0x45580L);
```

Questions to answer:
1. Is this a pool allocator? Does it recycle freed blocks immediately?
2. What is the allocator's free-list / pool structure?
3. Are there multiple pools or one global pool?

If there's one global pool with immediate recycling, then true async requires a per-frame double-buffered pool. The scope would be: find all render task allocation call sites (xref to `FUN_140045580` or task constructors), patch them to use a frame-indexed pool, swap pools at flush boundary.

Also useful: **xref count on `FUN_140045580`** — how many call sites are there? This bounds the patching work.

---

## Files of Interest

| File | Contents |
|------|----------|
| `hooks/winmm_proxy.c` | Hook DLL — serialized render thread, stable |
| `logs/phase3c_winmain_full.c` | Full WinMain decompile (lines 556–627 are the relevant frame) |
| `logs/phase4_crash_site.txt` | Disassembly of crash site + FUN_140180dc0 loop entry |
| `logs/phase4_flush_body.c` | Full decompile of FUN_140180dc0 (flush body, 2078 bytes) |
| `openspec/engine/subsystems/renderer.md` | Spec — frame structure, task list layout, async blocker analysis |
| `justfile` | `just build-winmm` builds+deploys; `just ghidra-script <Name>` runs Ghidra headless |

## Ghidra Project

- Location: `ghidra_projects/EawProject`, process `StarWarsG.exe`
- Image base (ASLR off in Ghidra): `0x140000000`
- Run scripts: `just ghidra-script <ScriptName>` (no `.java` extension)
- Scripts live in `tools/ghidra_scripts/`
- Output logs go to `logs/`
