/*
 * winmm_proxy.c — proxy winmm.dll for EaW hook injection
 *
 * Phase 4 / Phase 5: Model A render thread (serialized) + Pump_Threads timing hook.
 *
 * Installs an inline hook on FUN_140180d90 (render flush wrapper, RVA 0x180d90).
 * The render flush (12-pass D3D9 draw + present + task drain) runs on a
 * dedicated render thread.  The main thread signals the render thread then
 * blocks on render_done — fully serialized, no concurrent access to render
 * task lists.
 *
 * True frame overlap requires double-buffering the render task lists so that
 * inter-flush WinMain code (lines 571-626) cannot race with flush#1 reads.
 * That investigation is deferred; this build establishes the stable baseline.
 *
 * Hook mechanics:
 *   - First 14 bytes of FUN_140180d90 are position-independent (verified):
 *       40 53           PUSH RBX
 *       48 83 ec 20     SUB RSP,0x20
 *       ba ff 0b 00 00  MOV EDX,0xbff
 *       48 8b d9        MOV RBX,RCX
 *   - Replaced with: FF 25 00 00 00 00 <abs addr of render_flush_hook>
 *   - Trampoline (VirtualAlloc exec): saved 14 bytes + JMP to RVA+14 (0x180d9e)
 *
 * Build: just build-winmm
 *
 * Steam launch options:
 *   WINEDLLOVERRIDES="winmm=n,b" PROTON_USE_NTSYNC=1 %command% STEAMMOD=...
 */

#include <windows.h>
#include <stdint.h>
#include <stdio.h>

static FILE *g_log_fp = NULL;

static void log_write(const char *s) {
    OutputDebugStringA(s);
    if (g_log_fp) { fputs(s, g_log_fp); fflush(g_log_fp); }
}

/* =========================================================================
 * WINMM proxy — forward the 4 functions StarWarsG.exe imports
 * ========================================================================= */

static HMODULE real_winmm = NULL;

typedef MMRESULT (WINAPI *timeEndPeriod_t)(UINT);
typedef MMRESULT (WINAPI *timeBeginPeriod_t)(UINT);
typedef MMRESULT (WINAPI *timeGetDevCaps_t)(LPTIMECAPS, UINT);
typedef DWORD   (WINAPI *timeGetTime_t)(void);

static timeEndPeriod_t   real_timeEndPeriod   = NULL;
static timeBeginPeriod_t real_timeBeginPeriod  = NULL;
static timeGetDevCaps_t  real_timeGetDevCaps   = NULL;
static timeGetTime_t     real_timeGetTime      = NULL;

MMRESULT WINAPI timeEndPeriod(UINT u)              { return real_timeEndPeriod(u); }
MMRESULT WINAPI timeBeginPeriod(UINT u)            { return real_timeBeginPeriod(u); }
MMRESULT WINAPI timeGetDevCaps(LPTIMECAPS p, UINT c) { return real_timeGetDevCaps(p, c); }
DWORD    WINAPI timeGetTime(void)                  { return real_timeGetTime(); }

/* =========================================================================
 * Render thread state
 * ========================================================================= */

/* auto-reset events: each SetEvent releases exactly one waiter */
static HANDLE g_render_kick = NULL;   /* main  → render: "flush this frame" */
static HANDLE g_render_done = NULL;   /* render → main: "flush complete"     */
static volatile LONG g_shutdown = 0;

/* param_1 passed to FUN_140180d90 (alGraphicsDriver*) */
static volatile void *g_render_param = NULL;

/*
 * Trampoline: saved prologue bytes + absolute JMP back to FUN_140180d90+14.
 * Calling g_trampoline(p) executes the real FUN_140180d90 from the start,
 * which calls FUN_140180dc0 (flush body) then FUN_140149650 (task drain).
 */
typedef void (WINAPI *RenderFlushFn)(void *);
static RenderFlushFn g_trampoline = NULL;

/* =========================================================================
 * Sim tick vtable hook state
 *
 * The game mode's virtual Tick() (vtable offset 0x158, index 43) runs once per
 * frame before both render flushes.  We intercept it here so that when true
 * async is later enabled, we have the infrastructure to enforce render_done
 * before sim tick N+1.  In fully serialized mode this hook adds QPC timing.
 *
 * DAT_140b15418 (RVA 0xb15418) is the current game-mode pointer.  We re-patch
 * whenever the active vtable changes (galactic ↔ tactical ↔ cinematic, etc.).
 * ========================================================================= */
#define GAME_MODE_PTR_RVA  0xb15418ULL
#define SIM_TICK_VTBL_IDX  (0x158 / 8)    /* = 43 */
#define SLOT22_VTBL_IDX    (0x0b0 / 8)    /* = 22 — GalacticModeClass::Service (FUN_14045e030) */

typedef void (WINAPI *SimTickFn)(void *, void *, void *, void *);
static SimTickFn  g_saved_sim_tick = NULL;
static void     **g_patched_vtable = NULL;

/* Set in sim_tick_hook; read at flush#1 entry (same thread, no race). */
static LARGE_INTEGER g_sim_tick_start;
static LARGE_INTEGER g_sim_tick_end;
static BOOL          g_sim_tick_valid = FALSE;

static void WINAPI sim_tick_hook(void *a, void *b, void *c, void *d)
{
    QueryPerformanceCounter(&g_sim_tick_start);
    g_saved_sim_tick(a, b, c, d);
    QueryPerformanceCounter(&g_sim_tick_end);
    g_sim_tick_valid = TRUE;
}

/* Forward declaration needed by slot22_vtbl_hook below. */
static LARGE_INTEGER g_qpc_freq;

/* vtable slot-22 timing hook — co-managed with sim_tick via try_patch_sim_tick.
 * FUN_14045e030 decompile shows 3 params (param_1, param_2*, param_3*); use
 * 4-wide passthrough so we never corrupt a register arg regardless of arity. */
typedef void (*Slot22VtblFn)(int64_t, int64_t, int64_t, int64_t);
static Slot22VtblFn g_saved_slot22_vtbl = NULL;
static LONG   g_slot22v_count  = 0;
static double g_slot22v_sum_ms = 0, g_slot22v_max_ms = 0;

static void slot22_vtbl_hook(int64_t a, int64_t b, int64_t c, int64_t d)
{
    LARGE_INTEGER t0, t1;
    QueryPerformanceCounter(&t0);
    g_saved_slot22_vtbl(a, b, c, d);
    QueryPerformanceCounter(&t1);
    double ms = (t1.QuadPart - t0.QuadPart) / ((double)g_qpc_freq.QuadPart / 1000.0);
    g_slot22v_sum_ms += ms;
    if (ms > g_slot22v_max_ms) g_slot22v_max_ms = ms;
    g_slot22v_count++;
    if (ms >= 100.0) {
        char s[64];
        sprintf(s, "[eaw-mt] SLOT22V %.1fms\n", ms);
        log_write(s);
    }
}

/* Called from render_flush_hook on every frame.  Patches the active game
 * mode's vtable[SIM_TICK_VTBL_IDX] with sim_tick_hook, re-patching when the
 * vtable pointer changes on mode transitions. */
static void try_patch_sim_tick(void)
{
    HMODULE exe = GetModuleHandleA(NULL);
    if (!exe) return;

    void *game_mode = *(void **)((BYTE *)exe + GAME_MODE_PTR_RVA);
    if (!game_mode) return;

    void **vtable = *(void ***)game_mode;
    if (!vtable || vtable == g_patched_vtable) return;

    /* Restore previous vtable entries on mode transition */
    if (g_patched_vtable && g_saved_sim_tick) {
        DWORD old;
        VirtualProtect(&g_patched_vtable[SIM_TICK_VTBL_IDX], 8,
                       PAGE_EXECUTE_READWRITE, &old);
        g_patched_vtable[SIM_TICK_VTBL_IDX] = (void *)g_saved_sim_tick;
        VirtualProtect(&g_patched_vtable[SIM_TICK_VTBL_IDX], 8, old, &old);
        if (g_saved_slot22_vtbl) {
            VirtualProtect(&g_patched_vtable[SLOT22_VTBL_IDX], 8,
                           PAGE_EXECUTE_READWRITE, &old);
            g_patched_vtable[SLOT22_VTBL_IDX] = (void *)g_saved_slot22_vtbl;
            VirtualProtect(&g_patched_vtable[SLOT22_VTBL_IDX], 8, old, &old);
            g_saved_slot22_vtbl = NULL;
        }
        g_patched_vtable = NULL;
        g_saved_sim_tick = NULL;
    }

    DWORD old;
    if (!VirtualProtect(&vtable[SIM_TICK_VTBL_IDX], 8,
                        PAGE_EXECUTE_READWRITE, &old)) {
        log_write("[eaw-mt] WARN: VirtualProtect for sim tick vtable failed\n");
        return;
    }
    g_saved_sim_tick = (SimTickFn)vtable[SIM_TICK_VTBL_IDX];
    vtable[SIM_TICK_VTBL_IDX] = (void *)sim_tick_hook;
    VirtualProtect(&vtable[SIM_TICK_VTBL_IDX], 8, old, &old);

    /* NOTE: slot22v vtable co-patch removed — DAT_140b15418 is SpaceModeClass,
     * not GalacticModeClass. FUN_14045e030 is hooked directly via inline trampoline
     * in install_galactic_slot22_hook(). */
    g_patched_vtable = vtable;
    log_write("[eaw-mt] Sim tick hook patched\n");
}

/* Forward declaration — definition appears in the Profiling section. */
static void write_abs_jmp(BYTE *dst, uint64_t target);

/* =========================================================================
 * Pump_Threads call-site hook
 *
 * FUN_140247a90 (RVA 0x247a90) is called from FUN_140488660 (RVA 0x488660,
 * TheGameScoringManagerClass::Service, 555 bytes) via a CALL rel32.  We
 * scan for that instruction at init time and redirect it through a near
 * stub → pump_threads_hook.
 *
 * Why call-site patch instead of inline trampoline:
 *   FUN_140247a90's prologue has RSP-relative home-space saves
 *   (MOV [RSP+8],RBX …).  If those bytes were copied into a CALL-based
 *   trampoline, the CALL pushes a return address first (RSP -= 8), making
 *   all three offsets wrong by 8 and corrupting the caller's saved regs.
 *
 * Why a near stub:
 *   CALL rel32 only reaches ±2 GB.  pump_threads_hook lives in our DLL
 *   which may be loaded far from the EXE.  A 14-byte FF 25 absolute JMP
 *   stub is allocated near the call site via alloc_near, within 2 GB.
 * ========================================================================= */
#define SCORE_SERVICE_RVA   0x488660ULL  /* FUN_140488660, 555 bytes */
#define SCORE_SERVICE_SIZE  555
#define PUMP_THREADS_RVA    0x247a90ULL  /* FUN_140247a90 */

typedef void (__fastcall *PumpThreadsFn)(void *);
static PumpThreadsFn g_pump_threads_orig = NULL;

/* Per-window Pump_Threads accumulators — main thread only. */
static LONG   g_pump_count  = 0;
static double g_pump_sum_ms = 0, g_pump_min_ms = 1e9, g_pump_max_ms = 0;

static void __fastcall pump_threads_hook(void *param_1)
{
    LARGE_INTEGER t0, t1;
    QueryPerformanceCounter(&t0);
    g_pump_threads_orig(param_1);
    QueryPerformanceCounter(&t1);

    double ms = (t1.QuadPart - t0.QuadPart) / ((double)g_qpc_freq.QuadPart / 1000.0);
    g_pump_sum_ms += ms;
    if (ms < g_pump_min_ms) g_pump_min_ms = ms;
    if (ms > g_pump_max_ms) g_pump_max_ms = ms;
    g_pump_count++;
}

/* Allocate executable memory within ±2 GB of near_addr. */
static BYTE *alloc_near(BYTE *near_addr, SIZE_T size)
{
    SYSTEM_INFO si;
    GetSystemInfo(&si);
    ptrdiff_t gran = (ptrdiff_t)si.dwAllocationGranularity;
    const ptrdiff_t limit = 0x70000000;

    for (ptrdiff_t delta = gran; delta < limit; delta += gran) {
        BYTE *lo = near_addr - delta;
        BYTE *hi = near_addr + delta;
        void *p;
        if (lo >= (BYTE *)si.lpMinimumApplicationAddress) {
            p = VirtualAlloc(lo, size, MEM_COMMIT|MEM_RESERVE, PAGE_EXECUTE_READWRITE);
            if (p) return (BYTE *)p;
        }
        p = VirtualAlloc(hi, size, MEM_COMMIT|MEM_RESERVE, PAGE_EXECUTE_READWRITE);
        if (p) return (BYTE *)p;
    }
    return NULL;
}

static BOOL install_pump_hook(void)
{
    HMODULE exe = GetModuleHandleA(NULL);
    if (!exe) return FALSE;

    BYTE *service_fn  = (BYTE *)exe + SCORE_SERVICE_RVA;
    BYTE *pump_target = (BYTE *)exe + PUMP_THREADS_RVA;

    g_pump_threads_orig = (PumpThreadsFn)pump_target;

    /* Scan FUN_140488660 for the E8 CALL targeting FUN_140247a90. */
    BYTE *call_site = NULL;
    for (int i = 0; i <= SCORE_SERVICE_SIZE - 5; i++) {
        if (service_fn[i] == 0xE8) {
            int32_t rel;
            memcpy(&rel, service_fn + i + 1, 4);
            if (service_fn + i + 5 + rel == pump_target) {
                call_site = service_fn + i;
                break;
            }
        }
    }

    if (!call_site) {
        log_write("[eaw-mt] WARN: Pump_Threads CALL not found in ScoringManager::Service\n");
        return FALSE;
    }

    /* Allocate a near stub: FF 25 00 00 00 00 <abs addr of pump_threads_hook> */
    BYTE *stub = alloc_near(call_site, 14);
    if (!stub) {
        log_write("[eaw-mt] WARN: alloc_near failed for Pump_Threads stub\n");
        return FALSE;
    }
    write_abs_jmp(stub, (uint64_t)pump_threads_hook);

    /* Patch call_site rel32 to target the stub. */
    int32_t new_rel = (int32_t)(stub - (call_site + 5));
    DWORD old;
    VirtualProtect(call_site + 1, 4, PAGE_EXECUTE_READWRITE, &old);
    memcpy(call_site + 1, &new_rel, 4);
    VirtualProtect(call_site + 1, 4, old, &old);
    FlushInstructionCache(GetCurrentProcess(), call_site, 5);

    log_write("[eaw-mt] Pump_Threads call-site hook installed\n");
    return TRUE;
}

/* =========================================================================
 * WaitMessage IAT hook (Phase 5 Step 3)
 *
 * WinMain:831-834 calls WaitMessage() when DAT_1409cf314==0 and both
 * FUN_14028af60 / FUN_14024cef0 report nothing pending.  During space
 * battles this condition fires frequently, stalling the main thread
 * 1000ms+ until the OS delivers a message — confirmed as the source of
 * all Class-A inter_max spikes where every inter-flush hook shows 0ms.
 *
 * Fix: redirect the IAT entry for WaitMessage to wait_message_hook, which
 * substitutes MsgWaitForMultipleObjectsEx with a 1ms timeout.  This bounds
 * the stall at 1ms while preserving message-processing semantics (the caller
 * still sees TRUE and can loop back into PeekMessage as before).
 *
 * Deferred option A — per-invocation duration measurement:
 *   Instrument the WaitMessage call site (WinMain RVA ~0x5dbc0, the E8 CALL
 *   5 bytes before the else branch) with a QPC-before/after wrapper to emit
 *   exact block durations per hitch frame.  Not needed now: inter_max already
 *   proves WaitMessage is the source and duration = inter_max - hook_sum.
 *   Worth adding in a later pass only if a future profiling session needs the
 *   per-invocation distribution (e.g., to distinguish single long stalls from
 *   many short ones within one 300-frame window).
 *
 * Deferred option C — decompile the triggering condition:
 *   FUN_14028af60 (RVA 0x28af60) and FUN_14024cef0 (RVA 0x24cef0) determine
 *   whether WaitMessage fires.  Understanding what "pending work" and "pending
 *   input" mean here — and why they return zero during heavy AI computation —
 *   would reveal whether the game can self-wake when background threads
 *   finish.  If the AI threads could PostMessage(hwnd, WM_USER, ...) on Lua
 *   coroutine completion, WaitMessage would return immediately instead of
 *   stalling, and this IAT patch would become unnecessary.  Option C is the
 *   architecturally correct fix; option B (this patch) is the pragmatic one.
 * ========================================================================= */

static LONG g_wait_msg_count   = 0;   /* WaitMessage calls intercepted this window */
static LONG g_wait_msg_timeout = 0;   /* of those, how many hit the 1ms timeout    */

static BOOL WINAPI wait_message_hook(void) {
    InterlockedIncrement(&g_wait_msg_count);
    DWORD r = MsgWaitForMultipleObjectsEx(0, NULL, 1, QS_ALLINPUT, 0);
    if (r == WAIT_TIMEOUT) InterlockedIncrement(&g_wait_msg_timeout);
    return TRUE;
}

static BOOL install_wait_message_iat_hook(void) {
    HMODULE exe = GetModuleHandleA(NULL);
    if (!exe) return FALSE;

    BYTE *base = (BYTE *)exe;
    IMAGE_DOS_HEADER   *dos = (IMAGE_DOS_HEADER *)base;
    IMAGE_NT_HEADERS64 *nt  = (IMAGE_NT_HEADERS64 *)(base + dos->e_lfanew);

    DWORD iat_rva =
        nt->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress;
    if (!iat_rva) {
        log_write("[eaw-mt] WARN: WaitMessage patch: no import directory\n");
        return FALSE;
    }

    HMODULE user32 = GetModuleHandleA("user32.dll");
    if (!user32) user32 = LoadLibraryA("user32.dll");
    FARPROC real_wm = user32 ? GetProcAddress(user32, "WaitMessage") : NULL;
    if (!real_wm) {
        log_write("[eaw-mt] WARN: WaitMessage not found in user32\n");
        return FALSE;
    }

    IMAGE_IMPORT_DESCRIPTOR *iid =
        (IMAGE_IMPORT_DESCRIPTOR *)(base + iat_rva);
    for (; iid->Name; iid++) {
        ULONGLONG *thunk = (ULONGLONG *)(base + iid->FirstThunk);
        for (; *thunk; thunk++) {
            if ((FARPROC)(uintptr_t)*thunk == real_wm) {
                DWORD old;
                VirtualProtect(thunk, 8, PAGE_READWRITE, &old);
                *thunk = (ULONGLONG)(uintptr_t)wait_message_hook;
                VirtualProtect(thunk, 8, old, &old);
                log_write("[eaw-mt] WaitMessage IAT hook installed\n");
                return TRUE;
            }
        }
    }
    log_write("[eaw-mt] WARN: WaitMessage not found in EXE IAT\n");
    return FALSE;
}

/* =========================================================================
 * File I/O IAT hooks (Phase 5 Step 4)
 *
 * Hypothesis: EaW loads unit assets synchronously on the main thread on
 * first spawn (models, textures, audio).  LoadingThreadClass only runs
 * during loading screens; gameplay spawns trigger blocking disk reads.
 *
 * Hook CreateFileA/W to record the last filename opened on the main thread.
 * Hook ReadFile to measure call duration; emit "[eaw-mt] IO SPIKE" for
 * calls >= 10ms.  Per-window totals reported as a separate ODS line.
 * ========================================================================= */

static DWORD g_main_thread_id = 0;
static char  g_last_opened_file[MAX_PATH] = "";

