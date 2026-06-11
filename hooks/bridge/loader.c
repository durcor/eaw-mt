// Loads the C++ bridge DLL exactly as the pure-C hook would (LoadLibrary + GetProcAddress)
// and calls into the validated sim:: code. Proves the cross-language boundary works at runtime.
#include <windows.h>
#include <stdio.h>

typedef int (*first_blocked_fn)(const void*);

int main(void) {
    HMODULE h = LoadLibraryA("fc_bridge.dll");
    if (!h) { printf("LOAD FAIL err=%lu\n", GetLastError()); return 1; }
    first_blocked_fn fb = (first_blocked_fn)GetProcAddress(h, "fc_bridge_first_blocked");
    if (!fb) { printf("GETPROC FAIL err=%lu\n", GetLastError()); return 2; }

    unsigned char all_true[12];  for (int i=0;i<12;i++) all_true[i]=1;
    unsigned char all_false[12]; for (int i=0;i<12;i++) all_false[i]=0;
    int r_true  = fb(all_true);   // expect 0 (FireGate::None — all gates passed)
    int r_false = fb(all_false);  // expect 1 (FireGate::OwnerPresent — first gate blocks)
    int pass = (r_true==0 && r_false==1);
    printf("%s LOAD OK; first_blocked(all_true)=%d (expect 0=None)  first_blocked(all_false)=%d (expect 1=OwnerPresent)\n",
           pass ? "PASS" : "FAIL", r_true, r_false);
    return pass ? 0 : 3;
}
