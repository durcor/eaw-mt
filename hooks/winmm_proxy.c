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
#include <psapi.h>
#include <stdint.h>
#include <stdio.h>
#include <setjmp.h>

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
#define GAME_MODE_PTR_RVA        0xb15418ULL
#define SIM_TICK_VTBL_IDX        (0x158 / 8)    /* = 43 */
#define SLOT22_VTBL_IDX          (0x0b0 / 8)    /* = 22 — GalacticModeClass::Service (FUN_14045e030) */
/* FUN_14035cc10 — combat_mode_sim_tick.  vtable[43] must equal this for the
 * object at GAME_MODE_PTR_RVA to be SpaceModeClass (active combat mode).
 * During galactic mode or mid-transition, vtable[43]=0x4b1600 (wrong class)
 * and FUN_140180dc0 (render flush body) calls that slot from the render thread —
 * patching it causes the galactic sim tick to run on the render thread → crash. */
#define COMBAT_MODE_SIM_TICK_RVA 0x35cc10ULL

typedef void (WINAPI *SimTickFn)(void *, void *, void *, void *);
static SimTickFn  g_saved_sim_tick = NULL;
static void     **g_patched_vtable = NULL;

/* QPC frequency — set once at DLL init before any hook runs. */
static LARGE_INTEGER g_qpc_freq;

/* Drop-in replacement for QueryPerformanceCounter that reads KUSER_SHARED_DATA
 * (VDSO) via real_timeGetTime instead of going through Wine's syscall JIT thunk.
 * The JIT thunk checks pending wake-address notifications before each syscall;
 * a corrupted wake_addr pointer causes a write-AV at ~3000 QPC calls/second.
 * real_timeGetTime is safe: it reads a shared memory page, no kernel transition.
 * Scale: ms * (freq/1000) → QPC ticks, so existing delta/(freq/1000) math is
 * correct and all profiling output stays in the right units. */
static void tgt_fake_qpc(LARGE_INTEGER *li) {
    li->QuadPart = (int64_t)real_timeGetTime() * (g_qpc_freq.QuadPart / 1000LL);
}

/* Set in sim_tick_hook; read at flush#1 entry (same thread, no race). */
static LARGE_INTEGER g_sim_tick_start;
static LARGE_INTEGER g_sim_tick_end;
static BOOL          g_sim_tick_valid = FALSE;
/* Forward-declared here so try_patch_sim_tick can test it; set by space_slot22_hook. */
static volatile LONG g_space_mode_seen     = 0; /* counter: incremented by space_slot22_hook, reset by galactic_slot22_hook */
static volatile LONG g_galactic_ever_active = 0; /* set by galactic_slot22_hook; informational only */

static void WINAPI sim_tick_hook(void *a, void *b, void *c, void *d)
{
    tgt_fake_qpc(&g_sim_tick_start);
    if (g_saved_sim_tick)
        g_saved_sim_tick(a, b, c, d);
    tgt_fake_qpc(&g_sim_tick_end);
    g_sim_tick_valid = TRUE;
}

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
    tgt_fake_qpc(&t0);
    g_saved_slot22_vtbl(a, b, c, d);
    tgt_fake_qpc(&t1);
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
    /* DISABLED: reading *(void***)game_mode every render frame races against the
     * galactic→space transition when the game mode object is freed mid-frame,
     * causing a crash regardless of whether we actually patch.  The safe fix is
     * an inline hook on FUN_14035cc10 (combat_mode_sim_tick, RVA 0x35cc10) at
     * DLL init — no per-frame vtable reads needed.  Disabled until then; sim_tick
     * timing stats will read zero in the profile output. */
    return;

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

    /* Only patch when vtable[43] is combat_mode_sim_tick (FUN_14035cc10).
     * Any other value means DAT_140b15418 is not yet SpaceModeClass — skip. */
    void *expected_sim_tick = (BYTE *)exe + COMBAT_MODE_SIM_TICK_RVA;
    if (vtable[SIM_TICK_VTBL_IDX] != expected_sim_tick)
        return;

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
    {
        char s[128];
        int64_t vtbl_rva = (BYTE *)vtable - (BYTE *)exe;
        int64_t fn_rva   = (BYTE *)g_saved_sim_tick - (BYTE *)exe;
        sprintf(s, "[eaw-mt] Sim tick hook patched vtbl_rva=%llx fn_rva=%llx\n",
                (long long)vtbl_rva, (long long)fn_rva);
        log_write(s);
    }
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
/* lua_resume call site inside Pump_Threads — per-resume timing shim (Step 2/32) */
#define PUMPE_RESUME_CALL_RVA  0x247b56ULL  /* E8 CALL thunk_FUN_1407bc310 in Pump_Threads */
#define PUMPE_RESUME_THUNK_RVA 0x7b9310ULL  /* thunk: jmp lua_resume@0x7bc310 */

typedef void (__fastcall *PumpThreadsFn)(void *);
static PumpThreadsFn g_pump_threads_orig = NULL;

/* Per-window Pump_Threads accumulators — main thread only. */
static LONG   g_pump_count  = 0;
static double g_pump_sum_ms = 0, g_pump_min_ms = 1e9, g_pump_max_ms = 0;

static void __fastcall pump_threads_hook(void *param_1)
{
    LARGE_INTEGER t0, t1;
    tgt_fake_qpc(&t0);
    g_pump_threads_orig(param_1);
    tgt_fake_qpc(&t1);

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

static DWORD g_main_thread_id   = 0;
static DWORD g_render_thread_id = 0;
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
    tgt_fake_qpc(&t0);
    BOOL ok = real_ReadFile_fn(hFile, lpBuf, nToRead, lpRead, lpOv);
    tgt_fake_qpc(&t1);

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
    tgt_fake_qpc(&_t0); \
    (call); \
    tgt_fake_qpc(&_t1); \
    ifc_sample((idx), _t0, _t1); \
} while (0)