static double g_io_win_sum_ms = 0.0;
static double g_io_win_max_ms = 0.0;
static LONG   g_io_win_count  = 0;
static LONG   g_io_win_bytes  = 0;

typedef HANDLE (WINAPI *CreateFileA_t)(LPCSTR, DWORD, DWORD, LPSECURITY_ATTRIBUTES, DWORD, DWORD, HANDLE);
typedef HANDLE (WINAPI *CreateFileW_t)(LPCWSTR, DWORD, DWORD, LPSECURITY_ATTRIBUTES, DWORD, DWORD, HANDLE);
typedef BOOL   (WINAPI *ReadFile_t)(HANDLE, LPVOID, DWORD, LPDWORD, LPOVERLAPPED);

static CreateFileA_t real_CreateFileA_fn = NULL;
static CreateFileW_t real_CreateFileW_fn = NULL;
static ReadFile_t    real_ReadFile_fn    = NULL;

static HANDLE WINAPI createfileA_hook(LPCSTR lpFileName, DWORD dwAccess,
    DWORD dwShare, LPSECURITY_ATTRIBUTES lpSec, DWORD dwCreate,
    DWORD dwFlags, HANDLE hTemplate)
{
    if (GetCurrentThreadId() == g_main_thread_id && lpFileName)
        strncpy(g_last_opened_file, lpFileName, MAX_PATH - 1);
    return real_CreateFileA_fn(lpFileName, dwAccess, dwShare, lpSec,
                               dwCreate, dwFlags, hTemplate);
}

static HANDLE WINAPI createfileW_hook(LPCWSTR lpFileName, DWORD dwAccess,
    DWORD dwShare, LPSECURITY_ATTRIBUTES lpSec, DWORD dwCreate,
    DWORD dwFlags, HANDLE hTemplate)
{
    if (GetCurrentThreadId() == g_main_thread_id && lpFileName)
        WideCharToMultiByte(CP_UTF8, 0, lpFileName, -1,
                            g_last_opened_file, MAX_PATH, NULL, NULL);
    return real_CreateFileW_fn(lpFileName, dwAccess, dwShare, lpSec,
                               dwCreate, dwFlags, hTemplate);
}

static BOOL WINAPI readfile_hook(HANDLE hFile, LPVOID lpBuf, DWORD nToRead,
    LPDWORD lpRead, LPOVERLAPPED lpOv)
{
    if (GetCurrentThreadId() != g_main_thread_id)
        return real_ReadFile_fn(hFile, lpBuf, nToRead, lpRead, lpOv);

    LARGE_INTEGER t0, t1;
    QueryPerformanceCounter(&t0);
    BOOL ok = real_ReadFile_fn(hFile, lpBuf, nToRead, lpRead, lpOv);
    QueryPerformanceCounter(&t1);

    double ms = (double)(t1.QuadPart - t0.QuadPart) /
                ((double)g_qpc_freq.QuadPart / 1000.0);
    DWORD bytes = (lpRead && *lpRead) ? *lpRead : nToRead;

    g_io_win_sum_ms += ms;
    if (ms > g_io_win_max_ms) g_io_win_max_ms = ms;
    InterlockedIncrement(&g_io_win_count);
    InterlockedExchangeAdd(&g_io_win_bytes, (LONG)bytes);

    if (ms >= 10.0) {
        char spike[MAX_PATH + 80];
        sprintf(spike, "[eaw-mt] IO SPIKE %.1fms %luKB file=%s\n",
                ms, (unsigned long)(bytes / 1024), g_last_opened_file);
        log_write(spike);
    }

    return ok;
}

static void install_io_iat_hooks(void) {
    HMODULE exe = GetModuleHandleA(NULL);
    if (!exe) return;
    BYTE *base = (BYTE *)exe;
    IMAGE_DOS_HEADER   *dos = (IMAGE_DOS_HEADER *)base;
    IMAGE_NT_HEADERS64 *nt  = (IMAGE_NT_HEADERS64 *)(base + dos->e_lfanew);
    DWORD iat_rva =
        nt->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress;
    if (!iat_rva) return;

    HMODULE k32 = GetModuleHandleA("kernel32.dll");
    if (!k32) k32 = LoadLibraryA("kernel32.dll");
    if (!k32) return;

    FARPROC real_cfa = GetProcAddress(k32, "CreateFileA");
    FARPROC real_cfw = GetProcAddress(k32, "CreateFileW");
    FARPROC real_rf  = GetProcAddress(k32, "ReadFile");

    real_CreateFileA_fn = (CreateFileA_t)(uintptr_t)real_cfa;
    real_CreateFileW_fn = (CreateFileW_t)(uintptr_t)real_cfw;
    real_ReadFile_fn    = (ReadFile_t)(uintptr_t)real_rf;

    int patched = 0;
    IMAGE_IMPORT_DESCRIPTOR *iid = (IMAGE_IMPORT_DESCRIPTOR *)(base + iat_rva);
    for (; iid->Name; iid++) {
        ULONGLONG *thunk = (ULONGLONG *)(base + iid->FirstThunk);
        for (; *thunk; thunk++) {
            FARPROC fn = (FARPROC)(uintptr_t)*thunk;
            DWORD old;
            if (real_cfa && fn == real_cfa) {
                VirtualProtect(thunk, 8, PAGE_READWRITE, &old);
                *thunk = (ULONGLONG)(uintptr_t)createfileA_hook;
                VirtualProtect(thunk, 8, old, &old);
                patched++;
            } else if (real_cfw && fn == real_cfw) {
                VirtualProtect(thunk, 8, PAGE_READWRITE, &old);
                *thunk = (ULONGLONG)(uintptr_t)createfileW_hook;
                VirtualProtect(thunk, 8, old, &old);
                patched++;
            } else if (real_rf && fn == real_rf) {
                VirtualProtect(thunk, 8, PAGE_READWRITE, &old);
                *thunk = (ULONGLONG)(uintptr_t)readfile_hook;
                VirtualProtect(thunk, 8, old, &old);
                patched++;
            }
        }
    }

    char msg[80];
    sprintf(msg, "[eaw-mt] IO IAT hooks: %d entries patched\n", patched);
    log_write(msg);
}

/* =========================================================================
 * Inter-flush decomposition hooks (Phase 5 Step 2)
 *
 * WinMain (RVA 0x5d990, 7440 bytes) lines 580–591 contain 8 calls between
 * flush#1-done and flush#2-entry.  Each is call-site patched (E8 scan in
 * WinMain body → near stub → hook) so that per-call QPC time is collected.
 *
 * A second OutputDebugStringA per profile window emits the breakdown.
 * ========================================================================= */
#define WINMAIN_RVA   0x5d990ULL
#define WINMAIN_SIZE  7440
#define N_IFC         27

/* Per-call accumulators — main thread only. */
static struct { LONG count; double sum_ms, min_ms, max_ms; } g_ifc[N_IFC] = {
    {0,0,1e9,0},{0,0,1e9,0},{0,0,1e9,0},{0,0,1e9,0},
    {0,0,1e9,0},{0,0,1e9,0},{0,0,1e9,0},{0,0,1e9,0},
    {0,0,1e9,0},{0,0,1e9,0},{0,0,1e9,0},{0,0,1e9,0},
    {0,0,1e9,0},{0,0,1e9,0},{0,0,1e9,0},{0,0,1e9,0},
    {0,0,1e9,0},{0,0,1e9,0},{0,0,1e9,0},{0,0,1e9,0},
    {0,0,1e9,0},{0,0,1e9,0},{0,0,1e9,0},{0,0,1e9,0},
    {0,0,1e9,0},{0,0,1e9,0}
};
static BYTE *g_ifc_site[N_IFC]; /* NULL if not installed */

static const char * const g_ifc_name[N_IFC] = {
    "24bb80","2505c0","2089e0","cmd_q",
    "2c2910","2c0c70","4908c0","caa60",
    /* lines 597-639 */
    "51d10","2d72c0","2d2ab0","321dc0",
    "1c8420","30c3b0",
    /* line 609: between hooks 8 and 9 */
    "339bc0",
    /* lines 828-871: post-hook-13 section */
    "4fba0","262b80","3b1b50","297e30","1dc60",
    /* lines 610-614: gap between hook 14 (339bc0) and hook 9 (2d72c0) */
    "022c50","067c80","6a1e0","25770","6d1d0",
    /* line 623: inside DAT_1409cf314 guard, between hooks 11 and 12 */
    "325190",
    /* line 907: late-loop "more subsystems", fires when DAT_1409cf314==1 */
    "27d800"
};
static const uint64_t g_ifc_rva[N_IFC] = {
    0x24bb80, 0x2505c0, 0x2089e0, 0x3b08c0,
    0x2c2910, 0x2c0c70, 0x4908c0, 0xcaa60,
    0x51d10,  0x2d72c0, 0x2d2ab0, 0x321dc0,
    0x1c8420, 0x30c3b0,
    0x339bc0,
    0x4fba0, 0x262b80, 0x3b1b50, 0x297e30, 0x1dc60,
    0x22c50, 0x67c80, 0x6a1e0, 0x25770, 0x6d1d0,
    0x325190,
    0x27d800
};

/* Gap timers — 3 unhooked spans between adjacent hook pairs.
 * Base is recorded at the EXIT of the boundary hook before the gap;
 * measurement happens at the ENTRY of the boundary hook after the gap. */
static LARGE_INTEGER g_gap_base_A;        /* set at exit of hook 24 (6d1d0)  */
static LARGE_INTEGER g_gap_base_B;        /* set at exit of hook 12 (1c8420) */
static LARGE_INTEGER g_gap_base_C;        /* set at exit of hook 13 (30c3b0) */
static LARGE_INTEGER g_gap_base_D;        /* set at exit of hook 19 (1dc60)  */
static LARGE_INTEGER g_gap_base_E;        /* set at exit of hook 17 (3b1b50) */
static BOOL          g_gap_base_A_valid = FALSE;
static BOOL          g_gap_base_B_valid = FALSE;
static BOOL          g_gap_base_C_valid = FALSE;
static BOOL          g_gap_base_D_valid = FALSE;
static BOOL          g_gap_base_E_valid = FALSE;
/* g_gap[0]=gapA: hook24_exit→hook9_entry    (FUN_14021caf0, 6-arg, unhookable)      */
/* g_gap[1]=gapB: hook12_exit→hook13_entry   (thunk_1768f0 + FUN_14044d*)            */
/* g_gap[2]=gapC: hook13_exit→hook15_entry   (FUN_14002ee40/305890/493f0)            */
/* g_gap[3]=gapD: hook19_exit→flush#2_entry  (WinMain ~line 871 to flush#2)          */
/* g_gap[4]=gapE: hook17_exit→hook8_entry    (lines 830→597 next frame: full back-half) */
static struct { LONG count; double sum_ms, max_ms; } g_gap[5] = {
    {0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0}
};

static void ifc_sample(int i, LARGE_INTEGER t0, LARGE_INTEGER t1) {
    double ms = (t1.QuadPart - t0.QuadPart) / ((double)g_qpc_freq.QuadPart / 1000.0);
    g_ifc[i].sum_ms += ms;
    if (ms < g_ifc[i].min_ms) g_ifc[i].min_ms = ms;
    if (ms > g_ifc[i].max_ms) g_ifc[i].max_ms = ms;
    g_ifc[i].count++;
}

#define IFC_WRAP(call, idx) do { \
    LARGE_INTEGER _t0, _t1; \
    QueryPerformanceCounter(&_t0); \
    (call); \
    QueryPerformanceCounter(&_t1); \
    ifc_sample((idx), _t0, _t1); \
} while (0)

#define IFC_WRAP_R(ret, call, idx) do { \
    LARGE_INTEGER _t0, _t1; \
    QueryPerformanceCounter(&_t0); \
    (ret) = (call); \
    QueryPerformanceCounter(&_t1); \
    ifc_sample((idx), _t0, _t1); \
} while (0)

typedef void (*VV_t)(void);
typedef void (*VP_t)(void *);
typedef void (*VPU_t)(void *, uint32_t);
typedef void (*VPI_t)(void *, int32_t);
typedef void (*VPIU_t)(void *, int32_t, uint32_t);
/* 4-arg pass-through for hooks 8-13: forwards all register args regardless of
   the actual arity so we never corrupt an argument Ghidra didn't show us. */
typedef void (*WIDE_t)(int64_t, int64_t, int64_t, int64_t);
typedef int64_t (*WIDES_t)(void);  /* 4fba0: no args, returns int64 */

static VV_t   g_ifc0=NULL;  static VV_t   g_ifc1=NULL;
static VV_t   g_ifc2=NULL;  static VP_t   g_ifc3=NULL;
static VPU_t  g_ifc4=NULL;  static VPU_t  g_ifc5=NULL;
static VP_t   g_ifc6=NULL;  static VV_t   g_ifc7=NULL;
static WIDE_t g_ifc8=NULL;  static WIDE_t g_ifc9=NULL;
static WIDE_t g_ifc10=NULL; static WIDE_t g_ifc11=NULL;
static WIDE_t g_ifc12=NULL; static WIDE_t g_ifc13=NULL;
static WIDE_t  g_ifc14=NULL;                               /* 339bc0 */
static WIDES_t g_ifc15=NULL;                               /* 4fba0 — returns value */
static WIDE_t  g_ifc16=NULL; static WIDE_t g_ifc17=NULL;  /* 262b80, 3b1b50 */
static WIDE_t  g_ifc18=NULL; static WIDE_t  g_ifc19=NULL;  /* 297e30, 1dc60  */
static WIDE_t  g_ifc20=NULL; static WIDES_t g_ifc21=NULL;  /* 022c50, 067c80 */
static WIDE_t  g_ifc22=NULL; static WIDES_t g_ifc23=NULL;  /* 6a1e0,  25770  */
static WIDE_t  g_ifc24=NULL; static WIDE_t  g_ifc25=NULL;  /* 6d1d0,  325190 */
static WIDE_t  g_ifc26=NULL;                               /* 27d800 */

static void ifc_hook0(void)                              { IFC_WRAP(g_ifc0(),         0); }
static void ifc_hook1(void)                              { IFC_WRAP(g_ifc1(),         1); }
static void ifc_hook2(void)                              { IFC_WRAP(g_ifc2(),         2); }
static void ifc_hook3(void *a)                           { IFC_WRAP(g_ifc3(a),        3); }
static void ifc_hook4(void *a, uint32_t b)               { IFC_WRAP(g_ifc4(a,b),      4); }
static void ifc_hook5(void *a, uint32_t b)               { IFC_WRAP(g_ifc5(a,b),      5); }
static void ifc_hook6(void *a)                           { IFC_WRAP(g_ifc6(a),        6); }
static void ifc_hook7(void)                              { IFC_WRAP(g_ifc7(),         7); }
/* Hooks 8-13 pass all 4 register args through (int64_t) so Ghidra under-counting
   a parameter cannot corrupt the callee's argument registers. */
static void ifc_hook8(int64_t a, int64_t b, int64_t c, int64_t d) {
    LARGE_INTEGER _t0, _t1;
    QueryPerformanceCounter(&_t0);
    if (g_gap_base_E_valid) {
        double ms = (_t0.QuadPart - g_gap_base_E.QuadPart) / ((double)g_qpc_freq.QuadPart / 1000.0);
        g_gap[4].sum_ms += ms;
        if (ms > g_gap[4].max_ms) g_gap[4].max_ms = ms;
        g_gap[4].count++;
        g_gap_base_E_valid = FALSE;
    }
    g_ifc8(a, b, c, d);
    QueryPerformanceCounter(&_t1);
    ifc_sample(8, _t0, _t1);
}
static void ifc_hook9 (int64_t a,int64_t b,int64_t c,int64_t d){
    LARGE_INTEGER _t0, _t1;
    QueryPerformanceCounter(&_t0);
    if (g_gap_base_A_valid) {
        double ms = (_t0.QuadPart - g_gap_base_A.QuadPart) / ((double)g_qpc_freq.QuadPart / 1000.0);
        g_gap[0].sum_ms += ms;
        if (ms > g_gap[0].max_ms) g_gap[0].max_ms = ms;
        g_gap[0].count++;
        g_gap_base_A_valid = FALSE;
    }
    g_ifc9(a,b,c,d);
    QueryPerformanceCounter(&_t1);
    ifc_sample(9, _t0, _t1);
}
static void ifc_hook10(int64_t a,int64_t b,int64_t c,int64_t d){IFC_WRAP(g_ifc10(a,b,c,d),10);}
static void ifc_hook11(int64_t a,int64_t b,int64_t c,int64_t d){IFC_WRAP(g_ifc11(a,b,c,d),11);}
static void ifc_hook12(int64_t a,int64_t b,int64_t c,int64_t d){
    LARGE_INTEGER _t0, _t1;
    QueryPerformanceCounter(&_t0);
    g_ifc12(a,b,c,d);
    QueryPerformanceCounter(&_t1);
    ifc_sample(12, _t0, _t1);
    g_gap_base_B = _t1;
    g_gap_base_B_valid = TRUE;
}
static void ifc_hook13(int64_t a,int64_t b,int64_t c,int64_t d){
    LARGE_INTEGER _t0, _t1;
    QueryPerformanceCounter(&_t0);
    if (g_gap_base_B_valid) {
        double ms = (_t0.QuadPart - g_gap_base_B.QuadPart) / ((double)g_qpc_freq.QuadPart / 1000.0);
        g_gap[1].sum_ms += ms;
        if (ms > g_gap[1].max_ms) g_gap[1].max_ms = ms;
        g_gap[1].count++;
        g_gap_base_B_valid = FALSE;
    }
    g_ifc13(a,b,c,d);
    QueryPerformanceCounter(&_t1);
    ifc_sample(13, _t0, _t1);
    g_gap_base_C = _t1;
    g_gap_base_C_valid = TRUE;
}
static void ifc_hook14(int64_t a,int64_t b,int64_t c,int64_t d){IFC_WRAP(g_ifc14(a,b,c,d),14);}
static int64_t ifc_hook15(void) {
    LARGE_INTEGER _t0, _t1;
    QueryPerformanceCounter(&_t0);
    if (g_gap_base_C_valid) {
        double ms = (_t0.QuadPart - g_gap_base_C.QuadPart) / ((double)g_qpc_freq.QuadPart / 1000.0);
        g_gap[2].sum_ms += ms;
        if (ms > g_gap[2].max_ms) g_gap[2].max_ms = ms;
        g_gap[2].count++;
        g_gap_base_C_valid = FALSE;
    }
    int64_t r = g_ifc15();
    QueryPerformanceCounter(&_t1);
    ifc_sample(15, _t0, _t1);
    return r;
}
static void ifc_hook16(int64_t a,int64_t b,int64_t c,int64_t d){IFC_WRAP(g_ifc16(a,b,c,d),16);}
static void ifc_hook17(int64_t a, int64_t b, int64_t c, int64_t d) {
    LARGE_INTEGER _t0, _t1;
    QueryPerformanceCounter(&_t0);
    g_ifc17(a, b, c, d);
    QueryPerformanceCounter(&_t1);
    ifc_sample(17, _t0, _t1);
    g_gap_base_E = _t1;
    g_gap_base_E_valid = TRUE;
}
static void ifc_hook18(int64_t a,int64_t b,int64_t c,int64_t d){IFC_WRAP(g_ifc18(a,b,c,d),18);}
static void ifc_hook19(int64_t a,int64_t b,int64_t c,int64_t d){
    LARGE_INTEGER _t0, _t1;
    QueryPerformanceCounter(&_t0);
    g_ifc19(a,b,c,d);
    QueryPerformanceCounter(&_t1);
    ifc_sample(19, _t0, _t1);
    g_gap_base_D = _t1;
    g_gap_base_D_valid = TRUE;
}
static void    ifc_hook20(int64_t a,int64_t b,int64_t c,int64_t d){IFC_WRAP(g_ifc20(a,b,c,d),20);}
static int64_t ifc_hook21(void) { int64_t r; IFC_WRAP_R(r, g_ifc21(), 21); return r; }
static void    ifc_hook22(int64_t a,int64_t b,int64_t c,int64_t d){IFC_WRAP(g_ifc22(a,b,c,d),22);}
static int64_t ifc_hook23(void) { int64_t r; IFC_WRAP_R(r, g_ifc23(), 23); return r; }
static void    ifc_hook24(int64_t a,int64_t b,int64_t c,int64_t d){
    LARGE_INTEGER _t0, _t1;
    QueryPerformanceCounter(&_t0);
    g_ifc24(a,b,c,d);
    QueryPerformanceCounter(&_t1);
    ifc_sample(24, _t0, _t1);
    g_gap_base_A = _t1;
    g_gap_base_A_valid = TRUE;
}
static void    ifc_hook25(int64_t a,int64_t b,int64_t c,int64_t d){IFC_WRAP(g_ifc25(a,b,c,d),25);}
static void    ifc_hook26(int64_t a,int64_t b,int64_t c,int64_t d){IFC_WRAP(g_ifc26(a,b,c,d),26);}

