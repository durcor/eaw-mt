// Offline replay side of the in-game STRUCTURAL ORACLE for the fire_control gate ladder
// (firing_body_lift_scope.md §8.51). The DTFC capture in hooks/winmm_proxy.c evaluates the 12 sim::FireGate
// predicates on each live 3825b0 fire call and emits `DTFCREC bits=<hex> fired=<0|1>` records. This tool
// replays each record through the REAL sim::fire_first_blocked_gate and checks the one-directional
// invariant: a shot the BINARY actually took (fired==1) must NOT be blocked by any sim gate (else sim has
// an over-strict / wrong gate). It also reports the per-gate block distribution observed in-game.
//
// Usage:  fire_control_oracle <capture-file>     (the file = lines grepped for DTFCREC, or the raw log)
//
// PASS iff every fired==1 record replays to FireGate::None AND at least one record was seen.
#include "../fire_control.h"
#include <cstdio>
#include <cstdint>
#include <cstring>

using namespace sim;

// Unpack the 12-bit mask (bit i set = gate i PASSES, sim::FireGate order) into a FireEligibility.
static FireEligibility unpack(uint16_t m) {
    FireEligibility g;
    g.owner_present     = (m >> 0)  & 1;
    g.target_present    = (m >> 1)  & 1;
    g.context_match     = (m >> 2)  & 1;
    g.target_targetable = (m >> 3)  & 1;
    g.target_queryable  = (m >> 4)  & 1;
    g.capability_match  = (m >> 5)  & 1;
    g.order_charge_ok   = (m >> 6)  & 1;
    g.not_self_target   = (m >> 7)  & 1;
    g.not_fogged        = (m >> 8)  & 1;
    g.diplomacy_ok      = (m >> 9)  & 1;
    g.firing_arc_ok     = (m >> 10) & 1;
    g.weapon_selected   = (m >> 11) & 1;
    return g;
}

static const char* gate_name(FireGate g) {
    switch (g) {
        case FireGate::None:             return "None";
        case FireGate::OwnerPresent:     return "OwnerPresent";
        case FireGate::TargetPresent:    return "TargetPresent";
        case FireGate::ContextMatch:     return "ContextMatch";
        case FireGate::TargetTargetable: return "TargetTargetable";
        case FireGate::TargetQueryable:  return "TargetQueryable";
        case FireGate::CapabilityMatch:  return "CapabilityMatch";
        case FireGate::OrderCharge:      return "OrderCharge";
        case FireGate::NotSelfTarget:    return "NotSelfTarget";
        case FireGate::NotFogged:        return "NotFogged";
        case FireGate::Diplomacy:        return "Diplomacy";
        case FireGate::FiringArc:        return "FiringArc";
        case FireGate::WeaponSelected:   return "WeaponSelected";
    }
    return "?";
}

