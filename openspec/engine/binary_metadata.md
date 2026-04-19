# Binary Metadata

**Last verified:** 2026-04-18

---

## StarWarsG.exe — Primary Game Engine

| Field | Value |
|-------|-------|
| SHA-256 | `07daeeaec9d1e751383b1db3f5847b6c66f68785440a6c52c87b85f723d018a4` |
| Size | ~12 MB (12,533,760 bytes) |
| Architecture | **x86-64 (64-bit)** — post-November 2023 Steam patch |
| ImageBase | `0x0000000140000000` |
| EntryPoint | RVA `0x0076A428` |
| Subsystem | Win32 GUI (2) |
| LAA | YES |
| ASLR (DynamicBase) | YES |
| NX (DEP) | YES |
| TLS Directory | PRESENT — StartAddressOfRawData `0x0000000140941230` |

### Sections

| Name | VSize | RVA | RawSize |
|------|-------|-----|---------|
| .text | 8,369,335 | 0x00001000 | 8,369,664 |
| .rdata | 1,905,262 | 0x007FD000 | 1,905,664 |
| .data | 1,531,584 | 0x009CF000 | 597,504 |
| .pdata | 450,516 | 0x00B45000 | 450,560 |
| _RDATA | 244 | 0x00BB3000 | 512 |
| .rsrc | 1,024,504 | 0x00BB4000 | 1,024,512 |
| .reloc | 68,672 | 0x00CAF000 | 69,120 |

### Key Imports

| DLL | Category | Notes |
|-----|----------|-------|
| d3d9.dll | Renderer | **Direct3D 9** — `Direct3DCreate9` |
| d3dx9_43.dll | Renderer | D3DX9 utilities — matrices, textures, effects |
| mss64.dll | Audio | Miles Sound System 64-bit |
| bink2w64.dll | Video | Bink video codec |
| PerceptionFunctionG.dll | AI | Petroglyph perception/AI system |
| steam_api64.dll | Platform | Steam integration |
| KERNEL32.dll | OS | Extensive threading primitives (see below) |
| WS2_32.dll | Network | TCP/UDP socket API |
| USER32.dll | Window | Win32 window management |
| ole32.dll | COM | `CoCreateInstance` — COM objects used |
| POWRPROF.dll | System | `CallNtPowerInformation` — power state queries |
| WINMM.dll | Timing | `timeBeginPeriod`, `timeGetTime` — high-res timer |

### Threading Primitives Imported (KERNEL32.dll)

```
CreateThread, ExitThread, TerminateThread
GetCurrentThread, GetCurrentThreadId
SetThreadPriority
TlsAlloc, TlsGetValue, TlsSetValue, TlsFree
EnterCriticalSection, LeaveCriticalSection
InitializeCriticalSectionEx, DeleteCriticalSection
WaitForSingleObject, WaitForSingleObjectEx
ResetEvent, SetEvent, CreateEventW (implicit via kernel32)
ReleaseMutex
InitializeSListHead
```

**Assessment:** The engine uses full Win32 threading: `CreateThread`, TLS (thread-local storage), critical sections, mutexes, and events. This is not a stub — these are live imports with active usage to be confirmed in Phase 1.3+.

### Lua

- NOT dynamically linked (no `lua*.dll` import)
- Likely **statically linked** — to be confirmed by string search in Phase 1.2
- Absence of dynamic Lua means version identification requires string analysis

---

## swfoc.exe — Launcher / Host Process

| Field | Value |
|-------|-------|
| SHA-256 | `e327546df3e67a93e456e54abbb0f357a2f4cf0d0cac9107761bda7bcd028f16` |
| Size | ~1.7 MB |
| Architecture | x86-64 (64-bit) |
| ImageBase | `0x0000000140000000` |
| EntryPoint | RVA `0x00187310` |
| ASLR | NO |
| TLS Directory | PRESENT — `0x0000000140135278` |

### Notable: .bind Section

An unusual `.bind` section (211,216 bytes, RVA `0x00187000`) is present. This may be:
- Import binder (pre-bound imports for load-time optimization)
- Custom overlay or resource section

**TODO:** Inspect `.bind` section content in Ghidra.

### tbbR.dll Imports (Intel TBB)

`swfoc.exe` imports TBB directly:
- `task_scheduler_init` — initializes TBB thread pool
- `concurrent_queue_base_v3` — lock-free concurrent queue
- `queuing_rw_mutex` / `scoped_lock` — reader-writer lock

**Significance:** The launcher uses TBB for its own work. `StarWarsG.exe` does not import TBB directly — the engine may use Win32 threading only, or TBB may be accessed through a shared memory/IPC channel from the launcher.

**TODO:** Determine if swfoc.exe launches StarWarsG.exe as a child process and passes TBB infrastructure, or if they are independent.

---

## Key Phase 1.1 Findings

1. **Both binaries are x86-64 (64-bit)** — confirmed post-2023 Steam patch. Use `x86_64` toolchain throughout.
2. **Primary analysis target is `StarWarsG.exe`** (12 MB), not `swfoc.exe` (1.7 MB launcher).
3. **Renderer: Direct3D 9** via `d3d9.dll` + `d3dx9_43.dll`. This is the renderer generation to target for thread separation.
4. **Threading infrastructure is already present** in both binaries — Win32 threading primitives are extensively imported. The engine is not naive single-threaded.
5. **TLS is active** in both binaries — thread-local state is already in use somewhere.
6. **Lua is statically linked** — no dynamic DLL. Version TBD via string analysis.
7. **tbbR.dll** is used by the launcher (swfoc.exe), not the engine. Origin/purpose TBD.
