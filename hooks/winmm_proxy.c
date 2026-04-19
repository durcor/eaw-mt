/*
 * winmm_proxy.c — proxy winmm.dll for EaW hook injection
 *
 * Forwards the 4 WINMM functions StarWarsG.exe imports to the real winmm.dll,
 * and runs hook code in DllMain. Place as winmm.dll in the game's corruption/
 * directory alongside StarWarsG.exe.
 *
 * Build (from inside nix develop):
 *   x86_64-w64-mingw32-gcc -shared -o patches/experimental/winmm.dll \
 *     hooks/winmm_proxy.c -lkernel32
 *
 * Inject via Steam launch options (no AppInit_DLLs needed):
 *   WINEDLLOVERRIDES="winmm=n,b" PROTON_USE_NTSYNC=1 %command% STEAMMOD=...
 *
 * Then copy patches/experimental/winmm.dll to the corruption/ game directory.
 */

#include <windows.h>

static HMODULE real_winmm = NULL;

/* -- Forwarded functions -------------------------------------------------- */

typedef MMRESULT (WINAPI *timeEndPeriod_t)(UINT uPeriod);
typedef MMRESULT (WINAPI *timeBeginPeriod_t)(UINT uPeriod);
typedef MMRESULT (WINAPI *timeGetDevCaps_t)(LPTIMECAPS ptc, UINT cbtc);
typedef DWORD   (WINAPI *timeGetTime_t)(void);

static timeEndPeriod_t   real_timeEndPeriod   = NULL;
static timeBeginPeriod_t real_timeBeginPeriod  = NULL;
static timeGetDevCaps_t  real_timeGetDevCaps   = NULL;
static timeGetTime_t     real_timeGetTime      = NULL;

MMRESULT WINAPI timeEndPeriod(UINT uPeriod) {
    return real_timeEndPeriod(uPeriod);
}

MMRESULT WINAPI timeBeginPeriod(UINT uPeriod) {
    return real_timeBeginPeriod(uPeriod);
}

MMRESULT WINAPI timeGetDevCaps(LPTIMECAPS ptc, UINT cbtc) {
    return real_timeGetDevCaps(ptc, cbtc);
}

DWORD WINAPI timeGetTime(void) {
    return real_timeGetTime();
}

/* -- DllMain -------------------------------------------------------------- */

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
    (void)lpvReserved;

    if (fdwReason == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(hinstDLL);

        real_winmm = LoadLibraryA("C:\\windows\\system32\\winmm.dll");
        if (!real_winmm) {
            OutputDebugStringA("[eaw-mt] FATAL: failed to load real winmm.dll\n");
            return FALSE;
        }

        real_timeEndPeriod  = (timeEndPeriod_t)  GetProcAddress(real_winmm, "timeEndPeriod");
        real_timeBeginPeriod = (timeBeginPeriod_t) GetProcAddress(real_winmm, "timeBeginPeriod");
        real_timeGetDevCaps = (timeGetDevCaps_t) GetProcAddress(real_winmm, "timeGetDevCaps");
        real_timeGetTime    = (timeGetTime_t)    GetProcAddress(real_winmm, "timeGetTime");

        if (!real_timeEndPeriod || !real_timeBeginPeriod ||
            !real_timeGetDevCaps || !real_timeGetTime) {
            OutputDebugStringA("[eaw-mt] FATAL: failed to resolve winmm exports\n");
            return FALSE;
        }

        OutputDebugStringA("[eaw-mt] winmm proxy loaded — hook active\n");
    }

    if (fdwReason == DLL_PROCESS_DETACH && real_winmm) {
        FreeLibrary(real_winmm);
    }

    return TRUE;
}