static BYTE *g_inter_stub_block = NULL;

static BOOL install_inter_hooks(void)
{
    HMODULE exe = GetModuleHandleA(NULL);
    if (!exe) return FALSE;

    BYTE *winmain = (BYTE *)exe + WINMAIN_RVA;

    g_ifc0  = (VV_t)  ((BYTE*)exe + g_ifc_rva[0]);
    g_ifc1  = (VV_t)  ((BYTE*)exe + g_ifc_rva[1]);
    g_ifc2  = (VV_t)  ((BYTE*)exe + g_ifc_rva[2]);
    g_ifc3  = (VP_t)  ((BYTE*)exe + g_ifc_rva[3]);
    g_ifc4  = (VPU_t) ((BYTE*)exe + g_ifc_rva[4]);
    g_ifc5  = (VPU_t) ((BYTE*)exe + g_ifc_rva[5]);
    g_ifc6  = (VP_t)  ((BYTE*)exe + g_ifc_rva[6]);
    g_ifc7  = (VV_t)  ((BYTE*)exe + g_ifc_rva[7]);
    g_ifc8  = (WIDE_t)((BYTE*)exe + g_ifc_rva[8]);
    g_ifc9  = (WIDE_t)((BYTE*)exe + g_ifc_rva[9]);
    g_ifc10 = (WIDE_t)((BYTE*)exe + g_ifc_rva[10]);
    g_ifc11 = (WIDE_t)((BYTE*)exe + g_ifc_rva[11]);
    g_ifc12 = (WIDE_t) ((BYTE*)exe + g_ifc_rva[12]);
    g_ifc13 = (WIDE_t) ((BYTE*)exe + g_ifc_rva[13]);
    g_ifc14 = (WIDE_t) ((BYTE*)exe + g_ifc_rva[14]);
    g_ifc15 = (WIDES_t)((BYTE*)exe + g_ifc_rva[15]);
    g_ifc16 = (WIDE_t) ((BYTE*)exe + g_ifc_rva[16]);
    g_ifc17 = (WIDE_t) ((BYTE*)exe + g_ifc_rva[17]);
    g_ifc18 = (WIDE_t) ((BYTE*)exe + g_ifc_rva[18]);
    g_ifc19 = (WIDE_t) ((BYTE*)exe + g_ifc_rva[19]);
    g_ifc20 = (WIDE_t) ((BYTE*)exe + g_ifc_rva[20]);
    g_ifc21 = (WIDES_t)((BYTE*)exe + g_ifc_rva[21]);
    g_ifc22 = (WIDE_t) ((BYTE*)exe + g_ifc_rva[22]);
    g_ifc23 = (WIDES_t)((BYTE*)exe + g_ifc_rva[23]);
    g_ifc24 = (WIDE_t) ((BYTE*)exe + g_ifc_rva[24]);
    g_ifc25 = (WIDE_t) ((BYTE*)exe + g_ifc_rva[25]);
    g_ifc26 = (WIDE_t) ((BYTE*)exe + g_ifc_rva[26]);

    g_inter_stub_block = alloc_near(winmain, N_IFC * 14);
    if (!g_inter_stub_block) {
        log_write("[eaw-mt] WARN: alloc_near failed for inter stubs\n");
        return FALSE;
    }

    void *hook_fns[N_IFC] = {
        ifc_hook0,  ifc_hook1,  ifc_hook2,  ifc_hook3,
        ifc_hook4,  ifc_hook5,  ifc_hook6,  ifc_hook7,
        ifc_hook8,  ifc_hook9,  ifc_hook10, ifc_hook11,
        ifc_hook12, ifc_hook13, ifc_hook14, ifc_hook15,
        ifc_hook16, ifc_hook17, ifc_hook18, ifc_hook19,
        ifc_hook20, ifc_hook21, ifc_hook22, ifc_hook23,
        ifc_hook24, ifc_hook25,
        ifc_hook26
    };

    int n = 0;
    for (int i = 0; i < N_IFC; i++) {
        BYTE *target = (BYTE*)exe + g_ifc_rva[i];
        BYTE *site   = NULL;

        /* Index 0 is a thunk: WinMain calls a stub wrapper, not 0x24bb80 directly.
           Skip the forward scan; handle via backtrack from site[1] after the loop. */
        if (i == 0) continue;

        /* Index 26 (27d800): return value unknown — skip call-site patch until
           decompiled. GapE (hook17→hook8) already brackets this region. */
        if (i == 26) continue;

        /* Hooks 8-13 cover WinMain lines 597-648, which come AFTER hook 7 (caa60,
           line 591). Scanning from byte 0 would find earlier call sites for the
           same functions in WinMain's init section, hooking wrong sites with wrong
           argument counts.  Start after site[7] once it is known. */
        int start_j = 0;
        if (i >= 8 && i <= 13 && g_ifc_site[7])
            start_j = (int)(g_ifc_site[7] - winmain) + 5;
        else if (i == 14 && g_ifc_site[8])
            start_j = (int)(g_ifc_site[8] - winmain) + 5;
        else if (i >= 15 && i <= 19 && g_ifc_site[13])
            start_j = (int)(g_ifc_site[13] - winmain) + 5;
        else if (i >= 20 && i <= 24 && g_ifc_site[14])
            start_j = (int)(g_ifc_site[14] - winmain) + 5;
        else if (i == 25 && g_ifc_site[11])
            start_j = (int)(g_ifc_site[11] - winmain) + 5;
        else if (i == 26 && g_ifc_site[17])
            start_j = (int)(g_ifc_site[17] - winmain) + 5;

        for (int j = start_j; j <= WINMAIN_SIZE - 5; j++) {
            if (winmain[j] == 0xE8) {
                int32_t rel;
                memcpy(&rel, winmain + j + 1, 4);
                if (winmain + j + 5 + rel == target) { site = winmain + j; break; }
            }
        }

        if (!site) {
            char m[96];
            sprintf(m, "[eaw-mt] WARN: inter hook [%s] not found in WinMain\n",
                    g_ifc_name[i]);
            log_write(m);
            continue;
        }

        BYTE    *stub    = g_inter_stub_block + i * 14;
        int32_t  new_rel = (int32_t)(stub - (site + 5));
        DWORD    old;

        write_abs_jmp(stub, (uint64_t)hook_fns[i]);

        VirtualProtect(site + 1, 4, PAGE_EXECUTE_READWRITE, &old);
        memcpy(site + 1, &new_rel, 4);
        VirtualProtect(site + 1, 4, old, &old);
        FlushInstructionCache(GetCurrentProcess(), site, 5);

        g_ifc_site[i] = site;
        n++;
    }

    /* Index 0 backtrack: thunk_FUN_14024bb80 is called immediately before 2505c0 (line 580→581).
       The thunk call site is the 5-byte CALL just before g_ifc_site[1]. */
    if (!g_ifc_site[0] && g_ifc_site[1]) {
        BYTE *candidate = g_ifc_site[1] - 5;
        if (candidate >= winmain && candidate[0] == 0xE8) {
            int32_t rel;
            memcpy(&rel, candidate + 1, 4);
            g_ifc0 = (VV_t)(candidate + 5 + rel); /* thunk target (JMPs to real fn) */

            BYTE    *stub    = g_inter_stub_block + 0 * 14;
            int32_t  new_rel = (int32_t)(stub - (candidate + 5));
            DWORD    old;
            write_abs_jmp(stub, (uint64_t)ifc_hook0);
            VirtualProtect(candidate + 1, 4, PAGE_EXECUTE_READWRITE, &old);
            memcpy(candidate + 1, &new_rel, 4);
            VirtualProtect(candidate + 1, 4, old, &old);
            FlushInstructionCache(GetCurrentProcess(), candidate, 5);
            g_ifc_site[0] = candidate;
            n++;
            log_write("[eaw-mt] inter hook [24bb80] patched via thunk backtrack\n");
        } else {
            log_write("[eaw-mt] WARN: inter hook [24bb80] thunk backtrack failed\n");
        }
    }

    char m[64];
    sprintf(m, "[eaw-mt] inter-flush hooks: %d/%d installed\n", n, N_IFC);
    log_write(m);
    return n > 0;
}

/* =========================================================================
 * Game-mode service hook (Phase 5 Step 6)
 *
 * FUN_14028d400 (RVA 0x28d400, 654 bytes) is called from WinMain ~line 865
 * once per frame.  It dispatches to vtable slot 22 of whatever game mode is
 * active (GalacticModeClass, SpaceModeClass, LandModeClass, …) and also
 * calls GameScoringManager::Service (FUN_140488660).  Hooking it here gives
 * a mode-agnostic measurement of the entire back-half service block.
 *
 * Confirmed via phase5_line865.c decompile:
 *   void FUN_14028d400(longlong param_1, undefined4 param_2)
 *   (**(code**)(**(longlong**)(param_1+0x38) + 0xb0))(…) // vtable[0xb0/8=22]
 * ========================================================================= */
#define GAME_SERVICE_RVA     0x28d400ULL  /* FUN_14028d400 — mode-agnostic slot-22 dispatch */
#define GALACTIC_SLOT22_RVA  0x45e030ULL  /* FUN_14045e030 — GalacticModeClass vtable slot 22 */
#define GA4D0_RVA            0x28a4d0ULL  /* FUN_14028a4d0 — periodic sub-call inside FUN_14028d400 */
#define GSVC_BODY_SIZE       654          /* FUN_14028d400 size for E8 scan */

typedef void (*GameServiceFn)(int64_t, int32_t);
static GameServiceFn g_gsvc_orig     = NULL;
static LONG          g_gsvc_count    = 0;
static double        g_gsvc_sum_ms   = 0, g_gsvc_max_ms = 0;

/* =========================================================================
 * GalacticModeClass slot-22 inline hook (Phase 5 Step 6b)
 *
 * FUN_14045e030 (RVA 0x45e030, 507 bytes) is GalacticModeClass vtable slot 22,
 * called from FUN_14028d400 via an indirect vtable dispatch.  The object that
 * carries this vtable is NOT DAT_140b15418 (SpaceModeClass, the tactical layer)
 * but a separate galactic-layer object at *(param_1+0x38) in FUN_14028d400.
 *
 * Since we cannot scan for a direct E8 call, we inline-hook FUN_14045e030
 * using the same FF25 trampoline pattern as the render-flush hook.
 *
 * Prologue bytes verified via Phase5DumpGalacticBytes.java (position-independent):
 *   48 89 5c 24 10  MOV [RSP+0x10], RBX
 *   48 89 6c 24 18  MOV [RSP+0x18], RBP
 *   56              PUSH RSI
 *   57              PUSH RDI
 *   41 57           PUSH R15
 * ========================================================================= */

static const BYTE galactic_slot22_prologue[14] = {
    0x48, 0x89, 0x5c, 0x24, 0x10,  /* MOV [RSP+0x10], RBX */
    0x48, 0x89, 0x6c, 0x24, 0x18,  /* MOV [RSP+0x18], RBP */
    0x56,                           /* PUSH RSI             */
    0x57,                           /* PUSH RDI             */
    0x41, 0x57,                     /* PUSH R15             */
};

typedef void (*GalacticSlot22Fn)(int64_t, int64_t, int64_t, int64_t);
static GalacticSlot22Fn g_gslot22_trampoline = NULL;
static LONG   g_gslot22_count  = 0;
static double g_gslot22_sum_ms = 0, g_gslot22_max_ms = 0;

static void galactic_slot22_hook(int64_t a, int64_t b, int64_t c, int64_t d)
{
    LARGE_INTEGER t0, t1;
    QueryPerformanceCounter(&t0);
    g_gslot22_trampoline(a, b, c, d);
    QueryPerformanceCounter(&t1);
    double ms = (t1.QuadPart - t0.QuadPart) / ((double)g_qpc_freq.QuadPart / 1000.0);
    g_gslot22_sum_ms += ms;
    if (ms > g_gslot22_max_ms) g_gslot22_max_ms = ms;
    g_gslot22_count++;
    if (ms >= 100.0) {
        char s[64];
        sprintf(s, "[eaw-mt] GSLOT22 %.1fms\n", ms);
        log_write(s);
    }
}

static BOOL install_galactic_slot22_hook(void)
{
    HMODULE exe = GetModuleHandleA(NULL);
    if (!exe) return FALSE;

    BYTE *gal_fn = (BYTE *)exe + GALACTIC_SLOT22_RVA;

    if (memcmp(gal_fn, galactic_slot22_prologue, 14) != 0) {
        log_write("[eaw-mt] WARN: FUN_14045e030 prologue mismatch — wrong binary?\n");
        return FALSE;
    }

    BYTE *tramp = (BYTE *)VirtualAlloc(NULL, 64, MEM_COMMIT|MEM_RESERVE,
                                       PAGE_EXECUTE_READWRITE);
    if (!tramp) {
        log_write("[eaw-mt] WARN: VirtualAlloc for galactic slot22 trampoline failed\n");
        return FALSE;
    }
    memcpy(tramp, galactic_slot22_prologue, 14);
    write_abs_jmp(tramp + 14, (uint64_t)(gal_fn + 14));

    g_gslot22_trampoline = (GalacticSlot22Fn)tramp;

    DWORD old;
    VirtualProtect(gal_fn, 14, PAGE_EXECUTE_READWRITE, &old);
    write_abs_jmp(gal_fn, (uint64_t)galactic_slot22_hook);
    VirtualProtect(gal_fn, 14, old, &old);
    FlushInstructionCache(GetCurrentProcess(), gal_fn, 14);

    log_write("[eaw-mt] GalacticModeClass slot22 (45e030) inline hook installed\n");
    return TRUE;
}

/* =========================================================================
 * SpaceModeClass slot-22 inline hook (Phase 5 Step 7b)
 *
 * FUN_1404d95a0 (RVA 0x4d95a0, 358 bytes) is SpaceModeClass vtable slot 22.
 * Confirmed as stall carrier: vtbl22_rva=0x4d95a0 logged on every GSVC stall.
 * The function has a linked-list loop calling FUN_140527dd0 (RVA 0x527dd0)
 * once per space unit — that per-unit call is the prime suspect.
 *
 * Prologue note: SUB RSP,0x20 starts at byte 11 and ends at byte 14.  A
 * 14-byte FF25 trampoline cuts mid-instruction, so we save 15 bytes and
 * write FF25 (14 bytes) leaving byte 14 (0x20) unreachable-but-intact.
 * The trampoline copies all 15 original bytes and JMPs to fn+15. ✓
 *
 * FUN_140527dd0 (528 bytes): direct E8 call from inside FUN_1404d95a0.
 * Same 15-byte prologue structure.  Hooked via call-site E8 scan.
 * ========================================================================= */
#define SPACE_SLOT22_INLINE_RVA  0x4d95a0ULL  /* FUN_1404d95a0 */
#define SPACE_UNIT_SVC_RVA       0x527dd0ULL  /* FUN_140527dd0 — per-unit loop body */
#define SPACE_SLOT22_BODY_SIZE   358

static const BYTE space_slot22_prologue[15] = {
    0x48, 0x89, 0x5c, 0x24, 0x08,  /* MOV [RSP+0x08], RBX */
    0x48, 0x89, 0x74, 0x24, 0x10,  /* MOV [RSP+0x10], RSI */
    0x57,                           /* PUSH RDI             */
    0x48, 0x83, 0xec, 0x20,         /* SUB RSP, 0x20        */
};

typedef void (*SpaceSlot22Fn)(int64_t, int32_t);
static SpaceSlot22Fn g_sslot22_trampoline = NULL;
static LONG    g_sslot22_count  = 0;
static double  g_sslot22_sum_ms = 0, g_sslot22_max_ms = 0;

static void space_slot22_hook(int64_t a, int32_t b)
{
    LARGE_INTEGER t0, t1;
    QueryPerformanceCounter(&t0);
    g_sslot22_trampoline(a, b);
    QueryPerformanceCounter(&t1);
    double ms = (t1.QuadPart - t0.QuadPart) / ((double)g_qpc_freq.QuadPart / 1000.0);
    g_sslot22_sum_ms += ms;
    if (ms > g_sslot22_max_ms) g_sslot22_max_ms = ms;
    g_sslot22_count++;
    if (ms >= 100.0) {
        char s[64];
        sprintf(s, "[eaw-mt] SSLOT22 %.1fms\n", ms);
        log_write(s);
    }
}

static BOOL install_space_slot22_hook(void)
{
    HMODULE exe = GetModuleHandleA(NULL);
    if (!exe) return FALSE;

    BYTE *fn = (BYTE *)exe + SPACE_SLOT22_INLINE_RVA;

    if (memcmp(fn, space_slot22_prologue, 15) != 0) {
        log_write("[eaw-mt] WARN: FUN_1404d95a0 prologue mismatch\n");
        return FALSE;
    }

    /* 15-byte save: 4 complete instructions; JMP back to fn+15. */
    BYTE *tramp = (BYTE *)VirtualAlloc(NULL, 64, MEM_COMMIT|MEM_RESERVE,
                                       PAGE_EXECUTE_READWRITE);
    if (!tramp) {
        log_write("[eaw-mt] WARN: VirtualAlloc for space slot22 trampoline failed\n");
        return FALSE;
    }
    memcpy(tramp, space_slot22_prologue, 15);
    write_abs_jmp(tramp + 15, (uint64_t)(fn + 15));

    g_sslot22_trampoline = (SpaceSlot22Fn)tramp;

    DWORD old;
    /* Protect 15 bytes; write_abs_jmp writes 14 — byte 14 (0x20) stays intact. */
    VirtualProtect(fn, 15, PAGE_EXECUTE_READWRITE, &old);
    write_abs_jmp(fn, (uint64_t)space_slot22_hook);
    VirtualProtect(fn, 15, old, &old);
    FlushInstructionCache(GetCurrentProcess(), fn, 15);

    log_write("[eaw-mt] SpaceModeClass slot22 (4d95a0) inline hook installed\n");
    return TRUE;
}

/* Per-unit service call inside FUN_1404d95a0 loop -------------------------*/
typedef void (*SpaceUnitSvcFn)(int64_t, int32_t);
static SpaceUnitSvcFn g_sunit22_orig  = NULL;
static LONG    g_sunit22_count  = 0;
static double  g_sunit22_sum_ms = 0, g_sunit22_max_ms = 0;

