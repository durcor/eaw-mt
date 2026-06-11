// Feasibility spike: thin extern "C" bridge exporting the validated sim::fire_control_decide
// into a Windows PE DLL loadable by the (pure-C) hook. Proves §8.51's "C++ can't link" was an
// assumption, not a constraint.
#include "fire_control.h"
#include "sim_rng.h"
#include <windows.h>

extern "C" __declspec(dllexport)
int fc_bridge_decide(const void* inputs_blob, unsigned int rng_seed, int* out_gate)
{
    // The hook fills a sim::FireControlInputs and passes &it as inputs_blob. Here we just
    // exercise the linked code so the linker pulls the whole sim/ fire_control closure.
    const sim::FireControlInputs& in = *reinterpret_cast<const sim::FireControlInputs*>(inputs_blob);
    eaw::SimRng rng(rng_seed);
    sim::FireControlDecision d = sim::fire_control_decide(in, rng, /*sink=*/nullptr);
    if (out_gate) *out_gate = static_cast<int>(d.blocked_gate);
    return static_cast<int>(d.outcome);
}

// A trivial second export to confirm a pure leaf links too.
extern "C" __declspec(dllexport)
int fc_bridge_first_blocked(const void* elig_blob)
{
    const sim::FireEligibility& g = *reinterpret_cast<const sim::FireEligibility*>(elig_blob);
    return static_cast<int>(sim::fire_first_blocked_gate(g));
}

BOOL WINAPI DllMain(HINSTANCE, DWORD, LPVOID) { return TRUE; }
