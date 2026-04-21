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

    /* Restore previous vtable entry on mode transition */
    if (g_patched_vtable && g_saved_sim_tick) {
        DWORD old;
        VirtualProtect(&g_patched_vtable[SIM_TICK_VTBL_IDX], 8,
                       PAGE_EXECUTE_READWRITE, &old);
        g_patched_vtable[SIM_TICK_VTBL_IDX] = (void *)g_saved_sim_tick;
        VirtualProtect(&g_patched_vtable[SIM_TICK_VTBL_IDX], 8, old, &old);
        g_patched_vtable = NULL;
        g_saved_sim_tick = NULL;
    }

    DWORD old;
    if (!VirtualProtect(&vtable[SIM_TICK_VTBL_IDX], 8,
                        PAGE_EXECUTE_READWRITE, &old)) {
        OutputDebugStringA("[eaw-mt] WARN: VirtualProtect for sim tick vtable failed\n");
        return;
    }
    g_saved_sim_tick = (SimTickFn)vtable[SIM_TICK_VTBL_IDX];
    vtable[SIM_TICK_VTBL_IDX] = (void *)sim_tick_hook;
    VirtualProtect(&vtable[SIM_TICK_VTBL_IDX], 8, old, &old);
    g_patched_vtable = vtable;
    OutputDebugStringA("[eaw-mt] Sim tick hook patched\n");
}