static void space_unit_svc_hook(int64_t a, int32_t b)
{
    LARGE_INTEGER t0, t1;
    QueryPerformanceCounter(&t0);
    g_sunit22_orig(a, b);
    QueryPerformanceCounter(&t1);
    double ms = (t1.QuadPart - t0.QuadPart) / ((double)g_qpc_freq.QuadPart / 1000.0);
    g_sunit22_sum_ms += ms;
    if (ms > g_sunit22_max_ms) g_sunit22_max_ms = ms;
    g_sunit22_count++;
}

static BOOL install_space_unit_svc_hook(void)
{
    HMODULE exe = GetModuleHandleA(NULL);
    if (!exe) return FALSE;

    BYTE *fn       = (BYTE *)exe + SPACE_SLOT22_INLINE_RVA;
    BYTE *svc_fn   = (BYTE *)exe + SPACE_UNIT_SVC_RVA;

    g_sunit22_orig = (SpaceUnitSvcFn)svc_fn;

    BYTE *call_site = NULL;
    for (int i = 0; i <= SPACE_SLOT22_BODY_SIZE - 5; i++) {
        if (fn[i] == 0xE8) {
            int32_t rel;
            memcpy(&rel, fn + i + 1, 4);
            if (fn + i + 5 + rel == svc_fn) {
                call_site = fn + i;
                break;
            }
        }
    }

    if (!call_site) {
        log_write("[eaw-mt] WARN: FUN_140527dd0 CALL not found in FUN_1404d95a0\n");
        return FALSE;
    }

    BYTE *stub = alloc_near(call_site, 14);
    if (!stub) {
        log_write("[eaw-mt] WARN: alloc_near failed for sunit22 stub\n");
        return FALSE;
    }
    write_abs_jmp(stub, (uint64_t)space_unit_svc_hook);

    int32_t new_rel = (int32_t)(stub - (call_site + 5));
    DWORD old;
    VirtualProtect(call_site + 1, 4, PAGE_EXECUTE_READWRITE, &old);
    memcpy(call_site + 1, &new_rel, 4);
    VirtualProtect(call_site + 1, 4, old, &old);
    FlushInstructionCache(GetCurrentProcess(), call_site, 5);

    log_write("[eaw-mt] space_unit_svc (527dd0) call-site hook in FUN_1404d95a0 installed\n");
    return TRUE;
}

/* =========================================================================
 * FUN_1403639d0 inline hook (Phase 5 Step 8a)
 *
 * The tail call from FUN_1404d95a0 is compiled as JMP (not CALL), so the
 * original E8 call-site scan missed it.  Hook FUN_1403639d0 at its entry
 * point directly to measure how much of the sslot22 time is here vs. in
 * FUN_1404d95a0's pre-tail body.
 *
 * Prologue: 19 bytes (7 instructions, all position-independent):
 *   40 55       PUSH RBP (with REX prefix)
 *   53          PUSH RBX
 *   56          PUSH RSI
 *   57          PUSH RDI
 *   41 54       PUSH R12
 *   48 8d 6c 24 c9    LEA RBP, [RSP-0x37]
 *   48 81 ec 00 01 00 00  SUB RSP, 0x100
 * write_abs_jmp writes 14 bytes; bytes 14-18 remain untouched (unreachable).
 * ========================================================================= */
#define TAIL22_RVA        0x3639d0ULL  /* FUN_1403639d0 — 3916 bytes */
#define A1A240_RVA        0x41a240ULL  /* FUN_14041a240 — unconditional post-loop */
#define S9D270_RVA        0x59d270ULL  /* FUN_14059d270 — unconditional post-loop */
#define T364920_RVA       0x364920ULL  /* FUN_140364920 — first unconditional call in tail22 body */
#define T2BE640_RVA       0x2be640ULL  /* FUN_1402be640 — called twice in tail22 body */
#define T490580_RVA       0x490580ULL  /* FUN_140490580 — unconditional call in tail22 body */
#define TAIL22_BODY_SIZE  3916
#define B2BE640_BODY_SIZE 1577         /* FUN_1402be640 body size for inner E8 scan */
#define TA6B80_RVA        0x3a6b80ULL  /* FUN_1403a6b80 — per-entity inner loop call */
#define TA62D0_RVA        0x2a62d0ULL  /* FUN_1402a62d0 — unconditional final call */
#define T20ED70_RVA       0x20ed70ULL  /* FUN_14020ed70 — preamble loop call */
#define A6B80_BODY_SIZE   2862         /* FUN_1403a6b80 body size for inner E8 scan */
#define PUMPE_RVA         0x247a90ULL  /* FUN_140247a90 — Pump_Threads (entity-level call site) */
#define A76B0_RVA         0x3a76b0ULL  /* FUN_1403a76b0 — movement system (line 361 of 3a6b80) */
#define A9E30_RVA         0x3a9e30ULL  /* FUN_1403a9e30 — command dispatch (line 99 of 3a6b80) */
#define E369E0_RVA        0x5369e0ULL  /* FUN_1405369e0 — preamble call (line 79 of 3a6b80) */
#define A76B0_BODY_SIZE   499          /* FUN_1403a76b0 body size for inner E8 scan */
#define B87010_RVA        0x387010ULL  /* FUN_140387010 — per-movement-component call (line 74 of 3a76b0) */
#define B87010_BODY_SIZE  344          /* FUN_140387010 body size for inner E8 scan */
#define B385CF0_RVA       0x385cf0ULL  /* FUN_140385cf0 — path-system name lookup (cold-cache + 381ff0) */
#define B385CF0_BODY_SIZE 376          /* body size for inner E8 scan */
#define B381FF0_RVA       0x381ff0ULL  /* FUN_140381ff0 — main movement step */
#define B381FF0_BODY_SIZE 746          /* body size for inner E8 scan */
#define B387400_RVA       0x387400ULL  /* FUN_140387400 — path-following update (+0x074 inside 387010) */
#define B387170_RVA       0x387170ULL  /* FUN_140387170 — rotation update (+0x143 inside 387010) */
#define D12D520_RVA       0x12d520ULL  /* FUN_14012d520 — name→index lookup in nav manager (inside 385cf0) */
#define D12D480_RVA       0x12d480ULL  /* FUN_14012d480 — write position/rotation to path system (inside 381ff0) */

static const BYTE tail22_prologue[19] = {
    0x40, 0x55,                              /* PUSH RBP (REX prefix)        */
    0x53,                                    /* PUSH RBX                     */
    0x56,                                    /* PUSH RSI                     */
    0x57,                                    /* PUSH RDI                     */
    0x41, 0x54,                              /* PUSH R12                     */
    0x48, 0x8d, 0x6c, 0x24, 0xc9,           /* LEA RBP, [RSP-0x37]          */
    0x48, 0x81, 0xec, 0x00, 0x01, 0x00, 0x00 /* SUB RSP, 0x100               */
};

typedef void (*Tail22InlineFn)(int64_t, int32_t);
static Tail22InlineFn g_tail22i_trampoline = NULL;
static LONG    g_tail22i_count  = 0;
static double  g_tail22i_sum_ms = 0, g_tail22i_max_ms = 0;

static void tail22i_hook(int64_t a, int32_t b)
{
    LARGE_INTEGER t0, t1;
    QueryPerformanceCounter(&t0);
    g_tail22i_trampoline(a, b);
    QueryPerformanceCounter(&t1);
    double ms = (t1.QuadPart - t0.QuadPart) / ((double)g_qpc_freq.QuadPart / 1000.0);
    g_tail22i_sum_ms += ms;
    if (ms > g_tail22i_max_ms) g_tail22i_max_ms = ms;
    g_tail22i_count++;
}

static BOOL install_tail22i_hook(void)
{
    HMODULE exe = GetModuleHandleA(NULL);
    if (!exe) return FALSE;

    BYTE *fn = (BYTE *)exe + TAIL22_RVA;

    if (memcmp(fn, tail22_prologue, 19) != 0) {
        log_write("[eaw-mt] WARN: FUN_1403639d0 prologue mismatch\n");
        return FALSE;
    }

    BYTE *tramp = (BYTE *)VirtualAlloc(NULL, 64, MEM_COMMIT|MEM_RESERVE,
                                       PAGE_EXECUTE_READWRITE);
    if (!tramp) {
        log_write("[eaw-mt] WARN: VirtualAlloc for tail22i trampoline failed\n");
        return FALSE;
    }
    /* Save 19 bytes; write_abs_jmp writes only 14 — bytes 14-18 stay in fn. */
    memcpy(tramp, tail22_prologue, 19);
    write_abs_jmp(tramp + 19, (uint64_t)(fn + 19));

    g_tail22i_trampoline = (Tail22InlineFn)tramp;

    DWORD old;
    VirtualProtect(fn, 19, PAGE_EXECUTE_READWRITE, &old);
    write_abs_jmp(fn, (uint64_t)tail22i_hook);
    VirtualProtect(fn, 19, old, &old);
    FlushInstructionCache(GetCurrentProcess(), fn, 19);

    log_write("[eaw-mt] FUN_1403639d0 (3639d0) inline hook installed\n");
    return TRUE;
}

/* E8 call-site hooks for FUN_14041a240 and FUN_14059d270 inside FUN_1404d95a0.
 * Both are called unconditionally after the per-unit loop.             --------*/
typedef void* (*A1a240Fn)(void);
static A1a240Fn g_a1a240_orig   = NULL;
static LONG     g_a1a240_count  = 0;
static double   g_a1a240_sum_ms = 0, g_a1a240_max_ms = 0;

static void* a1a240_hook(void)
{
    LARGE_INTEGER t0, t1;
    QueryPerformanceCounter(&t0);
    void *ret = g_a1a240_orig();
    QueryPerformanceCounter(&t1);
    double ms = (t1.QuadPart - t0.QuadPart) / ((double)g_qpc_freq.QuadPart / 1000.0);
    g_a1a240_sum_ms += ms;
    if (ms > g_a1a240_max_ms) g_a1a240_max_ms = ms;
    g_a1a240_count++;
    return ret;
}

typedef void (*S9d270Fn)(int64_t);
static S9d270Fn g_s9d270_orig   = NULL;
static LONG     g_s9d270_count  = 0;
static double   g_s9d270_sum_ms = 0, g_s9d270_max_ms = 0;

static void s9d270_hook(int64_t a)
{
    LARGE_INTEGER t0, t1;
    QueryPerformanceCounter(&t0);
    g_s9d270_orig(a);
    QueryPerformanceCounter(&t1);
    double ms = (t1.QuadPart - t0.QuadPart) / ((double)g_qpc_freq.QuadPart / 1000.0);
    g_s9d270_sum_ms += ms;
    if (ms > g_s9d270_max_ms) g_s9d270_max_ms = ms;
    g_s9d270_count++;
}

static BOOL install_post_loop_hooks(void)
{
    HMODULE exe = GetModuleHandleA(NULL);
    if (!exe) return FALSE;

    BYTE *space_fn  = (BYTE *)exe + SPACE_SLOT22_INLINE_RVA;
    BYTE *a1a240_fn = (BYTE *)exe + A1A240_RVA;
    BYTE *s9d270_fn = (BYTE *)exe + S9D270_RVA;

    g_a1a240_orig = (A1a240Fn)a1a240_fn;
    g_s9d270_orig = (S9d270Fn)s9d270_fn;

    /* Allocate two stubs near FUN_1404d95a0. */
    BYTE *stub_block = alloc_near(space_fn, 14 * 2);
    if (!stub_block) {
        log_write("[eaw-mt] WARN: alloc_near failed for post-loop stubs\n");
        return FALSE;
    }
    BYTE *a1a240_stub = stub_block + 0 * 14;
    BYTE *s9d270_stub = stub_block + 1 * 14;

    write_abs_jmp(a1a240_stub, (uint64_t)a1a240_hook);
    write_abs_jmp(s9d270_stub, (uint64_t)s9d270_hook);

    int n = 0;
    for (int i = 0; i <= SPACE_SLOT22_BODY_SIZE - 5; i++) {
        if (space_fn[i] == 0xE8) {
            int32_t rel;
            memcpy(&rel, space_fn + i + 1, 4);
            BYTE *target = space_fn + i + 5 + rel;
            BYTE *stub = NULL;
            if (target == a1a240_fn) stub = a1a240_stub;
            else if (target == s9d270_fn) stub = s9d270_stub;

            if (stub) {
                int32_t new_rel = (int32_t)(stub - (space_fn + i + 5));
                DWORD old;
                VirtualProtect(space_fn + i + 1, 4, PAGE_EXECUTE_READWRITE, &old);
                memcpy(space_fn + i + 1, &new_rel, 4);
                VirtualProtect(space_fn + i + 1, 4, old, &old);
                FlushInstructionCache(GetCurrentProcess(), space_fn + i, 5);
                n++;
            }
        }
    }

    char m[80];
    sprintf(m, "[eaw-mt] post-loop hooks: %d/2 installed (41a240, 59d270)\n", n);
    log_write(m);
    return n > 0;
}

/* =========================================================================
 * E8 call-site hooks inside FUN_1403639d0 body (Phase 5 Step 8)
 *
 * tail22i confirmed: entire 1099ms avg stall lives in FUN_1403639d0.
 * Hook three prime suspects:
 *   FUN_140364920 (0x364920) — unconditional first call before any early return
 *   FUN_1402be640 (0x2be640) — two calls (linked-list/per-entity service)
 *   FUN_140490580 (0x490580) — unconditional call at line 148
 * ========================================================================= */
typedef void (*T364920Fn)(int64_t, int64_t);
static T364920Fn g_t364920_orig   = NULL;
static LONG      g_t364920_count  = 0;
static double    g_t364920_sum_ms = 0, g_t364920_max_ms = 0;

static void t364920_hook(int64_t a, int64_t b)
{
    LARGE_INTEGER t0, t1;
    QueryPerformanceCounter(&t0);
    g_t364920_orig(a, b);
    QueryPerformanceCounter(&t1);
    double ms = (t1.QuadPart - t0.QuadPart) / ((double)g_qpc_freq.QuadPart / 1000.0);
    g_t364920_sum_ms += ms;
    if (ms > g_t364920_max_ms) g_t364920_max_ms = ms;
    g_t364920_count++;
}

typedef void (*T2be640Fn)(int64_t, int64_t);
static T2be640Fn g_t2be640_orig   = NULL;
static LONG      g_t2be640_count  = 0;
static double    g_t2be640_sum_ms = 0, g_t2be640_max_ms = 0;

static void t2be640_hook(int64_t a, int64_t b)
{
    LARGE_INTEGER t0, t1;
    QueryPerformanceCounter(&t0);
    g_t2be640_orig(a, b);
    QueryPerformanceCounter(&t1);
    double ms = (t1.QuadPart - t0.QuadPart) / ((double)g_qpc_freq.QuadPart / 1000.0);
    g_t2be640_sum_ms += ms;
    if (ms > g_t2be640_max_ms) g_t2be640_max_ms = ms;
    g_t2be640_count++;
}

typedef void (*T490580Fn)(int64_t);
static T490580Fn g_t490580_orig   = NULL;
static LONG      g_t490580_count  = 0;
static double    g_t490580_sum_ms = 0, g_t490580_max_ms = 0;

static void t490580_hook(int64_t a)
{
    LARGE_INTEGER t0, t1;
    QueryPerformanceCounter(&t0);
    g_t490580_orig(a);
    QueryPerformanceCounter(&t1);
    double ms = (t1.QuadPart - t0.QuadPart) / ((double)g_qpc_freq.QuadPart / 1000.0);
    g_t490580_sum_ms += ms;
    if (ms > g_t490580_max_ms) g_t490580_max_ms = ms;
    g_t490580_count++;
}

static BOOL install_tail22_body_hooks(void)
{
    HMODULE exe = GetModuleHandleA(NULL);
    if (!exe) return FALSE;

    BYTE *tail_fn = (BYTE *)exe + TAIL22_RVA;
    BYTE *fn364920 = (BYTE *)exe + T364920_RVA;
    BYTE *fn2be640 = (BYTE *)exe + T2BE640_RVA;
    BYTE *fn490580 = (BYTE *)exe + T490580_RVA;

    g_t364920_orig = (T364920Fn)fn364920;
    g_t2be640_orig = (T2be640Fn)fn2be640;
    g_t490580_orig = (T490580Fn)fn490580;

    BYTE *stub_block = alloc_near(tail_fn, 14 * 3);
    if (!stub_block) {
        log_write("[eaw-mt] WARN: alloc_near failed for tail22 body stubs\n");
        return FALSE;
    }
    BYTE *stub_364920 = stub_block + 0 * 14;
    BYTE *stub_2be640 = stub_block + 1 * 14;
    BYTE *stub_490580 = stub_block + 2 * 14;

    write_abs_jmp(stub_364920, (uint64_t)t364920_hook);
    write_abs_jmp(stub_2be640, (uint64_t)t2be640_hook);
    write_abs_jmp(stub_490580, (uint64_t)t490580_hook);

    int n = 0;
    for (int i = 0; i <= TAIL22_BODY_SIZE - 5; i++) {
        if (tail_fn[i] == 0xE8) {
            int32_t rel;
            memcpy(&rel, tail_fn + i + 1, 4);
            BYTE *target = tail_fn + i + 5 + rel;
            BYTE *stub = NULL;
            if (target == fn364920) stub = stub_364920;
            else if (target == fn2be640) stub = stub_2be640;
            else if (target == fn490580) stub = stub_490580;

            if (stub) {
                int32_t new_rel = (int32_t)(stub - (tail_fn + i + 5));
                DWORD old;
                VirtualProtect(tail_fn + i + 1, 4, PAGE_EXECUTE_READWRITE, &old);
                memcpy(tail_fn + i + 1, &new_rel, 4);
                VirtualProtect(tail_fn + i + 1, 4, old, &old);
                FlushInstructionCache(GetCurrentProcess(), tail_fn + i, 5);
                n++;
            }
        }
    }

    char m[96];
    sprintf(m, "[eaw-mt] tail22 body hooks: %d/4 patched (364920x1, 2be640x2, 490580x1)\n", n);
    log_write(m);
    return n > 0;
}

/* =========================================================================
 * E8 call-site hooks inside FUN_1402be640 body (Phase 5 Step 9)
 *
 * FUN_1402be640 (1577 bytes, called ×2 per tail22i) is the confirmed stall.
 * Hook three prime suspects:
 *   FUN_1403a6b80 (0x3a6b80) — per-entity inner loop; n >> 52 if it's the culprit
 *   FUN_1402a62d0 (0x2a62d0) — unconditional final call (×2 per 2be640 = ×4 per tail22i)
 *   FUN_14020ed70 (0x20ed70) — preamble loop call (×DAT_140a16fb0 per 2be640 invocation)
 * ========================================================================= */
typedef void (*Ta6b80Fn)(int64_t, int32_t, int8_t);
static Ta6b80Fn g_ta6b80_orig   = NULL;
static LONG     g_ta6b80_count  = 0;
static double   g_ta6b80_sum_ms = 0, g_ta6b80_max_ms = 0;

static void ta6b80_hook(int64_t a, int32_t b, int8_t c)
{
    LARGE_INTEGER t0, t1;
    QueryPerformanceCounter(&t0);
    g_ta6b80_orig(a, b, c);
    QueryPerformanceCounter(&t1);
    double ms = (t1.QuadPart - t0.QuadPart) / ((double)g_qpc_freq.QuadPart / 1000.0);
    g_ta6b80_sum_ms += ms;
    if (ms > g_ta6b80_max_ms) g_ta6b80_max_ms = ms;
    g_ta6b80_count++;
}

