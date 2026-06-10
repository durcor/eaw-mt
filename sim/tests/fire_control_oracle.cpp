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
#include <cmath>

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
    // ── lead-solve records (DTFCLREC) ──
    long l_records = 0, l_verdict_mismatch = 0, l_exact = 0, l_near = 0, l_far = 0;
    double l_max_err = 0.0;
    // ── end-to-end composed outcome (reach + fire_control_decide) ──
    long e_reach1 = 0, e_reach0 = 0, e_fire = 0, e_compose_fail = 0;
    // ── stage-J cooldown records (DTFCJREC, §8.55) ──
    long j_records = 0, j_roll = 0, j_btwn = 0;
    long j_recharge_bug = 0, j_burst_bug = 0, j_drawmiss = 0;

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
        } else if (const char* L = std::strstr(line, "DTFCLREC")) {
            float tp[3],tv[3],fv[3],mz[3],sr[3],gs,ps,eo[3];
            auto v3 = [&](const char* key, float* o) {
                const char* k = std::strstr(L, key);
                return k && std::sscanf(k + (int)std::strlen(key), "%g,%g,%g", o, o+1, o+2) == 3;
            };
            const char* kg = std::strstr(L,"\tgs="); const char* kp = std::strstr(L,"\tps=");
            if (!(v3("\ttp=",tp)&&v3("\ttv=",tv)&&v3("\tfv=",fv)&&v3("\tmz=",mz)&&v3("\tsr=",sr)&&
                  kg && kp && v3("\teo=",eo))) continue;
            if (std::sscanf(kg+4,"%g",&gs)!=1 || std::sscanf(kp+4,"%g",&ps)!=1) continue;
            int reach = -1; const char* kr = std::strstr(L,"\treach=");
            if (kr) std::sscanf(kr+7,"%d",&reach);
            ++l_records;
            bool sol = false;
            eaw::vec3 lead = firing_intercept_lead(eaw::vec3{tp[0],tp[1],tp[2]}, eaw::vec3{tv[0],tv[1],tv[2]},
                                                   eaw::vec3{fv[0],fv[1],fv[2]}, eaw::vec3{mz[0],mz[1],mz[2]},
                                                   eaw::vec3{sr[0],sr[1],sr[2]}, gs, ps, &sol);
            // the fire-relevant bit: does sim agree with the binary on whether a solution exists?
            bool sim_zero = (lead.x==0.0f && lead.y==0.0f && lead.z==0.0f);
            bool bin_zero = (eo[0]==0.0f && eo[1]==0.0f && eo[2]==0.0f);
            if (sim_zero != bin_zero) {
                ++l_verdict_mismatch;
                if (l_verdict_mismatch <= 16)
                    std::printf("  LEAD VERDICT sim_zero=%d bin_zero=%d  sim=(%.4g,%.4g,%.4g) bin=(%.4g,%.4g,%.4g)\n",
                                sim_zero, bin_zero, lead.x,lead.y,lead.z, eo[0],eo[1],eo[2]);
            }
            // value accuracy (bit-exact vs near vs far), to separate transcription error from FP noise.
            auto bits = [](float a){ uint32_t u; std::memcpy(&u,&a,4); return u; };
            bool exact = bits(lead.x)==bits(eo[0]) && bits(lead.y)==bits(eo[1]) && bits(lead.z)==bits(eo[2]);
            double ex=lead.x-eo[0], ey=lead.y-eo[1], ez=lead.z-eo[2];
            double err=std::sqrt(ex*ex+ey*ey+ez*ez);
            double mag=std::sqrt((double)eo[0]*eo[0]+(double)eo[1]*eo[1]+(double)eo[2]*eo[2]);
            double rel = mag>1e-6 ? err/mag : err;
            if (exact) ++l_exact; else if (rel < 1e-4) ++l_near; else { ++l_far; if (rel>l_max_err) l_max_err=rel; }

            // ── end-to-end: run the FULL fire_control_decide on this real fired shot. Gates + range pass
            // trivially (validated separately, §8.51/§8.52); shooter_ref = sr (mat1, no second bone, no
            // RNG); the lead is recomputed (bit-exact per above) and reach is the captured 383ba0. The
            // outcome MUST be Fire iff (lead non-zero AND reach) — the binary's composed fire decision.
            if (reach >= 0) {
                if (reach) ++e_reach1; else ++e_reach0;
                FireControlInputs fin;
                fin.has_explicit_context = true;  fin.context_aim = eaw::vec3{mz[0],mz[1],mz[2]};
                fin.muzzle_valid = true;          fin.weapon_range = 1e30f;   // range passes
                fin.target_pos = eaw::vec3{tp[0],tp[1],tp[2]};
                fin.target_vel = eaw::vec3{tv[0],tv[1],tv[2]};
                fin.frame_vel  = eaw::vec3{fv[0],fv[1],fv[2]};
                fin.muzzle_mat1_t = eaw::vec3{sr[0],sr[1],sr[2]};   // → shooter_ref = sr (no RNG)
                fin.gamespeed = gs; fin.proj_speed = ps;
                fin.aim_reachable = (reach != 0);
                fin.no_spread = true;
                static int dummy_owner; fin.spawn.requester = &dummy_owner; fin.spawn.template_id = 1;
                eaw::SimRng rr(0);
                FireControlDecision dd = fire_control_decide(fin, rr);
                bool sim_lead_nz = !(lead.x==0.0f && lead.y==0.0f && lead.z==0.0f);
                bool expect_fire = sim_lead_nz && (reach != 0);
                bool got_fire = (dd.outcome == FireOutcome::Fire);
                if (got_fire) ++e_fire;
                if (got_fire != expect_fire) {
                    ++e_compose_fail;
                    if (e_compose_fail <= 16)
                        std::printf("  COMPOSE FAIL reach=%d lead_nz=%d expect_fire=%d got_fire=%d outcome=%d\n",
                                    reach, sim_lead_nz, expect_fire, got_fire, (int)dd.outcome);
                }
            }
        } else if (const char* J = std::strstr(line, "DTFCJREC")) {
            // Stage-J replay: the binary's post +0x58/+0x5c writes must equal the lifted ladder's output
            // bit-exact (pure integer compare — the FP path inside is fully determined by the captured
            // float bits). Roll branch (pre5c ∈ {0,1}) additionally needs the captured :410 draw; its
            // (lo,hi) args are cross-checked against (int)(w228*100)/(int)(w22c*100) — a mismatch means
            // the recorded last-in-fire draw was NOT the cooldown roll (j_drawmiss, a FAIL).
            auto geti = [&](const char* key, long* o) {
                const char* k = std::strstr(J, key);
                return k && std::sscanf(k + (int)std::strlen(key), "%ld", o) == 1;
            };
            auto hexf = [&](const char* key, float* o) {
                const char* k = std::strstr(J, key);
                unsigned u;
                if (!k || std::sscanf(k + (int)std::strlen(key), "%x", &u) != 1) return false;
                std::memcpy(o, &u, 4); return true;
            };
            long pre5c, dn, da, db, dr, a0, a1, w230, gs, post58, post5c;
            float m0, m1, mod1, mod9, fold, w228, w22c, w234;
            if (!(geti("\tpre5c=",&pre5c) && geti("\tdn=",&dn) && geti("\tda=",&da) &&
                  geti("\tdb=",&db) && geti("\tdr=",&dr) && geti("\ta0=",&a0) && geti("\ta1=",&a1) &&
                  geti("\tw230=",&w230) && geti("\tgs=",&gs) &&
                  geti("\tpost58=",&post58) && geti("\tpost5c=",&post5c) &&
                  hexf("\tm0=",&m0) && hexf("\tm1=",&m1) && hexf("\tmod1=",&mod1) &&
                  hexf("\tmod9=",&mod9) && hexf("\tfold=",&fold) &&
                  hexf("\tw228=",&w228) && hexf("\tw22c=",&w22c) && hexf("\tw234=",&w234))) continue;
            ++j_records;
            CooldownModInputs m;
            m.cat0_active = (a0 != 0); m.cat0_mult = m0;
            m.cat1_active = (a1 != 0); m.cat1_mult = m1;
            m.owner_rof_mod = mod1; m.owner_recharge_mod = mod9; m.buff_fold = fold;
            m.burst_count = (eaw::u32)w230; m.shot_delay = w234;
            const float gsf = (float)gs;
            int counter = (int)pre5c;
            if (counter != 0) counter -= 1;
            CooldownState exp;
            if (counter == 0) {
                ++j_roll;
                const int lo = (int)(w228 * 100.0f), hi = (int)(w22c * 100.0f);
                if (dn < 1 || da != lo || db != hi) {
                    ++j_drawmiss;
                    if (j_drawmiss <= 16)
                        std::printf("  J DRAWMISS pre5c=%ld dn=%ld (da,db)=(%ld,%ld) want (%d,%d)\n",
                                    pre5c, dn, da, db, lo, hi);
                    continue;
                }
                const int base = (int)(((int64_t)((float)(int)dr * gsf) & 0xffffffffLL) / 100);
                exp = firing_recharge_after_burst(base, m);
            } else {
                ++j_btwn;
                exp = firing_delay_between_shots(gsf, m, counter);
            }
            if (exp.recharge != (int)post58) {
                ++j_recharge_bug;
                if (j_recharge_bug <= 16)
                    std::printf("  J RECHARGE pre5c=%ld sim=%d bin=%ld  (mod1=%g mod9=%g fold=%g a0=%ld a1=%ld)\n",
                                pre5c, exp.recharge, post58, mod1, mod9, fold, a0, a1);
            }
            if (exp.burst != (int)post5c) {
                ++j_burst_bug;
                if (j_burst_bug <= 16)
                    std::printf("  J BURST pre5c=%ld sim=%d bin=%ld  (w230=%ld mod9=%g fold=%g)\n",
                                pre5c, exp.burst, post5c, w230, mod9, fold);
            }
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
    std::printf("[lead solve]   records=%ld  verdict_mismatch=%ld  exact=%ld  near=%ld  far=%ld  max_rel_err=%.3g\n",
                l_records, l_verdict_mismatch, l_exact, l_near, l_far, l_max_err);
    std::printf("[end-to-end]   reach1=%ld  reach0=%ld  decide_fire=%ld  compose_fail=%ld\n",
                e_reach1, e_reach0, e_fire, e_compose_fail);
    std::printf("[stage-J]      records=%ld  roll=%ld  between=%ld  drawmiss=%ld  recharge_bug=%ld  burst_bug=%ld\n",
                j_records, j_roll, j_btwn, j_drawmiss, j_recharge_bug, j_burst_bug);

    int fail = 0;
    if (records == 0 && g_records == 0 && l_records == 0) { std::printf("NO RECORDS — capture produced no DTFC*REC lines\n"); return 1; }
    if (bug != 0)                  { std::printf("GATE FAIL — sim gate blocked %ld real shots\n", bug); fail = 1; }
    if (g_transcribe_mismatch != 0){ std::printf("RANGE FAIL — sim range gate disagrees with the binary on %ld records\n", g_transcribe_mismatch); fail = 1; }
    if (g_ground_bug != 0)         { std::printf("RANGE FAIL — sim range gate rejects %ld real shots\n", g_ground_bug); fail = 1; }
    if (l_verdict_mismatch != 0)   { std::printf("LEAD FAIL — sim lead disagrees with the binary on solution-existence for %ld records\n", l_verdict_mismatch); fail = 1; }
    if (l_far != 0)                { std::printf("LEAD FAIL — sim lead value diverges (>1e-4 rel) on %ld records (max %.3g)\n", l_far, l_max_err); fail = 1; }
    if (e_compose_fail != 0)       { std::printf("COMPOSE FAIL — fire_control_decide outcome != expected on %ld records\n", e_compose_fail); fail = 1; }
    if (j_drawmiss != 0)           { std::printf("STAGE-J FAIL — the captured in-fire draw was not the :410 cooldown roll on %ld records\n", j_drawmiss); fail = 1; }
    if (j_recharge_bug != 0)       { std::printf("STAGE-J FAIL — recharge (+0x58) mismatch on %ld records\n", j_recharge_bug); fail = 1; }
    if (j_burst_bug != 0)          { std::printf("STAGE-J FAIL — burst (+0x5c) mismatch on %ld records\n", j_burst_bug); fail = 1; }
    if (fail) return 1;
    std::printf("ALL PASS — gates clean (%ld shots); range bit-exact (%ld); lead bit-exact (%ld); fire_control_decide outcome matches the composed factors on all %ld end-to-end records; stage-J cooldown bit-exact (%ld)\n",
                fired, g_records, l_records, e_reach1 + e_reach0, j_records);
    return 0;
}
