/*
 * winmm_proxy.c — proxy winmm.dll for EaW hook injection
 *
 * Phase 4 Step 2: Model A render thread separation.
 *
 * Installs an inline hook on FUN_140180d90 (render flush wrapper, RVA 0x180d90).
 * The render flush (12-pass D3D9 draw + present) runs on a dedicated render thread
 * instead of the main thread. Main thread signals the render thread and waits for
 * completion (serialized for now — enables true frame overlap later by removing
 * the wait and double-buffering the render task list).
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
 * Build (from inside nix develop):
 *   x86_64-w64-mingw32-gcc -shared -O2 -o patches/experimental/winmm.dll \
 *     hooks/winmm_proxy.c -lkernel32
 *
 * Deploy:
 *   cp patches/experimental/winmm.dll \
 *     ~/gam/steam/steamapps/common/"Star Wars Empire at War"/corruption/
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

/* Trampoline: saved prologue bytes + absolute JMP back to RVA+14 */
typedef void (WINAPI *RenderFlushFn)(void *);
static RenderFlushFn g_trampoline = NULL;

/* =========================================================================
 * Profiling
 *
 * Timing is split across two threads but there is no race: the implementation
 * is serialized — render_done is signaled before the main thread reads any
 * render-thread timing values on the next call to render_flush_hook.
 *
 * Logged every PROFILE_WINDOW frames via OutputDebugString.
 * ========================================================================= */

#define PROFILE_WINDOW 300

static LARGE_INTEGER g_qpc_freq;

/* Set by render thread, read by main thread after render_done (safe). */
static LARGE_INTEGER g_flush_start;
static LARGE_INTEGER g_flush_end;

/* Accumulators — only touched by main thread. */
static LARGE_INTEGER g_frame_start;
static BOOL          g_frame_start_valid = FALSE;
static LONG          g_frame_count       = 0;

static double g_flush_sum_ms = 0, g_flush_min_ms = 1e9, g_flush_max_ms = 0;
static double g_frame_sum_ms = 0, g_frame_min_ms = 1e9, g_frame_max_ms = 0;

static void profile_report_and_reset(void) {
    double freq = (double)g_qpc_freq.QuadPart / 1000.0;
    double n    = (double)PROFILE_WINDOW;

    double flush_avg = g_flush_sum_ms / n;
    double frame_avg = g_frame_sum_ms / n;
    double pct       = (frame_avg > 0.0) ? (flush_avg / frame_avg * 100.0) : 0.0;

    char buf[256];
    sprintf(buf,
        "[eaw-mt] profile(%d frames): "
        "flush avg=%.2f min=%.2f max=%.2f ms | "
        "frame avg=%.2f min=%.2f max=%.2f ms | "
        "flush=%.1f%% of frame\n",
        PROFILE_WINDOW,
        flush_avg, g_flush_min_ms, g_flush_max_ms,
        frame_avg, g_frame_min_ms, g_frame_max_ms,
        pct);
    OutputDebugStringA(buf);

    /* Reset accumulators */
    g_frame_count   = 0;
    g_flush_sum_ms  = g_frame_sum_ms  = 0;
    g_flush_min_ms  = g_frame_min_ms  = 1e9;
    g_flush_max_ms  = g_frame_max_ms  = 0;
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
    /* Guard: log and skip if param_1 looks invalid (NULL or below 64KB).
     * This catches transitions where the graphics driver is torn down or not
     * yet initialized (e.g. land battle load). If this fires, the caller is
     * invoking the render flush with a bad driver pointer — a game-side issue
     * that the original code also had to handle somehow. */
    if ((uintptr_t)param_1 < 0x10000) {
        char buf[128];
        sprintf(buf, "[eaw-mt] render_flush_hook: skipping bad param_1=%p\n", param_1);
        OutputDebugStringA(buf);
        return;
    }

    /* --- record timing from the previous frame (render_done already fired) --- */
    LARGE_INTEGER now;
    QueryPerformanceCounter(&now);
    double freq = (double)g_qpc_freq.QuadPart / 1000.0;

    if (g_frame_start_valid && g_frame_count < PROFILE_WINDOW) {
        double flush_ms = (g_flush_end.QuadPart - g_flush_start.QuadPart) / freq;
        double frame_ms = (now.QuadPart - g_frame_start.QuadPart) / freq;

        g_flush_sum_ms += flush_ms;
        g_frame_sum_ms += frame_ms;
        if (flush_ms < g_flush_min_ms) g_flush_min_ms = flush_ms;
        if (flush_ms > g_flush_max_ms) g_flush_max_ms = flush_ms;
        if (frame_ms < g_frame_min_ms) g_frame_min_ms = frame_ms;
        if (frame_ms > g_frame_max_ms) g_frame_max_ms = frame_ms;

        g_frame_count++;
        if (g_frame_count == PROFILE_WINDOW)
            profile_report_and_reset();
    }

    g_frame_start       = now;
    g_frame_start_valid = TRUE;

    /* --- kick render thread --- */
    g_render_param = param_1;
    SetEvent(g_render_kick);
    WaitForSingleObject(g_render_done, INFINITE);
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

        /* Install inline hook */
        if (!install_render_hook()) return FALSE;

        /* Start render thread */
        HANDLE thread = CreateThread(NULL, 0, render_thread_proc, NULL, 0, NULL);
        if (!thread) {
            OutputDebugStringA("[eaw-mt] FATAL: CreateThread failed\n");
            return FALSE;
        }
        CloseHandle(thread);

        OutputDebugStringA("[eaw-mt] Model A render thread active\n");
    }

    if (fdwReason == DLL_PROCESS_DETACH) {
        InterlockedExchange(&g_shutdown, 1);
        if (g_render_kick) SetEvent(g_render_kick);
        if (real_winmm) FreeLibrary(real_winmm);
    }

    return TRUE;
}