typedef void (*Ta62d0Fn)(int64_t);
static Ta62d0Fn g_ta62d0_orig   = NULL;
static LONG     g_ta62d0_count  = 0;
static double   g_ta62d0_sum_ms = 0, g_ta62d0_max_ms = 0;

static void ta62d0_hook(int64_t a)
{
    LARGE_INTEGER t0, t1;
    QueryPerformanceCounter(&t0);
    g_ta62d0_orig(a);
    QueryPerformanceCounter(&t1);
    double ms = (t1.QuadPart - t0.QuadPart) / ((double)g_qpc_freq.QuadPart / 1000.0);
    g_ta62d0_sum_ms += ms;
    if (ms > g_ta62d0_max_ms) g_ta62d0_max_ms = ms;
    g_ta62d0_count++;
}

typedef void (*T20ed70Fn)(int64_t);
static T20ed70Fn g_t20ed70_orig   = NULL;
static LONG      g_t20ed70_count  = 0;
static double    g_t20ed70_sum_ms = 0, g_t20ed70_max_ms = 0;

static void t20ed70_hook(int64_t a)
{
    LARGE_INTEGER t0, t1;
    QueryPerformanceCounter(&t0);
    g_t20ed70_orig(a);
    QueryPerformanceCounter(&t1);
    double ms = (t1.QuadPart - t0.QuadPart) / ((double)g_qpc_freq.QuadPart / 1000.0);
    g_t20ed70_sum_ms += ms;
    if (ms > g_t20ed70_max_ms) g_t20ed70_max_ms = ms;
    g_t20ed70_count++;
}

static BOOL install_2be640_body_hooks(void)
{
    HMODULE exe = GetModuleHandleA(NULL);
    if (!exe) return FALSE;

    BYTE *fn2be640 = (BYTE *)exe + T2BE640_RVA;
    BYTE *fna6b80  = (BYTE *)exe + TA6B80_RVA;
    BYTE *fna62d0  = (BYTE *)exe + TA62D0_RVA;
    BYTE *fn20ed70 = (BYTE *)exe + T20ED70_RVA;

    g_ta6b80_orig  = (Ta6b80Fn)fna6b80;
    g_ta62d0_orig  = (Ta62d0Fn)fna62d0;
    g_t20ed70_orig = (T20ed70Fn)fn20ed70;

    BYTE *stub_block = alloc_near(fn2be640, 14 * 3);
    if (!stub_block) {
        log_write("[eaw-mt] WARN: alloc_near failed for 2be640 body stubs\n");
        return FALSE;
    }
    BYTE *stub_a6b80  = stub_block + 0 * 14;
    BYTE *stub_a62d0  = stub_block + 1 * 14;
    BYTE *stub_20ed70 = stub_block + 2 * 14;

    write_abs_jmp(stub_a6b80,  (uint64_t)ta6b80_hook);
    write_abs_jmp(stub_a62d0,  (uint64_t)ta62d0_hook);
    write_abs_jmp(stub_20ed70, (uint64_t)t20ed70_hook);

    int n = 0;
    for (int i = 0; i <= B2BE640_BODY_SIZE - 5; i++) {
        if (fn2be640[i] == 0xE8) {
            int32_t rel;
            memcpy(&rel, fn2be640 + i + 1, 4);
            BYTE *target = fn2be640 + i + 5 + rel;
            BYTE *stub = NULL;
            if (target == fna6b80)  stub = stub_a6b80;
            else if (target == fna62d0)  stub = stub_a62d0;
            else if (target == fn20ed70) stub = stub_20ed70;

            if (stub) {
                int32_t new_rel = (int32_t)(stub - (fn2be640 + i + 5));
                DWORD old;
                VirtualProtect(fn2be640 + i + 1, 4, PAGE_EXECUTE_READWRITE, &old);
                memcpy(fn2be640 + i + 1, &new_rel, 4);
                VirtualProtect(fn2be640 + i + 1, 4, old, &old);
                FlushInstructionCache(GetCurrentProcess(), fn2be640 + i, 5);
                n++;
            }
        }
    }

    char m[96];
    sprintf(m, "[eaw-mt] 2be640 body hooks: %d patched (3a6b80xN, 2a62d0x1, 20ed70xM)\n", n);
    log_write(m);
    return n > 0;
}

/* =========================================================================
 * Per-entity Pump_Threads hook inside FUN_1403a6b80 (Phase 5 Step 10)
 *
 * Root cause confirmed: FUN_1403a6b80 calls FUN_140247a90 (Pump_Threads)
 * via a ServiceRate/LastService gate to execute entity Lua AI coroutines.
 * One entity's coroutine takes 22,310ms.  This hook captures the per-entity
 * Pump_Threads call time separately from the WinMain pump_ms metric.
 * ========================================================================= */
typedef void (*PumpEFn)(int64_t);
static PumpEFn g_pumpe_orig   = NULL;
static LONG    g_pumpe_count  = 0;
static double  g_pumpe_sum_ms = 0, g_pumpe_max_ms = 0;

static void pumpe_hook(int64_t a)
{
    LARGE_INTEGER t0, t1;
    QueryPerformanceCounter(&t0);
    g_pumpe_orig(a);
    QueryPerformanceCounter(&t1);
    double ms = (t1.QuadPart - t0.QuadPart) / ((double)g_qpc_freq.QuadPart / 1000.0);
    g_pumpe_sum_ms += ms;
    if (ms > g_pumpe_max_ms) g_pumpe_max_ms = ms;
    g_pumpe_count++;
    if (ms >= 100.0) {
        char s[64];
        sprintf(s, "[eaw-mt] PUMPE %.1fms (entity Lua AI)\n", ms);
        log_write(s);
    }
}

static BOOL install_pumpe_hook(void)
{
    HMODULE exe = GetModuleHandleA(NULL);
    if (!exe) return FALSE;

    BYTE *fn_a6b80  = (BYTE *)exe + TA6B80_RVA;
    BYTE *fn_pumpe  = (BYTE *)exe + PUMPE_RVA;

    g_pumpe_orig = (PumpEFn)fn_pumpe;

    BYTE *stub = alloc_near(fn_a6b80, 14);
    if (!stub) {
        log_write("[eaw-mt] WARN: alloc_near failed for pumpe stub\n");
        return FALSE;
    }
    write_abs_jmp(stub, (uint64_t)pumpe_hook);

    int n = 0;
    for (int i = 0; i <= A6B80_BODY_SIZE - 5; i++) {
        if (fn_a6b80[i] == 0xE8) {
            int32_t rel;
            memcpy(&rel, fn_a6b80 + i + 1, 4);
            if (fn_a6b80 + i + 5 + rel == fn_pumpe) {
                int32_t new_rel = (int32_t)(stub - (fn_a6b80 + i + 5));
                DWORD old;
                VirtualProtect(fn_a6b80 + i + 1, 4, PAGE_EXECUTE_READWRITE, &old);
                memcpy(fn_a6b80 + i + 1, &new_rel, 4);
                VirtualProtect(fn_a6b80 + i + 1, 4, old, &old);
                FlushInstructionCache(GetCurrentProcess(), fn_a6b80 + i, 5);
                n++;
            }
        }
    }

    char m[80];
    sprintf(m, "[eaw-mt] pumpe (Pump_Threads in 3a6b80): %d call site(s) patched\n", n);
    log_write(m);
    return n > 0;
}

/* =========================================================================
 * Second stall source hooks inside FUN_1403a6b80 (Phase 5 Step 11)
 *
 * Window 2 shows ta6b80 max=1050ms with pumpe=0.03ms — a second stall source
 * exists in FUN_1403a6b80 unrelated to Lua AI.  Hook three candidates:
 *   FUN_1403a76b0 (0x3a76b0) — movement system, line 361, always-called path
 *   FUN_1403a9e30 (0x3a9e30) — command dispatch, line 99, pending-command loop
 *   FUN_1405369e0 (0x5369e0) — preamble call, line 79, conditional
 * ========================================================================= */
typedef void (*A76b0Fn)(int64_t, int32_t);
static A76b0Fn g_a76b0_orig   = NULL;
static LONG    g_a76b0_count  = 0;
static double  g_a76b0_sum_ms = 0, g_a76b0_max_ms = 0;

static void a76b0_hook(int64_t a, int32_t b)
{
    LARGE_INTEGER t0, t1;
    QueryPerformanceCounter(&t0);
    g_a76b0_orig(a, b);
    QueryPerformanceCounter(&t1);
    double ms = (t1.QuadPart - t0.QuadPart) / ((double)g_qpc_freq.QuadPart / 1000.0);
    g_a76b0_sum_ms += ms;
    if (ms > g_a76b0_max_ms) g_a76b0_max_ms = ms;
    g_a76b0_count++;
}

typedef void (*A9e30Fn)(int64_t, int32_t, int64_t, int32_t, int64_t,
                         int32_t, int32_t, int32_t, int32_t,
                         int32_t, int32_t, int32_t, int32_t, int32_t);
static A9e30Fn g_a9e30_orig   = NULL;
static LONG    g_a9e30_count  = 0;
static double  g_a9e30_sum_ms = 0, g_a9e30_max_ms = 0;

static void a9e30_hook(int64_t a, int32_t b, int64_t c, int32_t d, int64_t e,
                        int32_t f, int32_t g, int32_t h, int32_t i2,
                        int32_t j, int32_t k2, int32_t l, int32_t m2, int32_t n2)
{
    LARGE_INTEGER t0, t1;
    QueryPerformanceCounter(&t0);
    g_a9e30_orig(a, b, c, d, e, f, g, h, i2, j, k2, l, m2, n2);
    QueryPerformanceCounter(&t1);
    double ms = (t1.QuadPart - t0.QuadPart) / ((double)g_qpc_freq.QuadPart / 1000.0);
    g_a9e30_sum_ms += ms;
    if (ms > g_a9e30_max_ms) g_a9e30_max_ms = ms;
    g_a9e30_count++;
}

typedef void (*E369e0Fn)(int64_t, int32_t);
static E369e0Fn g_e369e0_orig   = NULL;
static LONG     g_e369e0_count  = 0;
static double   g_e369e0_sum_ms = 0, g_e369e0_max_ms = 0;

static void e369e0_hook(int64_t a, int32_t b)
{
    LARGE_INTEGER t0, t1;
    QueryPerformanceCounter(&t0);
    g_e369e0_orig(a, b);
    QueryPerformanceCounter(&t1);
    double ms = (t1.QuadPart - t0.QuadPart) / ((double)g_qpc_freq.QuadPart / 1000.0);
    g_e369e0_sum_ms += ms;
    if (ms > g_e369e0_max_ms) g_e369e0_max_ms = ms;
    g_e369e0_count++;
}

static BOOL install_a6b80_stall2_hooks(void)
{
    HMODULE exe = GetModuleHandleA(NULL);
    if (!exe) return FALSE;

    BYTE *fn_a6b80  = (BYTE *)exe + TA6B80_RVA;
    BYTE *fn_a76b0  = (BYTE *)exe + A76B0_RVA;
    BYTE *fn_a9e30  = (BYTE *)exe + A9E30_RVA;
    BYTE *fn_e369e0 = (BYTE *)exe + E369E0_RVA;

    g_a76b0_orig  = (A76b0Fn)fn_a76b0;
    g_a9e30_orig  = (A9e30Fn)fn_a9e30;
    g_e369e0_orig = (E369e0Fn)fn_e369e0;

    BYTE *stub_block = alloc_near(fn_a6b80, 14 * 3);
    if (!stub_block) {
        log_write("[eaw-mt] WARN: alloc_near failed for a6b80 stall2 stubs\n");
        return FALSE;
    }
    BYTE *stub_a76b0  = stub_block + 0 * 14;
    BYTE *stub_a9e30  = stub_block + 1 * 14;
    BYTE *stub_e369e0 = stub_block + 2 * 14;

    write_abs_jmp(stub_a76b0,  (uint64_t)a76b0_hook);
    write_abs_jmp(stub_a9e30,  (uint64_t)a9e30_hook);
    write_abs_jmp(stub_e369e0, (uint64_t)e369e0_hook);

    int n = 0;
    for (int i = 0; i <= A6B80_BODY_SIZE - 5; i++) {
        if (fn_a6b80[i] == 0xE8) {
            int32_t rel;
            memcpy(&rel, fn_a6b80 + i + 1, 4);
            BYTE *target = fn_a6b80 + i + 5 + rel;
            BYTE *stub = NULL;
            if (target == fn_a76b0)  stub = stub_a76b0;
            else if (target == fn_a9e30)  stub = stub_a9e30;
            else if (target == fn_e369e0) stub = stub_e369e0;

            if (stub) {
                int32_t new_rel = (int32_t)(stub - (fn_a6b80 + i + 5));
                DWORD old;
                VirtualProtect(fn_a6b80 + i + 1, 4, PAGE_EXECUTE_READWRITE, &old);
                memcpy(fn_a6b80 + i + 1, &new_rel, 4);
                VirtualProtect(fn_a6b80 + i + 1, 4, old, &old);
                FlushInstructionCache(GetCurrentProcess(), fn_a6b80 + i, 5);
                n++;
            }
        }
    }

    char m[96];
    sprintf(m, "[eaw-mt] a6b80 stall2 hooks: %d/3 patched (a76b0, a9e30, 5369e0)\n", n);
    log_write(m);
    return n > 0;
}

/* =========================================================================
 * Per-movement-component hook inside FUN_1403a76b0 (Phase 5 Step 12)
 *
 * FUN_140387010 (RVA 0x387010) is called for every movement component
 * unconditionally at line 74 of FUN_1403a76b0.  The a76b0 max=1236ms with
 * n=200 implies one movement component takes 1236ms on some entity.
 * ========================================================================= */
typedef void (*B87010Fn)(int64_t, int32_t);
static B87010Fn g_b87010_orig   = NULL;
static LONG     g_b87010_count  = 0;
static double   g_b87010_sum_ms = 0, g_b87010_max_ms = 0;

static void b87010_hook(int64_t a, int32_t b)
{
    LARGE_INTEGER t0, t1;
    QueryPerformanceCounter(&t0);
    g_b87010_orig(a, b);
    QueryPerformanceCounter(&t1);
    double ms = (t1.QuadPart - t0.QuadPart) / ((double)g_qpc_freq.QuadPart / 1000.0);
    g_b87010_sum_ms += ms;
    if (ms > g_b87010_max_ms) g_b87010_max_ms = ms;
    g_b87010_count++;
}

static BOOL install_b87010_hook(void)
{
    HMODULE exe = GetModuleHandleA(NULL);
    if (!exe) return FALSE;

    BYTE *fn_a76b0  = (BYTE *)exe + A76B0_RVA;
    BYTE *fn_b87010 = (BYTE *)exe + B87010_RVA;

    g_b87010_orig = (B87010Fn)fn_b87010;

    BYTE *stub = alloc_near(fn_a76b0, 14);
    if (!stub) {
        log_write("[eaw-mt] WARN: alloc_near failed for b87010 stub\n");
        return FALSE;
    }
    write_abs_jmp(stub, (uint64_t)b87010_hook);

    int n = 0;
    for (int i = 0; i <= A76B0_BODY_SIZE - 5; i++) {
        if (fn_a76b0[i] == 0xE8) {
            int32_t rel;
            memcpy(&rel, fn_a76b0 + i + 1, 4);
            if (fn_a76b0 + i + 5 + rel == fn_b87010) {
                int32_t new_rel = (int32_t)(stub - (fn_a76b0 + i + 5));
                DWORD old;
                VirtualProtect(fn_a76b0 + i + 1, 4, PAGE_EXECUTE_READWRITE, &old);
                memcpy(fn_a76b0 + i + 1, &new_rel, 4);
                VirtualProtect(fn_a76b0 + i + 1, 4, old, &old);
                FlushInstructionCache(GetCurrentProcess(), fn_a76b0 + i, 5);
                n++;
            }
        }
    }

    char m[80];
    sprintf(m, "[eaw-mt] b87010 (387010 in 3a76b0): %d call site(s) patched\n", n);
    log_write(m);
    return n > 0;
}

/* =========================================================================
 * Phase 5 Step 13 — FUN_14012d520 inside FUN_140385cf0
 *                   FUN_14012d480 inside FUN_140381ff0
 *
 * FUN_140387010 is the confirmed terminal stall carrier.  Two sub-callees are
 * candidates:
 *   FUN_14012d520 (RVA 0x12d520) — name→index lookup in global nav manager,
 *     called from FUN_140385cf0 (376 bytes) when inner cache is cold.
 *   FUN_14012d480 (RVA 0x12d480) — writes position/rotation to path system,
 *     called from FUN_140381ff0 (746 bytes).
 *
 * Hook approach: E8 scan inside 385cf0 body for 12d520; E8 scan inside 381ff0
 * body for 12d480.
 * ========================================================================= */
typedef int32_t (*D12d520Fn)(int64_t, const char *);
static D12d520Fn g_d12d520_orig   = NULL;
static LONG      g_d12d520_count  = 0;
static double    g_d12d520_sum_ms = 0, g_d12d520_max_ms = 0;

static int32_t d12d520_hook(int64_t manager, const char *name)
{
    LARGE_INTEGER t0, t1;
    QueryPerformanceCounter(&t0);
    int32_t r = g_d12d520_orig(manager, name);
    QueryPerformanceCounter(&t1);
    double ms = (t1.QuadPart - t0.QuadPart) / ((double)g_qpc_freq.QuadPart / 1000.0);
    g_d12d520_sum_ms += ms;
    if (ms > g_d12d520_max_ms) g_d12d520_max_ms = ms;
    g_d12d520_count++;
    return r;
}

static BOOL install_d12d520_hook(void)
{
    HMODULE exe = GetModuleHandleA(NULL);
    if (!exe) return FALSE;

    BYTE *fn_385cf0 = (BYTE *)exe + B385CF0_RVA;
    BYTE *fn_12d520 = (BYTE *)exe + D12D520_RVA;

    g_d12d520_orig = (D12d520Fn)fn_12d520;

    BYTE *stub = alloc_near(fn_385cf0, 14);
    if (!stub) {
        log_write("[eaw-mt] WARN: alloc_near failed for d12d520 stub\n");
        return FALSE;
    }
    write_abs_jmp(stub, (uint64_t)d12d520_hook);

    int n = 0;
    for (int i = 0; i <= B385CF0_BODY_SIZE - 5; i++) {
        if (fn_385cf0[i] == 0xE8) {
            int32_t rel;
            memcpy(&rel, fn_385cf0 + i + 1, 4);
            if (fn_385cf0 + i + 5 + rel == fn_12d520) {
                int32_t new_rel = (int32_t)(stub - (fn_385cf0 + i + 5));
                DWORD old;
                VirtualProtect(fn_385cf0 + i + 1, 4, PAGE_EXECUTE_READWRITE, &old);
                memcpy(fn_385cf0 + i + 1, &new_rel, 4);
                VirtualProtect(fn_385cf0 + i + 1, 4, old, &old);
                FlushInstructionCache(GetCurrentProcess(), fn_385cf0 + i, 5);
                n++;
            }
        }
    }

    char m[80];
    sprintf(m, "[eaw-mt] d12d520 (12d520 in 385cf0): %d call site(s) patched\n", n);
    log_write(m);
    return n > 0;
}

typedef void (*D12d480Fn)(int64_t, int32_t, void *);
static D12d480Fn g_d12d480_orig   = NULL;
static LONG      g_d12d480_count  = 0;
static double    g_d12d480_sum_ms = 0, g_d12d480_max_ms = 0;

