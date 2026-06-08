/*
 * winmm_proxy.c — proxy winmm.dll for EaW hook injection
 *
 * Phase 5: shipped stability/perf fixes for StarWarsG.exe (FoC / Thrawn's Revenge
 * under Wine/Proton + DXVK).  See hooks/SHIPPING.md for the full fix manifest and
 * env kill-switches.
 *
 * BUILD MODES (compile-time):
 *   just build-winmm           → RELEASE: fixes only, profiler compiled out (default).
 *   just build-winmm-profile   → adds -DEAW_PROFILE: full per-function timing profiler
 *                                + 300-frame stats dump + stall-sampling watchdog.
 *   All `#ifdef EAW_PROFILE` blocks are measurement only; the functional fixes are
 *   always compiled.  The release build is what you ship.
 *
 * Render hook: inline hook on FUN_140180d90 (render flush wrapper, RVA 0x180d90).
 * The render-thread offload (Model A) is dormant — flush runs on the main thread
 * (measured ceiling ~0: flush is 0.01ms/frame; DXVK executes draws async).  The
 * hook is retained as the frame anchor + binary-version sanity check.  First 14
 * bytes of FUN_140180d90 are position-independent (PUSH RBX / SUB RSP,0x20 /
 * MOV EDX,0xbff / MOV RBX,RCX) → replaced with FF 25 abs-JMP to render_flush_hook.
 *
 * Steam launch options:
 *   WINEDLLOVERRIDES="winmm=n,b" PROTON_USE_NTSYNC=1 %command% STEAMMOD=...
 */

#include <windows.h>
#include <psapi.h>
#include <stdint.h>
#include <stdio.h>
#include <setjmp.h>
#include <math.h>

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
/* Lua 5.1 debug API (Step 34 — per-coroutine deadline enforcement) */
#define LUA_SETHOOK_RVA        0x7bb640ULL  /* lua_sethook(L, fn, mask, count) */
#define LUA_THROW_RVA          0x7bc470ULL  /* luaD_throw(L, errcode) */
#define LUA_MASKCOUNT          0x08         /* fire hook every N instructions */
#define LUA_ERRRUN             2
#define RESUME_HOOK_COUNT      5000         /* opcode interval between deadline checks */
#define RESUME_HOOK_DEADLINE_MS 200         /* abort any single resume > 200ms */

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

/* Step 35 open-storm profiler (temporary — remove with the rest of the Step 35
 * scaffolding). The first-encounter stall is the main thread blocked in
 * NtCreateFile; this characterizes the open storm during a pump so we can pick
 * the fix: count, fail-rate, unique-vs-repeat paths, timing, sampled paths.
 * Active only while g_open_profile_active (set around g_pumpe_orig). All state is
 * touched only on the main thread (opens during the pump + pumpe_hook), so no
 * atomics needed. Unique/repeat uses a generation-stamped hash table — reset is
 * an O(1) gen bump, no per-pump memset. */
static volatile LONG g_open_profile_active = 0;
#define OPEN_SLOTS    8192
#define OPEN_SAMPLE_N 24
static uint32_t g_open_slot_hash[OPEN_SLOTS];
static uint32_t g_open_slot_gen [OPEN_SLOTS];
static uint32_t g_open_gen      = 0;
static long     g_open_n = 0, g_open_fail = 0, g_open_unique = 0, g_open_repeat = 0;
static double   g_open_sum_ms = 0.0, g_open_max_ms = 0.0;
static int      g_open_sample_n = 0;
static char     g_open_sample[OPEN_SAMPLE_N][MAX_PATH];
/* paths of REAL opens that FAILED but were NOT short-circuited (ms>0) — shows
 * what the negative cache is still missing. */
static int      g_open_fail_sample_n = 0;
static char     g_open_fail_sample[OPEN_SAMPLE_N][MAX_PATH];

static void open_profile_record(const char *path, int failed, double ms)
{
    if (!path) path = "";
    g_open_n++;
    g_open_sum_ms += ms;
    if (ms > g_open_max_ms) g_open_max_ms = ms;
    if (failed) g_open_fail++;
    /* real (non-short-circuited) failed open → sample what we're still missing */
    if (failed && ms > 0.0 && g_open_fail_sample_n < OPEN_SAMPLE_N) {
        strncpy(g_open_fail_sample[g_open_fail_sample_n], path, MAX_PATH - 1);
        g_open_fail_sample[g_open_fail_sample_n][MAX_PATH - 1] = '\0';
        g_open_fail_sample_n++;
    }
    /* djb2 over the path */
    uint32_t h = 5381;
    for (const unsigned char *p = (const unsigned char *)path; *p; p++)
        h = ((h << 5) + h) ^ *p;
    uint32_t slot = h % OPEN_SLOTS;
    if (g_open_slot_gen[slot] == g_open_gen && g_open_slot_hash[slot] == h) {
        g_open_repeat++;                 /* same path already seen this pump */
    } else {
        g_open_slot_gen[slot]  = g_open_gen;
        g_open_slot_hash[slot] = h;
        g_open_unique++;
        if (g_open_sample_n < OPEN_SAMPLE_N) {
            strncpy(g_open_sample[g_open_sample_n], path, MAX_PATH - 1);
            g_open_sample[g_open_sample_n][MAX_PATH - 1] = '\0';
            g_open_sample_n++;
        }
    }
}

typedef HANDLE (WINAPI *CreateFileA_t)(LPCSTR, DWORD, DWORD, LPSECURITY_ATTRIBUTES, DWORD, DWORD, HANDLE);
typedef HANDLE (WINAPI *CreateFileW_t)(LPCWSTR, DWORD, DWORD, LPSECURITY_ATTRIBUTES, DWORD, DWORD, HANDLE);
typedef BOOL   (WINAPI *ReadFile_t)(HANDLE, LPVOID, DWORD, LPDWORD, LPOVERLAPPED);

static CreateFileA_t real_CreateFileA_fn = NULL;
static CreateFileW_t real_CreateFileW_fn = NULL;
static ReadFile_t    real_ReadFile_fn    = NULL;

/* =========================================================================
 * Step 35 FIX — filesystem negative directory index ("fscache")
 *
 * The first-encounter stall is ~5000 failed CreateFile opens (96% fail) per
 * battle: the engine probes loose .ALO/.DDS asset files that don't exist on
 * disk (they live in MEG archives), each costing ~2.5ms via Wine NtCreateFile.
 *
 * Fix: the first time we see a read-open of an asset file in a directory, we
 * enumerate that directory ONCE (FindFirstFile sweep) and cache the set of
 * filenames present. Subsequent opens of files NOT in that set return
 * FILE_NOT_FOUND instantly — identical to what NtCreateFile would return,
 * without the syscall. One readdir replaces thousands of opens.
 *
 * Correctness:
 *  - A hash collision can only yield a false PRESENT (harmless fall-through to
 *    a real open), never a false ABSENT.
 *  - A directory that fails to enumerate, is empty, or is too large is marked
 *    "unindexable" and never short-circuited (always fall through).
 *  - Scoped to read-only OPEN_EXISTING of known asset extensions; saves/config
 *    are never touched.
 *  - Main-thread only (matches where the storm occurs) → no locks needed.
 *
 * Kill switch: EAW_NO_FSCACHE=1.
 * ========================================================================= */
#define FS_MAX_DIRS    96
#define FS_SET_SLOTS   32768          /* per-dir filename hash table (power of 2) */
#define FS_MAX_FILES   20000          /* >this in one dir → unindexable (fall through) */
typedef struct {
    char      dir[MAX_PATH];          /* normalized: lowercased, '\'-separated, no trailing sep */
    int       status;                 /* 0=unused, 1=indexed, 2=unindexable, 3=nonexistent(all-absent) */
    uint32_t *set;                    /* FS_SET_SLOTS hashes, 0=empty slot */
    int       nfiles;
} fs_dir_t;
static fs_dir_t      g_fs_dirs[FS_MAX_DIRS];
static int           g_fs_ndirs        = 0;
static int           g_fscache_enabled = -1;   /* -1=uninit, 0/1 resolved from env */
static long          g_fscache_hits    = 0;    /* opens short-circuited as absent */

/* lowercase + '/'→'\' + collapse duplicate separators, into dst (bounded).
 * Collapsing handles probe paths like "...GameData\\Data\..." (doubled backslash)
 * that would otherwise break FindFirstFile and dir-cache keying. */
static void fs_norm(char *dst, size_t cap, const char *src)
{
    size_t i = 0, j = 0;
    for (; src[i] && j + 1 < cap; i++) {
        char c = src[i];
        if (c == '/') c = '\\';
        else if (c >= 'A' && c <= 'Z') c = (char)(c - 'A' + 'a');
        if (c == '\\' && j > 0 && dst[j - 1] == '\\') continue;  /* collapse run */
        dst[j++] = c;
    }
    dst[j] = '\0';
}
static uint32_t fs_hash(const char *s)   /* djb2 over already-lowercased string */
{
    uint32_t h = 5381;
    for (const unsigned char *p = (const unsigned char *)s; *p; p++)
        h = ((h << 5) + h) ^ *p;
    return h ? h : 1;                     /* reserve 0 for empty slot */
}
static int fs_has_asset_ext(const char *path)
{
    size_t n = strlen(path);
    if (n < 4) return 0;
    const char *e = path + n - 4;
    /* case-insensitive compare of the 4-char extension */
    char x[5]; fs_norm(x, sizeof(x), e);
    return !strcmp(x, ".dds") || !strcmp(x, ".alo") || !strcmp(x, ".ala") ||
           !strcmp(x, ".tga");
}
static int fs_is_read_open(DWORD dwAccess, DWORD dwCreate)
{
    /* only short-circuit pure reads of an existing file */
    if (dwAccess & GENERIC_WRITE) return 0;
    return dwCreate == OPEN_EXISTING;
}
/* Enumerate `dir` (normalized, lowercase — Wine resolves case-insensitively)
 * into a fresh filename set. A directory that does not exist is recorded as
 * status=3 (all-absent): every file under a missing dir is provably absent. */
static void fs_index_dir(fs_dir_t *d, const char *dir)
{
    char pattern[MAX_PATH];
    snprintf(pattern, sizeof(pattern), "%s\\*", (dir[0] ? dir : "."));
    WIN32_FIND_DATAA fd;
    HANDLE h = FindFirstFileA(pattern, &fd);
    if (h == INVALID_HANDLE_VALUE) {
        DWORD e = GetLastError();
        /* dir missing or empty → nothing can be opened from it → all-absent */
        d->status = (e == ERROR_PATH_NOT_FOUND || e == ERROR_FILE_NOT_FOUND ||
                     e == ERROR_NO_MORE_FILES) ? 3 : 2;
        return;
    }
    uint32_t *set = (uint32_t *)calloc(FS_SET_SLOTS, sizeof(uint32_t));
    if (!set) { FindClose(h); d->status = 2; return; }
    int nfiles = 0, overflow = 0;
    do {
        if (fd.cFileName[0] == '.' &&
            (fd.cFileName[1] == '\0' ||
             (fd.cFileName[1] == '.' && fd.cFileName[2] == '\0')))
            continue;                                 /* skip . and .. */
        if (nfiles >= FS_MAX_FILES) { overflow = 1; break; }
        char low[MAX_PATH];
        fs_norm(low, sizeof(low), fd.cFileName);
        uint32_t hsh = fs_hash(low);
        uint32_t slot = hsh & (FS_SET_SLOTS - 1);
        while (set[slot] && set[slot] != hsh) slot = (slot + 1) & (FS_SET_SLOTS - 1);
        if (!set[slot]) { set[slot] = hsh; }
        nfiles++;
    } while (FindNextFileA(h, &fd));
    FindClose(h);
    if (overflow) { free(set); d->status = 2; return; }  /* too big → don't trust */
    d->set    = set;
    d->nfiles = nfiles;
    d->status = 1;
}
/* Decide whether a path is in scope for the negative cache. Scope = anything
 * under an EaW "Data\" asset root (covers Models/Textures/Sounds/Scripts/...),
 * plus bare filenames (CWD-relative) with a known asset extension. EaW never
 * writes into Data\ during play and saves/config live elsewhere, so short-
 * circuiting absent Data\ reads is safe. */
static int fs_should_cache(const char *path)
{
    char low[MAX_PATH];
    fs_norm(low, sizeof(low), path);
    if (strstr(low, "\\data\\") || strncmp(low, "data\\", 5) == 0) return 1;
    if (!strchr(low, '\\')) return fs_has_asset_ext(path);  /* bare → CWD */
    return 0;
}
/* Returns 1 if `path` is a known-absent asset file (safe to short-circuit). */
static int fs_absent(const char *path)
{
    char norm[MAX_PATH];
    fs_norm(norm, sizeof(norm), path);
    /* split at last separator */
    char *sep = strrchr(norm, '\\');
    char dirbuf[MAX_PATH], filebuf[MAX_PATH];
    if (sep) {
        size_t dl = (size_t)(sep - norm);
        memcpy(dirbuf, norm, dl); dirbuf[dl] = '\0';
        snprintf(filebuf, sizeof(filebuf), "%s", sep + 1);
    } else {
        dirbuf[0] = '\0';                  /* CWD */
        snprintf(filebuf, sizeof(filebuf), "%s", norm);
    }
    /* find or create dir entry (dirbuf is normalized lowercase) */
    fs_dir_t *d = NULL;
    for (int i = 0; i < g_fs_ndirs; i++)
        if (!strcmp(g_fs_dirs[i].dir, dirbuf)) { d = &g_fs_dirs[i]; break; }
    if (!d) {
        if (g_fs_ndirs >= FS_MAX_DIRS) return 0;   /* table full → fall through */
        d = &g_fs_dirs[g_fs_ndirs++];
        snprintf(d->dir, sizeof(d->dir), "%s", dirbuf);
        d->status = 0; d->set = NULL; d->nfiles = 0;
        fs_index_dir(d, dirbuf);           /* dirbuf is normalized; Wine is case-insensitive */
    }
    if (d->status == 3) return 1;          /* directory doesn't exist → file is absent */
    if (d->status != 1) return 0;          /* unindexable → fall through */
    uint32_t hsh = fs_hash(filebuf);
    uint32_t slot = hsh & (FS_SET_SLOTS - 1);
    while (d->set[slot]) {
        if (d->set[slot] == hsh) return 0; /* present → fall through to real open */
        slot = (slot + 1) & (FS_SET_SLOTS - 1);
    }
    return 1;                               /* hit empty slot → genuinely absent */
}
/* Resolve the kill switch once; returns 1 if fscache should run. */
static int fscache_on(void)
{
    if (g_fscache_enabled < 0) {
        const char *e = getenv("EAW_NO_FSCACHE");
        g_fscache_enabled = (e && e[0] && e[0] != '0') ? 0 : 1;
        log_write(g_fscache_enabled
            ? "[eaw-mt] FSCACHE: enabled (negative dir index; set EAW_NO_FSCACHE=1 to disable)\n"
            : "[eaw-mt] FSCACHE: DISABLED via EAW_NO_FSCACHE\n");
    }
    return g_fscache_enabled;
}

static HANDLE WINAPI createfileA_hook(LPCSTR lpFileName, DWORD dwAccess,
    DWORD dwShare, LPSECURITY_ATTRIBUTES lpSec, DWORD dwCreate,
    DWORD dwFlags, HANDLE hTemplate)
{
    int is_main = (GetCurrentThreadId() == g_main_thread_id);
    if (is_main && lpFileName)
        strncpy(g_last_opened_file, lpFileName, MAX_PATH - 1);
    if (is_main && lpFileName && fscache_on() &&
        fs_is_read_open(dwAccess, dwCreate) && fs_should_cache(lpFileName) &&
        fs_absent(lpFileName)) {
        g_fscache_hits++;
        if (g_open_profile_active) open_profile_record(lpFileName, 1, 0.0);
        SetLastError(ERROR_FILE_NOT_FOUND);
        return INVALID_HANDLE_VALUE;
    }
    if (is_main && g_open_profile_active) {
        LARGE_INTEGER a, b; tgt_fake_qpc(&a);
        HANDLE h = real_CreateFileA_fn(lpFileName, dwAccess, dwShare, lpSec,
                                       dwCreate, dwFlags, hTemplate);
        tgt_fake_qpc(&b);
        double ms = (double)(b.QuadPart - a.QuadPart) /
                    ((double)g_qpc_freq.QuadPart / 1000.0);
        open_profile_record(lpFileName, h == INVALID_HANDLE_VALUE, ms);
        return h;
    }
    return real_CreateFileA_fn(lpFileName, dwAccess, dwShare, lpSec,
                               dwCreate, dwFlags, hTemplate);
}

static HANDLE WINAPI createfileW_hook(LPCWSTR lpFileName, DWORD dwAccess,
    DWORD dwShare, LPSECURITY_ATTRIBUTES lpSec, DWORD dwCreate,
    DWORD dwFlags, HANDLE hTemplate)
{
    int is_main = (GetCurrentThreadId() == g_main_thread_id);
    if (is_main && lpFileName)
        WideCharToMultiByte(CP_UTF8, 0, lpFileName, -1,
                            g_last_opened_file, MAX_PATH, NULL, NULL);
    if (is_main && lpFileName && fscache_on() &&
        fs_is_read_open(dwAccess, dwCreate) &&
        fs_should_cache(g_last_opened_file) && fs_absent(g_last_opened_file)) {
        g_fscache_hits++;
        if (g_open_profile_active) open_profile_record(g_last_opened_file, 1, 0.0);
        SetLastError(ERROR_FILE_NOT_FOUND);
        return INVALID_HANDLE_VALUE;
    }
    if (is_main && g_open_profile_active) {
        LARGE_INTEGER a, b; tgt_fake_qpc(&a);
        HANDLE h = real_CreateFileW_fn(lpFileName, dwAccess, dwShare, lpSec,
                                       dwCreate, dwFlags, hTemplate);
        tgt_fake_qpc(&b);
        double ms = (double)(b.QuadPart - a.QuadPart) /
                    ((double)g_qpc_freq.QuadPart / 1000.0);
        open_profile_record(g_last_opened_file, h == INVALID_HANDLE_VALUE, ms);
        return h;
    }
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

/* ════════════════════════════════════════════════════════════════════════════════════════════════
 * MILESTONE pfire — GATED IN-GAME PARALLEL-FIRE TAKEOVER  (firing_body_lift_scope.md §8.10, step 4)
 *
 * The riskiest control-flow change in the project: a live-tick takeover of the per-object update walk
 * inside FUN_1402be640 (the master_update_list sweep, decomp/2be640.c:64-71), to be restructured into
 * the two-phase tick proven host-side in sim/tests/parallel_fire_test.cpp:
 *     PhaseA: settle every object (3a6b80 minus its fire half)
 *       --- barrier ---
 *     PhaseB: run the binary fire body per firer, reading the now-SETTLED world
 *       --- barrier ---
 *     PhaseC: drain buffered spawns/sfx/signals in canonical order
 *
 * Rollout (mirroring milestone a1): 1-SHARD FIRST — a control-flow takeover with NO real worker
 * threads — gated behind EAW_PFIRE (default OFF). With EAW_PFIRE unset, install_pfire_hooks() is never
 * called and the image is byte-for-byte stock.
 *
 * STAGE A (commit ea0b855) — IDENTITY SCAFFOLD (EAW_PFIRE=1). Repoint the two E8 call sites inside
 * FUN_1402be640 that own the seam — the per-object 3a6b80 call (the walk) and the final 2a62d0 call
 * (the PhaseB/PhaseC flush point) — to interceptors that PURELY PASS THROUGH to the untouched originals
 * (call sites repointed, bodies intact; replay via saved real pointers, the a1 idiom). Proved in the
 * live tick: the seam interception finds + repoints exactly one walk call + one flush site, the stubs
 * run per object per tick without faulting, and the structural oracles stay bit-exact green (passthrough
 * cannot perturb the create/order path). Bit-identity was the strongest oracle and it was valid only at
 * STAGE A, before the PhaseA/B split introduces the intended retrofit delta (§8.10 / CLAIM 2).
 *
 * STAGE B (this commit) — TWO-PHASE SPLIT, 1-SHARD (EAW_PFIRE=2). The retrofit. The fire half of the
 * per-object update is FUN_1403a76b0 (the hardpoint fire body, 3a6b80.c:372). We repoint the a76b0 call
 * site INSIDE 3a6b80's body to pfire_a76b0_intercept: at level 2 it does NOT fire — it RECORDS the
 * (firer, tick) and returns, so 3a6b80 runs settle-only (PhaseA). The deferred fires are then replayed
 * IN WALK ORDER at the 2a62d0 flush (PhaseB), after every object in this manager's walk has settled —
 * the barrier. So every fire reads the SETTLED world. PhaseB calls a76b0 at its ENTRY, so it still
 * passes through the b3a76b0 / DTWA-B3 oracle detours (rank stamped by an incrementing per-tick counter
 * at each a76b0 entry ⇒ walk-order replay keeps DTDRAIN rank_down=0). 1-SHARD: no command buffer — the
 * binary fire creates projectiles inline in canonical (walk) order; the b3 SpawnCommand buffer is a
 * later N-shard step. EXPECTED: a retrofit DELTA vs stock (fire now reads fully-settled positions);
 * validated by determinism + the structural invariants (DTWA idfail=0, DTDRAIN rank_down=0, DTWA-B3
 * §3 map all /0), NOT by stock-equivalence. justfile: just pfire=2 launch-foc-desktop.
 * ════════════════════════════════════════════════════════════════════════════════════════════════ */
typedef void (*PfireA6b80Fn)(int64_t, int32_t, int8_t);
typedef void (*PfireA62d0Fn)(int64_t);
typedef void (*PfireA76b0Fn)(int64_t, int32_t);   /* FUN_1403a76b0(ship, tick) — the fire body */
static PfireA6b80Fn g_pfire_a6b80_real = NULL;   /* untouched FUN_1403a6b80 (call-site patched) */
static PfireA62d0Fn g_pfire_a62d0_real = NULL;   /* untouched FUN_1402a62d0 (call-site patched) */
static PfireA76b0Fn g_pfire_a76b0_real = NULL;   /* FUN_1403a76b0 ENTRY (carries DT oracle detours) */
static int  g_pfire_level   = -1;                /* 0 off / 1 STAGE A identity / 2 STAGE B two-phase */
static LONG g_pfire_a6b80_calls = 0;             /* per-object walk invocations observed */
static LONG g_pfire_a62d0_calls = 0;             /* flush-point invocations observed */

/* PhaseA deferred-fire buffer: firers whose a76b0 was suppressed during the settle walk, replayed in
 * walk order at the flush. Single-threaded (1-shard): filled by the walk, drained by THIS manager's
 * 2a62d0 (the walk→a62d0 pairing is guaranteed — 2be640 has no return between the walk and 2a62d0). */
#define PFIRE_FIRE_CAP 8192
typedef struct { int64_t ship; int32_t tick; } PfireFireRec;
static PfireFireRec g_pfire_fire_buf[PFIRE_FIRE_CAP];
static int      g_pfire_fire_count = 0;
static uint32_t g_pfire_tot_deferred = 0, g_pfire_tot_fired = 0, g_pfire_overflow = 0,
                g_pfire_flushes = 0, g_pfire_maxfill = 0;
static uint32_t g_pfire_last_log = 0xFFFFFFFFu;

/* A4.1 scan-vs-fire timing (the A4.0 fork measurement, §8.17): decompose the threaded-PhaseB cost into the
 * per-fire body (3825b0/reimpl, threaded in BOTH forks) vs the rest of the fire subtree a76b0 — chiefly the
 * throttled opp-scan 387400 (LIFTED in fork A, kept serial in fork B). Uses the detour points we are already
 * inside (no new prologue): a76b0 timed in the flush-replay loop (inclusive = whole fire subtree per ship),
 * the fire body timed inside dtwa_b3_3825b0_hook (inner reimpl/trampoline call only + the whole hook, so the
 * oracle-validation overhead is reported separately and does not confound the split). scan_overhead =
 * a76b0 − fire_body_hook. Cumulative QPC, reported with the PFIRE-B balance line. */
static LARGE_INTEGER g_pf_qpf = {0};
static long long g_pf_a76b0_qpc = 0;   /* Σ a76b0 inclusive (whole fire subtree, flush replay) */
static long long g_pf_fire_qpc  = 0;   /* Σ the inner reimpl/trampoline call (the fire body that threads) */
static long long g_pf_hook_qpc  = 0;   /* Σ the whole 3825b0 hook (fire body + oracle validation) */
static long g_pf_a76b0_n = 0, g_pf_fire_n = 0;

/* A4.1 create-deferral (§8.17 structural prereq, 1-shard): at EAW_PFIRE>=4 the reimpl COMPUTES the
 * projectile payload + geometry and BUFFERS a spawn command instead of creating inline (29f810); the
 * 2a62d0 flush drains create+init+Class-2b (R2a+R2b) in canonical (rank,seq) order AFTER the a76b0 walk
 * replay. Removes the §0 inline-consume blocker on the threaded path; still 1-shard so insertion order ==
 * ascending rank == canonical (no sort yet — that's the N-shard A4.3 step). Defined below (needs R2a/R2b/
 * dtwa_b3_check); forward-declared here for the flush. The reimpl + applier are oracle-build-only (they
 * need the DTWA-B3 hook to run), so the drain is too — in the release build level>=4 falls back to STAGE B
 * (nothing buffered, no drain). */
#if defined(EAW_PROFILE) || defined(EAW_ORACLE)
static void pfire_drain_spawns(void);
#endif
static uint32_t g_pfire_spawn_drained = 0, g_pfire_spawn_overflow = 0, g_pfire_spawn_maxfill = 0;
/* A4.1 DEBUG (root-causing the drained=0 paradox): which R2 branch the reimpl takes + drain entry. */
static uint32_t g_pfdbg_buf = 0, g_pfdbg_inline = 0, g_pfdbg_drainenter = 0, g_pfdbg_drain_maxn = 0, g_pfdbg_reimpl_lvl = 0;
static uint32_t g_pfdbg_entry = 0, g_pfdbg_fb_arc = 0, g_pfdbg_fb_p3 = 0, g_pfdbg_r1cfail = 0;
/* stage-pass counters (how far each call gets) + R1c internal rejects, to localize the under-fire bug. */
static uint32_t g_pfdbg_pass_g1 = 0, g_pfdbg_pass_g2a = 0, g_pfdbg_pass_g2b = 0;
static uint32_t g_pfdbg_r1c_lead0 = 0, g_pfdbg_r1c_aimgate = 0;
/* pf==2 create-POSITION observe (methodology #27): per real binary fire, recompute the reimpl create-pos on
 * the rewound-LCG pre-state and diff vs the binary's actual 29f810 pos arg. n=compared, match=within eps,
 * origin=observe spawned at ~(0,0,0) while binary did not (the trap), nofire=binary fired but observe didn't. */
static uint32_t g_pfobs_n = 0, g_pfobs_match = 0, g_pfobs_mismatch = 0, g_pfobs_origin = 0, g_pfobs_nofire = 0, g_pfobs_fallback = 0, g_pfobs_detail = 0;

static int pfire_on(void) {
    if (g_pfire_level < 0) {
        const char *e = getenv("EAW_PFIRE");
        g_pfire_level = (e && e[0] && e[0] != '0') ? atoi(e) : 0;
        if (g_pfire_level < 0) g_pfire_level = 0;
        if (g_pfire_level >= 4)
            log_write("[eaw-mt] PFIRE: in-game fire takeover ARMED (EAW_PFIRE=4) — A4.1 create-deferral (1-shard buffer+canonical drain)\n");
        else if (g_pfire_level == 3)
            log_write("[eaw-mt] PFIRE: in-game fire takeover ARMED (EAW_PFIRE=3) — A3.3 reimpl body takeover (1-shard, two-phase)\n");
        else if (g_pfire_level >= 2)
            log_write("[eaw-mt] PFIRE: in-game fire takeover ARMED (EAW_PFIRE=2) — STAGE B two-phase split (1-shard)\n");
        else if (g_pfire_level == 1)
            log_write("[eaw-mt] PFIRE: in-game fire takeover ARMED (EAW_PFIRE=1) — STAGE A identity passthrough\n");
        else
            log_write("[eaw-mt] PFIRE: off (EAW_PFIRE=1 identity / =2 two-phase / =3 reimpl takeover to arm)\n");
    }
    return g_pfire_level;
}

/* call-site replacement for the per-object FUN_1403a6b80 (2be640:70). Always runs the real settle body;
 * at level 2 the fire half is suppressed from WITHIN (via the a76b0 call-site redirect below). */
static void pfire_a6b80_intercept(int64_t obj, int32_t p2, int8_t c3) {
    InterlockedIncrement(&g_pfire_a6b80_calls);
    if (g_pfire_a6b80_real) g_pfire_a6b80_real(obj, p2, c3);
}

/* call-site replacement for FUN_1403a76b0 (the fire body) INSIDE FUN_1403a6b80 (3a6b80.c:372). Only
 * reached during the settle walk (3a6b80's sole caller is the walk). Level 1: fire immediately (identity).
 * Level 2: DEFER — record the firer and return, so 3a6b80 settles without firing; PhaseB replays it. */
static void pfire_a76b0_intercept(int64_t ship, int32_t tick) {
    if (g_pfire_level >= 2) {
        if (g_pfire_fire_count < PFIRE_FIRE_CAP) {
            g_pfire_fire_buf[g_pfire_fire_count].ship = ship;
            g_pfire_fire_buf[g_pfire_fire_count].tick = tick;
            g_pfire_fire_count++;
            g_pfire_tot_deferred++;
            return;
        }
        g_pfire_overflow++;            /* buffer full — fire inline now, never drop a shot */
    }
    if (g_pfire_a76b0_real) g_pfire_a76b0_real(ship, tick);
}

/* call-site replacement for the final FUN_1402a62d0 (2be640:249) — the PhaseB/PhaseC flush point.
 * Level 2: replay the deferred fires IN WALK ORDER (PhaseB, post-settle barrier; creates land inline in
 * canonical order = PhaseC for 1-shard), then delegate to the original. Level 1: pure passthrough. */
static void pfire_a62d0_intercept(int64_t mgr) {
    LONG c = InterlockedIncrement(&g_pfire_a62d0_calls);
    if (g_pfire_level >= 2 && g_pfire_fire_count > 0) {
        int n = g_pfire_fire_count;
        if ((uint32_t)n > g_pfire_maxfill) g_pfire_maxfill = (uint32_t)n;
        for (int i = 0; i < n; i++) {       /* PhaseB: walk-order replay of the binary fire body */
            if (g_pfire_a76b0_real) {
                LARGE_INTEGER ta, tb; QueryPerformanceCounter(&ta);
                g_pfire_a76b0_real(g_pfire_fire_buf[i].ship, g_pfire_fire_buf[i].tick);
                QueryPerformanceCounter(&tb);
                g_pf_a76b0_qpc += (tb.QuadPart - ta.QuadPart); g_pf_a76b0_n++;
            }
            g_pfire_tot_fired++;
        }
        g_pfire_fire_count = 0;
        g_pfire_flushes++;
        /* A4.1 PhaseC: drain the buffered spawn commands (create+init+Class-2b) in canonical order. At
         * level<4 nothing was buffered (the reimpl created inline during replay), so this is a no-op.
         * Oracle-build-only: the buffering reimpl needs the DTWA-B3 hook (release ⇒ level>=4 == STAGE B). */
#if defined(EAW_PROFILE) || defined(EAW_ORACLE)
        if (g_pfire_level >= 4) pfire_drain_spawns();
#endif
    }
    if ((c & 0xfff) == 1) {                  /* periodic live-evidence */
        char m[192];
        sprintf(m, "[eaw-mt] PFIRE-%s: live — walk=%ld flush=%ld deferred=%u fired=%u maxfill=%u overflow=%u\n",
                g_pfire_level >= 4 ? "C" : (g_pfire_level >= 2 ? "B" : "A"), g_pfire_a6b80_calls, g_pfire_a62d0_calls,
                g_pfire_tot_deferred, g_pfire_tot_fired, g_pfire_maxfill, g_pfire_overflow);
        log_write(m);
        if (g_pfire_level >= 2) {        /* reimpl-fire diagnostics (pf>=3 takeover; pf==2 = the observe funnel) */
            char sm[384];
            sprintf(sm, "[eaw-mt] PFIREDBG entry=%u | passG1=%u passG2a=%u passG2b=%u reachedR2=%u | rej: g1=%u g2a=%u g2b=%u fb_arc=%u p3eq0(lifted)=%u r1c(lead0=%u aim383ba0=%u) | lvl=%u\n",
                    g_pfdbg_entry,
                    g_pfdbg_pass_g1, g_pfdbg_pass_g2a, g_pfdbg_pass_g2b, g_pfdbg_inline,
                    g_pfdbg_entry - g_pfdbg_pass_g1,                          /* gate1 rejects */
                    g_pfdbg_pass_g1 - g_pfdbg_fb_arc - g_pfdbg_pass_g2a,      /* gate2a rejects (excl fb_arc) */
                    g_pfdbg_pass_g2a - g_pfdbg_pass_g2b,                      /* gate2b rejects (p3==0 is now a LIFTED sub-path, not a reject) */
                    g_pfdbg_fb_arc, g_pfdbg_fb_p3, g_pfdbg_r1c_lead0, g_pfdbg_r1c_aimgate,
                    g_pfdbg_reimpl_lvl);
            log_write(sm);
        }
        if (g_pfire_level == 2) {        /* pf==2 create-POSITION observe vs the binary's actual spawn pos */
            char om[224];
            sprintf(om, "[eaw-mt] PFOBS-SUM n=%u match=%u mismatch=%u origin=%u GENUINE_underfire=%u fallback=%u\n",
                    g_pfobs_n, g_pfobs_match, g_pfobs_mismatch, g_pfobs_origin, g_pfobs_nofire, g_pfobs_fallback);
            log_write(om);
        }
        /* A4.1 scan-vs-fire split (the A4.0 fork measurement) */
        if (g_pf_qpf.QuadPart > 0 && g_pf_a76b0_qpc > 0) {
            double per_ms = (double)g_pf_qpf.QuadPart / 1000.0;
            double fc = (double)g_pf_a76b0_qpc / per_ms;       /* fire-control subtree (a76b0) */
            double fire = (double)g_pf_fire_qpc / per_ms;      /* the fire body (reimpl) — threads in A & B */
            double hook = (double)g_pf_hook_qpc / per_ms;      /* fire body + oracle validation */
            double scan = fc - hook;                           /* opp-scan + dispatch (≈ what fork A would lift) */
            double val  = hook - fire;                         /* oracle-validation overhead (subtracted from both) */
            double fire_frac = fc > 0.0 ? fire / fc : 0.0;     /* B-relevant: fraction already threaded */
            double scan_frac = fc > 0.0 ? scan / fc : 0.0;     /* A-relevant: fraction fork B leaves serial */
            char tn[320];
            snprintf(tn, sizeof tn,
                "PFIRESPLIT\tfc_ms=%.1f\tfire_ms=%.1f\tscan_ms=%.1f\tval_ms=%.1f\tfire/fc=%.4f\tscan/fc=%.4f\ta76b0_n=%ld\tfire_n=%ld\n",
                fc, fire, scan, val, fire_frac, scan_frac, g_pf_a76b0_n, g_pf_fire_n);
            log_write(tn);
        }
    }
    if (g_pfire_a62d0_real) g_pfire_a62d0_real(mgr);
}

/* Helper: repoint every E8 call to `target` within [fn, fn+size) to `stub`. Returns the count. */
static int pfire_repoint_calls(BYTE *fn, int size, BYTE *target, BYTE *stub) {
    int n = 0;
    for (int i = 0; i <= size - 5; i++) {
        if (fn[i] != 0xE8) continue;
        int32_t rel; memcpy(&rel, fn + i + 1, 4);
        if (fn + i + 5 + rel != target) continue;
        int32_t new_rel = (int32_t)(stub - (fn + i + 5));
        DWORD old;
        VirtualProtect(fn + i + 1, 4, PAGE_EXECUTE_READWRITE, &old);
        memcpy(fn + i + 1, &new_rel, 4);
        VirtualProtect(fn + i + 1, 4, old, &old);
        FlushInstructionCache(GetCurrentProcess(), fn + i, 5);
        n++;
    }
    return n;
}

/* Repoint the seam call sites. Only called when EAW_PFIRE is armed, so the default image is untouched.
 *   - 3a6b80 (walk) + 2a62d0 (flush) call sites INSIDE FUN_1402be640 → the per-object seam + flush point.
 *   - a76b0  (fire body) call site INSIDE FUN_1403a6b80 (3a6b80.c:372) → so the settle walk can suppress
 *     the fire (STAGE B); g_pfire_a76b0_real keeps the a76b0 ENTRY so PhaseB replay still passes through
 *     the DT oracle entry-detours (b3a76b0 rank stamp / DTWA-B3). (In the EAW_PROFILE build the timing
 *     installer install_2be640_body_hooks repoints the same 2be640 sites; PFIRE targets the release /
 *     EAW_ORACLE builds, which do not run it, so they do not collide.) */
static BOOL install_pfire_hooks(void) {
    HMODULE exe = GetModuleHandleA(NULL);
    if (!exe) return FALSE;
    QueryPerformanceFrequency(&g_pf_qpf);   /* A4.1 scan-vs-fire timer */
    BYTE *fn2be640 = (BYTE *)exe + T2BE640_RVA;
    BYTE *fn3a6b80 = (BYTE *)exe + TA6B80_RVA;
    BYTE *fna6b80  = (BYTE *)exe + TA6B80_RVA;   /* 3a6b80, the per-object update */
    BYTE *fna62d0  = (BYTE *)exe + TA62D0_RVA;   /* 2a62d0, the final call */
    BYTE *fna76b0  = (BYTE *)exe + A76B0_RVA;    /* 3a76b0, the fire body */
    g_pfire_a6b80_real = (PfireA6b80Fn)fna6b80;
    g_pfire_a62d0_real = (PfireA62d0Fn)fna62d0;
    g_pfire_a76b0_real = (PfireA76b0Fn)fna76b0;

    BYTE *stub_block = alloc_near(fn2be640, 14 * 3);
    if (!stub_block) { log_write("[eaw-mt] WARN: alloc_near failed for pfire stubs\n"); return FALSE; }
    BYTE *stub_a6b80 = stub_block + 0 * 14;
    BYTE *stub_a62d0 = stub_block + 1 * 14;
    BYTE *stub_a76b0 = stub_block + 2 * 14;
    write_abs_jmp(stub_a6b80, (uint64_t)pfire_a6b80_intercept);
    write_abs_jmp(stub_a62d0, (uint64_t)pfire_a62d0_intercept);
    write_abs_jmp(stub_a76b0, (uint64_t)pfire_a76b0_intercept);

    int n_a6b80 = pfire_repoint_calls(fn2be640, B2BE640_BODY_SIZE, fna6b80, stub_a6b80);
    int n_a62d0 = pfire_repoint_calls(fn2be640, B2BE640_BODY_SIZE, fna62d0, stub_a62d0);
    int n_a76b0 = pfire_repoint_calls(fn3a6b80, A6B80_BODY_SIZE, fna76b0, stub_a76b0);

    char m[192];
    sprintf(m, "[eaw-mt] PFIRE: seam repointed — a6b80(walk)=%d a62d0(flush)=%d a76b0(fire)=%d site(s)\n",
            n_a6b80, n_a62d0, n_a76b0);
    log_write(m);
    return (n_a6b80 > 0 && n_a76b0 > 0);
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

/* Step 35 option-2 investigation: sampling profiler for the 33s first-encounter
 * stall. When a pump runs past STALL_SAMPLE_MS, the watchdog thread suspends the
 * main thread, captures RIP + stack return addresses, resumes, then logs them.
 * Over a long stall this yields a function-level breakdown of the C asset/model
 * loading we want to trigger directly. Temporary — remove with the rest of the
 * Step 35 scaffolding once the direction is locked. */
#define STALL_SAMPLE_MS        1500   /* a pump running this long = a real stall */
#define STALL_SAMPLE_PERIOD_MS 100    /* sample interval once in a stall */
#define STALL_SAMPLE_STACKN    40     /* stack qwords scanned per sample */
#define STALL_SAMPLE_MAX       256    /* cap samples per stall to bound log spam */
static volatile DWORD g_wd_pump_start = 0;   /* timeGetTime() when current pump armed */
static HANDLE         g_main_thread_h  = NULL;/* real handle to main thread (for sampling) */

/* Per-resume stats fed by pumpe_lua_resume_shim (reset each stats period) */
static volatile LONG  g_resume_n    = 0;   /* total lua_resume calls */
static volatile LONG  g_resume_slow = 0;   /* resumes exceeding PUMP_DEADLINE_MS */
static volatile LONG  g_resume_max_ms = 0; /* max single resume duration (ms) */
static volatile LONG  g_resume_abort = 0;  /* coroutines killed by deadline hook */

/* Lua 5.1 deadline enforcement (Step 34) */
typedef void (*LuaSethookFn)(void *L, void *fn, int mask, int count);
typedef void (*LuaThrowFn)(void *L, int errcode);
static LuaSethookFn   g_lua_sethook     = NULL;
static LuaThrowFn     g_lua_throw       = NULL;
static volatile DWORD g_hook_deadline_ms = 0;

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

/* NOTE: the background Lua-AI dispatch worker (Model B) was removed — it was the
 * abandoned offload experiment (Step 30), proven to corrupt main-thread vtable
 * state because EaW's Lua C closures touch shared sim state during lua_resume.
 * Pumping is synchronous on the main thread; the Fix B3 per-gsvc budget is the
 * stall guard. See openspec/engine/threading_model.md (Model B verdict). */

static volatile LONG g_post_prewarm_log = 0; /* arms after prewarm fires */

/* Watchdog thread for per-call deadline measurement (Step 2).
 * Polls every 1ms; counts calls that run past g_wd_deadline.
 * Does NOT write the abort flag — see PUMP_DEADLINE_MS comment above. */
static DWORD WINAPI pump_watchdog_thread(LPVOID unused)
{
    (void)unused;
    DWORD    last_sample = 0;
    int      samples_this_stall = 0;
    uint64_t img_base = (uint64_t)GetModuleHandleA(NULL);
    for (;;) {
        Sleep(1);
        if (!g_wd_armed) { samples_this_stall = 0; continue; }
        DWORD now = real_timeGetTime();
        /* unsigned subtraction wraps correctly across DWORD rollover */
        if ((DWORD)(now - g_wd_deadline) < 0x80000000UL)
            InterlockedIncrement(&g_wd_fired);

        /* Stall sampler: only once a pump has run absurdly long. */
        DWORD elapsed = now - g_wd_pump_start;
        if (elapsed < STALL_SAMPLE_MS || !g_main_thread_h ||
            samples_this_stall >= STALL_SAMPLE_MAX)
            continue;
        if ((DWORD)(now - last_sample) < STALL_SAMPLE_PERIOD_MS) continue;
        last_sample = now;

        /* Capture RIP + stack while the main thread is suspended; do NOT log
         * here (the suspended main thread may hold the CRT FILE lock → deadlock).
         * Copy raw values out, resume, then format and write. */
        uint64_t rip = 0, stack[STALL_SAMPLE_STACKN];
        int got = 0, nstk = 0;
        if (SuspendThread(g_main_thread_h) != (DWORD)-1) {
            CONTEXT ctx; memset(&ctx, 0, sizeof(ctx));
            ctx.ContextFlags = CONTEXT_CONTROL | CONTEXT_INTEGER;
            if (GetThreadContext(g_main_thread_h, &ctx)) {
                rip = ctx.Rip;
                uint64_t rsp = ctx.Rsp;
                for (int k = 0; k < STALL_SAMPLE_STACKN; k++) {
                    /* reading the main thread's (mapped) stack while suspended is safe */
                    stack[k] = *(volatile uint64_t *)(rsp + (uint64_t)k * 8);
                }
                nstk = STALL_SAMPLE_STACKN;
                got = 1;
            }
            ResumeThread(g_main_thread_h);
        }
        if (!got) continue;

        /* Log RIP, then the stack qwords that look like in-module return addrs. */
        char line[512];
        int off = snprintf(line, sizeof(line),
            "[eaw-mt] STALL-SAMPLE #%d elapsed=%ums rip_rva=0x%llx callers=",
            samples_this_stall, (unsigned)elapsed,
            (unsigned long long)(rip - img_base));
        int shown = 0;
        for (int k = 0; k < nstk && shown < 8 && off < (int)sizeof(line) - 16; k++) {
            uint64_t v = stack[k];
            if (v > img_base && v < img_base + 0x1000000ULL) {  /* ~16MB .text window */
                off += snprintf(line + off, sizeof(line) - off, "%s0x%llx",
                                shown ? "," : "",
                                (unsigned long long)(v - img_base));
                shown++;
            }
        }
        off += snprintf(line + off, sizeof(line) - off, "\n");
        log_write(line);
        samples_this_stall++;
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

/* Called every RESUME_HOOK_COUNT Lua opcodes from inside a resumed coroutine.
 * If the deadline has passed, kill the coroutine via luaD_throw(LUA_ERRRUN).
 * lua_resume will return LUA_ERRRUN and Pump_Threads will treat it as dead. */
static void lua_deadline_hook(void *L, void *ar)
{
    (void)ar;
    DWORD now = real_timeGetTime();
    DWORD dl  = g_hook_deadline_ms;
    if (dl && now >= dl) {
        LONG n = InterlockedIncrement(&g_resume_abort);
        char s[96];
        snprintf(s, sizeof(s),
            "[eaw-mt] RESUME_ABORT #%ld L=%p elapsed>%ums prewarm=%d\n",
            (long)n, L, RESUME_HOOK_DEADLINE_MS, (int)g_in_prewarm_pumpe);
        if (g_log_fp) { fputs(s, g_log_fp); fflush(g_log_fp); }
        g_hook_deadline_ms = 0;
        g_lua_throw(L, LUA_ERRRUN);
        /* NOT REACHED */
    }
}

static int pumpe_lua_resume_shim(void *L, void *cont_flag, int nargs)
{
    if (g_lua_sethook) {
        g_hook_deadline_ms = real_timeGetTime() + RESUME_HOOK_DEADLINE_MS;
        g_lua_sethook(L, lua_deadline_hook, LUA_MASKCOUNT, RESUME_HOOK_COUNT);
    }
    DWORD t0 = real_timeGetTime();
    int result = g_lua_resume_thunk_orig(L, cont_flag, nargs);
    DWORD elapsed = real_timeGetTime() - t0;
    if (g_lua_sethook) {
        g_lua_sethook(L, NULL, 0, 0);
        g_hook_deadline_ms = 0;
    }

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

    /* Wire deadline hook API functions (Step 34) */
    g_lua_sethook = (LuaSethookFn)((BYTE *)exe + LUA_SETHOOK_RVA);
    g_lua_throw   = (LuaThrowFn)((BYTE *)exe + LUA_THROW_RVA);

    {
        char s[96];
        snprintf(s, sizeof(s),
            "[eaw-mt] pumpe lua_resume shim installed (rva=0x247b56 -> thunk@7b9310) deadline=%dms\n",
            RESUME_HOOK_DEADLINE_MS);
        log_write(s);
    }
    return TRUE;
}

static void pumpe_hook(int64_t a)
{
#ifdef EAW_PROFILE
    /* Stall-sampler watchdog (measurement only): a 1ms-polling thread that can
     * suspend+sample the main thread during a stall. Not in the release build. */
    static LONG g_pumpe_once = 0;
    if (InterlockedCompareExchange(&g_pumpe_once, 1, 0) == 0) {
        log_write("[eaw-mt] DBG: pumpe_hook first call\n");
        g_main_thread_h = OpenThread(THREAD_GET_CONTEXT | THREAD_SUSPEND_RESUME,
                                     FALSE, GetCurrentThreadId());
        if (!g_main_thread_h)
            log_write("[eaw-mt] WARN: OpenThread(main) failed — stall sampler disabled\n");
        /* Create watchdog thread here, not in DllMain: CreateThread inside
         * DllMain holds the loader lock and can deadlock Wine's DLL loader. */
        HANDLE wdh = CreateThread(NULL, 0, pump_watchdog_thread, NULL, 0, NULL);
        if (wdh) { CloseHandle(wdh); log_write("[eaw-mt] pumpe watchdog thread started (deadline=4ms)\n"); }
        else      { log_write("[eaw-mt] WARN: failed to start pumpe watchdog thread\n"); }
    }
#endif

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

    /* t0/t1 timing is FUNCTIONAL: it feeds g_pumpe_frame_used_ms, the Fix B3
     * per-gsvc budget accumulator. Required in every build. */
    LARGE_INTEGER t0, t1;
    tgt_fake_qpc(&t0);

#ifdef EAW_PROFILE
    /* Watchdog arming + open-storm profiler (measurement only). */
    g_wd_pump_start = real_timeGetTime();
    g_wd_deadline   = g_wd_pump_start + PUMP_DEADLINE_MS;
    InterlockedExchange(&g_wd_armed, 1);

    g_open_gen++;
    g_open_n = g_open_fail = g_open_unique = g_open_repeat = 0;
    g_open_sum_ms = g_open_max_ms = 0.0;
    g_open_sample_n = 0;
    g_open_fail_sample_n = 0;
    g_fscache_hits = 0;
    g_open_profile_active = 1;
#endif

    g_pumpe_orig(a);

#ifdef EAW_PROFILE
    g_open_profile_active = 0;
    InterlockedExchange(&g_wd_armed, 0);

    /* If this pump was a stall, dump the open-storm characterization. */
    if ((real_timeGetTime() - g_wd_pump_start) >= 300 && g_open_n > 0) {
        char hdr[256];
        snprintf(hdr, sizeof(hdr),
            "[eaw-mt] OPEN-STORM opens=%ld fail=%ld(%.0f%%) unique=%ld repeat=%ld"
            " fscache_hits=%ld open_sum_ms=%.0f open_max_ms=%.1f\n",
            g_open_n, g_open_fail,
            g_open_n ? (100.0 * (double)g_open_fail / (double)g_open_n) : 0.0,
            g_open_unique, g_open_repeat, g_fscache_hits, g_open_sum_ms, g_open_max_ms);
        log_write(hdr);
        for (int k = 0; k < g_open_fail_sample_n; k++) {
            char ln[MAX_PATH + 48];
            snprintf(ln, sizeof(ln), "[eaw-mt] OPEN-STORM miss[%d]=%s\n",
                     k, g_open_fail_sample[k]);
            log_write(ln);
        }
    }
#endif

    tgt_fake_qpc(&t1);
    double ms = (t1.QuadPart - t0.QuadPart) / ((double)g_qpc_freq.QuadPart / 1000.0);
    g_pumpe_frame_used_ms += ms;   /* Fix B3 budget accumulator (functional) */
#ifdef EAW_PROFILE
    g_pumpe_sum_ms += ms;
    if (ms > g_pumpe_max_ms) g_pumpe_max_ms = ms;
    g_pumpe_count++;
#endif
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
/* alHModel ctor (FUN_14012b330) null render-pass element guard.
 * Source render-pass array (param_2+0xf0..+0xf8) may have elements whose +8
 * field is null (not yet populated by renderer).  At rva=0x12b6bf the ctor
 * does MOV RAX,[RCX] where RCX=null → AV.  The next instruction CALL [RAX+0x28]
 * would also crash.  Recovery: set RAX=0, skip 6 bytes (both instructions).
 * Post-skip: RDI=0 (plVar5=null), so the render-pass slot stores NULL — safe
 * because an uninitialized dest slot (also 0) passes the CMP/JZ immediately. */
#define ALMODEL_NULL_RP_RVA  0x12b6bfULL  /* MOV RAX,[RCX] — render-pass +8 null guard */
#define ALMODEL_NULL_RP_SKIP 6            /* skip MOV(3) + CALL(3) */
/* Step 35: second null render-pass site in the SAME ctor, different loop.
 * At rva=0x12b6fb: MOV [RCX+0xb0], R14 — writes the model back-ref (param_1) into
 * each render-pass element from the [R15] array.  When an element slot is null
 * (RCX=0) the store faults av_write @0xb0.  Recovery: skip the 7-byte store; the
 * (nonexistent) null element simply doesn't get the back-ref and the loop's INC
 * ESI / next-iteration logic at 0x12b702 continues normally.  Confirmed crash on
 * 6th battle entry 2026-05-29 (RCX=0, R14=0x4b730c20 back-ref). */
#define ALMODEL_NULL_RP2_RVA  0x12b6fbULL /* MOV [RCX+0xb0],R14 — render-pass back-ref null guard */
#define ALMODEL_NULL_RP2_SKIP 7           /* skip the 7-byte REX.WR MOV [RCX+disp32],R14 */

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

    /* Step 35: prewarm is DISABLED BY DEFAULT. A/B testing (2026-05-29) confirmed
     * prewarm is the cause of the progressive battle-re-entry crashes: it pumps Lua
     * coroutines through the SHARED global_State and hack-restores the string table
     * / GC counters, leaving cumulative corruption that surfaces as null-deref
     * crashes (0x7b958d, 0x12b6fb, 0x20e6da) after ~5-6 battle entries. With prewarm
     * off the game ran 10+ entries with zero crashes. Opt back in with EAW_PREWARM=1
     * (experimental — only for investigating a corruption-free re-implementation). */
    static int s_prewarm_on = -1;
    if (s_prewarm_on < 0) {
        const char *e = getenv("EAW_PREWARM");
        s_prewarm_on = (e && e[0] && e[0] != '0') ? 1 : 0;
        log_write(s_prewarm_on
            ? "[eaw-mt] PREWARM: ENABLED via EAW_PREWARM (experimental — corrupts global_State over battle re-entries)\n"
            : "[eaw-mt] PREWARM: disabled by default (set EAW_PREWARM=1 to re-enable)\n");
    }

    if (s_prewarm_on && g_pumpe_count > 0) {
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

#if defined(EAW_PROFILE) || defined(EAW_ORACLE)
/* =========================================================================
 * Differential-test harness (decomp Phase 1) — per-tick world-state fingerprint.
 *
 * Emits, once per sim tick, an FNV-1a hash over the deterministic VALUE fields of
 * every active locomotor component the movement coordinator services this tick.
 * This is the correctness oracle for the sim-parallel rewrite (Phases 3-4): a
 * re-implemented movement path must reproduce this hash stream tick-for-tick on a
 * fixed save+replay (lockstep determinism makes the trace reproducible across runs).
 *
 * Enumeration mirrors FUN_1403a76b0 (decomp/3a76b0.c): coordinator+0x2d0 -> list L
 * (skip if coordinator+0x3a0 & 0x40); L+0x10 = count, L+0x8 = ptr array (stride 8);
 * element e = arr[i]; entity = *(e+0x20); active iff *(entity+0x4d)==1. Per active
 * component we fold {iteration index, entity motion-state @+0x48, component speed
 * float @+0x28, component countdown @+0x58} — all value-deterministic. (Transform
 * offsets get added once Phase 2 recovers the entity layout; the framework is the
 * deliverable, the hashed field set is a knob.) Profile-build only; runtime-gated
 * by EAW_DIFFTRACE=1. Grep "DIFFTRACE" out of the log to extract the golden trace.
 * ========================================================================= */
#define DT_FNV_BASIS 0xcbf29ce484222325ULL
#define DT_FNV_PRIME 0x00000100000001b3ULL
static int                g_dt_enabled  = -1;
static uint32_t           g_dt_tick     = 0xFFFFFFFFu;
static uint64_t           g_dt_hash     = DT_FNV_BASIS;
static uint32_t           g_dt_count    = 0;
static volatile uint32_t *g_dt_frame_ctr = NULL;   /* DAT_140b0a320, RVA 0xb0a320 */
static uint32_t           g_dt_pos_n    = 0;       /* GameObjects whose position was folded this tick */
static int                g_dt_pos_have = 0;       /* a sample position captured this tick */
static float              g_dt_pos_xyz[3];         /* first folded GameObject's x/y/z (eyeball check) */
/* Locomotor velocity capture for the same first-folded GameObject — the oracle for the lifted
 * position integration (sim/locomotor.cpp): verify offline that pos[t]-pos[t-1] == velocity[t].
 * locomotor_state = *(coord+0xa8); velocity = state+0x14/18/1c; state enum = state+0x48. */
static int                g_dt_loco_have = 0;
static uint64_t           g_dt_ent       = 0;      /* coord ptr — only compare consecutive same-ent ticks */
static float              g_dt_vel_xyz[3];
static int32_t            g_dt_lstate    = -1;
static float              g_dt_heading   = 0;      /* coord+0x8c heading angle (deg) — SimpleSpace straight move */
static float              g_dt_speed     = 0;      /* state+0xec speed scalar */
static int32_t            g_dt_timer     = -1;     /* state+0x5c drift timer (0x2c mode magnitude index) */
static int                g_dt_tab_done  = 0;      /* runtime speed table DAT_140b31440 dumped once */
static int                g_dt_mat_done  = 0;      /* runtime Hermite basis DAT_140b2f1f0 dumped once */
/* vfunc_59 (0x625990) cubic-Hermite mover bit-exact oracle counters (sim/spline_mover). */
static uint32_t           g_dt_spl2_pass = 0, g_dt_spl2_fail = 0, g_dt_spl2_total = 0;
static int                g_dt_spl2_shown = 0;     /* first-mismatch detail emitted */
static uint32_t           g_dt_spl2_last = 0xFFFFFFFFu;  /* last tick a DTSPL2 summary was emitted */
/* Spline-segment capture (0x13 Moving state): the two active control points node[idx], node[idx+1]
 * from the state+0x18 array (stride 0x34, idx=state+0x60) for the FUN_140625990 spline oracle.
 * Each node: pos[0..2], tangent[3..5], weight[6], arc[0xc]. */
static int                g_dt_spl_have  = 0;
static int32_t            g_dt_spl_idx   = -1;
static float              g_dt_spl_n0[8];          /* px,py,pz,tx,ty,tz,w,arc */
static float              g_dt_spl_n1[8];
static float              g_dt_spl_z     = 0;      /* owner current z (held by the mover) */
static uintptr_t          g_dt_imgbase   = 0;      /* StarWarsG.exe base, for vtable->RVA */
static uint32_t           g_dt_loco_rva  = 0;      /* locomotor vtable RVA of the first-folded ship */
/* Dedicated Starship latch: the first ship each tick whose locomotor is a
 * StarshipLocomotorBehaviorClass (vtable RVA 0x8ae250) — auto-targets the unit family the lifted
 * sim/locomotor.cpp integrator models (raw per-tick velocity at state+0x14/18/1c, states 0..8). */
static int                g_dt_ss_have   = 0;
static uint64_t           g_dt_ss_ent    = 0;
static float              g_dt_ss_vel[3];
static int32_t            g_dt_ss_state  = -1;

static int dt_on(void) {
    if (g_dt_enabled < 0) {
        const char *e = getenv("EAW_DIFFTRACE");
        g_dt_enabled = (e && e[0] && e[0] != '0') ? 1 : 0;
        log_write(g_dt_enabled
            ? "[eaw-mt] DIFFTRACE: enabled (per-tick movement-state + sim-position fingerprint)\n"
            : "[eaw-mt] DIFFTRACE: off (set EAW_DIFFTRACE=1 to capture golden trace)\n");
    }
    return g_dt_enabled;
}
static inline void dt_fold(const void *p, size_t n) {
    const uint8_t *b = (const uint8_t *)p;
    uint64_t x = g_dt_hash;
    for (size_t i = 0; i < n; i++) { x ^= b[i]; x *= DT_FNV_PRIME; }
    g_dt_hash = x;
}
static void dt_emit(void) {
    char buf[160];
    snprintf(buf, sizeof buf, "DIFFTRACE\ttick=%u\tn=%u\tpos=%u\th=%016llx\n",
             g_dt_tick, g_dt_count, g_dt_pos_n, (unsigned long long)g_dt_hash);
    log_write(buf);
    /* Human-readable determinism check: one GameObject's sim position per tick.
     * Same save+orders replayed twice must reproduce it; a moving unit's must change smoothly. */
    if (g_dt_pos_have) {
        snprintf(buf, sizeof buf, "DTPOS\ttick=%u\tx=%.6f\ty=%.6f\tz=%.6f\n",
                 g_dt_tick, g_dt_pos_xyz[0], g_dt_pos_xyz[1], g_dt_pos_xyz[2]);
        log_write(buf);
    }
    /* Locomotor velocity + heading/speed of the same sampled ship (loco = vtable RVA = family).
     * hd/sp feed the SimpleSpace straight-move oracle: pos += (cos hd, sin hd, 0) * sp. */
    if (g_dt_loco_have) {
        snprintf(buf, sizeof buf,
                 "DTVEL\ttick=%u\tent=%llx\tloco=%x\tlstate=%d\tvx=%.6f\tvy=%.6f\tvz=%.6f\thd=%.6f\tsp=%.6f\ttm=%d\n",
                 g_dt_tick, (unsigned long long)g_dt_ent, g_dt_loco_rva, g_dt_lstate,
                 g_dt_vel_xyz[0], g_dt_vel_xyz[1], g_dt_vel_xyz[2], g_dt_heading, g_dt_speed, g_dt_timer);
        log_write(buf);
    }
    /* Spline-segment control points (0x13 Moving) — oracle for the lifted hermite_spline_eval. */
    if (g_dt_spl_have) {
        char sb[224];
        snprintf(sb, sizeof sb,
                 "DTSPL\ttick=%u\tn=0\tidx=%d\tz=%.6f\tpx=%.6f\tpy=%.6f\ttx=%.6f\tty=%.6f\ttz=%.6f\tw=%.6f\ta=%.6f\n",
                 g_dt_tick, g_dt_spl_idx, g_dt_spl_z, g_dt_spl_n0[0], g_dt_spl_n0[1],
                 g_dt_spl_n0[3], g_dt_spl_n0[4], g_dt_spl_n0[5], g_dt_spl_n0[6], g_dt_spl_n0[7]);
        log_write(sb);
        snprintf(sb, sizeof sb,
                 "DTSPL\ttick=%u\tn=1\tidx=%d\tz=%.6f\tpx=%.6f\tpy=%.6f\ttx=%.6f\tty=%.6f\ttz=%.6f\tw=%.6f\ta=%.6f\n",
                 g_dt_tick, g_dt_spl_idx, g_dt_spl_z, g_dt_spl_n1[0], g_dt_spl_n1[1],
                 g_dt_spl_n1[3], g_dt_spl_n1[4], g_dt_spl_n1[5], g_dt_spl_n1[6], g_dt_spl_n1[7]);
        log_write(sb);
    }
    /* Starship-confirmed velocity — the oracle target for the lifted integrator. */
    if (g_dt_ss_have) {
        snprintf(buf, sizeof buf,
                 "DTVELS\ttick=%u\tent=%llx\tlstate=%d\tvx=%.4f\tvy=%.4f\tvz=%.4f\n",
                 g_dt_tick, (unsigned long long)g_dt_ss_ent, g_dt_ss_state,
                 g_dt_ss_vel[0], g_dt_ss_vel[1], g_dt_ss_vel[2]);
        log_write(buf);
    }
}
/* Scan a ship's behavior array (coord+0x278 ptr / +0x290 count) for a locomotor behavior; return
 * its primary vtable RVA, or 0 if none. Identifies the locomotor FAMILY (families differ in their
 * velocity representation — Starship stores raw per-tick velocity at state+0x14, capital/Fleet a
 * unit direction). Profile-only; bounds-checked. */
static uint32_t dt_loco_vtbl_rva(int64_t coord, uintptr_t base) {
    int64_t barr = *(int64_t *)(coord + 0x278);
    int32_t bcount = *(int32_t *)(coord + 0x290);
    if (!barr || bcount <= 0 || bcount > 256) return 0;
    for (int i = 0; i < bcount; i++) {
        int64_t b = *(int64_t *)(barr + (int64_t)i * 8);
        if (!b) continue;
        int64_t vt = *(int64_t *)b;
        if (vt <= (int64_t)base) continue;
        uint32_t rva = (uint32_t)((uintptr_t)vt - base);
        switch (rva) {
            case 0x8ae250: /* Starship */   case 0x899c58: /* Fleet */
            case 0x8a6198: /* Fighter */    case 0x8ad798: /* Walk */
            case 0x8aeaf8: /* SimpleSpace */ case 0x8adda0: /* base */
            case 0x8a3630: /* LandTeamContainer */ case 0x8af8d0: /* LandTeamInfantry */
            case 0x8aff88: /* Team */
                return rva;
        }
    }
    return 0;
}

/* ── In-process bit-exact oracle for the lifted SimpleSpaceLocomotor vfunc_59 (0x625990) cubic-Hermite
 * mover core (sim/spline_mover.cpp). Re-transcribed here in C (the hook is a single C TU; sim/ is C++)
 * and compared raw-bit against the engine's just-written position. Inputs come from the passive
 * DTSPL node read; time = the frame counter = vfunc_59's param_3; out pos = coord+0x78/0x7c (the
 * caller writes vfunc_59's param_7 there via FUN_1403a8f90). Position-core only (X/Y); Z is passthrough. */
static float dt_spl_normalize3(float x, float y, float z, float *ox, float *oy) {
    float len = sqrtf(x*x + y*y + z*z);
    if (0.0f < len) { float inv = 1.0f/len; *ox = x*inv; *oy = y*inv; }
    else { *ox = x; *oy = y; }
    return len;
}
/* Hermite basis M^-1 rows {a,b,c,d} x inputs {p0,p1,m0,m1} — exact integers (DAT_140b2f1f0 runtime). */
static const float DT_HERM[4][4] = {
    { 1.f, 0.f, 0.f, 0.f}, {0.f, 0.f, 1.f, 0.f}, {-3.f, 3.f, -2.f, -1.f}, {2.f, -2.f, 1.f, 1.f}
};
static inline float dt_herm(int r, float p0, float p1, float m0, float m1) {
    return ((p0*DT_HERM[r][0] + p1*DT_HERM[r][1]) + m0*DT_HERM[r][2]) + m1*DT_HERM[r][3];
}
/* Recompute the spline X/Y position from two raw nodes (n[0..2]=pos, n[3..5]=heading, n[6]=speed,
 * n[0xc]=arc-time) at frame-time `tm`. Mirrors spline_step() grouping exactly. */
static void dt_spline_pos(const float *n0, const float *n1, float tm, float *px, float *py) {
    float dT = n1[0xc] - n0[0xc];
    float u = ((float)tm - n0[0xc]) / dT;
    float t = 0.0f; if (0.0f <= u) { t = 1.0f; if (u <= 1.0f) t = u; }
    float t2 = t*t, t3 = t*t2;
    float n0x, n0y, n1x, n1y;
    dt_spl_normalize3(n0[3], n0[4], n0[5], &n0x, &n0y);
    dt_spl_normalize3(n1[3], n1[4], n1[5], &n1x, &n1y);
    float m0x = n0[6]*n0x*dT, m0y = n0[6]*n0y*dT;
    float m1x = n1[6]*n1x*dT, m1y = n1[6]*n1y*dT;
    float ax = dt_herm(0, n0[0], n1[0], m0x, m1x), bx = dt_herm(1, n0[0], n1[0], m0x, m1x);
    float cx = dt_herm(2, n0[0], n1[0], m0x, m1x), dx = dt_herm(3, n0[0], n1[0], m0x, m1x);
    float ay = dt_herm(0, n0[1], n1[1], m0y, m1y), by = dt_herm(1, n0[1], n1[1], m0y, m1y);
    float cy = dt_herm(2, n0[1], n1[1], m0y, m1y), dy = dt_herm(3, n0[1], n1[1], m0y, m1y);
    *px = ((bx*t + ax) + cx*t2) + dx*t3;
    *py = ((by*t + ay) + cy*t2) + dy*t3;
}

/* Fold one coordinator's serviced components into the current tick accumulator. */
static void dt_fold_coordinator(int64_t coord) {
    if (!coord) return;
    /* One-shot: dump the runtime speed table DAT_140b31440[0..0x96) (FUN_14049d400's lookup, used by
     * the 0x2c drift mover for the per-tick magnitude). Static image is zero; this is the live data. */
    if (!g_dt_tab_done && g_dt_imgbase) {
        const float *tab = (const float *)(g_dt_imgbase + 0xb31440);
        char tb[64];
        for (int i = 0; i < 0x96; i++) {
            snprintf(tb, sizeof tb, "DTTAB\ti=%d\tv=%.6f\n", i, tab[i]);
            log_write(tb);
        }
        g_dt_tab_done = 1;
    }
    /* One-shot: dump the runtime Hermite basis matrix DAT_140b2f1f0[0..16) (row-major 4x4; rows =
     * t^0..t^3 coefficients, cols = p0,p1,m0,m1). Static image is zero; loaded at runtime. */
    if (!g_dt_mat_done && g_dt_imgbase) {
        const float *mat = (const float *)(g_dt_imgbase + 0xb2f1f0);
        char mb[64];
        for (int i = 0; i < 16; i++) {
            snprintf(mb, sizeof mb, "DTMAT\ti=%d\tv=%.6f\n", i, mat[i]);
            log_write(mb);
        }
        g_dt_mat_done = 1;
    }
    /* `coord` is the serviced GameObjectClass (a76b0's param). entity+0x78/+0x7c/+0x80 = the
     * unit's authoritative SIM world position (x/y/z floats, written each tick by the locomotor
     * vfunc_6 — deterministic, distinct from the render-side node transform). Fold it for
     * transform coverage, before the hardpoint-disabled gate so every serviced ship is counted. */
    uint32_t px = *(uint32_t *)(coord + 0x78);
    uint32_t py = *(uint32_t *)(coord + 0x7c);
    uint32_t pz = *(uint32_t *)(coord + 0x80);
    dt_fold(&px, 4); dt_fold(&py, 4); dt_fold(&pz, 4);
    g_dt_pos_n++;
    if (!g_dt_pos_have) {
        memcpy(&g_dt_pos_xyz[0], &px, 4);
        memcpy(&g_dt_pos_xyz[1], &py, 4);
        memcpy(&g_dt_pos_xyz[2], &pz, 4);
        g_dt_pos_have = 1;
        /* Same ship: capture its locomotor velocity + state + family for the integration oracle. */
        int64_t lst = *(int64_t *)(coord + 0xa8);          /* locomotor_state */
        if (lst) {
            memcpy(&g_dt_vel_xyz[0], (void *)(lst + 0x14), 4);
            memcpy(&g_dt_vel_xyz[1], (void *)(lst + 0x18), 4);
            memcpy(&g_dt_vel_xyz[2], (void *)(lst + 0x1c), 4);
            g_dt_lstate = *(int32_t *)(lst + 0x48);
            memcpy(&g_dt_heading, (void *)(coord + 0x8c), 4);  /* heading angle (deg) */
            memcpy(&g_dt_speed,   (void *)(lst + 0xec), 4);    /* speed scalar */
            g_dt_timer = *(int32_t *)(lst + 0x5c);             /* 0x2c drift timer (table index) */
            /* Spline-segment capture in 0x13 Moving state: the two active control points. */
            if (g_dt_lstate == 0x13) {
                int64_t base = *(int64_t *)(lst + 0x18);       /* node array begin (state+0x18) */
                int64_t end  = *(int64_t *)(lst + 0x20);       /* end (state+0x20) */
                int32_t idx  = *(int32_t *)(lst + 0x60);       /* current segment index */
                if (base && end > base) {
                    int32_t cnt = (int32_t)((end - base) / 0x34);
                    if (idx >= 0 && idx + 1 < cnt) {
                        const float *n0 = (const float *)(base + (int64_t)idx * 0x34);
                        const float *n1 = (const float *)(base + (int64_t)(idx + 1) * 0x34);
                        for (int k = 0; k < 6; k++) {          /* pos[0..2], tangent[3..5] */
                            g_dt_spl_n0[k] = n0[k]; g_dt_spl_n1[k] = n1[k];
                        }
                        g_dt_spl_n0[6] = n0[6];  g_dt_spl_n1[6] = n1[6];   /* weight */
                        g_dt_spl_n0[7] = n0[0xc]; g_dt_spl_n1[7] = n1[0xc]; /* arc-param */
                        g_dt_spl_idx = idx;
                        g_dt_spl_z = *(float *)(coord + 0x80);  /* held z */
                        g_dt_spl_have = 1;
                    }
                }
            }
            g_dt_ent = (uint64_t)coord;
            g_dt_loco_rva = g_dt_imgbase ? dt_loco_vtbl_rva(coord, g_dt_imgbase) : 0;
            g_dt_loco_have = 1;
        }
    }
    /* Starship-family latch: first ship this tick with a StarshipLocomotorBehaviorClass — the unit
     * type the lifted integrator models. Auto-targets the right units when a battle has them. */
    if (!g_dt_ss_have && g_dt_imgbase && dt_loco_vtbl_rva(coord, g_dt_imgbase) == 0x8ae250) {
        int64_t lst = *(int64_t *)(coord + 0xa8);
        if (lst) {
            memcpy(&g_dt_ss_vel[0], (void *)(lst + 0x14), 4);
            memcpy(&g_dt_ss_vel[1], (void *)(lst + 0x18), 4);
            memcpy(&g_dt_ss_vel[2], (void *)(lst + 0x1c), 4);
            g_dt_ss_state = *(int32_t *)(lst + 0x48);
            g_dt_ss_ent = (uint64_t)coord;
            g_dt_ss_have = 1;
        }
    }
    if (*(uint8_t *)(coord + 0x3a0) & 0x40) return;        /* coordinator disabled */
    int64_t L = *(int64_t *)(coord + 0x2d0);
    if (!L) return;
    int32_t n = *(int32_t *)(L + 0x10);
    int64_t arr = *(int64_t *)(L + 0x8);
    if (!arr || n <= 0 || n > 1000000) return;             /* sanity bound */
    for (int i = 0; i < n; i++) {
        int64_t e = *(int64_t *)(arr + (int64_t)i * 8);
        if (!e) continue;
        int64_t ent = *(int64_t *)(e + 0x20);
        if (!ent) continue;
        if (*(int8_t *)(ent + 0x4d) != 1) continue;        /* match a76b0's active filter */
        uint32_t idx       = (uint32_t)i;
        int32_t  motion    = *(int32_t *)(ent + 0x48);     /* motion-state (value) */
        uint32_t speedbits = *(uint32_t *)(e + 0x28);      /* component speed float bits */
        uint32_t countdown = *(uint32_t *)(e + 0x58);      /* rate-limit countdown */
        dt_fold(&idx, 4);
        dt_fold(&motion, 4);
        dt_fold(&speedbits, 4);
        dt_fold(&countdown, 4);
        g_dt_count++;
        /* NOTE: the absolute world transform is deliberately NOT folded. It lives in the
         * render-side Alamo node manager (resolved via FUN_140385cf0/FUN_14012d2c0, gated on
         * entity+0x4e==1), which is NEVER set for the sim-serviced hardpoint components. The
         * authoritative SIM position is the flat GameObject field at coord+0x78, folded once per
         * coordinator above (this `e` is a HardPointClass; its owner ship = coord). The render
         * matrix is a separate representation. See openspec movement_structs.md "Transform". */
    }
}

/* =========================================================================
 * DTSTEER — Fighter steering controller (FUN_1405caaf0) input/output capture.
 *
 * The full FighterLocomotorBehaviorClass orientation controller can't be
 * validated from the read-only per-tick fold: its STEERING TARGET is a stack
 * local the mover (e.g. FUN_1405cc220) computes and passes as param_2 — it is
 * NOT at any fixed entity/order offset (decomp/5cc220.c:180-223). So we wrap
 * FUN_1405caaf0 itself with an inline trampoline, latch ONE maneuvering fighter,
 * and emit one DTSTEER line per tick carrying the controller's COMPLETE input
 * set plus the before/after orientation:
 *   owner pos (entity+0x78/7c/80), target (param_2[0..2]),
 *   roll/pitch/yaw before & after (entity+0x84/88/8c),
 *   yaw budget (*param_4), pitch budget (*param_6), speed budget (*param_5),
 *   commanded max speed (param_3, float), locomotor state (state+0x48),
 *   hard-turn flag (state+0x1d4).
 * Offline this lets the lifted controller be checked: predict(before, target,
 * budgets) == after, and after[t] == before[t+1] (continuity across ticks).
 *
 * arg count = 7 (counted from the call site decomp/5cc220.c:223 — under-counting
 * corrupts the stack). Calling convention: param_3 is a FLOAT (3rd positional
 * arg) -> xmm2 under win64; the typedef MUST declare it `float` so the compiler
 * marshals it correctly on the forward call.
 *
 * Prologue (objdump): 48 8b c4 / 48 89 58 18 / 48 89 70 20 / 55 / 41 56 = 14
 * bytes ending cleanly before `push r15` — exactly the FF25 trampoline width.
 * Profile-build only; runtime-gated by EAW_DIFFTRACE=1.
 * ========================================================================= */
#define STEER_CAAF0_RVA   0x5caaf0ULL
#define STEER_STALE_TICKS 240u
static const BYTE steer_caaf0_prologue[14] = {
    0x48, 0x8b, 0xc4,              /* mov rax, rsp        */
    0x48, 0x89, 0x58, 0x18,        /* mov [rax+0x18], rbx */
    0x48, 0x89, 0x70, 0x20,        /* mov [rax+0x20], rsi */
    0x55,                          /* push rbp            */
    0x41, 0x56,                    /* push r14            */
};
typedef void (*Caaf0Fn)(int64_t behavior, float *target, float maxspeed,
                        float *yaw_budget, float *speed_budget, float *pitch_budget,
                        int32_t flag);
static Caaf0Fn  g_caaf0_trampoline = NULL;
static int64_t  g_steer_ent      = 0;             /* latched fighter entity */
static uint32_t g_steer_lasttick = 0xFFFFFFFFu;   /* last tick a line was emitted for it */
static uint32_t g_steer_seentick = 0;             /* last tick the latch was serviced */

static void b5caaf0_hook(int64_t behavior, float *target, float maxspeed,
                         float *yaw_budget, float *speed_budget, float *pitch_budget,
                         int32_t flag)
{
    int64_t  entity = behavior ? *(int64_t *)(behavior + 0x28) : 0;
    int      do_capture = 0;
    uint32_t tick = 0;

    if (entity && g_dt_frame_ctr && dt_on()) {
        tick = *g_dt_frame_ctr;
        if (!g_steer_ent) {
            g_steer_ent = entity; g_steer_seentick = tick;
        } else if (entity != g_steer_ent &&
                   (uint32_t)(tick - g_steer_seentick) > STEER_STALE_TICKS) {
            g_steer_ent = entity; g_steer_seentick = tick;   /* old fighter died/idled */
        }
        if (entity == g_steer_ent && tick != g_steer_lasttick) do_capture = 1;
    }

    if (!do_capture) {
        g_caaf0_trampoline(behavior, target, maxspeed,
                           yaw_budget, speed_budget, pitch_budget, flag);
        return;
    }

    /* --- inputs, sampled BEFORE the controller mutates angles/budgets --- */
    float ox = *(float *)(entity + 0x78), oy = *(float *)(entity + 0x7c), oz = *(float *)(entity + 0x80);
    float r0 = *(float *)(entity + 0x84), p0 = *(float *)(entity + 0x88), y0 = *(float *)(entity + 0x8c);
    float tx = target ? target[0] : 0.0f, ty = target ? target[1] : 0.0f, tz = target ? target[2] : 0.0f;
    float yb = yaw_budget   ? *yaw_budget   : 0.0f;
    float pb = pitch_budget ? *pitch_budget : 0.0f;
    float sb = speed_budget ? *speed_budget : 0.0f;
    int32_t state = -1, ht = -1;
    int64_t lst = *(int64_t *)(entity + 0xa8);
    if (lst) { state = *(int32_t *)(lst + 0x48); ht = *(uint8_t *)(lst + 0x1d4); }

    g_caaf0_trampoline(behavior, target, maxspeed,
                       yaw_budget, speed_budget, pitch_budget, flag);

    /* --- outputs: the new orientation the controller committed --- */
    float r1 = *(float *)(entity + 0x84), p1 = *(float *)(entity + 0x88), y1 = *(float *)(entity + 0x8c);

    char s[448];
    snprintf(s, sizeof s,
        "DTSTEER\ttick=%u\tent=%llx\tstate=%d\tht=%d\tox=%.6f\toy=%.6f\toz=%.6f\t"
        "tx=%.6f\tty=%.6f\ttz=%.6f\tr0=%.6f\tp0=%.6f\ty0=%.6f\tr1=%.6f\tp1=%.6f\ty1=%.6f\t"
        "yb=%.6f\tpb=%.6f\tsb=%.6f\tms=%.6f\n",
        tick, (unsigned long long)entity, state, ht, ox, oy, oz, tx, ty, tz,
        r0, p0, y0, r1, p1, y1, yb, pb, sb, maxspeed);
    log_write(s);
    g_steer_lasttick = tick; g_steer_seentick = tick;
}

static BOOL install_b5caaf0_hook(void)
{
    HMODULE exe = GetModuleHandleA(NULL);
    if (!exe) return FALSE;
    BYTE *fn = (BYTE *)exe + STEER_CAAF0_RVA;
    if (memcmp(fn, steer_caaf0_prologue, 14) != 0) {
        log_write("[eaw-mt] WARN: FUN_1405caaf0 prologue mismatch — DTSTEER not installed\n");
        return FALSE;
    }
    BYTE *tramp = (BYTE *)VirtualAlloc(NULL, 64, MEM_COMMIT|MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    if (!tramp) {
        log_write("[eaw-mt] WARN: VirtualAlloc for DTSTEER trampoline failed\n");
        return FALSE;
    }
    memcpy(tramp, steer_caaf0_prologue, 14);
    write_abs_jmp(tramp + 14, (uint64_t)(fn + 14));
    g_caaf0_trampoline = (Caaf0Fn)tramp;

    DWORD old;
    VirtualProtect(fn, 14, PAGE_EXECUTE_READWRITE, &old);
    write_abs_jmp(fn, (uint64_t)b5caaf0_hook);
    VirtualProtect(fn, 14, old, &old);
    FlushInstructionCache(GetCurrentProcess(), fn, 14);
    log_write("[eaw-mt] DTSTEER: FUN_1405caaf0 fighter-steering hook installed\n");
    return TRUE;
}

/* =========================================================================
 * DTYAW — isolated capture of the yaw/roll bank-to-turn integrator (FUN_1405c95a0).
 *
 * DTSTEER captures the WHOLE controller (FUN_1405caaf0); its pitch channel was
 * validated, but the yaw channel is the bank-to-turn integrator FUN_1405c95a0,
 * which needs the EXACT yaw-error and turn-rate constants the controller hands it.
 * Rather than reconstruct that error offline through the full caaf0 pipeline, we
 * wrap FUN_1405c95a0 DIRECTLY and capture its complete I/O:
 *   roll/pitch/yaw before & after (entity+0x84/88/8c),
 *   ye  = the signed yaw error (param_2, the float in xmm1),
 *   bud0/bud1 = the turn budget (*param_3) before & after,
 *   a/b = the raw unit-template turn rates (template+0x38c/+0x394) — c95a0 uses
 *         only their ratio so the game-speed scale cancels; capture them RAW,
 *   cap = the yaw clamp (template+0x39c), read RAW.
 * Offline: fighter_steer_yaw(roll0,yaw0,ye,bud0,a,b,cap) == {roll1,yaw1,bud1}.
 *
 * arg count = 3 (decomp/5c95a0.c): param_1=rcx, param_2=FLOAT (xmm1, 2nd
 * positional), param_3=float* (r8). The typedef MUST declare param_2 `float`.
 *
 * Prologue (objdump): 48 89 5c 24 10 / 57 / 48 81 ec 80 00 00 00 / 0f 29 74 24 70
 * = 5+1+7+5 = 18 bytes (first instruction boundary >= the 14-byte FF25 width; none
 * are RIP-relative -> relocatable). Own latch, independent of DTSTEER.
 * Profile-build only; runtime-gated by EAW_DIFFTRACE=1.
 * ========================================================================= */
#define STEER_C95A0_RVA   0x5c95a0ULL
static const BYTE steer_c95a0_prologue[18] = {
    0x48, 0x89, 0x5c, 0x24, 0x10,  /* mov [rsp+0x10], rbx */
    0x57,                          /* push rdi            */
    0x48, 0x81, 0xec, 0x80, 0x00, 0x00, 0x00,  /* sub rsp, 0x80 */
    0x0f, 0x29, 0x74, 0x24, 0x70,  /* movaps [rsp+0x70], xmm6 */
};
typedef void (*C95a0Fn)(int64_t behavior, float yaw_err, float *yaw_budget);
static C95a0Fn  g_c95a0_trampoline = NULL;
static int64_t  g_yaw_ent      = 0;
static uint32_t g_yaw_lasttick = 0xFFFFFFFFu;
static uint32_t g_yaw_seentick = 0;

static void b5c95a0_hook(int64_t behavior, float yaw_err, float *yaw_budget)
{
    int64_t  entity = behavior ? *(int64_t *)(behavior + 0x28) : 0;
    int      do_capture = 0;
    uint32_t tick = 0;

    if (entity && g_dt_frame_ctr && dt_on()) {
        tick = *g_dt_frame_ctr;
        if (!g_yaw_ent) {
            g_yaw_ent = entity; g_yaw_seentick = tick;
        } else if (entity != g_yaw_ent &&
                   (uint32_t)(tick - g_yaw_seentick) > STEER_STALE_TICKS) {
            g_yaw_ent = entity; g_yaw_seentick = tick;
        }
        if (entity == g_yaw_ent && tick != g_yaw_lasttick) do_capture = 1;
    }

    if (!do_capture) { g_c95a0_trampoline(behavior, yaw_err, yaw_budget); return; }

    /* --- inputs, sampled BEFORE the integrator mutates angles/budget --- */
    float r0 = *(float *)(entity + 0x84), p0 = *(float *)(entity + 0x88), y0 = *(float *)(entity + 0x8c);
    float bud0 = yaw_budget ? *yaw_budget : 0.0f;
    int64_t tmpl = *(int64_t *)(entity + 0x298);
    float a = 0.0f, bb = 0.0f, cap = 0.0f;
    if (tmpl) { a = *(float *)(tmpl + 0x38c); bb = *(float *)(tmpl + 0x394); cap = *(float *)(tmpl + 0x39c); }

    g_c95a0_trampoline(behavior, yaw_err, yaw_budget);

    /* --- outputs --- */
    float r1 = *(float *)(entity + 0x84), y1 = *(float *)(entity + 0x8c);
    float bud1 = yaw_budget ? *yaw_budget : 0.0f;

    char s[384];
    snprintf(s, sizeof s,
        "DTYAW\ttick=%u\tent=%llx\tr0=%.6f\tp0=%.6f\ty0=%.6f\tye=%.6f\tbud0=%.6f\t"
        "a=%.6f\tb=%.6f\tcap=%.6f\tr1=%.6f\ty1=%.6f\tbud1=%.6f\n",
        tick, (unsigned long long)entity, r0, p0, y0, yaw_err, bud0, a, bb, cap, r1, y1, bud1);
    log_write(s);
    g_yaw_lasttick = tick; g_yaw_seentick = tick;
}

static BOOL install_b5c95a0_hook(void)
{
    HMODULE exe = GetModuleHandleA(NULL);
    if (!exe) return FALSE;
    BYTE *fn = (BYTE *)exe + STEER_C95A0_RVA;
    if (memcmp(fn, steer_c95a0_prologue, 18) != 0) {
        log_write("[eaw-mt] WARN: FUN_1405c95a0 prologue mismatch — DTYAW not installed\n");
        return FALSE;
    }
    BYTE *tramp = (BYTE *)VirtualAlloc(NULL, 64, MEM_COMMIT|MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    if (!tramp) {
        log_write("[eaw-mt] WARN: VirtualAlloc for DTYAW trampoline failed\n");
        return FALSE;
    }
    memcpy(tramp, steer_c95a0_prologue, 18);
    write_abs_jmp(tramp + 18, (uint64_t)(fn + 18));
    g_c95a0_trampoline = (C95a0Fn)tramp;

    DWORD old;
    VirtualProtect(fn, 14, PAGE_EXECUTE_READWRITE, &old);
    write_abs_jmp(fn, (uint64_t)b5c95a0_hook);
    VirtualProtect(fn, 14, old, &old);
    FlushInstructionCache(GetCurrentProcess(), fn, 14);
    log_write("[eaw-mt] DTYAW: FUN_1405c95a0 yaw/roll-integrator hook installed\n");
    return TRUE;
}

/* =========================================================================
 * DTFIRE — hardpoint fire-budget distribution (FUN_1403a76b0) I/O capture.
 *
 * This is the in-game oracle for the Phase-3 Unit-4 lift (sim/hardpoint.cpp).
 * FUN_1403a76b0 distributes a per-ship fire budget across the ship's HardPointClass
 * mounts (DynamicVectorClass<HardPointClass*> at ship+0x2d0): it sums the
 * game-speed-scaled mount weights (total_w via FUN_140540070), computes
 *   avail   = (capacity(396070) - base(396df0+bias, clamped)) * total_w
 * then per mount
 *   share_i = (budget_i / total_w) * avail
 * and consumes share_i from the mount (FUN_140387f50 decrements mount+0x28).
 *
 * The headline lifted finding is the total_w CANCELLATION:
 *   share_i = budget_i * (capacity - base)
 * — total_w (and therefore the whole 540070 game-speed weight scale) cancels out
 * of every share; weight only GATES distribution (needs total_w>0). The precise
 * oracle for that is: for every active mount that fires, the consumed FRACTION
 *   k_i = (b0_i - b1_i) / b0_i
 * must be a SINGLE shared constant (= capacity-base), INDEPENDENT of the mount's
 * weight w. So we capture, per latched ship per tick, each active mount's
 * budget-before (b0, mount+0x28), weight (w, mount+0x58), and budget-after (b1).
 *
 * We can't call 396070/396df0 ourselves to log capacity/base: 396df0 has a side
 * effect (it writes the clamped fire-fraction back to ship+0x5c). So we recover
 * (capacity-base) purely from the per-mount before/after deltas offline — which is
 * exactly the cross-mount-constant assertion above. The per-mount dispatcher
 * FUN_140387010 (run after distribution) does NOT touch mount+0x28 (verified:
 * decomp/387010.c), so the after-read is the clean post-distribution budget.
 *
 * arg count = 2 (decomp/3a76b0.c): param_1=ship (rcx), param_2=tick (edx, int).
 *
 * Prologue (objdump): 4c 8b dc / 49 89 73 20 / 41 56 / 48 83 ec 70 / 48 8b f1
 * = 3+4+2+4+3 = 16 bytes (>= the 14-byte FF25 width, instruction boundary, none
 * RIP-relative). Own latch, independent of DTSTEER/DTYAW.
 * Profile-build only; runtime-gated by EAW_DIFFTRACE=1.
 * ========================================================================= */
#define FIRE_A76B0_RVA   0x3a76b0ULL
#define FIRE_STALE_TICKS 240u
#define DTFIRE_MAXHP     128
static const BYTE fire_a76b0_prologue[16] = {
    0x4c, 0x8b, 0xdc,              /* mov r11, rsp        */
    0x49, 0x89, 0x73, 0x20,        /* mov [r11+0x20], rsi */
    0x41, 0x56,                    /* push r14            */
    0x48, 0x83, 0xec, 0x70,        /* sub rsp, 0x70       */
    0x48, 0x8b, 0xf1,              /* mov rsi, rcx        */
};
typedef void (*A76b0Fn)(int64_t ship, int32_t tick_param);
static A76b0Fn  g_a76b0_trampoline = NULL;
static int64_t  g_fire_ent      = 0;
static uint32_t g_fire_lasttick = 0xFFFFFFFFu;
static uint32_t g_fire_seentick = 0;

/* Scan a ship's hardpoint vector: count active mounts, sum the raw fire-rate weights
 * (+0x58, what FUN_140540070 feeds into total_w), and note whether any active mount
 * carries budget (+0x28 > 0). The distribution path in FUN_1403a76b0 only consumes
 * when total_w>0 (avail = (cap-base)*total_w) AND a mount has budget. */
static void dtfire_scan(int64_t arr, int count, int *n_active_out, float *total_w_out,
                        int *has_budget_out)
{
    int   na = 0, has_budget = 0;
    float tw = 0.0f;
    for (int i = 0; i < count; i++) {
        int64_t m = *(int64_t *)(arr + (int64_t)i * 8);
        if (m) {
            int64_t owner = *(int64_t *)(m + 0x20);
            if (owner && *(uint8_t *)(owner + 0x4d) == 1) {
                na++;
                tw += *(float *)(m + 0x58);
                if (*(float *)(m + 0x28) > 0.0f) has_budget = 1;
            }
        }
    }
    *n_active_out = na; *total_w_out = tw; *has_budget_out = has_budget;
}

/* Survey counters: across the whole capture, do ANY ships exercise the distribution
 * path? na2 = ship-ticks with >=2 active mounts; twpos = those with total_w>0 (the
 * precondition for the lifted distribution to do anything); consume = those that
 * actually decremented budget. If twpos stays 0 through a battle, the lifted
 * distribution is dormant in this content (zero-weight hardpoints) and weapons fire
 * via the unconditional per-mount dispatch (387010->387400) instead. */
static uint32_t g_surv_na2 = 0, g_surv_twpos = 0, g_surv_consume = 0;
static uint32_t g_surv_last = 0xFFFFFFFFu;
static float    g_surv_maxtw = 0.0f;
static int64_t  g_surv_maxtw_ent = 0;

/* ── DTSPL2: bit-exact oracle for the lifted vfunc_59 cubic-Hermite mover (sim/spline_mover) ───────────
 * Detours SimpleSpaceLocomotorBehaviorClass::vfunc_59 (RVA 0x625990) at its ENTRY. The path/order object
 * is param_2 (NOT the locomotor coordinator): the node array lives in the container at param_2+0x18 and
 * is read through the engine's own indexer FUN_14054fc00; the active segment index is at param_2+0x60,
 * the mover sub-mode at param_2+0x48, and FUN_1405c5910 is the "path exhausted" predicate. We let the
 * original run first (it writes the engine's XY result into *param_7 and advances param_2+0x60), then
 * recompute the position for the SETTLED segment at the same frame-time and compare raw bits. Only the
 * clean cubic-Hermite branch is scored (mode!=5, path not exhausted, segment dt>0, distinct endpoints);
 * the past-end-linear and degenerate-same-endpoint branches write a different formula and are gated out.
 * Lean EAW_ORACLE build; runtime-gated by EAW_DIFFTRACE=1. (Earlier this capture read the locomotor's
 * own +0x18 — the wrong object — and saw base0-for-everyone; the path lives in param_2, hence the move
 * to a real 0x625990 entry detour.) */
#define SPL_625990_RVA 0x625990ULL
static const BYTE spl625990_prologue[14] = {   /* mov %rcx,8(%rsp); push rbp/rbx/rdi/r12/r14/r15 (clean @14) */
    0x48,0x89,0x4c,0x24,0x08, 0x55, 0x53, 0x57, 0x41,0x54, 0x41,0x56, 0x41,0x57
};
typedef uint64_t (*Spl625990Fn)(int64_t,int64_t,uint32_t,float*,float*,float,float*,float*,float*);
static Spl625990Fn g_spl625990_trampoline = NULL;
typedef char    (*SplPredFn)(int64_t);         /* FUN_1405c5910: path-exhausted predicate            */
typedef int64_t (*SplAccFn)(int64_t,int32_t);  /* FUN_14054fc00: node-array indexer -> &node[idx]     */

/* gating-reason counters for the periodic DTSPL2 line (answers "is vfunc_59 even called, and on which
 * branch?" — calls=0 would mean the spline mover is never invoked for the captured scene). */
static uint32_t g_spl2_calls=0, g_spl2_mode5=0, g_spl2_ended=0,
                g_spl2_dt0=0, g_spl2_degen=0, g_spl2_cmp=0;

static uint64_t dt_spl625990_hook(int64_t p1, int64_t p2, uint32_t p3, float *p4, float *p5,
                                  float p6, float *p7, float *p8, float *p9) {
    g_spl2_calls++;
    /* run the real mover first: it writes *p7 (out XY) and *p9 (out speed) and advances p2+0x60 */
    uint64_t ret = g_spl625990_trampoline(p1, p2, p3, p4, p5, p6, p7, p8, p9);

    if (!dt_on()) return ret;                  /* recompute/log only under EAW_DIFFTRACE=1 */
    if (!p2 || !p7 || !g_dt_imgbase || !g_dt_frame_ctr) goto spl2_diag;
    if (*(int32_t *)(p2 + 0x48) == 5) { g_spl2_mode5++; goto spl2_diag; }   /* FUN_140627930 sub-mover */
    {
    SplPredFn ended = (SplPredFn)(g_dt_imgbase + 0x5c5910);
    if (ended(p2)) { g_spl2_ended++; goto spl2_diag; }                      /* past-end / linear branch */
    SplAccFn acc = (SplAccFn)(g_dt_imgbase + 0x54fc00);
    int32_t idx = *(int32_t *)(p2 + 0x60);
    const float *n0 = (const float *)acc(p2 + 0x18, idx);
    const float *n1 = (const float *)acc(p2 + 0x18, idx + 1);
    if (!n0 || !n1) { g_spl2_dt0++; goto spl2_diag; }
    float dT = n1[0xc] - n0[0xc];
    if (!(0.0f < dT)) { g_spl2_dt0++; goto spl2_diag; }                     /* line 85 else (drift)     */
    if (n0[0]==n1[0] && n0[1]==n1[1] && n0[2]==n1[2]) { g_spl2_degen++; goto spl2_diag; } /* lines 92-112 */

    float rpx, rpy;
    dt_spline_pos(n0, n1, (float)p3, &rpx, &rpy);
    uint32_t gx, gy, ex, ey;
    memcpy(&gx, &rpx, 4);   memcpy(&gy, &rpy, 4);
    memcpy(&ex, &p7[0], 4); memcpy(&ey, &p7[1], 4);
    g_spl2_cmp++; g_dt_spl2_total++;
    if (gx == ex && gy == ey) g_dt_spl2_pass++;
    else {
        g_dt_spl2_fail++;
        if (!g_dt_spl2_shown) {
            g_dt_spl2_shown = 1;
            char xb[256];
            snprintf(xb, sizeof xb,
                "DTSPL2MISS\ttick=%u\tidx=%d\trecomp=(%.6f,%.6f/%08x,%08x)\teng=(%.6f,%.6f/%08x,%08x)\n",
                *g_dt_frame_ctr, idx, rpx, rpy, gx, gy, p7[0], p7[1], ex, ey);
            log_write(xb);
        }
    }
    }
spl2_diag:
    { uint32_t tk = g_dt_frame_ctr ? *g_dt_frame_ctr : 0;
      if (tk != g_dt_spl2_last && (tk & 0x3ffu) == 0) {   /* every 1024 ticks */
        g_dt_spl2_last = tk;
        char db[256];
        snprintf(db, sizeof db,
            "DTSPL2\ttick=%u\tpass=%u\tfail=%u\tcmp=%u\tcalls=%u\tmode5=%u\tended=%u\tdt0=%u\tdegen=%u\n",
            tk, g_dt_spl2_pass, g_dt_spl2_fail, g_spl2_cmp, g_spl2_calls,
            g_spl2_mode5, g_spl2_ended, g_spl2_dt0, g_spl2_degen);
        log_write(db);
      } }
    return ret;
}

static BOOL install_spl625990_hook(void) {
    HMODULE exe = GetModuleHandleA(NULL);
    if (!exe) return FALSE;
    BYTE *fn = (BYTE *)exe + SPL_625990_RVA;
    if (memcmp(fn, spl625990_prologue, sizeof spl625990_prologue) != 0) {
        log_write("[eaw-mt] WARN: vfunc_59 (0x625990) prologue mismatch — DTSPL2 not installed\n");
        return FALSE;
    }
    BYTE *tramp = (BYTE *)VirtualAlloc(NULL, 64, MEM_COMMIT|MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    if (!tramp) { log_write("[eaw-mt] WARN: VirtualAlloc for DTSPL2 trampoline failed\n"); return FALSE; }
    memcpy(tramp, spl625990_prologue, sizeof spl625990_prologue);   /* 7 prologue instrs == 14 bytes */
    write_abs_jmp(tramp + sizeof spl625990_prologue, (uint64_t)(fn + sizeof spl625990_prologue));
    g_spl625990_trampoline = (Spl625990Fn)tramp;

    DWORD old;
    VirtualProtect(fn, 14, PAGE_EXECUTE_READWRITE, &old);
    write_abs_jmp(fn, (uint64_t)dt_spl625990_hook);
    VirtualProtect(fn, 14, old, &old);
    FlushInstructionCache(GetCurrentProcess(), fn, 14);
    log_write("[eaw-mt] DTSPL2: vfunc_59 (0x625990) Hermite-mover entry oracle installed\n");
    return TRUE;
}

/* ════════════════════════════════════════════════════════════════════════════════════════════════
 * ENGINE-SOURCE INTEGRATION — Increment 1: the real WorldApply adapter + the DTWA in-game oracle.
 *
 * The parallel-command system (sim/sim_parallel.{h,cpp}, sim/command_sink.h) is host-validated; the
 * design doc (sim_parallel_command_schema.md §10) lists the *real* WorldApply as the one still-
 * unbuilt piece that has an in-game validation path now. WorldApply is the Phase-B boundary the drain
 * applies ops to (sim/sim_parallel.h:109): create_object (Class 2), apply_signal (Class 2b), flush_sfx
 * (Class 3). Here we realize it against the live engine functions and validate, against the engine's
 * OWN calls, the determinism claims the whole schema rests on — chiefly invariant I1 (object ids are
 * assigned by dense append == canonical order). No live mutation, no threads this increment.
 * ════════════════════════════════════════════════════════════════════════════════════════════════ */

/* ── Piece 1: the real WorldApply adapter (sim::WorldApply realized against the live engine). ───────
 * Resolves the live manager/dispatcher and marshals args per the confirmed call-site signatures
 * (FUN_14029f810 / FUN_140240940 / FUN_1402d5290). Per the approved scope these are BUILT but NOT
 * invoked to mutate this increment (calling create_object live would double-spawn); the pointer
 * resolution + arg layout they use is exactly what the DTWA oracle below validates against the
 * engine's own calls. Wiring them into a live Phase-B drain is the next, separately-gated increment. */
typedef int64_t (*EngCreateObjFn)(int64_t,int64_t,int32_t,const float*,int64_t,int8_t,int8_t);
typedef void    (*EngSignalFn)(int64_t,uint32_t,const void*);   /* FUN_140240940(dispatcher,sig,payload) */
typedef int32_t (*EngSfxFn)(int64_t,int64_t,int32_t,int8_t,uint32_t);

/* combat-mode singleton (DAT_140b15418) -> GameObjectManager (param_1 of CreateObject, +0x18). */
static int64_t eng_world_manager(void) {
    if (!g_dt_imgbase) return 0;
    int64_t combat = *(int64_t *)(g_dt_imgbase + 0xb15418);
    return combat ? *(int64_t *)(combat + 0x18) : 0;
}
/* Class 2: append-create. Returns the new object's OWN id (obj+0x50 = the monotonic id allocated by
 * FUN_1402ac980 off mgr+0x620; NOT obj+0x58, which is param_3 = the owner/parent ref). NOT called yet. */
__attribute__((used)) static int64_t eng_create_object(uint32_t template_id, const float pos[3], uint32_t flags) {
    int64_t mgr = eng_world_manager();
    if (!mgr) return -1;
    EngCreateObjFn fn = (EngCreateObjFn)(g_dt_imgbase + 0x29f810);
    int64_t obj = fn(mgr, (int64_t)template_id, (int32_t)flags, pos, 0, 1, 0);
    return obj ? (int64_t)*(int32_t *)(obj + 0x50) : -1;
}
/* Class 2b: per-object +0x38 signal fan-out (analogue of one FUN_140220ed0 call). NOT called yet. */
__attribute__((used)) static void eng_apply_signal(void *emitter, uint32_t sig_id, const void *payload) {
    if (!g_dt_imgbase || !emitter) return;
    EngSignalFn fn = (EngSignalFn)(g_dt_imgbase + 0x240940);
    fn((int64_t)emitter + 0x38, sig_id, payload);
}
/* Class 3: presentation cue (off-lockstep). NOT called this increment. */
__attribute__((used)) static void eng_flush_sfx(uint32_t sfx_id) {
    if (!g_dt_imgbase) return;
    EngSfxFn fn = (EngSfxFn)(g_dt_imgbase + 0x2d5290);
    fn((int64_t)(g_dt_imgbase + 0xb27e60), (int64_t)sfx_id, 0, 0, 1);
}

/* ── Piece 2a: DTWA-SPAWN — detours GameObjectManager::CreateObject (RVA 0x29f810) at entry. ─────────
 * RE-POINTED 2026-06-05 to the CORRECT id field. The static append-site decompile (commit 4ab7467)
 * found the object carries TWO id-like fields 8 bytes apart:
 *   obj+0x50 (plVar4[10]) = the object's OWN unique id, allocated by FUN_1402ac980(mgr) as a
 *                           POST-INCREMENT monotonic counter at mgr+0x620 (saturating 0x3fffff,
 *                           per-manager, never reused). Key of the canonical map at mgr+0x80. THIS is
 *                           the I1 subject — the dense, monotonic, gap-free id.
 *   obj+0x58 (plVar4[0xb]) = CreateObject's param_3 = an OWNER/PARENT ref (shared by siblings, reused).
 * The earlier DTWA rounds read obj+0x58 and "contradicted" I1 — a DTSPL2 wrong-field harness bug. This
 * version reads obj+0x50 and proves the allocator directly: snapshot ctr_b = *(int32_t*)(mgr+0x620)
 * BEFORE the real call; after, the OWN id must equal ctr_b (post-increment returns the old value) and
 * the counter must have advanced by exactly one (ctr_a == ctr_b+1). That pair IS strict, gap-free,
 * per-manager monotonicity. We also keep the eng_world_manager()==param_1 resolution check.
 * No mutation. Lean EAW_ORACLE build; runtime-gated by EAW_DIFFTRACE=1.
 * Prologue (objdump 0x29f810): 4c894c2420 / 4889542410 / 53 / 4154 / 4156 = 15 bytes, clean before
 * `push r15`; all position-independent (mov [rsp+x],reg + pushes). */
#define DTWA_29F810_RVA 0x29f810ULL
#define DTWA_MGR_CTR_OFF 0x620          /* per-manager monotonic id counter (FUN_1402ac980 post-incr) */
#define DTWA_OBJ_OWNID_OFF 0x50         /* obj's own monotonic id (plVar4[10]) */
#define DTWA_OBJ_OWNER_OFF 0x58         /* obj's owner/parent ref (plVar4[0xb] = param_3) */
#define DTWA_SATUR_ID 0x3fffff          /* counter saturation value (also the constructor init) */
static const BYTE dtwa_29f810_prologue[15] = {  /* mov [rsp+0x20],r9; mov [rsp+0x10],rdx; push rbx/r12/r14 */
    0x4c,0x89,0x4c,0x24,0x20, 0x48,0x89,0x54,0x24,0x10, 0x53, 0x41,0x54, 0x41,0x56
};
typedef int64_t (*Create29f810Fn)(int64_t,int64_t,int32_t,const float*,int64_t,int8_t,int8_t);
static Create29f810Fn g_dtwa_29f810_trampoline = NULL;
/* id_eq_ctr = own_id == counter-before (id is the pre-increment value); ctr_grew1 = counter advanced
 * by exactly one; both == calls (with fails 0) is the bit-exact I1 confirmation. satur/errpath flag the
 * 22-bit overflow + the mgr+0x63e early-return path where no fresh id is assigned. */
static uint32_t g_dtwa_calls=0, g_dtwa_null=0, g_dtwa_id_eq_ctr=0, g_dtwa_ctr_grew1=0,
                g_dtwa_mono=0, g_dtwa_mgr_pass=0, g_dtwa_mgr_fail=0, g_dtwa_schemafit=0,
                g_dtwa_satur=0, g_dtwa_errpath=0, g_dtwa_idfail=0, g_dtwa_ctrfail=0;
static uint32_t g_dtwa_last = 0xFFFFFFFFu;
static int      g_dtwa_shown = 0, g_dtwa_mgrshown = 0;
/* per-manager last own-id, to confirm each manager's id strictly increases across its spawns. Few
 * managers exist; a tiny linear table suffices. */
#define DTWA_NMGR 16
static int64_t  g_dtwa_mgr_key[DTWA_NMGR];
static int32_t  g_dtwa_mgr_lastid[DTWA_NMGR];
static int      g_dtwa_nmgr = 0;
static uint32_t g_dtwa_detail = 0;      /* first-N per-call detail lines */

/* The id source is selected by mgr+0x63e: when SET, FUN_1402ac980 delegates to FUN_14028a9e0 which
 * post-increments a GLOBAL counter at DAT_140b153e0+0x80 (RVA 0xb15460); when CLEAR it post-increments
 * the per-manager counter mgr+0x620. We snapshot BOTH before the call and validate against the one the
 * flag selects, so the check holds in every game mode. */
#define DTWA_GLOBAL_CTR_RVA 0xb15460ULL
/* ── DT-DRAIN (milestone a0, inproc_integration_milestone.md §1): in-game validation of the §7 canonical
 * drain key. FUN_1403a76b0 services each ship in master-list VISITATION order; b3a76b0_hook stamps the
 * current emitter's (visitation rank, obj+0x50 id) and opens an attribution window around the real call,
 * so a projectile spawn (29f810) fired inside that window is attributed to the firing ship. At each
 * emitter TRANSITION within a tick we count how the two candidate drain keys move in emission order:
 *   rank_down : a later emitter had a SMALLER rank   -> the (rank,seq) key INVERTS emission order  (expect 0)
 *   id_down   : a later emitter had a SMALLER obj+0x50 -> emission order is descending-id (head-insert)
 *   id_up     : a later emitter had a LARGER  obj+0x50 -> an ascending-id sort agrees with emission order
 * Headline (the in-game analogue of shard_scheduler_test's negative control):
 *   rank_down == 0   => (gom,rank,seq) reproduces the live serial emission order, AND
 *   id_down  >> id_up => an (object_id,seq) drain key would REORDER the creates -> desync.
 * Observe-only; no control-flow change; all under dt_on() (EAW_DIFFTRACE=1). */
static uint32_t g_drain_tick      = 0xFFFFFFFFu;
static uint32_t g_drain_rank      = 0;     /* per-tick visitation counter (reset each tick) */
static uint32_t g_drain_cur_rank  = 0;     /* visitation rank of the ship currently in 3a76b0 */
static int32_t  g_drain_cur_objid = 0;     /* obj+0x50 of that ship */
static int      g_drain_in_window = 0;     /* inside the 3a76b0 trampoline => a spawn is attributable */
static int      g_drain_have_prev = 0;     /* per-tick: a prior distinct emitter has been recorded */
static uint32_t g_drain_prev_rank = 0;
static int32_t  g_drain_prev_objid= 0;
static uint32_t g_drain_attr=0, g_drain_unattr=0, g_drain_trans=0,
                g_drain_rank_up=0, g_drain_rank_down=0,
                g_drain_id_up=0, g_drain_id_down=0, g_drain_id_eq=0;
static uint32_t g_drain_last=0xFFFFFFFFu, g_drain_detail=0;

/* ── DTWA-B3 (firing_body_lift_scope.md §3/§8.8): in-game STRUCTURAL oracle for the b3 projectile
 * create+init restructure. b3 (host) split 3825b0:261-401 into a Phase-A ProjectileInit payload + a
 * Phase-B applier; this oracle proves the §3 FIELD→SOURCE MAP against the live binary — i.e. the
 * proj+0xe8 record the binary writes inline really comes from the sources the lift reads, so the lift's
 * payload would reproduce it. dtwa_b3_3825b0_hook entry-detours the firing body; the projectile is
 * captured via the 29f810 piggyback above; at exit the binary's record is compared to a recompute from
 * PURE memory reads (no engine calls — 5400f0's lifetime is read from its cache field owner_type+0x4a0
 * that the binary just filled, sidestepping its memoizing write). Observe-only; EAW_DIFFTRACE=1. */
static int     g_b3_in_fire   = 0;       /* inside a 3825b0 call → next 29f810 create is the projectile */
static int64_t g_b3_last_proj = 0;       /* the projectile captured by the 29f810 piggyback */
static int64_t g_pf_reimpl_p2 = 0;       /* the reimpl's RESOLVED param_2 (post-405870 redirect) for the tgt oracle */
static uint32_t g_b3_n=0, g_b3_skip=0, g_b3_charge=0;
static uint32_t g_b3_firer_ok=0,g_b3_firer_bad=0, g_b3_tgt_ok=0,g_b3_tgt_bad=0,
                g_b3_sub_ok=0,g_b3_sub_bad=0,g_b3_sub_reasn=0, g_b3_dmg_ok=0,g_b3_dmg_bad=0,
                g_b3_life_ok=0,g_b3_life_bad=0, g_b3_vis_ok=0,g_b3_vis_bad=0;
static uint32_t g_b3_detail=0, g_b3_missshown=0, g_b3_last=0xFFFFFFFFu;

/* ── PATH-A / A3 chunk R2: create-args observe capture (firing_body_lift_scope.md §8.14) ───────────────
 * The §8.14 A-reimpl applier (pfire_apply_spawn, a later post-R1 chunk) must re-issue the projectile
 * create with the SAME args the binary passes to 29f810 at 3825b0:266:
 *     29f810(mgr=*(owner+0x2b8), template=lVar7, type_index=*(294bc0(&registry,owner+0x58)+0x4c),
 *            pos=&local_2b8, orient=&local_230, 1, 0)
 * Those args are exactly the params the existing dtwa_29f810_hook already receives (arg3 "flags" IS the
 * type_index; arg5 "p5" IS the orient ptr). R2-observe captures them when the firing-window piggyback
 * fires (the FIRST create inside a 3825b0 call = the projectile), then validates the RNG-INDEPENDENT ones
 * the applier reconstructs from firer state — mgr == *(owner+0x2b8) (re-confirms the Int #1 / I2
 * manager-resolution the applier depends on) and template == proj+0x298 — and logs pos/type_index for the
 * payload map. (pos/orient are spread-RNG-perturbed geometry → stored, not bit-asserted here; their math is
 * DTFINT-validated and their in-game fidelity lands at the A3.3 takeover gate.) Observe-only, RNG-free,
 * zero control-flow risk — same class as DTWA-B3. */
typedef struct {
    int64_t mgr, template_id; int32_t type_index; float pos[3]; int64_t orient;
    int have;
} PfireSpawnArgs;
static PfireSpawnArgs g_b3_args;          /* args of the in-flight projectile's 29f810 create */
static uint32_t g_b3_arg_mgr_ok=0, g_b3_arg_mgr_bad=0, g_b3_arg_tmpl_ok=0, g_b3_arg_tmpl_bad=0,
                g_b3_arg_n=0, g_b3_arg_detail=0;

/* ── PATH-A / A3 chunk R1 (firing_body_lift_scope.md §8.14): the DECISION/geometry skeleton, transcribed
 * to C from 3825b0:60-260. The §8.14 A-reimpl runs the fire DECISION by CALLING the binary gate leaves
 * (pure reads on stable state per the §8.5 write-audit) + the glue. R1-gate-1 is the first bounded,
 * RNG-FREE sub-chunk (3825b0:60-105): the early gates that early-return 0 on failure. It is observe-
 * validated one-directionally: a fired shot (binary r==1) MUST have passed every early gate, so
 * binary r==1 ⇒ pfire_r1_gate1()==1 (g1_bug counts the impossible r==1 & g1==0; g1_nofire counts the
 * expected g1==1 & r==0 = passed-early-gates-but-blocked-later). Run BEFORE the trampoline on the same
 * pre-call state the binary sees; the only side-effectful gate (35f470 rebuilds a per-call global fog
 * buffer) is idempotent — the binary's own call rebuilds it again, so the binary is unperturbed.
 * Types mirror the decompile: p2 = longlong* (so `p2+N` is *8 byte indexing; `(int64_t)p2+N` is bytes). */
typedef char    (*R1_39b140Fn)(int64_t*);                       /* FUN_14039b140(param_2) */
typedef char    (*R1_540140Fn)(int64_t, int64_t*);              /* FUN_140540140(owner_type, param_2) */
typedef char    (*R1_35f470Fn)(int64_t, int32_t, int64_t*, int32_t); /* FUN_14035f470(b15418,sid,p2,1) */
typedef char    (*R1_39a540Fn)(int64_t, int64_t*);             /* FUN_14039a540(owner, param_2) */
typedef int64_t (*R1_VfuncFn)(int64_t*, int32_t);              /* (*(*p2+0x10))(p2, selector) */
static uint32_t g_r1_g1_ok=0, g_r1_g1_bug=0, g_r1_g1_nofire=0, g_r1_g1_n=0;
static uint32_t g_r1_g2_ok=0, g_r1_g2_bug=0, g_r1_g2_nofire=0;   /* combined gate1+gate2a verdict */

/* Returns 1 if the firer passes the 3825b0:60-105 early gates (binary would continue), else 0. */
static int pfire_r1_gate1(int64_t p1, int64_t *p2, int64_t p3) {
    if (!g_dt_imgbase) return 1;                       /* can't eval → don't claim a verdict */
    int64_t owner = *(int64_t *)(p1 + 0x10);          /* 3825b0:62 */
    if (owner == 0) return 0;
    if (p2 == 0) return 0;                             /* :65 */
    if (p3 != 0 && *(int64_t *)(p3 + 0x10) != (int64_t)p2) return 0;   /* :68 */
    if ((*(uint8_t *)((int64_t)p2 + 0x74 * 8) & 0x40) != 0) return 0;  /* :71  param_2+0x74 (longlong*) */
    int64_t owner_type = *(int64_t *)(p1 + 0x20);

    R1_VfuncFn vf = *(R1_VfuncFn *)(*(int64_t *)p2 + 0x10);            /* :74 vfunc_2(p2,0x11) */
    if (vf(p2, 0x11) != 0) return 0;

    R1_39b140Fn f39b140 = (R1_39b140Fn)(g_dt_imgbase + 0x39b140);     /* :78 */
    if (f39b140(p2) != 0 &&
        *(int8_t *)(*(int64_t *)(owner + 0x298) + 0xa4) == 0) return 0;

    int64_t order_rec = *(int64_t *)(owner + 0x100);                  /* :83 local_238 source */
    if (order_rec != 0) {
        if (*(int32_t *)(order_rec + 0x394) > 0) return 0;            /* :85 */
        if (*(int32_t *)(order_rec + 0x394) < 0 &&
            *(int32_t *)(owner_type + 0x48) != 10) return 0;          /* :88 */
    }

    R1_540140Fn f540140 = (R1_540140Fn)(g_dt_imgbase + 0x540140);     /* :93 */
    if (f540140(owner_type, p2) == 1) return 0;

    int64_t b15418 = *(int64_t *)(g_dt_imgbase + 0xb15418ULL);       /* DAT_140b15418 shroud/sensor mgr */
    R1_35f470Fn f35f470 = (R1_35f470Fn)(g_dt_imgbase + 0x35f470);     /* :97 fog (idempotent rebuild) */
    if (f35f470(b15418, *(int32_t *)(owner + 0x58), p2, 1) == 1) return 0;

    if ((*(int8_t *)(owner + 0x3b4) == 1 ||                           /* :102-105 */
         *(int8_t *)((int64_t)p2 + 0x3b4) == 1)) {
        R1_39a540Fn f39a540 = (R1_39a540Fn)(g_dt_imgbase + 0x39a540);
        if (f39a540(owner, p2) == 0) return 0;
    }
    return 1;                                                          /* reached :106 — early gates pass */
}

/* R1 gate-2a (3825b0:107-163): param_3 resolve + targeting gates, ending at the 159-163 compound that
 * decides whether the binary enters the aim/fire body. Assumes gate-1 passed. Returns 1 if the firer
 * would enter the aim body (continues toward firing), 0 if it bails. RNG-free. The dead-in-tactical arc
 * gate (owner_type+0x4e==1, §I3) is treated as assume-PASS: if it executed and blocked, the binary would
 * not fire, so the one-directional oracle (r==1 ⇒ verdict==1) is unaffected; skipping its buffer-passing
 * dot-product avoids a crash-risk transcription on a path that is dead in the tactical sim. */
typedef uint32_t* (*R1_398440Fn)(int64_t, uint32_t*);          /* FUN_140398440(owner, &buf12) */
typedef int64_t   (*R1_394a80Fn)(int64_t*, void*, char, char); /* FUN_140394a80(p2, buf, 0, 0) */
typedef int64_t   (*R1_397e00Fn)(int64_t, int64_t, int32_t);   /* FUN_140397e00(owner, p2, sub_id) */
typedef char      (*R1_39b950Fn)(int64_t, int32_t, char);      /* FUN_14039b950(rec, sel, 1) */

static int pfire_r1_gate2a(int64_t p1, int64_t *p2, int64_t p3) {
    int64_t owner      = *(int64_t *)(p1 + 0x10);
    int64_t owner_type = *(int64_t *)(p1 + 0x20);

    if (p3 == 0) {                                                     /* :107-109 resolve param_3 */
        uint32_t buf12[4] = {0,0,0,0};                                 /* local_214 (12B; 16 for slack) */
        R1_398440Fn f398440 = (R1_398440Fn)(g_dt_imgbase + 0x398440);
        R1_394a80Fn f394a80 = (R1_394a80Fn)(g_dt_imgbase + 0x394a80);
        uint32_t *u8 = f398440(owner, buf12);
        p3 = f394a80(p2, u8, 0, 0);
    }
    /* :112 arc gate — DEAD in tactical (+0x4e==0); assume-pass (see header). */

    int64_t order_rec = *(int64_t *)(owner + 0x100);                  /* lVar7 from :83 == local_238 */
    int64_t lVar9  = *(int64_t *)(owner_type + 0x220);                /* :147 */
    int64_t lVar13 = *(int64_t *)(owner_type + 0x210);                /* :148 */
    if (lVar9 != 0 && order_rec != 0 && *(int32_t *)(order_rec + 0x394) < 0)
        lVar13 = lVar9;                                               /* :149-151 */
    int32_t sub_id = (p3 == 0) ? -1 : *(int32_t *)(p3 + 0x18);        /* :152-157 uVar16 */

    R1_397e00Fn f397e00 = (R1_397e00Fn)(g_dt_imgbase + 0x397e00);     /* :158 aim resolver */
    int64_t lv7 = f397e00(owner, (int64_t)p2, sub_id);
    /* :159-163 compound: continue iff lv7!=0 OR (fallback via owner+0x2b0 flags AND lVar13!=0). */
    if (lv7 != 0) return 1;
    int64_t b2b0 = *(int64_t *)(owner + 0x2b0);
    int fallback_ok;
    if (b2b0 == 0) {
        fallback_ok = 1;                                              /* lVar7==0 ⇒ left disjunct true */
    } else {
        R1_39b950Fn f39b950 = (R1_39b950Fn)(g_dt_imgbase + 0x39b950);
        fallback_ok = (f39b950(b2b0, 0x29, 1) == 0 &&
                       f39b950(*(int64_t *)(owner + 0x2b0), 0x22, 1) == 0);
    }
    if (fallback_ok && lVar13 != 0) return 1;                         /* enters aim body via lVar13 */
    return 0;                                                          /* compound false ⇒ binary returns 0 */
}

/* R1 gate-2b (3825b0:164-209): aim-point + LOS + range — the dominant per-tick fire/no-fire filter.
 * Assumes gate1+gate2a passed (reached :164). Returns 1 = reaches the spread/fire point (:210), 0 = bails
 * (no LOS or out of range), -1 = SKIP (the param_3==0 aim path uses 383f70 + the 405870 redirect, both of
 * which DRAW THE GLOBAL LCG — re-invoking them observe-only would perturb the sim's RNG stream; that path
 * is validated at the A3.3 takeover where the reimpl's draw replaces the binary's). The param_3!=0 path
 * (385c70 aim + 385e70 LOS + 3857d0/397780 range) is RNG-free + shared-write-free (verified) ⇒ safe to
 * re-invoke pre-trampoline. 385e70 writes param_2[0..2] (vec3) + param_3[0..0xb] (3x4) — buffers over-sized
 * to 64B; matrices init from DAT_140800820 (48B) mirroring :188-200. FP grouping matches :203-204
 * (dy² first, then +dx²). */
typedef void*   (*R1_385c70Fn)(int64_t, void*);                       /* FUN_140385c70(param_3, &vec) aim */
typedef char    (*R1_385e70Fn)(int64_t, float*, void*, void*);        /* FUN_140385e70(p1,&pos,&m1,&m2) LOS */
/* param_3==0 RNG-aim leaves (3825b0:164-181). Both 405870 + 383f70 draw the global LCG (1ffb40) but write
 * NO shared state (405870 reads-only+returns a redirected target; 383f70 writes only its out-aim+flag args)
 * ⇒ observe-safe under the pf==2 LCG save/restore. 398440(p2,&scratch) at :165 is pure & its output is dead
 * (the subsequent 383f70/local_res8 don't read it) — reuse R1_398440Fn. The decomps confirm all 3. */
typedef int64_t (*R1_405870Fn)(int64_t);                              /* UnitAI redirect: 405870(vfunc16(p2)) → target ptr / 0 (1 LCG draw) */
typedef float*  (*R1_383f70Fn)(int64_t, float*, int64_t, int8_t*);   /* aim: 383f70(p1,&aim_out,target,&flag) → &aim_out (0/1 LCG draw) */
typedef float   (*R1_3857d0Fn)(int64_t);                              /* FUN_1403857d0(p1) max-range delta */
typedef float   (*R1_397780Fn)(int64_t*);                             /* FUN_140397780(p2) target extent */
typedef float   (*R1_SqrtFn)(float);                                  /* FUN_140776d48 engine sqrt */
static uint32_t g_r1_g3_ok=0, g_r1_g3_bug=0, g_r1_g3_nofire=0, g_r1_g3_skip=0;

/* R1c geometry (3825b0:210-260): spread → lead-solve (399450) → fire gate (lead!=0 && 383ba0) → dispersion
 * (381dc0) → dir→Euler (20acd0), producing the create's pos/orient. RNG-TAINTED (spread draws ffbb0/ffdb0;
 * 381dc0 draws too) ⇒ NOT observe-validatable — transcription validated at the A3.3 takeover (the reimpl's
 * draws replace the binary's). 399450 uses the §8.12 locked global scratch DAT_140b2c380 (1-shard fine;
 * N-shard wraps it in the lock). ⚠️ ABI: 381dc0's 3rd arg (dist) is float-BY-VALUE (XMM) — typed float here
 * (the all-int64 passthrough B381dc0Fn would mis-pass it). ⚠️ STACK ADJACENCY: the create takes &local_2b8
 * (pos) and &local_230 (orient) as contiguous vec3s, so the binary's stack locals are mirrored in one float
 * S[] indexed by (0x2d8-rbp_off)/4 — S[0..2]=local_2d8/2d4/2d0 (aim in), S[4..6]=local_2c8/2c4/2c0,
 * S[8..10]=local_2b8/uStack_2b4/local_2b0 (=create pos), S[12..14]=local_2a8/2a4/2a0, S[42..44]=local_230/
 * 22c/228 (=create orient). Caller passes aim in S[0..2] + dist + the 385e70 matrices (mat1/mat2). Returns
 * 1 (fire → create with pos=&S[8], orient=&S[42]) / 0 (no fire). */
/* ⚠️ FUN_1401ffbb0 is `float ffbb0(uint *lcg, float lo, float hi)` → uniform random FLOAT in [lo,hi].
 * objdump (0x1ffbb0): `comiss xmm1,xmm2` (lo=XMM1, hi=XMM2 — FLOAT args, NOT GP regs) and the result is
 * returned in XMM0 (NOT RAX). Ghidra dropped the lo/hi args AND mistyped them as undefined8 GP regs; the
 * int64-typedef passed bounds in RDX/R8 (ignored) and read RAX (garbage) ⇒ XMM1/XMM2≈0 ⇒ (0-0)*r+0=0 ⇒
 * EVERY spread-weapon projectile spawned at the origin (confirmed by the pf==2 PFOBS observe: spread=1
 * fires all obs=0,0,0 with mat1==mat2==real). The bounds are the two muzzle-matrix translation columns
 * (mat1[k],mat2[k]) — the spread cone. */
typedef float (*R1_ffbb0Fn)(uint32_t*, float, float);     /* (LCG, lo, hi) → uniform float in [lo,hi] (XMM0) */
typedef char    (*R1_ffdb0Fn)(uint32_t*, int32_t);        /* FUN_1401ffdb0(&LCG, pct) prob roll */
/* ⚠️ 370f00 is Ghidra-typed `void`/undefined4 but actually RETURNS A FLOAT (the projectile speed) in XMM0 —
 * 3825b0:230 `uVar16 = 370f00(...)` then feeds 399450's param_6, which 399450 uses as a FLOAT multiplier
 * (`local_104 * param_6`, 399450:214). Transcribing it as int passes garbage (leftover RAX) into a float arg
 * ⇒ lead = x*garbage_as_0 = 0 ⇒ the reimpl never fires. Must capture+pass it as float. */
typedef float   (*R1_370f00Fn)(int64_t, int64_t);         /* FUN_140370f00(lVar7,0) → projectile speed (float) */
typedef float*  (*R1_399450Fn)(int64_t, float*, int64_t, float*, float*, float); /* lead solver; param_6=speed (float) */
typedef char    (*R1_383ba0Fn)(int64_t, float*);          /* FUN_140383ba0 fire-gate predicate */
typedef int64_t (*R1_3973b0Fn)(int64_t*);                 /* FUN_1403973b0 type-record (error path) */
typedef float*  (*R1_381dc0Fn)(int64_t, float*, float, float*, int64_t, int32_t); /* dispersion (float arg3) */
typedef void*   (*R1_20acd0Fn)(void*, float*, float*);    /* FUN_14020acd0 dir→Euler (DTARG2-lifted) */

/* A4.1 buffered spawn command: everything R2a (create+init) + R2b (Class-2b) need to APPLY one shot at the
 * canonical drain, captured by value in Phase-A. owner/owner_type/local_238 are re-derived from p1 at drain
 * time; rank/objid carry the DTDRAIN canonical key (g_drain_cur_* of the firing a76b0, set during replay). */
typedef struct { int64_t p1, p2, p3, lVar7; float S[48]; uint32_t rank; int32_t objid; } PfireSpawnRec;
#define PFIRE_SPAWN_CAP 4096
static PfireSpawnRec g_pfire_spawn_buf[PFIRE_SPAWN_CAP];
static int g_pfire_spawn_count = 0;

/* append one shot's payload; returns 1 if buffered, 0 if the buffer is full (caller falls back to inline
 * create — never drop a shot). Captures the current firing a76b0's canonical (rank,objid). */
static int pfire_spawn_buf_append(int64_t p1, int64_t p2, int64_t p3, int64_t lVar7, const float *S) {
    if (g_pfire_spawn_count >= PFIRE_SPAWN_CAP) { g_pfire_spawn_overflow++; return 0; }
    PfireSpawnRec *r = &g_pfire_spawn_buf[g_pfire_spawn_count++];
    r->p1 = p1; r->p2 = p2; r->p3 = p3; r->lVar7 = lVar7;
    memcpy(r->S, S, sizeof r->S);
    r->rank = g_drain_cur_rank; r->objid = g_drain_cur_objid;
    return 1;
}

/* FUN_1401ffbb0(LCG, lo, hi) → uniform random float in [lo,hi]. lo/hi are FLOAT args (XMM1/XMM2), result
 * in XMM0 — call it directly through the float-typed pointer (no GP-reg bit-packing). */
static inline float pfire_ffbb0_range(R1_ffbb0Fn f, uint32_t *lcg, float lo, float hi) {
    return f(lcg, lo, hi);
}

static int pfire_r1c_geom(int64_t p1, int64_t *p2, int64_t lVar7, float *S, float dist,
                          float *mat1, float *mat2) {
    int64_t owner      = *(int64_t *)(p1 + 0x10);
    int64_t owner_type = *(int64_t *)(p1 + 0x20);
    uint32_t *LCG = (uint32_t *)(g_dt_imgbase + 0xa13e24);

    if (*(int8_t *)(owner_type + 0x4f) == 1) {                  /* :210 weapon has spread → 3 LCG draws */
        R1_ffbb0Fn fbb0 = (R1_ffbb0Fn)(g_dt_imgbase + 0x1ffbb0);
        /* spread cone: uniform random between the two muzzle matrices' translation columns (mat1[k],mat2[k]) */
        S[8] = pfire_ffbb0_range(fbb0, LCG, mat1[3],  mat2[3]);   /* :211 x */
        S[9] = pfire_ffbb0_range(fbb0, LCG, mat1[7],  mat2[7]);   /* :212 y */
        S[6] = pfire_ffbb0_range(fbb0, LCG, mat1[11], mat2[11]);  /* :213 z */
    } else {
        R1_ffdb0Fn fdb0 = (R1_ffdb0Fn)(g_dt_imgbase + 0x1ffdb0);
        if (*(int32_t *)(p1 + 0x3c) < 0 || fdb0(LCG, 0x32) != 0) {   /* :215-219 use mat1 col */
            S[8] = mat1[3]; S[6] = mat1[11]; S[9] = mat1[7];
        } else {                                                     /* :222-224 use mat2 col */
            S[8] = mat2[3]; S[6] = mat2[11]; S[9] = mat2[7];
        }
    }
    S[4] = S[8]; S[5] = S[9];        /* :226 local_2c8 = (local_2b8,uStack_2b4) */
    S[12] = S[0]; S[13] = S[1];      /* :227 local_2a8 = (local_2d8,fStack_2d4) [aim x,y] */
    S[14] = S[2];                    /* :228 local_2a0 = local_2d0 [aim z] */
    S[10] = S[6];                    /* :229 local_2b0 = local_2c0 */

    float buf220[4] = {0,0,0,0};                                 /* local_220 scratch for 399450/381dc0 */
    R1_370f00Fn f370f00 = (R1_370f00Fn)(g_dt_imgbase + 0x370f00);
    float fSpeed = f370f00(lVar7, 0);                            /* :230 projectile speed (float, XMM0) */
    R1_399450Fn f399450 = (R1_399450Fn)(g_dt_imgbase + 0x399450);
    float *pf = f399450(owner, buf220, (int64_t)p2, &S[12], &S[4], fSpeed);  /* :231 lead solver (param_6=speed) */
    float lx = pf[0], ly = pf[1], lz = pf[2];                    /* :233-235 */
    { static int dbg = 0; if (dbg < 8) { dbg++; char b[256];
        snprintf(b, sizeof b, "[eaw-mt] PFLEAD spd=%.3f aim=%.2f,%.2f,%.2f dir=%.2f,%.2f,%.2f lead=%.4f,%.4f,%.4f\n",
                 fSpeed, S[12],S[13],S[14], S[4],S[5],S[6], lx,ly,lz);
        log_write(b); } }
    if (!(ly*ly + lx*lx + lz*lz != 0.0f)) { g_pfdbg_r1c_lead0++; return 0; }   /* :236 no lead solution */
    S[0] = lx; S[1] = ly; S[2] = lz;                            /* :237 local_2d8/2d4/2d0 = lead */
    S[16] = lx; S[17] = ly; S[18] = lz;  /* preserve the ORIGINAL lead (fVar14/17/18) for R2b guided delta */
    R1_383ba0Fn f383ba0 = (R1_383ba0Fn)(g_dt_imgbase + 0x383ba0);
    if (f383ba0(p1, &S[0]) == 0) { g_pfdbg_r1c_aimgate++; return 0; }   /* :238 fire-gate predicate fail */

    int64_t lVar9 = *(int64_t *)((int64_t)p2 + 0x53 * 8);      /* :239 p2[0x53] type-def */
    if (*(int64_t *)(lVar9 + 0x1648) == 0) {                    /* :240 missing XML category mask */
        R1_3973b0Fn f3973b0 = (R1_3973b0Fn)(g_dt_imgbase + 0x3973b0);
        f3973b0(p2);                                            /* :241 (error log omitted — cosmetic) */
        lVar9 = *(int64_t *)((int64_t)p2 + 0x53 * 8);          /* :248 re-read */
    }
    int64_t mask = *(int64_t *)(lVar9 + 0x1648);

    S[4] = S[0]; S[5] = S[1];        /* :250 local_2c8 = (lead.x, lead.y) */
    S[6] = S[2];                     /* :251 local_2c0 = lead.z */
    int32_t uVar16 = *(int32_t *)(lVar7 + 0x1fe8);              /* :252 guided flag (genuine int32) */
    R1_381dc0Fn f381dc0 = (R1_381dc0Fn)(g_dt_imgbase + 0x381dc0);
    pf = f381dc0(p1, buf220, dist, &S[4], mask, uVar16);        /* :253 dispersion (dist = float arg3) */
    S[0] = pf[0]; S[1] = pf[1]; S[2] = pf[2];                   /* :255-257 local_2d8.. = dispersed dir */

    R1_20acd0Fn f20acd0 = (R1_20acd0Fn)(g_dt_imgbase + 0x20acd0);
    f20acd0(&S[4], &S[8], &S[0]);    /* :258 20acd0(&local_2c8,&local_2b8,&local_2d8) → Euler in local_2c8 */
    S[44] = S[6];                    /* :259 local_228 = local_2c0 */
    S[42] = S[4]; S[43] = S[5];      /* :260 local_230 = local_2c8 → create orient */
    return 1;                        /* fire: create pos=&S[8], orient=&S[42] */
}

/* R2a applier — create + scalar init (3825b0:261-328): 294bc0 type-index lookup → 29f810 create →
 * write the proj+0xe8 (=plVar12[0x1d]) record: firer id / damage (+charge) / lifetime / velocity / target /
 * vis-frame. This is the DTWA-B3-validated §3 field map, now as the actual WRITE path (the takeover applier).
 * Transcription-only (creates a real object ⇒ runs only at the A3.3 takeover, validated by DTWA-B3). Inputs:
 * lVar7 = weapon template, local_238 = charge order-record, S[] = R1c geometry (create pos=&S[8],
 * orient=&S[42]; S[2]=disp.z, S[10]=spread.z for the +0x6c velocity term). Returns the created projectile.
 * ⚠️ The muzzle-SFX block (:272-284, Class-3 presentation feeding the render handle rec+0xb0) is DEFERRED
 * (local_res20=0 ⇒ rec+0xb0=0); add it back post-validation (or route via the a1 SFX buffer). */
typedef int64_t (*R2_294bc0Fn)(int64_t, int32_t);     /* registry lookup → type record */
typedef int32_t (*R2_5400f0Fn)(int64_t);              /* lifetime (caches at owner_type+0x4a0) */
typedef int64_t (*R2_3952a0Fn)(int64_t, int32_t);     /* charge damage-scale record */
typedef int64_t (*R2_39b1a0Fn)(int64_t*);             /* sub-extent applicability */
typedef char    (*R2_372210Fn)(int64_t);              /* sub-extent suppressor */
typedef void    (*R2_39d6a0Fn)(int64_t);              /* post-create vis register */

static int64_t pfire_r2a_create_init(int64_t p1, int64_t *p2, int64_t lVar7, int64_t local_238, float *S) {
    int64_t owner      = *(int64_t *)(p1 + 0x10);
    int64_t owner_type = *(int64_t *)(p1 + 0x20);

    R2_294bc0Fn f294bc0 = (R2_294bc0Fn)(g_dt_imgbase + 0x294bc0);
    int64_t treg = f294bc0((int64_t)(g_dt_imgbase + 0xa16fd0), *(int32_t *)(owner + 0x58));  /* :261 */
    int32_t type_index = *(int32_t *)(treg + 0x4c);
    int64_t mgr = *(int64_t *)(owner + 0x2b8);
    EngCreateObjFn f29f810 = (EngCreateObjFn)(g_dt_imgbase + 0x29f810);
    int64_t proj = f29f810(mgr, lVar7, type_index, &S[8], (int64_t)&S[42], 1, 0);  /* :265 create */
    /* :272-284 muzzle SFX (Class-3) DEFERRED → local_res20 = 0. */

    int64_t rec = *(int64_t *)(proj + 0xe8);                          /* :286 plVar12[0x1d] */
    *(int32_t *)(rec + 0x58) = *(int32_t *)(owner + 0x50);            /* :287 firer id */
    *(int32_t *)(rec + 0x5c) = 0;                                     /* :288 */

    float dmg = *(float *)(owner_type + 0x478);                       /* :289-292 damage */
    if (dmg <= 0.0f) dmg = *(float *)(lVar7 + 0x474);
    *(float *)(rec + 0x64) = dmg;
    if (local_238 != 0 && *(int32_t *)(local_238 + 0x394) < 0) {      /* :293-298 charge scale */
        *(int32_t *)(local_238 + 0x394) += 1;
        R2_3952a0Fn f3952a0 = (R2_3952a0Fn)(g_dt_imgbase + 0x3952a0);
        int64_t cr = f3952a0(owner, 0x46);
        if (cr != 0) *(float *)(rec + 0x64) = *(float *)(cr + 0x17c) * *(float *)(rec + 0x64);
    }

    R2_5400f0Fn f5400f0 = (R2_5400f0Fn)(g_dt_imgbase + 0x5400f0);     /* :300-305 lifetime */
    int32_t life = f5400f0(owner_type);
    *(int32_t *)(rec + 0x68) = (life == 0) ? *(int32_t *)(lVar7 + 0x440) : f5400f0(owner_type);

    R1_3857d0Fn f3857d0 = (R1_3857d0Fn)(g_dt_imgbase + 0x3857d0);     /* :306-309 velocity base */
    R1_397780Fn f397780 = (R1_397780Fn)(g_dt_imgbase + 0x397780);
    float spd = f3857d0(p1);
    if (spd <= 0.0f) spd = *(float *)(lVar7 + 0x4bc);
    *(float *)(rec + 0x6c) = spd;
    *(float *)(rec + 0x6c) = f397780(p2) + *(float *)(rec + 0x6c);    /* :310-311 +extent (addend LEFT) */
    R2_39b1a0Fn f39b1a0 = (R2_39b1a0Fn)(g_dt_imgbase + 0x39b1a0);     /* :312-316 +sub-extent */
    R2_372210Fn f372210 = (R2_372210Fn)(g_dt_imgbase + 0x372210);
    if (f39b1a0(p2) != 0 && f372210(*(int64_t *)(owner + 0x298)) == 0)
        *(float *)(rec + 0x6c) = f397780((int64_t *)p2[0x56]) + *(float *)(rec + 0x6c);
    *(int64_t *)(rec + 0x08) = (int64_t)p2;                           /* :318 target */
    { float d = S[2] - S[10]; uint32_t u; memcpy(&u,&d,4); u &= 0x7fffffffu; memcpy(&d,&u,4); /* :320 ABS */
      *(float *)(rec + 0x6c) = d + *(float *)(rec + 0x6c); }

    int32_t vo = *(int32_t *)(owner_type + 0x4a4);                    /* :321-328 vis frame */
    if (vo > 0) {
        int64_t b15418 = *(int64_t *)(g_dt_imgbase + 0xb15418ULL);
        int32_t base = (b15418 == 0) ? 0 : *(int32_t *)(b15418 + 0x10);
        *(int32_t *)(rec + 0x60) = base + vo;
        R2_39d6a0Fn f39d6a0 = (R2_39d6a0Fn)(g_dt_imgbase + 0x39d6a0);
        f39d6a0(proj);
        *(int64_t *)(rec + 0xb0) = 0;                                 /* local_res20 (SFX deferred) */
    }
    return proj;
}

/* R2b applier — Class-2b emits + guided/ballistic transform (3825b0:342-402). After R2a created+scalar-
 * init'd the projectile: (1) 220e90 = register the projectile as a listener on the target's +0x38 list
 * (Class-2b); (2) target sub-id; (3) guided (lVar7+0x1fe8==1): velocity = target-frame matrix (22d410 of
 * the target's orientation) × (dispersed_dir − original_lead) [needs R1c's lead at S[16..18]]; ballistic:
 * read back the just-created projectile's transform fields (plVar12+0x7c/0x84/0x8c/[0xf]/[0x10]/[0x11]) —
 * which is WHY create-then-init is mandatory — + store the dispersed dir; (4) 3a06a0 = shot-register
 * (firer record + shroud mgr, Class-2b). Transcription-only (A3.3-validated). rec = proj+0xe8. */
typedef int64_t (*R2_058570Fn)(void);                 /* listener-key source */
typedef void    (*R2_220e90Fn)(int64_t, int64_t, int64_t, int32_t);  /* Class-2b listener register */
typedef void    (*R2_22d410Fn)(float*, float*);       /* target-frame matrix transform */
typedef void    (*R2_3a06a0Fn)(int64_t, int64_t*, int64_t);          /* Class-2b shot-register */

static void pfire_r2b_emit(int64_t p1, int64_t proj, int64_t *p2, int64_t p3, int64_t lVar7, float *S) {
    int64_t owner = *(int64_t *)(p1 + 0x10);
    int64_t rec   = *(int64_t *)(proj + 0xe8);

    R1_VfuncFn vf = *(R1_VfuncFn *)(*(int64_t *)proj + 0x10);         /* :342 vfunc_2(proj, 8) */
    int64_t lv13 = vf((int64_t *)proj, 8);
    int64_t lkey = (lv13 == 0) ? 0 : lv13 + 0x18;                     /* :343-348 local_res8 */
    R2_058570Fn f058570 = (R2_058570Fn)(g_dt_imgbase + 0x058570);
    R2_220e90Fn f220e90 = (R2_220e90Fn)(g_dt_imgbase + 0x220e90);
    f220e90(f058570(), (int64_t)p2 + 0x38, lkey, 0x28);              /* :349-350 Class-2b listener */
    *(int32_t *)(rec + 0x5c) = 0;                                     /* :351 */
    int32_t sub = (p3 == 0) ? (int32_t)0xffffffff : *(int32_t *)(p3 + 0x18);  /* :352-358 */
    *(int32_t *)(rec + 0x10) = sub;

    if (*(int32_t *)(lVar7 + 0x1fe8) == 1) {                          /* :359 guided */
        int64_t tgt = *(int64_t *)(rec + 8);                          /* :360 */
        *(int64_t *)(rec + 0x18) = tgt;                              /* :361 */
        *(int32_t *)(rec + 0x20) = sub;                             /* :362 */
        float fv14 = S[0] - S[16], fv17 = S[1] - S[17], fv18 = S[2] - S[18];  /* :363-365 disp − lead */
        if (tgt != 0) {
            float in_mat[12], out_mat[12];                            /* :367-372 target frame */
            memcpy(in_mat, (void *)(tgt + 0x248), 48);
            R2_22d410Fn f22d410 = (R2_22d410Fn)(g_dt_imgbase + 0x22d410);
            f22d410(in_mat, out_mat);                                 /* :373 */
            *(float *)(rec + 0x24) = out_mat[0]*fv14 + out_mat[1]*fv17 + out_mat[2]*fv18;   /* :374 */
            *(float *)(rec + 0x28) = out_mat[4]*fv14 + out_mat[5]*fv17 + out_mat[6]*fv18;   /* :377 */
            *(float *)(rec + 0x2c) = out_mat[8]*fv14 + out_mat[9]*fv17 + out_mat[10]*fv18;  /* :380 */
        }
    } else {                                                          /* :385 ballistic (read-back create) */
        *(int32_t *)(rec + 0x74) = *(int32_t *)(proj + 0x84);
        *(int32_t *)(rec + 0x78) = (int32_t)*(int64_t *)(proj + 0x11 * 8);
        *(int32_t *)(rec + 0x7c) = *(int32_t *)(proj + 0x8c);
        *(int32_t *)(rec + 0x80) = (int32_t)*(int64_t *)(proj + 0xf * 8);
        *(int32_t *)(rec + 0x84) = *(int32_t *)(proj + 0x7c);
        *(int32_t *)(rec + 0x88) = (int32_t)*(int64_t *)(proj + 0x10 * 8);
        *(int32_t *)(rec + 0x8c) = *(int32_t *)(*(int64_t *)(*(int64_t *)(proj + 0x57*8) + 0x20) + 0x10);
        R1_370f00Fn f370f00 = (R1_370f00Fn)(g_dt_imgbase + 0x370f00);
        *(float *)(rec + 0x90) = f370f00(lVar7, 0);                 /* :394-395 speed (float, not int) */
        *(int64_t *)(rec + 0x18) = 0;                                /* :396 */
        *(int32_t *)(rec + 0x20) = (int32_t)0xffffffff;             /* :397 */
        *(float *)(rec + 0x3c) = S[0];                              /* :398-400 dispersed dir */
        *(float *)(rec + 0x40) = S[1];
        *(float *)(rec + 0x44) = S[2];
    }
    R2_3a06a0Fn f3a06a0 = (R2_3a06a0Fn)(g_dt_imgbase + 0x3a06a0);
    f3a06a0(owner, p2, proj);                                        /* :402 Class-2b shot-register */
    { static int dbg = 0; if (dbg < 8) { dbg++; char b[256];
        snprintf(b, sizeof b, "[eaw-mt] PFPOS proj@%.1f,%.1f,%.1f  firer@%.1f,%.1f,%.1f  rec0xb0=%llx guided=%d\n",
                 *(float*)(proj+0x78), *(float*)(proj+0x7c), *(float*)(proj+0x80),
                 *(float*)(owner+0x78), *(float*)(owner+0x7c), *(float*)(owner+0x80),
                 (unsigned long long)*(int64_t*)(rec+0xb0), (*(int32_t*)(lVar7+0x1fe8)==1));
        log_write(b); } }
}

/* R3 cooldown (3825b0:403-497): recharge-timer/burst-counter recompute (param_1+0x58/0x5c/0x68, own-object
 * → Phase-A-safe) + R3b listener rebind (param_1+0xa8, Class-2b). Two branches: burst-EXHAUSTED (iVar3==0,
 * draws the LCG :410) recompute; intra-burst (else) simpler. Both end with a damage-table scaling
 * (374b50/39b950/398010) + a container-reduce: 535cb0 builds a multi-bucket container in cont[] (header on
 * stack + heap nodes), 395c70 populates from the shroud mgr, 33fb70 = sum-of-per-bucket-maxima (Plus across,
 * GreaterThan within — vtables 0x85ae90/0x85ef60 via objdump), 535fb0 destroys. local_148 = local_208+0xc0
 * (confirmed rbp+0x10 vs +0xd0). RNG-RETROFIT (draws LCG) ⇒ transcription-only, A3.3-validated (behavior).
 * The :418 deregister's 294bc0 takes the firer's registry slot param_1+0xc8 (Ghidra dropped that 2nd arg).
 * Quirks transcribed: uVar16 shroud default (b15418==0, rare) → 0; branch2 :488 stale-iVar6 edge (fVar15<=0)
 * → last 374b50 idx. */
typedef int32_t (*R3_ffb40Fn)(uint32_t*, int32_t, int32_t);  /* FUN_1401ffb40(&LCG, lo, hi) */
typedef void    (*R3_285d70Fn)(int64_t, int64_t);            /* registry deregister */
typedef int32_t (*R3_374b50Fn)(int64_t, int32_t);           /* damage-table index */
typedef float   (*R3_398010Fn)(int64_t, int32_t);           /* difficulty/heat multiplier */
typedef void    (*R3_535cb0Fn)(void*);                       /* container ctor */
typedef void    (*R3_395c70Fn)(int64_t, void*);             /* container populate */
typedef float   (*R3_33fb70Fn)(void*, void*, void*);        /* reduce: Σ per-bucket max */
typedef void    (*R3_535fb0Fn)(void*);                       /* container dtor */
typedef void    (*R3_3846c0Fn)(int64_t, int64_t*);          /* listener deregister */
typedef void    (*R3_382510Fn)(int64_t, int64_t, int64_t*); /* listener register */

/* shared damage-table scaling (3825b0:423-437 == 461-472); returns fVar15, *last_idx = 2nd 374b50 result. */
static float pfire_r3_dmgscale(int64_t owner, int64_t owner_type, float fVar1, int32_t *last_idx) {
    R3_374b50Fn f374b50 = (R3_374b50Fn)(g_dt_imgbase + 0x374b50);
    R1_39b950Fn f39b950 = (R1_39b950Fn)(g_dt_imgbase + 0x39b950);
    R3_398010Fn f398010 = (R3_398010Fn)(g_dt_imgbase + 0x398010);
    int64_t tdef = *(int64_t *)(owner + 0x298);
    int32_t i0 = f374b50(tdef, 0);
    float fVar15 = fVar1;
    if (f39b950(owner, i0, 1) != 0) fVar15 = *(float *)(owner_type + 0x250 + (int64_t)i0 * 4);
    fVar15 = fVar15 * f398010(owner, 1);
    int32_t i1 = f374b50(tdef, 1);
    if (f39b950(owner, i1, 1) != 0) fVar15 = fVar15 * *(float *)(owner_type + 0x250 + (int64_t)i1 * 4);
    *last_idx = i1;
    return fVar15;
}

static void pfire_r3_cooldown(int64_t p1, int64_t *p2) {
    int64_t owner      = *(int64_t *)(p1 + 0x10);
    int64_t owner_type = *(int64_t *)(p1 + 0x20);
    float fVar1 = *(float *)(g_dt_imgbase + 0x7ffaf8);          /* DAT_1407ffaf8 */
    float c7c0  = *(float *)(g_dt_imgbase + 0x8007c0);          /* DAT_1408007c0 */
    float b0a   = (float)*(int32_t *)(g_dt_imgbase + 0xb0a340); /* DAT_140b0a340 (int32 hz) */
    int64_t gt_v = g_dt_imgbase + 0x85ef60, pl_v = g_dt_imgbase + 0x85ae90;  /* GreaterThan / Plus vtables */
    R3_535cb0Fn f535cb0 = (R3_535cb0Fn)(g_dt_imgbase + 0x535cb0);
    R3_395c70Fn f395c70 = (R3_395c70Fn)(g_dt_imgbase + 0x395c70);
    R3_33fb70Fn f33fb70 = (R3_33fb70Fn)(g_dt_imgbase + 0x33fb70);
    char cont[512];
    int32_t last_idx = 0;

    int32_t iVar3 = *(int32_t *)(p1 + 0x5c);                    /* :403 burst counter */
    if (iVar3 != 0) { iVar3 -= 1; *(int32_t *)(p1 + 0x5c) = iVar3; }

    if (iVar3 == 0) {                                            /* :409 burst-exhausted recompute */
        R3_ffb40Fn fffb40 = (R3_ffb40Fn)(g_dt_imgbase + 0x1ffb40);
        float m = *(float *)(g_dt_imgbase + 0x8007f0);          /* DAT_1408007f0 */
        int32_t r = fffb40((uint32_t *)(g_dt_imgbase + 0xa13e24),               /* :410 LCG */
                           (int32_t)(*(float *)(owner_type + 0x228) * m),
                           (int32_t)(*(float *)(owner_type + 0x22c) * m));
        { float f = (float)r * b0a; int64_t ll = (int64_t)f; ll &= 0xffffffffLL;
          *(int32_t *)(p1 + 0x58) = (int32_t)(ll / 100); }                      /* :412 */
        int64_t b15418 = *(int64_t *)(g_dt_imgbase + 0xb15418ULL);
        *(int32_t *)(p1 + 0x68) = (b15418 != 0) ? *(int32_t *)(b15418 + 0x10) : 0; /* :414-417 (def 0) */
        if (*(int32_t *)(p1 + 0xc8) != -1) {                                    /* :418 deregister */
            R2_294bc0Fn f294bc0 = (R2_294bc0Fn)(g_dt_imgbase + 0x294bc0);
            int64_t e = f294bc0((int64_t)(g_dt_imgbase + 0xa16fd0), *(int32_t *)(p1 + 0xc8));
            if (e != 0) { R3_285d70Fn f285d70 = (R3_285d70Fn)(g_dt_imgbase + 0x285d70);
                          f285d70(e, p1); *(int32_t *)(p1 + 0xc8) = -1; }
        }
        float fVar15 = pfire_r3_dmgscale(owner, owner_type, fVar1, &last_idx);  /* :423-437 */
        if (fVar15 < fVar1)                                                     /* :438-440 */
            *(int32_t *)(p1 + 0x58) = (int32_t)((float)*(uint32_t *)(p1 + 0x58) * fVar15 + c7c0);
        fVar15 = ((R3_398010Fn)(g_dt_imgbase + 0x398010))(owner, 9);           /* :442 */
        f535cb0(cont); f395c70(owner, cont);                                   /* :443-444 */
        int64_t gth = gt_v, plh = pl_v;
        float fVar17 = f33fb70(cont + 0xc0, &plh, &gth);                       /* :447 */
        fVar15 = fVar15 * (fVar17 + fVar1);                                    /* :448 */
        if (0.0f < fVar15 && fVar15 < fVar1)                                   /* :449-451 */
            *(int32_t *)(p1 + 0x58) = (int32_t)((float)*(uint32_t *)(p1 + 0x58) / fVar15 + c7c0);
        if (fVar15 <= 0.0f) fVar15 = 0.0f;                                     /* :452-454 */
        *(int32_t *)(p1 + 0x5c) = (int32_t)((float)*(uint32_t *)(owner_type + 0x230) * fVar15 + c7c0);
    } else {                                                     /* :458 intra-burst */
        { float f = b0a * *(float *)(owner_type + 0x234); *(int32_t *)(p1 + 0x58) = (int32_t)(int64_t)f; } /* :459 */
        float fVar15 = pfire_r3_dmgscale(owner, owner_type, fVar1, &last_idx); /* :461-472 */
        int32_t iVar6 = last_idx;                                              /* :488 stale-iVar6 edge */
        *(int32_t *)(p1 + 0x58) = (int32_t)((float)*(uint32_t *)(p1 + 0x58) * fVar15 + c7c0); /* :475 */
        fVar15 = ((R3_398010Fn)(g_dt_imgbase + 0x398010))(owner, 9);          /* :477 */
        f535cb0(cont); f395c70(owner, cont);
        int64_t gth = gt_v, plh = pl_v;
        float fVar17 = f33fb70(cont + 0xc0, &plh, &gth);
        fVar15 = fVar15 * (fVar17 + fVar1);
        if (fVar15 <= 0.0f) *(int32_t *)(p1 + 0x5c) = 0;                       /* :484-488 */
        else iVar6 = (int32_t)((float)*(uint32_t *)(p1 + 0x58) / fVar15 + c7c0);
        *(int32_t *)(p1 + 0x58) = iVar6;
    }
    ((R3_535fb0Fn)(g_dt_imgbase + 0x535fb0))(cont);            /* :491 container dtor */

    if (*(int64_t *)(p1 + 0xa8) != (int64_t)p2) {              /* :492-496 R3b listener rebind (Class-2b) */
        if (*(int64_t *)(p1 + 0xa8) != 0)
            ((R3_3846c0Fn)(g_dt_imgbase + 0x3846c0))(p1, (int64_t *)(p1 + 0xa8));
        ((R3_382510Fn)(g_dt_imgbase + 0x382510))(p1, (int64_t)p2, (int64_t *)(p1 + 0xa8));
    }
}

static int pfire_r1_gate2b(int64_t p1, int64_t *p2, int64_t p3) {
    int64_t owner      = *(int64_t *)(p1 + 0x10);
    int64_t owner_type = *(int64_t *)(p1 + 0x20);

    if (p3 == 0) {                                                     /* re-resolve param_3 (:107-109) */
        uint32_t buf12[4] = {0,0,0,0};
        R1_398440Fn f398440 = (R1_398440Fn)(g_dt_imgbase + 0x398440);
        R1_394a80Fn f394a80 = (R1_394a80Fn)(g_dt_imgbase + 0x394a80);
        p3 = f394a80(p2, f398440(owner, buf12), 0, 0);
    }
    if (p3 == 0) return -1;       /* :164 binary takes the RNG 383f70/405870 aim path → SKIP */

    float fpos[16]; float aim[4] = {0,0,0,0};
    R1_385c70Fn f385c70 = (R1_385c70Fn)(g_dt_imgbase + 0x385c70);      /* :183 aim (param_3!=0 path) */
    float *pa = (float *)f385c70(p3, fpos);
    aim[0]=pa[0]; aim[1]=pa[1]; aim[2]=pa[2];

    float mat1[16], mat2[16];                                          /* :188-200 matrix init */
    memcpy(mat1, (void *)(g_dt_imgbase + 0x800820), 48);
    memcpy(mat2, (void *)(g_dt_imgbase + 0x800820), 48);
    memset(fpos, 0, sizeof fpos);                                      /* :192 local_2c8 = 0 */
    R1_385e70Fn f385e70 = (R1_385e70Fn)(g_dt_imgbase + 0x385e70);      /* :201 LOS + firing pos */
    if (f385e70(p1, fpos, mat1, mat2) == 0) return 0;                  /* :202 no LOS → no fire */

    R1_SqrtFn esqrt = (R1_SqrtFn)(g_dt_imgbase + 0x776d48);
    R1_3857d0Fn f3857d0 = (R1_3857d0Fn)(g_dt_imgbase + 0x3857d0);
    R1_397780Fn f397780 = (R1_397780Fn)(g_dt_imgbase + 0x397780);
    float dy = fpos[1] - aim[1];                                       /* :203 local_2c8.y - aim.y */
    float dx = fpos[0] - aim[0];                                       /* :204 local_2c8.x - aim.x */
    float dist = esqrt(dy*dy + dx*dx);
    float maxr = f3857d0(p1) + f397780(p2);                            /* :206-208 */
    float minr = *(float *)(owner_type + 0x23c);                       /* :209 */
    if (dist <= maxr && minr <= dist) return 1;                        /* in range → reaches :210 spread */
    return 0;
}

/* ── A3 ASSEMBLY (firing_body_lift_scope.md §8.14): the takeover reimplementation of FUN_1403825b0, split
 * into a SHARED geometry core (pfire_compute_geom = 3825b0:60-260) + the takeover applier (pfire_fire_reimpl)
 * + the position observe (pfire_observe_geom). SAFE-ROLLOUT: the reimpl handles only the COMMON tactical
 * path; rare/dead edges fall back to the stock binary (arc gate owner_type+0x4e==1; param_3==0 RNG aim path).
 * Each binary leaf is called exactly ONCE per fire (like the binary). p2 may be NULL-redirected only inside
 * the fallback path, so the reimpl's p2 stays the caller's. */
#define PFIRE_FALLBACK 2
/* SHARED geometry core (3825b0:60-260): R1 gates + gate2a + gate2b aim/LOS/range + R1c spread/lead/
 * dispersion/Euler. Fills S[48] (create pos=&S[8], orient=&S[42]) and outputs lVar7 (weapon template),
 * local_238 (charge order-record), the RESOLVED p3, and the RESOLVED p2 (the param_3==0 path may redirect
 * the target via 405870 — the lead solver + Class-2b listener + shot-register all use the redirected target).
 * Returns 1=fire (S valid) / 0=no-fire / PFIRE_FALLBACK. out_mat1col/out_mat2col (NULL-skippable) report the
 * two muzzle-matrix translation columns for the position observe. The param_3==0 RNG-aim path (405870/383f70)
 * draws the LCG but writes no shared state ⇒ the pf==2 observe replays it under the LCG save/restore. BOTH the
 * takeover (pfire_fire_reimpl) and the pf==2 observe (pfire_observe_geom) call this so the observed geometry
 * is the SAME code the takeover would run. */
static int pfire_compute_geom(int64_t p1, int64_t *p2arg, int64_t p3,
                              float *S, int64_t *out_lVar7, int64_t *out_local238, int64_t *out_p3,
                              int64_t *out_p2, float *out_mat1col, float *out_mat2col) {
    if (!g_dt_imgbase) return PFIRE_FALLBACK;
    g_pfdbg_entry++;
    int64_t p2 = (int64_t)p2arg;
    int64_t owner      = *(int64_t *)(p1 + 0x10);
    int64_t owner_type = *(int64_t *)(p1 + 0x20);

    /* ---- gate1 (60-105) ---- */
    if (owner == 0 || p2 == 0) return 0;
    if (p3 != 0 && *(int64_t *)(p3 + 0x10) != p2) return 0;
    if ((*(uint8_t *)(p2 + 0x74 * 8) & 0x40) != 0) return 0;
    R1_VfuncFn vf = *(R1_VfuncFn *)(*(int64_t *)p2 + 0x10);
    if (vf((int64_t *)p2, 0x11) != 0) return 0;
    if (((R1_39b140Fn)(g_dt_imgbase + 0x39b140))((int64_t *)p2) != 0 &&
        *(int8_t *)(*(int64_t *)(owner + 0x298) + 0xa4) == 0) return 0;
    int64_t local_238 = *(int64_t *)(owner + 0x100);
    if (local_238 != 0) {
        if (*(int32_t *)(local_238 + 0x394) > 0) return 0;
        if (*(int32_t *)(local_238 + 0x394) < 0 && *(int32_t *)(owner_type + 0x48) != 10) return 0;
    }
    if (((R1_540140Fn)(g_dt_imgbase + 0x540140))(owner_type, (int64_t *)p2) == 1) return 0;
    if (((R1_35f470Fn)(g_dt_imgbase + 0x35f470))(*(int64_t *)(g_dt_imgbase + 0xb15418ULL),
            *(int32_t *)(owner + 0x58), (int64_t *)p2, 1) == 1) return 0;
    if ((*(int8_t *)(owner + 0x3b4) == 1 || *(int8_t *)(p2 + 0x3b4) == 1) &&
        ((R1_39a540Fn)(g_dt_imgbase + 0x39a540))(owner, (int64_t *)p2) == 0) return 0;

    g_pfdbg_pass_g1++;                                                /* DEBUG: passed gate1 */
    /* ---- gate2a (107-163): resolve param_3, arc-gate fallback, compound → lVar7 ---- */
    if (p3 == 0) {
        uint32_t buf12[4] = {0,0,0,0};
        p3 = ((R1_394a80Fn)(g_dt_imgbase + 0x394a80))((int64_t *)p2,
                 ((R1_398440Fn)(g_dt_imgbase + 0x398440))(owner, buf12), 0, 0);
    }
    if (*(int8_t *)(owner_type + 0x4e) == 1) { g_pfdbg_fb_arc++; return PFIRE_FALLBACK; }  /* :112 arc gate → binary */
    int64_t lVar9_220 = *(int64_t *)(owner_type + 0x220);
    int64_t lVar13    = *(int64_t *)(owner_type + 0x210);
    if (lVar9_220 != 0 && local_238 != 0 && *(int32_t *)(local_238 + 0x394) < 0) lVar13 = lVar9_220;
    int32_t sub_id = (p3 == 0) ? -1 : *(int32_t *)(p3 + 0x18);
    int64_t lVar7 = ((R1_397e00Fn)(g_dt_imgbase + 0x397e00))(owner, p2, sub_id);
    if (lVar7 == 0) {                                                  /* :159-163 fallback disjunct */
        int64_t b2b0 = *(int64_t *)(owner + 0x2b0);
        int fb;
        if (b2b0 == 0) fb = 1;
        else { R1_39b950Fn f = (R1_39b950Fn)(g_dt_imgbase + 0x39b950);
               fb = (f(b2b0, 0x29, 1) == 0 && f(*(int64_t *)(owner + 0x2b0), 0x22, 1) == 0); }
        if (fb && lVar13 != 0) lVar7 = lVar13; else return 0;
    }

    g_pfdbg_pass_g2a++;                                              /* DEBUG: passed gate2a */
    /* ---- gate2b (164-209): aim → 385e70 LOS → range. Two aim paths converge here:
     *  p3!=0 (:183) 385c70(p3) ; p3==0 (:164-181) the RNG-aim path (398440 dead read + optional 405870 UnitAI
     *  target-redirect + 383f70 aim). A4 lift (was a fallback): the redirect REASSIGNS the target for the rest
     *  of the body (lead solver + Class-2b), so p2 is updated and exported via out_p2. */
    memset(S, 0, 48 * sizeof(float));
    float mat1[16], mat2[16];
    if (p3 != 0) {
        float *pa = (float *)((R1_385c70Fn)(g_dt_imgbase + 0x385c70))(p3, &S[4]);  /* :183 aim */
        S[0] = pa[0]; S[1] = pa[1]; S[2] = pa[2];
    } else {
        g_pfdbg_fb_p3++;                                             /* count: this fire took the RNG-aim path */
        float scratch3[4] = {0,0,0,0};                              /* :165 398440(p2,&buf) — pure, output dead */
        ((R1_398440Fn)(g_dt_imgbase + 0x398440))(p2, (uint32_t *)scratch3);
        int8_t flag = 0;                                            /* :166 local_res8 low byte cleared */
        if (*(int8_t *)(p2 + 0x348) != -1) {                        /* :167 param_2[0x69] low byte != -1 */
            R1_VfuncFn vf2 = *(R1_VfuncFn *)(*(int64_t *)p2 + 0x10);
            int64_t redir = ((R1_405870Fn)(g_dt_imgbase + 0x405870))(vf2((int64_t *)p2, 0x16));  /* :168-169 */
            if (redir == 0) return 0;                               /* :170-172 */
            p2 = redir;                                             /* redirect propagates to lead/Class-2b */
        }
        float *pa = ((R1_383f70Fn)(g_dt_imgbase + 0x383f70))(p1, &S[4], p2, &flag);  /* :174 aim (writes &S[4]) */
        S[0] = pa[0]; S[1] = pa[1]; S[2] = pa[2];                   /* :175-177 local_2d8 = aim */
        if (flag == 0) return 0;                                    /* :178 aim-fail gate */
    }
    memcpy(mat1, (void *)(g_dt_imgbase + 0x800820), 48);              /* :188-200 */
    memcpy(mat2, (void *)(g_dt_imgbase + 0x800820), 48);
    S[4] = S[5] = S[6] = 0.0f;                                        /* :192 local_2c8 = 0 */
    if (((R1_385e70Fn)(g_dt_imgbase + 0x385e70))(p1, &S[4], mat1, mat2) == 0) return 0;  /* :201 LOS */
    /* observe: the muzzle-matrix translation columns 385e70 just filled (mat2 stays the identity init when
     * p1+0x3c<0 — single-bone — since 385e70's branch A leaves param_4 untouched). */
    if (out_mat1col) { out_mat1col[0]=mat1[3]; out_mat1col[1]=mat1[7]; out_mat1col[2]=mat1[11]; }
    if (out_mat2col) { out_mat2col[0]=mat2[3]; out_mat2col[1]=mat2[7]; out_mat2col[2]=mat2[11]; }
    R1_SqrtFn esqrt = (R1_SqrtFn)(g_dt_imgbase + 0x776d48);
    float dy = S[5] - S[1], dx = S[4] - S[0];                         /* :203-204 */
    float dist = esqrt(dy*dy + dx*dx);
    float maxr = ((R1_3857d0Fn)(g_dt_imgbase + 0x3857d0))(p1)
               + ((R1_397780Fn)(g_dt_imgbase + 0x397780))((int64_t *)p2);
    if (!(dist <= maxr && *(float *)(owner_type + 0x23c) <= dist)) return 0;  /* :208-209 range */

    g_pfdbg_pass_g2b++;                                              /* DEBUG: passed gate2b (aim/LOS/range) */
    /* ---- R1c geometry (210-260): spread/lead/fire-gate/dispersion/Euler ---- */
    if (pfire_r1c_geom(p1, (int64_t *)p2, lVar7, S, dist, mat1, mat2) == 0) { g_pfdbg_r1cfail++; return 0; }  /* fire gate */

    *out_lVar7 = lVar7; *out_local238 = local_238; if (out_p3) *out_p3 = p3;   /* p3 resolved in gate2a */
    if (out_p2) *out_p2 = p2;                                          /* p2 maybe redirected (405870) */
    return 1;
}

/* ── A3 ASSEMBLY: the unified takeover reimplementation of FUN_1403825b0 (firing_body_lift_scope.md §8.14).
 * Mirrors 3825b0:60-497 (gates+geometry via the shared pfire_compute_geom → R2a/R2b applier → R3 cooldown).
 * Returns 0 = no fire, 1 = fired, PFIRE_FALLBACK(2) = "I didn't handle this — run the binary":
 *   - arc gate owner_type+0x4e==1 (DEAD in tactical per I3) → fallback;
 *   - param_3==0 after resolution (the 383f70/405870 RNG aim path, never hit tactically) → fallback.
 * RNG-tainted from R1c on ⇒ validated at the takeover by structural invariants + fire cadence + the pf==2
 * position observe (pfire_observe_geom), NOT bit-exact at takeover (§8.15). */
static int pfire_fire_reimpl(int64_t p1, int64_t *p2arg, int64_t p3) {
    float S[48];
    int64_t lVar7 = 0, local_238 = 0, p3r = p3, p2r = (int64_t)p2arg;
    g_pf_reimpl_p2 = (int64_t)p2arg;                   /* default (fallback fires keep the caller's target) */
    int g = pfire_compute_geom(p1, p2arg, p3, S, &lVar7, &local_238, &p3r, &p2r, NULL, NULL);
    if (g != 1) return g;                              /* 0 no-fire / PFIRE_FALLBACK propagated */
    g_pf_reimpl_p2 = p2r;                              /* the resolved (maybe 405870-redirected) target */

    /* ---- R2 applier (261-402): create+init + Class-2b ---- A3.3 inline create (1-shard).
     * p2r = the RESOLVED target (the param_3==0 path may have redirected it via 405870) — the binary uses the
     * redirected param_2 for the lead solver, the +0x38 listener, and the 3a06a0 shot-register. */
    g_pfdbg_reimpl_lvl = (uint32_t)g_pfire_level;     /* DEBUG: the level the reimpl actually sees */
    g_pfdbg_inline++;                                  /* DEBUG: reached R2 (= reimpl actually fires) */
    int64_t proj = pfire_r2a_create_init(p1, (int64_t *)p2r, lVar7, local_238, S);
    pfire_r2b_emit(p1, proj, (int64_t *)p2r, p3r, lVar7, S);

    /* ---- R3 cooldown + listener rebind (403-497) ---- self-write + Class-2b rebind (inline at 1-shard).
     * Uses p2r: the binary reassigns param_2 IN PLACE at :169, so every later reference (incl. R3) sees the
     * redirected target. */
    pfire_r3_cooldown(p1, (int64_t *)p2r);
    return 1;
}

/* pf==2 create-POSITION observe (methodology #27). Recomputes the reimpl create geometry via the SAME
 * pfire_compute_geom the takeover runs — the caller rewinds the LCG to the binary's pre-fire state so the
 * spread/dispersion draws replay the binary's exact sequence. Outputs the computed create-pos (S[8..10]),
 * the two muzzle-matrix translation columns, and flags (bit0=spread weapon, bit1=p1+0x3c<0 single-bone =>
 * 385e70 leaves mat2 unfilled). Returns 1=fire(pos valid)/0/PFIRE_FALLBACK. Drives NOTHING — pure compute. */
static int pfire_observe_geom(int64_t p1, int64_t *p2arg, int64_t p3,
                              float out_pos[3], float out_mat1col[3], float out_mat2col[3],
                              uint32_t *out_flags) {
    float S[48];
    int64_t lVar7 = 0, local_238 = 0, p3r = p3, p2r = (int64_t)p2arg;
    int g = pfire_compute_geom(p1, p2arg, p3, S, &lVar7, &local_238, &p3r, &p2r, out_mat1col, out_mat2col);
    if (g != 1) return g;
    out_pos[0] = S[8]; out_pos[1] = S[9]; out_pos[2] = S[10];   /* create pos = &S[8] = (S8,S9,S10) */
    uint32_t f = 0;
    int64_t owner_type = *(int64_t *)(p1 + 0x20);
    if (*(int8_t *)(owner_type + 0x4f) == 1) f |= 1;            /* spread weapon (ffbb0 cone draw) */
    if (*(int32_t *)(p1 + 0x3c) < 0)         f |= 2;            /* single-bone => 385e70 mat2 unfilled */
    *out_flags = f;
    return 1;
}

/* run the real FUN_1403a76b0 with the spawn-attribution window open. */
static inline void dt_drain_tramp(int64_t ship, int32_t t) {
    g_drain_in_window = 1; g_a76b0_trampoline(ship, t); g_drain_in_window = 0;
}

/* attribute one firing-path spawn to the current 3a76b0 emitter and update the key-inversion counts. */
static inline void dt_drain_on_spawn(void) {
    if (!g_drain_in_window) { g_drain_unattr++; return; }   /* Pass-D / death / non-firing create */
    g_drain_attr++;
    if (!g_drain_have_prev) {
        g_drain_have_prev = 1;
        g_drain_prev_rank = g_drain_cur_rank; g_drain_prev_objid = g_drain_cur_objid;
        return;
    }
    if (g_drain_cur_rank == g_drain_prev_rank) return;       /* same emitter (more shots) — not a transition */
    g_drain_trans++;
    if (g_drain_cur_rank > g_drain_prev_rank) g_drain_rank_up++; else g_drain_rank_down++;
    if      (g_drain_cur_objid > g_drain_prev_objid) g_drain_id_up++;
    else if (g_drain_cur_objid < g_drain_prev_objid) g_drain_id_down++;
    else                                             g_drain_id_eq++;
    if (g_drain_detail < 48) {
        char db[160];
        snprintf(db, sizeof db, "DTDRAIN1\tev=%u\ttick=%u\tprev_rank=%u\tprev_id=%d\tcur_rank=%u\tcur_id=%d\n",
                 g_drain_detail, g_drain_tick, g_drain_prev_rank, g_drain_prev_objid,
                 g_drain_cur_rank, g_drain_cur_objid);
        log_write(db); g_drain_detail++;
    }
    g_drain_prev_rank = g_drain_cur_rank; g_drain_prev_objid = g_drain_cur_objid;
}

/* ════════════════════════════════════════════════════════════════════════════════════════════════
 * MILESTONE a1 — SAFE GATED SFX TAKEOVER (inproc_integration_milestone.md §2)
 *
 * a0 confirmed (observe-only) that the §7 canonical drain key reproduces live serial emission order. a1
 * is the first CONTROL-FLOW step: prove the buffer→canonical-drain plumbing works against the LIVE
 * engine, using the one write-class genuinely safe to defer — Class-3 SFX (off-lockstep; the
 * SFXEventManager re-sorts its queue by wall-clock `timeGetTime()` in FUN_1402d72c0, so a
 * fraction-of-a-tick deferral is inaudible AND cannot perturb the deterministic sim). We intercept the
 * SFX emit FUN_1402d5290 at its 387400 call site, BUFFER the args during the tick, and REPLAY them in
 * canonical (rank,seq) order at the tick boundary.
 *
 * VALIDATION CLAIM: deferring SFX must leave the deterministic sim untouched. The IDEAL check is the
 * per-tick DIFFTRACE position fingerprint bit-identical to baseline — but that stream is emitted only in
 * the PROFILE build (a76b0_hook/dt_emit, #ifdef EAW_PROFILE), which crashes on battle load, so it is NOT
 * available in this stable oracle build. The in-build proxy a1 actually checks (all run-independent
 * INVARIANTS, so valid before/after even without identical inputs):
 *   (1) stability across battles with EAW_A1=1;
 *   (2) DTA1 balance — buffered==drained (±one tick residual), overflow=0, drains>0;
 *   (3) the structural sim-ordering oracles already in this build stay PASS with buffering armed —
 *       DTWA-SPAWN id-allocator (idfail=0, ctrfail=0) + DTDRAIN rank key (rank_down=0) — i.e. SFX
 *       deferral does not perturb the create/order sim path;
 *   (4) audio still plays (the queue keeps getting fed).
 * If any of those breaks, FUN_1402d5290 had a hidden sim side effect and a1 has FALSIFIED the Class-3
 * classification. The full position-fingerprint A/B is the profile-build follow-up (blocked on its battle
 * crash). No creates are deferred (the §0 inline-consume blocker is untouched); no threads.
 *
 * GATING — two independent switches, BOTH required to buffer:
 *   EAW_DIFFTRACE=1  oracle mode (gives g_dt_frame_ctr + the fingerprint to validate against).
 *   EAW_A1=1         arm the takeover. DEFAULT OFF: with EAW_A1 unset the intercept is a pure
 *                    passthrough to the untouched FUN_1402d5290, so the call-site patch is inert.
 * Kill-switch = absence of EAW_A1. Single-threaded (sim/main thread) — no locking.
 * REPLAY PATH: the call site is REPOINTED (not entry-detoured), so FUN_1402d5290's body is intact and we
 * replay by calling it directly via g_a1_sfx_real — sidestepping 2d5290's non-detourable prologue. */
#define A1_SFX_CAP 4096
typedef struct { uint32_t tick, rank, seq;
                 int64_t p1, p2; int32_t p3; int8_t p4; uint32_t p5; } A1SfxRec;
static A1SfxRec  g_a1_buf[A1_SFX_CAP];
static int       g_a1_count    = 0;
static uint32_t  g_a1_tick     = 0xFFFFFFFFu;
static uint32_t  g_a1_seq      = 0;
static EngSfxFn  g_a1_sfx_real = NULL;     /* untouched FUN_1402d5290 (call-site patched, body intact) */
static int       g_a1_enabled  = -1;
static uint32_t  g_a1_tot_buffered=0, g_a1_tot_drained=0, g_a1_overflow=0,
                 g_a1_drains=0, g_a1_maxfill=0, g_a1_reorders=0;
static uint32_t  g_a1_last_log = 0xFFFFFFFFu;

static int a1_on(void) {
    if (g_a1_enabled < 0) {
        const char *e = getenv("EAW_A1");
        g_a1_enabled = (e && e[0] && e[0] != '0') ? 1 : 0;
        log_write(g_a1_enabled
            ? "[eaw-mt] A1: SFX takeover ARMED (EAW_A1=1) — 2d5290 buffered + canonical-order drained\n"
            : "[eaw-mt] A1: SFX takeover off (EAW_A1=1 to arm; also needs EAW_DIFFTRACE=1)\n");
    }
    return g_a1_enabled;
}

/* replay the buffered SFX for the just-finished tick in canonical (rank,seq) order, then clear. seq is
 * already monotonic, so the stable sort only regroups by emitter rank; g_a1_reorders counts the moves it
 * makes (evidence the canonical key is live, not a no-op). */
static void a1_drain_now(void) {
    if (g_a1_count <= 0) { g_a1_count = 0; return; }
    for (int i = 1; i < g_a1_count; i++) {
        A1SfxRec key = g_a1_buf[i];
        int j = i - 1;
        while (j >= 0 && (g_a1_buf[j].rank > key.rank ||
               (g_a1_buf[j].rank == key.rank && g_a1_buf[j].seq > key.seq))) {
            g_a1_buf[j+1] = g_a1_buf[j]; j--; g_a1_reorders++;
        }
        g_a1_buf[j+1] = key;
    }
    if ((uint32_t)g_a1_count > g_a1_maxfill) g_a1_maxfill = (uint32_t)g_a1_count;
    for (int i = 0; i < g_a1_count; i++) {
        A1SfxRec *r = &g_a1_buf[i];
        if (g_a1_sfx_real) g_a1_sfx_real(r->p1, r->p2, r->p3, r->p4, r->p5);
        g_a1_tot_drained++;
    }
    g_a1_drains++;
    g_a1_count = 0;
}

/* drain the previous tick's buffer when the sim advances; periodic DTA1 summary every 1024 ticks. */
static void a1_maybe_drain(uint32_t tick) {
    if (tick == g_a1_tick) return;
    a1_drain_now();
    g_a1_tick = tick;
    g_a1_seq  = 0;
    if (dt_on() && a1_on() && tick != g_a1_last_log && (tick & 0x3ffu) == 0) {
        g_a1_last_log = tick;
        char s[224];
        snprintf(s, sizeof s,
            "DTA1\ttick=%u\tbuffered=%u\tdrained=%u\tdrains=%u\tmaxfill=%u\treorders=%u\toverflow=%u\n",
            tick, g_a1_tot_buffered, g_a1_tot_drained, g_a1_drains,
            g_a1_maxfill, g_a1_reorders, g_a1_overflow);
        log_write(s);
    }
}

/* call-site replacement for FUN_1402d5290 (387400:99). Tail-reached via the repointed E8, so it sees the
 * exact 5 args the caller pushed and its return flows back to the (return-ignoring) caller. */
static int32_t a1_sfx_intercept(int64_t p1, int64_t p2, int32_t p3, int8_t p4, uint32_t p5) {
    if (!(dt_on() && a1_on()))                       /* default / non-oracle: inert passthrough */
        return g_a1_sfx_real ? g_a1_sfx_real(p1, p2, p3, p4, p5) : 0;
    uint32_t tick = g_dt_frame_ctr ? *g_dt_frame_ctr : 0;
    a1_maybe_drain(tick);
    if (g_a1_count >= A1_SFX_CAP) {                  /* buffer full — pass through now, never drop */
        g_a1_overflow++;
        return g_a1_sfx_real ? g_a1_sfx_real(p1, p2, p3, p4, p5) : 0;
    }
    A1SfxRec *r = &g_a1_buf[g_a1_count++];
    r->tick = tick; r->rank = g_drain_cur_rank; r->seq = g_a1_seq++;
    r->p1 = p1; r->p2 = p2; r->p3 = p3; r->p4 = p4; r->p5 = p5;
    g_a1_tot_buffered++;
    return 0;                                        /* 387400:99 ignores the SFX return value */
}

static int64_t dtwa_29f810_hook(int64_t mgr, int64_t template_id, int32_t flags,
                                const float *pos, int64_t p5, int8_t p6, int8_t p7) {
    /* snapshot BOTH id counters + the path-select flag BEFORE the append. */
    int64_t gctr_p = g_dt_imgbase ? (int64_t)(g_dt_imgbase + DTWA_GLOBAL_CTR_RVA) : 0;
    int32_t ctr_b = 0, gctr_b = 0; int8_t errflag_b = 0; int have_before = 0;
    if (mgr && dt_on()) {
        ctr_b      = *(int32_t *)(mgr + DTWA_MGR_CTR_OFF);
        gctr_b     = gctr_p ? *(int32_t *)gctr_p : 0;
        errflag_b  = *(int8_t  *)(mgr + 0x63e);   /* SET => global path, CLEAR => per-manager path */
        have_before = 1;
    }
    int64_t ret = g_dtwa_29f810_trampoline(mgr, template_id, flags, pos, p5, p6, p7);
    /* DTWA-B3 piggyback: when inside a 3825b0 firing window, the FIRST create is the projectile. */
    if (g_b3_in_fire && ret && !g_b3_last_proj) {
        g_b3_last_proj = ret;
        /* R2: capture the create args (arg3 "flags" = type_index, arg5 "p5" = orient ptr). */
        g_b3_args.mgr = mgr; g_b3_args.template_id = template_id;
        g_b3_args.type_index = flags; g_b3_args.orient = p5;
        if (pos) { g_b3_args.pos[0]=pos[0]; g_b3_args.pos[1]=pos[1]; g_b3_args.pos[2]=pos[2]; }
        else     { g_b3_args.pos[0]=g_b3_args.pos[1]=g_b3_args.pos[2]=0.0f; }
        g_b3_args.have = 1;
    }
    if (!dt_on() || !have_before) goto dtwa_diag;
    if (ret == 0) { g_dtwa_null++; goto dtwa_diag; }
    dt_drain_on_spawn();   /* DT-DRAIN a0: attribute this create to the current 3a76b0 emitter */
    {
    int32_t own_id    = *(int32_t *)(ret + DTWA_OBJ_OWNID_OFF);   /* the I1 subject */
    int32_t owner_ref = *(int32_t *)(ret + DTWA_OBJ_OWNER_OFF);   /* param_3 (for the detail line only) */
    /* the counter the allocator actually used this call (flag set => global, else per-manager). */
    int32_t sel_b = errflag_b ? gctr_b : ctr_b;
    int32_t sel_a = errflag_b ? (gctr_p ? *(int32_t *)gctr_p : 0) : *(int32_t *)(mgr + DTWA_MGR_CTR_OFF);
    g_dtwa_calls++;
    if (errflag_b != 0) g_dtwa_errpath++;     /* informational: count global-path spawns */
    if (own_id == DTWA_SATUR_ID) g_dtwa_satur++;
    /* I1 core: own id == pre-increment counter, and the counter advanced by exactly one. */
    if (own_id == sel_b)        g_dtwa_id_eq_ctr++; else g_dtwa_idfail++;
    if (sel_a == sel_b + 1)     g_dtwa_ctr_grew1++; else g_dtwa_ctrfail++;
    /* per-(manager,path) strict monotonicity of the own id. global-path spawns key on a sentinel so
     * all managers sharing the global counter are tracked as one stream. */
    { int64_t key = errflag_b ? (int64_t)-1 : mgr;
      int slot = -1;
      for (int i = 0; i < g_dtwa_nmgr; i++) if (g_dtwa_mgr_key[i] == key) { slot = i; break; }
      if (slot < 0 && g_dtwa_nmgr < DTWA_NMGR) { slot = g_dtwa_nmgr++; g_dtwa_mgr_key[slot] = key; g_dtwa_mgr_lastid[slot] = -1; }
      if (slot >= 0) {
        if (g_dtwa_mgr_lastid[slot] < 0 || own_id > g_dtwa_mgr_lastid[slot]) g_dtwa_mono++;
        g_dtwa_mgr_lastid[slot] = own_id;
      } else { g_dtwa_mono++; }   /* >16 streams (won't happen); don't penalise */
    }
    /* manager-pointer resolution: our adapter must resolve the same `this` the engine got as param_1. */
    { int64_t mgr_res = eng_world_manager();
      if (mgr_res == mgr) g_dtwa_mgr_pass++;
      else {
        g_dtwa_mgr_fail++;
        if (!g_dtwa_mgrshown) {
            g_dtwa_mgrshown = 1; char mb[256];
            snprintf(mb, sizeof mb,
                "DTWAMGR\ttick=%u\tengine_mgr=%llx\tresolved=%llx\ttmpl=%lld\tflags=%d\n",
                g_dt_frame_ctr?*g_dt_frame_ctr:0, (unsigned long long)mgr,
                (unsigned long long)mgr_res, (long long)template_id, flags);
            log_write(mb);
        }
      } }
    /* SpawnCommand schema fidelity: (template,flags) are scalars; pos must be a readable float[3]. */
    if (pos) g_dtwa_schemafit++;
    /* first 64 per-call detail lines: own_id vs selected counter vs owner_ref — direct allocator evidence. */
    if (g_dtwa_detail < 64) {
        char rb[256];
        snprintf(rb, sizeof rb,
            "DTWA1\tev=%u\ttick=%u\town_id=%d\tsel_b=%d\tsel_a=%d\towner_ref=%d\tid_eq_ctr=%d\tgrew1=%d\tgpath=%d\tmgrmatch=%d\ttmpl=%lld\n",
            g_dtwa_detail, g_dt_frame_ctr?*g_dt_frame_ctr:0, own_id, sel_b, sel_a, owner_ref,
            (own_id==sel_b)?1:0, (sel_a==sel_b+1)?1:0, errflag_b?1:0,
            (eng_world_manager()==mgr)?1:0, (long long)template_id);
        log_write(rb);
        g_dtwa_detail++;
    }
    if ((own_id != sel_b || sel_a != sel_b + 1) && !g_dtwa_shown) {
        g_dtwa_shown = 1; char xb[256];
        snprintf(xb, sizeof xb,
            "DTWAMISS\ttick=%u\town_id=%d\tsel_b=%d\tsel_a=%d\towner_ref=%d\tgpath=%d\ttmpl=%lld\tflags=%d\n",
            g_dt_frame_ctr?*g_dt_frame_ctr:0, own_id, sel_b, sel_a, owner_ref,
            errflag_b?1:0, (long long)template_id, flags);
        log_write(xb);
    }
    }
dtwa_diag:
    { uint32_t tk = g_dt_frame_ctr ? *g_dt_frame_ctr : 0;
      if (dt_on() && tk != g_dtwa_last && (tk & 0x3ffu) == 0) {   /* every 1024 ticks */
        g_dtwa_last = tk;
        char db[256];
        snprintf(db, sizeof db,
            "DTWA\ttick=%u\tcalls=%u\tid_eq_ctr=%u\tgrew1=%u\tmono=%u\tidfail=%u\tctrfail=%u\tsatur=%u\terrpath=%u\tmgr_pass=%u\tmgr_fail=%u\tschemafit=%u\tnull=%u\n",
            tk, g_dtwa_calls, g_dtwa_id_eq_ctr, g_dtwa_ctr_grew1, g_dtwa_mono,
            g_dtwa_idfail, g_dtwa_ctrfail, g_dtwa_satur, g_dtwa_errpath,
            g_dtwa_mgr_pass, g_dtwa_mgr_fail, g_dtwa_schemafit, g_dtwa_null);
        log_write(db);
        /* DT-DRAIN a0 summary: the canonical-key vs object_id-key inversion counts. */
        char dd[256];
        snprintf(dd, sizeof dd,
            "DTDRAIN\ttick=%u\tattr=%u\tunattr=%u\ttrans=%u\trank_up=%u\trank_down=%u\tid_up=%u\tid_down=%u\tid_eq=%u\n",
            tk, g_drain_attr, g_drain_unattr, g_drain_trans, g_drain_rank_up,
            g_drain_rank_down, g_drain_id_up, g_drain_id_down, g_drain_id_eq);
        log_write(dd);
      } }
    return ret;
}

static BOOL install_dtwa_spawn_hook(void) {
    HMODULE exe = GetModuleHandleA(NULL);
    if (!exe) return FALSE;
    if (!g_dt_imgbase) g_dt_imgbase = (uintptr_t)exe;
    BYTE *fn = (BYTE *)exe + DTWA_29F810_RVA;
    size_t n = sizeof dtwa_29f810_prologue;          /* 15 */
    if (memcmp(fn, dtwa_29f810_prologue, n) != 0) {
        log_write("[eaw-mt] WARN: CreateObject (0x29f810) prologue mismatch — DTWA-SPAWN not installed\n");
        return FALSE;
    }
    BYTE *tramp = (BYTE *)VirtualAlloc(NULL, 64, MEM_COMMIT|MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    if (!tramp) { log_write("[eaw-mt] WARN: VirtualAlloc for DTWA-SPAWN trampoline failed\n"); return FALSE; }
    memcpy(tramp, dtwa_29f810_prologue, n);
    write_abs_jmp(tramp + n, (uint64_t)(fn + n));
    g_dtwa_29f810_trampoline = (Create29f810Fn)tramp;
    DWORD old;
    VirtualProtect(fn, n, PAGE_EXECUTE_READWRITE, &old);
    write_abs_jmp(fn, (uint64_t)dtwa_29f810_hook);
    VirtualProtect(fn, n, old, &old);
    FlushInstructionCache(GetCurrentProcess(), fn, n);
    log_write("[eaw-mt] DTWA-SPAWN: CreateObject (0x29f810) I1 append-order + manager-resolution oracle installed\n");
    return TRUE;
}

/* ── DTWA-B3: structural oracle for the b3 projectile create+init restructure (see globals above). ────
 * Entry-detour FUN_1403825b0. Prologue (objdump 0x3825b0): mov rax,rsp; push rbp/rbx/rsi/rdi/r13/r15;
 * lea -0x218(rax),rbp = 18 bytes, all position-independent (clean cut before `sub rsp`). */
#define B3_3825B0_RVA 0x3825b0ULL
#define GAME_MODE_DAT_RVA 0xb15418ULL   /* DAT_140b15418 (holds the shroud/mode object ptr) */
static const BYTE b3_3825b0_prologue[18] = {
    0x48,0x8b,0xc4, 0x55, 0x53, 0x56, 0x57, 0x41,0x55, 0x41,0x57,
    0x48,0x8d,0xa8, 0xe8,0xfd,0xff,0xff
};
typedef int64_t (*B3_3825b0Fn)(int64_t,int64_t,int64_t);
static B3_3825b0Fn g_b3_3825b0_trampoline = NULL;

static inline int bits_eq_f(float a, float b) {
    uint32_t ua, ub; memcpy(&ua,&a,4); memcpy(&ub,&b,4); return ua==ub;
}

static void dtwa_b3_check(int64_t proj, int64_t owner, int64_t owner_type,
                          int64_t p2, int64_t p3, int64_t local238, int32_t order_pre) {
    int64_t rec  = *(int64_t *)(proj + 0xe8);     /* plVar12[0x1d] motion/combat record */
    if (!rec) { g_b3_skip++; return; }
    int64_t tmpl = *(int64_t *)(proj + 0x298);    /* proj type_def == the create template lVar7 */
    g_b3_n++;

    /* 1. firer_id (rec+0x58 = owner+0x50) */
    if (*(int32_t *)(rec + 0x58) == *(int32_t *)(owner + 0x50)) g_b3_firer_ok++; else g_b3_firer_bad++;
    /* 2. target (rec+0x08 = param_2) */
    if (*(int64_t *)(rec + 0x08) == p2) g_b3_tgt_ok++; else g_b3_tgt_bad++;
    /* 3. target sub-id (rec+0x10 = resolved-param_3 ? *(param_3+0x18) : -1). When the CALLER passes a
     *    non-null param_3 it is used unchanged (gated at 3825b0:68), so the entry value is authoritative
     *    and we assert. When the caller passes NULL, 3825b0:107-110 REASSIGNS param_3 via 398440/394a80 —
     *    that resolved value is an internal local invisible from an entry/exit detour, so we COUNT those
     *    separately (sub_reasn) rather than mis-assert against the stale entry NULL. */
    if (p3) { if (*(int32_t *)(rec + 0x10) == *(int32_t *)(p3 + 0x18)) g_b3_sub_ok++; else g_b3_sub_bad++; }
    else      g_b3_sub_reasn++;
    /* 4. damage (rec+0x64): owner override else template; charge path (order_pre<0) is recompute-only via
     *    3952a0 → counted separately, not asserted (avoids calling a stateful engine fn). */
    int is_charge = (local238 && order_pre < 0);
    if (is_charge) { g_b3_charge++; }
    else {
        float od = *(float *)(owner_type + 0x478);
        float exp_dmg = (od <= 0.0f) ? *(float *)(tmpl + 0x474) : od;
        if (bits_eq_f(exp_dmg, *(float *)(rec + 0x64))) g_b3_dmg_ok++; else g_b3_dmg_bad++;
    }
    /* 5. lifetime (rec+0x68): 5400f0(owner_type) result lives cached at owner_type+0x4a0 (the binary just
     *    filled it); 0 ⇒ template+0x440. Pure read of the cache — no engine call. */
    { int32_t ol = *(int32_t *)(owner_type + 0x4a0);
      uint32_t exp_life = (ol == 0) ? *(uint32_t *)(tmpl + 0x440) : (uint32_t)ol;
      if (*(uint32_t *)(rec + 0x68) == exp_life) g_b3_life_ok++; else g_b3_life_bad++; }
    /* 6. vis_frame (rec+0x60): only written when owner_type+0x4a4 > 0; = mode+0x10 + that offset. */
    { int32_t vo = *(int32_t *)(owner_type + 0x4a4);
      if (vo > 0) {
        int64_t mode = g_dt_imgbase ? *(int64_t *)(g_dt_imgbase + GAME_MODE_DAT_RVA) : 0;
        int32_t exp_vis = (mode ? *(int32_t *)(mode + 0x10) : 0) + vo;
        if (*(int32_t *)(rec + 0x60) == exp_vis) g_b3_vis_ok++; else g_b3_vis_bad++;
      } }

    /* R2 (§8.14): validate the captured 29f810 create args the applier will reconstruct.
     * mgr == *(owner+0x2b8) re-confirms the I2 manager-resolution; template == proj+0x298 (tmpl). */
    if (g_b3_args.have) {
        g_b3_arg_n++;
        int64_t exp_mgr = *(int64_t *)(owner + 0x2b8);
        if (g_b3_args.mgr == exp_mgr)        g_b3_arg_mgr_ok++;  else g_b3_arg_mgr_bad++;
        if (g_b3_args.template_id == tmpl)   g_b3_arg_tmpl_ok++; else g_b3_arg_tmpl_bad++;
        if (g_b3_arg_detail < 16) {
            char ab[256];
            snprintf(ab, sizeof ab,
                "DTB3ARG\tev=%u\tmgr=%d(exp_mgr=%d)\ttmpl=%d\ttype_idx=%d\tpos=%.2f,%.2f,%.2f\n",
                g_b3_arg_detail, (g_b3_args.mgr==exp_mgr), (g_b3_args.template_id==tmpl),
                (g_b3_args.template_id==tmpl), g_b3_args.type_index,
                g_b3_args.pos[0], g_b3_args.pos[1], g_b3_args.pos[2]);
            log_write(ab); g_b3_arg_detail++;
        }
        g_b3_args.have = 0;
    }

    if (g_b3_detail < 48) {
        char rb[320];
        snprintf(rb, sizeof rb,
            "DTB3\tev=%u\ttick=%u\tfirer=%d/%d\ttgt=%d\tsub=%d/%d\tdmg=%.3f\tlife=%u\tvis=%d\tcharge=%d\n",
            g_b3_detail, g_dt_frame_ctr?*g_dt_frame_ctr:0,
            *(int32_t *)(rec + 0x58), *(int32_t *)(owner + 0x50),
            (*(int64_t *)(rec + 0x08) == p2),
            *(int32_t *)(rec + 0x10), (p3 ? *(int32_t *)(p3 + 0x18) : -1),
            *(float *)(rec + 0x64), *(uint32_t *)(rec + 0x68), *(int32_t *)(rec + 0x60), is_charge);
        log_write(rb); g_b3_detail++;
    }
    if ((g_b3_firer_bad|g_b3_tgt_bad|g_b3_sub_bad|g_b3_dmg_bad|g_b3_life_bad|g_b3_vis_bad)
        && !g_b3_missshown) {
        g_b3_missshown = 1; char xb[320];
        snprintf(xb, sizeof xb,
            "DTB3MISS\ttick=%u\tfirer=%d/%d\ttgt=%d\tsub=%d/%d\tdmg_got=%.4f\tlife_got=%u\tvis_got=%d\n",
            g_dt_frame_ctr?*g_dt_frame_ctr:0,
            *(int32_t *)(rec + 0x58), *(int32_t *)(owner + 0x50),
            (*(int64_t *)(rec + 0x08) == p2),
            *(int32_t *)(rec + 0x10), (p3 ? *(int32_t *)(p3 + 0x18) : -1),
            *(float *)(rec + 0x64), *(uint32_t *)(rec + 0x68), *(int32_t *)(rec + 0x60));
        log_write(xb);
    }
}

/* A4.1 PhaseC drain — apply the buffered spawn commands (create+init+Class-2b) in canonical order at the
 * 2a62d0 flush, AFTER the a76b0 walk replay (so every fire read the settled world in Phase-A). 1-shard:
 * insertion order == ascending rank == canonical, so no sort. Re-stamps the DTDRAIN key (g_drain_cur_*)
 * from each record so the create/order oracle (rank_down) is evaluated on the canonical drain sequence;
 * have_prev reset starts a fresh sequence (the drain is a distinct pass from the replay-phase creates).
 * g_b3_in_fire is set around R2a so the DTWA-SPAWN piggyback captures the deferred proj + create-args, and
 * dtwa_b3_check validates each drained projectile's §3 fields (the A3.3 free-validation, relocated here). */
static void pfire_drain_spawns(void) {
    int n = g_pfire_spawn_count;
    g_pfdbg_drainenter++;                                  /* DEBUG: drain was called */
    if ((uint32_t)n > g_pfdbg_drain_maxn) g_pfdbg_drain_maxn = (uint32_t)n;
    if (n <= 0) return;
    if ((uint32_t)n > g_pfire_spawn_maxfill) g_pfire_spawn_maxfill = (uint32_t)n;
    g_drain_have_prev = 0;
    int armed = dt_on();
    for (int i = 0; i < n; i++) {
        PfireSpawnRec *rec = &g_pfire_spawn_buf[i];
        int64_t p1 = rec->p1, p2 = rec->p2, p3 = rec->p3, lVar7 = rec->lVar7;
        int64_t owner      = *(int64_t *)(p1 + 0x10);
        int64_t owner_type = *(int64_t *)(p1 + 0x20);
        int64_t local_238  = owner ? *(int64_t *)(owner + 0x100) : 0;
        int32_t order_pre  = local_238 ? *(int32_t *)(local_238 + 0x394) : 0;   /* before R2a's charge++ */
        g_drain_cur_rank = rec->rank; g_drain_cur_objid = rec->objid;           /* canonical key */
        g_b3_in_fire = 1; g_b3_last_proj = 0; g_b3_args.have = 0;
        g_drain_in_window = 1;
        int64_t proj = pfire_r2a_create_init(p1, (int64_t *)p2, lVar7, local_238, rec->S);
        pfire_r2b_emit(p1, proj, (int64_t *)p2, p3, lVar7, rec->S);
        g_drain_in_window = 0;
        g_b3_in_fire = 0;
        g_pfire_spawn_drained++;
        if (armed && proj && owner && owner_type)
            dtwa_b3_check(proj, owner, owner_type, p2, p3, local_238, order_pre);
    }
    g_pfire_spawn_count = 0;
}

static int64_t dtwa_b3_3825b0_hook(int64_t p1, int64_t p2, int64_t p3) {
    LARGE_INTEGER hk0; QueryPerformanceCounter(&hk0);   /* A4.1: whole-hook span (fire body + validation) */
    int armed = dt_on();
    int pf = pfire_on();
    int64_t owner=0, owner_type=0, local238=0; int32_t order_pre=0; int r1_g1=0, r1_g2a=0, r1_full=0;
    if (armed && p1) {
        owner      = *(int64_t *)(p1 + 0x10);
        owner_type = *(int64_t *)(p1 + 0x20);
        local238   = owner ? *(int64_t *)(owner + 0x100) : 0;
        order_pre  = local238 ? *(int32_t *)(local238 + 0x394) : 0;
        /* R1 observe: evaluate the gate verdict on the PRE-call state (gate1 early, +gate2a targeting,
         * +gate2b aim/LOS/range). gate2b returns -1 when the binary takes the RNG aim path (skip).
         * DISABLED at takeover (pf>=3): r is then the reimpl's OWN output, so the r-vs-verdict compare is
         * tautological, AND the reimpl already calls these leaves exactly once — re-invoking the gate
         * functions here would double-call them (wasteful, and the idempotent-but-global 35f470 fog rebuild
         * twice). The free validation at takeover is dtwa_b3_check on the reimpl's created projectile. */
        if (pf < 3) {
            r1_g1 = pfire_r1_gate1(p1, (int64_t *)p2, p3);
            r1_g2a = r1_g1 ? pfire_r1_gate2a(p1, (int64_t *)p2, p3) : 0;
            r1_full = r1_g2a ? pfire_r1_gate2b(p1, (int64_t *)p2, p3) : 0;
        }
        g_b3_in_fire = 1; g_b3_last_proj = 0; g_b3_args.have = 0;  /* reset per call → have=1 ⟺ THIS call created */
    }
    /* ── A3.3 TAKEOVER (EAW_PFIRE=3) ── drive the firing body from the lifted reimpl instead of the binary.
     * Runs in PhaseB (a76b0 is deferred to the 2a62d0 flush at level>=2, so every 3825b0 call here is on the
     * settled world). pfire_fire_reimpl handles the common tactical path (returns 0 no-fire / 1 fired) and
     * returns PFIRE_FALLBACK for the rare/dead edges (arc gate owner_type+0x4e==1; param_3==0 RNG aim path) —
     * it bails BEFORE any RNG draw on those, so the binary fallback re-runs cleanly with no LCG perturbation.
     * 1-shard: the reimpl's R2a create runs INLINE (= PhaseC), and it calls the binary 29f810, so the
     * DTWA-SPAWN piggyback still captures g_b3_last_proj and dtwa_b3_check validates the reimpl's projectile
     * §3 fields (firer/tgt/dmg/life/sub/vis — all RNG-free) bit-exact for free. */
    int64_t r;
    /* pf==2 POSITION OBSERVE — snapshot the weapon LCG BEFORE the binary fires, so the observe can rewind to
     * this exact state and replay the binary's spread/dispersion draw sequence (methodology #27). */
    uint32_t *LCGw = g_dt_imgbase ? (uint32_t *)(g_dt_imgbase + 0xa13e24) : NULL;
    uint32_t lcg_pre = (pf == 2 && LCGw && armed && p1) ? *LCGw : 0;
    LARGE_INTEGER fb0; QueryPerformanceCounter(&fb0);   /* A4.1: fire-body span (the part that threads) */
    if (pf >= 3) {
        r = pfire_fire_reimpl(p1, (int64_t *)p2, p3);
        if (r == PFIRE_FALLBACK) r = g_b3_3825b0_trampoline(p1, p2, p3);
    } else {
        r = g_b3_3825b0_trampoline(p1, p2, p3);
    }
    { LARGE_INTEGER fb1; QueryPerformanceCounter(&fb1);
      g_pf_fire_qpc += (fb1.QuadPart - fb0.QuadPart); g_pf_fire_n++; }
    if (armed && p1) {
        g_b3_in_fire = 0;
        /* pf==2 POSITION OBSERVE — rewind the LCG and recompute the reimpl create-pos on the binary's exact
         * pre-fire draw state, then diff vs the binary's captured 29f810 pos arg (g_b3_args.pos). Surfaces
         * the create-geometry traps (methodology #27) without driving the body — binary fires normally. */
        if (pf == 2 && LCGw && g_b3_args.have && owner_type) {
            uint32_t lcg_post = *LCGw;
            *LCGw = lcg_pre;                              /* rewind → replay the binary's draw sequence */
            float opos[3]={0,0,0}, m1[3]={0,0,0}, m2[3]={0,0,0}; uint32_t oflags=0;
            int ofire = pfire_observe_geom(p1, (int64_t *)p2, p3, opos, m1, m2, &oflags);
            *LCGw = lcg_post;                             /* restore → live RNG stream untouched */
            if (ofire == 1) {
                g_pfobs_n++;
                float *bp = g_b3_args.pos;
                float ex=opos[0]-bp[0], ey=opos[1]-bp[1], ez=opos[2]-bp[2];
                float err2 = ex*ex + ey*ey + ez*ez;
                int origin = (opos[0]*opos[0]+opos[1]*opos[1]+opos[2]*opos[2] < 1.0f) &&
                             (bp[0]*bp[0]+bp[1]*bp[1]+bp[2]*bp[2] >= 1.0f);
                if (err2 <= 0.25f) g_pfobs_match++;
                else {
                    g_pfobs_mismatch++;
                    if (origin) g_pfobs_origin++;
                    if (g_pfobs_detail < 32) {
                        char ob[256];
                        snprintf(ob, sizeof ob,
                            "PFOBS\tobs=%.1f,%.1f,%.1f\tbin=%.1f,%.1f,%.1f\tmat1=%.1f,%.1f,%.1f\tmat2=%.1f,%.1f,%.1f\tspread=%d\tp3c_neg=%d\n",
                            opos[0],opos[1],opos[2], bp[0],bp[1],bp[2], m1[0],m1[1],m1[2], m2[0],m2[1],m2[2],
                            (oflags&1)?1:0, (oflags&2)?1:0);
                        log_write(ob); g_pfobs_detail++;
                    }
                }
            } else if (ofire == PFIRE_FALLBACK) {
                g_pfobs_fallback++;   /* reimpl deferred (arc/param_3==0) → fires via the binary at takeover (OK) */
            } else {
                g_pfobs_nofire++;     /* GENUINE under-fire: binary fired but the reimpl said no-fire (BAD) */
            }
        }
        if (pf < 3) {
            /* R1 gate-1: r==1 ⇒ gate1 must pass (g1_bug = impossible); g1==1 & r==0 = blocked later. */
            g_r1_g1_n++;
            if (r == 1) { if (r1_g1) g_r1_g1_ok++; else g_r1_g1_bug++; }
            else if (r1_g1) g_r1_g1_nofire++;
            /* gate1+gate2a verdict (r==1 ⇒ must pass; g2_bug must be 0). */
            if (r == 1) { if (r1_g2a) g_r1_g2_ok++; else g_r1_g2_bug++; }
            else if (r1_g2a) g_r1_g2_nofire++;
            /* gate1+gate2a+gate2b verdict; r1_full==-1 = skipped RNG aim path (param_3==0). */
            if (r1_full == -1) g_r1_g3_skip++;
            else if (r == 1) { if (r1_full == 1) g_r1_g3_ok++; else g_r1_g3_bug++; }
            else if (r1_full == 1) g_r1_g3_nofire++;
        }
        if (r == 1 && g_b3_last_proj && owner && owner_type) {
            /* tgt oracle checks rec+0x08 == param_2. At takeover the param_3==0 path may REDIRECT param_2
             * (405870), and the projectile correctly stores the redirected target — so compare against the
             * reimpl's RESOLVED p2 (g_pf_reimpl_p2), not the stale pre-redirect arg, else redirected fires
             * false-flag. (At pf<3 the binary's internal redirect is invisible, so use the entry p2 as before.) */
            int64_t chk_p2 = (pf >= 3) ? g_pf_reimpl_p2 : p2;
            dtwa_b3_check(g_b3_last_proj, owner, owner_type, chk_p2, p3, local238, order_pre);
        }
        uint32_t tk = g_dt_frame_ctr ? *g_dt_frame_ctr : 0;
        if (tk != g_b3_last && (tk & 0x3ffu) == 0) {     /* every 1024 ticks */
            g_b3_last = tk; char db[384];
            snprintf(db, sizeof db,
                "DTB3SUM\ttick=%u\tn=%u\tfirer=%u/%u\ttgt=%u/%u\tsub=%u/%u\tsub_reasn=%u\tdmg=%u/%u\tlife=%u/%u\tvis=%u/%u\tcharge=%u\tskip=%u\targ_n=%u\targ_mgr=%u/%u\targ_tmpl=%u/%u%s\n",
                tk, g_b3_n, g_b3_firer_ok,g_b3_firer_bad, g_b3_tgt_ok,g_b3_tgt_bad,
                g_b3_sub_ok,g_b3_sub_bad, g_b3_sub_reasn, g_b3_dmg_ok,g_b3_dmg_bad, g_b3_life_ok,g_b3_life_bad,
                g_b3_vis_ok,g_b3_vis_bad, g_b3_charge, g_b3_skip,
                g_b3_arg_n, g_b3_arg_mgr_ok,g_b3_arg_mgr_bad, g_b3_arg_tmpl_ok,g_b3_arg_tmpl_bad,
                pf >= 3 ? "\tTAKEOVER=reimpl" : "");
            log_write(db);
            if (pf < 3) {
                snprintf(db, sizeof db,
                    "DTR1SUM\ttick=%u\tg1_n=%u\tg1_ok=%u\tg1_bug=%u\tg1_nofire=%u\tg2_ok=%u\tg2_bug=%u\tg2_nofire=%u\tg3_ok=%u\tg3_bug=%u\tg3_nofire=%u\tg3_skip=%u\n",
                    tk, g_r1_g1_n, g_r1_g1_ok, g_r1_g1_bug, g_r1_g1_nofire,
                    g_r1_g2_ok, g_r1_g2_bug, g_r1_g2_nofire,
                    g_r1_g3_ok, g_r1_g3_bug, g_r1_g3_nofire, g_r1_g3_skip);
                log_write(db);
            } else {
                snprintf(db, sizeof db,
                    "DTR1SUM\ttick=%u\tTAKEOVER(EAW_PFIRE=3): reimpl drives the fire body; r-vs-gate observe disabled (r is the reimpl's own verdict)\n", tk);
                log_write(db);
            }
        }
    }
    { LARGE_INTEGER hk1; QueryPerformanceCounter(&hk1);
      g_pf_hook_qpc += (hk1.QuadPart - hk0.QuadPart); }   /* A4.1: whole-hook span */
    return r;
}

static BOOL install_dtwa_b3_hook(void) {
    HMODULE exe = GetModuleHandleA(NULL);
    if (!exe) return FALSE;
    if (!g_dt_imgbase) g_dt_imgbase = (uintptr_t)exe;
    BYTE *fn = (BYTE *)exe + B3_3825B0_RVA;
    size_t n = sizeof b3_3825b0_prologue;            /* 18 */
    if (memcmp(fn, b3_3825b0_prologue, n) != 0) {
        log_write("[eaw-mt] WARN: firing body (0x3825b0) prologue mismatch — DTWA-B3 not installed\n");
        return FALSE;
    }
    BYTE *tramp = (BYTE *)VirtualAlloc(NULL, 64, MEM_COMMIT|MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    if (!tramp) { log_write("[eaw-mt] WARN: VirtualAlloc for DTWA-B3 trampoline failed\n"); return FALSE; }
    memcpy(tramp, b3_3825b0_prologue, n);
    write_abs_jmp(tramp + n, (uint64_t)(fn + n));
    g_b3_3825b0_trampoline = (B3_3825b0Fn)tramp;
    DWORD old;
    VirtualProtect(fn, n, PAGE_EXECUTE_READWRITE, &old);
    write_abs_jmp(fn, (uint64_t)dtwa_b3_3825b0_hook);
    VirtualProtect(fn, n, old, &old);
    FlushInstructionCache(GetCurrentProcess(), fn, n);
    log_write("[eaw-mt] DTWA-B3: firing body (0x3825b0) projectile init §3 source-map oracle installed\n");
    return TRUE;
}

/* ── Piece 2b: DTWASIG — detours the signal fan-out FUN_140240940(dispatcher, sig_id, payload). ──────
 * FUN_140220ed0 (the schema's emit entry) is a 26-byte null-check thunk that tail-jumps here; 0x240940
 * is the real fan-out and has a clean 17-byte position-independent prologue, so we detour it. Validates
 * that the Command schema captures (emitter = dispatcher-0x38, sig_id, payload) losslessly and records
 * the live signal-id traffic (evidence the modeled Class-2b channel == what the engine actually emits).
 * Hot path — trampoline-passthrough, all bookkeeping under dt_on(). EAW_DIFFTRACE=1.
 * Prologue (objdump 0x240940): 488bc4 / 4c894018 / 895010 / 57 / 4157 / 4883ec68 = 17 bytes clean. */
#define DTWA_240940_RVA 0x240940ULL
static const BYTE dtwa_240940_prologue[17] = {  /* mov rax,rsp; mov [rax+0x18],r8; mov [rax+0x10],edx; push rdi/r15; sub rsp,0x68 */
    0x48,0x8b,0xc4, 0x4c,0x89,0x40,0x18, 0x89,0x50,0x10, 0x57, 0x41,0x57, 0x48,0x83,0xec,0x68
};
typedef void (*Sig240940Fn)(int64_t,uint32_t,const void*);
static Sig240940Fn g_dtwa_240940_trampoline = NULL;
#define DTWASIG_NSIG 48
static uint32_t g_dtwasig_calls=0, g_dtwasig_sigok=0, g_dtwasig_disp0=0, g_dtwasig_pl0=0, g_dtwasig_emok=0;
static uint32_t g_dtwasig_sigseen[DTWASIG_NSIG], g_dtwasig_sigcnt[DTWASIG_NSIG]; static int g_dtwasig_nseen=0;
static uint32_t g_dtwasig_sigover=0;   /* distinct sig_ids beyond DTWASIG_NSIG (table-full overflow) */
static uint32_t g_dtwasig_last=0xFFFFFFFFu;

static void dtwa_240940_hook(int64_t dispatcher, uint32_t sig_id, const void *payload) {
    g_dtwa_240940_trampoline(dispatcher, sig_id, payload);
    if (!dt_on()) return;
    g_dtwasig_calls++;
    /* Command-schema field recoverability (the three fields a Class-2b Command needs):
     *  emitter = dispatcher-0x38 (the containing object); recoverable iff dispatcher is a real ptr.
     *  sig_id  = small enum; payload = opaque blob ptr. We confirm each is present, not dropped. */
    if (dispatcher == 0)              g_dtwasig_disp0++;
    else if (dispatcher >= 0x38)      g_dtwasig_emok++;   /* dispatcher-0x38 is a valid emitter base addr */
    if (payload == NULL)              g_dtwasig_pl0++;
    if (sig_id < 0x1000)              g_dtwasig_sigok++;   /* sane signal-id range (Command.sig_id enum) */
    /* per-sig histogram: quantify the live Class-2b channel (the modeled 8 sigs are a subset) */
    { int found=0; for (int i=0;i<g_dtwasig_nseen;i++) if (g_dtwasig_sigseen[i]==sig_id) { g_dtwasig_sigcnt[i]++; found=1; break; }
      if (!found) { if (g_dtwasig_nseen<DTWASIG_NSIG) { g_dtwasig_sigseen[g_dtwasig_nseen]=sig_id; g_dtwasig_sigcnt[g_dtwasig_nseen]=1; g_dtwasig_nseen++; }
                    else g_dtwasig_sigover++; } }
    { uint32_t tk = g_dt_frame_ctr ? *g_dt_frame_ctr : 0;
      if (tk != g_dtwasig_last && (tk & 0x3ffu)==0) {   /* every 1024 ticks */
        g_dtwasig_last = tk;
        char db[1024];
        int off = snprintf(db, sizeof db,
            "DTWASIG\ttick=%u\tcalls=%u\tsigok=%u\temok=%u\tdisp0=%u\tpl0=%u\tdistinct=%d\tsigover=%u\thist=",
            tk, g_dtwasig_calls, g_dtwasig_sigok, g_dtwasig_emok, g_dtwasig_disp0,
            g_dtwasig_pl0, g_dtwasig_nseen, g_dtwasig_sigover);
        for (int i=0;i<g_dtwasig_nseen && off<(int)sizeof db-20;i++)
            off += snprintf(db+off, sizeof db-off, "%x:%u,", g_dtwasig_sigseen[i], g_dtwasig_sigcnt[i]);
        snprintf(db+off, sizeof db-off, "\n");
        log_write(db);
      } }
}

static BOOL install_dtwa_sig_hook(void) {
    HMODULE exe = GetModuleHandleA(NULL);
    if (!exe) return FALSE;
    if (!g_dt_imgbase) g_dt_imgbase = (uintptr_t)exe;
    BYTE *fn = (BYTE *)exe + DTWA_240940_RVA;
    size_t n = sizeof dtwa_240940_prologue;          /* 17 */
    if (memcmp(fn, dtwa_240940_prologue, n) != 0) {
        log_write("[eaw-mt] WARN: signal fan-out (0x240940) prologue mismatch — DTWASIG not installed\n");
        return FALSE;
    }
    BYTE *tramp = (BYTE *)VirtualAlloc(NULL, 64, MEM_COMMIT|MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    if (!tramp) { log_write("[eaw-mt] WARN: VirtualAlloc for DTWASIG trampoline failed\n"); return FALSE; }
    memcpy(tramp, dtwa_240940_prologue, n);
    write_abs_jmp(tramp + n, (uint64_t)(fn + n));
    g_dtwa_240940_trampoline = (Sig240940Fn)tramp;
    DWORD old;
    VirtualProtect(fn, n, PAGE_EXECUTE_READWRITE, &old);
    write_abs_jmp(fn, (uint64_t)dtwa_240940_hook);
    VirtualProtect(fn, n, old, &old);
    FlushInstructionCache(GetCurrentProcess(), fn, n);
    log_write("[eaw-mt] DTWASIG: signal fan-out (0x240940) Command-schema/traffic oracle installed\n");
    return TRUE;
}

static void b3a76b0_hook(int64_t ship, int32_t tick_param)
{
    int      count = 0, n_active = 0, has_budget = 0, cheap = 0;
    int64_t  arr = 0;
    float    total_w = 0.0f;

    /* DT-DRAIN a0: stamp this ship's master-list visitation rank + obj+0x50 id (reset per tick).
     * Runs for EVERY serviced ship before any early return, so the attribution covers all firers. */
    if (ship && g_dt_frame_ctr && dt_on()) {
        uint32_t dtk = *g_dt_frame_ctr;
        a1_maybe_drain(dtk);   /* a1: flush the previous tick's deferred SFX in canonical order */
        if (dtk != g_drain_tick) { g_drain_tick = dtk; g_drain_rank = 0; g_drain_have_prev = 0; }
        g_drain_cur_rank  = g_drain_rank++;
        g_drain_cur_objid = *(int32_t *)(ship + 0x50);
    }

    if (ship && g_dt_frame_ctr && dt_on()) {
        int64_t vec = *(int64_t *)(ship + 0x2d0);
        /* same gate as the engine: vector present and (ship+0x3a0 & 0x40)==0 */
        if (vec && (*(uint8_t *)(ship + 0x3a0) & 0x40) == 0) {
            count = *(int *)(vec + 0x10);
            if (count > 0 && count <= DTFIRE_MAXHP) {
                arr = *(int64_t *)(vec + 8);
                dtfire_scan(arr, count, &n_active, &total_w, &has_budget);
                cheap = (n_active >= 2);
            }
        }
    }

    if (!cheap) { dt_drain_tramp(ship, tick_param); return; }

    /* --- survey accounting + periodic flush (settles dormant-vs-mislatched) --- */
    uint32_t tick = *g_dt_frame_ctr;
    g_surv_na2++;
    if (total_w > 0.0f) {
        g_surv_twpos++;
        if (total_w > g_surv_maxtw) { g_surv_maxtw = total_w; g_surv_maxtw_ent = ship; }
    }
    if (tick != g_surv_last && (tick & 0x1ffu) == 0) {       /* once per 512 ticks */
        g_surv_last = tick;
        char sv[256];
        snprintf(sv, sizeof sv,
            "DTFIRESURVEY\ttick=%u\tna2_shipticks=%u\ttwpos_shipticks=%u\t"
            "consume_shipticks=%u\tmaxtw=%.6f\tmaxtw_ent=%llx\n",
            tick, g_surv_na2, g_surv_twpos, g_surv_consume,
            g_surv_maxtw, (unsigned long long)g_surv_maxtw_ent);
        log_write(sv);
    }

    /* only a ship that can actually distribute (total_w>0 and some budget) goes to the
     * snapshot + consumption-latched oracle emission below */
    if (!(total_w > 0.0f && has_budget)) { dt_drain_tramp(ship, tick_param); return; }

    /* --- snapshot each active mount's budget+weight BEFORE distribution.
     * We do this for EVERY qualifying ship (not just the latched one) so we can detect
     * which ship actually CONSUMES budget this tick — a ship can pass the weight/budget
     * gate yet not fire (capacity<=base, or negligible weight), and such a ship must not
     * be allowed to hog the latch and starve out the real firing ships. --- */
    static float   b0[DTFIRE_MAXHP], w[DTFIRE_MAXHP], b1[DTFIRE_MAXHP];
    static int     act[DTFIRE_MAXHP];
    static int64_t mp[DTFIRE_MAXHP];
    for (int i = 0; i < count; i++) {
        int64_t m = *(int64_t *)(arr + (int64_t)i * 8);
        mp[i] = m;
        if (m) {
            int64_t owner = *(int64_t *)(m + 0x20);
            act[i] = (owner && *(uint8_t *)(owner + 0x4d) == 1) ? 1 : 0;
            b0[i]  = *(float *)(m + 0x28);
            w[i]   = *(float *)(m + 0x58);
        } else { act[i] = 0; b0[i] = 0.0f; w[i] = 0.0f; }
    }

    dt_drain_tramp(ship, tick_param);

    /* --- after: re-read budgets (387010 leaves mount+0x28 untouched) + detect consumption --- */
    int consumed = 0;
    for (int i = 0; i < count; i++) {
        b1[i] = mp[i] ? *(float *)(mp[i] + 0x28) : 0.0f;
        if (act[i] && b0[i] - b1[i] > 1e-4f) consumed = 1;
    }
    if (!consumed) return;   /* qualifying but idle this tick — don't latch or emit */
    g_surv_consume++;

    /* latch onto a ship that is actually firing; release it only after FIRE_STALE_TICKS
     * elapse with no fire from it, then a different firing ship may take the latch. */
    if (!g_fire_ent) {
        g_fire_ent = ship;
    } else if (ship != g_fire_ent &&
               (uint32_t)(tick - g_fire_seentick) > FIRE_STALE_TICKS) {
        g_fire_ent = ship;
    }
    if (ship != g_fire_ent) return;
    g_fire_seentick = tick;
    if (tick == g_fire_lasttick) return;
    g_fire_lasttick = tick;

    for (int i = 0; i < count; i++) {
        if (!mp[i] || !act[i]) continue;   /* only active mounts participate in distribution */
        char s[256];
        snprintf(s, sizeof s,
            "DTFIRE\ttick=%u\tent=%llx\tnm=%d\tna=%d\tidx=%d\tb0=%.6f\tw=%.6f\tb1=%.6f\n",
            tick, (unsigned long long)ship, count, n_active, i, b0[i], w[i], b1[i]);
        log_write(s);
    }
}

static BOOL install_b3a76b0_hook(void)
{
    HMODULE exe = GetModuleHandleA(NULL);
    if (!exe) return FALSE;
    BYTE *fn = (BYTE *)exe + FIRE_A76B0_RVA;
    if (memcmp(fn, fire_a76b0_prologue, 16) != 0) {
        log_write("[eaw-mt] WARN: FUN_1403a76b0 prologue mismatch — DTFIRE not installed\n");
        return FALSE;
    }
    BYTE *tramp = (BYTE *)VirtualAlloc(NULL, 64, MEM_COMMIT|MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    if (!tramp) {
        log_write("[eaw-mt] WARN: VirtualAlloc for DTFIRE trampoline failed\n");
        return FALSE;
    }
    memcpy(tramp, fire_a76b0_prologue, 16);
    write_abs_jmp(tramp + 16, (uint64_t)(fn + 16));
    g_a76b0_trampoline = (A76b0Fn)tramp;

    DWORD old;
    VirtualProtect(fn, 14, PAGE_EXECUTE_READWRITE, &old);
    write_abs_jmp(fn, (uint64_t)b3a76b0_hook);
    VirtualProtect(fn, 14, old, &old);
    FlushInstructionCache(GetCurrentProcess(), fn, 14);
    log_write("[eaw-mt] DTFIRE: FUN_1403a76b0 hardpoint fire-budget hook installed\n");
    return TRUE;
}

/* =========================================================================
 * DTRNG — global sim RNG (FUN_1401ffb40) I/O capture.
 *
 * FUN_1401ffb40 is the bounded-int draw on the ONE shared LCG state word
 * (DAT_140a13e24):  s' = s*0x41c64e6d + 0xbdf,  result = (s'>>10)&mask + lo,
 * rejection-sampled while > range. 202 call sites pull from it — the
 * opportunity-target scan start index in FUN_140387400, Starship locomotor
 * jitter, entity-spine perturbations — so it is THE determinism primitive for
 * the whole Phase-3/4 replay. We wrap it directly and capture each draw's
 * complete I/O:
 *   s_in  = *state before the call,
 *   a, b  = the range args (edx, r8d),
 *   ret   = the returned int,
 *   s_out = *state after the call (encodes the rejection draw COUNT).
 * Offline (tools/analyze_rng_oracle.py): SimRng(s_in).range_i(a,b) must == ret
 * AND leave state == s_out — validating constants, masking AND draw count.
 *
 * Hot path (thousands/tick) -> sample 1-in-DTRNG_SAMPLE and cap total lines.
 * The trampoline ALWAYS runs exactly once (no extra draws -> NO determinism
 * perturbation); only logging is sampled, and each tuple is self-contained.
 *
 * arg count = 3 (decomp/1ffb40.c): param_1=rcx uint*, param_2=edx int,
 * param_3=r8d int; returns eax int.
 * Prologue (objdump): 41 3b d0 / 45 8b c8 / 44 0f 4e ca / 41 0f 4e d0
 * = 3+3+4+4 = 14 bytes, all register-only (relocatable), boundary == FF25 width.
 * Profile-build only; runtime-gated by EAW_DIFFTRACE=1.
 * ========================================================================= */
#define RNG_FFB40_RVA    0x1ffb40ULL
#define DTRNG_SAMPLE     64u       /* log 1 of every N draws (power of two) */
#define DTRNG_MAXLINES   20000u    /* hard cap on emitted lines */
static const BYTE rng_ffb40_prologue[14] = {
    0x41, 0x3b, 0xd0,              /* cmp    edx, r8d */
    0x45, 0x8b, 0xc8,              /* mov    r9d, r8d */
    0x44, 0x0f, 0x4e, 0xca,        /* cmovle r9d, edx */
    0x41, 0x0f, 0x4e, 0xd0,        /* cmovle edx, r8d */
};
typedef int (*Ffb40Fn)(uint32_t *state, int a, int b);
static Ffb40Fn  g_ffb40_trampoline = NULL;
static uint32_t g_rng_callctr = 0;
static uint32_t g_rng_lines   = 0;

static int b1ffb40_hook(uint32_t *state, int a, int b)
{
    if (!(g_dt_frame_ctr && dt_on())) return g_ffb40_trampoline(state, a, b);

    uint32_t idx = g_rng_callctr++;
    int sample = (state != NULL) && ((idx & (DTRNG_SAMPLE - 1u)) == 0u) &&
                 (g_rng_lines < DTRNG_MAXLINES);
    if (!sample) return g_ffb40_trampoline(state, a, b);

    uint32_t s_in = *state;
    int ret = g_ffb40_trampoline(state, a, b);
    uint32_t s_out = *state;

    char s[160];
    snprintf(s, sizeof s,
        "DTRNG\ttick=%u\ts_in=%08x\ta=%d\tb=%d\tret=%d\ts_out=%08x\n",
        *g_dt_frame_ctr, s_in, a, b, ret, s_out);
    log_write(s);
    g_rng_lines++;
    return ret;
}

static BOOL install_b1ffb40_hook(void)
{
    HMODULE exe = GetModuleHandleA(NULL);
    if (!exe) return FALSE;
    BYTE *fn = (BYTE *)exe + RNG_FFB40_RVA;
    if (memcmp(fn, rng_ffb40_prologue, 14) != 0) {
        log_write("[eaw-mt] WARN: FUN_1401ffb40 prologue mismatch — DTRNG not installed\n");
        return FALSE;
    }
    BYTE *tramp = (BYTE *)VirtualAlloc(NULL, 64, MEM_COMMIT|MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    if (!tramp) {
        log_write("[eaw-mt] WARN: VirtualAlloc for DTRNG trampoline failed\n");
        return FALSE;
    }
    memcpy(tramp, rng_ffb40_prologue, 14);
    write_abs_jmp(tramp + 14, (uint64_t)(fn + 14));
    g_ffb40_trampoline = (Ffb40Fn)tramp;

    DWORD old;
    VirtualProtect(fn, 14, PAGE_EXECUTE_READWRITE, &old);
    write_abs_jmp(fn, (uint64_t)b1ffb40_hook);
    VirtualProtect(fn, 14, old, &old);
    FlushInstructionCache(GetCurrentProcess(), fn, 14);
    log_write("[eaw-mt] DTRNG: FUN_1401ffb40 global-RNG hook installed\n");
    return TRUE;
}

/* =========================================================================
 * DTDMG — DamageTracking decay/expiry (FUN_14058bd80) I/O capture.
 *
 * In-game oracle for the Phase-3 IN sim behavior #1 lift (sim/damage_tracking.cpp).
 * DamageTrackingBehaviorClass::vfunc_6 is a TIMED-EFFECT-LIST DECAY TICKER over the
 * entity's own damage/status effects (entity+0x1a0 -> DamageEffectList sub-object).
 * Each tick it computes one shared decay scalar
 *   decay = (FUN_1403727a0 + FUN_1403725f0) * _DAT_140b168fc
 * walks the intrusive list decrementing each node's +0x14 duration, removes nodes
 * whose new duration is NOT > 0 (exactly-0 removed), and — edge-triggered on the tick
 * the LAST effect expires (count@+0x20 hits 0) — emits parameterless signal 0x2d on
 * entity+0x38. Tail: rate@+0x0c = (DAT_140b0a340 * accum@+0x08) / interval(behavior+0x34),
 * accum := 0. (decomp/58bd80.c.)
 *
 * The lifted core takes the resolved `decay` scalar (DamageEnv) and a CommandSink; this
 * oracle confirms the in-game reading of: (a) every surviving node decays by a SINGLE
 * shared scalar — recoverable offline as decay = dur_before - dur_after, equal across all
 * survivors; (b) removed nodes are exactly those with dur_before - decay <= 0; (c) the
 * count→0 transition (the proxy for the 0x2d emit) is edge-triggered; (d) the rate readout
 * = hz*accum/interval with accum reset. We snapshot the list BEFORE the trampoline (keys +
 * durations, insertion order) and re-walk survivors AFTER (freed nodes are gone; survivors
 * remain), so the per-node before/after deltas yield `decay` with no need to call the
 * world-coupled rate fns ourselves (3727a0 reads a player-diplomacy table + buff stack).
 *
 * The trampoline runs exactly once (no extra list mutation -> no determinism perturbation);
 * only logging is gated + latched onto one active entity at a time.
 *
 * arg count = 2 (decomp/58bd80.c): param_1=rcx behavior(this), param_2=rdx entity. Returns
 * undefined8 (eax 0x650000) — preserved through the wrapper.
 * Prologue (objdump @14058bd80): 40 53 / 55 / 57 / 41 54 / 41 56 / 48 83 ec 40 /
 * 0f 29 74 24 30 = 2+1+1+2+2+4+5 = 17 bytes (a 14-byte FF25 patch lands mid-movaps, so
 * copy 17; all push/sub/movaps reg — none RIP-relative). Own latch, independent of the
 * other DT hooks. Profile-build only; runtime-gated by EAW_DIFFTRACE=1.
 * ========================================================================= */
#define DMG_58BD80_RVA     0x58bd80ULL
#define DTDMG_MAXEFF       64
#define DTDMG_STALE_TICKS  240u
static const BYTE dmg_58bd80_prologue[17] = {
    0x40, 0x53,                    /* push rbx                 */
    0x55,                          /* push rbp                 */
    0x57,                          /* push rdi                 */
    0x41, 0x54,                    /* push r12                 */
    0x41, 0x56,                    /* push r14                 */
    0x48, 0x83, 0xec, 0x40,        /* sub rsp, 0x40            */
    0x0f, 0x29, 0x74, 0x24, 0x30,  /* movaps [rsp+0x30], xmm6  */
};
typedef int64_t (*Dmg58bd80Fn)(int64_t behavior, int64_t entity);
static Dmg58bd80Fn g_dmg_trampoline = NULL;
static int64_t  g_dmg_ent         = 0;
static uint32_t g_dmg_lasttick    = 0xFFFFFFFFu;
static uint32_t g_dmg_seentick    = 0;
static uint32_t g_dmg_surv_active = 0;          /* entity-ticks with >=1 active effect      */
static uint32_t g_dmg_surv_emit   = 0;          /* entity-ticks where list emptied (0x2d emit) */
static uint32_t g_dmg_surv_last   = 0xFFFFFFFFu;

static int64_t b58bd80_hook(int64_t behavior, int64_t entity)
{
    /* self-contained lazy init (don't depend on the a76b0 hook firing first this tick) */
    if (!g_dt_frame_ctr) {
        HMODULE exe = GetModuleHandleA(NULL);
        if (exe) { g_dt_frame_ctr = (volatile uint32_t *)((BYTE *)exe + 0xb0a320);
                   g_dt_imgbase = (uintptr_t)exe; }
    }
    if (!(entity && g_dt_frame_ctr && dt_on()))
        return g_dmg_trampoline(behavior, entity);

    int64_t list = *(int64_t *)(entity + 0x1a0);
    if (!list) return g_dmg_trampoline(behavior, entity);

    int64_t count_before = *(int64_t *)(list + 0x20);
    if (count_before <= 0 || count_before > DTDMG_MAXEFF)   /* no active effects → skip */
        return g_dmg_trampoline(behavior, entity);

    uint32_t tick = *g_dt_frame_ctr;
    g_dmg_surv_active++;

    /* snapshot keys + durations BEFORE decay (intrusive list, insertion order).
     * sentinel = *(list+0x18); first node = *(sentinel); next = *(node); stop at sentinel. */
    static uint32_t key0[DTDMG_MAXEFF];
    static float    dur0[DTDMG_MAXEFF];
    int     n0 = 0;
    int64_t sentinel = *(int64_t *)(list + 0x18);
    if (sentinel) {
        for (int64_t node = *(int64_t *)sentinel;
             node && node != sentinel && n0 < DTDMG_MAXEFF; node = *(int64_t *)node) {
            key0[n0] = *(uint32_t *)(node + 0x10);
            dur0[n0] = *(float *)(node + 0x14);
            n0++;
        }
    }
    float   accum_before = *(float *)(list + 8);
    int32_t interval     = *(int32_t *)(behavior + 0x34);

    int64_t r = g_dmg_trampoline(behavior, entity);

    int64_t count_after = *(int64_t *)(list + 0x20);
    float   rate_after  = *(float *)(list + 0xc);
    int     emitted     = (count_before > 0 && count_after == 0) ? 1 : 0;
    if (emitted) g_dmg_surv_emit++;

    /* re-walk survivors AFTER (expired nodes were unlinked + freed; survivors remain) */
    static uint32_t key1[DTDMG_MAXEFF];
    static float    dur1[DTDMG_MAXEFF];
    int n1 = 0;
    if (sentinel) {
        for (int64_t node = *(int64_t *)sentinel;
             node && node != sentinel && n1 < DTDMG_MAXEFF; node = *(int64_t *)node) {
            key1[n1] = *(uint32_t *)(node + 0x10);
            dur1[n1] = *(float *)(node + 0x14);
            n1++;
        }
    }

    /* periodic survey flush — settles "is DamageTracking even exercised in this content?" */
    if (tick != g_dmg_surv_last && (tick & 0x1ffu) == 0) {       /* once per 512 ticks */
        g_dmg_surv_last = tick;
        char sv[200];
        snprintf(sv, sizeof sv,
            "DTDMGSURVEY\ttick=%u\tactive_entticks=%u\temit_entticks=%u\n",
            tick, g_dmg_surv_active, g_dmg_surv_emit);
        log_write(sv);
    }

    /* latch one active entity for ROUTINE per-tick sampling; release after STALE ticks. */
    if (!g_dmg_ent) {
        g_dmg_ent = entity;
    } else if (entity != g_dmg_ent &&
               (uint32_t)(tick - g_dmg_seentick) > DTDMG_STALE_TICKS) {
        g_dmg_ent = entity;
    }
    if (entity == g_dmg_ent) g_dmg_seentick = tick;

    /* Log detail when EITHER: (a) routine sample — the latched entity on a fresh tick, OR
     * (b) emitted — an empty-transition (the rare 0x2d CommandSink event) fired on ANY entity.
     * (b) bypasses the latch so we never miss the seam event we're hunting; an emitted entity
     * goes empty (count_after==0) so its next call early-returns at count_before<=0 → no dup. */
    int routine = (entity == g_dmg_ent && tick != g_dmg_lasttick);
    if (!routine && !emitted) return r;
    if (routine) g_dmg_lasttick = tick;

    /* tick rate DAT_140b0a340 (RVA 0xb0a340) — the rate-readout numerator hz. The decompile reads
     * it as (float)DAT_140b0a340, i.e. an INT32 cast to float (the value is 30), so read int32 then
     * cast — reading the raw bytes as a float would print ~0 (0x1e's bit pattern). Confirmed = 30 by
     * the 3 captured samples where rate==accum exactly with interval==30 (rate=hz*accum/iv). */
    float tick_hz = g_dt_imgbase ? (float)*(int32_t *)(g_dt_imgbase + 0xb0a340) : 0.0f;

    char s[256];
    snprintf(s, sizeof s,
        "DTDMG\ttick=%u\tent=%llx\tnb=%d\tna=%d\taccum=%.6f\trate=%.6f\tiv=%d\thz=%.6f\temit=%d\n",
        tick, (unsigned long long)entity, (int)count_before, (int)count_after,
        accum_before, rate_after, interval, tick_hz, emitted);
    log_write(s);

    for (int i = 0; i < n0; i++) {           /* pre-decay durations (recover decay offline) */
        char e[160];
        snprintf(e, sizeof e, "DTDMGEFF\ttick=%u\tph=pre\tidx=%d\tkey=%08x\tdur=%.6f\n",
                 tick, i, key0[i], dur0[i]);
        log_write(e);
    }
    for (int i = 0; i < n1; i++) {           /* post-decay survivor durations */
        char e[160];
        snprintf(e, sizeof e, "DTDMGEFF\ttick=%u\tph=post\tidx=%d\tkey=%08x\tdur=%.6f\n",
                 tick, i, key1[i], dur1[i]);
        log_write(e);
    }
    return r;
}

static BOOL install_b58bd80_hook(void)
{
    HMODULE exe = GetModuleHandleA(NULL);
    if (!exe) return FALSE;
    BYTE *fn = (BYTE *)exe + DMG_58BD80_RVA;
    if (memcmp(fn, dmg_58bd80_prologue, 17) != 0) {
        log_write("[eaw-mt] WARN: FUN_14058bd80 prologue mismatch — DTDMG not installed\n");
        return FALSE;
    }
    BYTE *tramp = (BYTE *)VirtualAlloc(NULL, 64, MEM_COMMIT|MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    if (!tramp) {
        log_write("[eaw-mt] WARN: VirtualAlloc for DTDMG trampoline failed\n");
        return FALSE;
    }
    memcpy(tramp, dmg_58bd80_prologue, 17);
    write_abs_jmp(tramp + 17, (uint64_t)(fn + 17));
    g_dmg_trampoline = (Dmg58bd80Fn)tramp;

    DWORD old;
    VirtualProtect(fn, 14, PAGE_EXECUTE_READWRITE, &old);
    write_abs_jmp(fn, (uint64_t)b58bd80_hook);
    VirtualProtect(fn, 14, old, &old);
    FlushInstructionCache(GetCurrentProcess(), fn, 14);
    log_write("[eaw-mt] DTDMG: FUN_14058bd80 DamageTracking decay hook installed\n");
    return TRUE;
}

/* =========================================================================
 * DTNRG — EnergyPool regen (FUN_14056c030) I/O capture.
 *
 * In-game oracle for the Phase-3 IN sim behavior #2 lift (sim/energy_pool.cpp).
 * EnergyPoolBehaviorClass::vfunc_6 is a PURE self-contained Phase-A regen ticker:
 *   value' = clamp(value + regen_rate*dt, 0, max_energy)
 * on its own pool sub-object (entity+0xf0)+0xf8, where regen_rate = template+0xddc
 * (template = entity+0x298), dt = FUN_140398010(entity,4) (per-tick game-speed/
 * difficulty time-scale), max_energy = FUN_140372320 = base(template+0xdd0)*(1+Σbuffs).
 * Guards: pool present + not disabled (pool+0x130 == 0). A drain gate (template+0x8b)
 * can force-zero the pool instead of regenning. (decomp/56c030.c.)
 *
 * Unlike DTDMG (whose 0x2d emit only fires at unit death = dormant), an energy pool
 * regenerates EVERY combat tick, so this oracle gets rich continuous data. We snapshot
 * the pool value BEFORE the trampoline and re-read it AFTER; the per-tick delta recovers
 * the game-speed scale offline as  scale = (value' - value) / regen_rate  (the DTDMG
 * technique — no need to call the world-coupled dt fn ourselves). The validation:
 *   (a) scale is a SINGLE shared scalar across ALL entities & ticks at a given game speed
 *       (every pool regens by rate_i * the same dt) — a genuine multi-sample cross-check;
 *   (b) when value sits at the base cap the delta is ~0 (upper clamp), and never < 0;
 *   (c) a disabled pool (pool+0x130 != 0) does not change (guarded no-op).
 * We also capture base(template+0xdd0) so buffed pools (value' > base) are filterable.
 *
 * The trampoline runs exactly once (no extra mutation -> no determinism perturbation);
 * only logging is gated + latched onto one active entity at a time.
 *
 * arg count = 3 (decomp/56c030.c): param_1=rcx behavior(this), param_2=rdx entity,
 * param_3=r8d mode-flag (int32). Returns undefined8 — preserved through the wrapper.
 * Prologue (objdump @14056c030): 40 56 / 48 83 ec 40 / 0f 29 74 24 30 / 48 8b f2 =
 * 2+4+5+3 = 14 bytes exactly (a 14-byte FF25 patch lands on the mov rsi,rdx boundary;
 * all push/sub/movaps/mov-reg — none RIP-relative; the two body `call`s are past +14,
 * reached via the trampoline tail). Own latch, independent of the other DT hooks.
 * Profile-build only; runtime-gated by EAW_DIFFTRACE=1.
 * ========================================================================= */
#define NRG_56C030_RVA     0x56c030ULL
#define DTNRG_STALE_TICKS  240u
static const BYTE nrg_56c030_prologue[14] = {
    0x40, 0x56,                    /* rex push rsi             */
    0x48, 0x83, 0xec, 0x40,        /* sub rsp, 0x40            */
    0x0f, 0x29, 0x74, 0x24, 0x30,  /* movaps [rsp+0x30], xmm6  */
    0x48, 0x8b, 0xf2,              /* mov rsi, rdx             */
};
typedef int64_t (*Nrg56c030Fn)(int64_t behavior, int64_t entity, int32_t mode);
static Nrg56c030Fn g_nrg_trampoline = NULL;
static int64_t  g_nrg_ent       = 0;
static uint32_t g_nrg_lasttick  = 0xFFFFFFFFu;
static uint32_t g_nrg_seentick  = 0;
static uint32_t g_nrg_pool_ticks = 0;          /* entity-ticks with a present, enabled pool   */
static uint32_t g_nrg_regen_ticks = 0;         /* entity-ticks where the value actually moved  */
static uint32_t g_nrg_interior_lines = 0;      /* emitted DTNRGINT lines (latch-bypass, capped)  */
static uint32_t g_nrg_surv_last  = 0xFFFFFFFFu;

static int64_t b56c030_hook(int64_t behavior, int64_t entity, int32_t mode)
{
    if (!g_dt_frame_ctr) {
        HMODULE exe = GetModuleHandleA(NULL);
        if (exe) { g_dt_frame_ctr = (volatile uint32_t *)((BYTE *)exe + 0xb0a320);
                   g_dt_imgbase = (uintptr_t)exe; }
    }
    if (!(entity && g_dt_frame_ctr && dt_on()))
        return g_nrg_trampoline(behavior, entity, mode);

    int64_t pool     = *(int64_t *)(entity + 0xf0);
    int64_t template_ = *(int64_t *)(entity + 0x298);
    if (!pool || !template_)
        return g_nrg_trampoline(behavior, entity, mode);

    uint32_t tick = *g_dt_frame_ctr;

    /* snapshot BEFORE */
    float   value_before = *(float *)(pool + 0xf8);
    uint8_t disabled     = *(uint8_t *)(pool + 0x130);
    float   regen_rate   = *(float *)(template_ + 0xddc);
    float   base_max     = *(float *)(template_ + 0xdd0);
    uint8_t drain_flag   = *(uint8_t *)(template_ + 0x8b);
    int32_t obj_id       = *(int32_t *)(entity + 0x50);
    int32_t sel_id       = g_dt_imgbase ? *(int32_t *)(g_dt_imgbase + 0xa286f0) : 0;
    int     selected     = (obj_id == sel_id) ? 1 : 0;

    g_nrg_pool_ticks++;

    int64_t r = g_nrg_trampoline(behavior, entity, mode);

    float value_after = *(float *)(pool + 0xf8);
    if (value_after != value_before) g_nrg_regen_ticks++;

    /* INTERIOR sample (any entity, bypasses the latch) — a clean regen tick where the new value
     * lands STRICTLY inside (0, base): the clamp is inactive so dt = (va-vb)/rate is recoverable
     * exactly. Latched samples are dominated by pools that refill to the cap each interval (va==base,
     * one-sided dt bound only); these interior landings (drain outpaced one regen step) are the ones
     * that pin the game-speed scale. Same "bypass the latch for the rare informative event" tactic as
     * DTDMG's emit. Capped so a pathological stream can't flood the log. */
    if (!disabled && !drain_flag && regen_rate > 0.001f &&
        value_after != value_before &&
        value_after > 0.001f && value_after < base_max - 0.001f &&
        g_nrg_interior_lines < 4000u) {
        g_nrg_interior_lines++;
        char in[320];
        snprintf(in, sizeof in,
            "DTNRGINT\ttick=%u\tent=%llx\tvb=%.6f\tva=%.6f\trate=%.6f\tbase=%.6f\n",
            tick, (unsigned long long)entity, value_before, value_after, regen_rate, base_max);
        log_write(in);
    }

    /* periodic survey — settles how often the regen path actually runs in this content */
    if (tick != g_nrg_surv_last && (tick & 0x1ffu) == 0) {       /* once per 512 ticks */
        g_nrg_surv_last = tick;
        char sv[220];
        snprintf(sv, sizeof sv,
            "DTNRGSURVEY\ttick=%u\tpool_entticks=%u\tregen_entticks=%u\tinterior=%u\n",
            tick, g_nrg_pool_ticks, g_nrg_regen_ticks, g_nrg_interior_lines);
        log_write(sv);
    }

    /* latch one entity for routine per-tick sampling; release after STALE ticks. */
    if (!g_nrg_ent) {
        g_nrg_ent = entity;
    } else if (entity != g_nrg_ent &&
               (uint32_t)(tick - g_nrg_seentick) > DTNRG_STALE_TICKS) {
        g_nrg_ent = entity;
    }
    if (entity == g_nrg_ent) g_nrg_seentick = tick;

    int routine = (entity == g_nrg_ent && tick != g_nrg_lasttick);
    if (!routine) return r;
    g_nrg_lasttick = tick;

    char s[320];
    snprintf(s, sizeof s,
        "DTNRG\ttick=%u\tent=%llx\tvb=%.6f\tva=%.6f\trate=%.6f\tbase=%.6f"
        "\tdis=%d\tdrain=%d\tsel=%d\tmode=%d\n",
        tick, (unsigned long long)entity, value_before, value_after,
        regen_rate, base_max, (int)disabled, (int)drain_flag, selected, (int)mode);
    log_write(s);
    return r;
}

static BOOL install_b56c030_hook(void)
{
    HMODULE exe = GetModuleHandleA(NULL);
    if (!exe) return FALSE;
    BYTE *fn = (BYTE *)exe + NRG_56C030_RVA;
    if (memcmp(fn, nrg_56c030_prologue, 14) != 0) {
        log_write("[eaw-mt] WARN: FUN_14056c030 prologue mismatch — DTNRG not installed\n");
        return FALSE;
    }
    BYTE *tramp = (BYTE *)VirtualAlloc(NULL, 64, MEM_COMMIT|MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    if (!tramp) {
        log_write("[eaw-mt] WARN: VirtualAlloc for DTNRG trampoline failed\n");
        return FALSE;
    }
    memcpy(tramp, nrg_56c030_prologue, 14);
    write_abs_jmp(tramp + 14, (uint64_t)(fn + 14));
    g_nrg_trampoline = (Nrg56c030Fn)tramp;

    DWORD old;
    VirtualProtect(fn, 14, PAGE_EXECUTE_READWRITE, &old);
    write_abs_jmp(fn, (uint64_t)b56c030_hook);
    VirtualProtect(fn, 14, old, &old);
    FlushInstructionCache(GetCurrentProcess(), fn, 14);
    log_write("[eaw-mt] DTNRG: FUN_14056c030 EnergyPool regen hook installed\n");
    return TRUE;
}

/* =========================================================================
 * DTABIL — in-game oracle for the lifted AbilityCountdown ticker (sim/ability_countdown.cpp,
 * from FUN_14042f910). The behavior is a self-contained INTEGER-tick cooldown/chargeup ticker over
 * the entity's own recharge manager at entity+0x1e8: a fixed 0x4d-slot array of {timer @+0x8+i*4,
 * chargeup-target @+0x13c+i*4, mode byte @+0x270+i (0=countdown / !=0=chargeup)}, last-serviced
 * tick @+0x2c0. Per tick delta = param_3(tick) - mgr->last_tick; COUNTDOWN floors at 0 (and on the
 * exact tick a slot reaches 0 emits sig 0x2c, gated), CHARGEUP ceils at the per-slot target.
 *
 * The oracle snapshots the slot timers BEFORE, runs the original once, snapshots AFTER, and logs
 * (per active slot) tb/ta/mode/target/delta so the lift can reproduce ta offline with NO scalar to
 * recover (pure integer arithmetic — no game-speed dt, unlike DTNRG). It also flags the cooldown→0
 * EDGE (the 0x2c emit trigger) from ANY entity (latch-bypass, like DTNRGINT), since edges are the
 * rare informative event. Trampoline runs exactly once (no determinism perturbation); only logging
 * is gated + latched onto one entity at a time. Profile-build only; runtime-gated by EAW_DIFFTRACE=1.
 *
 * arg count = 3 (decomp/42f910.c): param_1=rcx behavior(this), param_2=rdx entity, param_3=r8d
 * tick (the real sim tick the spine passes). Prologue (objdump @14042f910): 44 89 44 24 18 / 53 /
 * 41 54 / 41 55 / 41 56 / 48 83 ec 48 = 5+1+2+2+2+4 = 16 bytes (mov [rsp+0x18],r8d + 4 pushes + sub
 * rsp,0x48 — all relocatable; the body `call 0x4c2f70` is past +16, reached via the trampoline tail).
 * ========================================================================= */
#define ABIL_42F910_RVA    0x42f910ULL
#define ABIL_SLOTS         0x4d              /* 77 ability slots — the 42f910 loop bound */
static const BYTE abil_42f910_prologue[16] = {
    0x44, 0x89, 0x44, 0x24, 0x18,  /* mov [rsp+0x18], r8d */
    0x53,                          /* push rbx            */
    0x41, 0x54,                    /* push r12            */
    0x41, 0x55,                    /* push r13            */
    0x41, 0x56,                    /* push r14            */
    0x48, 0x83, 0xec, 0x48,        /* sub rsp, 0x48       */
};
typedef int64_t (*Abil42f910Fn)(int64_t behavior, int64_t entity, int32_t tick);
static Abil42f910Fn g_abil_trampoline = NULL;
static uint32_t g_abil_active_st = 0;           /* active slot-ticks (timer!=0 || mode!=0 || moved) */
static uint32_t g_abil_cd_st     = 0;           /* countdown-mode active slot-ticks                 */
static uint32_t g_abil_cu_st     = 0;           /* chargeup-mode active slot-ticks                  */
static uint32_t g_abil_edge_lines = 0;          /* emitted DTABILEDGE lines (cooldown→0), capped     */
static uint32_t g_abil_lines      = 0;          /* emitted DTABIL routine lines, capped              */
static uint32_t g_abil_surv_last  = 0xFFFFFFFFu;

static int64_t b42f910_hook(int64_t behavior, int64_t entity, int32_t tick_arg)
{
    if (!g_dt_frame_ctr) {
        HMODULE exe = GetModuleHandleA(NULL);
        if (exe) { g_dt_frame_ctr = (volatile uint32_t *)((BYTE *)exe + 0xb0a320);
                   g_dt_imgbase = (uintptr_t)exe; }
    }
    if (!(entity && dt_on()))
        return g_abil_trampoline(behavior, entity, tick_arg);

    int64_t mgr = *(int64_t *)(entity + 0x1e8);
    if (!mgr)
        return g_abil_trampoline(behavior, entity, tick_arg);

    uint32_t tick      = (uint32_t)tick_arg;
    int32_t  last_tick = *(int32_t *)(mgr + 0x2c0);
    int32_t  delta     = (int32_t)tick - last_tick;

    /* snapshot BEFORE: each slot's timer + mode + chargeup target */
    int32_t tb[ABIL_SLOTS]; uint8_t md[ABIL_SLOTS]; int32_t tgt[ABIL_SLOTS];
    for (int i = 0; i < ABIL_SLOTS; ++i) {
        tb[i]  = *(int32_t *)(mgr + 0x8   + (int64_t)i * 4);
        md[i]  = *(uint8_t *)(mgr + 0x270 + (int64_t)i);
        tgt[i] = *(int32_t *)(mgr + 0x13c + (int64_t)i * 4);
    }

    int64_t r = g_abil_trampoline(behavior, entity, tick_arg);

    /* snapshot AFTER + per-slot accounting / edge + routine logging. Routine lines are emitted for
     * every ACTIVE slot of ANY entity (latch-bypass, capped): a single-entity latch is unreliable
     * here because most entities' ability arrays are all-idle, so a stuck latch on an ability-less
     * unit (serviced every tick → never stale) yields zero data. The entity field on each line keeps
     * the per-ability trajectories distinguishable; the cap bounds total volume. */
    for (int i = 0; i < ABIL_SLOTS; ++i) {
        int32_t ta = *(int32_t *)(mgr + 0x8 + (int64_t)i * 4);
        int act = (tb[i] != 0 || md[i] != 0 || ta != tb[i]);
        if (!act) continue;
        g_abil_active_st++;
        if (md[i] == 0) g_abil_cd_st++; else g_abil_cu_st++;

        /* EDGE: a countdown slot that reached 0 this tick = the sig-0x2c emit trigger. The rarest
         * informative event — capped separately so it survives even if the routine cap is hit. */
        if (md[i] == 0 && tb[i] > 0 && ta == 0 && g_abil_edge_lines < 4000u) {
            g_abil_edge_lines++;
            char e[256];
            snprintf(e, sizeof e,
                "DTABILEDGE\ttick=%u\tent=%llx\tslot=%d\ttb=%d\tdelta=%d\n",
                tick, (unsigned long long)entity, i, tb[i], delta);
            log_write(e);
        }

        if (g_abil_lines < 20000u) {
            g_abil_lines++;
            char s[256];
            snprintf(s, sizeof s,
                "DTABIL\ttick=%u\tent=%llx\tslot=%d\tmode=%d\ttb=%d\tta=%d\ttgt=%d\tdelta=%d\n",
                tick, (unsigned long long)entity, i, (int)md[i], tb[i], ta, tgt[i], delta);
            log_write(s);
        }
    }

    /* periodic survey — settles how active the ability arrays are in this content */
    if (tick != g_abil_surv_last && (tick & 0x1ffu) == 0) {       /* once per 512 ticks */
        g_abil_surv_last = tick;
        char sv[256];
        snprintf(sv, sizeof sv,
            "DTABILSURVEY\ttick=%u\tactive_st=%u\tcd_st=%u\tcu_st=%u\tedges=%u\n",
            tick, g_abil_active_st, g_abil_cd_st, g_abil_cu_st, g_abil_edge_lines);
        log_write(sv);
    }
    return r;
}

static BOOL install_b42f910_hook(void)
{
    HMODULE exe = GetModuleHandleA(NULL);
    if (!exe) return FALSE;
    BYTE *fn = (BYTE *)exe + ABIL_42F910_RVA;
    if (memcmp(fn, abil_42f910_prologue, 16) != 0) {
        log_write("[eaw-mt] WARN: FUN_14042f910 prologue mismatch — DTABIL not installed\n");
        return FALSE;
    }
    BYTE *tramp = (BYTE *)VirtualAlloc(NULL, 64, MEM_COMMIT|MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    if (!tramp) {
        log_write("[eaw-mt] WARN: VirtualAlloc for DTABIL trampoline failed\n");
        return FALSE;
    }
    memcpy(tramp, abil_42f910_prologue, 16);
    write_abs_jmp(tramp + 16, (uint64_t)(fn + 16));
    g_abil_trampoline = (Abil42f910Fn)tramp;

    DWORD old;
    VirtualProtect(fn, 16, PAGE_EXECUTE_READWRITE, &old);
    write_abs_jmp(fn, (uint64_t)b42f910_hook);
    VirtualProtect(fn, 16, old, &old);
    FlushInstructionCache(GetCurrentProcess(), fn, 16);
    log_write("[eaw-mt] DTABIL: FUN_14042f910 AbilityCountdown hook installed\n");
    return TRUE;
}

/* =========================================================================
 * DTUAI — in-game oracle for the lifted UnitAI fog/sensor-visibility ticker (sim/unit_ai.cpp,
 * from FUN_1404f6070). Behavior #4 — the FIRST cross-entity behavior: reads the global object list
 * and writes a per-(observer,object) visibility matrix plus the SHARED sensor grid (DAT_140b31900).
 *
 * The deterministic core's cleanly-reproducible outputs (what this oracle validates bit-exact):
 *   (1) THROTTLE: ai+0x20 (next_due) advances by a FIXED +300 the tick it is due (next_due <= now)
 *       and the sensor mgr exists — never reset to now+300. So on any tick where next_due CHANGED,
 *       it must be exactly old+300 (the non-tautological PASS signal, like DTNRG's `va`).
 *   (2) TRANSFORM CHANGE-DETECT + RECACHE: the 12-float observer transform (entity+0x248..+0x274) is
 *       compared against the cache (ai+0x68..+0x94); on a refresh tick, if it changed the cache is
 *       recached to it (and object_moved fires), else the cache is untouched.
 * Plus it CHARACTERISES the cross-boundary Block-C visibility matrix (not bit-exact reproducible — the
 * bit VALUE is the env LOS predicate's output): the per-(observer) bit array at ai+0x58 (count ai+0x60),
 * its set-count before/after, and the edge counts e01/e10 = exactly the number of became_visible /
 * became_hidden notifies the core would push into the shared grid this tick. fog/sensor gates are
 * logged so the analyzer can check the clear-path (fog off => all bits 0) and notify suppression.
 *
 * Snapshots BEFORE, runs the original once (trampoline — no determinism perturbation), snapshots
 * AFTER. Logs DTUAI lines for refresh ticks (throttle gold) + a sparse steady-state sample, capped.
 *
 * arg count = 3 (uniform vfunc_6 call site: rcx=behavior, rdx=entity, r8d=tick — the body ignores the
 * tick param and reads now from game[2], but we keep the 3-arg shape so the call frame matches). The
 * ai manager = *(*(behavior+0x28)+0x108); the observer transform is read from entity(rdx)=param_2.
 * Prologue (PE @1404f6070): 40 56 / 57 / 41 56 / 48 83 ec 20 / 48 8b f2 / 4c 8b f1 = 2+1+2+4+3+3 = 15
 * bytes (push rsi/rdi/r14 + sub rsp,0x20 + mov rsi,rdx + mov r14,rcx — all position-independent),
 * ending cleanly before the relative `call <spine>` at +15 (reached via the trampoline tail).
 * ========================================================================= */
#define UAI_4F6070_RVA   0x4f6070ULL
#define UAI_MAXVIS       4096            /* cap on the per-observer vis-array digest scan */
static const BYTE uai_4f6070_prologue[15] = {
    0x40, 0x56,                    /* push rsi      */
    0x57,                          /* push rdi      */
    0x41, 0x56,                    /* push r14      */
    0x48, 0x83, 0xec, 0x20,        /* sub rsp, 0x20 */
    0x48, 0x8b, 0xf2,              /* mov rsi, rdx  */
    0x4c, 0x8b, 0xf1,              /* mov r14, rcx  */
};
typedef int64_t (*Uai4f6070Fn)(int64_t behavior, int64_t entity, int32_t tick);
static Uai4f6070Fn g_uai_trampoline = NULL;
static uint32_t g_uai_lines      = 0;          /* emitted DTUAI lines, capped                        */
static uint32_t g_uai_ticks      = 0;          /* observer-ticks the hook logged-eligible            */
static uint32_t g_uai_advances   = 0;          /* throttle advances seen (next_due += 300)           */
static uint32_t g_uai_bad_adv    = 0;          /* advances where nda != ndb+300 (model violation)    */
static uint32_t g_uai_e01        = 0;          /* total became_visible edges                         */
static uint32_t g_uai_e10        = 0;          /* total became_hidden edges                          */
static uint32_t g_uai_fogoff     = 0;          /* fog-off (clear-path) observer-ticks                */
static uint32_t g_uai_cachebad   = 0;          /* cache post-condition violations                    */
static uint32_t g_uai_surv_last  = 0xFFFFFFFFu;

static int popcnt_bytes(const uint8_t *p, int n) { int c = 0; for (int i = 0; i < n; ++i) c += (p[i] != 0); return c; }

static int64_t b4f6070_hook(int64_t behavior, int64_t entity, int32_t tick_arg)
{
    if (!g_dt_frame_ctr) {
        HMODULE exe = GetModuleHandleA(NULL);
        if (exe) { g_dt_frame_ctr = (volatile uint32_t *)((BYTE *)exe + 0xb0a320);
                   g_dt_imgbase = (uintptr_t)exe; }
    }
    if (!(behavior && g_dt_imgbase && dt_on()))
        return g_uai_trampoline(behavior, entity, tick_arg);

    int64_t owner = *(int64_t *)(behavior + 0x28);
    if (!owner) return g_uai_trampoline(behavior, entity, tick_arg);
    int64_t ai = *(int64_t *)(owner + 0x108);
    if (!ai) return g_uai_trampoline(behavior, entity, tick_arg);

    /* global state the throttle / fog gates read */
    int64_t game   = *(int64_t *)(g_dt_imgbase + 0xb15418);
    int32_t now    = game ? *(int32_t *)(game + 0x10) : 0;
    int     sensor = (*(int64_t *)(g_dt_imgbase + 0xb31900)) != 0;
    int8_t  fogg   = *(int8_t *)(g_dt_imgbase + 0xa284c4);
    int8_t  h34b   = *(int8_t *)(owner + 0x34b);
    int8_t  h348   = *(int8_t *)(owner + 0x348);
    int     fog    = (fogg != 0) && !((h34b == -1) && (h348 == -1));   /* sweep vs clear path */

    /* BEFORE snapshot: throttle + transform cache + transform + vis-array digest */
    int32_t ndb     = *(int32_t *)(ai + 0x20);
    int32_t initb   = *(int32_t *)(ai + 0x18);
    float   cacheb[12], xform[12];
    for (int i = 0; i < 12; ++i) {
        cacheb[i] = *(float *)(ai + 0x68 + (int64_t)i * 4);
        xform[i]  = *(float *)(entity + 0x248 + (int64_t)i * 4);
    }
    int     chg = 0;
    for (int i = 0; i < 12; ++i) { if (cacheb[i] != xform[i]) { chg = 1; break; } }

    int64_t vdata = *(int64_t *)(ai + 0x58);
    int32_t vcnt  = *(int32_t *)(ai + 0x60);
    int     scan  = (vdata && vcnt > 0) ? (vcnt < UAI_MAXVIS ? vcnt : UAI_MAXVIS) : 0;
    uint8_t vbits[UAI_MAXVIS];
    for (int i = 0; i < scan; ++i) vbits[i] = *(uint8_t *)(vdata + i);
    int vb = popcnt_bytes(vbits, scan);

    int64_t r = g_uai_trampoline(behavior, entity, tick_arg);

    /* AFTER snapshot */
    int32_t nda   = *(int32_t *)(ai + 0x20);
    int32_t va = 0, e01 = 0, e10 = 0;
    int64_t vdata2 = *(int64_t *)(ai + 0x58);     /* array may have been (re)allocated on first pass */
    int32_t vcnt2  = *(int32_t *)(ai + 0x60);
    int     scan2  = (vdata2 && vcnt2 > 0) ? (vcnt2 < UAI_MAXVIS ? vcnt2 : UAI_MAXVIS) : 0;
    int     emin   = (scan2 < scan) ? scan2 : scan;
    for (int i = 0; i < scan2; ++i) {
        uint8_t a = *(uint8_t *)(vdata2 + i);
        va += (a != 0);
        if (i < emin) {
            if (!vbits[i] && a) e01++;
            else if (vbits[i] && !a) e10++;
        }
    }
    /* cache post-condition: on an advance, recached-to-xform iff changed; else cache untouched */
    int advanced = (nda != ndb);
    int cacheok = 1;
    for (int i = 0; i < 12; ++i) {
        float ca = *(float *)(ai + 0x68 + (int64_t)i * 4);
        float want = (advanced && chg) ? xform[i] : cacheb[i];
        if (ca != want) { cacheok = 0; break; }
    }

    /* accounting */
    g_uai_ticks++;
    if (advanced) { g_uai_advances++; if (nda != ndb + 300) g_uai_bad_adv++; }
    if (!cacheok) g_uai_cachebad++;
    if (!fog) g_uai_fogoff++;
    g_uai_e01 += (uint32_t)e01;
    g_uai_e10 += (uint32_t)e10;

    /* emit a line for refresh ticks (the throttle gold) + a sparse steady-state sample, capped */
    int want_line = advanced || (e01 | e10) || ((now & 0xffu) == 0);
    if (want_line && g_uai_lines < 20000u) {
        g_uai_lines++;
        char s[384];
        snprintf(s, sizeof s,
            "DTUAI\ttick=%u\tent=%llx\tndb=%d\tnda=%d\tnow=%d\tinit=%d\tchg=%d\tcacheok=%d"
            "\tvcnt=%d\tvb=%d\tva=%d\te01=%d\te10=%d\tfog=%d\tsen=%d\n",
            (unsigned)now, (unsigned long long)owner, ndb, nda, now, initb, chg, cacheok,
            vcnt, vb, va, e01, e10, fog, sensor);
        log_write(s);
    }

    if (now != (int32_t)g_uai_surv_last && ((uint32_t)now & 0x1ffu) == 0) {     /* once per 512 ticks */
        g_uai_surv_last = (uint32_t)now;
        char sv[256];
        snprintf(sv, sizeof sv,
            "DTUAISURVEY\ttick=%u\tobs_ticks=%u\tadvances=%u\tbad_adv=%u\tcache_bad=%u"
            "\te01=%u\te10=%u\tfogoff=%u\n",
            (unsigned)now, g_uai_ticks, g_uai_advances, g_uai_bad_adv, g_uai_cachebad,
            g_uai_e01, g_uai_e10, g_uai_fogoff);
        log_write(sv);
    }
    return r;
}

static BOOL install_b4f6070_hook(void)
{
    HMODULE exe = GetModuleHandleA(NULL);
    if (!exe) return FALSE;
    BYTE *fn = (BYTE *)exe + UAI_4F6070_RVA;
    if (memcmp(fn, uai_4f6070_prologue, 15) != 0) {
        log_write("[eaw-mt] WARN: FUN_1404f6070 prologue mismatch — DTUAI not installed\n");
        return FALSE;
    }
    BYTE *tramp = (BYTE *)VirtualAlloc(NULL, 64, MEM_COMMIT|MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    if (!tramp) {
        log_write("[eaw-mt] WARN: VirtualAlloc for DTUAI trampoline failed\n");
        return FALSE;
    }
    memcpy(tramp, uai_4f6070_prologue, 15);
    write_abs_jmp(tramp + 15, (uint64_t)(fn + 15));
    g_uai_trampoline = (Uai4f6070Fn)tramp;

    DWORD old;
    VirtualProtect(fn, 15, PAGE_EXECUTE_READWRITE, &old);
    write_abs_jmp(fn, (uint64_t)b4f6070_hook);
    VirtualProtect(fn, 15, old, &old);
    FlushInstructionCache(GetCurrentProcess(), fn, 15);
    log_write("[eaw-mt] DTUAI: FUN_1404f6070 UnitAI visibility hook installed\n");
    return TRUE;
}

/* =========================================================================
 * DTNEB — in-game oracle for the lifted Nebula effect-ramp ticker (sim/nebula.cpp, from
 * FUN_140437b60). Behavior #5 — the "clean float integrator": its per-tick STAGE 1 is a semi-implicit
 * damped harmonic oscillator that runs UNCONDITIONALLY, so the deterministic core is continuously
 * observable (like EnergyPool #2, but a 2nd-order spring). The state lives on the sub-object at
 * entity+0xf0: value(+0x11c), equilibrium(+0x120), velocity(+0x124), freq(+0x12c), enter_tick(+0x104).
 *
 * What this oracle validates bit-exact: the SPRING STEP. Using the BEFORE state and the per-tick game
 * dt (DAT_140b0a344), the lifted math
 *     w=freq*2; wdt=w*dt; dx=value-eq; inv=1/(1+wdt+0.48*wdt^2+0.235*wdt^3); tmp=(dx*w+vel)*dt;
 *     value'=(tmp+dx)*inv+eq;  velocity'=(vel-tmp*w)*inv
 * must reproduce the AFTER value'/velocity' EXACTLY — EXCEPT on a LINGER-pin tick, where STAGE 2
 * overwrites value->1, velocity->0, eq->1 (only when eq<1). Those are detected and counted separately
 * (the spring still ran first; its output is just clobbered). It also CHARACTERISES the membership/
 * throttle: enter edges (enter_tick -1 -> now), leave edges (-> -1), and the per-entry LINGER window.
 *
 * Snapshots BEFORE, runs the original once (trampoline — no determinism perturbation), snapshots AFTER.
 * Raw fields are logged so the Python analyzer can re-reproduce the spring in float32 and characterise
 * the throttle; an inline spring-mismatch counter gives an immediate PASS signal (cf. DTUAI bad_adv).
 *
 * arg count = 3 (uniform vfunc_6 call site: rcx=behavior, rdx=entity, r8d=tick — the nebula body uses
 * only behavior+entity, but we keep the 3-arg shape so the call frame matches). The sub-object is read
 * from entity(rdx)=param_2; dt/tick are the clock-region globals (DAT_140b0a344 / DAT_140b0a320).
 * Prologue (PE @140437b60): 48 8b c4 / 48 89 58 08 / 48 89 70 18 / 48 89 78 20 = 3+4+4+4 = 15 bytes
 * (mov rax,rsp + the MSVC home-register saves — all position-independent), ending cleanly before the
 * push rbp at +15 (reached via the trampoline tail).
 * ========================================================================= */
#define NEB_437B60_RVA   0x437b60ULL
static const BYTE neb_437b60_prologue[15] = {
    0x48, 0x8b, 0xc4,              /* mov rax, rsp        */
    0x48, 0x89, 0x58, 0x08,        /* mov [rax+8], rbx    */
    0x48, 0x89, 0x70, 0x18,        /* mov [rax+0x18], rsi */
    0x48, 0x89, 0x78, 0x20,        /* mov [rax+0x20], rdi */
};
typedef int64_t (*Neb437b60Fn)(int64_t behavior, int64_t entity, int32_t tick);
static Neb437b60Fn g_neb_trampoline = NULL;
static uint32_t g_neb_lines       = 0;      /* emitted DTNEB lines, capped                        */
static uint32_t g_neb_ticks       = 0;      /* nebula-entity-ticks the hook processed             */
static uint32_t g_neb_spring_ok   = 0;      /* ticks the lifted spring reproduced bit-exact        */
static uint32_t g_neb_spring_bad  = 0;      /* ticks the spring MISMATCHED (model violation)       */
static uint32_t g_neb_linger_pins = 0;      /* LINGER-pin ticks (value clobbered to 1; spring n/a) */
static uint32_t g_neb_enters      = 0;      /* enter_tick -1 -> now edges                          */
static uint32_t g_neb_leaves      = 0;      /* enter_tick set -> -1 edges                          */
static uint32_t g_neb_active      = 0;      /* ticks where eq!=0 or value!=0 (spring non-trivial)  */
static uint32_t g_neb_surv_last   = 0xFFFFFFFFu;

static int neb_biteq(float a, float b) { return memcmp(&a, &b, sizeof(float)) == 0; }

static int64_t b437b60_hook(int64_t behavior, int64_t entity, int32_t tick_arg)
{
    if (!g_dt_frame_ctr) {
        HMODULE exe = GetModuleHandleA(NULL);
        if (exe) { g_dt_frame_ctr = (volatile uint32_t *)((BYTE *)exe + 0xb0a320);
                   g_dt_imgbase = (uintptr_t)exe; }
    }
    if (!(entity && g_dt_imgbase && dt_on()))
        return g_neb_trampoline(behavior, entity, tick_arg);

    int64_t sub = *(int64_t *)(entity + 0xf0);
    if (!sub) return g_neb_trampoline(behavior, entity, tick_arg);

    float   dt   = *(float   *)(g_dt_imgbase + 0xb0a344);
    int32_t tick = *(int32_t *)(g_dt_imgbase + 0xb0a320);

    /* BEFORE snapshot */
    float   vb   = *(float   *)(sub + 0x11c);   /* value       */
    float   eqb  = *(float   *)(sub + 0x120);   /* equilibrium */
    float   velb = *(float   *)(sub + 0x124);   /* velocity    */
    float   frq  = *(float   *)(sub + 0x12c);   /* freq        */
    int32_t etb  = *(int32_t *)(sub + 0x104);   /* enter_tick  */

    int64_t r = g_neb_trampoline(behavior, entity, tick_arg);

    /* AFTER snapshot */
    float   va   = *(float   *)(sub + 0x11c);
    float   eqa  = *(float   *)(sub + 0x120);
    float   vela = *(float   *)(sub + 0x124);
    int32_t eta  = *(int32_t *)(sub + 0x104);

    /* reproduce STAGE 1 with the lifted math, using the BEFORE equilibrium (op order = sim/nebula.cpp) */
    float w   = frq * 2.0f;
    float wdt = w * dt;
    float dx  = vb - eqb;
    float inv = 1.0f / (wdt * 0.48f * wdt + wdt + 1.0f + wdt * 0.235f * wdt * wdt);
    float tmp = (dx * w + velb) * dt;
    float pred_v   = (tmp + dx) * inv + eqb;
    float pred_vel = (velb - tmp * w) * inv;

    /* LINGER pin: STAGE 2 clobbered value->1, velocity->0, eq->1 (only fires when eq was < 1) */
    int linger_pin = (eqb < 1.0f) && neb_biteq(va, 1.0f) && neb_biteq(vela, 0.0f) && neb_biteq(eqa, 1.0f);
    int spring_ok  = linger_pin ? 1 : (neb_biteq(va, pred_v) && neb_biteq(vela, pred_vel));

    int enter_edge = (etb == -1) && (eta != -1);
    int leave_edge = (etb != -1) && (eta == -1);
    int active     = (eqb != 0.0f) || (vb != 0.0f);

    /* accounting */
    g_neb_ticks++;
    if (linger_pin)        g_neb_linger_pins++;
    else if (spring_ok)    g_neb_spring_ok++;
    else                   g_neb_spring_bad++;
    if (enter_edge)        g_neb_enters++;
    if (leave_edge)        g_neb_leaves++;
    if (active)            g_neb_active++;

    /* emit a line for: spring-active ticks, membership edges, linger pins, mismatches, + a sparse
     * sample — capped. Raw fields let the analyzer re-reproduce the spring in float32. */
    int want_line = active || enter_edge || leave_edge || linger_pin || !spring_ok || ((tick & 0x3ffu) == 0);
    if (want_line && g_neb_lines < 20000u) {
        g_neb_lines++;
        char s[448];
        snprintf(s, sizeof s,
            "DTNEB\ttick=%d\tent=%llx\tdt=%.9g\tfreq=%.9g\tvb=%.9g\teqb=%.9g\tvelb=%.9g\tetb=%d"
            "\tva=%.9g\teqa=%.9g\tvela=%.9g\teta=%d\tpv=%.9g\tpvel=%.9g\tspring_ok=%d\tpin=%d"
            "\tenter=%d\tleave=%d\n",
            tick, (unsigned long long)entity, (double)dt, (double)frq,
            (double)vb, (double)eqb, (double)velb, etb,
            (double)va, (double)eqa, (double)vela, eta,
            (double)pred_v, (double)pred_vel, spring_ok, linger_pin, enter_edge, leave_edge);
        log_write(s);
    }

    if (tick != (int32_t)g_neb_surv_last && ((uint32_t)tick & 0x1ffu) == 0) {   /* once per 512 ticks */
        g_neb_surv_last = (uint32_t)tick;
        char sv[256];
        snprintf(sv, sizeof sv,
            "DTNEBSURVEY\ttick=%d\tticks=%u\tspring_ok=%u\tspring_bad=%u\tpins=%u"
            "\tenters=%u\tleaves=%u\tactive=%u\n",
            tick, g_neb_ticks, g_neb_spring_ok, g_neb_spring_bad, g_neb_linger_pins,
            g_neb_enters, g_neb_leaves, g_neb_active);
        log_write(sv);
    }
    return r;
}

static BOOL install_b437b60_hook(void)
{
    HMODULE exe = GetModuleHandleA(NULL);
    if (!exe) return FALSE;
    BYTE *fn = (BYTE *)exe + NEB_437B60_RVA;
    if (memcmp(fn, neb_437b60_prologue, 15) != 0) {
        log_write("[eaw-mt] WARN: FUN_140437b60 prologue mismatch — DTNEB not installed\n");
        return FALSE;
    }
    BYTE *tramp = (BYTE *)VirtualAlloc(NULL, 64, MEM_COMMIT|MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    if (!tramp) {
        log_write("[eaw-mt] WARN: VirtualAlloc for DTNEB trampoline failed\n");
        return FALSE;
    }
    memcpy(tramp, neb_437b60_prologue, 15);
    write_abs_jmp(tramp + 15, (uint64_t)(fn + 15));
    g_neb_trampoline = (Neb437b60Fn)tramp;

    DWORD old;
    VirtualProtect(fn, 15, PAGE_EXECUTE_READWRITE, &old);
    write_abs_jmp(fn, (uint64_t)b437b60_hook);
    VirtualProtect(fn, 15, old, &old);
    FlushInstructionCache(GetCurrentProcess(), fn, 15);
    log_write("[eaw-mt] DTNEB: FUN_140437b60 Nebula effect-ramp hook installed\n");
    return TRUE;
}

/* =========================================================================
 * DTAST — in-game oracle for the lifted AsteroidFieldDamage probabilistic damage trigger
 * (sim/asteroid_field_damage.cpp, from FUN_140437310). Behavior #6 — the SIBLING of Nebula #5
 * (adjacent RVA, same sub-object at entity+0xf0). Per tick: gate (mode/predicate/config) -> sphere
 * scan -> per in-range asteroid-field object draw one sim-LCG roll (range_f(0,1)) and apply collision
 * damage if roll <= prob (DAT_140b16d64); stamp sub+0x10c = now if any found, else -1. apply (436920)
 * damages self + spawns fx into the GOM + draws MORE RNG (a float + an int, with rejection).
 *
 * Unlike Nebula's spring, the deterministic core here is the sim-RNG + the proximity stamp, so the
 * oracle validates two things across the BEFORE/AFTER snapshot of the trampoline:
 *   (1) PROXIMITY BICONDITIONAL — every "stamp now" is preceded by >=1 roll and vice-versa, so
 *       (seed advanced this tick)  <=>  (sub+0x10c == now). A violation means the +0x10c SM and the
 *       RNG consumption disagree (model break). Counts bicond_ok/bad.
 *   (2) LCG RECURRENCE + DRAW COUNT — brute-force forward-step the lifted recurrence
 *       s' = s*0x41c64e6d + 0xbdf from seed_before; the number of steps k to reach seed_after is the
 *       per-tick draw count (membership-dependent: the determinism-contract quantity). k found within
 *       the bound => lcg_ok (the recurrence + constants match the live seed transition); else lcg_bad.
 * The first gate roll is reproduced (range_f(seed_before,0,1)) and logged for spot inspection (the
 * apply decision itself is env — 436920's self-damage + spawn — and not externally observable, so the
 * roll VALUE and the k draw-count are what we validate, not the per-asteroid apply count).
 *
 * arg count = 3 (uniform vfunc_6 call site rcx/rdx/r8d; body uses behavior=rcx, entity=rdx). Globals:
 * sim-RNG seed DAT_140a13e24 (imgbase+0xa13e24), probability DAT_140b16d64 (imgbase+0xb16d64), tick
 * counter DAT_140b0a320. Prologue (PE @140437310): 48 89 5c 24 18 / 55 / 56 / 41 55 / 41 56 / 41 57 /
 * 48 8d 6c 24 c9 = 5+1+1+2+2+2+5 = 18 bytes (mov-home + 5 pushes + lea rbp — all position-independent),
 * cut before the SUB RSP at +0x12 (reached via the trampoline tail).
 * ========================================================================= */
#define AST_437310_RVA   0x437310ULL
static const BYTE ast_437310_prologue[18] = {
    0x48, 0x89, 0x5c, 0x24, 0x18,  /* mov [rsp+0x18], rbx */
    0x55,                          /* push rbp            */
    0x56,                          /* push rsi            */
    0x41, 0x55,                    /* push r13            */
    0x41, 0x56,                    /* push r14            */
    0x41, 0x57,                    /* push r15            */
    0x48, 0x8d, 0x6c, 0x24, 0xc9,  /* lea rbp, [rsp-0x37] */
};
typedef int64_t (*Ast437310Fn)(int64_t behavior, int64_t entity, int32_t tick);
static Ast437310Fn g_ast_trampoline = NULL;
static uint32_t g_ast_lines      = 0;   /* emitted DTAST lines, capped                            */
static uint32_t g_ast_ticks      = 0;   /* asteroid-field-entity-ticks the hook processed         */
static uint32_t g_ast_active     = 0;   /* ticks that stamped now (>=1 asteroid in range)         */
static uint32_t g_ast_clear      = 0;   /* ticks with sub+0x10c == -1 after (no asteroid found)   */
static uint32_t g_ast_bicond_ok  = 0;   /* (seed advanced) == (stamp now) held                    */
static uint32_t g_ast_bicond_bad = 0;   /* biconditional VIOLATED (model break)                   */
static uint32_t g_ast_lcg_ok     = 0;   /* seed transition reproduced by k forward LCG steps      */
static uint32_t g_ast_lcg_bad    = 0;   /* seed transition NOT reachable within the step bound    */
static uint32_t g_ast_draw_min   = 0xFFFFFFFFu; /* min per-tick LCG draw count (over advanced ticks)*/
static uint32_t g_ast_draw_max   = 0;   /* max per-tick LCG draw count                            */
static uint32_t g_ast_surv_last  = 0xFFFFFFFFu;

static int64_t b437310_hook(int64_t behavior, int64_t entity, int32_t tick_arg)
{
    if (!g_dt_frame_ctr) {
        HMODULE exe = GetModuleHandleA(NULL);
        if (exe) { g_dt_frame_ctr = (volatile uint32_t *)((BYTE *)exe + 0xb0a320);
                   g_dt_imgbase = (uintptr_t)exe; }
    }
    if (!(entity && g_dt_imgbase && dt_on()))
        return g_ast_trampoline(behavior, entity, tick_arg);

    int64_t sub = *(int64_t *)(entity + 0xf0);
    if (!sub) return g_ast_trampoline(behavior, entity, tick_arg);

    int32_t  now  = *(int32_t  *)(g_dt_imgbase + 0xb0a320);
    float    prob = *(float    *)(g_dt_imgbase + 0xb16d64);

    /* BEFORE snapshot */
    uint32_t seed_b = *(volatile uint32_t *)(g_dt_imgbase + 0xa13e24);
    int32_t  tickb  = *(int32_t  *)(sub + 0x10c);

    int64_t r = g_ast_trampoline(behavior, entity, tick_arg);

    /* AFTER snapshot */
    uint32_t seed_a = *(volatile uint32_t *)(g_dt_imgbase + 0xa13e24);
    int32_t  ticka  = *(int32_t  *)(sub + 0x10c);

    int advanced  = (seed_a != seed_b);
    int stamp_now = (ticka == now);
    int bicond_ok = (advanced == stamp_now);

    /* brute-force the LCG draw count: forward-step the lifted recurrence to seed_after */
    uint32_t k = 0, s = seed_b;
    if (advanced) {
        for (k = 1; k <= 256u; ++k) {
            s = s * 0x41c64e6du + 0xbdfu;
            if (s == seed_a) break;
        }
    }
    int lcg_ok = !advanced || (k <= 256u);

    /* reproduce the FIRST gate roll from seed_before (range_f(0,1) = (field/32767) in [0,1)) */
    uint32_t s1 = seed_b * 0x41c64e6du + 0xbdfu;
    float roll0 = (float)(uint32_t)((s1 >> 10) & 0x7fffu) / 32767.0f;
    int apply0_pred = roll0 <= prob;   /* predicted apply on asteroid 0 (env-only; not observable) */

    /* accounting */
    g_ast_ticks++;
    if (stamp_now)       g_ast_active++;
    if (ticka == -1)     g_ast_clear++;
    if (bicond_ok)       g_ast_bicond_ok++; else g_ast_bicond_bad++;
    if (advanced) {
        if (lcg_ok)      g_ast_lcg_ok++; else g_ast_lcg_bad++;
        if (lcg_ok) {
            if (k < g_ast_draw_min) g_ast_draw_min = k;
            if (k > g_ast_draw_max) g_ast_draw_max = k;
        }
    }

    int want_line = advanced || stamp_now || !bicond_ok || (ticka != tickb) || ((now & 0x3ffu) == 0);
    if (want_line && g_ast_lines < 20000u) {
        g_ast_lines++;
        char ln[384];
        snprintf(ln, sizeof ln,
            "DTAST\ttick=%d\tent=%llx\tseedb=%08x\tseeda=%08x\tk=%u\tprob=%.9g\troll0=%.9g"
            "\tapply0=%d\ttickb=%d\tticka=%d\tnow=%d\tadv=%d\tstampnow=%d\tbicond=%d\tlcg=%d\n",
            now, (unsigned long long)entity, seed_b, seed_a, k, (double)prob, (double)roll0,
            apply0_pred, tickb, ticka, now, advanced, stamp_now, bicond_ok, lcg_ok);
        log_write(ln);
    }

    if (now != (int32_t)g_ast_surv_last && ((uint32_t)now & 0x1ffu) == 0) {   /* once per 512 ticks */
        g_ast_surv_last = (uint32_t)now;
        char sv[256];
        snprintf(sv, sizeof sv,
            "DTASTSURVEY\ttick=%d\tticks=%u\tactive=%u\tclear=%u\tbicond_ok=%u\tbicond_bad=%u"
            "\tlcg_ok=%u\tlcg_bad=%u\tdraw_min=%u\tdraw_max=%u\n",
            now, g_ast_ticks, g_ast_active, g_ast_clear, g_ast_bicond_ok, g_ast_bicond_bad,
            g_ast_lcg_ok, g_ast_lcg_bad,
            (g_ast_draw_min == 0xFFFFFFFFu ? 0u : g_ast_draw_min), g_ast_draw_max);
        log_write(sv);
    }
    return r;
}

static BOOL install_b437310_hook(void)
{
    HMODULE exe = GetModuleHandleA(NULL);
    if (!exe) return FALSE;
    BYTE *fn = (BYTE *)exe + AST_437310_RVA;
    if (memcmp(fn, ast_437310_prologue, 18) != 0) {
        log_write("[eaw-mt] WARN: FUN_140437310 prologue mismatch — DTAST not installed\n");
        return FALSE;
    }
    BYTE *tramp = (BYTE *)VirtualAlloc(NULL, 64, MEM_COMMIT|MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    if (!tramp) {
        log_write("[eaw-mt] WARN: VirtualAlloc for DTAST trampoline failed\n");
        return FALSE;
    }
    memcpy(tramp, ast_437310_prologue, 18);
    write_abs_jmp(tramp + 18, (uint64_t)(fn + 18));
    g_ast_trampoline = (Ast437310Fn)tramp;

    DWORD old;
    VirtualProtect(fn, 18, PAGE_EXECUTE_READWRITE, &old);
    write_abs_jmp(fn, (uint64_t)b437310_hook);
    VirtualProtect(fn, 18, old, &old);
    FlushInstructionCache(GetCurrentProcess(), fn, 18);
    log_write("[eaw-mt] DTAST: FUN_140437310 AsteroidFieldDamage hook installed\n");
    return TRUE;
}

/* =========================================================================
 * DTARG — in-game oracle for the lifted Targeting game-mode DISPATCHER
 * (sim/targeting_dispatch.cpp, from FUN_140633a30). Behavior #7 (part 1).
 *
 * Targeting is the AI fire-control orchestrator; 0x633a30 is its thin dispatcher: run the base
 * LocomotorCommonClass::vfunc_6 (FUN_1404c2f70 — a 15-byte stub that only writes behavior+0x30, so it
 * touches NEITHER the entity NOR the controller), resolve a mode int, then route mode 1 -> FUN_140634810
 * (individual targeting), mode 2 -> FUN_140633ae0 (team targeting), else NoOp (return 0x650000). Mode
 * resolution: controller = entity->get_component(1) (entity vtbl +0x10); controller present ->
 * controller->control_mode() (vtbl +0x238); else DAT_140b15418 present ? game-speed mode (vtbl +0xe0)
 * : default DAT_140b153fc.
 *
 * The dispatcher core is a trivial integer decision, so the oracle validates it as a BICONDITIONAL
 * predicted == actual:
 *   * PREDICTED: the hook replicates the resolution (reads the mode the SAME way the dispatcher does —
 *     faithful because the base vfunc_6 runs after this read but cannot change the entity/controller)
 *     and maps it: pred = mode==1?1 : mode==2?2 : 0(NoOp).
 *   * ACTUAL: tiny entry MARKERS on FUN_140634810 / FUN_140633ae0 bump per-impl run counters; snapshot
 *     them BEFORE/AFTER the dispatcher trampoline -> the delta says which sub-impl actually ran
 *     (d1 -> Mode1, d2 -> Mode2, neither -> NoOp). `multi` (both ran) is an invariant violation.
 * bicond holds iff pred == act. The oracle also reports the empirical DISTRIBUTION (which source feeds
 * the mode, and how often mode 1 vs 2 vs other occurs in live combat — useful for prioritising the
 * deferred sub-body lifts).
 *
 * arg count = 3 (uniform vfunc_6 call site rcx=behavior, rdx=entity, r8d=tick). Prologues (PE):
 *   633a30 dispatcher: 48 89 5c 24 08 / 48 89 6c 24 10 / 48 89 74 24 18 / 57 / 48 83 ec 20 / 41 8b e8 /
 *     48 8b f2 / 48 8b f9 = 29 bytes, cut BEFORE the E8 CALL (LocomotorCommonClass::vfunc_6) at +0x1d.
 *   634810 mode1: 44 89 44 24 18 / 55 / 53 / 56 / 57 / 41 54 / 41 55 / 41 56 / 41 57 / 48 8d 6c 24 e1
 *     = 22 bytes, cut at +0x16 (before SUB RSP); 633ae0 mode2: 44 89 44 24 18 / 55 / 56 / 57 / 41 56 /
 *     41 57 / 48 8d 6c 24 e0 = 17 bytes, cut at +0x11. All position-independent (no rel call/jmp copied).
 * ========================================================================= */
#define TARG_633A30_RVA  0x633a30ULL
#define TARG_634810_RVA  0x634810ULL
#define TARG_633AE0_RVA  0x633ae0ULL
static const BYTE targ_633a30_prologue[29] = {
    0x48, 0x89, 0x5c, 0x24, 0x08,  /* mov [rsp+0x08], rbx */
    0x48, 0x89, 0x6c, 0x24, 0x10,  /* mov [rsp+0x10], rbp */
    0x48, 0x89, 0x74, 0x24, 0x18,  /* mov [rsp+0x18], rsi */
    0x57,                          /* push rdi            */
    0x48, 0x83, 0xec, 0x20,        /* sub rsp, 0x20       */
    0x41, 0x8b, 0xe8,              /* mov ebp, r8d        */
    0x48, 0x8b, 0xf2,              /* mov rsi, rdx        */
    0x48, 0x8b, 0xf9,              /* mov rdi, rcx        */
};
static const BYTE targ_634810_prologue[22] = {
    0x44, 0x89, 0x44, 0x24, 0x18,  /* mov [rsp+0x18], r8d */
    0x55, 0x53, 0x56, 0x57,        /* push rbp/rbx/rsi/rdi */
    0x41, 0x54, 0x41, 0x55, 0x41, 0x56, 0x41, 0x57,  /* push r12/r13/r14/r15 */
    0x48, 0x8d, 0x6c, 0x24, 0xe1,  /* lea rbp, [rsp-0x1f] */
};
static const BYTE targ_633ae0_prologue[17] = {
    0x44, 0x89, 0x44, 0x24, 0x18,  /* mov [rsp+0x18], r8d */
    0x55, 0x56, 0x57,              /* push rbp/rsi/rdi    */
    0x41, 0x56, 0x41, 0x57,        /* push r14/r15        */
    0x48, 0x8d, 0x6c, 0x24, 0xe0,  /* lea rbp, [rsp-0x20] */
};
typedef int64_t (*TargSubFn)(int64_t behavior, int64_t entity, int32_t tick);
typedef int64_t (*Targ633a30Fn)(int64_t behavior, int64_t entity, int32_t tick);
static TargSubFn    g_targ_m1_tramp = NULL;  /* FUN_140634810 (mode 1) past-prologue continuation */
static TargSubFn    g_targ_m2_tramp = NULL;  /* FUN_140633ae0 (mode 2) past-prologue continuation */
static Targ633a30Fn g_targ_trampoline = NULL;
static volatile uint32_t g_targ_m1_runs = 0; /* bumped at FUN_140634810 entry  */
static volatile uint32_t g_targ_m2_runs = 0; /* bumped at FUN_140633ae0 entry  */
static uint32_t g_targ_lines     = 0;
static uint32_t g_targ_ticks     = 0;   /* targeting-entity-ticks the dispatcher hook processed   */
static uint32_t g_targ_pred_m1   = 0, g_targ_pred_m2 = 0, g_targ_pred_noop = 0;
static uint32_t g_targ_act_m1    = 0, g_targ_act_m2 = 0, g_targ_act_noop  = 0;
static uint32_t g_targ_src_ctrl  = 0, g_targ_src_mgr = 0, g_targ_src_def   = 0;
static uint32_t g_targ_bicond_ok = 0, g_targ_bicond_bad = 0;
static uint32_t g_targ_multi     = 0;   /* both sub-impls ran in one dispatch (invariant violation) */
static uint32_t g_targ_surv_last = 0xFFFFFFFFu;

static int64_t b634810_marker(int64_t behavior, int64_t entity, int32_t tick)
{
    g_targ_m1_runs++;
    return g_targ_m1_tramp(behavior, entity, tick);
}
static int64_t b633ae0_marker(int64_t behavior, int64_t entity, int32_t tick)
{
    g_targ_m2_runs++;
    return g_targ_m2_tramp(behavior, entity, tick);
}

static int64_t b633a30_hook(int64_t behavior, int64_t entity, int32_t tick_arg)
{
    if (!g_dt_frame_ctr) {
        HMODULE exe = GetModuleHandleA(NULL);
        if (exe) { g_dt_frame_ctr = (volatile uint32_t *)((BYTE *)exe + 0xb0a320);
                   g_dt_imgbase = (uintptr_t)exe; }
    }
    if (!(entity && g_dt_imgbase && dt_on()))
        return g_targ_trampoline(behavior, entity, tick_arg);

    /* Replicate the dispatcher's mode resolution (633a30.c:12-21). Faithful: the base vfunc_6
     * (FUN_1404c2f70) runs inside the trampoline AFTER this read but only writes behavior+0x30. */
    int src = 2, mode = 0, read_ok = 0;
    int64_t vtbl = *(int64_t *)entity;
    if (vtbl) {
        typedef int64_t (*GetCompFn)(int64_t, int32_t);
        GetCompFn gc = (GetCompFn)(*(int64_t *)(vtbl + 0x10));
        int64_t controller = gc ? gc(entity, 1) : 0;
        if (controller) {
            int64_t cvt = *(int64_t *)controller;
            typedef int32_t (*ModeFn)(int64_t);
            ModeFn cmode = cvt ? (ModeFn)(*(int64_t *)(cvt + 0x238)) : 0;
            mode = cmode ? cmode(controller) : 0;
            src = 0; read_ok = 1;
        } else {
            int64_t mgr = *(int64_t *)(g_dt_imgbase + 0xb15418);
            if (mgr) {
                int64_t mvt = *(int64_t *)mgr;
                typedef int32_t (*GsmFn)(int64_t);
                GsmFn gsm = mvt ? (GsmFn)(*(int64_t *)(mvt + 0xe0)) : 0;
                mode = gsm ? gsm(mgr) : 0;
                src = 1; read_ok = 1;
            } else {
                mode = *(int32_t *)(g_dt_imgbase + 0xb153fc);
                src = 2; read_ok = 1;
            }
        }
    }
    int pred = (mode == 1) ? 1 : (mode == 2) ? 2 : 0;

    uint32_t m1b = g_targ_m1_runs, m2b = g_targ_m2_runs;
    int64_t r = g_targ_trampoline(behavior, entity, tick_arg);
    uint32_t d1 = g_targ_m1_runs - m1b, d2 = g_targ_m2_runs - m2b;

    int multi = (d1 && d2);
    int act   = multi ? -1 : d1 ? 1 : d2 ? 2 : 0;
    int bicond_ok = (act >= 0) && (pred == act);

    g_targ_ticks++;
    if (pred == 1) g_targ_pred_m1++; else if (pred == 2) g_targ_pred_m2++; else g_targ_pred_noop++;
    if (act  == 1) g_targ_act_m1++;  else if (act  == 2) g_targ_act_m2++;  else if (act == 0) g_targ_act_noop++;
    if (src  == 0) g_targ_src_ctrl++; else if (src == 1) g_targ_src_mgr++;  else g_targ_src_def++;
    if (multi)     g_targ_multi++;
    if (bicond_ok) g_targ_bicond_ok++; else g_targ_bicond_bad++;

    int32_t now = (int32_t)*g_dt_frame_ctr;
    int want = !bicond_ok || multi || (g_targ_ticks <= 40) || (pred == 2) || (act == 2) ||
               ((now & 0x3ff) == 0);
    if (want && g_targ_lines < 20000u) {
        g_targ_lines++;
        char ln[320];
        snprintf(ln, sizeof ln,
            "DTARG\ttick=%d\tent=%llx\tmode=%d\tsrc=%d\tpred=%d\tact=%d\td1=%u\td2=%u"
            "\tbicond=%d\tmulti=%d\treadok=%d\n",
            now, (unsigned long long)entity, mode, src, pred, act, d1, d2, bicond_ok, multi, read_ok);
        log_write(ln);
    }
    if (now != (int32_t)g_targ_surv_last && ((uint32_t)now & 0x1ffu) == 0) {
        g_targ_surv_last = (uint32_t)now;
        char sv[320];
        snprintf(sv, sizeof sv,
            "DTARGSURVEY\ttick=%d\tticks=%u\tpred(m1/m2/noop)=%u/%u/%u\tact(m1/m2/noop)=%u/%u/%u"
            "\tsrc(ctrl/mgr/def)=%u/%u/%u\tbicond_ok=%u\tbicond_bad=%u\tmulti=%u\n",
            now, g_targ_ticks, g_targ_pred_m1, g_targ_pred_m2, g_targ_pred_noop,
            g_targ_act_m1, g_targ_act_m2, g_targ_act_noop,
            g_targ_src_ctrl, g_targ_src_mgr, g_targ_src_def,
            g_targ_bicond_ok, g_targ_bicond_bad, g_targ_multi);
        log_write(sv);
    }
    return r;
}

/* Install one inline trampoline (whole-instruction prologue copy + FF25 abs jmp). */
static BOOL install_targ_tramp(uint64_t rva, const BYTE *prologue, size_t n, void *hookfn,
                               void **out_tramp, const char *tag)
{
    HMODULE exe = GetModuleHandleA(NULL);
    if (!exe) return FALSE;
    BYTE *fn = (BYTE *)exe + rva;
    if (memcmp(fn, prologue, n) != 0) {
        char w[160];
        snprintf(w, sizeof w, "[eaw-mt] WARN: %s prologue mismatch — DTARG part not installed\n", tag);
        log_write(w);
        return FALSE;
    }
    BYTE *tramp = (BYTE *)VirtualAlloc(NULL, 64, MEM_COMMIT|MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    if (!tramp) { log_write("[eaw-mt] WARN: VirtualAlloc for DTARG trampoline failed\n"); return FALSE; }
    memcpy(tramp, prologue, n);
    write_abs_jmp(tramp + n, (uint64_t)(fn + n));
    *out_tramp = tramp;
    DWORD old;
    VirtualProtect(fn, n, PAGE_EXECUTE_READWRITE, &old);
    write_abs_jmp(fn, (uint64_t)hookfn);
    VirtualProtect(fn, n, old, &old);
    FlushInstructionCache(GetCurrentProcess(), fn, n);
    return TRUE;
}

static void install_b633a30_hook(void)
{
    void *t1 = NULL, *t2 = NULL, *t3 = NULL;
    /* Markers first so the dispatcher's biconditional sees them on its first call. */
    if (install_targ_tramp(TARG_634810_RVA, targ_634810_prologue, sizeof targ_634810_prologue,
                           (void *)b634810_marker, &t1, "FUN_140634810"))
        g_targ_m1_tramp = (TargSubFn)t1;
    if (install_targ_tramp(TARG_633AE0_RVA, targ_633ae0_prologue, sizeof targ_633ae0_prologue,
                           (void *)b633ae0_marker, &t2, "FUN_140633ae0"))
        g_targ_m2_tramp = (TargSubFn)t2;
    if (install_targ_tramp(TARG_633A30_RVA, targ_633a30_prologue, sizeof targ_633a30_prologue,
                           (void *)b633a30_hook, &t3, "FUN_140633a30")) {
        g_targ_trampoline = (Targ633a30Fn)t3;
        log_write("[eaw-mt] DTARG: TargetingBehaviorClass dispatcher + sub-impl markers installed\n");
    }
}

/* =========================================================================
 * DTARG2 — in-game oracle for the lifted Targeting team-targeting AIM GEOMETRY core
 * (sim/targeting_aim.cpp, from FUN_14020acd0). Behavior #7 (part 2).
 *
 * The mode-2 fire-control body FUN_140633ae0 (the DTARG-confirmed dominant in-combat path) is a
 * target-selection / fire-control orchestration FSM (the deferred Phase-B emit seam). Its ONE embedded,
 * pure, bit-matchable numeric core is the aim geometry FUN_14020acd0: direction(self->tgt) -> Euler
 * {roll=0, pitch, yaw} (deg). 14020acd0 has its own stable entry point and is a leaf-ish pure function
 * (reads only its 3 pointer args + .rdata constants, calls atan2/sqrt), so we oracle it directly:
 *   * snapshot self.xyz (rdx) + tgt.xyz (r8) BEFORE the trampoline,
 *   * run the real 14020acd0 once (fills out[0..2] = {roll, pitch, yaw}),
 *   * recompute the prediction in-process calling the engine's OWN atan2 (FUN_14078437c) + sqrt
 *     (FUN_140776d48) -> BIT-EXACT vs the binary (neb_biteq), with PI/2.0/360.0 read from .rdata so the
 *     rad2deg = 360 / (PI*2.0) division is the binary's runtime float32 op (no compile-time fold).
 *   * compare roll/pitch/yaw bit-for-bit.
 *
 * Prologue (25 bytes, all position-independent; first rip-rel is at +0x35):
 *   48 8b c4 / 48 89 58 08 / 48 89 68 10 / 48 89 70 18 / 48 89 78 20 / 41 56 / 48 83 ec 50
 *   = mov rax,rsp; mov[rax+8],rbx; mov[rax+10],rbp; mov[rax+18],rsi; mov[rax+20],rdi; push r14;
 *     sub rsp,0x50  — cut at +0x19. Entry rcx=out, rdx=self, r8=tgt (3 ptr args, MS x64). */
#define AIM_14020ACD0_RVA  0x20acd0ULL
static const BYTE aim_14020acd0_prologue[25] = {
    0x48, 0x8b, 0xc4,              /* mov rax, rsp        */
    0x48, 0x89, 0x58, 0x08,        /* mov [rax+0x08], rbx */
    0x48, 0x89, 0x68, 0x10,        /* mov [rax+0x10], rbp */
    0x48, 0x89, 0x70, 0x18,        /* mov [rax+0x18], rsi */
    0x48, 0x89, 0x78, 0x20,        /* mov [rax+0x20], rdi */
    0x41, 0x56,                    /* push r14            */
    0x48, 0x83, 0xec, 0x50,        /* sub rsp, 0x50       */
};
typedef void *(*Aim14020acd0Fn)(void *out, float *self, float *tgt);
typedef float (*AimAtan2Fn)(float y, float x);   /* FUN_14078437c atan2(y,x), xmm0/xmm1 in, xmm0 out */
typedef float (*AimSqrtFn)(float s);             /* FUN_140776d48 sqrt(s),    xmm0 in/out            */
static Aim14020acd0Fn g_aim_trampoline = NULL;
static AimAtan2Fn     g_aim_atan2 = NULL;
static AimSqrtFn      g_aim_sqrt  = NULL;
static uint32_t g_aim_ticks   = 0;
static uint32_t g_aim_yaw_ok = 0,   g_aim_yaw_bad = 0;
static uint32_t g_aim_pitch_ok = 0, g_aim_pitch_bad = 0;
static uint32_t g_aim_roll_ok = 0,  g_aim_roll_bad = 0;
static uint32_t g_aim_path[4] = {0, 0, 0, 0};  /* branch dist: 0=both degenerate,1=pitch-only,2=yaw-only,3=both computed */
static uint32_t g_aim_lines = 0;
static uint32_t g_aim_surv_last = 0xFFFFFFFFu;

static void *b14020acd0_hook(void *out, float *self, float *tgt)
{
    if (!g_dt_frame_ctr) {
        HMODULE exe = GetModuleHandleA(NULL);
        if (exe) { g_dt_frame_ctr = (volatile uint32_t *)((BYTE *)exe + 0xb0a320);
                   g_dt_imgbase = (uintptr_t)exe; }
    }
    if (!g_aim_atan2 && g_dt_imgbase) {
        g_aim_atan2 = (AimAtan2Fn)(g_dt_imgbase + 0x78437c);   /* engine atan2 */
        g_aim_sqrt  = (AimSqrtFn) (g_dt_imgbase + 0x776d48);   /* engine sqrt  */
    }
    if (!(out && self && tgt && g_dt_imgbase && g_aim_atan2 && g_aim_sqrt && dt_on()))
        return g_aim_trampoline(out, self, tgt);

    /* Snapshot inputs (the function reads them; copy before, in case it writes through aliasing out). */
    float sx = self[0], sy = self[1], sz = self[2];
    float tx = tgt[0],  ty = tgt[1],  tz = tgt[2];

    void *r = g_aim_trampoline(out, self, tgt);   /* fills out[0..2] = {roll, pitch, yaw} */

    float *o = (float *)out;
    float roll_a = o[0], pitch_a = o[1], yaw_a = o[2];

    /* Predict with the engine's own atan2/sqrt + .rdata constants (forces the binary's runtime float32
     * groupings: rad2deg = 360 / (PI*2.0), horiz = sqrt(dy*dy + dx*dx); pitch negated via ^0x80000000). */
    float aim_pi  = *(float *)(g_dt_imgbase + 0x8007dc);
    float aim_two = *(float *)(g_dt_imgbase + 0x8007d4);
    float aim_360 = *(float *)(g_dt_imgbase + 0x8007f4);
    float rad2deg = aim_360 / (aim_pi * aim_two);
    float dx = tx - sx, dy = ty - sy, dz = tz - sz;

    int yaw_comp, pitch_comp;   /* 0 = degenerate (skipped, angle forced 0), 1 = computed via atan2 */
    float yaw_p;
    if (dx == 0.0f && dy == 0.0f) { yaw_p = 0.0f; yaw_comp = 0; }
    else { yaw_p = g_aim_atan2(dy, dx) * rad2deg; if (yaw_p < 0.0f) yaw_p += aim_360; yaw_comp = 1; }
    float pitch_p;
    if (dz == 0.0f && dx == 0.0f) { pitch_p = 0.0f; pitch_comp = 0; }
    else {
        float horiz = g_aim_sqrt(dy * dy + dx * dx);
        float p = g_aim_atan2(dz, horiz) * rad2deg;
        uint32_t pb; memcpy(&pb, &p, 4); pb ^= 0x80000000u; memcpy(&pitch_p, &pb, 4);
        pitch_comp = 1;
    }
    float roll_p = 0.0f;
    int path = (yaw_comp << 1) | pitch_comp;   /* 0=both degenerate, 1=pitch only, 2=yaw only, 3=both */

    int yaw_ok   = neb_biteq(yaw_a, yaw_p);
    int pitch_ok = neb_biteq(pitch_a, pitch_p);
    int roll_ok  = neb_biteq(roll_a, roll_p);

    g_aim_ticks++;
    if (yaw_ok)   g_aim_yaw_ok++;   else g_aim_yaw_bad++;
    if (pitch_ok) g_aim_pitch_ok++; else g_aim_pitch_bad++;
    if (roll_ok)  g_aim_roll_ok++;  else g_aim_roll_bad++;
    g_aim_path[path & 3]++;

    int32_t now = (int32_t)*g_dt_frame_ctr;
    int want = !yaw_ok || !pitch_ok || !roll_ok || (g_aim_ticks <= 40) || ((now & 0x3ff) == 0);
    if (want && g_aim_lines < 20000u) {
        g_aim_lines++;
        char ln[384];
        snprintf(ln, sizeof ln,
            "DTARG2\ttick=%d\tpath=%d\tyaw(a/p)=%.6f/%.6f\tpitch(a/p)=%.6f/%.6f\troll(a/p)=%.6f/%.6f"
            "\tyaw_ok=%d\tpitch_ok=%d\troll_ok=%d\n",
            now, path, yaw_a, yaw_p, pitch_a, pitch_p, roll_a, roll_p, yaw_ok, pitch_ok, roll_ok);
        log_write(ln);
    }
    if (now != (int32_t)g_aim_surv_last && ((uint32_t)now & 0x1ffu) == 0) {
        g_aim_surv_last = (uint32_t)now;
        char sv[384];
        snprintf(sv, sizeof sv,
            "DTARG2SURVEY\ttick=%d\tticks=%u\tyaw(ok/bad)=%u/%u\tpitch(ok/bad)=%u/%u\troll(ok/bad)=%u/%u"
            "\tpath(deg/pitch/yaw/full)=%u/%u/%u/%u\n",
            now, g_aim_ticks, g_aim_yaw_ok, g_aim_yaw_bad, g_aim_pitch_ok, g_aim_pitch_bad,
            g_aim_roll_ok, g_aim_roll_bad, g_aim_path[0], g_aim_path[1], g_aim_path[2], g_aim_path[3]);
        log_write(sv);
    }
    return r;
}

static void install_b14020acd0_hook(void)
{
    void *t = NULL;
    if (install_targ_tramp(AIM_14020ACD0_RVA, aim_14020acd0_prologue, sizeof aim_14020acd0_prologue,
                           (void *)b14020acd0_hook, &t, "FUN_14020acd0")) {
        g_aim_trampoline = (Aim14020acd0Fn)t;
        log_write("[eaw-mt] DTARG2: Targeting aim-geometry (FUN_14020acd0) oracle installed\n");
    }
}

/* =========================================================================
 * DTLURE — in-game oracle for the lifted shared 3D squared-distance primitive
 * (sim/geom_distance.cpp, from FUN_140397640). Behavior #9 (Lure) numeric core.
 *
 * LureBehaviorClass::vfunc_6 (0x62b4c0) is — like UnitAI #4 / Targeting #7 — predominantly a cross-entity
 * orchestration seam (scan the lure's target squads, apply FUN_14062b270 to each member in range). Its
 * ONE embedded, pure, bit-matchable numeric core is the RADIUS GATE's squared distance, computed by
 * FUN_140397640: a 62-byte PURE LEAF (no intrinsic calls) that reads only an entity's world position
 * (GameObjectClass+0x78/+0x7c/+0x80) and a 3-float query point, reused by range checks across MANY
 * behaviors. Per the #7-part-2 banked lesson we oracle it directly at its own entry:
 *   * snapshot entity.xyz (rcx+0x78/0x7c/0x80) + query.xyz (rdx[0..2]) BEFORE the trampoline,
 *   * run the real 140397640 once (returns dist² in xmm0 as a double),
 *   * recompute pred = (double)(dx*dx)+(double)(dy*dy)+(double)(dz*dz) — float32 squares promoted then
 *     summed left-assoc in double, the binary's exact grouping — and compare BIT-FOR-BIT (memcmp, 8 bytes).
 *
 * Prologue (15 bytes, all position-independent — leaf, no rip-rel, first 3 movss/subss):
 *   f3 0f 10 49 7c / f3 0f 5c 4a 04 / f3 0f 10 41 78
 *   = movss xmm1,[rcx+0x7c]; subss xmm1,[rdx+0x4]; movss xmm0,[rcx+0x78]  — cut at +0x0f (>=14 for FF25).
 * Entry rcx=entity, rdx=query (float*); returns double in xmm0. */
#define DIST_397640_RVA  0x397640ULL
static const BYTE dist_397640_prologue[15] = {
    0xf3, 0x0f, 0x10, 0x49, 0x7c,   /* movss xmm1, [rcx+0x7c] */
    0xf3, 0x0f, 0x5c, 0x4a, 0x04,   /* subss xmm1, [rdx+0x4]  */
    0xf3, 0x0f, 0x10, 0x41, 0x78,   /* movss xmm0, [rcx+0x78] */
};
typedef double (*Dist397640Fn)(void *entity, float *query);
static Dist397640Fn g_dist_trampoline = NULL;
static uint32_t g_dist_ticks = 0;
static uint32_t g_dist_ok = 0, g_dist_bad = 0;
static uint32_t g_dist_zero = 0;            /* coverage: dist²==0 (entity AT the query point) */
static double   g_dist_max = 0.0;           /* coverage: largest dist² observed              */
static uint32_t g_dist_lines = 0;
static uint32_t g_dist_surv_last = 0xFFFFFFFFu;

static double b397640_hook(void *entity, float *query)
{
    if (!g_dt_frame_ctr) {
        HMODULE exe = GetModuleHandleA(NULL);
        if (exe) { g_dt_frame_ctr = (volatile uint32_t *)((BYTE *)exe + 0xb0a320);
                   g_dt_imgbase = (uintptr_t)exe; }
    }
    if (!(entity && query && g_dt_imgbase && dt_on()))
        return g_dist_trampoline(entity, query);

    /* Snapshot inputs before the call (the function only reads them, but stay robust to aliasing). */
    float ex = *(float *)((BYTE *)entity + 0x78);
    float ey = *(float *)((BYTE *)entity + 0x7c);
    float ez = *(float *)((BYTE *)entity + 0x80);
    float qx = query[0], qy = query[1], qz = query[2];

    double act = g_dist_trampoline(entity, query);   /* the real squared distance (double, xmm0) */

    /* Predict: float32 deltas, float32 squares, each promoted to double, summed left-assoc ((dx²+dy²)+dz²)
     * — the exact codegen of FUN_140397640 (subss/mulss/cvtps2pd/addsd). */
    float dx = ex - qx, dy = ey - qy, dz = ez - qz;
    double pred = (double)(dx * dx) + (double)(dy * dy) + (double)(dz * dz);

    int ok = (memcmp(&act, &pred, sizeof act) == 0);   /* bit-exact double compare */

    g_dist_ticks++;
    if (ok) g_dist_ok++; else g_dist_bad++;
    if (act == 0.0) g_dist_zero++;
    if (act > g_dist_max) g_dist_max = act;

    int32_t now = (int32_t)*g_dt_frame_ctr;
    int want = !ok || (g_dist_ticks <= 40) || ((now & 0x3ff) == 0);
    if (want && g_dist_lines < 20000u) {
        g_dist_lines++;
        char ln[256];
        /* %a (hex float) so any mismatch is fully visible at bit precision; the memcmp above is authoritative. */
        snprintf(ln, sizeof ln, "DTLURE\ttick=%d\tdist2(a/p)=%a/%a\tok=%d\n", now, act, pred, ok);
        log_write(ln);
    }
    if (now != (int32_t)g_dist_surv_last && ((uint32_t)now & 0x1ffu) == 0) {
        g_dist_surv_last = (uint32_t)now;
        char sv[256];
        snprintf(sv, sizeof sv,
            "DTLURESURVEY\ttick=%d\tticks=%u\tdist2(ok/bad)=%u/%u\tzero=%u\tmax=%a\n",
            now, g_dist_ticks, g_dist_ok, g_dist_bad, g_dist_zero, g_dist_max);
        log_write(sv);
    }
    return act;
}

static void install_b397640_hook(void)
{
    void *t = NULL;
    if (install_targ_tramp(DIST_397640_RVA, dist_397640_prologue, sizeof dist_397640_prologue,
                           (void *)b397640_hook, &t, "FUN_140397640")) {
        g_dist_trampoline = (Dist397640Fn)t;
        log_write("[eaw-mt] DTLURE: shared squared-distance (FUN_140397640) oracle installed\n");
    }
}

/* =========================================================================
 * DTREVEAL — in-game oracle for the lifted Reveal fog re-reveal MOVE-THRESHOLD GATE
 * (sim/reveal_gate.cpp, from RevealBehaviorClass::vfunc_6 FUN_1405373c0). Behavior #10.
 *
 * Reveal is the passive fog-of-war revealer on ~every mobile unit. Each tick it throttles its global
 * object re-scan (the deferred Phase-B emit seam) on movement: the gate at 0x5374ba..0x537613 computes
 * a 2D float32 squared move distance moved_sq = dx*dx + dy*dy (cur = entity world pos +0x78/+0x7c, last
 * = behavior slot +0x40/+0x44) and skips the rescan iff moved_sq < threshold (slot +0x48). That gate is
 * the only embedded pure numeric core; everything past it is cross-entity orchestration.
 *
 * Unlike the Lure leaf (#9), this gate is MID-FUNCTION and the binary never persists moved_sq to memory,
 * so we cannot read its computed value back. Instead the oracle's ground truth is an INLINE-ASM
 * transcription of the gate's exact SSE instruction sequence (subss/subss/mulss/mulss/addss) evaluated
 * on the REAL captured inputs: we trampoline at the function entry (0x5373c0), snapshot cur (entity
 * +0x78/+0x7c) and last/threshold (behavior +0x40/+0x44/+0x48) BEFORE the call (the rescan path
 * overwrites +0x40/+0x44), then compare the lifted float32 expression `dx*dx+dy*dy` against the asm
 * transcription bit-for-bit. This confirms the lift reproduces the binary's instruction sequence (no
 * FMA contraction / double promotion / reassociation) across the live in-game input distribution.
 *
 * Entry prologue (20 bytes, all position-independent — first rip-rel is the mov rax,[rdx+0x298] at +0x14):
 *   48 89 5c 24 08 / 48 89 6c 24 10 / 48 89 74 24 18 / 57 / 48 83 ec 70
 *   = mov [rsp+8],rbx; mov [rsp+10],rbp; mov [rsp+18],rsi; push rdi; sub rsp,0x70.
 *   Entry rcx=self(behavior), rdx=entity, r8d=p3 (3 args, MS x64). */
#define REVEAL_5373C0_RVA  0x5373c0ULL
static const BYTE reveal_5373c0_prologue[20] = {
    0x48, 0x89, 0x5c, 0x24, 0x08,   /* mov [rsp+0x08], rbx */
    0x48, 0x89, 0x6c, 0x24, 0x10,   /* mov [rsp+0x10], rbp */
    0x48, 0x89, 0x74, 0x24, 0x18,   /* mov [rsp+0x18], rsi */
    0x57,                           /* push rdi            */
    0x48, 0x83, 0xec, 0x70,         /* sub rsp, 0x70       */
};
typedef uint64_t (*Reveal5373c0Fn)(void *self, void *entity, uint32_t p3);
static Reveal5373c0Fn g_rev_trampoline = NULL;
static uint32_t g_rev_ticks = 0;
static uint32_t g_rev_ok = 0, g_rev_bad = 0;
static uint32_t g_rev_zero = 0;             /* coverage: moved_sq==0 (unit stationary since last reveal) */
static uint32_t g_rev_skip = 0;             /* coverage: gate would SKIP rescan  (moved_sq <  threshold) */
static uint32_t g_rev_rescan = 0;           /* coverage: gate would OPEN  rescan  (moved_sq >= threshold) */
static float    g_rev_max = 0.0f;           /* coverage: largest moved_sq observed                       */
static uint32_t g_rev_lines = 0;
static uint32_t g_rev_surv_last = 0xFFFFFFFFu;

/* Faithful transcription of the gate's SSE ops: dx=cx-lx, dy=cy-ly, dx²+dy² — all float32 (subss/mulss/
 * addss), no widening. AT&T `op src,dst` => dst op= src. */
static float reveal_gate_asm_movedsq(float cx, float cy, float lx, float ly)
{
    __asm__ __volatile__(
        "subss %2, %0\n\t"   /* cx -= lx   => dx        */
        "subss %3, %1\n\t"   /* cy -= ly   => dy        */
        "mulss %0, %0\n\t"   /* dx *= dx   => dx²       */
        "mulss %1, %1\n\t"   /* dy *= dy   => dy²       */
        "addss %1, %0\n\t"   /* dx² += dy² => moved_sq  */
        : "+x"(cx), "+x"(cy)
        : "x"(lx), "x"(ly));
    return cx;
}

static uint64_t reveal_5373c0_hook(void *self, void *entity, uint32_t p3)
{
    if (!g_dt_frame_ctr) {
        HMODULE exe = GetModuleHandleA(NULL);
        if (exe) { g_dt_frame_ctr = (volatile uint32_t *)((BYTE *)exe + 0xb0a320);
                   g_dt_imgbase = (uintptr_t)exe; }
    }
    if (!(self && entity && g_dt_imgbase && dt_on()))
        return g_rev_trampoline(self, entity, p3);

    /* Snapshot BEFORE the call: the rescan path stores cur into self+0x40/+0x44. */
    float cur_x = *(float *)((BYTE *)entity + 0x78);
    float cur_y = *(float *)((BYTE *)entity + 0x7c);
    float last_x = *(float *)((BYTE *)self + 0x40);
    float last_y = *(float *)((BYTE *)self + 0x44);
    float thr    = *(float *)((BYTE *)self + 0x48);

    uint64_t ret = g_rev_trampoline(self, entity, p3);

    /* Predict via the lifted float32 expression; ground-truth via the asm transcription of the gate. */
    float dx = cur_x - last_x, dy = cur_y - last_y;
    float pred = dx * dx + dy * dy;
    float act  = reveal_gate_asm_movedsq(cur_x, cur_y, last_x, last_y);

    int ok = (memcmp(&act, &pred, sizeof act) == 0);   /* bit-exact float32 compare */

    g_rev_ticks++;
    if (ok) g_rev_ok++; else g_rev_bad++;
    if (act == 0.0f) g_rev_zero++;
    if (act <  thr)  g_rev_skip++; else g_rev_rescan++;
    if (act > g_rev_max) g_rev_max = act;

    int32_t now = (int32_t)*g_dt_frame_ctr;
    int want = !ok || (g_rev_ticks <= 40) || ((now & 0x3ff) == 0);
    if (want && g_rev_lines < 20000u) {
        g_rev_lines++;
        char ln[256];
        /* %a (hex float) so any mismatch is fully visible at bit precision; the memcmp is authoritative. */
        snprintf(ln, sizeof ln, "DTREVEAL\ttick=%d\tmoved2(a/p)=%a/%a\tthr=%a\tskip=%d\tok=%d\n",
                 now, (double)act, (double)pred, (double)thr, (act < thr), ok);
        log_write(ln);
    }
    if (now != (int32_t)g_rev_surv_last && ((uint32_t)now & 0x1ffu) == 0) {
        g_rev_surv_last = (uint32_t)now;
        char sv[256];
        snprintf(sv, sizeof sv,
            "DTREVEALSURVEY\ttick=%d\tticks=%u\tmoved2(ok/bad)=%u/%u\tzero=%u\tskip/rescan=%u/%u\tmax=%a\n",
            now, g_rev_ticks, g_rev_ok, g_rev_bad, g_rev_zero, g_rev_skip, g_rev_rescan, (double)g_rev_max);
        log_write(sv);
    }
    return ret;
}

static void install_reveal_5373c0_hook(void)
{
    void *t = NULL;
    if (install_targ_tramp(REVEAL_5373C0_RVA, reveal_5373c0_prologue, sizeof reveal_5373c0_prologue,
                           (void *)reveal_5373c0_hook, &t, "FUN_1405373c0")) {
        g_rev_trampoline = (Reveal5373c0Fn)t;
        log_write("[eaw-mt] DTREVEAL: Reveal move-threshold gate (FUN_1405373c0) oracle installed\n");
    }
}

/* =========================================================================
 * DTFINT — in-game bit-exact oracle for the lifted PROJECTILE-INTERCEPT LEAD SOLVER
 * (sim/firing_intercept.cpp, b2 piece 1, from FUN_140399e20 + FUN_140393b70 simple mode).
 *
 * 399e20 is the quadratic projectile-intercept solver (the direct-aim path of the full lead solver
 * 399450). We entry-detour it, snapshot its inputs BEFORE the call — target pos (+0x78/+0x7c/+0x80),
 * target vel (+0x254/+0x264/+0x274), reference-frame velocity ref+{0xc,0x1c,0x2c} where
 * ref=(tgt+0xa8 ? *(tgt+0xa8)+0x164 : tgt+0x248), muzzle (param_4), shooter-ref (param_5), projectile
 * speed (param_6), gamespeed (DAT_140b0a340) — run the binary, then recompute the lead aim via the lifted
 * formula (identical grouping to sim/firing_intercept.cpp; the discriminant root uses the engine's OWN
 * sqrt 776d48 so the only floating intrinsic matches bit-for-bit). Compare the 3-float output bit-for-bit.
 *
 * The internal predictor 393b70 has two modes selected by DAT_140b2c37c: 0 = linear (lifted), != 0 =
 * curved/iterative (deferred). We bit-compare linear-mode solved calls + no-solution calls (binary writes
 * {0,0,0}); curved-mode solved calls are counted only (coverage of whether the curved lift is needed).
 *
 * Prologue (15 bytes, position-independent): 48 8b c4 / 48 89 58 08 / 57 / 48 81 ec 90 00 00 00
 *   = mov rax,rsp; mov [rax+8],rbx; push rdi; sub rsp,0x90.
 * Entry (MS x64): rcx=p1, rdx=out(float[3]), r8=target, r9=muzzle(float[3]), [stk]=sref(float[3]), speed.
 * Observe-only; all under dt_on() (EAW_DIFFTRACE=1). */
#define FINT_399E20_RVA 0x399e20ULL
static const BYTE fint_399e20_prologue[15] = {
    0x48,0x8b,0xc4, 0x48,0x89,0x58,0x08, 0x57, 0x48,0x81,0xec,0x90,0x00,0x00,0x00
};
typedef int64_t (*Fint399e20Fn)(int64_t,float*,int64_t,float*,float*,float);
typedef float   (*FintSqrtFn)(float);
static Fint399e20Fn g_fint_tramp = NULL;
static uint32_t g_fint_calls=0, g_fint_ok=0, g_fint_bad=0, g_fint_nosol=0, g_fint_nosolbad=0,
                g_fint_complex=0, g_fint_lines=0;
static uint32_t g_fint_surv_last=0xFFFFFFFFu;

static int64_t fint_399e20_hook(int64_t p1, float *out, int64_t tgt, float *muzzle, float *sref, float speed)
{
    if (!g_dt_frame_ctr) {
        HMODULE exe = GetModuleHandleA(NULL);
        if (exe) { g_dt_frame_ctr=(volatile uint32_t*)((BYTE*)exe+0xb0a320); g_dt_imgbase=(uintptr_t)exe; }
    }
    if (!(out && tgt && muzzle && sref && g_dt_imgbase && dt_on()))
        return g_fint_tramp(p1, out, tgt, muzzle, sref, speed);

    /* snapshot inputs BEFORE the call (399e20 does not mutate the target). */
    float g = (float)*(int32_t*)(g_dt_imgbase + 0xb0a340);   /* (float)DAT_140b0a340: INT32 hz (=30), not a float reinterpret */
    int32_t predmode = *(int32_t*)(g_dt_imgbase + 0xb2c37c);
    float tpx=*(float*)(tgt+0x78),  tpy=*(float*)(tgt+0x7c),  tpz=*(float*)(tgt+0x80);
    float tvx=*(float*)(tgt+0x254), tvy=*(float*)(tgt+0x264), tvz=*(float*)(tgt+0x274);
    int64_t ref=*(int64_t*)(tgt+0xa8); ref = ref ? ref+0x164 : tgt+0x248;
    float fvx=*(float*)(ref+0xc), fvy=*(float*)(ref+0x1c), fvz=*(float*)(ref+0x2c);
    float mx=muzzle[0], my=muzzle[1], mz=muzzle[2];
    float rsx=sref[0],  rsy=sref[1],  rsz=sref[2];

    int64_t r = g_fint_tramp(p1, out, tgt, muzzle, sref, speed);

    /* recompute — mirror sim/firing_intercept.cpp exactly; engine sqrt (776d48) for the root. */
    FintSqrtFn esqrt = (FintSqrtFn)(g_dt_imgbase + 0x776d48);
    float relx=mx-tpx, rely=my-tpy, relz=mz-tpz;
    float sx=mx-rsx, sy=my-rsy, sz=mz-rsz;
    float rvy=(tvy-fvy)*g, rvx=(tvx-fvx)*g, rvz=(tvz-fvz)*g;
    float a=(rvy*rvy+rvx*rvx+rvz*rvz)-(((g*speed)*g)*speed);
    float b=(rvy*sy+sx*rvx+rvz*sz)*2.0f;
    float c=sx*sx+sy*sy+sz*sz;
    float disc=b*b-a*4.0f*c;
    float t=0.0f; int solved=0;
    if (0.0f<=disc) {
        if (a==0.0f) { if (b!=0.0f){ t=(-c)/b; if (0.0f<=t) solved=1; } }
        else {
            float root=esqrt(disc);
            float t1=(root-b)/(a*2.0f);
            float t2=((-b)-root)/(a*2.0f);
            if (0.0f<t1){ if(0.0f<t2){ if(t2<=t1)t1=t2; t=t1; solved=1; } else { t=t1; solved=1; } }
            else if (0.0f<t2){ t=t2; solved=1; }
        }
    }
    float aim[3];
    if (!solved){ aim[0]=aim[1]=aim[2]=0.0f; }
    else { float f=g*t;
        aim[0]=relx+((tvx-fvx)*f+tpx);
        aim[1]=rely+((tvy-fvy)*f+tpy);
        aim[2]=relz+((tvz-fvz)*f+tpz); }

    g_fint_calls++;
    int ok=1, compared=0;
    if (!solved)            { ok=(memcmp(out,aim,12)==0); g_fint_nosol++; if(!ok)g_fint_nosolbad++; compared=1; }
    else if (predmode!=0)   { g_fint_complex++; }                 /* curved predictor: deferred, skip */
    else                    { ok=(memcmp(out,aim,12)==0); compared=1; if(ok)g_fint_ok++; else g_fint_bad++; }

    int32_t now=(int32_t)*g_dt_frame_ctr;
    int want = (compared && !ok) || (g_fint_calls<=40) || ((now&0x3ff)==0);
    if (want && g_fint_lines<20000u) {
        g_fint_lines++;
        char ln[320];
        snprintf(ln,sizeof ln,
          "DTFINT\ttick=%d\tsolved=%d\tmode=%d\taim_a=(%a,%a,%a)\taim_p=(%a,%a,%a)\tok=%d\n",
          now, solved, predmode, (double)out[0],(double)out[1],(double)out[2],
          (double)aim[0],(double)aim[1],(double)aim[2], ok);
        log_write(ln);
    }
    if (now!=(int32_t)g_fint_surv_last && ((uint32_t)now&0x1ffu)==0) {
        g_fint_surv_last=(uint32_t)now;
        char sv[256];
        snprintf(sv,sizeof sv,
          "DTFINTSURVEY\ttick=%d\tcalls=%u\tok/bad=%u/%u\tnosol(ok)=%u(bad %u)\tcomplex=%u\n",
          now, g_fint_calls, g_fint_ok, g_fint_bad, g_fint_nosol, g_fint_nosolbad, g_fint_complex);
        log_write(sv);
    }
    return r;
}

static void install_fint_399e20_hook(void)
{
    void *t=NULL;
    if (install_targ_tramp(FINT_399E20_RVA, fint_399e20_prologue, sizeof fint_399e20_prologue,
                           (void*)fint_399e20_hook, &t, "FUN_140399e20")) {
        g_fint_tramp = (Fint399e20Fn)t;
        log_write("[eaw-mt] DTFINT: projectile-intercept lead solver (FUN_140399e20) oracle installed\n");
    }
}

/* =========================================================================
 * DTDYN — in-game bit-exact oracle for the lifted DYNAMICTRANSFORM REBUILD
 * (sim/dynamic_transform.cpp, b2′ Pass-C, from DynamicTransformBehavior FUN_1403ac530 + the Givens leaves
 * FUN_1400480f0 / FUN_1402cf8d0). 3ac530 rebuilds the object's 3×4 transform (+0x248..+0x274) each tick
 * from its stored Euler pose (degrees, +0x90/+0x94/+0x98) and world position (+0x78/+0x7c/+0x80). We
 * entry-detour it, snapshot the Euler + position BEFORE the call, run the binary, then recompute the 12
 * matrix floats via the lifted formula (identity + 3 Givens by deg2rad·euler_z/_y/_x + a fixed 90° roll;
 * sin/cos via the engine's OWN 776650/776150 with the small-angle guard) and compare bit-for-bit.
 *
 * Prologue (15 bytes, position-independent): 48 8b c4 / 48 89 58 10 / 48 89 68 18 / 48 89 70 20
 *   = mov rax,rsp; mov [rax+0x10],rbx; mov [rax+0x18],rbp; mov [rax+0x20],rsi.  Entry rcx=obj, dl=flag.
 * Observe-only; all under dt_on() (EAW_DIFFTRACE=1). */
#define DYN_3AC530_RVA 0x3ac530ULL
static const BYTE dyn_3ac530_prologue[15] = {
    0x48,0x8b,0xc4, 0x48,0x89,0x58,0x10, 0x48,0x89,0x68,0x18, 0x48,0x89,0x70,0x20
};
typedef void  (*Dyn3ac530Fn)(int64_t, int8_t);
typedef float (*DynScFn)(float);
static Dyn3ac530Fn g_dyn_tramp = NULL;
static uint32_t g_dyn_calls=0, g_dyn_ok=0, g_dyn_bad=0, g_dyn_lines=0;
static uint32_t g_dyn_surv_last=0xFFFFFFFFu;

/* the engine's small-angle guard: |angle| above ~2^-69 → sin/cos, else (angle, 1.0). */
static inline void dyn_guarded_sc(float angle, DynScFn es, DynScFn ec, float *s, float *c) {
    uint32_t bits; memcpy(&bits, &angle, 4);
    if (0x1d000000u < (bits & 0x7f800000u)) { *s = es(angle); *c = ec(angle); }
    else                                    { *s = angle;     *c = 1.0f; }
}
static inline void dyn_rot01(float *m, float a, DynScFn es, DynScFn ec) {   /* 480f0: cols 0,1 */
    float s,c; dyn_guarded_sc(a,es,ec,&s,&c);
    for (int r=0;r<3;r++){int i=4*r; float x=m[i],y=m[i+1]; m[i+1]=y*c-x*s; m[i]=x*c+y*s;}
}
static inline void dyn_rot12(float *m, float a, DynScFn es, DynScFn ec) {   /* cf8d0: cols 1,2 */
    float s,c; dyn_guarded_sc(a,es,ec,&s,&c);
    for (int r=0;r<3;r++){int i=4*r; float y=m[i+1],x=m[i+2]; m[i+2]=x*c-y*s; m[i+1]=y*c+x*s;}
}
static inline void dyn_rot20(float *m, float a, DynScFn es, DynScFn ec) {   /* inline: cols 0,2 */
    float s,c; dyn_guarded_sc(a,es,ec,&s,&c);
    for (int r=0;r<3;r++){int i=4*r; float x=m[i+2],y=m[i]; m[i+2]=x*c+y*s; m[i]=y*c-x*s;}
}

static void dyn_3ac530_hook(int64_t obj, int8_t flag)
{
    if (!g_dt_frame_ctr) {
        HMODULE exe = GetModuleHandleA(NULL);
        if (exe) { g_dt_frame_ctr=(volatile uint32_t*)((BYTE*)exe+0xb0a320); g_dt_imgbase=(uintptr_t)exe; }
    }
    if (!(obj && g_dt_imgbase && dt_on())) { g_dyn_tramp(obj, flag); return; }

    /* snapshot inputs BEFORE the rebuild. */
    float ex=*(float*)(obj+0x90), ey=*(float*)(obj+0x94), ez=*(float*)(obj+0x98);
    float px=*(float*)(obj+0x78), py=*(float*)(obj+0x7c), pz=*(float*)(obj+0x80);

    g_dyn_tramp(obj, flag);

    const float *bm = (const float*)(obj+0x248);   /* the binary's rebuilt 3×4 matrix */

    /* recompute — mirror sim/dynamic_transform.cpp; engine sin/cos (776650/776150). */
    DynScFn es=(DynScFn)(g_dt_imgbase+0x776650), ec=(DynScFn)(g_dt_imgbase+0x776150);
    float m[12];
    m[0]=1.0f;m[1]=0.0f;m[2]=0.0f;m[3]=px;
    m[4]=0.0f;m[5]=1.0f;m[6]=0.0f;m[7]=py;
    m[8]=0.0f;m[9]=0.0f;m[10]=1.0f;m[11]=pz;
    const float PI_F=3.1415927410125732f;
    const float d2r=(PI_F*2.0f)/360.0f;
    dyn_rot01(m, d2r*ez, es, ec);
    dyn_rot20(m, d2r*ey, es, ec);
    dyn_rot12(m, d2r*ex, es, ec);
    dyn_rot01(m, d2r*90.0f, es, ec);

    int ok = (memcmp(bm, m, sizeof m) == 0);
    g_dyn_calls++; if (ok) g_dyn_ok++; else g_dyn_bad++;

    int32_t now=(int32_t)*g_dt_frame_ctr;
    int want = !ok || (g_dyn_calls<=40) || ((now&0x3ff)==0);
    if (want && g_dyn_lines<20000u) {
        g_dyn_lines++;
        char ln[320];
        snprintf(ln,sizeof ln,
          "DTDYN\ttick=%d\teuler=(%a,%a,%a)\tm0_a/p=%a/%a\tm10_a/p=%a/%a\tok=%d\n",
          now,(double)ex,(double)ey,(double)ez,(double)bm[0],(double)m[0],
          (double)bm[10],(double)m[10], ok);
        log_write(ln);
    }
    if (now!=(int32_t)g_dyn_surv_last && ((uint32_t)now&0x1ffu)==0) {
        g_dyn_surv_last=(uint32_t)now;
        char sv[160];
        snprintf(sv,sizeof sv,"DTDYNSURVEY\ttick=%d\tcalls=%u\tok/bad=%u/%u\n",
                 now, g_dyn_calls, g_dyn_ok, g_dyn_bad);
        log_write(sv);
    }
}

static void install_dyn_3ac530_hook(void)
{
    void *t=NULL;
    if (install_targ_tramp(DYN_3AC530_RVA, dyn_3ac530_prologue, sizeof dyn_3ac530_prologue,
                           (void*)dyn_3ac530_hook, &t, "FUN_1403ac530")) {
        g_dyn_tramp = (Dyn3ac530Fn)t;
        log_write("[eaw-mt] DTDYN: DynamicTransform rebuild (FUN_1403ac530) oracle installed\n");
    }
}

/* =========================================================================
 * DTTK — in-game oracle for the lifted TELEKINESIS interp-timeline core
 * (sim/telekinesis_target.cpp, from FUN_14063f210). Behavior #8 (part 1).
 *
 * TelekinesisTargetBehaviorClass::vfunc_6 (0x63f210) is the Force force-grip effect: a 3-mode
 * lifecycle SM over a per-entity telekinesis slot (slot = entity+0x160; mode int at slot+0x8).
 * mode 1 GRAB / mode 3 RELEASE share one interp timeline; mode 2 HOLD (63f730) is a spin + damage
 * body (deferred). The timeline (63f210.c:51-57, 63f470.c:28-35):
 *     now      = (float)sim_clock / (float)hz      (DAT_140b0a320 / DAT_140b0a340)
 *     t        = max((now - start)/duration, 0)     (start=slot+0x24, dur=_DAT_140b15ac4; clamp-low)
 *     complete = (1.0 <= t)
 * GRAB lerps toward target = DAT_140b15ac0 + slot+0x14; RELEASE lerps toward slot+0x14 directly. The
 * interpolated value is pushed as the Z of the set-position apply (FUN_1403a8f90 -> entity+0x80). On
 * completion both modes rebase slot+0x24 := now and CHANGE slot+0x8 (GRAB 1->2, RELEASE 3->0) — the
 * ONLY writes to slot+0x8 in either body, so a mode change across the tick == a completion edge.
 *
 * Validated (this hook reads the slot fields + config globals BEFORE the trampoline — the base
 * vfunc_6 runs inside it but only writes behavior+0x30, so the inputs are faithful):
 *   (1) COMPLETION BICONDITIONAL — predicted complete (1.0<=t) == actual (slot+0x8 changed across the
 *       tick). The lifted timeline decides the snap-to-final edge exactly when the binary does.
 *   (2) REBASE BIT-EXACT — on a completion, slot+0x24_after == predicted now (the engine writes
 *       slot+0x24 := fVar5 = now). A rock-solid explicit slot store.
 *   (3) INTERP LERP BIT-EXACT — on a NON-completion tick, entity+0x80_after == predicted
 *       lerp(slot+0x30_before, to, t), bit-for-bit (the continuously-observable quantity, Nebula-class).
 *
 * PART 2 (HOLD, FUN_14063f730 — mode 2): once GRAB completes (slot+0x8 := 2), each tick spins the
 * orientation + bobs the z-position by a phase angle = spin_t*omega (spin_t = max(elapsed/spin_per,0)),
 * and runs a self-clocked periodic-damage scheduler. Two more checks on mode-2 ticks:
 *   (4) Z-BOB POSE BIT-EXACT — entity+0x80_after == slot+0x14 + sin(angle)*bob_amp + height_offset,
 *       where sin uses the engine's OWN FUN_140776650 (called in-process for bit-exactness) but only when
 *       the angle clears the small-angle guard (exp field > 0x1d), else sin -> angle. height_offset is the
 *       SAME grip-height const DAT_140b15ac0 GRAB adds; the z-bob has no ramp (only the orientation does).
 *   (5) DAMAGE-DEADLINE REBASE BIT-EXACT — the damage cadence is a pure self-clocked function: due iff
 *       (slot+0x48 <= now && slot+0x48 != 0), and on fire slot+0x48_after == interval(slot+0x44) + slot+0x48.
 *       The gameplay damage write is to SELF (cf. AsteroidFieldDamage #6); only the event dispatch
 *       FUN_1402d5320 is the deferred cross-boundary seam. Emitted as DTTKHOLD lines.
 *
 * PART 3 (RELEASE, FUN_14063f470 — mode 3): the lifecycle terminus. Its interp timeline + lerp +
 * biconditional + rebase are the SHARED part-1 path (RELEASE lerps toward the raw slot+0x14). What is
 * NEW is the completion TEARDOWN (63f470.c:38-48) — on a mode-3 completion edge the slot is reset to a
 * canonical inert end-state, checked here:
 *   (6) MODE TERMINUS — slot+0x8_after == 0 (not just "changed"): the grip is fully over; the next
 *       vfunc_6 hits the Idle fall-through (the slot ptr stays non-null but every mode branch is false).
 *   (7) SENTINEL — slot+0x4c_after == 0x3fffff (the lone non-zero, non-`now` teardown write).
 *   (8) DAMAGE DISARM — slot+0x40_after == slot+0x48_after == 0: HOLD's self-clocked damage is switched
 *       off. Emitted as DTTKREL lines. (The ability re-enable cascade + the GOM dispatch FUN_1402d5320
 *       that complete the teardown stay deferred — the same Phase-B env seam as HOLD.)
 * Plus DTTKSURVEY periodic totals + a mode distribution (how often GRAB/HOLD/RELEASE/idle run).
 *
 * arg count = 3 (uniform vfunc_6 rcx=behavior, rdx=entity, r8d=tick). Prologue (PE, 63f210):
 *   48 89 5c 24 08 / 48 89 74 24 10 / 48 89 7c 24 18 / 55 / 48 8d 6c 24 a9 / 48 81 ec b0 00 00 00 /
 *   48 8b fa / 48 8b f1 = 34 bytes, cut BEFORE the E8 CALL (LocomotorCommonClass::vfunc_6) at +0x22.
 *   All position-independent (rsp-relative movs, push, lea, sub — no rel call/jmp copied).
 * ========================================================================= */
#define TK_63F210_RVA  0x63f210ULL
static const BYTE tk_63f210_prologue[34] = {
    0x48, 0x89, 0x5c, 0x24, 0x08,        /* mov [rsp+0x08], rbx */
    0x48, 0x89, 0x74, 0x24, 0x10,        /* mov [rsp+0x10], rsi */
    0x48, 0x89, 0x7c, 0x24, 0x18,        /* mov [rsp+0x18], rdi */
    0x55,                                /* push rbp            */
    0x48, 0x8d, 0x6c, 0x24, 0xa9,        /* lea rbp, [rsp-0x57] */
    0x48, 0x81, 0xec, 0xb0, 0x00, 0x00, 0x00,  /* sub rsp, 0xb0 */
    0x48, 0x8b, 0xfa,                    /* mov rdi, rdx        */
    0x48, 0x8b, 0xf1,                    /* mov rsi, rcx        */
};
typedef int64_t (*Tk63f210Fn)(int64_t behavior, int64_t entity, int32_t tick);
static Tk63f210Fn g_tk_trampoline = NULL;
static uint32_t g_tk_lines     = 0;
static uint32_t g_tk_ticks     = 0;   /* telekinesis-entity-ticks the hook processed (slot present) */
static uint32_t g_tk_noslot    = 0;   /* slot==0 (no telekinesis active)                            */
static uint32_t g_tk_grab = 0, g_tk_hold = 0, g_tk_release = 0, g_tk_idle = 0;   /* mode distribution */
static uint32_t g_tk_interp_ok = 0, g_tk_interp_bad = 0;   /* (3) interp lerp bit-exact */
static uint32_t g_tk_comp_edges = 0;                       /* completion edges observed */
static uint32_t g_tk_bicond_ok  = 0, g_tk_bicond_bad = 0;  /* (1) completion biconditional */
static uint32_t g_tk_rebase_ok  = 0, g_tk_rebase_bad = 0;  /* (2) rebase start==now on completion */
static uint32_t g_tk_surv_last  = 0xFFFFFFFFu;
/* HOLD (mode 2, FUN_14063f730) part-2 oracle — see the b63f210_hook HOLD block below. */
static uint32_t g_tk_hold_pose_ok  = 0, g_tk_hold_pose_bad  = 0;  /* (4) z-bob pose bit-exact (entity+0x80) */
static uint32_t g_tk_hold_sched_ok = 0, g_tk_hold_sched_bad = 0;  /* (5) damage-deadline rebase bit-exact */
static uint32_t g_tk_hold_fires    = 0;                          /* damage-fire ticks observed */
static uint32_t g_tk_hold_lines    = 0;
/* RELEASE (mode 3, FUN_14063f470) part-3 oracle — the completion teardown end-state. The interp lerp +
 * biconditional + rebase for mode 3 are already covered by the shared part-1 path; these check the
 * RELEASE-SPECIFIC teardown on the completion edge: mode terminus 0, the 0x3fffff sentinel, damage disarm. */
static uint32_t g_tk_rel_term_ok = 0, g_tk_rel_term_bad = 0;       /* (6) mode_after == 0 (lifecycle terminus) */
static uint32_t g_tk_rel_sent_ok = 0, g_tk_rel_sent_bad = 0;       /* (7) slot+0x4c == 0x3fffff (sentinel)    */
static uint32_t g_tk_rel_disarm_ok = 0, g_tk_rel_disarm_bad = 0;   /* (8) slot+0x40 == slot+0x48 == 0 (disarm) */
static uint32_t g_tk_rel_edges = 0;                               /* RELEASE completion edges observed */
static uint32_t g_tk_rel_lines = 0;
typedef float (*TkEngTrigFn)(float);   /* FUN_140776650 = engine sin(float)->float, xmm0 in/out (leaf) */
static TkEngTrigFn g_tk_eng_sin = NULL;

static int64_t b63f210_hook(int64_t behavior, int64_t entity, int32_t tick_arg)
{
    if (!g_dt_frame_ctr) {
        HMODULE exe = GetModuleHandleA(NULL);
        if (exe) { g_dt_frame_ctr = (volatile uint32_t *)((BYTE *)exe + 0xb0a320);
                   g_dt_imgbase = (uintptr_t)exe; }
    }
    if (!g_tk_eng_sin && g_dt_imgbase)
        g_tk_eng_sin = (TkEngTrigFn)(g_dt_imgbase + 0x776650);   /* engine sin for the HOLD z-bob */
    if (!(entity && g_dt_imgbase && dt_on()))
        return g_tk_trampoline(behavior, entity, tick_arg);

    int64_t slot = *(int64_t *)(entity + 0x160);
    if (!slot) {
        g_tk_noslot++;
        return g_tk_trampoline(behavior, entity, tick_arg);   /* NoSlot path: 0x80650001, nothing to validate */
    }
    int mode = *(int32_t *)(slot + 8);

    /* Config globals + slot inputs, read BEFORE the trampoline (faithful — base vfunc_6 only
     * writes behavior+0x30). 63f210.c:39,51-56. */
    uint32_t sim_clock = *g_dt_frame_ctr;
    int      hz        = *(int32_t *)(g_dt_imgbase + 0xb0a340);
    float    angle_base= *(float *)(g_dt_imgbase + 0xb15ac0);
    float    duration  = *(float *)(g_dt_imgbase + 0xb15ac4);
    float    start     = *(float *)(slot + 0x24);
    float    slot_off  = *(float *)(slot + 0x14);
    float    from      = *(float *)(slot + 0x30);
    /* HOLD (mode 2) inputs + config (63f730.c). slot_off == slot+0x14 is also the z-bob base. */
    float    spin_per  = *(float *)(g_dt_imgbase + 0xb15ac8);   /* _DAT_140b15ac8 spin period */
    float    omega     = *(float *)(g_dt_imgbase + 0x819b3c);   /* DAT_140819b3c angular freq  */
    float    bob_amp   = *(float *)(g_dt_imgbase + 0xb15ad4);   /* _DAT_140b15ad4 z-bob amp    */
    float    nxt_dmg   = *(float *)(slot + 0x48);               /* next-damage deadline (sec)  */
    float    dmg_ivl   = *(float *)(slot + 0x44);               /* damage interval (sec)       */

    float    now   = (float)sim_clock / (float)hz;
    float    t_raw = (now - start) / duration;
    float    t     = (0.0f <= t_raw) ? t_raw : 0.0f;
    int      pred_complete = (1.0f <= t);
    /* GRAB (mode 1) lerps toward angle_base+slot_off; RELEASE (mode 3) toward slot_off directly. */
    float    to       = (mode == 1) ? (angle_base + slot_off) : slot_off;
    float    lerp_val = (to - from) * t + from;

    /* HOLD (mode 2) predictions (63f730.c). z-bob: pos_z = slot+0x14 + sin(angle)*bob_amp + height_off,
     * where height_off == angle_base (DAT_140b15ac0, the SAME grip-height constant GRAB adds), angle =
     * spin_t*omega, spin_t = max(elapsed/spin_per,0). sin is the engine's own FUN_140776650 BUT only when
     * the angle clears the small-angle guard (exp field > 0x1d); else sin -> angle (small-angle limit).
     * Damage: a self-clocked deadline at slot+0x48 — due iff (deadline<=now && deadline!=0), and on fire
     * the deadline advances by interval (slot+0x44). pos_z lands in entity+0x80 (same channel as GRAB). */
    float hold_elapsed = now - start;
    float hold_spin_t  = hold_elapsed / spin_per;
    if (!(0.0f <= hold_spin_t)) hold_spin_t = 0.0f;
    float hold_angle   = hold_spin_t * omega;
    uint32_t ang_bits;  memcpy(&ang_bits, &hold_angle, sizeof ang_bits);
    float hold_sin     = ((ang_bits & 0x7f800000u) > 0x1d000000u && g_tk_eng_sin)
                             ? g_tk_eng_sin(hold_angle) : hold_angle;
    /* Grouping matches the binary codegen (63f730 @0x63f8d3-0x63f905): slot_z + (sin*bob + offset), NOT
     * the decompile's flattened left-assoc — FP add is non-associative, off by a sub-ULP at bob extrema. */
    float hold_pred_z  = slot_off + (hold_sin * bob_amp + angle_base);
    int   hold_due     = (nxt_dmg <= now) && (nxt_dmg != 0.0f);
    float hold_pred_nxt= hold_due ? (dmg_ivl + nxt_dmg) : nxt_dmg;

    int64_t r = g_tk_trampoline(behavior, entity, tick_arg);

    int   mode_after  = *(int32_t *)(slot + 8);
    float start_after = *(float *)(slot + 0x24);
    float val80       = *(float *)(entity + 0x80);
    float nxt_after   = *(float *)(slot + 0x48);
    uint32_t sent_after = *(uint32_t *)(slot + 0x4c);   /* RELEASE sentinel (63f470.c:48)  */
    float    dmg_after  = *(float *)(slot + 0x40);      /* RELEASE damage-amount disarm    */

    /* Only modes 1 (GRAB) and 3 (RELEASE) run the interp timeline; 2 (HOLD) and others don't. */
    int is_interp = (mode == 1 || mode == 3);
    int act_complete = (mode_after != mode);    /* slot+0x8 changes only on a completion edge */
    int bicond_ok = 0, rebase_ok = 0, interp_ok = 0, want = 0;

    g_tk_ticks++;
    if (mode == 1) g_tk_grab++; else if (mode == 2) g_tk_hold++;
    else if (mode == 3) g_tk_release++; else g_tk_idle++;

    if (is_interp) {
        bicond_ok = (pred_complete == act_complete);
        if (bicond_ok) g_tk_bicond_ok++; else g_tk_bicond_bad++;
        if (act_complete) {
            g_tk_comp_edges++;
            rebase_ok = neb_biteq(start_after, now);     /* slot+0x24 := now on completion */
            if (rebase_ok) g_tk_rebase_ok++; else g_tk_rebase_bad++;
        } else {
            interp_ok = neb_biteq(val80, lerp_val);       /* entity+0x80 == lerp this tick */
            if (interp_ok) g_tk_interp_ok++; else g_tk_interp_bad++;
        }
        want = !bicond_ok || (act_complete && !rebase_ok) || (!act_complete && !interp_ok) ||
               act_complete || (g_tk_ticks <= 40);
    }

    int32_t nowt = (int32_t)sim_clock;

    /* HOLD (mode 2) part-2 validation: (4) z-bob pose entity+0x80 bit-exact, (5) damage-deadline
     * rebase bit-exact (proves the damage cadence is a pure self-clocked function — the gameplay
     * write is to SELF, only the event dispatch FUN_1402d5320 is the deferred cross-boundary seam). */
    int hold_pose_ok = 0, hold_sched_ok = 0, hold_want = 0;
    if (mode == 2) {
        hold_pose_ok  = neb_biteq(val80, hold_pred_z);
        if (hold_pose_ok)  g_tk_hold_pose_ok++;  else g_tk_hold_pose_bad++;
        hold_sched_ok = neb_biteq(nxt_after, hold_pred_nxt);
        if (hold_sched_ok) g_tk_hold_sched_ok++; else g_tk_hold_sched_bad++;
        if (hold_due) g_tk_hold_fires++;
        hold_want = !hold_pose_ok || !hold_sched_ok || hold_due || (g_tk_hold <= 40);
        if (hold_want && g_tk_hold_lines < 20000u) {
            g_tk_hold_lines++;
            char hl[400];
            snprintf(hl, sizeof hl,
                "DTTKHOLD\ttick=%d\tent=%llx\tspin_t=%.9g\tangle=%.9g\tdue=%d\tpose_ok=%d\tsched_ok=%d"
                "\tval80=%.9g\tpredz=%.9g\tnxt_after=%.9g\tpred_nxt=%.9g\n",
                nowt, (unsigned long long)entity, (double)hold_spin_t, (double)hold_angle, hold_due,
                hold_pose_ok, hold_sched_ok, (double)val80, (double)hold_pred_z,
                (double)nxt_after, (double)hold_pred_nxt);
            log_write(hl);
        }
    }

    /* RELEASE (mode 3) part-3 validation: on a completion edge the slot is torn down to its canonical inert
     * end-state. The shared path above already checked the biconditional + rebase; here we confirm the
     * RELEASE-SPECIFIC writes the lift predicts (63f470.c:39,46-48): mode terminus 0, the 0x3fffff sentinel,
     * and the damage disarm (amount slot+0x40 and deadline slot+0x48 both zeroed — HOLD's clock switched off).
     * The ability re-enable cascade + the GOM event dispatch FUN_1402d5320 that follow stay deferred (env). */
    int rel_term_ok = 0, rel_sent_ok = 0, rel_disarm_ok = 0, rel_want = 0;
    if (mode == 3 && act_complete) {
        g_tk_rel_edges++;
        rel_term_ok   = (mode_after == 0);
        if (rel_term_ok)   g_tk_rel_term_ok++;   else g_tk_rel_term_bad++;
        rel_sent_ok   = (sent_after == 0x3fffffu);
        if (rel_sent_ok)   g_tk_rel_sent_ok++;   else g_tk_rel_sent_bad++;
        rel_disarm_ok = (dmg_after == 0.0f) && (nxt_after == 0.0f);
        if (rel_disarm_ok) g_tk_rel_disarm_ok++; else g_tk_rel_disarm_bad++;
        rel_want = !rel_term_ok || !rel_sent_ok || !rel_disarm_ok || (g_tk_rel_edges <= 40);
        if (rel_want && g_tk_rel_lines < 20000u) {
            g_tk_rel_lines++;
            char rl[320];
            snprintf(rl, sizeof rl,
                "DTTKREL\ttick=%d\tent=%llx\tmode_after=%d\tterm_ok=%d\tsent=%#x\tsent_ok=%d"
                "\tdmg_after=%.9g\tnxt_after=%.9g\tdisarm_ok=%d\n",
                nowt, (unsigned long long)entity, mode_after, rel_term_ok, sent_after, rel_sent_ok,
                (double)dmg_after, (double)nxt_after, rel_disarm_ok);
            log_write(rl);
        }
    }

    if (want && g_tk_lines < 20000u) {
        g_tk_lines++;
        char ln[360];
        snprintf(ln, sizeof ln,
            "DTTK\ttick=%d\tent=%llx\tmode=%d\tmode_after=%d\tt=%.9g\tpred_comp=%d\tact_comp=%d"
            "\tbicond=%d\trebase_ok=%d\tinterp_ok=%d\tval80=%.9g\tlerp=%.9g\n",
            nowt, (unsigned long long)entity, mode, mode_after, (double)t, pred_complete, act_complete,
            bicond_ok, rebase_ok, interp_ok, (double)val80, (double)lerp_val);
        log_write(ln);
    }
    if (nowt != (int32_t)g_tk_surv_last && ((uint32_t)nowt & 0x1ffu) == 0) {
        g_tk_surv_last = (uint32_t)nowt;
        char sv[360];
        snprintf(sv, sizeof sv,
            "DTTKSURVEY\ttick=%d\tticks=%u\tnoslot=%u\tmode(grab/hold/rel/idle)=%u/%u/%u/%u"
            "\tinterp(ok/bad)=%u/%u\tcomp_edges=%u\tbicond(ok/bad)=%u/%u\trebase(ok/bad)=%u/%u"
            "\thold_pose(ok/bad)=%u/%u\thold_sched(ok/bad)=%u/%u\thold_fires=%u"
            "\trel_edges=%u\trel_term(ok/bad)=%u/%u\trel_sent(ok/bad)=%u/%u\trel_disarm(ok/bad)=%u/%u\n",
            nowt, g_tk_ticks, g_tk_noslot, g_tk_grab, g_tk_hold, g_tk_release, g_tk_idle,
            g_tk_interp_ok, g_tk_interp_bad, g_tk_comp_edges,
            g_tk_bicond_ok, g_tk_bicond_bad, g_tk_rebase_ok, g_tk_rebase_bad,
            g_tk_hold_pose_ok, g_tk_hold_pose_bad, g_tk_hold_sched_ok, g_tk_hold_sched_bad,
            g_tk_hold_fires,
            g_tk_rel_edges, g_tk_rel_term_ok, g_tk_rel_term_bad, g_tk_rel_sent_ok, g_tk_rel_sent_bad,
            g_tk_rel_disarm_ok, g_tk_rel_disarm_bad);
        log_write(sv);
    }
    return r;
}

static void install_b63f210_hook(void)
{
    void *t = NULL;
    if (install_targ_tramp(TK_63F210_RVA, tk_63f210_prologue, sizeof tk_63f210_prologue,
                           (void *)b63f210_hook, &t, "FUN_14063f210")) {
        g_tk_trampoline = (Tk63f210Fn)t;
        log_write("[eaw-mt] DTTK: TelekinesisTargetBehaviorClass interp-timeline oracle installed\n");
    }
}

/* =========================================================================
 * Runtime-watch offset dumper (decomp Phase 2) — find the entity transform offset.
 *
 * The entity world position is method-accessed, not a flat field; the engine stores
 * transforms as 4x3 matrices (12 floats, 0x30 stride). To pin the entity's matrix
 * offset empirically: latch one active entity, sample a byte window of it every
 * OW_PERIOD ticks, and log the float offsets that CHANGED since the last sample with
 * their per-sample delta. The world-position row reveals itself as a run of finite
 * floats with small, smooth deltas (movement); a 12-float 0x30-stride block = the
 * matrix. Profile-build only; runtime-gated by EAW_OFFWATCH=1. Grep "OFFWATCH".
 *
 * Window 0x40..0x478 is inside the confirmed object extent (movement code reads up to
 * entity+0x478), so the bounded read stays within a live active entity.
 * ========================================================================= */
/* v2: the entity's flat 0x40..0x478 window proved static in a moving battle — the live
 * transform is behind a pointer. So watch the entity window (level 0) AND follow every
 * plausible pointer slot in the entity one level deep (level 1), reporting the floats that
 * change. VirtualQuery-guard every followed read so a stale pointer can't fault. Rotate to
 * the next active entity if the watched one stays idle (don't get stuck on a static unit). */
#define OW_OFF0    0x40
#define OW_END     0x478
#define OW_WIN     (OW_END - OW_OFF0)
#define OW_PERIOD  8                     /* a76b0 ticks between samples (self-clocked) */
#define OW_ROUNDS  60
#define OW_SUB_LO  0x08                  /* entity pointer-slot scan range */
#define OW_SUB_HI  0x500
#define OW_SUBLEN  0x100                 /* bytes watched in each followed sub-object */
#define OW_NSLOTS  ((OW_SUB_HI - OW_SUB_LO) / 8)
#define OW_MAXLOG  200                   /* per-round changed-field log cap */
#define OW_SIG     0.01f                 /* |delta| above this = real motion (not jitter) */
static int      g_ow_enabled    = -1;
static int64_t  g_ow_coord      = 0;     /* latched coordinator (avoid multi-coord thrash) */
static int64_t  g_ow_ent        = 0;
static int      g_ow_entidx     = 0;     /* which active entity to latch (rotation) */
static int      g_ow_idle       = 0;     /* consecutive rounds with 0 changes */
static uint8_t  g_ow_prev[OW_WIN];
static int      g_ow_have_prev  = 0;
static uint32_t g_ow_ticks      = 0;     /* a76b0 calls on the latched coordinator */
static uint32_t g_ow_next       = 0;
static int      g_ow_rounds     = 0;
static int64_t  g_ow_subptr[OW_NSLOTS];          /* last pointer seen per slot */
static uint8_t  g_ow_subprev[OW_NSLOTS][OW_SUBLEN];
static uint8_t  g_ow_subhave[OW_NSLOTS];
static int64_t  g_ow_bestcoord  = 0;             /* coordinator with most active entities */
static int      g_ow_bestcnt    = 0;
static int      g_ow_surveyN    = 0;             /* a76b0 calls observed before latching a coord */
static int      g_ow_survey_pending = 0;

static int ow_on(void) {
    if (g_ow_enabled < 0) {
        const char *e = getenv("EAW_OFFWATCH");
        g_ow_enabled = (e && e[0] && e[0] != '0') ? 1 : 0;
        log_write(g_ow_enabled
            ? "[eaw-mt] OFFWATCH: enabled (entity transform-offset finder, 2-level)\n"
            : "[eaw-mt] OFFWATCH: off (set EAW_OFFWATCH=1 to hunt the transform offset)\n");
    }
    return g_ow_enabled;
}
static inline int ow_finite(float f) {       /* exclude NaN/Inf */
    uint32_t b; memcpy(&b, &f, 4);
    return ((b >> 23) & 0xff) != 0xff;
}
static void ow_reset_subs(void) { memset(g_ow_subhave, 0, sizeof g_ow_subhave); }
/* fault-free readability test for a followed pointer */
static int ow_readable(const void *p, size_t n) {
    MEMORY_BASIC_INFORMATION mbi;
    if (VirtualQuery(p, &mbi, sizeof mbi) == 0) return 0;
    if (mbi.State != MEM_COMMIT) return 0;
    if (mbi.Protect & (PAGE_NOACCESS | PAGE_GUARD)) return 0;
    uintptr_t end = (uintptr_t)mbi.BaseAddress + mbi.RegionSize;
    if ((uintptr_t)p + n > end) return 0;
    return 1;
}
/* k-th active entity in the coordinator's list, or 0 */
static int64_t ow_nth_active(int64_t coord, int k) {
    if (!coord || (*(uint8_t *)(coord + 0x3a0) & 0x40)) return 0;
    int64_t L = *(int64_t *)(coord + 0x2d0);
    if (!L) return 0;
    int32_t n = *(int32_t *)(L + 0x10);
    int64_t arr = *(int64_t *)(L + 0x8);
    if (!arr || n <= 0 || n > 1000000) return 0;
    int cnt = 0;
    for (int i = 0; i < n; i++) {
        int64_t e = *(int64_t *)(arr + (int64_t)i * 8);
        if (!e) continue;
        int64_t ent = *(int64_t *)(e + 0x20);
        if (ent && *(int8_t *)(ent + 0x4d) == 1) { if (cnt == k) return ent; cnt++; }
    }
    return 0;
}
static int ow_present(int64_t coord, int64_t ent) {
    if (!coord || (*(uint8_t *)(coord + 0x3a0) & 0x40)) return 0;
    int64_t L = *(int64_t *)(coord + 0x2d0);
    if (!L) return 0;
    int32_t n = *(int32_t *)(L + 0x10);
    int64_t arr = *(int64_t *)(L + 0x8);
    if (!arr || n <= 0 || n > 1000000) return 0;
    for (int i = 0; i < n; i++) {
        int64_t e = *(int64_t *)(arr + (int64_t)i * 8);
        if (e && *(int64_t *)(e + 0x20) == ent && *(int8_t *)(ent + 0x4d) == 1) return 1;
    }
    return 0;
}
static int ow_active_count(int64_t coord) {
    if (!coord || (*(uint8_t *)(coord + 0x3a0) & 0x40)) return 0;
    int64_t L = *(int64_t *)(coord + 0x2d0);
    if (!L) return 0;
    int32_t n = *(int32_t *)(L + 0x10);
    int64_t arr = *(int64_t *)(L + 0x8);
    if (!arr || n <= 0 || n > 1000000) return 0;
    int cnt = 0;
    for (int i = 0; i < n; i++) {
        int64_t e = *(int64_t *)(arr + (int64_t)i * 8);
        if (!e) continue;
        int64_t ent = *(int64_t *)(e + 0x20);
        if (ent && *(int8_t *)(ent + 0x4d) == 1) cnt++;
    }
    return cnt;
}
/* One-time structural dump of the latched entity: pointer slots + coordinate-like floats.
 * Lets us reason about layout offline even if the live-diff misses the position. */
static void ow_dump_survey(void) {
    char b[128];
    snprintf(b, sizeof b, "OWSURVEY ent=%p coord=%p nactive=%d\n",
             (void *)g_ow_ent, (void *)g_ow_coord, ow_active_count(g_ow_coord));
    log_write(b);
    for (int slot = OW_SUB_LO; slot < OW_SUB_HI; slot += 8) {
        if (!ow_readable((void *)(g_ow_ent + slot), 8)) continue;
        int64_t ptr; memcpy(&ptr, (void *)(g_ow_ent + slot), 8);
        uint64_t up = (uint64_t)ptr;
        if ((up & 3) || up < 0x10000 || up > 0x00007fffffffffffULL) continue;
        if (!ow_readable((void *)ptr, 8)) continue;
        snprintf(b, sizeof b, "  ptr@0x%x=%p\n", slot, (void *)ptr);
        log_write(b);
    }
    for (int off = 0; off < OW_SUB_HI; off += 4) {
        if (!ow_readable((void *)(g_ow_ent + off), 4)) continue;
        float f; memcpy(&f, (void *)(g_ow_ent + off), 4);
        if (!ow_finite(f)) continue;
        float af = f < 0 ? -f : f;
        if (af < 1.0f || af > 1e7f) continue;          /* coordinate-like magnitude */
        snprintf(b, sizeof b, "  f@0x%x=%.2f\n", off, (double)f);
        log_write(b);
    }
}
/* Report changed finite floats between two buffers. Logs only floats that moved by > OW_SIG
 * (jitter near 0 is ignored). Flags a "VEC3?" when 3 consecutive 4-byte slots all moved — that
 * is the position/transform signature. *sig accumulates the count of real (>OW_SIG) movers (used
 * to decide whether the entity is actually moving, for rotation). Returns total significant movers. */
static int ow_diff(const uint8_t *cur, const uint8_t *prev, int len,
                   const char *tag, int slot, int *logbudget, int *sig) {
    int moved = 0, run = 0;
    for (int o = 0; o < len; o += 4) {
        float cf, pf;
        memcpy(&cf, cur + o, 4);
        memcpy(&pf, prev + o, 4);
        if (!ow_finite(cf) || cf < -1e9f || cf > 1e9f) { run = 0; continue; }
        float d = cf - pf;
        float ad = d < 0 ? -d : d;
        if (ad <= OW_SIG) { run = 0; continue; }         /* unchanged or jitter → breaks vec3 run */
        moved++; (*sig)++;
        run++;
        if (*logbudget > 0) {
            char ln[120];
            if (slot < 0)
                snprintf(ln, sizeof ln, "  %s off=0x%x val=%.3f d=%.4f%s\n",
                         tag, OW_OFF0 + o, (double)cf, (double)d, run >= 3 ? "  <-VEC3?" : "");
            else
                snprintf(ln, sizeof ln, "  %s slot=0x%x+0x%x val=%.3f d=%.4f%s\n",
                         tag, slot, o, (double)cf, (double)d, run >= 3 ? "  <-VEC3?" : "");
            log_write(ln);
            (*logbudget)--;
        }
    }
    return moved;
}
static void ow_sample(int64_t coord) {
    if (g_ow_rounds >= OW_ROUNDS) return;
    /* latch the coordinator with the MOST active entities (over the first 8 a76b0 calls),
     * so entity rotation has room to find a mover (a 1-entity coordinator traps rotation) */
    if (!g_ow_coord) {
        int c = ow_active_count(coord);
        if (c > g_ow_bestcnt) { g_ow_bestcnt = c; g_ow_bestcoord = coord; }
        if (++g_ow_surveyN >= 8) { if (!g_ow_bestcoord) return; g_ow_coord = g_ow_bestcoord; }
        else return;
    }
    if (coord != g_ow_coord) return;
    g_ow_ticks++;                                  /* self-clock: a76b0 call count, not 0xb0a320 */
    if (g_ow_ent && !ow_present(coord, g_ow_ent)) { g_ow_ent = 0; g_ow_have_prev = 0; }
    if (!g_ow_ent) {
        g_ow_ent = ow_nth_active(coord, g_ow_entidx);
        if (!g_ow_ent) { g_ow_entidx = 0; g_ow_ent = ow_nth_active(coord, 0); }
        if (!g_ow_ent) { g_ow_coord = 0; return; }
        g_ow_have_prev = 0; ow_reset_subs(); g_ow_survey_pending = 1; g_ow_next = g_ow_ticks + 2;
        return;
    }
    if (g_ow_ticks < g_ow_next) return;
    g_ow_next = g_ow_ticks + OW_PERIOD;

    if (g_ow_survey_pending) { ow_dump_survey(); g_ow_survey_pending = 0; }

    uint8_t cur[OW_WIN];
    memcpy(cur, (void *)(g_ow_ent + OW_OFF0), OW_WIN);   /* entity is present+active: safe */
    if (g_ow_have_prev) {
        uint32_t fc = g_dt_frame_ctr ? *g_dt_frame_ctr : 0;
        int budget = OW_MAXLOG, sig = 0;
        char hdr[112];
        snprintf(hdr, sizeof hdr, "OFFWATCH round=%d atick=%u fctr=%u idx=%d ent=%p\n",
                 g_ow_rounds, g_ow_ticks, fc, g_ow_entidx, (void *)g_ow_ent);
        log_write(hdr);
        ow_diff(cur, g_ow_prev, OW_WIN, "ent", -1, &budget, &sig);   /* level 0 */
        /* level 1: follow each plausible pointer slot one deep */
        for (int slot = OW_SUB_LO; slot < OW_SUB_HI; slot += 8) {
            int idx = (slot - OW_SUB_LO) / 8;
            if (!ow_readable((void *)(g_ow_ent + slot), 8)) { g_ow_subhave[idx] = 0; continue; }
            int64_t ptr; memcpy(&ptr, (void *)(g_ow_ent + slot), 8);
            uint64_t up = (uint64_t)ptr;
            if ((up & 3) || up < 0x10000 || up > 0x00007fffffffffffULL ||
                !ow_readable((void *)ptr, OW_SUBLEN)) { g_ow_subhave[idx] = 0; continue; }
            uint8_t sb[OW_SUBLEN];
            memcpy(sb, (void *)ptr, OW_SUBLEN);
            if (g_ow_subhave[idx] && g_ow_subptr[idx] == ptr)
                ow_diff(sb, g_ow_subprev[idx], OW_SUBLEN, "sub", slot, &budget, &sig);
            memcpy(g_ow_subprev[idx], sb, OW_SUBLEN);
            g_ow_subptr[idx] = ptr; g_ow_subhave[idx] = 1;
        }
        g_ow_rounds++;
        if (sig == 0) {                            /* no real motion: rotate to the next entity */
            if (++g_ow_idle >= 2) {
                g_ow_entidx++; g_ow_ent = 0; g_ow_have_prev = 0; ow_reset_subs(); g_ow_idle = 0;
            }
        } else g_ow_idle = 0;
    }
    memcpy(g_ow_prev, cur, OW_WIN);
    g_ow_have_prev = 1;
}
#endif /* EAW_PROFILE */

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

#ifdef EAW_PROFILE
    if (!g_dt_frame_ctr) {                                 /* shared by DIFFTRACE + OFFWATCH */
        HMODULE exe = GetModuleHandleA(NULL);
        if (exe) { g_dt_frame_ctr = (volatile uint32_t *)((BYTE *)exe + 0xb0a320);
                   g_dt_imgbase = (uintptr_t)exe; }
    }
    if (dt_on()) {
        uint32_t g = g_dt_frame_ctr ? *g_dt_frame_ctr : 0;
        if (g != g_dt_tick) {                              /* new sim tick: flush previous */
            if (g_dt_tick != 0xFFFFFFFFu) dt_emit();
            g_dt_tick = g; g_dt_hash = DT_FNV_BASIS; g_dt_count = 0;
            g_dt_pos_n = 0; g_dt_pos_have = 0;
            g_dt_loco_have = 0; g_dt_lstate = -1; g_dt_loco_rva = 0; g_dt_timer = -1;
            g_dt_ss_have = 0; g_dt_ss_state = -1; g_dt_spl_have = 0; g_dt_spl_idx = -1;
        }
        dt_fold_coordinator(a);                            /* aggregate all coordinators/tick */
    }
    if (ow_on()) ow_sample(a);                             /* transform-offset finder */
#endif
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
    /* Functional (Fix B2): reset the per-component opp-target budget before each call.
     * b385190_hook gates on g_opp_per_call, so this reset is required in every build. */
    g_opp_per_call = 0;
#ifdef EAW_PROFILE
    LARGE_INTEGER t0, t1;
    tgt_fake_qpc(&t0);
    g_b387400_orig(a, b);
    tgt_fake_qpc(&t1);
    double ms = (t1.QuadPart - t0.QuadPart) / ((double)g_qpc_freq.QuadPart / 1000.0);
    g_b387400_sum_ms += ms;
    if (ms > g_b387400_max_ms) g_b387400_max_ms = ms;
    g_b387400_count++;
#else
    g_b387400_orig(a, b);
#endif
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
        /* 387400 reset-wrapper is functional (Fix B2) — always wired. The 385cf0/381ff0
         * redirects are timing-only and compiled out of the release build. */
        if      (target == fn_b387400) { stub = stub_b387400; cnt = &n387400; }
#ifdef EAW_PROFILE
        else if (target == fn_b385cf0) { stub = stub_b385cf0; cnt = &n385cf0; }
        else if (target == fn_b381ff0) { stub = stub_b381ff0; cnt = &n381ff0; }
#endif
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
    /* Functional (Fix B2): cap opp-target search candidates per component. */
    if (g_opp_per_call >= OPP_SEARCH_CAP) {
#ifdef EAW_PROFILE
        InterlockedIncrement(&g_b385190_capped);
#endif
        return (int64_t *)0;  /* budget exhausted — short-circuit the search */
    }
    g_opp_per_call++;
#ifdef EAW_PROFILE
    LARGE_INTEGER t0, t1;
    tgt_fake_qpc(&t0);
    int64_t *r = g_b385190_orig(component, candidate, score);
    tgt_fake_qpc(&t1);
    double ms = (t1.QuadPart - t0.QuadPart) / ((double)g_qpc_freq.QuadPart / 1000.0);
    g_b385190_sum_ms += ms;
    if (ms > g_b385190_max_ms) g_b385190_max_ms = ms;
    InterlockedIncrement(&g_b385190_count);
    return r;
#else
    return g_b385190_orig(component, candidate, score);
#endif
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

#if defined(EAW_PROFILE) || defined(EAW_ORACLE)
/* a1 SFX takeover installer (inproc_integration_milestone.md §2): repoint every FUN_1402d5290 call site
 * inside the KNOWN return-ignoring callers to a1_sfx_intercept. The three decompiled callers cover the
 * combat-hot SFX classes: 387400:99 (movement cue, cold in the menu demo), 386660:71 (death/destruction
 * cue — fires as ships die, the hot one), 42f460:49 (ability-activation cue). All three discard the
 * return value, so suppressing the live call while buffering is safe. The 2d5290 body stays intact and
 * serves as the replay path (g_a1_sfx_real). Inert until EAW_A1=1 — see a1_sfx_intercept. */
#define A1_SFX_RVA 0x2d5290ULL
static const struct { uint64_t rva; int size; const char *name; } g_a1_sfx_callers[] = {
    { B387400_RVA, B387400_BODY_SIZE, "387400" },   /* movement cue */
    { 0x386660ULL, 1057,              "386660" },   /* death/destruction cue (hot) */
    { 0x42f460ULL,  410,              "42f460" },   /* ability-activation cue */
};
static BOOL install_a1_sfx_hook(void) {
    HMODULE exe = GetModuleHandleA(NULL);
    if (!exe) return FALSE;
    if (!g_dt_imgbase) g_dt_imgbase = (uintptr_t)exe;
    BYTE *fn2d5290 = (BYTE *)exe + A1_SFX_RVA;
    g_a1_sfx_real  = (EngSfxFn)fn2d5290;             /* replay path: untouched 2d5290 body */

    BYTE *stub = alloc_near((BYTE *)exe + B387400_RVA, 14);
    if (!stub) { log_write("[eaw-mt] WARN: alloc_near failed for a1 SFX stub\n"); return FALSE; }
    write_abs_jmp(stub, (uint64_t)a1_sfx_intercept);

    int total = 0; DWORD old; char m[160];
    for (size_t c = 0; c < sizeof g_a1_sfx_callers / sizeof g_a1_sfx_callers[0]; c++) {
        BYTE *fn = (BYTE *)exe + g_a1_sfx_callers[c].rva;
        int n = 0;
        for (int i = 0; i <= g_a1_sfx_callers[c].size - 5; i++) {
            if (fn[i] != 0xE8) continue;
            int32_t rel; memcpy(&rel, fn + i + 1, 4);
            if (fn + i + 5 + rel != fn2d5290) continue;
            int32_t new_rel = (int32_t)(stub - (fn + i + 5));
            VirtualProtect(fn + i + 1, 4, PAGE_EXECUTE_READWRITE, &old);
            memcpy(fn + i + 1, &new_rel, 4);
            VirtualProtect(fn + i + 1, 4, old, &old);
            FlushInstructionCache(GetCurrentProcess(), fn + i, 5);
            n++;
        }
        sprintf(m, "[eaw-mt] A1-SFX: %s — %d 2d5290 site(s) repointed\n", g_a1_sfx_callers[c].name, n);
        log_write(m);
        total += n;
    }
    sprintf(m, "[eaw-mt] A1-SFX: %d total call site(s) repointed (EAW_A1=1 to arm buffering)\n", total);
    log_write(m);
    return total > 0;
}
#endif /* EAW_PROFILE || EAW_ORACLE */

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

static uint32_t djb2(const char *s);  /* defined below (nf_cache section) */

/* FUN_14025ec10 — art-model object lookup by path string.
 * 2 args: (char *path, char *filter) — confirmed by decompile. */
typedef int64_t (*B25ec10Fn)(int64_t, int64_t);
static B25ec10Fn g_b25ec10_orig  = NULL;
static LONG      g_b25ec10_count = 0;
static double    g_b25ec10_sum_ms = 0, g_b25ec10_max_ms = 0;

/* Result cache (the real Source-B fix). Each call runs a ~119-iteration probe loop
 * (b141f70 ×~105/call); nf_cache makes each probe fast but the loop itself runs on
 * every movement order (~9.6k b141f70 calls/window) — the residual lag, worst in
 * fast-forward. Memoizing the whole result per (path,filter) skips the loop entirely
 * on repeat. Correctness: store the full key string and strcmp (no hash-collision →
 * wrong-object risk); flush the whole cache when the asset-DB pointer (DAT_140a62700)
 * changes (battle reload), so we never hand back a stale/freed registry object.
 * Negative results (r==0) are cached too — that's the empty-animation lag case.
 * Kill switch: EAW_NO_B25CACHE=1. */
#define B25_SLOTS 16384            /* power of 2 */
#define B25_KEYLEN 160
static char     g_b25_key[B25_SLOTS][B25_KEYLEN];
static int64_t  g_b25_result[B25_SLOTS];
static uint8_t  g_b25_used[B25_SLOTS];
static int64_t  g_b25_db_seen   = 0;   /* asset-DB ptr the cache is valid for */
static LONG     g_b25_hits = 0, g_b25_inserts = 0;
static int      g_b25cache_off  = -1;

static int64_t b25ec10_hook(int64_t a, int64_t b) {
    if (g_b25cache_off < 0) {
        const char *e = getenv("EAW_NO_B25CACHE");
        g_b25cache_off = (e && e[0] && e[0] != '0') ? 1 : 0;
        log_write(g_b25cache_off
            ? "[eaw-mt] B25CACHE: DISABLED via EAW_NO_B25CACHE\n"
            : "[eaw-mt] B25CACHE: enabled (set EAW_NO_B25CACHE=1 to disable)\n");
    }
    const char *path = (const char *)a;
    const char *filt = (const char *)b;

    /* Build the lookup key "path|filter"; bail to passthrough if too long. */
    char key[B25_KEYLEN];
    int usable = 0;
    if (!g_b25cache_off && path) {
        int n = snprintf(key, sizeof(key), "%s|%s", path, filt ? filt : "");
        if (n > 0 && n < (int)sizeof(key)) usable = 1;
    }

    if (usable) {
        /* Flush on asset-DB change (battle reload) — cached pointers would be stale. */
        int64_t db = *(int64_t *)((BYTE *)GetModuleHandleA(NULL) + 0xA62700ULL);
        if (db != g_b25_db_seen) { memset(g_b25_used, 0, sizeof(g_b25_used)); g_b25_db_seen = db; }

        uint32_t kh = djb2(key);
        uint32_t slot = kh & (B25_SLOTS - 1);
        for (int i = 0; i < 24; i++) {
            uint32_t s = (slot + i) & (B25_SLOTS - 1);
            if (!g_b25_used[s]) break;
            if (strcmp(g_b25_key[s], key) == 0) {       /* exact-string hit */
                InterlockedIncrement(&g_b25_hits);
                g_b25ec10_count++;
                return g_b25_result[s];
            }
        }
        /* miss → run the real (slow) lookup once, then insert */
        LARGE_INTEGER t0, t1; tgt_fake_qpc(&t0);
        int64_t r = g_b25ec10_orig(a, b); tgt_fake_qpc(&t1);
        double ms = (t1.QuadPart - t0.QuadPart) / ((double)g_qpc_freq.QuadPart / 1000.0);
        g_b25ec10_sum_ms += ms; if (ms > g_b25ec10_max_ms) g_b25ec10_max_ms = ms;
        g_b25ec10_count++;
        for (int i = 0; i < 24; i++) {
            uint32_t s = (slot + i) & (B25_SLOTS - 1);
            if (!g_b25_used[s] || strcmp(g_b25_key[s], key) == 0) {
                memcpy(g_b25_key[s], key, sizeof(key));
                g_b25_result[s] = r; g_b25_used[s] = 1;
                InterlockedIncrement(&g_b25_inserts);
                break;
            }
        }
        return r;
    }

    /* passthrough (cache off / unusable key) */
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
    /* Kill switch: EAW_NO_MEGSKIP=1 forces meg_has to always allow the real scan,
     * disabling the index skip (back to pre-Fix-E behavior) without a rebuild. */
    static int s_megskip_off = -1;
    if (s_megskip_off < 0) {
        const char *e = getenv("EAW_NO_MEGSKIP");
        s_megskip_off = (e && e[0] && e[0] != '0') ? 1 : 0;
        log_write(s_megskip_off
            ? "[eaw-mt] MEGSKIP: DISABLED via EAW_NO_MEGSKIP (index skip off)\n"
            : "[eaw-mt] MEGSKIP: enabled (set EAW_NO_MEGSKIP=1 to disable)\n");
    }
    if (s_megskip_off) return TRUE;
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

/* Recursively index every *.meg under dir_path. Completeness matters: b2136f0_hook
 * skips the MEG scan for paths absent from the index, so a MEG the index misses would
 * cause false-absent (missing asset). The game keeps MEGs at Data\*.meg AND deeper
 * (e.g. Data\Audio\SFX\*.meg), so a flat scan is not enough. */
static void scan_meg_tree(const char *dir_path, int *meg_count, int depth) {
    if (depth > 6) return;
    scan_meg_dir(dir_path, meg_count);
    char pattern[MAX_PATH];
    snprintf(pattern, MAX_PATH, "%s\\*", dir_path);
    WIN32_FIND_DATAA fd;
    HANDLE h = FindFirstFileA(pattern, &fd);
    if (h == INVALID_HANDLE_VALUE) return;
    do {
        if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) continue;
        if (fd.cFileName[0] == '.') continue;          /* skip . and .. */
        char sub[MAX_PATH];
        snprintf(sub, MAX_PATH, "%s\\%s", dir_path, fd.cFileName);
        scan_meg_tree(sub, meg_count, depth + 1);
    } while (FindNextFileA(h, &fd));
    FindClose(h);
}

static void build_meg_index(void) {
    /* Derive the game directory from the EXE path, NOT the CWD: build_meg_index runs
     * at DLL init, before the game chdir's to its own folder, so GetCurrentDirectoryA
     * returns the launch dir (e.g. the repo) and `<cwd>\Data\*.meg` finds nothing.
     * GetModuleFileNameA(NULL) is the StarWarsG.exe path; strip the filename to get
     * the game dir (…\corruption), which holds Data\*.meg regardless of CWD timing. */
    char dir[MAX_PATH];
    DWORD exe_len = GetModuleFileNameA(NULL, dir, MAX_PATH);
    if (exe_len == 0 || exe_len >= MAX_PATH) {
        GetCurrentDirectoryA(MAX_PATH, dir);  /* fallback */
    } else {
        char *bs = strrchr(dir, '\\');
        if (bs) *bs = '\0'; else GetCurrentDirectoryA(MAX_PATH, dir);
    }
    int meg_count = 0;

    /* Base game MEGs: recurse all of <game>\Data (catches Data\Audio\SFX\*.meg etc.). */
    char data_dir[MAX_PATH];
    snprintf(data_dir, MAX_PATH, "%s\\Data", dir);
    scan_meg_tree(data_dir, &meg_count, 0);

    /* Active mod MEGs: <game>\Mods\*\Data (recursive). The current mod
     * (Imperial_Civil_War) ships loose files today, but a mod that packs MEGs would
     * otherwise be missed → false-absent. Scan every mod's Data tree to be safe. */
    char mods_pat[MAX_PATH];
    snprintf(mods_pat, MAX_PATH, "%s\\Mods\\*", dir);
    WIN32_FIND_DATAA mfd;
    HANDLE mh = FindFirstFileA(mods_pat, &mfd);
    if (mh != INVALID_HANDLE_VALUE) {
        do {
            if (!(mfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) continue;
            if (mfd.cFileName[0] == '.') continue;
            char mod_data[MAX_PATH];
            snprintf(mod_data, MAX_PATH, "%s\\Mods\\%s\\Data", dir, mfd.cFileName);
            scan_meg_tree(mod_data, &meg_count, 0);
        } while (FindNextFileA(mh, &mfd));
        FindClose(mh);
    }

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
    /* FUNCTIONAL: reset the per-gsvc Lua AI budget at the start of each service
     * pass — Fix B3's pumpe_hook accumulator depends on this. This hook is kept
     * in the release build (installed unconditionally) for the reset alone. */
    g_pumpe_frame_used_ms = 0.0;
#ifdef EAW_PROFILE
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
#else
    g_gsvc_orig(a, b);
#endif
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
            "  pumpe(247a90):   avg=%.2f max=%.2f ms (n=%ld skip=%ld bg=%ld bg_max=%.0fms wd=%ld rv_n=%ld rv_max=%ldms rv_slow=%ld rv_abort=%ld)\n"
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
            "  b25ec10(25ec10): avg=%.2f max=%.2f ms (n=%ld cache_hits=%ld ins=%ld)\n"
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
            (long)g_resume_n, (long)g_resume_max_ms, (long)g_resume_slow, (long)g_resume_abort,
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
            (long)g_b25_hits, (long)g_b25_inserts,
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
    InterlockedExchange(&g_resume_abort, 0);
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
#ifndef EAW_PROFILE
    /* RELEASE: pure passthrough. Render offload is dormant and the per-frame
     * profiling/sim-tick instrumentation is measurement-only. */
    g_trampoline(param_1);
}
#else
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
#endif /* EAW_PROFILE */


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
        /* Corrupted wake_addr signature: junk in the upper dword, lower 32 bits = 0
         * (a real waiter-byte pointer is never 4GB-aligned). Covers the original
         * 0x6fff00000000 race AND other corrupt values e.g. 0x2ad00000000 seen at
         * the 0x3a9e7d thunk — the inner instruction-pattern checks are the 2nd gate. */
        if ((bad & 0xFFFFFFFFULL) == 0 && (bad >> 32) != 0) {
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

    /* alHModel ctor null render-pass element guard (FUN_14012b330 rva=0x12b6bf).
     * Fires during prewarm AND on the first live pump of the same entity.
     * Null element at [RCX+0] where RCX was loaded from param_2+0xf0 array +8 slot.
     * Skip both faulting MOV and the following CALL; store NULL for that slot. */
    if (code == EXCEPTION_ACCESS_VIOLATION &&
        ep->ExceptionRecord->NumberParameters >= 2 &&
        ep->ExceptionRecord->ExceptionInformation[0] == 0 /* read */ &&
        ep->ExceptionRecord->ExceptionInformation[1] == 0 /* fault_addr == null */) {
        uint64_t img_base_rp = (uint64_t)GetModuleHandleA(NULL);
        uint64_t rip_rp      = (uint64_t)ep->ContextRecord->Rip;
        if (img_base_rp && rip_rp == img_base_rp + ALMODEL_NULL_RP_RVA) {
            ep->ContextRecord->Rax  = 0;
            ep->ContextRecord->Rip += ALMODEL_NULL_RP_SKIP;
            static volatile LONG g_almodel_rp_suppressed = 0;
            LONG n = InterlockedIncrement(&g_almodel_rp_suppressed);
            char s[96];
            snprintf(s, sizeof(s),
                "[eaw-mt] ALMODEL_NULL_RP #%ld prewarm=%d -> null slot\n",
                (long)n, (int)g_in_prewarm_pumpe);
            if (g_log_fp) { fputs(s, g_log_fp); fflush(g_log_fp); }
            return EXCEPTION_CONTINUE_EXECUTION;
        }
    }

    /* Step 35: alHModel ctor null render-pass back-ref guard (rva=0x12b6fb).
     * Different loop than ALMODEL_NULL_RP: MOV [RCX+0xb0],R14 with RCX=null →
     * av_write at fault_addr 0xb0.  Skip the 7-byte store and continue the loop. */
    if (code == EXCEPTION_ACCESS_VIOLATION &&
        ep->ExceptionRecord->NumberParameters >= 2 &&
        ep->ExceptionRecord->ExceptionInformation[0] == 1 /* write */ &&
        ep->ExceptionRecord->ExceptionInformation[1] == 0xb0 /* RCX+0xb0, RCX=null */) {
        uint64_t img_base_rp2 = (uint64_t)GetModuleHandleA(NULL);
        uint64_t rip_rp2      = (uint64_t)ep->ContextRecord->Rip;
        if (img_base_rp2 && rip_rp2 == img_base_rp2 + ALMODEL_NULL_RP2_RVA) {
            ep->ContextRecord->Rip += ALMODEL_NULL_RP2_SKIP;
            static volatile LONG g_almodel_rp2_suppressed = 0;
            LONG n = InterlockedIncrement(&g_almodel_rp2_suppressed);
            char s[96];
            snprintf(s, sizeof(s),
                "[eaw-mt] ALMODEL_NULL_RP2 #%ld prewarm=%d -> skip back-ref\n",
                (long)n, (int)g_in_prewarm_pumpe);
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

#if defined(EAW_PROFILE) || defined(EAW_ORACLE)
/* =========================================================================
 * A2MEASURE — a2.0 Amdahl measurement (a2_integration_scope.md §2).
 *
 * The serial-gate fallback keeps the fire-control subtree SERIAL and parallelizes the cheap-mass. To know
 * if the speedup is worth it we time, per tick, the per-object body FUN_1403a6b80 (which contains the
 * whole update) vs its fire-control call FUN_1403a76b0 (3a76b0 → 387400 → 3825b0, the part that stays
 * serial). The parallelizable fraction is p = (body - fire)/body; the Amdahl ceiling is 1/((1-p)+p/N).
 *
 * Lean QPC timers ONLY: gated by EAW_A2MEASURE=1, which installs JUST these two entry-detours and SKIPS
 * every DT hook, so no instrumentation overhead confounds the timing. body times include fire times
 * (3a6b80 calls 3a76b0), so cheap = body - fire is the cheap-mass time. Cumulative (time-weighted) ratio.
 * 3a6b80 prologue (15B, position-independent): 48 8b c4 / 56 / 48 81 ec f0 00 00 00 / 0f 29 78 b8
 *   = mov rax,rsp; push rsi; sub rsp,0xf0; movaps [rax-0x48],xmm7. */
#define A2M_A6B80_RVA 0x3a6b80ULL
static const BYTE a2m_a6b80_prologue[15] = {
    0x48,0x8b,0xc4, 0x56, 0x48,0x81,0xec,0xf0,0x00,0x00,0x00, 0x0f,0x29,0x78,0xb8
};
typedef void (*A2mBodyFn)(void*, uint32_t, char);
typedef void (*A2mFireFn)(int64_t, int32_t);
static A2mBodyFn g_a2m_body_tramp=NULL;
static A2mFireFn g_a2m_fire_tramp=NULL;
static LARGE_INTEGER g_a2m_qpf;
static long long g_a2m_body_qpc=0, g_a2m_fire_qpc=0;
static long g_a2m_body_calls=0, g_a2m_fire_calls=0;
static uint32_t g_a2m_last_log=0xFFFFFFFFu;
static int g_a2m_enabled=-1;

/* §8.12 lift-vs-lock gate: time the two racy global-scratch leaves on the fire path (399450 lead solver,
 * 35f470 fog) as a fraction of the fire-control subtree (a76b0) — i.e. how much of the parallelizable
 * fire work locking them would serialize. Both are sub-calls of a76b0, so their time is inside fire_ms.
 * 15-byte position-independent prologues (objdump): boundary before the first RIP-relative access. */
#define A2M_399450_RVA 0x399450ULL
#define A2M_35F470_RVA 0x35f470ULL
static const BYTE a2m_399450_prologue[15] = {
    0x48,0x8b,0xc4, 0x48,0x89,0x58,0x10, 0x48,0x89,0x70,0x20, 0x55, 0x57, 0x41,0x57
};
static const BYTE a2m_35f470_prologue[15] = {
    0x48,0x89,0x5c,0x24,0x08, 0x48,0x89,0x6c,0x24,0x10, 0x48,0x89,0x74,0x24,0x18
};
typedef float*  (*A2mSolverFn)(int64_t, float*, int64_t, float*, float*, float);
typedef int64_t (*A2mFogFn)(int64_t, int32_t, int64_t, int8_t);
static A2mSolverFn g_a2m_solver_tramp=NULL;
static A2mFogFn    g_a2m_fog_tramp=NULL;
static long long g_a2m_solver_qpc=0, g_a2m_fog_qpc=0;
static long g_a2m_solver_calls=0, g_a2m_fog_calls=0;

static int a2measure_on(void) {
    if (g_a2m_enabled<0) {
        const char* e=getenv("EAW_A2MEASURE");
        g_a2m_enabled=(e&&e[0]&&e[0]!='0')?1:0;
        log_write(g_a2m_enabled
            ? "[eaw-mt] A2MEASURE: ON — timing 3a6b80 body vs 3a76b0 fire-control (DT hooks skipped)\n"
            : "[eaw-mt] A2MEASURE: off (EAW_A2MEASURE=1 to measure the parallel fraction)\n");
    }
    return g_a2m_enabled;
}

static void a2m_fire_timer(int64_t ship, int32_t t) {
    LARGE_INTEGER a,b; QueryPerformanceCounter(&a);
    g_a2m_fire_tramp(ship,t);
    QueryPerformanceCounter(&b);
    g_a2m_fire_qpc += (b.QuadPart-a.QuadPart); g_a2m_fire_calls++;
}

static float* a2m_solver_timer(int64_t p1, float* p2, int64_t p3, float* p4, float* p5, float p6) {
    LARGE_INTEGER a,b; QueryPerformanceCounter(&a);
    float* r = g_a2m_solver_tramp(p1,p2,p3,p4,p5,p6);
    QueryPerformanceCounter(&b);
    g_a2m_solver_qpc += (b.QuadPart-a.QuadPart); g_a2m_solver_calls++;
    return r;
}

static int64_t a2m_fog_timer(int64_t p1, int32_t p2, int64_t p3, int8_t p4) {
    LARGE_INTEGER a,b; QueryPerformanceCounter(&a);
    int64_t r = g_a2m_fog_tramp(p1,p2,p3,p4);
    QueryPerformanceCounter(&b);
    g_a2m_fog_qpc += (b.QuadPart-a.QuadPart); g_a2m_fog_calls++;
    return r;
}

static void a2m_body_timer(void* obj, uint32_t p2, char p3) {
    LARGE_INTEGER a,b; QueryPerformanceCounter(&a);
    g_a2m_body_tramp(obj,p2,p3);
    QueryPerformanceCounter(&b);
    g_a2m_body_qpc += (b.QuadPart-a.QuadPart); g_a2m_body_calls++;

    if (!g_dt_frame_ctr) {
        HMODULE exe=GetModuleHandleA(NULL);
        if (exe){ g_dt_frame_ctr=(volatile uint32_t*)((BYTE*)exe+0xb0a320); g_dt_imgbase=(uintptr_t)exe; }
    }
    if (g_dt_frame_ctr && g_a2m_qpf.QuadPart>0) {
        uint32_t now=*g_dt_frame_ctr;
        if (now!=g_a2m_last_log && (now & 0x1ffu)==0) {   /* every 512 ticks */
            g_a2m_last_log=now;
            double per_ms=(double)g_a2m_qpf.QuadPart/1000.0;
            double body=(double)g_a2m_body_qpc/per_ms, fire=(double)g_a2m_fire_qpc/per_ms;
            double cheap=body-fire;
            double p = body>0.0 ? cheap/body : 0.0;
            double a2x=1.0/((1.0-p)+p/2.0), a4x=1.0/((1.0-p)+p/4.0), a8x=1.0/((1.0-p)+p/8.0);
            double ainf = p<1.0 ? 1.0/(1.0-p) : 999.0;
            char ln[360];
            snprintf(ln,sizeof ln,
              "A2MEASURE\ttick=%u\tbody_ms=%.1f\tfire_ms=%.1f\tcheap_ms=%.1f\tp=%.4f\tbody_n=%ld\tfire_n=%ld\tamdahl_2/4/8/inf=%.2f/%.2f/%.2f/%.2f\n",
              now, body, fire, cheap, p, g_a2m_body_calls, g_a2m_fire_calls, a2x,a4x,a8x,ainf);
            log_write(ln);
            /* §8.12 lift-vs-lock gate: the racy scratch leaves as a fraction of the fire subtree. */
            double solver=(double)g_a2m_solver_qpc/per_ms, fog=(double)g_a2m_fog_qpc/per_ms;
            double scratch=solver+fog;
            double scratch_of_fire = fire>0.0 ? scratch/fire : 0.0;   /* how much of PhaseB locking serializes */
            char sn[320];
            snprintf(sn,sizeof sn,
              "A2SCRATCH\ttick=%u\tsolver399450_ms=%.1f\tfog35f470_ms=%.1f\tscratch_ms=%.1f\tfire_ms=%.1f\tscratch/fire=%.4f\tsolver_n=%ld\tfog_n=%ld\n",
              now, solver, fog, scratch, fire, scratch_of_fire, g_a2m_solver_calls, g_a2m_fog_calls);
            log_write(sn);
        }
    }
}

static void install_a2_measure_hooks(void) {
    QueryPerformanceFrequency(&g_a2m_qpf);
    void* tb=NULL; void* tf=NULL;
    if (install_targ_tramp(A2M_A6B80_RVA, a2m_a6b80_prologue, sizeof a2m_a6b80_prologue,
                           (void*)a2m_body_timer, &tb, "FUN_1403a6b80")) {
        g_a2m_body_tramp=(A2mBodyFn)tb;
        log_write("[eaw-mt] A2MEASURE: 3a6b80 per-object body timer installed\n");
    }
    if (install_targ_tramp(FIRE_A76B0_RVA, fire_a76b0_prologue, sizeof fire_a76b0_prologue,
                           (void*)a2m_fire_timer, &tf, "FUN_1403a76b0")) {
        g_a2m_fire_tramp=(A2mFireFn)tf;
        log_write("[eaw-mt] A2MEASURE: 3a76b0 fire-control subtree timer installed\n");
    }
    void* ts=NULL; void* tg=NULL;
    if (install_targ_tramp(A2M_399450_RVA, a2m_399450_prologue, sizeof a2m_399450_prologue,
                           (void*)a2m_solver_timer, &ts, "FUN_140399450")) {
        g_a2m_solver_tramp=(A2mSolverFn)ts;
        log_write("[eaw-mt] A2MEASURE: 399450 lead-solver scratch timer installed (§8.12)\n");
    }
    if (install_targ_tramp(A2M_35F470_RVA, a2m_35f470_prologue, sizeof a2m_35f470_prologue,
                           (void*)a2m_fog_timer, &tg, "FUN_14035f470")) {
        g_a2m_fog_tramp=(A2mFogFn)tg;
        log_write("[eaw-mt] A2MEASURE: 35f470 fog scratch timer installed (§8.12)\n");
    }
}
#endif /* EAW_PROFILE || EAW_ORACLE */

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

        /* Render hook (frame anchor + binary-version sanity check; also builds g_trampoline).
         * Render-thread offload is dormant: flush runs on the main thread (measured ceiling
         * ~0). The render thread is not spawned. */
        if (!install_render_hook()) return FALSE;

        /* ---- Shipped fixes (always installed) ---- */
        install_wait_message_iat_hook();   /* WaitMessage->1ms cap (unfocused-window keep-alive) */
        install_io_iat_hooks();            /* fscache negative-dir cache (createfileA/W) */
        install_game_service_hook();       /* Fix B3: resets the per-gsvc AI budget each pass */
        install_pumpe_hook();              /* Fix B3: per-gsvc AI budget (PUMPE_BUDGET_MS=33) */
        install_prewarm_hook();            /* prewarm OFF by default (EAW_PREWARM=1 to opt in) */
        install_fn56480_wrapper();         /* prewarm allocator-safety wrapper (dormant when off) */
        install_a7190_guard();             /* prewarm null guard (dormant when off) */
        install_b87010_subcallee_hooks();  /* installs the b387400 reset-wrapper Fix B2 depends on */
        install_fix_b();                   /* Fix B1 (opp interval) + Fix B2 (b385190 budget cap) */
        install_b25ec10_subcallee_hooks(); /* b25ec10 art-model result cache (EAW_NO_B25CACHE) */
        build_meg_index();                 /* MEG archive filename index (for meg_has / nf_cache) */
        install_b141f70_subcallee_hooks(); /* nf_cache: MEG not-found memo (EAW_NO_MEGSKIP) */
        if (pfire_on()) install_pfire_hooks(); /* §8.10 step 4: gated 1-shard fire takeover (EAW_PFIRE=1; STAGE A identity) */

#ifdef EAW_PROFILE
        /* ---- Measurement-only instrumentation (compiled out of the release build) ---- */
        install_pump_hook();
        install_inter_hooks();
        install_slot22_hooks();
        install_space_slot22_hook();
        install_space_unit_svc_hook();
        install_tail22i_hook();
        install_post_loop_hooks();
        install_tail22_body_hooks();
        install_2be640_body_hooks();
        install_pumpe_resume_shim();
        install_a6b80_stall2_hooks();
        install_b87010_hook();
        install_d12d520_hook();
        install_d12d480_hook();
        install_b387400_subcallee_hooks();
        install_b3825b0_subcallee_hooks();
        install_b29f810_subcallee_hooks();
        install_b388b60_subcallee_hooks();
        install_b3989a0_subcallee_hooks();
        install_b3a4820_subcallee_hooks();
        install_b375380_subcallee_hooks();
        install_ga4d0_hook();
        install_galactic_slot22_hook();
        log_write("[eaw-mt] EAW_PROFILE build: profiler instrumentation active\n");
#endif

#if defined(EAW_PROFILE) || defined(EAW_ORACLE)
        /* ---- Differential-oracle I/O hooks (inline trampolines; runtime-gated by EAW_DIFFTRACE=1).
         * Available in the full profile build AND in the lean EAW_ORACLE build, which OMITS the
         * measurement-only timing/body-patching hooks above. The body-patchers (install_b*_subcallee_
         * hooks) physically rewrite E8 call sites and can fault on battle-load paths the menu demo
         * never exercises (observed: c0000005 inside b375380); the DT hooks below are whole-function
         * inline trampolines that only snapshot+log, so they are safe to run alone for a clean capture. */
        if (a2measure_on()) {
            install_a2_measure_hooks();   /* a2.0: ONLY the 2 Amdahl timers — DT hooks skipped to avoid confound */
            log_write("[eaw-mt] A2MEASURE mode: 2 timers only (all DT oracle hooks skipped)\n");
        } else {
        install_b5caaf0_hook();   /* DTSTEER: Fighter steering controller I/O (EAW_DIFFTRACE=1) */
        install_b5c95a0_hook();   /* DTYAW: Fighter yaw/roll bank-to-turn integrator I/O (EAW_DIFFTRACE=1) */
        install_b3a76b0_hook();   /* DTFIRE: hardpoint fire-budget distribution I/O (EAW_DIFFTRACE=1) */
        install_b1ffb40_hook();   /* DTRNG: global sim RNG draw I/O (EAW_DIFFTRACE=1) */
        install_b58bd80_hook();   /* DTDMG: DamageTracking decay/expiry I/O (EAW_DIFFTRACE=1) */
        install_b56c030_hook();   /* DTNRG: EnergyPool regen I/O (EAW_DIFFTRACE=1) */
        install_b42f910_hook();   /* DTABIL: AbilityCountdown cooldown/chargeup I/O (EAW_DIFFTRACE=1) */
        install_b4f6070_hook();   /* DTUAI: UnitAI fog/sensor-visibility I/O (EAW_DIFFTRACE=1) */
        install_b437b60_hook();   /* DTNEB: Nebula effect-ramp spring I/O (EAW_DIFFTRACE=1) */
        install_b437310_hook();   /* DTAST: AsteroidFieldDamage roll + proximity I/O (EAW_DIFFTRACE=1) */
        install_b633a30_hook();   /* DTARG: Targeting dispatcher mode-resolution biconditional (EAW_DIFFTRACE=1) */
        install_b14020acd0_hook();/* DTARG2: Targeting team-targeting aim-geometry bit-exact (EAW_DIFFTRACE=1) */
        install_b397640_hook();   /* DTLURE: Lure shared squared-distance bit-exact (EAW_DIFFTRACE=1) */
        install_reveal_5373c0_hook(); /* DTREVEAL: Reveal move-threshold gate bit-exact (EAW_DIFFTRACE=1) */
        install_fint_399e20_hook();   /* DTFINT: projectile-intercept lead solver bit-exact (EAW_DIFFTRACE=1) */
        install_dyn_3ac530_hook();    /* DTDYN: DynamicTransform rebuild bit-exact (EAW_DIFFTRACE=1) */
        install_b63f210_hook();   /* DTTK: Telekinesis interp-timeline biconditional + lerp (EAW_DIFFTRACE=1) */
        install_spl625990_hook(); /* DTSPL2: SimpleSpaceLocomotor vfunc_59 cubic-Hermite mover (EAW_DIFFTRACE=1) */
        install_dtwa_spawn_hook();/* DTWA-SPAWN: CreateObject I1 append-order + manager-resolution oracle (EAW_DIFFTRACE=1) */
        install_dtwa_b3_hook();   /* DTWA-B3: firing-body projectile init §3 source-map oracle (EAW_DIFFTRACE=1) */
        install_dtwa_sig_hook();  /* DTWASIG: signal fan-out Command-schema/traffic oracle (EAW_DIFFTRACE=1) */
        install_a1_sfx_hook();    /* a1: gated SFX takeover — 2d5290 buffer+canonical-drain (EAW_A1=1 to arm) */
        log_write("[eaw-mt] DT oracle hooks installed (EAW_DIFFTRACE=1 to capture)\n");
        }
#endif

        log_write("[eaw-mt] Model A: render on main thread (offload dormant)\n");
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
