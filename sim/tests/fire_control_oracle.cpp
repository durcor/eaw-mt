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

    long records = 0, fired = 0, bug = 0;
    long blocked_hist[13] = {0};   // index by (int)FireGate
    char line[512];
    while (std::fgets(line, sizeof line, f)) {
        const char* p = std::strstr(line, "DTFCREC");
        if (!p) continue;
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
            if (g != FireGate::None) {                  // sim blocked a shot the binary TOOK → bug
                ++bug;
                if (bug <= 16)
                    std::printf("  BUG  bits=%03x fired=1 but sim blocks at %s\n", bits, gate_name(g));
            }
        }
    }
    std::fclose(f);

    std::printf("=== fire_control_oracle ===\n");
    std::printf("records=%ld  fired=%ld  bug=%ld\n", records, fired, bug);
    std::printf("block distribution (first-blocking gate over all records):\n");
    for (int i = 0; i <= 12; ++i)
        if (blocked_hist[i]) std::printf("  %-16s %ld\n", gate_name((FireGate)i), blocked_hist[i]);

    if (records == 0) { std::printf("NO RECORDS — capture produced no DTFCREC lines\n"); return 1; }
    if (bug != 0)     { std::printf("%ld FAIL — sim gate blocked a real shot\n", bug); return 1; }
    std::printf("ALL PASS — no sim gate blocked any of the %ld real shots\n", fired);
    return 0;
}