static void d12d480_hook(int64_t system, int32_t slot, void *vec)
{
    LARGE_INTEGER t0, t1;
    QueryPerformanceCounter(&t0);
    g_d12d480_orig(system, slot, vec);
    QueryPerformanceCounter(&t1);
    double ms = (t1.QuadPart - t0.QuadPart) / ((double)g_qpc_freq.QuadPart / 1000.0);
    g_d12d480_sum_ms += ms;
    if (ms > g_d12d480_max_ms) g_d12d480_max_ms = ms;
    g_d12d480_count++;
}

static BOOL install_d12d480_hook(void)
{
    HMODULE exe = GetModuleHandleA(NULL);
    if (!exe) return FALSE;

    BYTE *fn_381ff0 = (BYTE *)exe + B381FF0_RVA;
    BYTE *fn_12d480 = (BYTE *)exe + D12D480_RVA;

    g_d12d480_orig = (D12d480Fn)fn_12d480;

    BYTE *stub = alloc_near(fn_381ff0, 14);
    if (!stub) {
        log_write("[eaw-mt] WARN: alloc_near failed for d12d480 stub\n");
        return FALSE;
    }
    write_abs_jmp(stub, (uint64_t)d12d480_hook);

    int n = 0;
    for (int i = 0; i <= B381FF0_BODY_SIZE - 5; i++) {
        if (fn_381ff0[i] == 0xE8) {
            int32_t rel;
            memcpy(&rel, fn_381ff0 + i + 1, 4);
            if (fn_381ff0 + i + 5 + rel == fn_12d480) {
                int32_t new_rel = (int32_t)(stub - (fn_381ff0 + i + 5));
                DWORD old;
                VirtualProtect(fn_381ff0 + i + 1, 4, PAGE_EXECUTE_READWRITE, &old);
                memcpy(fn_381ff0 + i + 1, &new_rel, 4);
                VirtualProtect(fn_381ff0 + i + 1, 4, old, &old);
                FlushInstructionCache(GetCurrentProcess(), fn_381ff0 + i, 5);
                n++;
            }
        }
    }

    char m[80];
    sprintf(m, "[eaw-mt] d12d480 (12d480 in 381ff0): %d call site(s) patched\n", n);
    log_write(m);
    return n > 0;
}

/* =========================================================================
 * Phase 5 Step 14 — Sub-callee hooks inside FUN_140387010 (344 bytes)
 *
 * d12d520 and d12d480 both showed n=0 across all windows (including stall
 * windows).  Complete call map of FUN_140387010 (+offset → target RVA):
 *   +0x040 → 35f790 (global pause check, fast guard)
 *   +0x074 → 387400 (path-following update, when entity motion state 5-10)
 *   +0x0b5 → 385cf0 (path-system name lookup, cold cache — fires each tick)
 *   +0x0da → 12d2a0 (×2, tiny wrapper — fast)
 *   +0x0f1 → 12d430 (×2, path slot acquire)
 *   +0x133 → 381ff0 (main movement step)
 *   +0x143 → 387170 (rotation update)
 *
 * Hook 387400, 385cf0, and 381ff0 via E8 scan inside 387010 body.
 * The one whose max tracks b87010_max (~1000ms) is the stall carrier.
 * ========================================================================= */

typedef void (*B387400Fn)(int64_t, int32_t);
static B387400Fn g_b387400_orig   = NULL;
static LONG      g_b387400_count  = 0;
static double    g_b387400_sum_ms = 0, g_b387400_max_ms = 0;

static void b387400_hook(int64_t a, int32_t b)
{
    LARGE_INTEGER t0, t1;
    QueryPerformanceCounter(&t0);
    g_b387400_orig(a, b);
    QueryPerformanceCounter(&t1);
    double ms = (t1.QuadPart - t0.QuadPart) / ((double)g_qpc_freq.QuadPart / 1000.0);
    g_b387400_sum_ms += ms;
    if (ms > g_b387400_max_ms) g_b387400_max_ms = ms;
    g_b387400_count++;
}

typedef int64_t (*B385cf0Fn)(int64_t);
static B385cf0Fn g_b385cf0_orig   = NULL;
static LONG      g_b385cf0_count  = 0;
static double    g_b385cf0_sum_ms = 0, g_b385cf0_max_ms = 0;

static int64_t b385cf0_hook(int64_t a)
{
    LARGE_INTEGER t0, t1;
    QueryPerformanceCounter(&t0);
    int64_t r = g_b385cf0_orig(a);
    QueryPerformanceCounter(&t1);
    double ms = (t1.QuadPart - t0.QuadPart) / ((double)g_qpc_freq.QuadPart / 1000.0);
    g_b385cf0_sum_ms += ms;
    if (ms > g_b385cf0_max_ms) g_b385cf0_max_ms = ms;
    g_b385cf0_count++;
    return r;
}

typedef int64_t (*B381ff0Fn)(int64_t);
static B381ff0Fn g_b381ff0_orig   = NULL;
static LONG      g_b381ff0_count  = 0;
static double    g_b381ff0_sum_ms = 0, g_b381ff0_max_ms = 0;

static int64_t b381ff0_hook(int64_t a)
{
    LARGE_INTEGER t0, t1;
    QueryPerformanceCounter(&t0);
    int64_t r = g_b381ff0_orig(a);
    QueryPerformanceCounter(&t1);
    double ms = (t1.QuadPart - t0.QuadPart) / ((double)g_qpc_freq.QuadPart / 1000.0);
    g_b381ff0_sum_ms += ms;
    if (ms > g_b381ff0_max_ms) g_b381ff0_max_ms = ms;
    g_b381ff0_count++;
    return r;
}

static BOOL install_b87010_subcallee_hooks(void)
{
    HMODULE exe = GetModuleHandleA(NULL);
    if (!exe) return FALSE;

    BYTE *fn_b87010  = (BYTE *)exe + B87010_RVA;
    BYTE *fn_b387400 = (BYTE *)exe + B387400_RVA;
    BYTE *fn_b385cf0 = (BYTE *)exe + B385CF0_RVA;
    BYTE *fn_b381ff0 = (BYTE *)exe + B381FF0_RVA;

    g_b387400_orig = (B387400Fn)fn_b387400;
    g_b385cf0_orig = (B385cf0Fn)fn_b385cf0;
    g_b381ff0_orig = (B381ff0Fn)fn_b381ff0;

    /* Allocate three stubs near the scan body */
    BYTE *stub_block = alloc_near(fn_b87010, 3 * 14);
    if (!stub_block) {
        log_write("[eaw-mt] WARN: alloc_near failed for b87010_subcallee stubs\n");
        return FALSE;
    }
    BYTE *stub_b387400 = stub_block + 0 * 14;
    BYTE *stub_b385cf0 = stub_block + 1 * 14;
    BYTE *stub_b381ff0 = stub_block + 2 * 14;

    write_abs_jmp(stub_b387400, (uint64_t)b387400_hook);
    write_abs_jmp(stub_b385cf0, (uint64_t)b385cf0_hook);
    write_abs_jmp(stub_b381ff0, (uint64_t)b381ff0_hook);

    int n387400 = 0, n385cf0 = 0, n381ff0 = 0;
    for (int i = 0; i <= B87010_BODY_SIZE - 5; i++) {
        if (fn_b87010[i] != 0xE8) continue;
        int32_t rel;
        memcpy(&rel, fn_b87010 + i + 1, 4);
        BYTE *target = fn_b87010 + i + 5 + rel;
        BYTE *stub   = NULL;
        int  *cnt    = NULL;
        if      (target == fn_b387400) { stub = stub_b387400; cnt = &n387400; }
        else if (target == fn_b385cf0) { stub = stub_b385cf0; cnt = &n385cf0; }
        else if (target == fn_b381ff0) { stub = stub_b381ff0; cnt = &n381ff0; }
        if (!stub) continue;
        int32_t new_rel = (int32_t)(stub - (fn_b87010 + i + 5));
        DWORD old;
        VirtualProtect(fn_b87010 + i + 1, 4, PAGE_EXECUTE_READWRITE, &old);
        memcpy(fn_b87010 + i + 1, &new_rel, 4);
        VirtualProtect(fn_b87010 + i + 1, 4, old, &old);
        FlushInstructionCache(GetCurrentProcess(), fn_b87010 + i, 5);
        (*cnt)++;
    }

    char m[128];
    sprintf(m, "[eaw-mt] b87010_sub: 387400=%d 385cf0=%d 381ff0=%d site(s) patched\n",
            n387400, n385cf0, n381ff0);
    log_write(m);
    return (n387400 + n385cf0 + n381ff0) > 0;
}

/* =========================================================================
 * FUN_14028a4d0 sub-hook (Phase 5 Step 7a)
 *
 * FUN_14028a4d0 (RVA 0x28a4d0) is called from FUN_14028d400 when the frame
 * counter at *(param_1+0x1f0) counts down to zero.  It fires BEFORE the
 * vtable[22] dispatch, which explains why gslot22 n=0 during stall frames:
 * the stall may be here, not in the vtable path.
 *
 * Hook approach: E8 call-site scan inside FUN_14028d400 body (654 bytes).
 * ========================================================================= */
/* Callee decompile shows it uses param_1 (RCX) — Ghidra's call-site shows void
 * but the argument is the same object as FUN_14028d400's param_1. */
typedef void (*GA4d0Fn)(int64_t);
static GA4d0Fn g_ga4d0_orig   = NULL;
static LONG    g_ga4d0_count  = 0;
static double  g_ga4d0_sum_ms = 0, g_ga4d0_max_ms = 0;

static void ga4d0_hook(int64_t a)
{
    LARGE_INTEGER t0, t1;
    QueryPerformanceCounter(&t0);
    g_ga4d0_orig(a);
    QueryPerformanceCounter(&t1);
    double ms = (t1.QuadPart - t0.QuadPart) / ((double)g_qpc_freq.QuadPart / 1000.0);
    g_ga4d0_sum_ms += ms;
    if (ms > g_ga4d0_max_ms) g_ga4d0_max_ms = ms;
    g_ga4d0_count++;
    if (ms >= 100.0) {
        char s[64];
        sprintf(s, "[eaw-mt] GA4D0 %.1fms\n", ms);
        log_write(s);
    }
}

static BOOL install_ga4d0_hook(void)
{
    HMODULE exe = GetModuleHandleA(NULL);
    if (!exe) return FALSE;

    BYTE *gsvc_fn  = (BYTE *)exe + GAME_SERVICE_RVA;
    BYTE *ga4d0_fn = (BYTE *)exe + GA4D0_RVA;

    g_ga4d0_orig = (GA4d0Fn)ga4d0_fn;

    BYTE *call_site = NULL;
    for (int i = 0; i <= GSVC_BODY_SIZE - 5; i++) {
        if (gsvc_fn[i] == 0xE8) {
            int32_t rel;
            memcpy(&rel, gsvc_fn + i + 1, 4);
            if (gsvc_fn + i + 5 + rel == ga4d0_fn) {
                call_site = gsvc_fn + i;
                break;
            }
        }
    }

    if (!call_site) {
        log_write("[eaw-mt] WARN: FUN_14028a4d0 CALL not found in FUN_14028d400\n");
        return FALSE;
    }

    BYTE *stub = alloc_near(call_site, 14);
    if (!stub) {
        log_write("[eaw-mt] WARN: alloc_near failed for ga4d0 stub\n");
        return FALSE;
    }
    write_abs_jmp(stub, (uint64_t)ga4d0_hook);

    int32_t new_rel = (int32_t)(stub - (call_site + 5));
    DWORD old;
    VirtualProtect(call_site + 1, 4, PAGE_EXECUTE_READWRITE, &old);
    memcpy(call_site + 1, &new_rel, 4);
    VirtualProtect(call_site + 1, 4, old, &old);
    FlushInstructionCache(GetCurrentProcess(), call_site, 5);

    log_write("[eaw-mt] ga4d0 (28a4d0) call-site hook inside FUN_14028d400 installed\n");
    return TRUE;
}

/* =========================================================================
 * Slot-22 tail call hooks (Phase 5 Step 5)
 *
 * Both SpaceModeClass and LandModeClass vtable slot 22 end with an
 * unconditional tail call to FUN_1403639d0 (RVA 0x3639d0, 3916 bytes).
 * Patch both call sites to confirm this function carries the episodic
 * 1000ms stall.  Also hook FUN_14028bf10 (GameModeManager signal dispatch,
 * #1 suspect) and FUN_140343b90 (auto-resolve battle selection, #2 suspect)
 * inside FUN_1403639d0 to attribute the stall within one run.
 *
 * NOTE: profiling confirmed the menu demo runs GalacticModeClass (slot 22 =
 * FUN_14045e030), so these hooks fired n=0.  The gsvc hook above captures
 * the actual stall path regardless of active mode.
 * ========================================================================= */
#define SPACE_SLOT22_RVA   0x4d95a0ULL  /* FUN_1404d95a0, 358 bytes */
#define LAND_SLOT22_RVA    0x3bb440ULL  /* FUN_1403bb440, 598 bytes */
#define SLOT22_TAIL_RVA    0x3639d0ULL  /* FUN_1403639d0, 3916 bytes — shared tail call */
#define SIGDISP_RVA        0x28bf10ULL  /* FUN_14028bf10 — GameModeManager signal dispatch */
#define BATTLE_SELECT_RVA  0x343b90ULL  /* FUN_140343b90 — tactical auto-resolve selection */

typedef void    (*Slot22TailFn)(int64_t, int64_t, int64_t, int64_t);
typedef int64_t (*SignalDispFn)(void *, int32_t, int32_t, int32_t, int64_t, uint8_t);
typedef int64_t (*BattleSelFn) (void *, void *);

static Slot22TailFn g_slot22_tail_orig = NULL;
static SignalDispFn g_sigdisp_orig     = NULL;
static BattleSelFn  g_bsel_orig        = NULL;

static LONG   g_tail22_count   = 0;
static double g_tail22_sum_ms  = 0, g_tail22_max_ms  = 0;
static LONG   g_sigdisp_count  = 0;
static double g_sigdisp_sum_ms = 0, g_sigdisp_max_ms = 0;
static LONG   g_bsel_count     = 0;
static double g_bsel_sum_ms    = 0, g_bsel_max_ms    = 0;

static void game_service_hook(int64_t a, int32_t b) {
    LARGE_INTEGER t0, t1;
    QueryPerformanceCounter(&t0);
    g_gsvc_orig(a, b);
    QueryPerformanceCounter(&t1);
    double ms = (t1.QuadPart - t0.QuadPart) / ((double)g_qpc_freq.QuadPart / 1000.0);
    g_gsvc_sum_ms += ms;
    if (ms > g_gsvc_max_ms) g_gsvc_max_ms = ms;
    g_gsvc_count++;
    if (ms >= 100.0) {
        /* Read vtable[22] (offset 0xB0) of the active game-mode object to find
         * which function is actually dispatched.  Read AFTER the call since the
         * object is long-lived; log RVA so we can look it up in Ghidra. */
        int64_t mode_obj = a ? *(int64_t *)(a + 0x38) : 0;
        uint64_t slot22_rva = 0;
        if (mode_obj) {
            int64_t vtbl = *(int64_t *)mode_obj;
            uint64_t slot22_fn = *(uint64_t *)(vtbl + 0xB0);
            slot22_rva = slot22_fn - (uint64_t)GetModuleHandleA(NULL);
        }
        char s[96];
        sprintf(s, "[eaw-mt] GSVC %.1fms vtbl22_rva=0x%llx mode_obj=%s\n",
                ms, (unsigned long long)slot22_rva, mode_obj ? "non-null" : "null");
        log_write(s);
    }
}

static void slot22_tail_hook(int64_t a, int64_t b, int64_t c, int64_t d) {
    LARGE_INTEGER t0, t1;
    QueryPerformanceCounter(&t0);
    g_slot22_tail_orig(a, b, c, d);
    QueryPerformanceCounter(&t1);
    double ms = (t1.QuadPart - t0.QuadPart) / ((double)g_qpc_freq.QuadPart / 1000.0);
    g_tail22_sum_ms += ms;
    if (ms > g_tail22_max_ms) g_tail22_max_ms = ms;
    g_tail22_count++;
    if (ms >= 100.0) {
        char s[64];
        sprintf(s, "[eaw-mt] TAIL22 %.1fms\n", ms);
        log_write(s);
    }
}

static int64_t signal_dispatch_hook(void *a, int32_t b, int32_t c, int32_t d, int64_t e, uint8_t f) {
    LARGE_INTEGER t0, t1;
    QueryPerformanceCounter(&t0);
    int64_t r = g_sigdisp_orig(a, b, c, d, e, f);
    QueryPerformanceCounter(&t1);
    double ms = (t1.QuadPart - t0.QuadPart) / ((double)g_qpc_freq.QuadPart / 1000.0);
    g_sigdisp_sum_ms += ms;
    if (ms > g_sigdisp_max_ms) g_sigdisp_max_ms = ms;
    g_sigdisp_count++;
    return r;
}

static int64_t battle_select_hook(void *a, void *b) {
    LARGE_INTEGER t0, t1;
    QueryPerformanceCounter(&t0);
    int64_t r = g_bsel_orig(a, b);
    QueryPerformanceCounter(&t1);
    double ms = (t1.QuadPart - t0.QuadPart) / ((double)g_qpc_freq.QuadPart / 1000.0);
    g_bsel_sum_ms += ms;
    if (ms > g_bsel_max_ms) g_bsel_max_ms = ms;
    g_bsel_count++;
    return r;
}