/* Forward declarations — definitions appear in later sections. */
static void write_abs_jmp(BYTE *dst, uint64_t target);
static LARGE_INTEGER g_qpc_freq;

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
        OutputDebugStringA("[eaw-mt] WARN: Pump_Threads CALL not found in ScoringManager::Service\n");
        return FALSE;
    }

    /* Allocate a near stub: FF 25 00 00 00 00 <abs addr of pump_threads_hook> */
    BYTE *stub = alloc_near(call_site, 14);
    if (!stub) {
        OutputDebugStringA("[eaw-mt] WARN: alloc_near failed for Pump_Threads stub\n");
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

    OutputDebugStringA("[eaw-mt] Pump_Threads call-site hook installed\n");
    return TRUE;
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
#define N_IFC         8

/* Per-call accumulators — main thread only. */
static struct { LONG count; double sum_ms, min_ms, max_ms; } g_ifc[N_IFC] = {
    {0,0,1e9,0},{0,0,1e9,0},{0,0,1e9,0},{0,0,1e9,0},
    {0,0,1e9,0},{0,0,1e9,0},{0,0,1e9,0},{0,0,1e9,0}
};
static BYTE *g_ifc_site[N_IFC]; /* NULL if not installed */

static const char * const g_ifc_name[N_IFC] = {
    "24bb80","2505c0","2089e0","cmd_q",
    "2c2910","2c0c70","4908c0","caa60"
};
static const uint64_t g_ifc_rva[N_IFC] = {
    0x24bb80, 0x2505c0, 0x2089e0, 0x3b08c0,
    0x2c2910, 0x2c0c70, 0x4908c0, 0xcaa60
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

typedef void (*VV_t)(void);
typedef void (*VP_t)(void *);
typedef void (*VPU_t)(void *, uint32_t);

static VV_t  g_ifc0=NULL; static VV_t  g_ifc1=NULL;
static VV_t  g_ifc2=NULL; static VP_t  g_ifc3=NULL;
static VPU_t g_ifc4=NULL; static VPU_t g_ifc5=NULL;
static VP_t  g_ifc6=NULL; static VV_t  g_ifc7=NULL;

static void ifc_hook0(void)                { IFC_WRAP(g_ifc0(),    0); }
static void ifc_hook1(void)                { IFC_WRAP(g_ifc1(),    1); }
static void ifc_hook2(void)                { IFC_WRAP(g_ifc2(),    2); }
static void ifc_hook3(void *a)             { IFC_WRAP(g_ifc3(a),   3); }
static void ifc_hook4(void *a, uint32_t b) { IFC_WRAP(g_ifc4(a,b), 4); }
static void ifc_hook5(void *a, uint32_t b) { IFC_WRAP(g_ifc5(a,b), 5); }
static void ifc_hook6(void *a)             { IFC_WRAP(g_ifc6(a),   6); }
static void ifc_hook7(void)                { IFC_WRAP(g_ifc7(),    7); }

static BYTE *g_inter_stub_block = NULL;

static BOOL install_inter_hooks(void)
{
    HMODULE exe = GetModuleHandleA(NULL);
    if (!exe) return FALSE;

    BYTE *winmain = (BYTE *)exe + WINMAIN_RVA;

    g_ifc0 = (VV_t) ((BYTE*)exe + g_ifc_rva[0]);
    g_ifc1 = (VV_t) ((BYTE*)exe + g_ifc_rva[1]);
    g_ifc2 = (VV_t) ((BYTE*)exe + g_ifc_rva[2]);
    g_ifc3 = (VP_t) ((BYTE*)exe + g_ifc_rva[3]);
    g_ifc4 = (VPU_t)((BYTE*)exe + g_ifc_rva[4]);
    g_ifc5 = (VPU_t)((BYTE*)exe + g_ifc_rva[5]);
    g_ifc6 = (VP_t) ((BYTE*)exe + g_ifc_rva[6]);
    g_ifc7 = (VV_t) ((BYTE*)exe + g_ifc_rva[7]);

    g_inter_stub_block = alloc_near(winmain, N_IFC * 14);
    if (!g_inter_stub_block) {
        OutputDebugStringA("[eaw-mt] WARN: alloc_near failed for inter stubs\n");
        return FALSE;
    }

    void *hook_fns[N_IFC] = {
        ifc_hook0, ifc_hook1, ifc_hook2, ifc_hook3,
        ifc_hook4, ifc_hook5, ifc_hook6, ifc_hook7
    };

    int n = 0;
    for (int i = 0; i < N_IFC; i++) {
        BYTE *target = (BYTE*)exe + g_ifc_rva[i];
        BYTE *site   = NULL;

        for (int j = 0; j <= WINMAIN_SIZE - 5; j++) {
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
            OutputDebugStringA(m);
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

    char m[64];
    sprintf(m, "[eaw-mt] inter-flush hooks: %d/%d installed\n", n, N_IFC);
    OutputDebugStringA(m);
    return n > 0;
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

    char buf[640];
    sprintf(buf,
        "[eaw-mt] profile(%ld frames):\n"
        "  flush    avg=%.2f min=%.2f max=%.2f ms (per flush, n=%ld)\n"
        "  frame    avg=%.2f min=%.2f max=%.2f ms\n"
        "  inter    avg=%.2f min=%.2f max=%.2f ms (flush#1->flush#2)\n"
        "  sim_tick avg=%.2f min=%.2f max=%.2f ms\n"
        "  pump     avg=%.2f min=%.2f max=%.2f ms (Pump_Threads, n=%ld)\n",
        (long)g_frame_count,
        g_flush_sum_ms / fln, g_flush_min_ms, g_flush_max_ms, (long)g_flush_count,
        g_frame_sum_ms / fn,  g_frame_min_ms, g_frame_max_ms,
        g_inter_sum_ms / fn,  g_inter_min_ms, g_inter_max_ms,
        g_sim_sum_ms   / fn,  g_sim_min_ms,   g_sim_max_ms,
        g_pump_sum_ms  / pn,  g_pump_min_ms,  g_pump_max_ms, (long)g_pump_count);
    OutputDebugStringA(buf);

    /* Inter-flush breakdown — second ODS call to avoid buffer overflow. */
    {
        char ibuf[640];
        int off = sprintf(ibuf, "[eaw-mt] inter breakdown:");
        for (int i = 0; i < N_IFC; i++) {
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
        OutputDebugStringA(ibuf);
    }

    g_frame_count = 0;
    g_flush_count = 0;
    g_pump_count  = 0;
    g_flush_sum_ms = g_frame_sum_ms = g_inter_sum_ms = g_sim_sum_ms = g_pump_sum_ms = 0;
    g_flush_min_ms = g_frame_min_ms = g_inter_min_ms = g_sim_min_ms = g_pump_min_ms = 1e9;
    g_flush_max_ms = g_frame_max_ms = g_inter_max_ms = g_sim_max_ms = g_pump_max_ms = 0;
    for (int i = 0; i < N_IFC; i++) {
        g_ifc[i].count  = 0;
        g_ifc[i].sum_ms = 0;
        g_ifc[i].min_ms = 1e9;
        g_ifc[i].max_ms = 0;
    }
}

/* =========================================================================
 * Render thread
 * ========================================================================= */

static DWORD WINAPI render_thread_proc(LPVOID unused) {
    (void)unused;
    OutputDebugStringA("[eaw-mt] Render thread started\n");

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

    OutputDebugStringA("[eaw-mt] Render thread exiting\n");
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
        OutputDebugStringA("[eaw-mt] FATAL: FUN_140180d90 bytes don't match — wrong binary version?\n");
        return FALSE;
    }

    /* Allocate executable trampoline memory */
    BYTE *tramp = (BYTE *)VirtualAlloc(NULL, 64,
        MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    if (!tramp) {
        OutputDebugStringA("[eaw-mt] FATAL: VirtualAlloc for trampoline failed\n");
        return FALSE;
    }

    /* Build trampoline: saved prologue + JMP to flush_fn+14 */
    memcpy(tramp, saved_prologue, 14);
    write_abs_jmp(tramp + 14, (uint64_t)(flush_fn + 14));

    g_trampoline = (RenderFlushFn)tramp;

    /* Patch flush_fn: write JMP to render_flush_hook */
    DWORD old_prot;
    if (!VirtualProtect(flush_fn, 14, PAGE_EXECUTE_READWRITE, &old_prot)) {
        OutputDebugStringA("[eaw-mt] FATAL: VirtualProtect failed\n");
        VirtualFree(tramp, 0, MEM_RELEASE);
        return FALSE;
    }

    write_abs_jmp(flush_fn, (uint64_t)render_flush_hook);

    VirtualProtect(flush_fn, 14, old_prot, &old_prot);

    /* Flush instruction cache so the CPU sees the patched bytes */
    FlushInstructionCache(GetCurrentProcess(), flush_fn, 14);

    OutputDebugStringA("[eaw-mt] Render hook installed at FUN_140180d90\n");
    return TRUE;
}

/* =========================================================================
 * DllMain
 * ========================================================================= */

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
    (void)lpvReserved;

    if (fdwReason == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(hinstDLL);

        /* Load real winmm */
        real_winmm = LoadLibraryA("C:\\windows\\system32\\winmm.dll");
        if (!real_winmm) {
            OutputDebugStringA("[eaw-mt] FATAL: failed to load real winmm.dll\n");
            return FALSE;
        }
        real_timeEndPeriod   = (timeEndPeriod_t)   GetProcAddress(real_winmm, "timeEndPeriod");
        real_timeBeginPeriod = (timeBeginPeriod_t)  GetProcAddress(real_winmm, "timeBeginPeriod");
        real_timeGetDevCaps  = (timeGetDevCaps_t)  GetProcAddress(real_winmm, "timeGetDevCaps");
        real_timeGetTime     = (timeGetTime_t)      GetProcAddress(real_winmm, "timeGetTime");
        if (!real_timeEndPeriod || !real_timeBeginPeriod ||
            !real_timeGetDevCaps || !real_timeGetTime) {
            OutputDebugStringA("[eaw-mt] FATAL: failed to resolve winmm exports\n");
            return FALSE;
        }

        /* Initialize QPC frequency for profiling */
        QueryPerformanceFrequency(&g_qpc_freq);

        /* Create sync events (auto-reset) */
        g_render_kick = CreateEventA(NULL, FALSE, FALSE, NULL);
        g_render_done = CreateEventA(NULL, FALSE, FALSE, NULL);
        if (!g_render_kick || !g_render_done) {
            OutputDebugStringA("[eaw-mt] FATAL: CreateEvent failed\n");
            return FALSE;
        }

        /* Install inline hook (also builds g_trampoline) */
        if (!install_render_hook()) return FALSE;

        /* Install Pump_Threads call-site hook (non-fatal if scan fails) */
        install_pump_hook();

        /* Install inter-flush decomposition hooks (non-fatal) */
        install_inter_hooks();

        /* Start render thread */
        HANDLE thread = CreateThread(NULL, 0, render_thread_proc, NULL, 0, NULL);
        if (!thread) {
            OutputDebugStringA("[eaw-mt] FATAL: CreateThread failed\n");
            return FALSE;
        }
        CloseHandle(thread);

        OutputDebugStringA("[eaw-mt] Model A render thread active (serialized)\n");
    }

    if (fdwReason == DLL_PROCESS_DETACH) {
        InterlockedExchange(&g_shutdown, 1);
        if (g_render_kick) SetEvent(g_render_kick);
        /* Restore vtable entry so the game doesn't call our hook after unload */
        if (g_patched_vtable && g_saved_sim_tick) {
            DWORD old;
            VirtualProtect(&g_patched_vtable[SIM_TICK_VTBL_IDX], 8,
                           PAGE_EXECUTE_READWRITE, &old);
            g_patched_vtable[SIM_TICK_VTBL_IDX] = (void *)g_saved_sim_tick;
            VirtualProtect(&g_patched_vtable[SIM_TICK_VTBL_IDX], 8, old, &old);
        }
        if (real_winmm) FreeLibrary(real_winmm);
    }

    return TRUE;
}