int main(int argc, char** argv) {
    if (argc < 2) { std::fprintf(stderr, "usage: %s <capture-file>\n", argv[0]); return 2; }
    std::FILE* f = std::fopen(argv[1], "r");
    if (!f) { std::fprintf(stderr, "cannot open %s\n", argv[1]); return 2; }

    // ── gate-ladder records (DTFCREC) ──
    long records = 0, fired = 0, bug = 0;
    long blocked_hist[13] = {0};   // index by (int)FireGate
    // ── geometry / range-gate records (DTFCGREC) ──
    long g_records = 0, g_fired = 0;
    long g_transcribe_mismatch = 0;   // sim::fire_range_gate_pass != binary's captured in_range
    long g_ground_bug = 0;            // binary fired but the range gate (sim) says out of range

    char line[512];
    while (std::fgets(line, sizeof line, f)) {
        if (const char* p = std::strstr(line, "DTFCREC")) {
            unsigned bits = 0; int fr = 0;
            const char* b = std::strstr(p, "bits=");
            const char* d = std::strstr(p, "fired=");
            if (!b || !d) continue;
            if (std::sscanf(b + 5, "%x", &bits) != 1) continue;
            if (std::sscanf(d + 6, "%d", &fr) != 1) continue;
            ++records;
            FireGate g = fire_first_blocked_gate(unpack((uint16_t)bits));
            blocked_hist[(int)g]++;
            if (fr == 1) {
                ++fired;
                if (g != FireGate::None) {              // sim blocked a shot the binary TOOK → bug
                    ++bug;
                    if (bug <= 16)
                        std::printf("  GATE BUG  bits=%03x fired=1 but sim blocks at %s\n", bits, gate_name(g));
                }
            }
        } else if (const char* q = std::strstr(line, "DTFCGREC")) {
            // NB: keys are tab-anchored — "\tin=" must not match the "in=" inside "\tmin=".
            float mx,my,ax,ay,wr,ex,mn; int in=0, fr=0;
            const char* pm = std::strstr(q,"\tmx="); const char* pM = std::strstr(q,"\tmy=");
            const char* pa = std::strstr(q,"\tax="); const char* pA = std::strstr(q,"\tay=");
            const char* pw = std::strstr(q,"\twr="); const char* pe = std::strstr(q,"\tex=");
            const char* pn = std::strstr(q,"\tmin="); const char* pi = std::strstr(q,"\tin=");
            const char* pf = std::strstr(q,"\tfired=");
            if (!(pm&&pM&&pa&&pA&&pw&&pe&&pn&&pi&&pf)) continue;
            if (std::sscanf(pm+4,"%g",&mx)!=1 || std::sscanf(pM+4,"%g",&my)!=1 ||
                std::sscanf(pa+4,"%g",&ax)!=1 || std::sscanf(pA+4,"%g",&ay)!=1 ||
                std::sscanf(pw+4,"%g",&wr)!=1 || std::sscanf(pe+4,"%g",&ex)!=1 ||
                std::sscanf(pn+5,"%g",&mn)!=1 || std::sscanf(pi+4,"%d",&in)!=1 ||
                std::sscanf(pf+7,"%d",&fr)!=1) continue;
            ++g_records;
            // muzzle/aim are 2D (x,y); z is unused by the range gate.
            bool sim_in = fire_range_gate_pass(eaw::vec3{mx,my,0}, eaw::vec3{ax,ay,0}, wr, ex, mn);
            if (sim_in != (in != 0)) {                 // FP-transcription mismatch vs the binary's verdict
                ++g_transcribe_mismatch;
                if (g_transcribe_mismatch <= 16)
                    std::printf("  RANGE MISMATCH sim=%d bin=%d  muzzle=(%.3g,%.3g) aim=(%.3g,%.3g) wr=%.3g ex=%.3g min=%.3g\n",
                                sim_in, in, mx, my, ax, ay, wr, ex, mn);
            }
            if (fr == 1) { ++g_fired; if (!sim_in) ++g_ground_bug; }   // fired but sim says out of range
        }
    }
    std::fclose(f);

    std::printf("=== fire_control_oracle ===\n");
    std::printf("[gate ladder]  records=%ld  fired=%ld  bug=%ld\n", records, fired, bug);
    std::printf("  block distribution (first-blocking gate):\n");
    for (int i = 0; i <= 12; ++i)
        if (blocked_hist[i]) std::printf("    %-16s %ld\n", gate_name((FireGate)i), blocked_hist[i]);
    std::printf("[range gate]   records=%ld  fired=%ld  transcribe_mismatch=%ld  ground_bug=%ld\n",
                g_records, g_fired, g_transcribe_mismatch, g_ground_bug);

    int fail = 0;
    if (records == 0 && g_records == 0) { std::printf("NO RECORDS — capture produced no DTFC*REC lines\n"); return 1; }
    if (bug != 0)                  { std::printf("GATE FAIL — sim gate blocked %ld real shots\n", bug); fail = 1; }
    if (g_transcribe_mismatch != 0){ std::printf("RANGE FAIL — sim range gate disagrees with the binary on %ld records\n", g_transcribe_mismatch); fail = 1; }
    if (g_ground_bug != 0)         { std::printf("RANGE FAIL — sim range gate rejects %ld real shots\n", g_ground_bug); fail = 1; }
    if (fail) return 1;
    std::printf("ALL PASS — gate ladder clean on %ld shots; range gate matches the binary on all %ld records (%ld real shots)\n",
                fired, g_records, g_fired);
    return 0;
}