static BOOL install_slot22_hooks(void) {
    HMODULE exe = GetModuleHandleA(NULL);
    if (!exe) return FALSE;

    BYTE *tail_target   = (BYTE *)exe + SLOT22_TAIL_RVA;
    BYTE *sigdisp_target = (BYTE *)exe + SIGDISP_RVA;
    BYTE *bsel_target   = (BYTE *)exe + BATTLE_SELECT_RVA;

    g_slot22_tail_orig = (Slot22TailFn)tail_target;
    g_sigdisp_orig     = (SignalDispFn)sigdisp_target;
    g_bsel_orig        = (BattleSelFn) bsel_target;

    /* Allocate 3 stubs near SpaceMode slot 22 (all scan targets within 2MB). */
    BYTE *space_fn = (BYTE *)exe + SPACE_SLOT22_RVA;
    BYTE *land_fn  = (BYTE *)exe + LAND_SLOT22_RVA;
    BYTE *tail_fn  = (BYTE *)exe + SLOT22_TAIL_RVA;

    BYTE *stub_block = alloc_near(space_fn, 14 * 3);
    if (!stub_block) {
        log_write("[eaw-mt] WARN: alloc_near failed for slot22 stubs\n");
        return FALSE;
    }
    BYTE *tail_stub    = stub_block + 0 * 14;
    BYTE *sigdisp_stub = stub_block + 1 * 14;
    BYTE *bsel_stub    = stub_block + 2 * 14;

    write_abs_jmp(tail_stub,    (uint64_t)slot22_tail_hook);
    write_abs_jmp(sigdisp_stub, (uint64_t)signal_dispatch_hook);
    write_abs_jmp(bsel_stub,    (uint64_t)battle_select_hook);

    int n = 0;

    /* Patch FUN_1403639d0 call site in SpaceMode slot 22 (358 bytes). */
    for (int j = 0; j <= 358 - 5; j++) {
        if (space_fn[j] == 0xE8) {
            int32_t rel; memcpy(&rel, space_fn + j + 1, 4);
            if (space_fn + j + 5 + rel == tail_target) {
                int32_t nr = (int32_t)(tail_stub - (space_fn + j + 5));
                DWORD old;
                VirtualProtect(space_fn + j + 1, 4, PAGE_EXECUTE_READWRITE, &old);
                memcpy(space_fn + j + 1, &nr, 4);
                VirtualProtect(space_fn + j + 1, 4, old, &old);
                FlushInstructionCache(GetCurrentProcess(), space_fn + j, 5);
                n++; break;
            }
        }
    }

    /* Patch FUN_1403639d0 call site in LandMode slot 22 (598 bytes). */
    for (int j = 0; j <= 598 - 5; j++) {
        if (land_fn[j] == 0xE8) {
            int32_t rel; memcpy(&rel, land_fn + j + 1, 4);
            if (land_fn + j + 5 + rel == tail_target) {
                int32_t nr = (int32_t)(tail_stub - (land_fn + j + 5));
                DWORD old;
                VirtualProtect(land_fn + j + 1, 4, PAGE_EXECUTE_READWRITE, &old);
                memcpy(land_fn + j + 1, &nr, 4);
                VirtualProtect(land_fn + j + 1, 4, old, &old);
                FlushInstructionCache(GetCurrentProcess(), land_fn + j, 5);
                n++; break;
            }
        }
    }

    /* Patch FUN_14028bf10 call site inside FUN_1403639d0 (3916 bytes). */
    for (int j = 0; j <= 3916 - 5; j++) {
        if (tail_fn[j] == 0xE8) {
            int32_t rel; memcpy(&rel, tail_fn + j + 1, 4);
            if (tail_fn + j + 5 + rel == sigdisp_target) {
                int32_t nr = (int32_t)(sigdisp_stub - (tail_fn + j + 5));
                DWORD old;
                VirtualProtect(tail_fn + j + 1, 4, PAGE_EXECUTE_READWRITE, &old);
                memcpy(tail_fn + j + 1, &nr, 4);
                VirtualProtect(tail_fn + j + 1, 4, old, &old);
                FlushInstructionCache(GetCurrentProcess(), tail_fn + j, 5);
                n++; break;
            }
        }
    }

    /* Patch BOTH FUN_140343b90 call sites inside FUN_1403639d0. */
    for (int j = 0; j <= 3916 - 5; j++) {
        if (tail_fn[j] == 0xE8) {
            int32_t rel; memcpy(&rel, tail_fn + j + 1, 4);
            if (tail_fn + j + 5 + rel == bsel_target) {
                int32_t nr = (int32_t)(bsel_stub - (tail_fn + j + 5));
                DWORD old;
                VirtualProtect(tail_fn + j + 1, 4, PAGE_EXECUTE_READWRITE, &old);
                memcpy(tail_fn + j + 1, &nr, 4);
                VirtualProtect(tail_fn + j + 1, 4, old, &old);
                FlushInstructionCache(GetCurrentProcess(), tail_fn + j, 5);
                n++;
            }
        }
    }

    char m[80];
    sprintf(m, "[eaw-mt] slot22 hooks: %d/5 patched (expect 2+1+2)\n", n);
    log_write(m);
    return n > 0;
}

static BOOL install_game_service_hook(void) {
    HMODULE exe = GetModuleHandleA(NULL);
    if (!exe) return FALSE;

    BYTE *winmain = (BYTE *)exe + WINMAIN_RVA;
    BYTE *target  = (BYTE *)exe + GAME_SERVICE_RVA;

    g_gsvc_orig = (GameServiceFn)target;

    BYTE *call_site = NULL;
    for (int i = 0; i <= WINMAIN_SIZE - 5; i++) {
        if (winmain[i] == 0xE8) {
            int32_t rel;
            memcpy(&rel, winmain + i + 1, 4);
            if (winmain + i + 5 + rel == target) {
                call_site = winmain + i;
                break;
            }
        }
    }

    if (!call_site) {
        log_write("[eaw-mt] WARN: FUN_14028d400 CALL not found in WinMain\n");
        return FALSE;
    }

    BYTE *stub = alloc_near(call_site, 14);
    if (!stub) {
        log_write("[eaw-mt] WARN: alloc_near failed for game_service stub\n");
        return FALSE;
    }
    write_abs_jmp(stub, (uint64_t)game_service_hook);

    int32_t new_rel = (int32_t)(stub - (call_site + 5));
    DWORD old;
    VirtualProtect(call_site + 1, 4, PAGE_EXECUTE_READWRITE, &old);
    memcpy(call_site + 1, &new_rel, 4);
    VirtualProtect(call_site + 1, 4, old, &old);
    FlushInstructionCache(GetCurrentProcess(), call_site, 5);

    log_write("[eaw-mt] game_service (28d400) call-site hook installed\n");
    return TRUE;
}

/* =========================================================================
 * Profiling
 *
 * FUN_140180d90 is called TWICE per frame (flush#1 then flush#2).  The
 * g_flush_serial counter (odd = flush#1, even = flush#2) lets us distinguish
 * the two and anchor frame time on consecutive flush#1 entries.
 *
 * Metrics per PROFILE_WINDOW true frames:
 *   flush_ms    — render thread round-trip for one flush (per flush, n≈2N)
 *   frame_ms    — flush#1(k) entry → flush#1(k+1) entry = full frame
 *   inter_ms    — flush#1 render_done → flush#2 entry
 *                 (covers all WinMain 580-591 subsystems)
 *   sim_tick_ms — vtable[43] dispatch duration (thin, < 5 µs; Pump_Threads
 *                 is accounted by pump_ms below)
 *   pump_ms     — FUN_140247a90 (Pump_Threads) per-invocation, n ≤ N
 *                 (0 when ServiceRate gate skips it)
 *
 * Thread-safety: g_flush_start/end are written by the render thread before
 * SetEvent(render_done); read by the main thread after WaitForSingleObject.
 * All other profiling state is main-thread-only.
 * ========================================================================= */

#define PROFILE_WINDOW 300

static LARGE_INTEGER g_qpc_freq;

/* Incremented on each render_flush_hook entry; odd = flush#1, even = flush#2. */
static volatile LONG g_flush_serial = 0;

/* Written by render thread before signaling render_done; read after the wait. */
static LARGE_INTEGER g_flush_start;
static LARGE_INTEGER g_flush_end;

/* Entry time of the most recent flush#1 (frame anchor). */
static LARGE_INTEGER g_frame1_anchor;
static BOOL          g_frame1_anchor_valid = FALSE;

/* Time recorded after flush#1's WaitForSingleObject returns. */
static LARGE_INTEGER g_flush1_done;
static BOOL          g_flush1_done_valid = FALSE;

/* Per-window accumulators — main thread only. */
static LONG   g_frame_count = 0;
static LONG   g_flush_count = 0;

static double g_flush_sum_ms = 0, g_flush_min_ms = 1e9, g_flush_max_ms = 0;
static double g_frame_sum_ms = 0, g_frame_min_ms = 1e9, g_frame_max_ms = 0;
static double g_inter_sum_ms = 0, g_inter_min_ms = 1e9, g_inter_max_ms = 0;
static double g_sim_sum_ms   = 0, g_sim_min_ms   = 1e9, g_sim_max_ms   = 0;

static void profile_report_and_reset(void) {
    double fn  = (double)(g_frame_count ? g_frame_count : 1);
    double fln = (double)(g_flush_count ? g_flush_count : 1);
    double pn  = (double)(g_pump_count  ? g_pump_count  : 1);

    char buf[768];
    sprintf(buf,
        "[eaw-mt] profile(%ld frames):\n"
        "  flush    avg=%.2f min=%.2f max=%.2f ms (per flush, n=%ld)\n"
        "  frame    avg=%.2f min=%.2f max=%.2f ms\n"
        "  inter    avg=%.2f min=%.2f max=%.2f ms (flush#1->flush#2)\n"
        "  sim_tick avg=%.2f min=%.2f max=%.2f ms\n"
        "  pump     avg=%.2f min=%.2f max=%.2f ms (Pump_Threads, n=%ld)\n"
        "  wait_msg n=%ld timeout=%ld (WaitMessage->1ms cap)\n",
        (long)g_frame_count,
        g_flush_sum_ms / fln, g_flush_min_ms, g_flush_max_ms, (long)g_flush_count,
        g_frame_sum_ms / fn,  g_frame_min_ms, g_frame_max_ms,
        g_inter_sum_ms / fn,  g_inter_min_ms, g_inter_max_ms,
        g_sim_sum_ms   / fn,  g_sim_min_ms,   g_sim_max_ms,
        g_pump_sum_ms  / pn,  g_pump_min_ms,  g_pump_max_ms, (long)g_pump_count,
        (long)g_wait_msg_count, (long)g_wait_msg_timeout);
    log_write(buf);

    /* Inter-flush breakdown — split into two ODS calls so Wine truncation doesn't hide hooks 8-13. */
    {
        char ibuf[640];
        int off = sprintf(ibuf, "[eaw-mt] inter breakdown1:");
        for (int i = 0; i < 8; i++) {
            if (!g_ifc_site[i]) continue;
            double n2 = (double)(g_ifc[i].count ? g_ifc[i].count : 1);
            off += sprintf(ibuf + off, "\n  %-6s avg=%.2f max=%.2f ms (n=%ld)",
                g_ifc_name[i],
                g_ifc[i].sum_ms / n2,
                g_ifc[i].max_ms,
                (long)g_ifc[i].count);
        }
        ibuf[off++] = '\n';
        ibuf[off]   = '\0';
        log_write(ibuf);

        off = sprintf(ibuf, "[eaw-mt] inter breakdown2:");
        for (int i = 8; i < 14; i++) {
            if (!g_ifc_site[i]) continue;
            double n2 = (double)(g_ifc[i].count ? g_ifc[i].count : 1);
            off += sprintf(ibuf + off, "\n  %-6s avg=%.2f max=%.2f ms (n=%ld)",
                g_ifc_name[i],
                g_ifc[i].sum_ms / n2,
                g_ifc[i].max_ms,
                (long)g_ifc[i].count);
        }
        ibuf[off++] = '\n';
        ibuf[off]   = '\0';
        log_write(ibuf);

        off = sprintf(ibuf, "[eaw-mt] inter breakdown3:");
        for (int i = 14; i < 20; i++) {
            if (!g_ifc_site[i]) continue;
            double n2 = (double)(g_ifc[i].count ? g_ifc[i].count : 1);
            off += sprintf(ibuf + off, "\n  %-6s avg=%.2f max=%.2f ms (n=%ld)",
                g_ifc_name[i],
                g_ifc[i].sum_ms / n2,
                g_ifc[i].max_ms,
                (long)g_ifc[i].count);
        }
        ibuf[off++] = '\n';
        ibuf[off]   = '\0';
        log_write(ibuf);

        off = sprintf(ibuf, "[eaw-mt] inter breakdown4:");
        for (int i = 20; i < N_IFC; i++) {
            if (!g_ifc_site[i]) continue;
            double n2 = (double)(g_ifc[i].count ? g_ifc[i].count : 1);
            off += sprintf(ibuf + off, "\n  %-6s avg=%.2f max=%.2f ms (n=%ld)",
                g_ifc_name[i],
                g_ifc[i].sum_ms / n2,
                g_ifc[i].max_ms,
                (long)g_ifc[i].count);
        }
        ibuf[off++] = '\n';
        ibuf[off]   = '\0';
        log_write(ibuf);

        /* Gap timers — unhooked spans between adjacent hooked calls. */
        off = sprintf(ibuf, "[eaw-mt] inter gaps:");
        off += sprintf(ibuf + off, "\n  gapA[21caf0] avg=%.2f max=%.2f ms (n=%ld)",
            g_gap[0].count ? g_gap[0].sum_ms / g_gap[0].count : 0.0,
            g_gap[0].max_ms, (long)g_gap[0].count);
        off += sprintf(ibuf + off, "\n  gapB[1768f0] avg=%.2f max=%.2f ms (n=%ld)",
            g_gap[1].count ? g_gap[1].sum_ms / g_gap[1].count : 0.0,
            g_gap[1].max_ms, (long)g_gap[1].count);
        off += sprintf(ibuf + off, "\n  gapC[2ee40+]  avg=%.2f max=%.2f ms (n=%ld)",
            g_gap[2].count ? g_gap[2].sum_ms / g_gap[2].count : 0.0,
            g_gap[2].max_ms, (long)g_gap[2].count);
        off += sprintf(ibuf + off, "\n  gapD[1dc60->f2] avg=%.2f max=%.2f ms (n=%ld)",
            g_gap[3].count ? g_gap[3].sum_ms / g_gap[3].count : 0.0,
            g_gap[3].max_ms, (long)g_gap[3].count);
        off += sprintf(ibuf + off, "\n  gapE[3b1b50->51d10] avg=%.2f max=%.2f ms (n=%ld)",
            g_gap[4].count ? g_gap[4].sum_ms / g_gap[4].count : 0.0,
            g_gap[4].max_ms, (long)g_gap[4].count);
        ibuf[off++] = '\n';
        ibuf[off]   = '\0';
        log_write(ibuf);
    }

    /* Game-service + slot-22 breakdown. */
    {
        char ibuf[2064];
        double gn   = (double)(g_gsvc_count    ? g_gsvc_count    : 1);
        double g22  = (double)(g_gslot22_count ? g_gslot22_count : 1);
        double a4n  = (double)(g_ga4d0_count   ? g_ga4d0_count   : 1);
        double ssn  = (double)(g_sslot22_count ? g_sslot22_count : 1);
        double sun  = (double)(g_sunit22_count ? g_sunit22_count : 1);
        double t22n = (double)(g_tail22i_count ? g_tail22i_count : 1);
        double a1n  = (double)(g_a1a240_count  ? g_a1a240_count  : 1);
        double s9n  = (double)(g_s9d270_count  ? g_s9d270_count  : 1);
        double tn   = (double)(g_tail22_count  ? g_tail22_count  : 1);
        double sdn  = (double)(g_sigdisp_count ? g_sigdisp_count : 1);
        double bn   = (double)(g_bsel_count    ? g_bsel_count    : 1);
        double t36n = (double)(g_t364920_count ? g_t364920_count : 1);
        double t2bn = (double)(g_t2be640_count ? g_t2be640_count : 1);
        double t49n = (double)(g_t490580_count ? g_t490580_count : 1);
        double a6n  = (double)(g_ta6b80_count  ? g_ta6b80_count  : 1);
        double a6d0 = (double)(g_ta62d0_count  ? g_ta62d0_count  : 1);
        double e70n = (double)(g_t20ed70_count ? g_t20ed70_count : 1);
        double pen  = (double)(g_pumpe_count   ? g_pumpe_count   : 1);
        double a76n = (double)(g_a76b0_count  ? g_a76b0_count   : 1);
        double a9n  = (double)(g_a9e30_count  ? g_a9e30_count   : 1);
        double e36n = (double)(g_e369e0_count ? g_e369e0_count  : 1);
        double b87n  = (double)(g_b87010_count  ? g_b87010_count  : 1);
        double d52n  = (double)(g_d12d520_count  ? g_d12d520_count  : 1);
        double d48n  = (double)(g_d12d480_count  ? g_d12d480_count  : 1);
        double b74n  = (double)(g_b387400_count  ? g_b387400_count  : 1);
        double b5n   = (double)(g_b385cf0_count  ? g_b385cf0_count  : 1);
        double b1n   = (double)(g_b381ff0_count  ? g_b381ff0_count  : 1);
        sprintf(ibuf,
            "[eaw-mt] gsvc(28d400):    avg=%.2f max=%.2f ms (n=%ld)\n"
            "  sslot22(4d95a0): avg=%.2f max=%.2f ms (n=%ld)\n"
            "  tail22i(3639d0): avg=%.2f max=%.2f ms (n=%ld)\n"
            "  t2be640(2be640): avg=%.2f max=%.2f ms (n=%ld)\n"
            "  ta6b80(3a6b80):  avg=%.2f max=%.2f ms (n=%ld)\n"
            "  pumpe(247a90):   avg=%.2f max=%.2f ms (n=%ld)\n"
            "  a76b0(3a76b0):   avg=%.2f max=%.2f ms (n=%ld)\n"
            "  b87010(387010):  avg=%.2f max=%.2f ms (n=%ld)\n"
            "  d12d520(12d520): avg=%.2f max=%.2f ms (n=%ld)\n"
            "  d12d480(12d480): avg=%.2f max=%.2f ms (n=%ld)\n"
            "  b387400(387400): avg=%.2f max=%.2f ms (n=%ld)\n"
            "  b385cf0(385cf0): avg=%.2f max=%.2f ms (n=%ld)\n"
            "  b381ff0(381ff0): avg=%.2f max=%.2f ms (n=%ld)\n"
            "  a9e30(3a9e30):   avg=%.2f max=%.2f ms (n=%ld)\n"
            "  e369e0(5369e0):  avg=%.2f max=%.2f ms (n=%ld)\n"
            "  t364920(364920): avg=%.2f max=%.2f ms (n=%ld)\n"
            "  t490580(490580): avg=%.2f max=%.2f ms (n=%ld)\n"
            "  ta62d0(2a62d0):  avg=%.2f max=%.2f ms (n=%ld)\n"
            "  t20ed70(20ed70): avg=%.2f max=%.2f ms (n=%ld)\n"
            "  a1a240(41a240):  avg=%.2f max=%.2f ms (n=%ld)\n"
            "  s9d270(59d270):  avg=%.2f max=%.2f ms (n=%ld)\n"
            "  sunit22(527dd0): avg=%.2f max=%.2f ms (n=%ld)\n"
            "  ga4d0(28a4d0):   avg=%.2f max=%.2f ms (n=%ld)\n"
            "  gslot22(45e030): avg=%.2f max=%.2f ms (n=%ld)\n"
            "  tail22(3639d0):  avg=%.2f max=%.2f ms (n=%ld)\n"
            "  sigdisp(28bf10): avg=%.2f max=%.2f ms (n=%ld)\n"
            "  bsel(343b90):    avg=%.2f max=%.2f ms (n=%ld)\n",
            g_gsvc_sum_ms    / gn,   g_gsvc_max_ms,    (long)g_gsvc_count,
            g_sslot22_sum_ms / ssn,  g_sslot22_max_ms, (long)g_sslot22_count,
            g_tail22i_sum_ms / t22n, g_tail22i_max_ms, (long)g_tail22i_count,
            g_t2be640_sum_ms / t2bn, g_t2be640_max_ms, (long)g_t2be640_count,
            g_ta6b80_sum_ms  / a6n,  g_ta6b80_max_ms,  (long)g_ta6b80_count,
            g_pumpe_sum_ms   / pen,  g_pumpe_max_ms,   (long)g_pumpe_count,
            g_a76b0_sum_ms   / a76n, g_a76b0_max_ms,   (long)g_a76b0_count,
            g_b87010_sum_ms  / b87n,  g_b87010_max_ms,  (long)g_b87010_count,
            g_d12d520_sum_ms / d52n,  g_d12d520_max_ms,  (long)g_d12d520_count,
            g_d12d480_sum_ms / d48n,  g_d12d480_max_ms,  (long)g_d12d480_count,
            g_b387400_sum_ms / b74n,  g_b387400_max_ms,  (long)g_b387400_count,
            g_b385cf0_sum_ms / b5n,   g_b385cf0_max_ms,  (long)g_b385cf0_count,
            g_b381ff0_sum_ms / b1n,   g_b381ff0_max_ms,  (long)g_b381ff0_count,
            g_a9e30_sum_ms   / a9n,   g_a9e30_max_ms,    (long)g_a9e30_count,
            g_e369e0_sum_ms  / e36n, g_e369e0_max_ms,  (long)g_e369e0_count,
            g_t364920_sum_ms / t36n, g_t364920_max_ms, (long)g_t364920_count,
            g_t490580_sum_ms / t49n, g_t490580_max_ms, (long)g_t490580_count,
            g_ta62d0_sum_ms  / a6d0, g_ta62d0_max_ms,  (long)g_ta62d0_count,
            g_t20ed70_sum_ms / e70n, g_t20ed70_max_ms, (long)g_t20ed70_count,
            g_a1a240_sum_ms  / a1n,  g_a1a240_max_ms,  (long)g_a1a240_count,
            g_s9d270_sum_ms  / s9n,  g_s9d270_max_ms,  (long)g_s9d270_count,
            g_sunit22_sum_ms / sun,  g_sunit22_max_ms, (long)g_sunit22_count,
            g_ga4d0_sum_ms   / a4n,  g_ga4d0_max_ms,   (long)g_ga4d0_count,
            g_gslot22_sum_ms / g22,  g_gslot22_max_ms, (long)g_gslot22_count,
            g_tail22_sum_ms  / tn,   g_tail22_max_ms,  (long)g_tail22_count,
            g_sigdisp_sum_ms / sdn,  g_sigdisp_max_ms, (long)g_sigdisp_count,
            g_bsel_sum_ms    / bn,   g_bsel_max_ms,    (long)g_bsel_count);
        log_write(ibuf);
    }

    /* File I/O summary — always a separate ODS call so Wine truncation can't hide it */
    {
        char ibuf[256];
        sprintf(ibuf, "[eaw-mt] io_rd: total=%.1fms max=%.1fms (n=%ld reads, %ldKB) main-thread\n",
                g_io_win_sum_ms, g_io_win_max_ms,
                (long)g_io_win_count, (long)(g_io_win_bytes / 1024));
        log_write(ibuf);
    }

    g_frame_count = 0;
    g_flush_count = 0;
    g_pump_count  = 0;
    g_wait_msg_count   = 0;
    g_wait_msg_timeout = 0;
    g_io_win_sum_ms = 0.0;
    g_io_win_max_ms = 0.0;
    g_io_win_count  = 0;
    g_io_win_bytes  = 0;
    g_gsvc_count    = 0; g_gsvc_sum_ms    = 0; g_gsvc_max_ms    = 0;
    g_sslot22_count = 0; g_sslot22_sum_ms = 0; g_sslot22_max_ms = 0;
    g_tail22i_count = 0; g_tail22i_sum_ms = 0; g_tail22i_max_ms = 0;
    g_a1a240_count  = 0; g_a1a240_sum_ms  = 0; g_a1a240_max_ms  = 0;
    g_s9d270_count  = 0; g_s9d270_sum_ms  = 0; g_s9d270_max_ms  = 0;
    g_sunit22_count = 0; g_sunit22_sum_ms = 0; g_sunit22_max_ms = 0;
    g_ga4d0_count   = 0; g_ga4d0_sum_ms   = 0; g_ga4d0_max_ms   = 0;
    g_gslot22_count = 0; g_gslot22_sum_ms = 0; g_gslot22_max_ms = 0;
    g_tail22_count  = 0; g_tail22_sum_ms  = 0; g_tail22_max_ms  = 0;
    g_sigdisp_count = 0; g_sigdisp_sum_ms = 0; g_sigdisp_max_ms = 0;
    g_bsel_count    = 0; g_bsel_sum_ms    = 0; g_bsel_max_ms    = 0;
    g_t364920_count = 0; g_t364920_sum_ms = 0; g_t364920_max_ms = 0;
    g_t2be640_count = 0; g_t2be640_sum_ms = 0; g_t2be640_max_ms = 0;
    g_t490580_count = 0; g_t490580_sum_ms = 0; g_t490580_max_ms = 0;
    g_ta6b80_count  = 0; g_ta6b80_sum_ms  = 0; g_ta6b80_max_ms  = 0;
    g_ta62d0_count  = 0; g_ta62d0_sum_ms  = 0; g_ta62d0_max_ms  = 0;
    g_t20ed70_count = 0; g_t20ed70_sum_ms = 0; g_t20ed70_max_ms = 0;
    g_pumpe_count   = 0; g_pumpe_sum_ms   = 0; g_pumpe_max_ms   = 0;
    g_a76b0_count   = 0; g_a76b0_sum_ms   = 0; g_a76b0_max_ms   = 0;
    g_a9e30_count   = 0; g_a9e30_sum_ms   = 0; g_a9e30_max_ms   = 0;
    g_e369e0_count  = 0; g_e369e0_sum_ms  = 0; g_e369e0_max_ms  = 0;
    g_b87010_count  = 0; g_b87010_sum_ms  = 0; g_b87010_max_ms  = 0;
    g_d12d520_count  = 0; g_d12d520_sum_ms  = 0; g_d12d520_max_ms  = 0;
    g_d12d480_count  = 0; g_d12d480_sum_ms  = 0; g_d12d480_max_ms  = 0;
    g_b387400_count  = 0; g_b387400_sum_ms  = 0; g_b387400_max_ms  = 0;
    g_b385cf0_count  = 0; g_b385cf0_sum_ms  = 0; g_b385cf0_max_ms  = 0;
    g_b381ff0_count  = 0; g_b381ff0_sum_ms  = 0; g_b381ff0_max_ms  = 0;
    g_flush_sum_ms = g_frame_sum_ms = g_inter_sum_ms = g_sim_sum_ms = g_pump_sum_ms = 0;
    g_flush_min_ms = g_frame_min_ms = g_inter_min_ms = g_sim_min_ms = g_pump_min_ms = 1e9;
    g_flush_max_ms = g_frame_max_ms = g_inter_max_ms = g_sim_max_ms = g_pump_max_ms = 0;
    for (int i = 0; i < N_IFC; i++) {
        g_ifc[i].count  = 0;
        g_ifc[i].sum_ms = 0;
        g_ifc[i].min_ms = 1e9;
        g_ifc[i].max_ms = 0;
    }
    for (int i = 0; i < 5; i++) {
        g_gap[i].count  = 0;
        g_gap[i].sum_ms = 0;
        g_gap[i].max_ms = 0;
    }
}

