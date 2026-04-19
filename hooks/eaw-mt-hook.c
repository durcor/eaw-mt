/*
 * eaw-mt-hook.c — EaW multithreading hook DLL
 *
 * Phase 4 Step 1: scaffold only. Confirms DLL loads cleanly under Wine/Proton.
 * No behavior changes; no patches yet.
 *
 * Build (64-bit, from inside nix develop):
 *   x86_64-w64-mingw32-gcc -shared -o patches/experimental/eaw-mt-hook.dll \
 *     hooks/eaw-mt-hook.c -lkernel32
 *
 * Inject via Steam launch options:
 *   WINEDLLOVERRIDES="eaw-mt-hook=n,b" %command%
 *
 * Verify with Frida:
 *   frida -n StarWarsG.exe -e \
 *     "Process.enumerateModules().forEach(m => { if(m.name.includes('hook')) console.log('Hook loaded:', m.name, m.base); })"
 */

#include <windows.h>

static const char *HOOK_TAG = "[eaw-mt]";

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
    (void)hinstDLL;
    (void)lpvReserved;

    switch (fdwReason) {
        case DLL_PROCESS_ATTACH:
            OutputDebugStringA("[eaw-mt] Hook DLL loaded — Phase 4 scaffold\n");
            DisableThreadLibraryCalls(hinstDLL);
            break;
        case DLL_PROCESS_DETACH:
            OutputDebugStringA("[eaw-mt] Hook DLL unloaded\n");
            break;
        default:
            break;
    }
    return TRUE;
}