#define IFC_WRAP_R(ret, call, idx) do { \
    LARGE_INTEGER _t0, _t1; \
    tgt_fake_qpc(&_t0); \
    (ret) = (call); \
    tgt_fake_qpc(&_t1); \
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
    tgt_fake_qpc(&_t0);
    if (g_gap_base_E_valid) {
        double ms = (_t0.QuadPart - g_gap_base_E.QuadPart) / ((double)g_qpc_freq.QuadPart / 1000.0);
        g_gap[4].sum_ms += ms;
        if (ms > g_gap[4].max_ms) g_gap[4].max_ms = ms;
        g_gap[4].count++;
        g_gap_base_E_valid = FALSE;
    }
    g_ifc8(a, b, c, d);
    tgt_fake_qpc(&_t1);
    ifc_sample(8, _t0, _t1);
}
static void ifc_hook9 (int64_t a,int64_t b,int64_t c,int64_t d){
    LARGE_INTEGER _t0, _t1;
    tgt_fake_qpc(&_t0);
    if (g_gap_base_A_valid) {
        double ms = (_t0.QuadPart - g_gap_base_A.QuadPart) / ((double)g_qpc_freq.QuadPart / 1000.0);
        g_gap[0].sum_ms += ms;
        if (ms > g_gap[0].max_ms) g_gap[0].max_ms = ms;
        g_gap[0].count++;
        g_gap_base_A_valid = FALSE;
    }
    g_ifc9(a,b,c,d);
    tgt_fake_qpc(&_t1);
    ifc_sample(9, _t0, _t1);
}
static void ifc_hook10(int64_t a,int64_t b,int64_t c,int64_t d){IFC_WRAP(g_ifc10(a,b,c,d),10);}
static void ifc_hook11(int64_t a,int64_t b,int64_t c,int64_t d){IFC_WRAP(g_ifc11(a,b,c,d),11);}
static void ifc_hook12(int64_t a,int64_t b,int64_t c,int64_t d){
    LARGE_INTEGER _t0, _t1;
    tgt_fake_qpc(&_t0);
    g_ifc12(a,b,c,d);
    tgt_fake_qpc(&_t1);
    ifc_sample(12, _t0, _t1);
    g_gap_base_B = _t1;
    g_gap_base_B_valid = TRUE;
}
static void ifc_hook13(int64_t a,int64_t b,int64_t c,int64_t d){
    LARGE_INTEGER _t0, _t1;
    tgt_fake_qpc(&_t0);
    if (g_gap_base_B_valid) {
        double ms = (_t0.QuadPart - g_gap_base_B.QuadPart) / ((double)g_qpc_freq.QuadPart / 1000.0);
        g_gap[1].sum_ms += ms;
        if (ms > g_gap[1].max_ms) g_gap[1].max_ms = ms;
        g_gap[1].count++;
        g_gap_base_B_valid = FALSE;
    }
    g_ifc13(a,b,c,d);
    tgt_fake_qpc(&_t1);
    ifc_sample(13, _t0, _t1);
    g_gap_base_C = _t1;
    g_gap_base_C_valid = TRUE;
}
static void ifc_hook14(int64_t a,int64_t b,int64_t c,int64_t d){IFC_WRAP(g_ifc14(a,b,c,d),14);}
static int64_t ifc_hook15(void) {
    LARGE_INTEGER _t0, _t1;
    tgt_fake_qpc(&_t0);
    if (g_gap_base_C_valid) {
        double ms = (_t0.QuadPart - g_gap_base_C.QuadPart) / ((double)g_qpc_freq.QuadPart / 1000.0);
        g_gap[2].sum_ms += ms;
        if (ms > g_gap[2].max_ms) g_gap[2].max_ms = ms;
        g_gap[2].count++;
        g_gap_base_C_valid = FALSE;
    }
    int64_t r = g_ifc15();
    tgt_fake_qpc(&_t1);
    ifc_sample(15, _t0, _t1);
    return r;
}
static void ifc_hook16(int64_t a,int64_t b,int64_t c,int64_t d){IFC_WRAP(g_ifc16(a,b,c,d),16);}
static void ifc_hook17(int64_t a, int64_t b, int64_t c, int64_t d) {
    LARGE_INTEGER _t0, _t1;
    tgt_fake_qpc(&_t0);
    g_ifc17(a, b, c, d);
    tgt_fake_qpc(&_t1);
    ifc_sample(17, _t0, _t1);
    g_gap_base_E = _t1;
    g_gap_base_E_valid = TRUE;
}
static void ifc_hook18(int64_t a,int64_t b,int64_t c,int64_t d){IFC_WRAP(g_ifc18(a,b,c,d),18);}
static void ifc_hook19(int64_t a,int64_t b,int64_t c,int64_t d){
    LARGE_INTEGER _t0, _t1;
    tgt_fake_qpc(&_t0);
    g_ifc19(a,b,c,d);
    tgt_fake_qpc(&_t1);
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
    tgt_fake_qpc(&_t0);
    g_ifc24(a,b,c,d);
    tgt_fake_qpc(&_t1);
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
#define GA4D0_SIZE           150          /* body size for internal E8 scan */
#define A7190_RVA            0x2a7190ULL  /* FUN_1402a7190 — called per-entry by FUN_14028a4d0; crashes if arg=0 */
#define GAE40_RVA            0x28ae40ULL  /* FUN_14028ae40 — reads battle-mode/thread entries after ai_init */
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
    /* Mark galactic active; reset the space-mode counter so pumpe stays sync while
     * the galactic loop is running.  Counter reaching 0 here also defeats any
     * spurious space_mode_seen accumulated during galactic init. */
    InterlockedExchange(&g_galactic_ever_active, 1);
    InterlockedExchange(&g_space_mode_seen, 0);
    LARGE_INTEGER t0, t1;
    tgt_fake_qpc(&t0);
    g_gslot22_trampoline(a, b, c, d);
    tgt_fake_qpc(&t1);
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
/* g_space_mode_seen is declared near try_patch_sim_tick (above) and set here
 * on first space_slot22_hook call to confirm space mode is active. */

static void space_slot22_hook(int64_t a, int32_t b)
{
    LARGE_INTEGER t0, t1;
    tgt_fake_qpc(&t0);
    InterlockedIncrement(&g_space_mode_seen);
    g_sslot22_trampoline(a, b);
    tgt_fake_qpc(&t1);
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
    tgt_fake_qpc(&t0);
    g_sunit22_orig(a, b);
    tgt_fake_qpc(&t1);
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
#define LUAC_STEP_RVA     0x7bc850ULL  /* FUN_1407bc850 — luaC_step (GC runner, called by luaC_checkGC) */
#define LUAC_FULLGC_RVA   0x7bcb20ULL  /* FUN_1407bcb20 — luaC_fullgc (full sweep; collectgarbage path) */
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
    tgt_fake_qpc(&t0);
    g_tail22i_trampoline(a, b);
    tgt_fake_qpc(&t1);
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
    tgt_fake_qpc(&t0);
    void *ret = g_a1a240_orig();
    tgt_fake_qpc(&t1);
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
    tgt_fake_qpc(&t0);
    g_s9d270_orig(a);
    tgt_fake_qpc(&t1);
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
    tgt_fake_qpc(&t0);
    g_t364920_orig(a, b);
    tgt_fake_qpc(&t1);
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
    tgt_fake_qpc(&t0);
    g_t2be640_orig(a, b);
    tgt_fake_qpc(&t1);
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
    tgt_fake_qpc(&t0);
    g_t490580_orig(a);
    tgt_fake_qpc(&t1);
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
    tgt_fake_qpc(&t0);
    g_ta6b80_orig(a, b, c);
    tgt_fake_qpc(&t1);
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
    tgt_fake_qpc(&t0);
    g_ta62d0_orig(a);
    tgt_fake_qpc(&t1);
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
    tgt_fake_qpc(&t0);
    g_t20ed70_orig(a);
    tgt_fake_qpc(&t1);
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
static LONG    g_pumpe_skip   = 0;   /* calls skipped (budget or BG busy) */
static LONG    g_pumpe_bg_dispatched = 0; /* calls dispatched to BG thread */
static double  g_pumpe_sum_ms = 0, g_pumpe_max_ms = 0;
static double  g_pumpe_bg_sum_ms = 0, g_pumpe_bg_max_ms = 0;

/* Per-gsvc-call Lua AI time budget.  Prevents stall accumulation when multiple
 * entities run slow Lua coroutines in the same service dispatch.
 * Reset by game_service_hook at the start of each gsvc call. */
#define PUMPE_BUDGET_MS   33.0
static double g_pumpe_frame_used_ms = 0.0;

/* Per-call deadline (Step 2): measure how often Pump_Threads calls exceed
 * PUMP_DEADLINE_MS.  Watchdog is measure-only — writing the abort flag at
 * *(param1+0x121) triggers Pump_Threads' abort-cleanup path, which writes
 * to a corrupted wake_addr (0x6fff00000000) and cascades into an ntdll
 * null-ptr crash (av_write@8 in ntdll list-insertion code).
 * The per-gsvc budget (PUMPE_BUDGET_MS) is the actual stall guard. */
#define PUMP_DEADLINE_MS  4
static volatile LONG  g_wd_armed    = 0;   /* 1 = watchdog timing window active */
static volatile DWORD g_wd_deadline = 0;   /* timeGetTime() deadline */
static volatile LONG  g_wd_fired    = 0;   /* diagnostic: pump-calls that exceeded deadline (watchdog) */

/* Per-resume stats fed by pumpe_lua_resume_shim (reset each stats period) */
static volatile LONG  g_resume_n    = 0;   /* total lua_resume calls */
static volatile LONG  g_resume_slow = 0;   /* resumes exceeding PUMP_DEADLINE_MS */
static volatile LONG  g_resume_max_ms = 0; /* max single resume duration (ms) */

/* Deferred prewarm (Step 29e): ai_manager saved during loading, fired from
 * pumpe_hook once g_pumpe_count > 0 (battle game loop running, Lua AI safe). */
static volatile int64_t g_deferred_ai_mgr = 0;
static volatile int64_t g_prewarm_lua_state = 0; /* lua_state of last prewarm'd entity */
static void prewarm_from_ai_manager(int64_t); /* forward decl — defined below */

/* VEH / prewarm SEH-recovery globals — declared here so prewarm_from_ai_manager
 * can reference them; the VEH handler (defined later) also uses them. */
static volatile LONG g_veh_entered        = 0;
static volatile LONG g_wake_suppressed    = 0;
static volatile LONG g_in_prewarm_pumpe   = 0;
static volatile LONG g_prewarm_jmp_valid  = 0;
static jmp_buf        g_prewarm_jmp;

/* Worker thread: one background Lua-AI thread at a time.  When an entity's
 * lua_resume is dispatched here, the main thread returns immediately and the
 * 14s init stall becomes invisible to the frame loop.
 *
 * Safety: each entity has an independent lua_State (entity+0x2d8).  After
 * pumpe_hook returns without calling g_pumpe_orig, FUN_1403a6b80 only reads
 * entity_manager+0x121 (done flag) and writes LastService timestamp — neither
 * field is written by the BG thread until after lua_resume completes.
 * If the main thread arrives for the same entity again while BG is running,
 * g_pumpe_bg_running blocks the call (skip), preventing double-resume. */
static volatile LONG g_pumpe_bg_running = 0;

typedef struct { int64_t a; LARGE_INTEGER t0; } PumpeWorkItem;

static DWORD WINAPI pumpe_bg_worker(LPVOID arg)
{
    PumpeWorkItem *item = (PumpeWorkItem *)arg;
    int64_t a = item->a;
    LARGE_INTEGER t0 = item->t0, t1;
    free(item);

    g_pumpe_orig(a);

    tgt_fake_qpc(&t1);
    double ms = (t1.QuadPart - t0.QuadPart) / ((double)g_qpc_freq.QuadPart / 1000.0);
    g_pumpe_bg_sum_ms += ms;
    if (ms > g_pumpe_bg_max_ms) g_pumpe_bg_max_ms = ms;

    if (ms >= 100.0) {
        char s[80];
        sprintf(s, "[eaw-mt] PUMPE-BG %.1fms (entity Lua AI, background)\n", ms);
        log_write(s);
    }

    InterlockedExchange(&g_pumpe_bg_running, 0);
    return 0;
}

static volatile LONG g_post_prewarm_log = 0; /* arms after prewarm fires */

/* Watchdog thread for per-call deadline measurement (Step 2).
 * Polls every 1ms; counts calls that run past g_wd_deadline.
 * Does NOT write the abort flag — see PUMP_DEADLINE_MS comment above. */
static DWORD WINAPI pump_watchdog_thread(LPVOID unused)
{
    (void)unused;
    for (;;) {
        Sleep(1);
        if (!g_wd_armed) continue;
        DWORD now = real_timeGetTime();
        /* unsigned subtraction wraps correctly across DWORD rollover */
        if ((DWORD)(now - g_wd_deadline) < 0x80000000UL)
            InterlockedIncrement(&g_wd_fired);
    }
}

/* -------------------------------------------------------------------------
 * lua_resume call-site shim (Phase 5 Step 2 / Step 32)
 *
 * Patches the single CALL at RVA 0x247b56 inside Pump_Threads that invokes
 * the lua_resume thunk.  Measures exact per-coroutine resume latency — no
 * 1ms poll latency — and feeds g_resume_slow / g_resume_max_ms for the
 * stats log.  The watchdog thread (wd=) continues to cover the pump-call
 * level; rv_* fields cover the per-resume level.
 * ------------------------------------------------------------------------- */
typedef int (*LuaResumeFn)(void *L, void *cont_flag, int nargs);
static LuaResumeFn g_lua_resume_thunk_orig = NULL;

static int pumpe_lua_resume_shim(void *L, void *cont_flag, int nargs)
{
    DWORD t0 = real_timeGetTime();
    int result = g_lua_resume_thunk_orig(L, cont_flag, nargs);
    DWORD elapsed = real_timeGetTime() - t0;

    /* Update max (lock-free CAS loop). */
    LONG cur;
    do {
        cur = g_resume_max_ms;
        if ((LONG)elapsed <= cur) break;
    } while (InterlockedCompareExchange(&g_resume_max_ms, (LONG)elapsed, cur) != cur);

    InterlockedIncrement(&g_resume_n);
    if (elapsed >= (DWORD)PUMP_DEADLINE_MS)
        InterlockedIncrement(&g_resume_slow);

    return result;
}

static BOOL install_pumpe_resume_shim(void)
{
    HMODULE exe = GetModuleHandleA(NULL);
    if (!exe) return FALSE;

    BYTE *call_site = (BYTE *)exe + PUMPE_RESUME_CALL_RVA;

    if (call_site[0] != 0xE8) {
        log_write("[eaw-mt] WARN: pumpe resume shim: unexpected opcode at call site\n");
        return FALSE;
    }
    int32_t existing_rel;
    memcpy(&existing_rel, call_site + 1, 4);
    BYTE *expected_thunk = (BYTE *)exe + PUMPE_RESUME_THUNK_RVA;
    if (call_site + 5 + existing_rel != expected_thunk) {
        log_write("[eaw-mt] WARN: pumpe resume shim: call target mismatch\n");
        return FALSE;
    }

    g_lua_resume_thunk_orig = (LuaResumeFn)expected_thunk;

    BYTE *stub = alloc_near(call_site, 14);
    if (!stub) {
        log_write("[eaw-mt] WARN: alloc_near failed for pumpe resume shim stub\n");
        return FALSE;
    }
    write_abs_jmp(stub, (uint64_t)pumpe_lua_resume_shim);

    int32_t new_rel = (int32_t)(stub - (call_site + 5));
    DWORD old;
    VirtualProtect(call_site + 1, 4, PAGE_EXECUTE_READWRITE, &old);
    memcpy(call_site + 1, &new_rel, 4);
    VirtualProtect(call_site + 1, 4, old, &old);
    FlushInstructionCache(GetCurrentProcess(), call_site, 5);

    log_write("[eaw-mt] pumpe lua_resume shim installed (rva=0x247b56 -> thunk@7b9310)\n");
    return TRUE;
}

static void pumpe_hook(int64_t a)
{
    static LONG g_pumpe_once = 0;
    if (InterlockedCompareExchange(&g_pumpe_once, 1, 0) == 0) {
        log_write("[eaw-mt] DBG: pumpe_hook first call\n");
        /* Create watchdog thread here, not in DllMain: CreateThread inside
         * DllMain holds the loader lock and can deadlock Wine's DLL loader. */
        HANDLE wdh = CreateThread(NULL, 0, pump_watchdog_thread, NULL, 0, NULL);
        if (wdh) { CloseHandle(wdh); log_write("[eaw-mt] pumpe watchdog thread started (deadline=4ms)\n"); }
        else      { log_write("[eaw-mt] WARN: failed to start pumpe watchdog thread\n"); }
    }

    /* Post-prewarm trace: log first N calls so we know the game loop resumed. */
    LONG rem = InterlockedCompareExchange(&g_post_prewarm_log, 0, 0);
    if (rem > 0) {
        InterlockedDecrement(&g_post_prewarm_log);
        char _tr[96];
        sprintf(_tr, "[eaw-mt] DBG: pumpe_hook post-prewarm rem=%ld a=0x%llx\n",
                (long)rem, (unsigned long long)a);
        log_write(_tr);
    }

    /* BG dispatch disabled (Step 30): EaW's Lua C closures access shared game
     * state (entity lists, resource counts, vtable objects).  Running lua_resume
     * on a background thread while the main thread reads the same state causes
     * main-thread vtable corruption → crash (confirmed: av_read@heap addr).
     *
     * Per-gsvc budget still applies: skips remaining entities once the cumulative
     * synchronous time exceeds PUMPE_BUDGET_MS for this service pass. */
    if (g_pumpe_frame_used_ms >= PUMPE_BUDGET_MS) {
        InterlockedIncrement(&g_pumpe_skip);
        return;
    }

    /* Watchdog: arm for timing measurement only (no abort flag write).
     * Writing *(a+0x121)=1 causes Pump_Threads to take an abort-cleanup
     * path that writes to corrupted wake_addr → ntdll null-ptr crash. */
    LARGE_INTEGER t0, t1;
    tgt_fake_qpc(&t0);

    g_wd_deadline = real_timeGetTime() + PUMP_DEADLINE_MS;
    InterlockedExchange(&g_wd_armed, 1);

    g_pumpe_orig(a);

    InterlockedExchange(&g_wd_armed, 0);

    tgt_fake_qpc(&t1);
    double ms = (t1.QuadPart - t0.QuadPart) / ((double)g_qpc_freq.QuadPart / 1000.0);
    g_pumpe_frame_used_ms += ms;
    g_pumpe_sum_ms += ms;
    if (ms > g_pumpe_max_ms) g_pumpe_max_ms = ms;
    g_pumpe_count++;
    if (ms >= 100.0) {
        int64_t pw = g_prewarm_lua_state;
        char s[96];
        sprintf(s, "[eaw-mt] PUMPE %.1fms (entity Lua AI, synchronous) lua=%llx prewarm=%d\n",
                ms, (unsigned long long)a, (pw && a == pw) ? 1 : 0);
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
 * Pre-warm hook — FUN_140052d10 (level-load) callsite in WinMain (Phase 5 Step 29)
 *
 * Problem: Pump_Threads (FUN_140247a90) stalls ~21s on its first call because
 * lua_resume initialises Lua state cold.  Even with the BG-thread dispatch from
 * Step 28, that init cost blocks the BG thread for ~21s at battle-start.
 *
 * Solution: after FUN_140052d10 returns (level loaded, DAT_140b15418 set,
 * entity AI coroutines created via FUN_1402a8730→FUN_140394510→vtable+0x68),
 * call Pump_Threads synchronously for every AI entity.  This moves the 21s
 * Lua init cost to the loading screen where the user already expects to wait.
 *
 * Hook point: FUN_1402a8730 (rva=0x2a8730) — entity/AI manager init.
 * Called from every battle loading path:
 *   FUN_140052d10 @ 0x53659  — save-game / pre-loop WinMain path
 *   FUN_140056480 @ 0x56d11  — skirmish / interactive battle path
 *   FUN_14008df00 @ 0x8e4e6  — galactic conquest space battle path
 *
 * FUN_1402a8730(param_1=ai_manager): iterates entity linked list, dispatches
 * per-entity service vtable+0x68 → FUN_1406cacd0 → coroutine CREATE path.
 * After it returns, entity AI coroutines exist; calling Pump_Threads for each
 * entity pre-warms the Lua state so battle-frame-1 sees ~1ms not ~14s.
 *
 * Entity enumeration (from FUN_1402a8730 decompile):
 *   sentinel   = ai_manager + 0x40
 *   node       = *(ai_manager + 0x48)         // linked list head
 *   entity     = *(node + 0x18) - 0x18        // struct base
 *   lua_state  = *(entity + 0x2d8)            // entity[0x5b]
 *
 * Pump_Threads argument confirmed: phase5_3a6b80.c:304
 *   FUN_140247a90(param_1[0x5b])  (param_1 = entity ptr, [0x5b] = +0x2d8)
 * ========================================================================= */
#define AI_INIT_RVA      0x2a8730ULL  /* FUN_1402a8730 — entity AI manager init */
/* Known callers of FUN_1402a8730 and their scan windows: */
#define FN_52D10_RVA     0x52d10ULL   /* FUN_140052d10 — save/pre-loop loader */
#define FN_52D10_SCAN    2500         /* callsite at offset +0x949 (0x53659); was 1100 — too small */
#define FN_56480_RVA     0x56480ULL   /* FUN_140056480 — skirmish loader */
#define FN_56480_SCAN    2300         /* covers fn+0x8c2 (28ae40 callsite at offset 0x8c2 = 2242) */
#define FN_8DF00_RVA     0x8df00ULL   /* FUN_14008df00 — galactic conquest loader */
#define FN_8DF00_SCAN    1600         /* callsite at offset +0x5e6 (0x8e4e6) */
#define BMA_RVA          0xb153e0ULL  /* &DAT_140b153e0 — battle-mode array object passed to FUN_14028a4d0 */

/* FUN_1407bfd90 (luaH_get, Table lookup by TValue key):
 *   0x7bfdf0  CMP dword [RDX], 4   ← crashes when node->next is a garbage non-canonical ptr
 *   0x7bfe04  LEA RAX, [not_found_sentinel]  ← "not found" return path
 *
 * The prewarm pump leaves garbage node->next pointers in live Lua Tables because
 * the major GC resizes tables and our restore only patches the custom EaW string-intern
 * map, not every Lua Table.  VEH handler detects a fault at the loop head and redirects
 * RIP to the "not found" return so the caller re-inserts with fresh pointers. */
#define LUAH_GET_LOOP_RVA    0x7bfdf0ULL  /* loop head — CMP dword [RDX],4 */
#define LUAH_GET_NOTFND_RVA  0x7bfe04ULL  /* "not found" return target */


/* FUN_14020a8c0 (RVA 0x20a8c0) — galactic-mode entity-relationship teardown.
 * First real instruction after prologue reads [RCX+8] (= param_1+8, the list
 * head pointer).  If param_1 is null (pre-existing game bug, observed after
 * 4+ battles), RCX=0 and the read faults.  Recovery: set RAX=0 and skip 4
 * bytes so the existing "test rax,rax / je epilogue" path returns cleanly. */
#define GALTEAR_NULL_RVA     0x20a8c6ULL  /* MOV RAX,[RCX+8] — param_1 null guard */

typedef void (*AiInitFn)(int64_t);
static AiInitFn  g_ai_init_orig      = NULL;
static int64_t   g_bma_obj           = 0;  /* &DAT_140b153e0 — battle-mode array object */

/* Snapshot and restore the battle-mode array field *(lVar2+0x20) that Lua AI
 * callbacks zero during prewarm.  FUN_14028a4d0 crashes if it reads a null
 * there; restoring the value lets the game loop's first ga4d0 call succeed.
 * Uses the same array layout that FUN_14028a4d0 iterates:
 *   array_base = *(bma_obj+0x48),  count = *(bma_obj+0x50)
 *   for each i: lVar2 = array_base[i]; guard: lVar2!=0 && *(lVar2+0x18)!=0
 *   protected field: *(lVar2+0x20) */

/* --- Arena-backed allocator for cap<4096 prewarm (Step 29j) -------------------
 *
 * cap=1024 entities crash the game's custom Lua allocator mid-allocation
 * (NULL+8 write in its internal bookkeeping).  The allocator holds a heap
 * critical section at the crash site; longjmp bypasses the release and
 * deadlocks all subsequent allocs (save-load freeze, etc.).
 *
 * Fix: replace the entity's Lua allocator (global_State->frealloc, hm+0x10)
 * with a bump-pointer arena backed by VirtualAlloc for the pump duration.
 * No heap CS is held, so longjmp is safe.  On success, install a hybrid
 * allocator that routes new allocs to the original fn and no-ops frees/
 * reallocs of arena-backed pointers (arena stays alive to back Lua objects
 * created during the pump).  On crash, restore the original allocator and
 * VirtualFree the arena. */

typedef void *(*lua_Alloc_t)(void *, void *, size_t, size_t);

typedef struct {
    void       *arena_base;
    size_t      arena_size;
    lua_Alloc_t orig_fn;
    void       *orig_ud;
} HybridAllocCtx;

static HybridAllocCtx g_hybrid_ctx;
static LPVOID         g_prewarm_arena      = NULL;
static size_t         g_prewarm_arena_used = 0;
static size_t         g_prewarm_arena_size = 0;

/* Pure bump-pointer allocator used ONLY during the arena-pump window. */
static void *arena_lua_alloc(void *ud, void *ptr, size_t osize, size_t nsize)
{
    (void)ud; (void)osize;
    if (nsize == 0) return NULL;
    size_t offset = (g_prewarm_arena_used + 7u) & ~(size_t)7u;
    if (!g_prewarm_arena || offset + nsize > g_prewarm_arena_size) return NULL;
    void *p = (char *)g_prewarm_arena + offset;
    if (ptr && osize > 0)
        memcpy(p, ptr, osize < nsize ? osize : nsize);
    g_prewarm_arena_used = offset + nsize;
    return p;
}

/* Hybrid allocator installed permanently after a successful arena pump.
 * Arena-range pointers: free → no-op, realloc → copy to original heap.
 * All other operations pass through to the original fn. */
static void *hybrid_lua_alloc(void *ud, void *ptr, size_t osize, size_t nsize)
{
    HybridAllocCtx *ctx = (HybridAllocCtx *)ud;
    if (ptr) {
        uintptr_t p    = (uintptr_t)ptr;
        uintptr_t base = (uintptr_t)ctx->arena_base;
        if (p >= base && p < base + ctx->arena_size) {
            if (nsize == 0) return NULL;
            void *np = ctx->orig_fn(ctx->orig_ud, NULL, 0, nsize);
            if (np && osize > 0) memcpy(np, ptr, osize < nsize ? osize : nsize);
            return np;
        }
    }
    return ctx->orig_fn(ctx->orig_ud, ptr, osize, nsize);
}
/* ------------------------------------------------------------------------------ */

static void prewarm_from_ai_manager(int64_t ai_manager)
{
    if (!ai_manager || !g_pumpe_orig) return;

    /* Snapshot battle-mode array *(lVar2+0x20) before running Lua */
    int64_t bma_arr  = g_bma_obj ? *(int64_t *)(g_bma_obj + 0x48) : 0;
    int32_t bma_cnt  = g_bma_obj ? *(int32_t *)(g_bma_obj + 0x50) : 0;
    if (bma_cnt < 0 || bma_cnt > 256) bma_cnt = 0;
    int64_t bma_saved[256];
    int8_t  bma_has[256];
    memset(bma_has, 0, sizeof(bma_has));
    for (int i = 0; i < bma_cnt; i++) {
        int64_t lv2 = bma_arr ? *(int64_t *)(bma_arr + i*8) : 0;
        if (!lv2 || !*(int64_t *)(lv2 + 0x18)) continue;
        bma_saved[i] = *(int64_t *)(lv2 + 0x20);
        bma_has[i]   = 1;
    }

    int64_t sentinel = ai_manager + 0x40;
    int64_t node     = *(int64_t *)(ai_manager + 0x48);
    int count = 0, pumped = 0;
    int major_gc_happened = 0;
    int64_t pumped_hm = 0;  /* hm of the entity we calloc'd in first pass */

    LARGE_INTEGER t0, t1;
    QueryPerformanceCounter(&t0);

    while (node != sentinel && count < 2048) {
        int64_t ref = *(int64_t *)(node + 0x18);
        if (ref) {
            int64_t entity    = ref - 0x18;
            int64_t lua_state = *(int64_t *)(entity + 0x2d8);
            /* Mirror FUN_1403a6b80's Pump_Threads guard:
             *   (entity[0x5b] != 0) && !(entity[0x74] & 2)
             * entity[0x74] in longlong* terms = byte at entity+0x3a0.
             * Bit 1 set = entity service is disabled/dead; skip pumping. */
            uint8_t eflags = *(uint8_t *)(entity + 0x3a0);
            if (lua_state && !(eflags & 2)) {
                /* Save the service message buffer state before prewarm.
                 *
                 * lua_state+0x58 → buf  — outer pointer to the message buffer object.
                 * buf+0x10       → wr   — linear write pointer into the payload area.
                 * buf+0x20       → hm   — pointer to the string-interning hash map.
                 * hm+0x00        → arr  — pointer to the hash map's bucket array.
                 * hm+0x08        → cnt  — element count.
                 * hm+0x0c        → cap  — bucket capacity (int32).
                 *
                 * Pump_Threads drives the Lua AI which calls FUN_140246fb0 (property
                 * lookup) → FUN_1407b9540 (append string to buffer) → FUN_1407bf8f0
                 * (intern string in hash map).  After the query executes, some cleanup
                 * writes 0xffffffffffffffef (a freed-slot sentinel) into the `next`
                 * fields of hash map entries while leaving the bucket array pointers
                 * intact.  The game loop's first ServiceRate lookup then traverses the
                 * stale chain and crashes (AV at 0xffffffffffffffff).
                 *
                 * Fix: restore ALL hash map structural fields and zero the bucket array
                 * so the game loop sees a clean empty table and re-interns from scratch.
                 */
                int64_t buf      = *(int64_t *)(lua_state + 0x58);
                int64_t hm       = buf ? *(int64_t *)(buf + 0x20) : 0;
                int32_t cap      = hm  ? *(int32_t *)(hm  + 0x0c) : 0;
                int64_t arr_pre  = hm  ? *(int64_t *)(hm  + 0x00) : 0;
                int64_t hm08_pre = hm  ? *(int64_t *)(hm  + 0x08) : 0;
                int32_t hm70_pre = hm  ? *(int32_t *)(hm  + 0x70) : 0;
                int32_t hm74_pre = hm  ? *(int32_t *)(hm  + 0x74) : 0;
                int64_t hm38_pre = hm  ? *(int64_t *)(hm  + 0x38) : 0;
                int64_t hm48_pre = hm  ? *(int64_t *)(hm  + 0x48) : 0;
                int64_t hm10_pre = hm  ? *(int64_t *)(hm  + 0x10) : 0; /* frealloc fn */
                int64_t hm18_pre = hm  ? *(int64_t *)(hm  + 0x18) : 0; /* alloc ud */

                if (cap <= 0) {
                    /* no valid string-intern hash map — skip silently */
                } else if (cap < 4096) {
                    /* cap=1024 (and similar) entities are skipped.  Their Lua AI
                     * issues side-effecting C callbacks (fog-of-war writes, entity
                     * spawn, etc.) that modify game state outside the Lua VM; there
                     * is no way to undo those modifications after the pump without
                     * a full game-state snapshot.  The one-time 33s first-encounter
                     * stall is acceptable; OS page cache makes subsequent encounters
                     * fast.  The arena infrastructure (HybridAllocCtx, arena_lua_alloc,
                     * hybrid_lua_alloc) remains compiled in for future investigation. */
                    char _sk[128];
                    sprintf(_sk, "[eaw-mt] PREWARM: skip entity %d cap=%d"
                            " hm70=%d hm74=%d (side-effects unsafe)\n",
                            count, cap, hm70_pre, hm74_pre);
                    log_write(_sk);
                } else {
                    char _pm[256];
                    sprintf(_pm, "[eaw-mt] PREWARM: pumping entity %d  lua=0x%llx buf=0x%llx hm=0x%llx"
                            "  cap=%d hm70=%d hm74=%d\n",
                            count, (unsigned long long)lua_state,
                            (unsigned long long)buf, (unsigned long long)hm,
                            (int)cap, (int)hm70_pre, (int)hm74_pre);
                    log_write(_pm);

                    /* RET-patch luaC_step (FUN_1407bc850) and luaC_fullgc (FUN_1407bcb20)
                     * for the duration of pumpe.
                     *
                     * luaC_step is called by every luaC_checkGC site (incremental GC).
                     * luaC_fullgc is called by collectgarbage("collect") via lua_gc —
                     * it bypasses the threshold check entirely and sweeps everything.
                     * Both must be silenced or the full GC frees TString objects that
                     * live tables still reference, corrupting hash chains (LUAH_NOKEY). */
                    HMODULE _exe_pw = GetModuleHandleA(NULL);
                    BYTE *_luac_step   = _exe_pw ? (BYTE*)_exe_pw + LUAC_STEP_RVA   : NULL;
                    BYTE *_luac_fullgc = _exe_pw ? (BYTE*)_exe_pw + LUAC_FULLGC_RVA : NULL;
                    BYTE  _luac_saved = 0,  _fullgc_saved = 0;
                    int   _luac_patched = 0, _fullgc_patched = 0;
                    if (_luac_step) {
                        DWORD _old_prot;
                        if (VirtualProtect(_luac_step, 1, PAGE_EXECUTE_READWRITE, &_old_prot)) {
                            _luac_saved   = *_luac_step;
                            *_luac_step   = 0xC3;
                            VirtualProtect(_luac_step, 1, _old_prot, &_old_prot);
                            FlushInstructionCache(GetCurrentProcess(), _luac_step, 1);
                            _luac_patched = 1;
                            char _lp[80];
                            sprintf(_lp, "[eaw-mt] PREWARM: luaC_step RET-patched (saved=0x%02x)\n",
                                    (unsigned)_luac_saved);
                            log_write(_lp);
                        }
                    }
                    if (_luac_fullgc) {
                        DWORD _old_prot;
                        if (VirtualProtect(_luac_fullgc, 1, PAGE_EXECUTE_READWRITE, &_old_prot)) {
                            _fullgc_saved   = *_luac_fullgc;
                            *_luac_fullgc   = 0xC3;
                            VirtualProtect(_luac_fullgc, 1, _old_prot, &_old_prot);
                            FlushInstructionCache(GetCurrentProcess(), _luac_fullgc, 1);
                            _fullgc_patched = 1;
                            char _lp[80];
                            sprintf(_lp, "[eaw-mt] PREWARM: luaC_fullgc RET-patched (saved=0x%02x)\n",
                                    (unsigned)_fullgc_saved);
                            log_write(_lp);
                        }
                    }

                    int pumped_ok = 0;
                    InterlockedExchange(&g_in_prewarm_pumpe, 1);
                    InterlockedExchange(&g_prewarm_jmp_valid, 1);
                    if (setjmp(g_prewarm_jmp) == 0) {
                        g_pumpe_orig(lua_state);
                        pumped_ok = 1;
                    } else {
                        char _fe[128];
                        sprintf(_fe, "[eaw-mt] PREWARM: entity %d faulted in pumpe, skipping\n", count);
                        log_write(_fe);
                    }
                    InterlockedExchange(&g_prewarm_jmp_valid, 0);
                    InterlockedExchange(&g_in_prewarm_pumpe, 0);

                    /* Restore luaC_step and luaC_fullgc — unconditional (pumped_ok or fault). */
                    if (_luac_patched && _luac_step) {
                        DWORD _old_prot;
                        VirtualProtect(_luac_step, 1, PAGE_EXECUTE_READWRITE, &_old_prot);
                        *_luac_step = _luac_saved;
                        VirtualProtect(_luac_step, 1, _old_prot, &_old_prot);
                        FlushInstructionCache(GetCurrentProcess(), _luac_step, 1);
                    }
                    if (_fullgc_patched && _luac_fullgc) {
                        DWORD _old_prot;
                        VirtualProtect(_luac_fullgc, 1, PAGE_EXECUTE_READWRITE, &_old_prot);
                        *_luac_fullgc = _fullgc_saved;
                        VirtualProtect(_luac_fullgc, 1, _old_prot, &_old_prot);
                        FlushInstructionCache(GetCurrentProcess(), _luac_fullgc, 1);
                    }

                    if (pumped_ok)
                        log_write("[eaw-mt] PREWARM: pumpe returned ok\n");

                    /* Restore lua_state+0x58 and wr: FUN_140246fb0 (ServiceRate
                     * lookup, called from the battle loop) reads lua_state+0x58
                     * unconditionally and passes it to FUN_1407b9540.  If we leave
                     * it at 0 (as Pump_Threads set it), that null propagates as
                     * param_1 → [0+0x20] AV at RVA 0x7b9589.
                     *
                     * Also restore wr (buf+0x10): Pump_Threads advances wr during
                     * the pump; if we leave the post-pump cursor, subsequent writes
                     * by FUN_1407b9540 go to arbitrary buffer positions. */
                    /* Read post-pump state for diagnostics. */
                    int64_t post_hm08 = hm  ? *(int64_t *)(hm  + 0x08) : 0;
                    int32_t post_hm70 = hm  ? *(int32_t *)(hm  + 0x70) : 0;
                    int32_t post_hm74 = hm  ? *(int32_t *)(hm  + 0x74) : 0;

                    *(int64_t *)(lua_state + 0x58) = buf;
                    if (buf) {
                        if (pumped_ok) {
                            int64_t cur_hm  = buf ? *(int64_t *)(buf + 0x20) : 0;
                            int64_t cur_arr = cur_hm ? *(int64_t *)(cur_hm + 0x00) : 0;
                            /* Check whether arr was reallocated during the pump.
                             *
                             * If cur_arr == arr_pre: arr was NOT moved by GC — it still
                             * holds stale TString chain pointers and game-side freed-slot
                             * sentinels (0xffffffffffffffef) that cause [ptr+0x10] AV at
                             * RVA 0x7bf960 when luaS_newlstr traverses a chain.
                             *
                             * Both GC functions are RET-patched (luaC_step + luaC_fullgc)
                             * so arr_pre is GUARANTEED not freed during pumpe.
                             * memset(arr_pre, 0, cap*8) clears all stale pointers; safe. */
                            int memset_ok = 0;
                            if (cur_arr == arr_pre && arr_pre && cap > 0 && cap <= 0x10000) {
                                int major_gc = (hm74_pre > 1000 &&
                                                post_hm74 >= 0 &&
                                                post_hm74 < hm74_pre / 100);
                                if (major_gc) {
                                    /* Unexpected: both GC functions were patched.
                                     * If hm74 still dropped >99%, something freed arr_pre.
                                     * Fall back to calloc to avoid touching a freed block. */
                                    void *fresh = calloc((size_t)cap, 8);
                                    if (fresh && cur_hm == hm && hm) {
                                        *(int64_t *)(cur_hm + 0x00) = (int64_t)fresh;
                                    }
                                    memset_ok = 3;
                                    major_gc_happened = 1;
                                    pumped_hm = (cur_hm ? cur_hm : hm);
                                } else {
                                    /* arr_pre is freed by Pump_Threads' C-level cleanup (not GC).
                                     * DO NOT memset: arr_pre[0] is the heap Flink of a free block.
                                     * Zeroing it corrupts the free list → av_write @0x8 in ntdll.
                                     * Allocate a fresh empty bucket array via Lua's own allocator
                                     * (hm+0x10=frealloc, hm+0x18=ud) so luaM_free can reclaim it
                                     * without allocator-mismatch heap corruption. */
                                    {
                                        char _dbg_fa[160];
                                        sprintf(_dbg_fa,
                                                "[eaw-mt] PREWARM-DBG: hm10_pre=0x%llx"
                                                " hm10_cur=0x%llx hm18_pre=0x%llx"
                                                " winmm_base=0x%llx\n",
                                                (unsigned long long)hm10_pre,
                                                (unsigned long long)(hm ? *(int64_t *)(hm + 0x10) : 0),
                                                (unsigned long long)hm18_pre,
                                                (unsigned long long)(uintptr_t)
                                                    GetModuleHandleA("winmm.dll"));
                                        log_write(_dbg_fa);
                                    }
                                    /* Use calloc (CRT heap) rather than fn_alloc: the game's
                                     * Lua allocator can be a non-game-binary function whose
                                     * address is non-executable in some sessions, crashing the
                                     * CALL.  EaW's Lua uses the CRT heap (realloc/free), which
                                     * is compatible with calloc, so there is no mismatch risk. */
                                    void *fresh_arr = calloc((size_t)cap, 8);
                                    if (fresh_arr) {
                                        memset(fresh_arr, 0, (size_t)cap * 8);
                                        if (cur_hm == hm && hm) {
                                            *(int64_t *)(cur_hm + 0x00) = (int64_t)(uintptr_t)fresh_arr;
                                            *(int32_t *)(cur_hm + 0x08) = 0;  /* nuse=0 */
                                        }
                                    }
                                    memset_ok = 1;
                                }
                            }
                            /* Restore cap, and load-factor thresholds (hm+0x70/0x74).
                             * Restoring only cap (not count) keeps the map empty while
                             * allowing hash % cap without dividing by zero.
                             * Restoring hm+0x70 and hm+0x74 prevents the 0<=0 load-factor
                             * check in FUN_1407b9540 from triggering FUN_1407bc850 on
                             * every call when both were zeroed by the pump cleanup. */
                            if (cur_hm == hm && hm && cap > 0) {
                                *(int32_t *)(cur_hm + 0x0c) = cap;
                                *(int32_t *)(cur_hm + 0x70) = hm70_pre;
                                *(int32_t *)(cur_hm + 0x74) = hm74_pre;
                                /* Pump_Threads zeros hm+0x38 during its C-level cleanup.
                                 * Without restore, second-battle pumpe crashes at RVA 0x7ba12c:
                                 * MOV R8,[RAX+0x38] → R8=null → MOV [R8+R9*8-8],0 → av_write. */
                                *(int64_t *)(cur_hm + 0x38) = hm38_pre;
                                *(int64_t *)(cur_hm + 0x48) = hm48_pre;
                            }
                            char _rs[512];
                            sprintf(_rs, "[eaw-mt] PREWARM: restore entity %d  arr=%llx  arr_moved=%d  memset=%d"
                                    "  hm08_pre=0x%llx  cap=%d"
                                    "  hm08_post=0x%llx  hm70=%d->%d  hm74=%d->%d"
                                    "  hm38=0x%llx  hm48=0x%llx\n",
                                    count, (unsigned long long)cur_arr,
                                    (cur_arr != arr_pre) ? 1 : 0, memset_ok,
                                    (unsigned long long)hm08_pre, (int)cap,
                                    (unsigned long long)post_hm08,
                                    (int)hm70_pre, (int)post_hm70,
                                    (int)hm74_pre, (int)post_hm74,
                                    (unsigned long long)hm38_pre,
                                    (unsigned long long)hm48_pre);
                            log_write(_rs);
                        }
                    }
                    if (pumped_ok) {
                        pumped++;
                        g_prewarm_lua_state = lua_state;
                    }
                }
            }
        }
        node = *(int64_t *)(node + 8);
        count++;
    }

    /* Second pass: major GC during entity 14's pump collects TString objects
     * from ALL entities' hash tables (shared global_State).  Every entity's arr
     * now contains pointers to freed TString objects — following those chains
     * causes av_read @ffffffffffffffff (non-canonical next ptr, e.g. at RVA
     * 0x7bfdf0).  For non-pumped entities, arr itself is still a live allocation
     * (GC frees GC objects, not raw allocations), so memset to zeros is safe. */
    if (major_gc_happened) {
        int64_t node2   = *(int64_t *)(ai_manager + 0x48);
        int count2 = 0, reset2 = 0;
        while (node2 != sentinel && count2 < 2048) {
            int64_t ref2 = *(int64_t *)(node2 + 0x18);
            if (ref2) {
                int64_t entity2    = ref2 - 0x18;
                int64_t lua_state2 = *(int64_t *)(entity2 + 0x2d8);
                uint8_t eflags2    = *(uint8_t *)(entity2 + 0x3a0);
                if (lua_state2 && !(eflags2 & 2)) {
                    int64_t buf2 = *(int64_t *)(lua_state2 + 0x58);
                    int64_t hm2  = buf2 ? *(int64_t *)(buf2 + 0x20) : 0;
                    int32_t cap2 = hm2  ? *(int32_t *)(hm2  + 0x0c) : 0;
                    int64_t arr2 = hm2  ? *(int64_t *)(hm2  + 0x00) : 0;
                    /* Skip the entity we already calloc'd in the first pass. */
                    if (hm2 && hm2 != pumped_hm && cap2 > 0 && cap2 <= 0x10000 && arr2) {
                        memset((void *)arr2, 0, (size_t)cap2 * 8);
                        reset2++;
                    }
                }
            }
            node2 = *(int64_t *)(node2 + 8);
            count2++;
        }
        char _r2[96];
        sprintf(_r2, "[eaw-mt] PREWARM: gc-sweep reset %d other entities\n", reset2);
        log_write(_r2);
    }

    /* Restore *(lVar2+0x20) for every battle-mode entry that was snapshotted */
    int restored = 0;
    for (int i = 0; i < bma_cnt; i++) {
        if (!bma_has[i]) continue;
        int64_t lv2 = bma_arr ? *(int64_t *)(bma_arr + i*8) : 0;
        if (!lv2) continue;
        *(int64_t *)(lv2 + 0x20) = bma_saved[i];
        restored++;
    }

    QueryPerformanceCounter(&t1);
    double ms = (t1.QuadPart - t0.QuadPart) / ((double)g_qpc_freq.QuadPart / 1000.0);
    char s[192];
    sprintf(s, "[eaw-mt] PREWARM: ai_mgr=0x%llx  %d walked  %d pumped  %d bma-restored  %.0fms\n",
            (unsigned long long)ai_manager, count, pumped, restored, ms);
    log_write(s);
}

static void ai_init_prewarm_hook(int64_t ai_manager)
{
    g_ai_init_orig(ai_manager);
    /* Fire prewarm immediately here, after AI entities are fully initialised
     * but while the C engine command processor is still inactive (loading screen).
     * Any Lua AI commands issued during the prewarm are never flushed to the live
     * game state, eliminating the pathfinder-kill and deployment-zone-block bugs
     * that occurred when prewarm ran from pumpe_hook (game-loop context).
     * Skip on first load (pumpe_count=0) — no prior battle to warm from. */
    g_deferred_ai_mgr = 0;
    if (g_pumpe_count > 0) {
        log_write("[eaw-mt] PREWARM: firing in ai_init_hook (loading, post-ai-init)\n");
        prewarm_from_ai_manager(ai_manager);
        InterlockedExchange(&g_post_prewarm_log, 10);
    }
}

static BOOL install_prewarm_hook(void)
{
    HMODULE exe = GetModuleHandleA(NULL);
    if (!exe) return FALSE;

    BYTE *fn_ai_init = (BYTE *)exe + AI_INIT_RVA;
    g_ai_init_orig   = (AiInitFn)fn_ai_init;
    g_bma_obj        = (int64_t)((BYTE *)exe + BMA_RVA);

    /* Single stub near fn_ai_init — within ±2GB of all three caller functions. */
    BYTE *stub = alloc_near(fn_ai_init, 14);
    if (!stub) {
        log_write("[eaw-mt] WARN: alloc_near failed for prewarm stub\n");
        return FALSE;
    }
    write_abs_jmp(stub, (uint64_t)ai_init_prewarm_hook);

    /* Patch every known callsite of FUN_1402a8730. */
    struct { BYTE *start; int size; const char *name; } callers[] = {
        { (BYTE *)exe + FN_52D10_RVA, FN_52D10_SCAN, "FUN_140052d10(save)"      },
        { (BYTE *)exe + FN_56480_RVA, FN_56480_SCAN, "FUN_140056480(skirmish)"  },
        { (BYTE *)exe + FN_8DF00_RVA, FN_8DF00_SCAN, "FUN_14008df00(galactic)"  },
    };

    int total = 0;
    for (int c = 0; c < 3; c++) {
        BYTE *fn = callers[c].start;
        int n = 0;
        for (int i = 0; i <= callers[c].size - 5; i++) {
            if (fn[i] == 0xE8) {
                int32_t rel;
                memcpy(&rel, fn + i + 1, 4);
                BYTE *target = fn + i + 5 + (ptrdiff_t)rel;
                if (target == fn_ai_init) {
                    int32_t new_rel = (int32_t)(stub - (fn + i + 5));
                    DWORD old;
                    VirtualProtect(fn + i + 1, 4, PAGE_EXECUTE_READWRITE, &old);
                    memcpy(fn + i + 1, &new_rel, 4);
                    VirtualProtect(fn + i + 1, 4, old, &old);
                    FlushInstructionCache(GetCurrentProcess(), fn + i, 5);
                    n++;
                }
            }
        }
        char m[128];
        sprintf(m, "[eaw-mt] prewarm: %s — %d callsite(s) patched\n", callers[c].name, n);
        log_write(m);
        total += n;
    }
    return total > 0;
}

/* =========================================================================
 * FUN_140056480 callsite wrapper (Phase 5 Step 29j)
 *
 * Loading-phase prewarm fails at every sub-callsite because Lua AI callbacks
 * have C-level side effects on game state (entity lists, battle-mode arrays)
 * that subsequent loading code depends on being pristine.  Running prewarm
 * DURING fn56480 corrupts that state.
 *
 * Solution: scan the entire exe image for CALL fn56480 sites and redirect
 * them to fn56480_wrapper.  The wrapper calls the FULL original fn56480
 * (all loading code runs cleanly), then runs prewarm after it returns.
 * At that point the loading is complete, entities are fully initialized,
 * and Lua callbacks are safe — exactly as in the normal game-loop first call.
 * The loading screen remains visible for the extra ~14s; the battle starts
 * warm with no in-battle freeze.
 *
 * Signature from Ghidra: undefined8 FUN_140056480(undefined8*, int, undefined4,
 *                                                   undefined4, char)
 * ========================================================================= */
typedef int64_t (*Fn56480_t)(int64_t *, int32_t, int32_t, int32_t, char);
static Fn56480_t g_fn56480_orig = NULL;

static int64_t fn56480_wrapper(int64_t *p1, int32_t p2, int32_t p3, int32_t p4, char p5)
{
    int64_t ret = g_fn56480_orig(p1, p2, p3, p4, p5);
    if (g_deferred_ai_mgr) {
        if (g_pumpe_count > 0) {
            /* Leave g_deferred_ai_mgr set — pumpe_hook will fire prewarm on the
             * first game-loop pump call, where all services are initialised and
             * the allocator is in a clean state.  Running prewarm here (loading
             * context) calls the allocator while game services are partially
             * set up, which corrupts the free list → crash. */
            log_write("[eaw-mt] PREWARM: deferring to pumpe_hook (loading context unsafe)\n");
        } else {
            g_deferred_ai_mgr = 0;
            log_write("[eaw-mt] PREWARM: skip — first load (pumpe_count=0)\n");
        }
    }
    return ret;
}

static BOOL install_fn56480_wrapper(void)
{
    HMODULE exe = GetModuleHandleA(NULL);
    if (!exe) return FALSE;
    BYTE *base = (BYTE *)exe;

    PIMAGE_DOS_HEADER dos = (PIMAGE_DOS_HEADER)base;
    PIMAGE_NT_HEADERS nt  = (PIMAGE_NT_HEADERS)(base + dos->e_lfanew);
    DWORD img_size = nt->OptionalHeader.SizeOfImage;

    BYTE *fn56480 = base + FN_56480_RVA;
    g_fn56480_orig = (Fn56480_t)fn56480;

    BYTE *stub = alloc_near(fn56480, 14);
    if (!stub) { log_write("[eaw-mt] WARN: alloc_near failed for fn56480 stub\n"); return FALSE; }
    write_abs_jmp(stub, (uint64_t)fn56480_wrapper);

    int n = 0;
    for (DWORD i = 0; i + 4 < img_size; i++) {
        if (base[i] != 0xE8) continue;
        int32_t rel;
        memcpy(&rel, base + i + 1, 4);
        BYTE *target = base + i + 5 + (ptrdiff_t)rel;
        if (target != fn56480) continue;
        int32_t new_rel = (int32_t)(stub - (base + i + 5));
        DWORD old;
        if (!VirtualProtect(base + i + 1, 4, PAGE_EXECUTE_READWRITE, &old)) continue;
        memcpy(base + i + 1, &new_rel, 4);
        VirtualProtect(base + i + 1, 4, old, &old);
        FlushInstructionCache(GetCurrentProcess(), base + i, 5);
        char m[96];
        sprintf(m, "[eaw-mt] fn56480-wrap: rva=0x%x\n", i);
        log_write(m);
        n++;
    }
    char m[64];
    sprintf(m, "[eaw-mt] fn56480 wrapper: %d callsite(s) patched\n", n);
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
    tgt_fake_qpc(&t0);
    g_a76b0_orig(a, b);
    tgt_fake_qpc(&t1);
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
    tgt_fake_qpc(&t0);
    g_a9e30_orig(a, b, c, d, e, f, g, h, i2, j, k2, l, m2, n2);
    tgt_fake_qpc(&t1);
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
    tgt_fake_qpc(&t0);
    g_e369e0_orig(a, b);
    tgt_fake_qpc(&t1);
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
    tgt_fake_qpc(&t0);
    g_b87010_orig(a, b);
    tgt_fake_qpc(&t1);
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
    tgt_fake_qpc(&t0);
    int32_t r = g_d12d520_orig(manager, name);
    tgt_fake_qpc(&t1);
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
    tgt_fake_qpc(&t0);
    g_d12d480_orig(system, slot, vec);
    tgt_fake_qpc(&t1);
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

/* g_opp_per_call declared here so b387400_hook can reset it before calling orig */
static volatile int g_opp_per_call = 0;

static void b387400_hook(int64_t a, int32_t b)
{
    g_opp_per_call = 0;
    LARGE_INTEGER t0, t1;
    tgt_fake_qpc(&t0);
    g_b387400_orig(a, b);
    tgt_fake_qpc(&t1);
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
    tgt_fake_qpc(&t0);
    int64_t r = g_b385cf0_orig(a);
    tgt_fake_qpc(&t1);
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
    tgt_fake_qpc(&t0);
    int64_t r = g_b381ff0_orig(a);
    tgt_fake_qpc(&t1);
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
 * Fix B — Opportunity target stall (Phase 5, 2026-04-26)
 *
 * Root cause: DAT_140b0a340 = 0.0f → periodic search fires EVERY tick.
 * FUN_140387400 iterates all N entities calling FUN_140385190 per candidate
 * → O(N) per tick per component → 1,000ms stall.
 *
 * Fix B1: Set DAT_140b0a340 = 60.0f → threshold = 30 ticks (≈1s at 30fps).
 *         Reduces periodic search frequency from every-tick to every-30-ticks.
 *
 * Fix B2 (inner loop cap): E8 scan inside FUN_140387400 (1904 bytes) for
 *         calls to FUN_140385190 (RVA 0x385190). Budget hook: allow at most
 *         OPP_SEARCH_CAP calls to FUN_140385190 per b387400 invocation.
 *         b387400_hook resets the per-call counter before each call to orig.
 *         FUN_140385190 returns NULL when budget exhausted → loop stops early.
 *
 * Both fixes together: periodic path fires 30x less often AND is capped at
 * OPP_SEARCH_CAP candidates even when it does fire.  The target-cleared path
 * (bVar3=true) is also capped by Fix B2.
 * ========================================================================= */
#define OPP_INTERVAL_RVA   0xb0a340ULL   /* DAT_140b0a340: opp-target interval float */
#define OPP_INTERVAL_NEW   60.0f         /* new value: threshold = 60*0.5 = 30 ticks */
#define B387400_BODY_SIZE  1904          /* FUN_140387400 body size for E8 scan */
#define B385190_RVA        0x385190ULL   /* FUN_140385190: per-candidate reachability */
#define OPP_SEARCH_CAP     25            /* max FUN_140385190 calls per b387400 call */

typedef int64_t * (*B385190Fn)(int64_t, int64_t, float *);
static B385190Fn g_b385190_orig      = NULL;
static LONG      g_b385190_count     = 0;   /* total calls (budget-allowed) */
static LONG      g_b385190_capped    = 0;   /* calls that hit the budget cap */
static double    g_b385190_sum_ms    = 0, g_b385190_max_ms = 0;

static int64_t * b385190_hook(int64_t component, int64_t candidate, float *score)
{
    if (g_opp_per_call >= OPP_SEARCH_CAP) {
        InterlockedIncrement(&g_b385190_capped);
        return (int64_t *)0;  /* budget exhausted — short-circuit the search */
    }
    g_opp_per_call++;
    LARGE_INTEGER t0, t1;
    tgt_fake_qpc(&t0);
    int64_t *r = g_b385190_orig(component, candidate, score);
    tgt_fake_qpc(&t1);
    double ms = (t1.QuadPart - t0.QuadPart) / ((double)g_qpc_freq.QuadPart / 1000.0);
    g_b385190_sum_ms += ms;
    if (ms > g_b385190_max_ms) g_b385190_max_ms = ms;
    InterlockedIncrement(&g_b385190_count);
    return r;
}

static BOOL install_fix_b(void)
{
    HMODULE exe = GetModuleHandleA(NULL);
    if (!exe) return FALSE;

    /* Fix B1: patch opp-target search interval from 0.0f to OPP_INTERVAL_NEW */
    float *interval = (float *)((BYTE *)exe + OPP_INTERVAL_RVA);
    float old_val   = *interval;
    DWORD old_prot;
    VirtualProtect(interval, 4, PAGE_EXECUTE_READWRITE, &old_prot);
    *interval = OPP_INTERVAL_NEW;
    VirtualProtect(interval, 4, old_prot, &old_prot);
    {
        char m[96];
        sprintf(m, "[eaw-mt] FIX-B1: opp interval %.1f->%.1f (threshold %.1f->%.1f ticks)\n",
                old_val, OPP_INTERVAL_NEW, old_val * 0.5f, OPP_INTERVAL_NEW * 0.5f);
        log_write(m);
    }

    /* Fix B2: hook FUN_140385190 call site(s) inside FUN_140387400 */
    BYTE *fn_b387400 = (BYTE *)exe + B387400_RVA;
    BYTE *fn_b385190 = (BYTE *)exe + B385190_RVA;
    g_b385190_orig = (B385190Fn)fn_b385190;

    BYTE *stub = alloc_near(fn_b387400, 14);
    if (!stub) {
        log_write("[eaw-mt] WARN: alloc_near failed for b385190 budget stub\n");
        return FALSE;
    }
    write_abs_jmp(stub, (uint64_t)b385190_hook);

    int n = 0;
    for (int i = 0; i <= B387400_BODY_SIZE - 5; i++) {
        if (fn_b387400[i] != 0xE8) continue;
        int32_t rel;
        memcpy(&rel, fn_b387400 + i + 1, 4);
        if (fn_b387400 + i + 5 + rel != fn_b385190) continue;
        int32_t new_rel = (int32_t)(stub - (fn_b387400 + i + 5));
        VirtualProtect(fn_b387400 + i + 1, 4, PAGE_EXECUTE_READWRITE, &old_prot);
        memcpy(fn_b387400 + i + 1, &new_rel, 4);
        VirtualProtect(fn_b387400 + i + 1, 4, old_prot, &old_prot);
        FlushInstructionCache(GetCurrentProcess(), fn_b387400 + i, 5);
        n++;
    }
    {
        char m[80];
        sprintf(m, "[eaw-mt] FIX-B2: b385190 budget=%d, %d call site(s) patched in 387400\n",
                OPP_SEARCH_CAP, n);
        log_write(m);
    }
    return TRUE;
}

/* =========================================================================
 * FUN_140387400 sub-callee hooks — isolating the true stall source
 *
 * Fix B profiling showed b385190_capped=0 in every window: FUN_140385190
 * averages <1 call per b387400 invocation and max=0.30ms — the search loop
 * is not the stall.  The 1067ms stall is in a different callee.
 *
 * Suspects from phase5_387400.c (both E8-scanned inside FUN_140387400):
 *   FUN_140384850  (RVA 0x384850) — "can reach current target?" — lines 87,106
 *   FUN_1403825b0  (RVA 0x3825b0) — "try to path to target"     — lines 171,176,232,295
 * ========================================================================= */
#define B384850_RVA  0x384850ULL
#define B3825B0_RVA  0x3825b0ULL

typedef char (*B384850Fn)(int64_t, int64_t, int64_t, int64_t);
static B384850Fn g_b384850_orig  = NULL;
static LONG      g_b384850_count = 0;
static double    g_b384850_sum_ms = 0, g_b384850_max_ms = 0;

static char b384850_hook(int64_t a, int64_t b, int64_t c, int64_t d)
{
    LARGE_INTEGER t0, t1;
    tgt_fake_qpc(&t0);
    char r = g_b384850_orig(a, b, c, d);
    tgt_fake_qpc(&t1);
    double ms = (t1.QuadPart - t0.QuadPart) / ((double)g_qpc_freq.QuadPart / 1000.0);
    g_b384850_sum_ms += ms;
    if (ms > g_b384850_max_ms) g_b384850_max_ms = ms;
    g_b384850_count++;
    return r;
}

typedef char (*B3825b0Fn)(int64_t, int64_t, int64_t, int64_t);
static B3825b0Fn g_b3825b0_orig  = NULL;
static LONG      g_b3825b0_count = 0;
static double    g_b3825b0_sum_ms = 0, g_b3825b0_max_ms = 0;

static char b3825b0_hook(int64_t a, int64_t b, int64_t c, int64_t d)
{
    LARGE_INTEGER t0, t1;
    tgt_fake_qpc(&t0);
    char r = g_b3825b0_orig(a, b, c, d);
    tgt_fake_qpc(&t1);
    double ms = (t1.QuadPart - t0.QuadPart) / ((double)g_qpc_freq.QuadPart / 1000.0);
    g_b3825b0_sum_ms += ms;
    if (ms > g_b3825b0_max_ms) g_b3825b0_max_ms = ms;
    g_b3825b0_count++;
    return r;
}

static BOOL install_b387400_subcallee_hooks(void)
{
    HMODULE exe = GetModuleHandleA(NULL);
    if (!exe) return FALSE;

    BYTE *fn_b387400 = (BYTE *)exe + B387400_RVA;
    BYTE *fn_b384850 = (BYTE *)exe + B384850_RVA;
    BYTE *fn_b3825b0 = (BYTE *)exe + B3825B0_RVA;

    g_b384850_orig = (B384850Fn)fn_b384850;
    g_b3825b0_orig = (B3825b0Fn)fn_b3825b0;

    BYTE *stubs = alloc_near(fn_b387400, 14 * 2);
    if (!stubs) {
        log_write("[eaw-mt] WARN: alloc_near failed for b387400 sub-callee stubs\n");
        return FALSE;
    }
    write_abs_jmp(stubs + 0,  (uint64_t)b384850_hook);
    write_abs_jmp(stubs + 14, (uint64_t)b3825b0_hook);

    int n384850 = 0, n3825b0 = 0;
    DWORD old_prot;
    for (int i = 0; i <= B387400_BODY_SIZE - 5; i++) {
        if (fn_b387400[i] != 0xE8) continue;
        int32_t rel;
        memcpy(&rel, fn_b387400 + i + 1, 4);
        BYTE *target = fn_b387400 + i + 5 + rel;
        BYTE *stub = NULL;
        int  *cnt  = NULL;
        if      (target == fn_b384850) { stub = stubs + 0;  cnt = &n384850; }
        else if (target == fn_b3825b0) { stub = stubs + 14; cnt = &n3825b0; }
        else continue;
        int32_t new_rel = (int32_t)(stub - (fn_b387400 + i + 5));
        VirtualProtect(fn_b387400 + i + 1, 4, PAGE_EXECUTE_READWRITE, &old_prot);
        memcpy(fn_b387400 + i + 1, &new_rel, 4);
        VirtualProtect(fn_b387400 + i + 1, 4, old_prot, &old_prot);
        FlushInstructionCache(GetCurrentProcess(), fn_b387400 + i, 5);
        (*cnt)++;
    }
    char m[96];
    sprintf(m, "[eaw-mt] b387400_sub: b384850=%d b3825b0=%d site(s) patched in 387400\n",
            n384850, n3825b0);
    log_write(m);
    return (n384850 + n3825b0) > 0;
}

/* =========================================================================
 * FUN_1403825b0 sub-callee hooks — isolating the stall within b3825b0
 *
 * FUN_1403825b0 (4034 bytes) confirmed as terminal stall carrier (max 1,072ms).
 * Four suspects from decompile, all inside the deep path:
 *   FUN_140385e70  (RVA 0x385e70) — formation/physics position (gates block)
 *   FUN_140399450  (RVA 0x399450) — attack-position / approach-vector calc
 *   FUN_140381dc0  (RVA 0x381dc0) — path direction compute
 *   FUN_14029f810  (RVA 0x29f810) — movement order + pathfinding submission
 * ========================================================================= */
#define B3825B0_BODY_SIZE  4034
#define B385E70_RVA  0x385e70ULL
#define B399450_RVA  0x399450ULL
#define B381DC0_RVA  0x381dc0ULL
#define B29F810_RVA        0x29f810ULL
#define B29F810_BODY_SIZE  2201        /* FUN_14029f810 body size for E8 scan */

typedef int64_t (*B385e70Fn)(int64_t, int64_t, int64_t, int64_t);
static B385e70Fn g_b385e70_orig  = NULL;
static LONG      g_b385e70_count = 0;
static double    g_b385e70_sum_ms = 0, g_b385e70_max_ms = 0;
static int64_t b385e70_hook(int64_t a, int64_t b, int64_t c, int64_t d) {
    LARGE_INTEGER t0, t1; tgt_fake_qpc(&t0);
    int64_t r = g_b385e70_orig(a, b, c, d); tgt_fake_qpc(&t1);
    double ms = (t1.QuadPart - t0.QuadPart) / ((double)g_qpc_freq.QuadPart / 1000.0);
    g_b385e70_sum_ms += ms; if (ms > g_b385e70_max_ms) g_b385e70_max_ms = ms;
    g_b385e70_count++; return r;
}

typedef int64_t (*B399450Fn)(int64_t, int64_t, int64_t, int64_t, int64_t, int64_t);
static B399450Fn g_b399450_orig  = NULL;
static LONG      g_b399450_count = 0;
static double    g_b399450_sum_ms = 0, g_b399450_max_ms = 0;
static int64_t b399450_hook(int64_t a, int64_t b, int64_t c, int64_t d, int64_t e, int64_t f) {
    LARGE_INTEGER t0, t1; tgt_fake_qpc(&t0);
    int64_t r = g_b399450_orig(a, b, c, d, e, f); tgt_fake_qpc(&t1);
    double ms = (t1.QuadPart - t0.QuadPart) / ((double)g_qpc_freq.QuadPart / 1000.0);
    g_b399450_sum_ms += ms; if (ms > g_b399450_max_ms) g_b399450_max_ms = ms;
    g_b399450_count++; return r;
}

typedef int64_t (*B381dc0Fn)(int64_t, int64_t, int64_t, int64_t, int64_t, int64_t);
static B381dc0Fn g_b381dc0_orig  = NULL;
static LONG      g_b381dc0_count = 0;
static double    g_b381dc0_sum_ms = 0, g_b381dc0_max_ms = 0;
static int64_t b381dc0_hook(int64_t a, int64_t b, int64_t c, int64_t d, int64_t e, int64_t f) {
    LARGE_INTEGER t0, t1; tgt_fake_qpc(&t0);
    int64_t r = g_b381dc0_orig(a, b, c, d, e, f); tgt_fake_qpc(&t1);
    double ms = (t1.QuadPart - t0.QuadPart) / ((double)g_qpc_freq.QuadPart / 1000.0);
    g_b381dc0_sum_ms += ms; if (ms > g_b381dc0_max_ms) g_b381dc0_max_ms = ms;
    g_b381dc0_count++; return r;
}

typedef int64_t (*B29f810Fn)(int64_t, int64_t, int64_t, int64_t, int64_t, int64_t, int64_t);
static B29f810Fn g_b29f810_orig  = NULL;
static LONG      g_b29f810_count = 0;
static double    g_b29f810_sum_ms = 0, g_b29f810_max_ms = 0;
static int64_t b29f810_hook(int64_t a, int64_t b, int64_t c, int64_t d, int64_t e, int64_t f, int64_t g) {
    LARGE_INTEGER t0, t1; tgt_fake_qpc(&t0);
    int64_t r = g_b29f810_orig(a, b, c, d, e, f, g); tgt_fake_qpc(&t1);
    double ms = (t1.QuadPart - t0.QuadPart) / ((double)g_qpc_freq.QuadPart / 1000.0);
    g_b29f810_sum_ms += ms; if (ms > g_b29f810_max_ms) g_b29f810_max_ms = ms;
    g_b29f810_count++; return r;
}

static BOOL install_b3825b0_subcallee_hooks(void)
{
    HMODULE exe = GetModuleHandleA(NULL);
    if (!exe) return FALSE;

    BYTE *fn_b3825b0 = (BYTE *)exe + B3825B0_RVA;
    BYTE *fn_b385e70 = (BYTE *)exe + B385E70_RVA;
    BYTE *fn_b399450 = (BYTE *)exe + B399450_RVA;
    BYTE *fn_b381dc0 = (BYTE *)exe + B381DC0_RVA;
    BYTE *fn_b29f810 = (BYTE *)exe + B29F810_RVA;

    g_b385e70_orig = (B385e70Fn)fn_b385e70;
    g_b399450_orig = (B399450Fn)fn_b399450;
    g_b381dc0_orig = (B381dc0Fn)fn_b381dc0;
    g_b29f810_orig = (B29f810Fn)fn_b29f810;

    BYTE *stubs = alloc_near(fn_b3825b0, 14 * 4);
    if (!stubs) {
        log_write("[eaw-mt] WARN: alloc_near failed for b3825b0 sub-callee stubs\n");
        return FALSE;
    }
    write_abs_jmp(stubs + 0,  (uint64_t)b385e70_hook);
    write_abs_jmp(stubs + 14, (uint64_t)b399450_hook);
    write_abs_jmp(stubs + 28, (uint64_t)b381dc0_hook);
    write_abs_jmp(stubs + 42, (uint64_t)b29f810_hook);

    int n385e70 = 0, n399450 = 0, n381dc0 = 0, n29f810 = 0;
    DWORD old_prot;
    for (int i = 0; i <= B3825B0_BODY_SIZE - 5; i++) {
        if (fn_b3825b0[i] != 0xE8) continue;
        int32_t rel;
        memcpy(&rel, fn_b3825b0 + i + 1, 4);
        BYTE *target = fn_b3825b0 + i + 5 + rel;
        BYTE *stub = NULL; int *cnt = NULL;
        if      (target == fn_b385e70) { stub = stubs + 0;  cnt = &n385e70; }
        else if (target == fn_b399450) { stub = stubs + 14; cnt = &n399450; }
        else if (target == fn_b381dc0) { stub = stubs + 28; cnt = &n381dc0; }
        else if (target == fn_b29f810) { stub = stubs + 42; cnt = &n29f810; }
        else continue;
        int32_t new_rel = (int32_t)(stub - (fn_b3825b0 + i + 5));
        VirtualProtect(fn_b3825b0 + i + 1, 4, PAGE_EXECUTE_READWRITE, &old_prot);
        memcpy(fn_b3825b0 + i + 1, &new_rel, 4);
        VirtualProtect(fn_b3825b0 + i + 1, 4, old_prot, &old_prot);
        FlushInstructionCache(GetCurrentProcess(), fn_b3825b0 + i, 5);
        (*cnt)++;
    }
    char m[128];
    sprintf(m, "[eaw-mt] b3825b0_sub: b385e70=%d b399450=%d b381dc0=%d b29f810=%d site(s) patched\n",
            n385e70, n399450, n381dc0, n29f810);
    log_write(m);
    return (n385e70 + n399450 + n381dc0 + n29f810) > 0;
}

/* =========================================================================
 * FUN_14029f810 sub-callee hooks — isolating the stall within b29f810
 *
 * FUN_14029f810 (2201 bytes) confirmed as terminal stall carrier (max 1,094ms).
 * Decompile shows FUN_140388b60 is the only non-trivial callee: it constructs
 * a 2208-byte GameObjectClass movement-order struct (pool-allocated at lVar3)
 * and is called immediately after the null-guard.
 *
 * Inside FUN_140388b60 (1998 bytes), the only substantive call is:
 *   FUN_1403989a0(param_1, 0, 1)  — registers the new move-order with the
 *   movement system (spatial index / pathfinding queue insertion).
 *
 * Two hooks:
 *   b388b60  — E8 scan in FUN_14029f810 body (2201 bytes)
 *   b3989a0  — E8 scan in FUN_140388b60 body (1998 bytes)
 * If b388b60_max ≈ b29f810_max, the stall is inside the ctor; then
 * b3989a0_max tells us whether it's the spatial-insert or the field-init.
 * ========================================================================= */
#define B388B60_RVA       0x388b60ULL
#define B388B60_BODY_SIZE 1998
#define B3989A0_RVA       0x3989a0ULL

typedef int64_t * (*B388b60Fn)(int64_t, int64_t, int64_t, int64_t,
                                int64_t, int64_t, int64_t, int64_t);
static B388b60Fn g_b388b60_orig  = NULL;
static LONG      g_b388b60_count = 0;
static double    g_b388b60_sum_ms = 0, g_b388b60_max_ms = 0;
static int64_t * b388b60_hook(int64_t a, int64_t b, int64_t c, int64_t d,
                               int64_t e, int64_t f, int64_t g2, int64_t h) {
    LARGE_INTEGER t0, t1; tgt_fake_qpc(&t0);
    int64_t *r = g_b388b60_orig(a, b, c, d, e, f, g2, h); tgt_fake_qpc(&t1);
    double ms = (t1.QuadPart - t0.QuadPart) / ((double)g_qpc_freq.QuadPart / 1000.0);
    g_b388b60_sum_ms += ms; if (ms > g_b388b60_max_ms) g_b388b60_max_ms = ms;
    g_b388b60_count++; return r;
}

typedef void (*B3989a0Fn)(int64_t, int64_t, int64_t);
static B3989a0Fn g_b3989a0_orig  = NULL;
static LONG      g_b3989a0_count = 0;
static double    g_b3989a0_sum_ms = 0, g_b3989a0_max_ms = 0;
static void b3989a0_hook(int64_t a, int64_t b, int64_t c) {
    LARGE_INTEGER t0, t1; tgt_fake_qpc(&t0);
    g_b3989a0_orig(a, b, c); tgt_fake_qpc(&t1);
    double ms = (t1.QuadPart - t0.QuadPart) / ((double)g_qpc_freq.QuadPart / 1000.0);
    g_b3989a0_sum_ms += ms; if (ms > g_b3989a0_max_ms) g_b3989a0_max_ms = ms;
    g_b3989a0_count++;
}

static BOOL install_b29f810_subcallee_hooks(void)
{
    HMODULE exe = GetModuleHandleA(NULL);
    if (!exe) return FALSE;

    BYTE *fn_b29f810 = (BYTE *)exe + B29F810_RVA;
    BYTE *fn_b388b60 = (BYTE *)exe + B388B60_RVA;

    g_b388b60_orig = (B388b60Fn)fn_b388b60;

    BYTE *stub = alloc_near(fn_b29f810, 14);
    if (!stub) {
        log_write("[eaw-mt] WARN: alloc_near failed for b388b60 stub\n");
        return FALSE;
    }
    write_abs_jmp(stub, (uint64_t)b388b60_hook);

    int n = 0;
    DWORD old_prot;
    for (int i = 0; i <= B29F810_BODY_SIZE - 5; i++) {
        if (fn_b29f810[i] != 0xE8) continue;
        int32_t rel;
        memcpy(&rel, fn_b29f810 + i + 1, 4);
        if (fn_b29f810 + i + 5 + rel != fn_b388b60) continue;
        int32_t new_rel = (int32_t)(stub - (fn_b29f810 + i + 5));
        VirtualProtect(fn_b29f810 + i + 1, 4, PAGE_EXECUTE_READWRITE, &old_prot);
        memcpy(fn_b29f810 + i + 1, &new_rel, 4);
        VirtualProtect(fn_b29f810 + i + 1, 4, old_prot, &old_prot);
        FlushInstructionCache(GetCurrentProcess(), fn_b29f810 + i, 5);
        n++;
    }
    char m[80];
    sprintf(m, "[eaw-mt] b29f810_sub: b388b60=%d call site(s) patched in 29f810\n", n);
    log_write(m);
    return n > 0;
}

static BOOL install_b388b60_subcallee_hooks(void)
{
    HMODULE exe = GetModuleHandleA(NULL);
    if (!exe) return FALSE;

    BYTE *fn_b388b60 = (BYTE *)exe + B388B60_RVA;
    BYTE *fn_b3989a0 = (BYTE *)exe + B3989A0_RVA;

    g_b3989a0_orig = (B3989a0Fn)fn_b3989a0;

    BYTE *stub = alloc_near(fn_b388b60, 14);
    if (!stub) {
        log_write("[eaw-mt] WARN: alloc_near failed for b3989a0 stub\n");
        return FALSE;
    }
    write_abs_jmp(stub, (uint64_t)b3989a0_hook);

    int n = 0;
    DWORD old_prot;
    for (int i = 0; i <= B388B60_BODY_SIZE - 5; i++) {
        if (fn_b388b60[i] != 0xE8) continue;
        int32_t rel;
        memcpy(&rel, fn_b388b60 + i + 1, 4);
        if (fn_b388b60 + i + 5 + rel != fn_b3989a0) continue;
        int32_t new_rel = (int32_t)(stub - (fn_b388b60 + i + 5));
        VirtualProtect(fn_b388b60 + i + 1, 4, PAGE_EXECUTE_READWRITE, &old_prot);
        memcpy(fn_b388b60 + i + 1, &new_rel, 4);
        VirtualProtect(fn_b388b60 + i + 1, 4, old_prot, &old_prot);
        FlushInstructionCache(GetCurrentProcess(), fn_b388b60 + i, 5);
        n++;
    }
    char m[80];
    sprintf(m, "[eaw-mt] b388b60_sub: b3989a0=%d call site(s) patched in 388b60\n", n);
    log_write(m);
    return n > 0;
}

/* =========================================================================
 * FUN_1403989a0 sub-callee hooks — isolating the stall within b3989a0
 *
 * FUN_1403989a0 (2733 bytes) confirmed as 100% stall source in FUN_140388b60.
 * Two suspects from decompile:
 *   FUN_14038cb30  (RVA 0x38cb30) — called twice (lines 178, 196): path
 *     constraint setup based on movement type.
 *   FUN_14037c050  (RVA 0x37c050) — called once (line 418): synchronous path
 *     solve gated by FUN_140374da0 ("needs pathfinding?").  This receives the
 *     952-byte path result buffer and all movement params.
 * The bimodal (fast/slow) timing of b3989a0 matches a conditional path gate:
 * most calls skip FUN_14037c050 (cached or trivial); rare calls block 1178ms.
 * ========================================================================= */
#define B3989A0_BODY_SIZE  2733
#define B38CB30_RVA        0x38cb30ULL
#define B37C050_RVA        0x37c050ULL
#define B381A90_RVA        0x381a90ULL
#define B384740_RVA        0x384740ULL
#define B3727A0_RVA        0x3727a0ULL
#define B3A4820_RVA        0x3a4820ULL
#define B3A8710_RVA        0x3a8710ULL
#define B3AC530_RVA        0x3ac530ULL
#define B38CF30_RVA        0x38cf30ULL
#define B3A59F0_RVA        0x3a59f0ULL

/* FUN_1403727a0 — unconditional first call in b3989a0 (line 119).
 * 2 args: (*(param_1+0x298), 0). Returns undefined4 stored at param_1+0x5c. */
typedef int64_t (*B3727a0Fn)(int64_t, int64_t);
static B3727a0Fn g_b3727a0_orig  = NULL;
static LONG      g_b3727a0_count = 0;
static double    g_b3727a0_sum_ms = 0, g_b3727a0_max_ms = 0;
static int64_t b3727a0_hook(int64_t a, int64_t b) {
    LARGE_INTEGER t0, t1; tgt_fake_qpc(&t0);
    int64_t r = g_b3727a0_orig(a, b); tgt_fake_qpc(&t1);
    double ms = (t1.QuadPart - t0.QuadPart) / ((double)g_qpc_freq.QuadPart / 1000.0);
    g_b3727a0_sum_ms += ms; if (ms > g_b3727a0_max_ms) g_b3727a0_max_ms = ms;
    g_b3727a0_count++; return r;
}

/* FUN_1403a4820 — init call gated by DAT_140b2c37b==0 (line 152). 1 arg. */
typedef void (*B3a4820Fn)(int64_t);
static B3a4820Fn g_b3a4820_orig  = NULL;
static LONG      g_b3a4820_count = 0;
static double    g_b3a4820_sum_ms = 0, g_b3a4820_max_ms = 0;
static void b3a4820_hook(int64_t a) {
    LARGE_INTEGER t0, t1; tgt_fake_qpc(&t0);
    g_b3a4820_orig(a); tgt_fake_qpc(&t1);
    double ms = (t1.QuadPart - t0.QuadPart) / ((double)g_qpc_freq.QuadPart / 1000.0);
    g_b3a4820_sum_ms += ms; if (ms > g_b3a4820_max_ms) g_b3a4820_max_ms = ms;
    g_b3a4820_count++;
}

/* FUN_1403a8710 — movement-AI setup gated by param_1+0x2a0 != 0 (line 155).
 * 2 args: (move_order, move_order+0x84). Prime suspect for the ~1058ms stall. */
typedef void (*B3a8710Fn)(int64_t, int64_t);
static B3a8710Fn g_b3a8710_orig  = NULL;
static LONG      g_b3a8710_count = 0;
static double    g_b3a8710_sum_ms = 0, g_b3a8710_max_ms = 0;
static void b3a8710_hook(int64_t a, int64_t b) {
    LARGE_INTEGER t0, t1; tgt_fake_qpc(&t0);
    g_b3a8710_orig(a, b); tgt_fake_qpc(&t1);
    double ms = (t1.QuadPart - t0.QuadPart) / ((double)g_qpc_freq.QuadPart / 1000.0);
    g_b3a8710_sum_ms += ms; if (ms > g_b3a8710_max_ms) g_b3a8710_max_ms = ms;
    g_b3a8710_count++;
}

/* FUN_1403ac530 — orientation/constraint setup, 2 call sites (lines 166, 173).
 * 2 args: (move_order, bool_flag). */
typedef void (*B3ac530Fn)(int64_t, int64_t);
static B3ac530Fn g_b3ac530_orig  = NULL;
static LONG      g_b3ac530_count = 0;
static double    g_b3ac530_sum_ms = 0, g_b3ac530_max_ms = 0;
static void b3ac530_hook(int64_t a, int64_t b) {
    LARGE_INTEGER t0, t1; tgt_fake_qpc(&t0);
    g_b3ac530_orig(a, b); tgt_fake_qpc(&t1);
    double ms = (t1.QuadPart - t0.QuadPart) / ((double)g_qpc_freq.QuadPart / 1000.0);
    g_b3ac530_sum_ms += ms; if (ms > g_b3ac530_max_ms) g_b3ac530_max_ms = ms;
    g_b3ac530_count++;
}

/* FUN_14038cf30 — conditional call at line 208 (3 args). */
typedef void (*B38cf30Fn)(int64_t, int64_t, int64_t);
static B38cf30Fn g_b38cf30_orig  = NULL;
static LONG      g_b38cf30_count = 0;
static double    g_b38cf30_sum_ms = 0, g_b38cf30_max_ms = 0;
static void b38cf30_hook(int64_t a, int64_t b, int64_t c) {
    LARGE_INTEGER t0, t1; tgt_fake_qpc(&t0);
    g_b38cf30_orig(a, b, c); tgt_fake_qpc(&t1);
    double ms = (t1.QuadPart - t0.QuadPart) / ((double)g_qpc_freq.QuadPart / 1000.0);
    g_b38cf30_sum_ms += ms; if (ms > g_b38cf30_max_ms) g_b38cf30_max_ms = ms;
    g_b38cf30_count++;
}

/* FUN_1403a59f0 — conditional at line 194. Ghidra shows 0 args at call site but
 * function decompile has (longlong *param_1, char param_2). Pass both through. */
typedef void (*B3a59f0Fn)(int64_t, int64_t);
static B3a59f0Fn g_b3a59f0_orig  = NULL;
static LONG      g_b3a59f0_count = 0;
static double    g_b3a59f0_sum_ms = 0, g_b3a59f0_max_ms = 0;
static void b3a59f0_hook(int64_t a, int64_t b) {
    LARGE_INTEGER t0, t1; tgt_fake_qpc(&t0);
    g_b3a59f0_orig(a, b); tgt_fake_qpc(&t1);
    double ms = (t1.QuadPart - t0.QuadPart) / ((double)g_qpc_freq.QuadPart / 1000.0);
    g_b3a59f0_sum_ms += ms; if (ms > g_b3a59f0_max_ms) g_b3a59f0_max_ms = ms;
    g_b3a59f0_count++;
}

/* =========================================================================
 * FUN_1403a4820 sub-hooks (Phase 5 Step 22)
 *
 * FUN_1403a4820 (RVA 0x3a4820, 2481 bytes) accounts for 100% of the
 * b3989a0 stall (max 1074ms matches exactly).  It is a target-acquisition
 * function that walks potential opponents and assigns them to move orders.
 *
 * Suspects inside b3a4820:
 *   FUN_1403729f0 (RVA 0x3729f0) — 4 args — unconditional target scan;
 *     called BEFORE the fast-exit "target unchanged" check at line 79.
 *     If it does an O(N) fleet walk, this is the root cause.
 *   FUN_1403a5840 (RVA 0x3a5840) — 1 arg — teardown of existing assignment;
 *     conditional (only when param_1[0x54] != 0).
 *   FUN_140265560 (RVA 0x265560) — 6 args — setup in main path (line 176).
 * ========================================================================= */
#define B3A4820_BODY_SIZE  2481
#define B3729F0_RVA        0x3729f0ULL
#define B3A5840_RVA        0x3a5840ULL
#define B265560_RVA        0x265560ULL

#define B375380_BODY_SIZE  5645
#define B375380_RVA        0x375380ULL
#define B25EC10_RVA        0x25ec10ULL
#define B3718F0_RVA        0x3718f0ULL

#define B25EC10_BODY_SIZE  774
#define B142F80_RVA        0x142f80ULL
#define B141F70_RVA        0x141f70ULL
#define B141F70_BODY_SIZE  501
#define B2136F0_RVA        0x2136f0ULL

/* FUN_1403729f0 — target scan, 4 args from call site:
 * (param_1[0x53]=fleet, uVar4=movement_type, param_1=move_order, plVar8=vehicle_list) */
typedef int64_t (*B3729f0Fn)(int64_t, int64_t, int64_t, int64_t);
static B3729f0Fn g_b3729f0_orig  = NULL;
static LONG      g_b3729f0_count = 0;
static double    g_b3729f0_sum_ms = 0, g_b3729f0_max_ms = 0;
static int64_t b3729f0_hook(int64_t a, int64_t b, int64_t c, int64_t d) {
    LARGE_INTEGER t0, t1; tgt_fake_qpc(&t0);
    int64_t r = g_b3729f0_orig(a, b, c, d); tgt_fake_qpc(&t1);
    double ms = (t1.QuadPart - t0.QuadPart) / ((double)g_qpc_freq.QuadPart / 1000.0);
    g_b3729f0_sum_ms += ms; if (ms > g_b3729f0_max_ms) g_b3729f0_max_ms = ms;
    g_b3729f0_count++; return r;
}

/* FUN_1403a5840 — teardown existing assignment; 1 arg confirmed by decompile. */
typedef void (*B3a5840Fn)(int64_t);
static B3a5840Fn g_b3a5840_orig  = NULL;
static LONG      g_b3a5840_count = 0;
static double    g_b3a5840_sum_ms = 0, g_b3a5840_max_ms = 0;
static void b3a5840_hook(int64_t a) {
    LARGE_INTEGER t0, t1; tgt_fake_qpc(&t0);
    g_b3a5840_orig(a); tgt_fake_qpc(&t1);
    double ms = (t1.QuadPart - t0.QuadPart) / ((double)g_qpc_freq.QuadPart / 1000.0);
    g_b3a5840_sum_ms += ms; if (ms > g_b3a5840_max_ms) g_b3a5840_max_ms = ms;
    g_b3a5840_count++;
}

/* FUN_140265560 — 6-arg setup call in main path (line 176 of b3a4820). */
typedef void (*B265560Fn)(int64_t, int64_t, int64_t, int64_t, int64_t, int64_t);
static B265560Fn g_b265560_orig  = NULL;
static LONG      g_b265560_count = 0;
static double    g_b265560_sum_ms = 0, g_b265560_max_ms = 0;
static void b265560_hook(int64_t a, int64_t b, int64_t c, int64_t d, int64_t e, int64_t f) {
    LARGE_INTEGER t0, t1; tgt_fake_qpc(&t0);
    g_b265560_orig(a, b, c, d, e, f); tgt_fake_qpc(&t1);
    double ms = (t1.QuadPart - t0.QuadPart) / ((double)g_qpc_freq.QuadPart / 1000.0);
    g_b265560_sum_ms += ms; if (ms > g_b265560_max_ms) g_b265560_max_ms = ms;
    g_b265560_count++;
}

static BOOL install_b3a4820_subcallee_hooks(void)
{
    HMODULE exe = GetModuleHandleA(NULL);
    if (!exe) return FALSE;

    BYTE *fn_b3a4820 = (BYTE *)exe + B3A4820_RVA;
    BYTE *fn_b3729f0 = (BYTE *)exe + B3729F0_RVA;
    BYTE *fn_b3a5840 = (BYTE *)exe + B3A5840_RVA;
    BYTE *fn_b265560 = (BYTE *)exe + B265560_RVA;

    g_b3729f0_orig = (B3729f0Fn)fn_b3729f0;
    g_b3a5840_orig = (B3a5840Fn)fn_b3a5840;
    g_b265560_orig = (B265560Fn)fn_b265560;

    BYTE *stubs = alloc_near(fn_b3a4820, 14 * 3);
    if (!stubs) {
        log_write("[eaw-mt] WARN: alloc_near failed for b3a4820 sub stubs\n");
        return FALSE;
    }
    write_abs_jmp(stubs +  0, (uint64_t)b3729f0_hook);
    write_abs_jmp(stubs + 14, (uint64_t)b3a5840_hook);
    write_abs_jmp(stubs + 28, (uint64_t)b265560_hook);

    int n3729f0 = 0, n3a5840 = 0, n265560 = 0;
    DWORD old_prot;
    for (int i = 0; i <= B3A4820_BODY_SIZE - 5; i++) {
        if (fn_b3a4820[i] != 0xE8) continue;
        int32_t rel;
        memcpy(&rel, fn_b3a4820 + i + 1, 4);
        BYTE *target = fn_b3a4820 + i + 5 + rel;
        BYTE *stub = NULL; int *cnt = NULL;
        if      (target == fn_b3729f0) { stub = stubs +  0; cnt = &n3729f0; }
        else if (target == fn_b3a5840) { stub = stubs + 14; cnt = &n3a5840; }
        else if (target == fn_b265560) { stub = stubs + 28; cnt = &n265560; }
        else continue;
        int32_t new_rel = (int32_t)(stub - (fn_b3a4820 + i + 5));
        VirtualProtect(fn_b3a4820 + i + 1, 4, PAGE_EXECUTE_READWRITE, &old_prot);
        memcpy(fn_b3a4820 + i + 1, &new_rel, 4);
        VirtualProtect(fn_b3a4820 + i + 1, 4, old_prot, &old_prot);
        FlushInstructionCache(GetCurrentProcess(), fn_b3a4820 + i, 5);
        (*cnt)++;
    }
    char m[128];
    sprintf(m, "[eaw-mt] b3a4820_sub: b3729f0=%d b3a5840=%d b265560=%d site(s) patched in 3a4820\n",
            n3729f0, n3a5840, n265560);
    log_write(m);
    return (n3729f0 + n3a5840 + n265560) > 0;
}

/* FUN_14025ec10 — art-model object lookup by path string.
 * 2 args: (char *path, char *filter) — confirmed by decompile. */
typedef int64_t (*B25ec10Fn)(int64_t, int64_t);
static B25ec10Fn g_b25ec10_orig  = NULL;
static LONG      g_b25ec10_count = 0;
static double    g_b25ec10_sum_ms = 0, g_b25ec10_max_ms = 0;
static int64_t b25ec10_hook(int64_t a, int64_t b) {
    LARGE_INTEGER t0, t1; tgt_fake_qpc(&t0);
    int64_t r = g_b25ec10_orig(a, b); tgt_fake_qpc(&t1);
    double ms = (t1.QuadPart - t0.QuadPart) / ((double)g_qpc_freq.QuadPart / 1000.0);
    g_b25ec10_sum_ms += ms; if (ms > g_b25ec10_max_ms) g_b25ec10_max_ms = ms;
    g_b25ec10_count++; return r;
}

/* FUN_1403718f0 — variant/slot lookup by hash key (4 args: obj, key, idx, sub_idx).
 * Decompile shows (longlong, uint, int, int) — 4 args; call site only shows 3
 * because Ghidra folds param_3/param_4 from R8/R9.  Pass all 4 through. */
typedef int64_t (*B3718f0Fn)(int64_t, int64_t, int64_t, int64_t);
static B3718f0Fn g_b3718f0_orig  = NULL;
static LONG      g_b3718f0_count = 0;
static double    g_b3718f0_sum_ms = 0, g_b3718f0_max_ms = 0;
static int64_t b3718f0_hook(int64_t a, int64_t b, int64_t c, int64_t d) {
    LARGE_INTEGER t0, t1; tgt_fake_qpc(&t0);
    int64_t r = g_b3718f0_orig(a, b, c, d); tgt_fake_qpc(&t1);
    double ms = (t1.QuadPart - t0.QuadPart) / ((double)g_qpc_freq.QuadPart / 1000.0);
    g_b3718f0_sum_ms += ms; if (ms > g_b3718f0_max_ms) g_b3718f0_max_ms = ms;
    g_b3718f0_count++; return r;
}

static BOOL install_b375380_subcallee_hooks(void)
{
    HMODULE exe = GetModuleHandleA(NULL);
    if (!exe) return FALSE;

    BYTE *fn_b375380 = (BYTE *)exe + B375380_RVA;
    BYTE *fn_b25ec10 = (BYTE *)exe + B25EC10_RVA;
    BYTE *fn_b3718f0 = (BYTE *)exe + B3718F0_RVA;

    g_b25ec10_orig = (B25ec10Fn)fn_b25ec10;
    g_b3718f0_orig = (B3718f0Fn)fn_b3718f0;

    BYTE *stubs = alloc_near(fn_b375380, 14 * 2);
    if (!stubs) {
        log_write("[eaw-mt] WARN: alloc_near failed for b375380 sub stubs\n");
        return FALSE;
    }
    write_abs_jmp(stubs +  0, (uint64_t)b25ec10_hook);
    write_abs_jmp(stubs + 14, (uint64_t)b3718f0_hook);

    int n25ec10 = 0, n3718f0 = 0;
    DWORD old_prot;
    for (int i = 0; i <= B375380_BODY_SIZE - 5; i++) {
        if (fn_b375380[i] != 0xE8) continue;
        int32_t rel;
        memcpy(&rel, fn_b375380 + i + 1, 4);
        BYTE *target = fn_b375380 + i + 5 + rel;
        BYTE *stub = NULL; int *cnt = NULL;
        if      (target == fn_b25ec10) { stub = stubs +  0; cnt = &n25ec10; }
        else if (target == fn_b3718f0) { stub = stubs + 14; cnt = &n3718f0; }
        else continue;
        int32_t new_rel = (int32_t)(stub - (fn_b375380 + i + 5));
        VirtualProtect(fn_b375380 + i + 1, 4, PAGE_EXECUTE_READWRITE, &old_prot);
        memcpy(fn_b375380 + i + 1, &new_rel, 4);
        VirtualProtect(fn_b375380 + i + 1, 4, old_prot, &old_prot);
        FlushInstructionCache(GetCurrentProcess(), fn_b375380 + i, 5);
        (*cnt)++;
    }
    char m[128];
    sprintf(m, "[eaw-mt] b375380_sub: b25ec10=%d b3718f0=%d site(s) patched in 375380\n",
            n25ec10, n3718f0);
    log_write(m);
    return (n25ec10 + n3718f0) > 0;
}

/* FUN_140142f80 — asset registration check (in-memory linear scan).
 * 2 args: (asset_db, path_longlong) — returns asset ptr or 0. Fast path expected. */
typedef int64_t (*B142f80Fn)(int64_t, int64_t);
static B142f80Fn g_b142f80_orig  = NULL;
static LONG      g_b142f80_count = 0;
static double    g_b142f80_sum_ms = 0, g_b142f80_max_ms = 0;
static int64_t b142f80_hook(int64_t a, int64_t b) {
    LARGE_INTEGER t0, t1; tgt_fake_qpc(&t0);
    int64_t r = g_b142f80_orig(a, b); tgt_fake_qpc(&t1);
    double ms = (t1.QuadPart - t0.QuadPart) / ((double)g_qpc_freq.QuadPart / 1000.0);
    g_b142f80_sum_ms += ms; if (ms > g_b142f80_max_ms) g_b142f80_max_ms = ms;
    g_b142f80_count++; return r;
}

/* FUN_140141f70 — synchronous asset load via MEG archive search (FUN_1402136f0).
 * 2 args: (asset_db, path_char*) — returns bool (loaded). MEG archive I/O path.
 * FIX: "not found" result cache — paths that returned 0 are permanently absent from
 * the MEG archive and never need to be re-probed. Caching them eliminates the
 * ~9ms × 119-probe × N-movement-order stall on repeat calls for the same ship type. */

/* djb2 hash over a NUL-terminated ASCII string. */
static uint32_t djb2(const char *s) {
    uint32_t h = 5381;
    unsigned char c;
    while ((c = (unsigned char)*s++)) h = ((h << 5) + h) ^ c;
    return h;
}

/* djb2 with case-folding and slash normalization — used for MEG filename index. */
static uint32_t djb2u(const char *s) {
    uint32_t h = 5381;
    unsigned char c;
    while ((c = (unsigned char)*s++)) {
        if (c == '\\') c = '/';
        if (c >= 'a' && c <= 'z') c -= 32;
        h = ((h << 5) + h) ^ c;
    }
    return h;
}

#define NF_SLOTS 65536   /* must be power of 2; handles ~24k unique paths at <40% load */
static uint32_t g_nf_hash[NF_SLOTS];
static BOOL     g_nf_used[NF_SLOTS];
static LONG     g_nf_hits  = 0;   /* cache hits (probes skipped) */
static LONG     g_nf_miss  = 0;   /* cache misses (real MEG probes, result=0, inserted) */

static BOOL nf_lookup(uint32_t h) {
    uint32_t slot = h & (NF_SLOTS - 1);
    for (int i = 0; i < 16; i++) {
        uint32_t s = (slot + i) & (NF_SLOTS - 1);
        if (!g_nf_used[s]) return FALSE;
        if (g_nf_hash[s] == h) return TRUE;
    }
    return FALSE;
}

static void nf_insert(uint32_t h) {
    uint32_t slot = h & (NF_SLOTS - 1);
    for (int i = 0; i < 16; i++) {
        uint32_t s = (slot + i) & (NF_SLOTS - 1);
        if (!g_nf_used[s]) { g_nf_hash[s] = h; g_nf_used[s] = TRUE; return; }
        if (g_nf_hash[s] == h) return;   /* already present */
    }
    /* probe chain full — drop silently (conservative: just takes the slow path) */
}

/* =========================================================================
 * MEG filename index (Fix E) — built at DLL init by scanning Data\*.meg headers.
 * Lets b2136f0_hook skip the ~9ms vtable MEG scan for paths absent from all
 * MEG archives.  Hash keys use djb2u (uppercase, backslash→slash).
 * ========================================================================= */
#define MEG_SLOTS 262144  /* 256K power-of-2; handles ~50k filenames at <20% load */
static uint32_t g_meg_hash[MEG_SLOTS];
static BOOL     g_meg_used[MEG_SLOTS];
static int      g_meg_file_count = 0;
static BOOL     g_meg_ready = FALSE;

static void meg_insert(uint32_t h) {
    uint32_t slot = h & (MEG_SLOTS - 1);
    for (int i = 0; i < 32; i++) {
        uint32_t s = (slot + i) & (MEG_SLOTS - 1);
        if (!g_meg_used[s]) { g_meg_hash[s] = h; g_meg_used[s] = TRUE; return; }
        if (g_meg_hash[s] == h) return;
    }
    /* chain full — silent drop; path takes the slow scan path */
}

static BOOL meg_has(const char *path) {
    if (!g_meg_ready) return TRUE;  /* index not built yet — allow all */
    /* MEG filenames are stored as "DATA\ART\..." (DATA\ prefix, backslashes, uppercase).
     * Game queries arrive as "data/art/..." (lowercase, forward slashes, same DATA prefix).
     * djb2u normalizes both to "DATA/ART/..." before hashing — no prefix stripping needed. */
    uint32_t h = djb2u(path);
    uint32_t slot = h & (MEG_SLOTS - 1);
    for (int i = 0; i < 32; i++) {
        uint32_t s = (slot + i) & (MEG_SLOTS - 1);
        if (!g_meg_used[s]) return FALSE;
        if (g_meg_hash[s] == h) return TRUE;
    }
    return FALSE;
}

static void parse_meg(const char *meg_path) {
    FILE *f = fopen(meg_path, "rb");
    if (!f) return;
    uint32_t num_filenames = 0, num_files = 0;
    if (fread(&num_filenames, 4, 1, f) != 1 ||
        fread(&num_files,     4, 1, f) != 1) { fclose(f); return; }
    char name_buf[512];
    for (uint32_t i = 0; i < num_filenames; i++) {
        uint16_t len = 0;
        if (fread(&len, 2, 1, f) != 1) break;
        if (len == 0 || len >= (uint16_t)sizeof(name_buf)) {
            fseek(f, (long)len, SEEK_CUR);
            continue;
        }
        if (fread(name_buf, len, 1, f) != 1) break;
        name_buf[len] = '\0';
        meg_insert(djb2u(name_buf));
        g_meg_file_count++;
    }
    fclose(f);
}

static void scan_meg_dir(const char *dir_path, int *meg_count) {
    char pattern[MAX_PATH];
    snprintf(pattern, MAX_PATH, "%s\\*.meg", dir_path);
    WIN32_FIND_DATAA fd;
    HANDLE h = FindFirstFileA(pattern, &fd);
    if (h == INVALID_HANDLE_VALUE) return;
    do {
        char full[MAX_PATH];
        snprintf(full, MAX_PATH, "%s\\%s", dir_path, fd.cFileName);
        parse_meg(full);
        (*meg_count)++;
    } while (FindNextFileA(h, &fd));
    FindClose(h);
}

static void build_meg_index(void) {
    char dir[MAX_PATH];
    GetCurrentDirectoryA(MAX_PATH, dir);
    int meg_count = 0;

    /* Base game MEGs: <game>\Data\*.meg */
    char data_dir[MAX_PATH];
    snprintf(data_dir, MAX_PATH, "%s\\Data", dir);
    scan_meg_dir(data_dir, &meg_count);

    /* Workshop MEGs: go 3 levels up (corruption→EaW→common→steamapps),
     * then enumerate workshop\content\32470\<item>\Data\*.meg and one level deeper. */
    char steamapps[MAX_PATH];
    char rel3up[MAX_PATH];
    snprintf(rel3up, MAX_PATH, "%s\\..\\..\\..", dir);
    GetFullPathNameA(rel3up, MAX_PATH, steamapps, NULL);

    char workshop_base[MAX_PATH];
    snprintf(workshop_base, MAX_PATH, "%s\\workshop\\content\\32470", steamapps);

    char item_pattern[MAX_PATH];
    snprintf(item_pattern, MAX_PATH, "%s\\*", workshop_base);
    WIN32_FIND_DATAA fd;
    HANDLE h = FindFirstFileA(item_pattern, &fd);
    if (h != INVALID_HANDLE_VALUE) {
        do {
            if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) continue;
            if (fd.cFileName[0] == '.') continue;  /* skip . and .. */
            char item_data[MAX_PATH];
            snprintf(item_data, MAX_PATH, "%s\\%s\\Data", workshop_base, fd.cFileName);
            /* Scan Data/*.meg */
            scan_meg_dir(item_data, &meg_count);
            /* Scan Data/<subdir>/*.meg one level deeper (e.g. Audio/, Art/) */
            char sub_pat[MAX_PATH];
            snprintf(sub_pat, MAX_PATH, "%s\\*", item_data);
            WIN32_FIND_DATAA fd2;
            HANDLE h2 = FindFirstFileA(sub_pat, &fd2);
            if (h2 != INVALID_HANDLE_VALUE) {
                do {
                    if (!(fd2.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) continue;
                    if (fd2.cFileName[0] == '.') continue;
                    char sub_dir[MAX_PATH];
                    snprintf(sub_dir, MAX_PATH, "%s\\%s", item_data, fd2.cFileName);
                    scan_meg_dir(sub_dir, &meg_count);
                } while (FindNextFileA(h2, &fd2));
                FindClose(h2);
            }
        } while (FindNextFileA(h, &fd));
        FindClose(h);
    }

    if (meg_count == 0) {
        log_write("[eaw-mt] build_meg_index: no .meg files found\n");
        return;
    }
    g_meg_ready = TRUE;
    char m[192];
    snprintf(m, sizeof(m),
             "[eaw-mt] build_meg_index: %d .meg files, %d filenames indexed\n",
             meg_count, g_meg_file_count);
    log_write(m);
}

typedef int64_t (*B141f70Fn)(int64_t, int64_t);
static B141f70Fn g_b141f70_orig  = NULL;
static LONG      g_b141f70_count = 0;
static double    g_b141f70_sum_ms = 0, g_b141f70_max_ms = 0;
static LONG g_path_sample_count = 0;  /* one-shot sampler: log first 16 paths seen */

static int64_t b141f70_hook(int64_t a, int64_t b) {
    const char *path = (const char *)b;
    if (path) {
        long n = InterlockedIncrement(&g_path_sample_count);
        if (n <= 16 && g_log_fp) {
            fprintf(g_log_fp, "[eaw-mt] b141f70 path[%ld]: \"%s\" meg=%d\n",
                    n, path, (int)meg_has(path));
            fflush(g_log_fp);
        }
        uint32_t h = djb2(path);
        if (nf_lookup(h)) {
            InterlockedIncrement(&g_nf_hits);
            g_b141f70_count++;   /* count for profiling even though we skip the call */
            return 0;
        }
        /* Yield before entering b141f70_orig to let any game background IO thread
         * complete its work.  Without this yield the game crashes deterministically
         * during the galactic initialization asset-loading pass (Heisenbug: the crash
         * disappears when fflush adds a syscall delay before each call).
         * SwitchToThread gives the scheduler an explicit yield point with no disk I/O. */
        SwitchToThread();
        LARGE_INTEGER t0, t1; tgt_fake_qpc(&t0);
        int64_t r = g_b141f70_orig(a, b); tgt_fake_qpc(&t1);
        double ms = (t1.QuadPart - t0.QuadPart) / ((double)g_qpc_freq.QuadPart / 1000.0);
        g_b141f70_sum_ms += ms; if (ms > g_b141f70_max_ms) g_b141f70_max_ms = ms;
        g_b141f70_count++;
        if (r == 0) { nf_insert(h); InterlockedIncrement(&g_nf_miss); }
        return r;
    }
    /* NULL path — fall through to original (handles any edge case) */
    LARGE_INTEGER t0, t1; tgt_fake_qpc(&t0);
    int64_t r = g_b141f70_orig(a, b); tgt_fake_qpc(&t1);
    double ms = (t1.QuadPart - t0.QuadPart) / ((double)g_qpc_freq.QuadPart / 1000.0);
    g_b141f70_sum_ms += ms; if (ms > g_b141f70_max_ms) g_b141f70_max_ms = ms;
    g_b141f70_count++; return r;
}

static BOOL install_b25ec10_subcallee_hooks(void)
{
    HMODULE exe = GetModuleHandleA(NULL);
    if (!exe) return FALSE;

    BYTE *fn_b25ec10 = (BYTE *)exe + B25EC10_RVA;
    BYTE *fn_b142f80 = (BYTE *)exe + B142F80_RVA;
    BYTE *fn_b141f70 = (BYTE *)exe + B141F70_RVA;

    g_b142f80_orig = (B142f80Fn)fn_b142f80;
    g_b141f70_orig = (B141f70Fn)fn_b141f70;

    BYTE *stubs = alloc_near(fn_b25ec10, 14 * 2);
    if (!stubs) {
        log_write("[eaw-mt] WARN: alloc_near failed for b25ec10 sub stubs\n");
        return FALSE;
    }
    write_abs_jmp(stubs +  0, (uint64_t)b142f80_hook);
    write_abs_jmp(stubs + 14, (uint64_t)b141f70_hook);

    int n142f80 = 0, n141f70 = 0;
    DWORD old_prot;
    for (int i = 0; i <= B25EC10_BODY_SIZE - 5; i++) {
        if (fn_b25ec10[i] != 0xE8) continue;
        int32_t rel;
        memcpy(&rel, fn_b25ec10 + i + 1, 4);
        BYTE *target = fn_b25ec10 + i + 5 + rel;
        BYTE *stub = NULL; int *cnt = NULL;
        if      (target == fn_b142f80) { stub = stubs +  0; cnt = &n142f80; }
        else if (target == fn_b141f70) { stub = stubs + 14; cnt = &n141f70; }
        else continue;
        int32_t new_rel = (int32_t)(stub - (fn_b25ec10 + i + 5));
        VirtualProtect(fn_b25ec10 + i + 1, 4, PAGE_EXECUTE_READWRITE, &old_prot);
        memcpy(fn_b25ec10 + i + 1, &new_rel, 4);
        VirtualProtect(fn_b25ec10 + i + 1, 4, old_prot, &old_prot);
        FlushInstructionCache(GetCurrentProcess(), fn_b25ec10 + i, 5);
        (*cnt)++;
    }
    char m[128];
    sprintf(m, "[eaw-mt] b25ec10_sub: b142f80=%d b141f70=%d site(s) patched in 25ec10\n",
            n142f80, n141f70);
    log_write(m);
    return (n142f80 + n141f70) > 0;
}

/* FUN_1402136f0 — MEG archive searcher, 5 args: (longlong* asset_db, longlong path, uint, u1, u1).
 * Called at 2 sites in FUN_140141f70 (+0x081, +0x126).
 * FIX E: if path not in any MEG archive, return 0 without the ~9ms vtable scan. */
typedef int64_t (*B2136f0Fn)(int64_t, int64_t, int64_t, int64_t, int64_t);
static B2136f0Fn g_b2136f0_orig  = NULL;
static LONG      g_b2136f0_count = 0;
static LONG      g_b2136f0_skip  = 0;
static double    g_b2136f0_sum_ms = 0, g_b2136f0_max_ms = 0;

static int64_t b2136f0_hook(int64_t a, int64_t b, int64_t c, int64_t d, int64_t e) {
    /* Fix E abandoned: vtable+0x18 does MEG scan AND filesystem fallback.
     * Returning 0 for meg=0 paths skips the filesystem fallback, breaking loose-file assets.
     * Call-through always; Fix A's b141f70 NF cache handles repeat stalls. */
    LARGE_INTEGER t0, t1; tgt_fake_qpc(&t0);
    int64_t r = g_b2136f0_orig(a, b, c, d, e); tgt_fake_qpc(&t1);
    double ms = (t1.QuadPart - t0.QuadPart) / ((double)g_qpc_freq.QuadPart / 1000.0);
    g_b2136f0_sum_ms += ms; if (ms > g_b2136f0_max_ms) g_b2136f0_max_ms = ms;
    g_b2136f0_count++;
    return r;
}

static BOOL install_b141f70_subcallee_hooks(void)
{
    HMODULE exe = GetModuleHandleA(NULL);
    if (!exe) return FALSE;

    BYTE *fn_b141f70 = (BYTE *)exe + B141F70_RVA;
    BYTE *fn_b2136f0 = (BYTE *)exe + B2136F0_RVA;

    g_b2136f0_orig = (B2136f0Fn)fn_b2136f0;

    BYTE *stubs = alloc_near(fn_b141f70, 14);
    if (!stubs) {
        log_write("[eaw-mt] WARN: alloc_near failed for b141f70 sub stubs\n");
        return FALSE;
    }
    write_abs_jmp(stubs, (uint64_t)b2136f0_hook);

    int n2136f0 = 0;
    DWORD old_prot;
    for (int i = 0; i <= B141F70_BODY_SIZE - 5; i++) {
        if (fn_b141f70[i] != 0xE8) continue;
        int32_t rel;
        memcpy(&rel, fn_b141f70 + i + 1, 4);
        BYTE *target = fn_b141f70 + i + 5 + rel;
        if (target != fn_b2136f0) continue;
        int32_t new_rel = (int32_t)(stubs - (fn_b141f70 + i + 5));
        VirtualProtect(fn_b141f70 + i + 1, 4, PAGE_EXECUTE_READWRITE, &old_prot);
        memcpy(fn_b141f70 + i + 1, &new_rel, 4);
        VirtualProtect(fn_b141f70 + i + 1, 4, old_prot, &old_prot);
        FlushInstructionCache(GetCurrentProcess(), fn_b141f70 + i, 5);
        n2136f0++;
    }
    char m[128];
    sprintf(m, "[eaw-mt] b141f70_sub: b2136f0=%d site(s) patched in 141f70\n", n2136f0);
    log_write(m);
    return n2136f0 > 0;
}

typedef void (*B38cb30Fn)(int64_t, int64_t, int64_t, int64_t);
static B38cb30Fn g_b38cb30_orig  = NULL;
static LONG      g_b38cb30_count = 0;
static double    g_b38cb30_sum_ms = 0, g_b38cb30_max_ms = 0;
static void b38cb30_hook(int64_t a, int64_t b, int64_t c, int64_t d) {
    LARGE_INTEGER t0, t1; tgt_fake_qpc(&t0);
    g_b38cb30_orig(a, b, c, d); tgt_fake_qpc(&t1);
    double ms = (t1.QuadPart - t0.QuadPart) / ((double)g_qpc_freq.QuadPart / 1000.0);
    g_b38cb30_sum_ms += ms; if (ms > g_b38cb30_max_ms) g_b38cb30_max_ms = ms;
    g_b38cb30_count++;
}

/* FUN_140381a90  per-hardpoint sub-movement-order creation (lines 300-301 of 3989a0).
 * Called once per ship hardpoint.  5 args from call site:
 *   (alloc'd 0xd8 buf, move_order, loop_idx, hardpoint_ptr, in_stack_val).
 * 5th arg is in_stack_ffffffffffffff88 — must pass through to avoid stack corruption. */
typedef int64_t (*B381a90Fn)(int64_t, int64_t, int64_t, int64_t, int64_t);
static B381a90Fn g_b381a90_orig  = NULL;
static LONG      g_b381a90_count = 0;
static double    g_b381a90_sum_ms = 0, g_b381a90_max_ms = 0;
static int64_t b381a90_hook(int64_t a, int64_t b, int64_t c, int64_t d, int64_t e) {
    LARGE_INTEGER t0, t1; tgt_fake_qpc(&t0);
    int64_t r = g_b381a90_orig(a, b, c, d, e); tgt_fake_qpc(&t1);
    double ms = (t1.QuadPart - t0.QuadPart) / ((double)g_qpc_freq.QuadPart / 1000.0);
    g_b381a90_sum_ms += ms; if (ms > g_b381a90_max_ms) g_b381a90_max_ms = ms;
    g_b381a90_count++; return r;
}

/* FUN_140384740  per-hardpoint second pass (lines 396 of 3989a0, inside second loop).
 * Ghidra call site showed 0 args but function decompile has longlong param_1 in RCX.
 * Must pass through — it dereferences param_1+0x20 on entry. */
typedef void (*B384740Fn)(int64_t);
static B384740Fn g_b384740_orig  = NULL;
static LONG      g_b384740_count = 0;
static double    g_b384740_sum_ms = 0, g_b384740_max_ms = 0;
static void b384740_hook(int64_t a) {
    LARGE_INTEGER t0, t1; tgt_fake_qpc(&t0);
    g_b384740_orig(a); tgt_fake_qpc(&t1);
    double ms = (t1.QuadPart - t0.QuadPart) / ((double)g_qpc_freq.QuadPart / 1000.0);
    g_b384740_sum_ms += ms; if (ms > g_b384740_max_ms) g_b384740_max_ms = ms;
    g_b384740_count++;
}

/* FUN_14037c050 takes 5 args: (path_buf+8, move_order, param_2, param_3, vtbl_ptr)
 * The 5th arg is *(undefined8*)(*(longlong*)(param_1+0x2b8)+0x20) — must be passed
 * through or the callee reads garbage off the stack and crashes. */
typedef void (*B37c050Fn)(int64_t, int64_t, int64_t, int64_t, int64_t);
static B37c050Fn g_b37c050_orig  = NULL;
static LONG      g_b37c050_count = 0;
static double    g_b37c050_sum_ms = 0, g_b37c050_max_ms = 0;
static void b37c050_hook(int64_t a, int64_t b, int64_t c, int64_t d, int64_t e) {
    LARGE_INTEGER t0, t1; tgt_fake_qpc(&t0);
    g_b37c050_orig(a, b, c, d, e); tgt_fake_qpc(&t1);
    double ms = (t1.QuadPart - t0.QuadPart) / ((double)g_qpc_freq.QuadPart / 1000.0);
    g_b37c050_sum_ms += ms; if (ms > g_b37c050_max_ms) g_b37c050_max_ms = ms;
    g_b37c050_count++;
}

static BOOL install_b3989a0_subcallee_hooks(void)
{
    HMODULE exe = GetModuleHandleA(NULL);
    if (!exe) return FALSE;

    BYTE *fn_b3989a0 = (BYTE *)exe + B3989A0_RVA;
    BYTE *fn_b38cb30 = (BYTE *)exe + B38CB30_RVA;
    BYTE *fn_b37c050 = (BYTE *)exe + B37C050_RVA;
    BYTE *fn_b381a90 = (BYTE *)exe + B381A90_RVA;
    BYTE *fn_b384740 = (BYTE *)exe + B384740_RVA;
    BYTE *fn_b3727a0 = (BYTE *)exe + B3727A0_RVA;
    BYTE *fn_b3a4820 = (BYTE *)exe + B3A4820_RVA;
    BYTE *fn_b3a8710 = (BYTE *)exe + B3A8710_RVA;
    BYTE *fn_b3ac530 = (BYTE *)exe + B3AC530_RVA;
    BYTE *fn_b38cf30 = (BYTE *)exe + B38CF30_RVA;
    BYTE *fn_b3a59f0 = (BYTE *)exe + B3A59F0_RVA;

    g_b38cb30_orig = (B38cb30Fn)fn_b38cb30;
    g_b37c050_orig = (B37c050Fn)fn_b37c050;
    g_b381a90_orig = (B381a90Fn)fn_b381a90;
    g_b384740_orig = (B384740Fn)fn_b384740;
    g_b3727a0_orig = (B3727a0Fn)fn_b3727a0;
    g_b3a4820_orig = (B3a4820Fn)fn_b3a4820;
    g_b3a8710_orig = (B3a8710Fn)fn_b3a8710;
    g_b3ac530_orig = (B3ac530Fn)fn_b3ac530;
    g_b38cf30_orig = (B38cf30Fn)fn_b38cf30;
    g_b3a59f0_orig = (B3a59f0Fn)fn_b3a59f0;

    BYTE *stubs = alloc_near(fn_b3989a0, 14 * 10);
    if (!stubs) {
        log_write("[eaw-mt] WARN: alloc_near failed for b3989a0 sub stubs\n");
        return FALSE;
    }
    write_abs_jmp(stubs +   0, (uint64_t)b38cb30_hook);
    write_abs_jmp(stubs +  14, (uint64_t)b37c050_hook);
    write_abs_jmp(stubs +  28, (uint64_t)b381a90_hook);
    write_abs_jmp(stubs +  42, (uint64_t)b384740_hook);
    write_abs_jmp(stubs +  56, (uint64_t)b3727a0_hook);
    write_abs_jmp(stubs +  70, (uint64_t)b3a4820_hook);
    write_abs_jmp(stubs +  84, (uint64_t)b3a8710_hook);
    write_abs_jmp(stubs +  98, (uint64_t)b3ac530_hook);
    write_abs_jmp(stubs + 112, (uint64_t)b38cf30_hook);
    write_abs_jmp(stubs + 126, (uint64_t)b3a59f0_hook);

    int n38cb30 = 0, n37c050 = 0, n381a90 = 0, n384740 = 0;
    int n3727a0 = 0, n3a4820 = 0, n3a8710 = 0, n3ac530 = 0, n38cf30 = 0, n3a59f0 = 0;
    DWORD old_prot;
    for (int i = 0; i <= B3989A0_BODY_SIZE - 5; i++) {
        if (fn_b3989a0[i] != 0xE8) continue;
        int32_t rel;
        memcpy(&rel, fn_b3989a0 + i + 1, 4);
        BYTE *target = fn_b3989a0 + i + 5 + rel;
        BYTE *stub = NULL; int *cnt = NULL;
        if      (target == fn_b38cb30) { stub = stubs +   0; cnt = &n38cb30; }
        else if (target == fn_b37c050) { stub = stubs +  14; cnt = &n37c050; }
        else if (target == fn_b381a90) { stub = stubs +  28; cnt = &n381a90; }
        else if (target == fn_b384740) { stub = stubs +  42; cnt = &n384740; }
        else if (target == fn_b3727a0) { stub = stubs +  56; cnt = &n3727a0; }
        else if (target == fn_b3a4820) { stub = stubs +  70; cnt = &n3a4820; }
        else if (target == fn_b3a8710) { stub = stubs +  84; cnt = &n3a8710; }
        else if (target == fn_b3ac530) { stub = stubs +  98; cnt = &n3ac530; }
        else if (target == fn_b38cf30) { stub = stubs + 112; cnt = &n38cf30; }
        else if (target == fn_b3a59f0) { stub = stubs + 126; cnt = &n3a59f0; }
        else continue;
        int32_t new_rel = (int32_t)(stub - (fn_b3989a0 + i + 5));
        VirtualProtect(fn_b3989a0 + i + 1, 4, PAGE_EXECUTE_READWRITE, &old_prot);
        memcpy(fn_b3989a0 + i + 1, &new_rel, 4);
        VirtualProtect(fn_b3989a0 + i + 1, 4, old_prot, &old_prot);
        FlushInstructionCache(GetCurrentProcess(), fn_b3989a0 + i, 5);
        (*cnt)++;
    }
    char m[256];
    sprintf(m, "[eaw-mt] b3989a0_sub: b38cb30=%d b37c050=%d b381a90=%d b384740=%d"
               " b3727a0=%d b3a4820=%d b3a8710=%d b3ac530=%d b38cf30=%d b3a59f0=%d patched\n",
            n38cb30, n37c050, n381a90, n384740,
            n3727a0, n3a4820, n3a8710, n3ac530, n38cf30, n3a59f0);
    log_write(m);
    return (n38cb30 + n37c050 + n381a90 + n384740 +
            n3727a0 + n3a4820 + n3a8710 + n3ac530 + n38cf30 + n3a59f0) > 0;
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
    tgt_fake_qpc(&t0);
    g_ga4d0_orig(a);
    tgt_fake_qpc(&t1);
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
 * FUN_1402a7190 null-argument guard (Phase 5 Step 29k)
 *
 * FUN_14028a4d0 calls FUN_1402a7190(*(lVar2+0x20)) for each battle-mode array
 * entry.  After prewarm, Lua AI callbacks leave some *(lVar2+0x20) = 0; the
 * first game-loop ga4d0 call then passes null to FUN_1402a7190, which crashes
 * at its first instruction (MOV RCX,[RCX+0x20]) with RCX=0.
 *
 * Fix: scan FUN_14028a4d0's body for CALL FUN_1402a7190 and redirect each to
 * a7190_guard, which skips the call when arg=0.  In normal game operation
 * (no prewarm), FUN_1402a7190 is never called with null, so the guard never
 * fires and behavior is unchanged.
 * ========================================================================= */
typedef void (*A7190Fn)(int64_t);
static A7190Fn g_a7190_orig = NULL;

static void a7190_guard(int64_t arg)
{
    if (!arg) return;
    g_a7190_orig(arg);
}

static BOOL install_a7190_guard(void)
{
    HMODULE exe = GetModuleHandleA(NULL);
    if (!exe) return FALSE;

    BYTE *fn_ga4d0 = (BYTE *)exe + GA4D0_RVA;
    BYTE *fn_a7190 = (BYTE *)exe + A7190_RVA;
    g_a7190_orig   = (A7190Fn)fn_a7190;

    BYTE *stub = alloc_near(fn_ga4d0, 14);
    if (!stub) { log_write("[eaw-mt] WARN: alloc_near failed for a7190 guard stub\n"); return FALSE; }
    write_abs_jmp(stub, (uint64_t)a7190_guard);

    int n = 0;
    for (int i = 0; i <= GA4D0_SIZE - 5; i++) {
        if (fn_ga4d0[i] != 0xE8) continue;
        int32_t rel;
        memcpy(&rel, fn_ga4d0 + i + 1, 4);
        BYTE *target = fn_ga4d0 + i + 5 + (ptrdiff_t)rel;
        if (target != fn_a7190) continue;
        int32_t new_rel = (int32_t)(stub - (fn_ga4d0 + i + 5));
        DWORD old;
        VirtualProtect(fn_ga4d0 + i + 1, 4, PAGE_EXECUTE_READWRITE, &old);
        memcpy(fn_ga4d0 + i + 1, &new_rel, 4);
        VirtualProtect(fn_ga4d0 + i + 1, 4, old, &old);
        FlushInstructionCache(GetCurrentProcess(), fn_ga4d0 + i, 5);
        char m[64];
        sprintf(m, "[eaw-mt] a7190-guard: patched ga4d0+0x%x\n", i);
        log_write(m);
        n++;
    }
    char m[64];
    sprintf(m, "[eaw-mt] a7190 guard: %d callsite(s) patched in FUN_14028a4d0\n", n);
    log_write(m);
    return n > 0;
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
    g_pumpe_frame_used_ms = 0.0;  /* reset per-gsvc Lua AI budget */
    LARGE_INTEGER t0, t1;
    tgt_fake_qpc(&t0);
    g_gsvc_orig(a, b);
    tgt_fake_qpc(&t1);
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
    tgt_fake_qpc(&t0);
    g_slot22_tail_orig(a, b, c, d);
    tgt_fake_qpc(&t1);
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
    tgt_fake_qpc(&t0);
    int64_t r = g_sigdisp_orig(a, b, c, d, e, f);
    tgt_fake_qpc(&t1);
    double ms = (t1.QuadPart - t0.QuadPart) / ((double)g_qpc_freq.QuadPart / 1000.0);
    g_sigdisp_sum_ms += ms;
    if (ms > g_sigdisp_max_ms) g_sigdisp_max_ms = ms;
    g_sigdisp_count++;
    return r;
}

static int64_t battle_select_hook(void *a, void *b) {
    LARGE_INTEGER t0, t1;
    tgt_fake_qpc(&t0);
    int64_t r = g_bsel_orig(a, b);
    tgt_fake_qpc(&t1);
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
        char ibuf[3200];
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
        double b19n  = (double)(g_b385190_count  ? g_b385190_count  : 1);
        double b850n = (double)(g_b384850_count  ? g_b384850_count  : 1);
        double b5b0n = (double)(g_b3825b0_count  ? g_b3825b0_count  : 1);
        double be70n = (double)(g_b385e70_count  ? g_b385e70_count  : 1);
        double r450n = (double)(g_b399450_count  ? g_b399450_count  : 1);
        double d1c0n = (double)(g_b381dc0_count  ? g_b381dc0_count  : 1);
        double f810n = (double)(g_b29f810_count  ? g_b29f810_count  : 1);
        double b88n  = (double)(g_b388b60_count  ? g_b388b60_count  : 1);
        double r9a0n = (double)(g_b3989a0_count  ? g_b3989a0_count  : 1);
        double f0n2  = (double)(g_b3729f0_count  ? g_b3729f0_count  : 1);
        double a84n  = (double)(g_b3a5840_count  ? g_b3a5840_count  : 1);
        double s60n  = (double)(g_b265560_count  ? g_b265560_count  : 1);
        double cb30n = (double)(g_b38cb30_count  ? g_b38cb30_count  : 1);
        double c050n = (double)(g_b37c050_count  ? g_b37c050_count  : 1);
        double a90n  = (double)(g_b381a90_count  ? g_b381a90_count  : 1);
        double t740n = (double)(g_b384740_count  ? g_b384740_count  : 1);
        double a7a0n = (double)(g_b3727a0_count  ? g_b3727a0_count  : 1);
        double a820n = (double)(g_b3a4820_count  ? g_b3a4820_count  : 1);
        double a710n = (double)(g_b3a8710_count  ? g_b3a8710_count  : 1);
        double ac30n = (double)(g_b3ac530_count  ? g_b3ac530_count  : 1);
        double cf30n = (double)(g_b38cf30_count  ? g_b38cf30_count  : 1);
        double f0n   = (double)(g_b3a59f0_count  ? g_b3a59f0_count  : 1);
        double ec10n = (double)(g_b25ec10_count  ? g_b25ec10_count  : 1);
        double f8f0n = (double)(g_b3718f0_count  ? g_b3718f0_count  : 1);
        double f80n  = (double)(g_b142f80_count  ? g_b142f80_count  : 1);
        double f70n  = (double)(g_b141f70_count  ? g_b141f70_count  : 1);
        double f36n  = (double)(g_b2136f0_count  ? g_b2136f0_count  : 1);
        sprintf(ibuf,
            "[eaw-mt] gsvc(28d400):    avg=%.2f max=%.2f ms (n=%ld)\n"
            "  sslot22(4d95a0): avg=%.2f max=%.2f ms (n=%ld)\n"
            "  tail22i(3639d0): avg=%.2f max=%.2f ms (n=%ld)\n"
            "  t2be640(2be640): avg=%.2f max=%.2f ms (n=%ld)\n"
            "  ta6b80(3a6b80):  avg=%.2f max=%.2f ms (n=%ld)\n"
            "  pumpe(247a90):   avg=%.2f max=%.2f ms (n=%ld skip=%ld bg=%ld bg_max=%.0fms wd=%ld rv_n=%ld rv_max=%ldms rv_slow=%ld)\n"
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
            "  bsel(343b90):    avg=%.2f max=%.2f ms (n=%ld)\n"
            "  b385190(385190): avg=%.2f max=%.2f ms (n=%ld)\n"
            "  b385190_capped:  %ld\n"
            "  b384850(384850): avg=%.2f max=%.2f ms (n=%ld)\n"
            "  b3825b0(3825b0): avg=%.2f max=%.2f ms (n=%ld)\n"
            "  b385e70(385e70): avg=%.2f max=%.2f ms (n=%ld)\n"
            "  b399450(399450): avg=%.2f max=%.2f ms (n=%ld)\n"
            "  b381dc0(381dc0): avg=%.2f max=%.2f ms (n=%ld)\n"
            "  b29f810(29f810): avg=%.2f max=%.2f ms (n=%ld)\n"
            "  b388b60(388b60): avg=%.2f max=%.2f ms (n=%ld)\n"
            "  b3989a0(3989a0): avg=%.2f max=%.2f ms (n=%ld)\n"
            "  b38cb30(38cb30): avg=%.2f max=%.2f ms (n=%ld)\n"
            "  b37c050(37c050): avg=%.2f max=%.2f ms (n=%ld)\n"
            "  b381a90(381a90): avg=%.2f max=%.2f ms (n=%ld)\n"
            "  b384740(384740): avg=%.2f max=%.2f ms (n=%ld)\n"
            "  b3729f0(3729f0): avg=%.2f max=%.2f ms (n=%ld)\n"
            "  b3a5840(3a5840): avg=%.2f max=%.2f ms (n=%ld)\n"
            "  b265560(265560): avg=%.2f max=%.2f ms (n=%ld)\n"
            "  b3727a0(3727a0): avg=%.2f max=%.2f ms (n=%ld)\n"
            "  b3a4820(3a4820): avg=%.2f max=%.2f ms (n=%ld)\n"
            "  b3a8710(3a8710): avg=%.2f max=%.2f ms (n=%ld)\n"
            "  b3ac530(3ac530): avg=%.2f max=%.2f ms (n=%ld)\n"
            "  b38cf30(38cf30): avg=%.2f max=%.2f ms (n=%ld)\n"
            "  b3a59f0(3a59f0): avg=%.2f max=%.2f ms (n=%ld)\n"
            "  b25ec10(25ec10): avg=%.2f max=%.2f ms (n=%ld)\n"
            "  b3718f0(3718f0): avg=%.2f max=%.2f ms (n=%ld)\n"
            "  b142f80(142f80): avg=%.2f max=%.2f ms (n=%ld)\n"
            "  b141f70(141f70): avg=%.2f max=%.2f ms (n=%ld)\n"
            "  nf_cache: hits=%ld miss=%ld\n"
            "  b2136f0(2136f0): avg=%.2f max=%.2f ms (n=%ld skip=%ld)\n"
            "  meg_idx: files=%d ready=%d\n",
            g_gsvc_sum_ms    / gn,   g_gsvc_max_ms,    (long)g_gsvc_count,
            g_sslot22_sum_ms / ssn,  g_sslot22_max_ms, (long)g_sslot22_count,
            g_tail22i_sum_ms / t22n, g_tail22i_max_ms, (long)g_tail22i_count,
            g_t2be640_sum_ms / t2bn, g_t2be640_max_ms, (long)g_t2be640_count,
            g_ta6b80_sum_ms  / a6n,  g_ta6b80_max_ms,  (long)g_ta6b80_count,
            g_pumpe_sum_ms   / pen,  g_pumpe_max_ms,   (long)g_pumpe_count, (long)g_pumpe_skip,
            (long)g_pumpe_bg_dispatched, g_pumpe_bg_max_ms, (long)g_wd_fired,
            (long)g_resume_n, (long)g_resume_max_ms, (long)g_resume_slow,
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
            g_bsel_sum_ms    / bn,   g_bsel_max_ms,    (long)g_bsel_count,
            g_b385190_sum_ms / b19n, g_b385190_max_ms, (long)g_b385190_count,
            (long)g_b385190_capped,
            g_b384850_sum_ms / b850n, g_b384850_max_ms, (long)g_b384850_count,
            g_b3825b0_sum_ms / b5b0n, g_b3825b0_max_ms, (long)g_b3825b0_count,
            g_b385e70_sum_ms / be70n, g_b385e70_max_ms, (long)g_b385e70_count,
            g_b399450_sum_ms / r450n, g_b399450_max_ms, (long)g_b399450_count,
            g_b381dc0_sum_ms / d1c0n, g_b381dc0_max_ms, (long)g_b381dc0_count,
            g_b29f810_sum_ms / f810n, g_b29f810_max_ms, (long)g_b29f810_count,
            g_b388b60_sum_ms / b88n,  g_b388b60_max_ms, (long)g_b388b60_count,
            g_b3989a0_sum_ms / r9a0n, g_b3989a0_max_ms, (long)g_b3989a0_count,
            g_b38cb30_sum_ms / cb30n, g_b38cb30_max_ms, (long)g_b38cb30_count,
            g_b37c050_sum_ms / c050n, g_b37c050_max_ms, (long)g_b37c050_count,
            g_b381a90_sum_ms / a90n,  g_b381a90_max_ms, (long)g_b381a90_count,
            g_b384740_sum_ms / t740n, g_b384740_max_ms, (long)g_b384740_count,
            g_b3729f0_sum_ms / f0n2,  g_b3729f0_max_ms, (long)g_b3729f0_count,
            g_b3a5840_sum_ms / a84n,  g_b3a5840_max_ms, (long)g_b3a5840_count,
            g_b265560_sum_ms / s60n,  g_b265560_max_ms, (long)g_b265560_count,
            g_b3727a0_sum_ms / a7a0n, g_b3727a0_max_ms, (long)g_b3727a0_count,
            g_b3a4820_sum_ms / a820n, g_b3a4820_max_ms, (long)g_b3a4820_count,
            g_b3a8710_sum_ms / a710n, g_b3a8710_max_ms, (long)g_b3a8710_count,
            g_b3ac530_sum_ms / ac30n, g_b3ac530_max_ms, (long)g_b3ac530_count,
            g_b38cf30_sum_ms / cf30n, g_b38cf30_max_ms, (long)g_b38cf30_count,
            g_b3a59f0_sum_ms / f0n,   g_b3a59f0_max_ms, (long)g_b3a59f0_count,
            g_b25ec10_sum_ms / ec10n, g_b25ec10_max_ms, (long)g_b25ec10_count,
            g_b3718f0_sum_ms / f8f0n, g_b3718f0_max_ms, (long)g_b3718f0_count,
            g_b142f80_sum_ms / f80n,  g_b142f80_max_ms, (long)g_b142f80_count,
            g_b141f70_sum_ms / f70n,  g_b141f70_max_ms, (long)g_b141f70_count,
            (long)g_nf_hits, (long)g_nf_miss,
            g_b2136f0_sum_ms / f36n,  g_b2136f0_max_ms, (long)g_b2136f0_count, (long)g_b2136f0_skip,
            g_meg_file_count, (int)g_meg_ready);
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
    g_pumpe_count   = 0; g_pumpe_sum_ms   = 0; g_pumpe_max_ms   = 0; g_pumpe_skip = 0;
    g_pumpe_bg_dispatched = 0; g_pumpe_bg_sum_ms = 0; g_pumpe_bg_max_ms = 0;
    InterlockedExchange(&g_wd_fired, 0);
    InterlockedExchange(&g_resume_n, 0);
    InterlockedExchange(&g_resume_slow, 0);
    InterlockedExchange(&g_resume_max_ms, 0);
    g_a76b0_count   = 0; g_a76b0_sum_ms   = 0; g_a76b0_max_ms   = 0;
    g_a9e30_count   = 0; g_a9e30_sum_ms   = 0; g_a9e30_max_ms   = 0;
    g_e369e0_count  = 0; g_e369e0_sum_ms  = 0; g_e369e0_max_ms  = 0;
    g_b87010_count  = 0; g_b87010_sum_ms  = 0; g_b87010_max_ms  = 0;
    g_d12d520_count  = 0; g_d12d520_sum_ms  = 0; g_d12d520_max_ms  = 0;
    g_d12d480_count  = 0; g_d12d480_sum_ms  = 0; g_d12d480_max_ms  = 0;
    g_b387400_count  = 0; g_b387400_sum_ms  = 0; g_b387400_max_ms  = 0;
    g_b385cf0_count  = 0; g_b385cf0_sum_ms  = 0; g_b385cf0_max_ms  = 0;
    g_b381ff0_count  = 0; g_b381ff0_sum_ms  = 0; g_b381ff0_max_ms  = 0;
    g_b385190_count  = 0; g_b385190_sum_ms  = 0; g_b385190_max_ms  = 0; g_b385190_capped = 0;
    g_b384850_count  = 0; g_b384850_sum_ms  = 0; g_b384850_max_ms  = 0;
    g_b3825b0_count  = 0; g_b3825b0_sum_ms  = 0; g_b3825b0_max_ms  = 0;
    g_b385e70_count  = 0; g_b385e70_sum_ms  = 0; g_b385e70_max_ms  = 0;
    g_b399450_count  = 0; g_b399450_sum_ms  = 0; g_b399450_max_ms  = 0;
    g_b381dc0_count  = 0; g_b381dc0_sum_ms  = 0; g_b381dc0_max_ms  = 0;
    g_b29f810_count  = 0; g_b29f810_sum_ms  = 0; g_b29f810_max_ms  = 0;
    g_b388b60_count  = 0; g_b388b60_sum_ms  = 0; g_b388b60_max_ms  = 0;
    g_b3989a0_count  = 0; g_b3989a0_sum_ms  = 0; g_b3989a0_max_ms  = 0;
    g_b38cb30_count  = 0; g_b38cb30_sum_ms  = 0; g_b38cb30_max_ms  = 0;
    g_b37c050_count  = 0; g_b37c050_sum_ms  = 0; g_b37c050_max_ms  = 0;
    g_b381a90_count  = 0; g_b381a90_sum_ms  = 0; g_b381a90_max_ms  = 0;
    g_b384740_count  = 0; g_b384740_sum_ms  = 0; g_b384740_max_ms  = 0;
    g_b3729f0_count  = 0; g_b3729f0_sum_ms  = 0; g_b3729f0_max_ms  = 0;
    g_b3a5840_count  = 0; g_b3a5840_sum_ms  = 0; g_b3a5840_max_ms  = 0;
    g_b265560_count  = 0; g_b265560_sum_ms  = 0; g_b265560_max_ms  = 0;
    g_b3727a0_count  = 0; g_b3727a0_sum_ms  = 0; g_b3727a0_max_ms  = 0;
    g_b3a4820_count  = 0; g_b3a4820_sum_ms  = 0; g_b3a4820_max_ms  = 0;
    g_b3a8710_count  = 0; g_b3a8710_sum_ms  = 0; g_b3a8710_max_ms  = 0;
    g_b3ac530_count  = 0; g_b3ac530_sum_ms  = 0; g_b3ac530_max_ms  = 0;
    g_b38cf30_count  = 0; g_b38cf30_sum_ms  = 0; g_b38cf30_max_ms  = 0;
    g_b3a59f0_count  = 0; g_b3a59f0_sum_ms  = 0; g_b3a59f0_max_ms  = 0;
    g_b25ec10_count  = 0; g_b25ec10_sum_ms  = 0; g_b25ec10_max_ms  = 0;
    g_b3718f0_count  = 0; g_b3718f0_sum_ms  = 0; g_b3718f0_max_ms  = 0;
    g_b142f80_count  = 0; g_b142f80_sum_ms  = 0; g_b142f80_max_ms  = 0;
    g_b141f70_count  = 0; g_b141f70_sum_ms  = 0; g_b141f70_max_ms  = 0;
    g_nf_hits = 0; g_nf_miss = 0;   /* counters reset per window; hash table persists */
    g_b2136f0_count  = 0; g_b2136f0_sum_ms  = 0; g_b2136f0_max_ms  = 0; g_b2136f0_skip = 0;
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
    g_render_thread_id = GetCurrentThreadId();
    log_write("[eaw-mt] Render thread started\n");

    while (!InterlockedCompareExchange(&g_shutdown, 0, 0)) {
        DWORD wait = WaitForSingleObject(g_render_kick, 1000);
        if (wait == WAIT_TIMEOUT) continue;
        if (InterlockedCompareExchange(&g_shutdown, 0, 0)) break;

        tgt_fake_qpc(&g_flush_start);
        /* Full flush+drain via trampoline (FUN_140180dc0 + FUN_140149650).
         * Main thread is blocked on render_done so there is no concurrent
         * access to render task lists or the allocator pools. */
        g_trampoline((void *)g_render_param);
        tgt_fake_qpc(&g_flush_end);

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
    tgt_fake_qpc(&entry_now);
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

    /* DIAGNOSTIC: call on main thread to check if crash is render-thread-related.
     * If crash disappears → render thread is the cause; revert to BG kick. */
    tgt_fake_qpc(&g_flush_start);
    g_trampoline(param_1);
    tgt_fake_qpc(&g_flush_end);

    /* After the wait, g_flush_end is valid for THIS flush.
     * Record the inter-flush anchor now (time flush#1 finished). */
    if (is_flush1) {
        tgt_fake_qpc(&g_flush1_done);
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
 * Vectored Exception Handler — crash diagnostics
 * ========================================================================= */

static LONG WINAPI veh_crash_handler(EXCEPTION_POINTERS *ep)
{
    DWORD code = ep->ExceptionRecord->ExceptionCode;

    /* -----------------------------------------------------------------------
     * Fast path: suppress the Wine JIT-thunk wake_addr corruption crash.
     *
     * Wine's JIT syscall dispatcher does, for every pending wake-address
     * notification before each syscall:
     *
     *   TEST rax, rax              ; rax = wake_addr (ptr to waiter byte)
     *   JZ   skip                  ; null → no waiter
     *   MOV BYTE PTR [rax], 0      ; ← CRASHES if wake_addr is corrupted
     *   MOV rcx, [rcx+0x298]      ; load next waiter pointer into rcx
     *   ...
     * skip:
     *
     * Under Proton/Wine a race condition sometimes writes 0x6fff00000000
     * (invalid) into the wake_addr field of a sync object.  The write faults.
     * Fix: verify the exact 10-byte sequence, then skip both instructions so
     * rcx retains the valid kernel-object pointer and execution continues.
     * ---------------------------------------------------------------------- */
    if (code == EXCEPTION_ACCESS_VIOLATION &&
        ep->ExceptionRecord->NumberParameters >= 2 &&
        ep->ExceptionRecord->ExceptionInformation[0] == 1) {
        ULONG_PTR bad = ep->ExceptionRecord->ExceptionInformation[1];
        /* Corrupted wake_addr: upper bits in Wine DLL range, lower 32 bits = 0 */
        if ((bad & 0xFFFFFFFFULL) == 0 && bad >= 0x6f0000000000ULL) {
            volatile unsigned char *ip =
                (volatile unsigned char *)ep->ContextRecord->Rip;
            int skip_len = 0;

            /* Wine JIT thunk (most common):
             *   MOV BYTE PTR [rax],0  = c6 00 00  (3 bytes)
             *   MOV rcx,[rcx+disp32] = 48 8b 89 ?? ?? ?? ??  (7 bytes)
             * Skip both so rcx retains the valid kernel-object pointer. */
            if (ip[0] == 0xc6 && ip[1] == 0x00 && ip[2] == 0x00 &&
                ip[3] == 0x48 && ip[4] == 0x8b && ip[5] == 0x89) {
                ep->ContextRecord->Rax = 0; /* clear garbage wake_addr from RAX */
                skip_len = 10;

            /* Generic c6 /0 imm8 — MOV r/m8, imm8 (other game-code sites).
             * Covers both "c6 00 00" and "c6 00 01" variants that don't have the
             * following MOV-rcx sequence of the Wine JIT thunk. */
            } else if (ip[0] == 0xc6) {
                unsigned char modrm = ip[1];
                int mod = (modrm >> 6) & 3, rm = modrm & 7;
                if (mod == 0 && rm != 4 && rm != 5) skip_len = 3; /* [reg], imm8 */
                else if (mod == 1 && rm != 4)        skip_len = 4; /* [reg+disp8], imm8 */
                else if (mod == 2 && rm != 4)        skip_len = 7; /* [reg+disp32], imm8 */

            /* REX + MOV r/m8, r8  (opcode 88, e.g. "44 88 38" = MOV [RAX], R14B). */
            } else if ((ip[0] & 0xf0) == 0x40 && ip[1] == 0x88) {
                unsigned char modrm = ip[2];
                int mod = (modrm >> 6) & 3, rm = modrm & 7;
                if (mod == 0 && rm != 4 && rm != 5) skip_len = 3;
                else if (mod == 1 && rm != 4)        skip_len = 4;
                else if (mod == 2 && rm != 4)        skip_len = 7;

            /* Plain MOV r/m8, r8  (opcode 88, no REX) */
            } else if (ip[0] == 0x88) {
                unsigned char modrm = ip[1];
                int mod = (modrm >> 6) & 3, rm = modrm & 7;
                if (mod == 0 && rm != 4 && rm != 5) skip_len = 2;
                else if (mod == 1 && rm != 4)        skip_len = 3;
                else if (mod == 2 && rm != 4)        skip_len = 6;
            }

            if (skip_len > 0) {
                ep->ContextRecord->Rip += skip_len;
                LONG n = InterlockedIncrement(&g_wake_suppressed);
                char s[96];
                snprintf(s, sizeof(s),
                    "[eaw-mt] WAKE_SUPPR #%ld addr=%016llx rva=0x%llx skip=%d\n",
                    (long)n, (unsigned long long)bad,
                    (unsigned long long)(ep->ContextRecord->Rip - skip_len -
                        (ULONG_PTR)GetModuleHandleA(NULL)),
                    skip_len);
                if (g_log_fp) { fputs(s, g_log_fp); fflush(g_log_fp); }
                return EXCEPTION_CONTINUE_EXECUTION;
            }
        }
    }

    /* luaH_get (FUN_1407bfd90) stale-next recovery.
     *
     * After prewarm the major GC leaves garbage node->next pointers in live Lua
     * Tables.  The loop at 0x7bfdf0 follows those pointers and faults with a
     * non-canonical address.  Rather than crash, redirect RIP to the "not found"
     * return path (0x7bfe04) so the caller inserts a fresh node on the next write. */
    if (code == EXCEPTION_ACCESS_VIOLATION) {
        uint64_t img_base_veh = (uint64_t)GetModuleHandleA(NULL);
        uint64_t rip_veh      = (uint64_t)ep->ContextRecord->Rip;
        if (img_base_veh && rip_veh == img_base_veh + LUAH_GET_LOOP_RVA) {
            ep->ContextRecord->Rip = img_base_veh + LUAH_GET_NOTFND_RVA;
            static volatile LONG g_luah_suppressed = 0;
            LONG n = InterlockedIncrement(&g_luah_suppressed);
            char s[96];
            snprintf(s, sizeof(s),
                "[eaw-mt] LUAH_NOKEY #%ld rdx=%016llx -> not_found\n",
                (long)n, (unsigned long long)ep->ContextRecord->Rdx);
            if (g_log_fp) { fputs(s, g_log_fp); fflush(g_log_fp); }
            return EXCEPTION_CONTINUE_EXECUTION;
        }
    }

    /* FUN_14020a8c0 null-param_1 guard (galactic teardown).
     * param_1==0 → MOV RAX,[RCX+8] faults as read-AV @0x8.
     * Set RAX=0 and skip the 4-byte instruction; the next test+je takes the
     * empty-list return path cleanly. */
    if (code == EXCEPTION_ACCESS_VIOLATION &&
        ep->ExceptionRecord->NumberParameters >= 2 &&
        ep->ExceptionRecord->ExceptionInformation[0] == 0 /* read */ ) {
        ULONG_PTR fault_addr = ep->ExceptionRecord->ExceptionInformation[1];
        uint64_t img_base_gt = (uint64_t)GetModuleHandleA(NULL);
        uint64_t rip_gt      = (uint64_t)ep->ContextRecord->Rip;
        if (img_base_gt && fault_addr <= 0x10 &&
            rip_gt == img_base_gt + GALTEAR_NULL_RVA) {
            ep->ContextRecord->Rax  = 0;
            ep->ContextRecord->Rip += 4;
            static volatile LONG g_galtear_suppressed = 0;
            LONG n = InterlockedIncrement(&g_galtear_suppressed);
            char s[96];
            snprintf(s, sizeof(s),
                "[eaw-mt] GALTEAR_NULL #%ld rcx=%016llx -> empty-list return\n",
                (long)n, (unsigned long long)ep->ContextRecord->Rcx);
            if (g_log_fp) { fputs(s, g_log_fp); fflush(g_log_fp); }
            return EXCEPTION_CONTINUE_EXECUTION;
        }
    }

    /* Prewarm pumpe faults are recovered via setjmp/longjmp in prewarm_from_ai_manager.
     * If the jmp_buf is valid, jump back to skip this entity; otherwise fall through
     * to the normal crash logger so the fault doesn't silently disappear. */
    if (g_in_prewarm_pumpe &&
        InterlockedCompareExchange(&g_prewarm_jmp_valid, 0, 1) == 1) {
        longjmp(g_prewarm_jmp, 1);
        /* NOT REACHED */
    }

    if (code == EXCEPTION_ACCESS_VIOLATION ||
        code == EXCEPTION_ILLEGAL_INSTRUCTION ||
        code == EXCEPTION_STACK_OVERFLOW ||
        code == EXCEPTION_PRIV_INSTRUCTION) {
        /* Re-entrancy guard: only log the first fault */
        if (InterlockedCompareExchange(&g_veh_entered, 1, 0) != 0)
            return EXCEPTION_CONTINUE_SEARCH;
        PVOID fault_insn = ep->ExceptionRecord->ExceptionAddress;
        DWORD tid = GetCurrentThreadId();
        const char *who = (tid == g_main_thread_id)   ? "MAIN"   :
                          (tid == g_render_thread_id)  ? "RENDER" : "OTHER";
        ULONG_PTR av_type = 0, av_addr = 0;
        if (code == EXCEPTION_ACCESS_VIOLATION &&
            ep->ExceptionRecord->NumberParameters >= 2) {
            av_type = ep->ExceptionRecord->ExceptionInformation[0];
            av_addr = ep->ExceptionRecord->ExceptionInformation[1];
        }
        uint64_t img_base = (uint64_t)GetModuleHandleA(NULL);
        uint64_t rip_abs  = (uint64_t)ep->ContextRecord->Rip;

        /* Find which loaded module contains the crash address. */
        char rip_mod_name[MAX_PATH] = "<unknown>";
        uint64_t rip_mod_base = 0;
        {
            HMODULE mods[512]; DWORD needed;
            if (EnumProcessModules(GetCurrentProcess(), mods, sizeof(mods), &needed)) {
                DWORD nmod = needed / sizeof(HMODULE);
                for (DWORD mi = 0; mi < nmod; mi++) {
                    MODULEINFO mi2;
                    if (GetModuleInformation(GetCurrentProcess(), mods[mi], &mi2, sizeof(mi2))) {
                        uint64_t base = (uint64_t)mi2.lpBaseOfDll;
                        uint64_t sz   = (uint64_t)mi2.SizeOfImage;
                        if (rip_abs >= base && rip_abs < base + sz) {
                            rip_mod_base = base;
                            GetModuleFileNameExA(GetCurrentProcess(), mods[mi],
                                                 rip_mod_name, MAX_PATH);
                            break;
                        }
                    }
                }
            }
        }
        /* Strip path — keep only filename. */
        char *rip_fn = rip_mod_name;
        for (char *p = rip_mod_name; *p; p++)
            if (*p == '\\' || *p == '/') rip_fn = p + 1;

        char s[512];
        snprintf(s, sizeof(s),
            "[eaw-mt] CRASH code=%08lx insn=%p tid=%lu(%s)\n"
            "[eaw-mt] CRASH av_%s @%p rip=%p rsp=%p\n"
            "[eaw-mt] CRASH img_base=%016llx rip_rva=0x%llx\n"
            "[eaw-mt] CRASH rip_mod=%s base=%016llx mod_rva=0x%llx\n",
            (unsigned long)code, fault_insn, (unsigned long)tid, who,
            av_type == 1 ? "write" : "read", (void *)av_addr,
            (void *)ep->ContextRecord->Rip,
            (void *)ep->ContextRecord->Rsp,
            (unsigned long long)img_base,
            (unsigned long long)(rip_abs - img_base),
            rip_fn, (unsigned long long)rip_mod_base,
            rip_mod_base ? (unsigned long long)(rip_abs - rip_mod_base) : 0ULL);
        if (g_log_fp) fputs(s, g_log_fp);
        /* Full register dump — identifies which register holds the bad address */
        char r[512];
        snprintf(r, sizeof(r),
            "[eaw-mt] REGS rax=%016llx rbx=%016llx rcx=%016llx rdx=%016llx\n"
            "[eaw-mt] REGS rsi=%016llx rdi=%016llx r8 =%016llx r9 =%016llx\n"
            "[eaw-mt] REGS r10=%016llx r11=%016llx r12=%016llx r13=%016llx\n"
            "[eaw-mt] REGS r14=%016llx r15=%016llx rbp=%016llx\n",
            (unsigned long long)ep->ContextRecord->Rax,
            (unsigned long long)ep->ContextRecord->Rbx,
            (unsigned long long)ep->ContextRecord->Rcx,
            (unsigned long long)ep->ContextRecord->Rdx,
            (unsigned long long)ep->ContextRecord->Rsi,
            (unsigned long long)ep->ContextRecord->Rdi,
            (unsigned long long)ep->ContextRecord->R8,
            (unsigned long long)ep->ContextRecord->R9,
            (unsigned long long)ep->ContextRecord->R10,
            (unsigned long long)ep->ContextRecord->R11,
            (unsigned long long)ep->ContextRecord->R12,
            (unsigned long long)ep->ContextRecord->R13,
            (unsigned long long)ep->ContextRecord->R14,
            (unsigned long long)ep->ContextRecord->R15,
            (unsigned long long)ep->ContextRecord->Rbp);
        if (g_log_fp) { fputs(r, g_log_fp); fflush(g_log_fp); }
        /* Stack dump — 64 slots; annotate slots that look like EXE return addresses */
        ULONG_PTR rsp = (ULONG_PTR)ep->ContextRecord->Rsp;
        PIMAGE_DOS_HEADER dos2 = (PIMAGE_DOS_HEADER)(uintptr_t)img_base;
        PIMAGE_NT_HEADERS nt2  = (PIMAGE_NT_HEADERS)(img_base + dos2->e_lfanew);
        uint64_t img_size = nt2->OptionalHeader.SizeOfImage;
        for (int i = 0; i < 64; i++) {
            ULONG_PTR val = ((volatile ULONG_PTR *)rsp)[i];
            uint64_t rva_v = (uint64_t)val - img_base;
            char stk[96];
            if (rva_v < img_size)
                snprintf(stk, sizeof(stk), "[eaw-mt] STK [%03x]=%016llx  rva=0x%llx\n",
                    i * 8, (unsigned long long)val, (unsigned long long)rva_v);
            else
                snprintf(stk, sizeof(stk), "[eaw-mt] STK [%03x]=%016llx\n",
                    i * 8, (unsigned long long)val);
            if (g_log_fp) fputs(stk, g_log_fp);
        }
        /* Dump 32 bytes of crash instruction bytes */
        ULONG_PTR rip = (ULONG_PTR)ep->ContextRecord->Rip;
        char ibuf[128];
        int iboff = 0;
        iboff += snprintf(ibuf, sizeof(ibuf), "[eaw-mt] INSN @%016llx:", (unsigned long long)rip);
        for (int i = -8; i < 24 && iboff < (int)sizeof(ibuf) - 4; i++)
            iboff += snprintf(ibuf + iboff, sizeof(ibuf) - iboff,
                "%s%02x", i == 0 ? " [" : (i == 1 ? "]" : " "),
                ((volatile unsigned char *)rip)[i]);
        ibuf[iboff++] = '\n'; ibuf[iboff] = 0;
        if (g_log_fp) { fputs(ibuf, g_log_fp); fflush(g_log_fp); }
    }
    return EXCEPTION_CONTINUE_SEARCH;
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
        AddVectoredExceptionHandler(1, veh_crash_handler);

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

        /* BISECT LEVEL 4: all non-sub-callee hooks restored.
         * Confirmed innocent: render hook (Level 2), WaitMessage IAT (Level 3).
         * If crash returns → culprit is in this group; split further.
         * If stable → crash lives in sub-callee hooks below. */
        install_pump_hook();
        install_wait_message_iat_hook();
        install_io_iat_hooks();
        install_inter_hooks();
        install_slot22_hooks();
        install_game_service_hook();
        install_space_slot22_hook();
        install_space_unit_svc_hook();
        install_tail22i_hook();
        install_post_loop_hooks();
        install_tail22_body_hooks();
        install_2be640_body_hooks();
        install_pumpe_hook();
        install_pumpe_resume_shim();
        install_prewarm_hook();
        install_fn56480_wrapper();
        install_a7190_guard();
        install_a6b80_stall2_hooks();
        install_b87010_hook();
        install_d12d520_hook();
        install_d12d480_hook();
        install_b87010_subcallee_hooks();
        install_fix_b();
        install_b387400_subcallee_hooks();
        install_b3825b0_subcallee_hooks();
        install_b29f810_subcallee_hooks();
        install_b388b60_subcallee_hooks();
        install_b3989a0_subcallee_hooks();
        install_b3a4820_subcallee_hooks();
        install_b375380_subcallee_hooks();
        install_b25ec10_subcallee_hooks();
        build_meg_index();
        install_b141f70_subcallee_hooks();
        install_ga4d0_hook();
        install_galactic_slot22_hook();

        /* DIAGNOSTIC: render thread disabled — g_trampoline called directly on main thread.
         * Keeping thread creation suppressed to isolate whether idle render thread
         * polling (WaitForSingleObject every 1000ms) was causing the use-after-free. */
        log_write("[eaw-mt] Model A: render on main thread (no render thread)\n");
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