/* =========================================================================
 * Render thread
 * ========================================================================= */

static DWORD WINAPI render_thread_proc(LPVOID unused) {
    (void)unused;
    log_write("[eaw-mt] Render thread started\n");

    while (!InterlockedCompareExchange(&g_shutdown, 0, 0)) {
        DWORD wait = WaitForSingleObject(g_render_kick, 1000);
        if (wait == WAIT_TIMEOUT) continue;
        if (InterlockedCompareExchange(&g_shutdown, 0, 0)) break;

        QueryPerformanceCounter(&g_flush_start);
        /* Full flush+drain via trampoline (FUN_140180dc0 + FUN_140149650).
         * Main thread is blocked on render_done so there is no concurrent
         * access to render task lists or the allocator pools. */
        g_trampoline((void *)g_render_param);
        QueryPerformanceCounter(&g_flush_end);

        SetEvent(g_render_done);
    }

    log_write("[eaw-mt] Render thread exiting\n");
    return 0;
}

/* =========================================================================
 * Hook function — replaces FUN_140180d90 on the main thread call path
 * ========================================================================= */

static void WINAPI render_flush_hook(void *param_1) {
    /* Ensure sim tick vtable is hooked for this game mode */
    try_patch_sim_tick();

    LARGE_INTEGER entry_now;
    QueryPerformanceCounter(&entry_now);
    double freq = (double)g_qpc_freq.QuadPart / 1000.0;

    LONG serial   = InterlockedIncrement(&g_flush_serial);
    BOOL is_flush1 = (serial & 1) != 0;   /* odd = flush#1, even = flush#2 */

    /* Accumulate previous flush's round-trip time.  g_flush_end is safe to
     * read here because render_done was awaited on the preceding call.
     * Skip serial==1 which has no predecessor. */
    if (serial > 1) {
        double flush_ms = (g_flush_end.QuadPart - g_flush_start.QuadPart) / freq;
        g_flush_sum_ms += flush_ms;
        if (flush_ms < g_flush_min_ms) g_flush_min_ms = flush_ms;
        if (flush_ms > g_flush_max_ms) g_flush_max_ms = flush_ms;
        g_flush_count++;
    }

    if (is_flush1) {
        /* Flush#1 entry: accumulate frame time (flush#1→flush#1) and sim_tick. */
        if (g_frame1_anchor_valid) {
            double frame_ms = (entry_now.QuadPart - g_frame1_anchor.QuadPart) / freq;
            g_frame_sum_ms += frame_ms;
            if (frame_ms < g_frame_min_ms) g_frame_min_ms = frame_ms;
            if (frame_ms > g_frame_max_ms) g_frame_max_ms = frame_ms;

            if (g_sim_tick_valid) {
                double sim_ms = (g_sim_tick_end.QuadPart - g_sim_tick_start.QuadPart) / freq;
                g_sim_sum_ms += sim_ms;
                if (sim_ms < g_sim_min_ms) g_sim_min_ms = sim_ms;
                if (sim_ms > g_sim_max_ms) g_sim_max_ms = sim_ms;
                g_sim_tick_valid = FALSE;
            }

            g_frame_count++;
            if (g_frame_count == PROFILE_WINDOW)
                profile_report_and_reset();
        }
        g_frame1_anchor       = entry_now;
        g_frame1_anchor_valid = TRUE;
    } else {
        /* Flush#2 entry: accumulate inter-flush time (Pump_Threads et al.). */
        if (g_flush1_done_valid) {
            double inter_ms = (entry_now.QuadPart - g_flush1_done.QuadPart) / freq;
            g_inter_sum_ms += inter_ms;
            if (inter_ms < g_inter_min_ms) g_inter_min_ms = inter_ms;
            if (inter_ms > g_inter_max_ms) g_inter_max_ms = inter_ms;
        }
        if (g_gap_base_D_valid) {
            double ms = (entry_now.QuadPart - g_gap_base_D.QuadPart) / freq;
            g_gap[3].sum_ms += ms;
            if (ms > g_gap[3].max_ms) g_gap[3].max_ms = ms;
            g_gap[3].count++;
            g_gap_base_D_valid = FALSE;
        }
    }

    /* Kick render thread and block until this flush+drain completes. */
    g_render_param = param_1;
    SetEvent(g_render_kick);
    WaitForSingleObject(g_render_done, INFINITE);

    /* After the wait, g_flush_end is valid for THIS flush.
     * Record the inter-flush anchor now (time flush#1 finished). */
    if (is_flush1) {
        QueryPerformanceCounter(&g_flush1_done);
        g_flush1_done_valid = TRUE;
    }
}


/* =========================================================================
 * Inline hook installation
 * ========================================================================= */

/*
 * RVA of FUN_140180d90 (render flush wrapper, 32 bytes).
 * Verified 2026-04-19 via Phase4DumpBytes.java.
 *
 * Bytes 0-13 (position-independent, safe to relocate):
 *   40 53           PUSH RBX
 *   48 83 ec 20     SUB RSP,0x20
 *   ba ff 0b 00 00  MOV EDX,0xbff
 *   48 8b d9        MOV RBX,RCX
 */
#define RENDER_FLUSH_RVA  0x180d90ULL

static const BYTE saved_prologue[14] = {
    0x40, 0x53,                         /* PUSH RBX      */
    0x48, 0x83, 0xec, 0x20,             /* SUB  RSP,0x20 */
    0xba, 0xff, 0x0b, 0x00, 0x00,       /* MOV  EDX,0xbff */
    0x48, 0x8b, 0xd9                    /* MOV  RBX,RCX  */
};

/* Write a 14-byte absolute indirect JMP: FF 25 00 00 00 00 <8-byte addr> */
static void write_abs_jmp(BYTE *dst, uint64_t target) {
    dst[0] = 0xFF; dst[1] = 0x25;
    dst[2] = dst[3] = dst[4] = dst[5] = 0x00;  /* JMP [RIP+0] */
    memcpy(dst + 6, &target, 8);
}

static BOOL install_render_hook(void) {
    HMODULE exe = GetModuleHandleA(NULL);
    if (!exe) return FALSE;

    BYTE *flush_fn = (BYTE *)exe + RENDER_FLUSH_RVA;

    /* Sanity-check: verify the bytes we expect are actually there */
    if (memcmp(flush_fn, saved_prologue, sizeof(saved_prologue)) != 0) {
        log_write("[eaw-mt] FATAL: FUN_140180d90 bytes don't match — wrong binary version?\n");
        return FALSE;
    }

    /* Allocate executable trampoline memory */
    BYTE *tramp = (BYTE *)VirtualAlloc(NULL, 64,
        MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    if (!tramp) {
        log_write("[eaw-mt] FATAL: VirtualAlloc for trampoline failed\n");
        return FALSE;
    }

    /* Build trampoline: saved prologue + JMP to flush_fn+14 */
    memcpy(tramp, saved_prologue, 14);
    write_abs_jmp(tramp + 14, (uint64_t)(flush_fn + 14));

    g_trampoline = (RenderFlushFn)tramp;

    /* Patch flush_fn: write JMP to render_flush_hook */
    DWORD old_prot;
    if (!VirtualProtect(flush_fn, 14, PAGE_EXECUTE_READWRITE, &old_prot)) {
        log_write("[eaw-mt] FATAL: VirtualProtect failed\n");
        VirtualFree(tramp, 0, MEM_RELEASE);
        return FALSE;
    }

    write_abs_jmp(flush_fn, (uint64_t)render_flush_hook);

    VirtualProtect(flush_fn, 14, old_prot, &old_prot);

    /* Flush instruction cache so the CPU sees the patched bytes */
    FlushInstructionCache(GetCurrentProcess(), flush_fn, 14);

    log_write("[eaw-mt] Render hook installed at FUN_140180d90\n");
    return TRUE;
}

/* =========================================================================
 * DllMain
 * ========================================================================= */

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
    (void)lpvReserved;

    if (fdwReason == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(hinstDLL);
        g_main_thread_id = GetCurrentThreadId();
        g_log_fp = fopen("eaw-mt.log", "w");

        /* Load real winmm */
        real_winmm = LoadLibraryA("C:\\windows\\system32\\winmm.dll");
        if (!real_winmm) {
            log_write("[eaw-mt] FATAL: failed to load real winmm.dll\n");
            return FALSE;
        }
        real_timeEndPeriod   = (timeEndPeriod_t)   GetProcAddress(real_winmm, "timeEndPeriod");
        real_timeBeginPeriod = (timeBeginPeriod_t)  GetProcAddress(real_winmm, "timeBeginPeriod");
        real_timeGetDevCaps  = (timeGetDevCaps_t)  GetProcAddress(real_winmm, "timeGetDevCaps");
        real_timeGetTime     = (timeGetTime_t)      GetProcAddress(real_winmm, "timeGetTime");
        if (!real_timeEndPeriod || !real_timeBeginPeriod ||
            !real_timeGetDevCaps || !real_timeGetTime) {
            log_write("[eaw-mt] FATAL: failed to resolve winmm exports\n");
            return FALSE;
        }

        /* Initialize QPC frequency for profiling */
        QueryPerformanceFrequency(&g_qpc_freq);

        /* Create sync events (auto-reset) */
        g_render_kick = CreateEventA(NULL, FALSE, FALSE, NULL);
        g_render_done = CreateEventA(NULL, FALSE, FALSE, NULL);
        if (!g_render_kick || !g_render_done) {
            log_write("[eaw-mt] FATAL: CreateEvent failed\n");
            return FALSE;
        }

        /* Install inline hook (also builds g_trampoline) */
        if (!install_render_hook()) return FALSE;

        /* Install Pump_Threads call-site hook (non-fatal if scan fails) */
        install_pump_hook();

        /* Replace WaitMessage with 1ms-capped equivalent via IAT patch */
        install_wait_message_iat_hook();

        /* Hook CreateFileA/W + ReadFile to measure main-thread disk I/O */
        install_io_iat_hooks();

        /* Install inter-flush decomposition hooks (non-fatal) */
        install_inter_hooks();

        /* Install slot-22 tail call + sub-hooks (non-fatal) */
        install_slot22_hooks();

        /* Install mode-agnostic game service hook on FUN_14028d400 (non-fatal) */
        install_game_service_hook();

        /* Inline hook SpaceModeClass slot 22 (FUN_1404d95a0) — non-fatal */
        install_space_slot22_hook();

        /* Call-site hook FUN_140527dd0 inside FUN_1404d95a0 (non-fatal) */
        install_space_unit_svc_hook();

        /* Inline hook FUN_1403639d0 — tail call target from FUN_1404d95a0 (non-fatal) */
        install_tail22i_hook();

        /* E8 call-site hooks for FUN_14041a240 + FUN_14059d270 in FUN_1404d95a0 (non-fatal) */
        install_post_loop_hooks();

        /* E8 hooks inside FUN_1403639d0 body — 364920, 2be640, 490580 (non-fatal) */
        install_tail22_body_hooks();

        /* E8 hooks inside FUN_1402be640 body — 3a6b80, 2a62d0, 20ed70 (non-fatal) */
        install_2be640_body_hooks();

        /* Per-entity Pump_Threads call site inside FUN_1403a6b80 (non-fatal) */
        install_pumpe_hook();

        /* Second stall source hooks inside FUN_1403a6b80 — movement, cmd, preamble (non-fatal) */
        install_a6b80_stall2_hooks();

        /* Per-movement-component hook inside FUN_1403a76b0 (non-fatal) */
        install_b87010_hook();

        /* Phase 5 Step 13 — name→index lookup inside FUN_140385cf0 (non-fatal) */
        install_d12d520_hook();

        /* Phase 5 Step 13 — physics write inside FUN_140381ff0 (non-fatal) */
        install_d12d480_hook();

        /* Phase 5 Step 14 — sub-callee hooks inside FUN_140387010 (non-fatal) */
        install_b87010_subcallee_hooks();

        /* Hook FUN_14028a4d0 call-site inside FUN_14028d400 (non-fatal) */
        install_ga4d0_hook();

        /* Inline hook GalacticModeClass slot 22 (FUN_14045e030) — non-fatal */
        install_galactic_slot22_hook();

        /* Start render thread */
        HANDLE thread = CreateThread(NULL, 0, render_thread_proc, NULL, 0, NULL);
        if (!thread) {
            log_write("[eaw-mt] FATAL: CreateThread failed\n");
            return FALSE;
        }
        CloseHandle(thread);

        log_write("[eaw-mt] Model A render thread active (serialized)\n");
    }

    if (fdwReason == DLL_PROCESS_DETACH) {
        InterlockedExchange(&g_shutdown, 1);
        if (g_render_kick) SetEvent(g_render_kick);
        /* Restore vtable entries so the game doesn't call our hooks after unload */
        if (g_patched_vtable && g_saved_sim_tick) {
            DWORD old;
            VirtualProtect(&g_patched_vtable[SIM_TICK_VTBL_IDX], 8,
                           PAGE_EXECUTE_READWRITE, &old);
            g_patched_vtable[SIM_TICK_VTBL_IDX] = (void *)g_saved_sim_tick;
            VirtualProtect(&g_patched_vtable[SIM_TICK_VTBL_IDX], 8, old, &old);
            if (g_saved_slot22_vtbl) {
                VirtualProtect(&g_patched_vtable[SLOT22_VTBL_IDX], 8,
                               PAGE_EXECUTE_READWRITE, &old);
                g_patched_vtable[SLOT22_VTBL_IDX] = (void *)g_saved_slot22_vtbl;
                VirtualProtect(&g_patched_vtable[SLOT22_VTBL_IDX], 8, old, &old);
            }
        }
        if (real_winmm) FreeLibrary(real_winmm);
        if (g_log_fp) { fclose(g_log_fp); g_log_fp = NULL; }
    }

    return TRUE;
}
