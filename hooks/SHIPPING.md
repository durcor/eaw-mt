# EaW Hook DLL — Shipping Manifest

`winmm.dll` is a proxy DLL (`hooks/winmm_proxy.c`) injected via Wine/Proton's
`WINEDLLOVERRIDES="winmm=n,b"`. It forwards the four `winmm` exports StarWarsG.exe imports
and installs a set of stability/performance fixes for FoC + Thrawn's Revenge under DXVK.

## Build

| Command | Result |
|---|---|
| `just build-winmm` | **Release** (default) — fixes only, profiler compiled out. Ship this. |
| `just build-winmm-profile` | Adds `-DEAW_PROFILE` — full per-function timing profiler, 300-frame stats dump (`eaw-mt.log`), and the stall-sampling watchdog thread. For diagnostics only. |

Both build the same source; `#ifdef EAW_PROFILE` blocks are measurement-only and never affect the
shipped fixes. The DLL is copied to the game's `corruption/` directory automatically.

## Shipped fixes (always active in both builds)

| Fix | What it does | Patch point | Kill-switch |
|---|---|---|---|
| **fscache** | Negative directory cache: enumerates a `Data\` dir once, returns FILE_NOT_FOUND instantly for absent assets — kills the first-encounter NtCreateFile open-storm (~14s→~1.1s cold). | `CreateFileA/W` IAT | `EAW_NO_FSCACHE=1` |
| **MEG index** | Builds a filename index of all `Data\*.meg` (+ workshop/mods) at startup from the exe path. | `build_meg_index` | — (see `meg_has`) |
| **meg_has skip** | Skips the ~9ms in-memory MEG scan for paths absent from the index. | `b2136f0` | `EAW_NO_MEGSKIP=1` |
| **nf_cache** | Memoizes MEG "not-found" lookups (7000× on repeat). | `b141f70` | (covered by `EAW_NO_MEGSKIP`) |
| **b25ec10 cache** | Caches art-model lookup results keyed on `path|filter`; flushes on asset-DB pointer change. | `b25ec10` | `EAW_NO_B25CACHE=1` |
| **Fix B1** | Opp-target search interval 0→60f (rescan every ~30 ticks, not every tick). | `DAT_140b0a340` | — |
| **Fix B2** | Caps opp-target search candidates per component (`OPP_SEARCH_CAP=25`). Depends on the b387400 reset-wrapper. | `b385190` in `b387400` | — |
| **Fix B3** | Per-gsvc Lua-AI time budget (`PUMPE_BUDGET_MS=33`): skips remaining entities once a service pass exceeds budget. Reset each pass by the game-service hook. | `pumpe`/`game_service` | — |
| **Prewarm OFF** | Lua AI prewarm is disabled by default — it corrupted the shared Lua `global_State` and caused progressive battle-re-entry crashes. | `ai_init_prewarm_hook` | `EAW_PREWARM=1` (opt back in) |
| **WaitMessage cap** | Caps the idle `WaitMessage` to 1ms so the game keeps running when the window is unfocused. | `WaitMessage` IAT | — |
| **VEH crash guards** | Vectored handler recovers specific read/write-AVs: alHModel null render-pass (`0x12b6bf`/`0x12b6fb`), Wine wake_addr corruption (WAKE_SUPPR), galactic teardown null. | `veh_crash_handler` | — |
| **QPC→timeGetTime** | All hook timing uses VDSO `timeGetTime` instead of `QueryPerformanceCounter` (QPC is unreliable in hooks under Wine). | `tgt_fake_qpc` | — |

## Render thread (Model A)

The render flush hook (`FUN_140180d90`) is installed as the frame anchor + binary-version sanity
check, but the render-thread **offload is dormant** — flush runs on the main thread. Measured
ceiling is ~0 (flush is 0.01ms/frame; DXVK executes draws asynchronously), so there is no perf
reason to enable it. See `openspec/engine/subsystems/renderer.md`.

## Environment variables

| Variable | Effect |
|---|---|
| `EAW_NO_FSCACHE=1` | Disable the negative directory cache. |
| `EAW_NO_MEGSKIP=1` | Disable the MEG index skip (`meg_has` always returns present). |
| `EAW_NO_B25CACHE=1` | Disable the b25ec10 art-model result cache. |
| `EAW_PREWARM=1` | Re-enable Lua AI prewarm (off by default; known to corrupt the Lua VM). |
| `EAW_PROFILE` | **Build-time** define (not runtime) — enables the profiler. |

All caches/skips are conservative: collisions can only produce false-present (never false-absent),
so a kill-switch is only needed for diagnostics, not correctness.
