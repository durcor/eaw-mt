// Companion DLL bridging the validated sim:: fire-control lift into the game: a thin extern "C"
// surface the (pure-C) winmm hook LoadLibrary's, so the in-game fire path runs the SAME code the
// §8.51-8.57 oracle validated instead of a parallel C transcription. §8.58 proved this links into a
// self-contained PE DLL (KERNEL32/msvcrt/ntdll only). B3.8 migrates the hook onto it, observe-first.
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

// §8.58 B3.8.1 — the in-process equivalence entry. Takes the 12-bit gate mask the hook's
// dtfc_gate_bits already produces (bit i set = gate i passes, sim::FireGate order — the SAME
// encoding the offline §8.51 oracle replays) and returns the first-blocked sim::FireGate as an int.
// Scalar in / scalar out: NO struct layout crosses the C->C++ boundary, so the marshalling is
// immune to ABI/padding drift between the pure-C hook and the C++ bridge.
extern "C" __declspec(dllexport)
int fc_bridge_gate_from_bits(unsigned int bits)
{
    sim::FireEligibility g;
    g.owner_present     = (bits >> 0)  & 1;
    g.target_present    = (bits >> 1)  & 1;
    g.context_match     = (bits >> 2)  & 1;
    g.target_targetable = (bits >> 3)  & 1;
    g.target_queryable  = (bits >> 4)  & 1;
    g.capability_match  = (bits >> 5)  & 1;
    g.order_charge_ok   = (bits >> 6)  & 1;
    g.not_self_target   = (bits >> 7)  & 1;
    g.not_fogged        = (bits >> 8)  & 1;
    g.diplomacy_ok      = (bits >> 9)  & 1;
    g.firing_arc_ok     = (bits >> 10) & 1;
    g.weapon_selected   = (bits >> 11) & 1;
    return static_cast<int>(sim::fire_first_blocked_gate(g));
}

// §8.59 B3.8.2 — stage-F range gate (§8.52). Scalar boundary: the hook passes the same DTFCG geometry
// it already captures (muzzle x/y, aim x/y, weapon_range, target_extent, min_range — the gate is 2D, z
// unused) and gets the in-range verdict back. Calls the SAME sim::fire_range_gate_pass that
// fire_control_decide uses, so the in-process verdict is bit-identical to the offline §8.52 replay.
extern "C" __declspec(dllexport)
int fc_bridge_range_gate(float mx, float my, float ax, float ay,
                         float weapon_range, float target_extent, float min_range)
{
    eaw::vec3 muzzle; muzzle.x = mx; muzzle.y = my;
    eaw::vec3 aim;    aim.x = ax;    aim.y = ay;
    return sim::fire_range_gate_pass(muzzle, aim, weapon_range, target_extent, min_range) ? 1 : 0;
}

BOOL WINAPI DllMain(HINSTANCE, DWORD, LPVOID) { return TRUE; }
