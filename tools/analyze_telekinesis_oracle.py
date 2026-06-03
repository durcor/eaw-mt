#!/usr/bin/env python3
"""DTTK in-game oracle: validate the lifted TELEKINESIS core (sim/telekinesis_target.cpp) against the
live binary. Behavior #8: part 1 = interp timeline (FUN_14063f210), part 2 = HOLD (FUN_14063f730).

TelekinesisTargetBehaviorClass::vfunc_6 (0x63f210) is the Force force-grip effect: a 3-mode lifecycle
SM over a per-entity telekinesis slot (slot = entity+0x160; mode int at slot+0x8). mode 1 GRAB and
mode 3 RELEASE share one interp timeline; mode 2 HOLD (63f730, spin + damage) is deferred. Timeline:
    now      = (float)sim_clock / (float)hz            (DAT_140b0a320 / DAT_140b0a340)
    t        = max((now - start)/duration, 0)           (start=slot+0x24, dur=_DAT_140b15ac4)
    complete = (1.0 <= t)
GRAB lerps toward DAT_140b15ac0 + slot+0x14; RELEASE toward slot+0x14. The interpolated value is the Z
of the set-position apply -> entity+0x80. On completion both modes rebase slot+0x24 := now and CHANGE
slot+0x8 (GRAB 1->2, RELEASE 3->0) — the only writes to slot+0x8 in either body, so a mode change across
the tick == a completion edge.

The winmm_proxy DTTK hook, per telekinesis-entity-tick on an interp mode (1 or 3), reads the slot fields
+ config globals BEFORE the trampoline (faithful — the base vfunc_6 runs inside it but only writes
behavior+0x30), computes the predicted timeline, runs the real call, then emits:
    DTTK tick=I ent=HEX mode=I mode_after=I t=F pred_comp=B act_comp=B bicond=B rebase_ok=B interp_ok=B
         val80=F lerp=F
The hook computes the bit-exact comparisons in-process (memcmp of the float bits). This analyzer
aggregates and independently re-derives the booleans' internal consistency, validating:
  (1) COMPLETION BICONDITIONAL — pred_comp == act_comp on every interp tick: the lifted timeline decides
      the snap-to-final edge exactly when the binary changes slot+0x8. [the core PASS]
  (2) REBASE BIT-EXACT — on every completion edge, slot+0x24_after == predicted now (rebase_ok). The
      engine writes slot+0x24 := now; a rock-solid explicit slot store.
  (3) INTERP LERP BIT-EXACT — on every NON-completion tick, entity+0x80 == predicted
      lerp(slot+0x30, to, t) bit-for-bit (interp_ok) — the continuously-observable quantity (Nebula-class).
It also reports the empirical mode DISTRIBUTION (GRAB/HOLD/RELEASE/idle) + completion-edge count —
intel for whether RELEASE/HOLD are exercised and worth a part-2 sub-body lift.

Usage: tools/analyze_telekinesis_oracle.py [eaw-mt.log]   (default: ./eaw-mt.log)
"""
import sys
import re

LINE = re.compile(
    r"DTTK\ttick=(-?\d+)\tent=([0-9a-fA-F]+)\tmode=(-?\d+)\tmode_after=(-?\d+)\tt=(\S+)"
    r"\tpred_comp=(\d+)\tact_comp=(\d+)\tbicond=(\d+)\trebase_ok=(\d+)\tinterp_ok=(\d+)"
    r"\tval80=(\S+)\tlerp=(\S+)")
SURVEY = re.compile(
    r"DTTKSURVEY\ttick=(-?\d+)\tticks=(\d+)\tnoslot=(\d+)\tmode\(grab/hold/rel/idle\)=(\d+)/(\d+)/(\d+)/(\d+)"
    r"\tinterp\(ok/bad\)=(\d+)/(\d+)\tcomp_edges=(\d+)\tbicond\(ok/bad\)=(\d+)/(\d+)\trebase\(ok/bad\)=(\d+)/(\d+)")
# HOLD totals are an optional tail on the survey line (part-2 build); parsed separately so old logs still match.
SURVEY_HOLD = re.compile(
    r"hold_pose\(ok/bad\)=(\d+)/(\d+)\thold_sched\(ok/bad\)=(\d+)/(\d+)\thold_fires=(\d+)")
# Part-2 per-tick HOLD record (mode 2): (4) z-bob pose entity+0x80, (5) damage-deadline rebase.
HOLD = re.compile(
    r"DTTKHOLD\ttick=(-?\d+)\tent=([0-9a-fA-F]+)\tspin_t=(\S+)\tangle=(\S+)\tdue=(\d+)"
    r"\tpose_ok=(\d+)\tsched_ok=(\d+)\tval80=(\S+)\tpredz=(\S+)\tnxt_after=(\S+)\tpred_nxt=(\S+)")
# RELEASE totals are an optional tail on the survey line (part-3 build); parsed separately.
SURVEY_REL = re.compile(
    r"rel_edges=(\d+)\trel_term\(ok/bad\)=(\d+)/(\d+)\trel_sent\(ok/bad\)=(\d+)/(\d+)\trel_disarm\(ok/bad\)=(\d+)/(\d+)")
# Part-3 per-edge RELEASE teardown record (mode 3 completion): (6) terminus 0, (7) sentinel, (8) disarm.
REL = re.compile(
    r"DTTKREL\ttick=(-?\d+)\tent=([0-9a-fA-F]+)\tmode_after=(-?\d+)\tterm_ok=(\d+)\tsent=(\S+)"
    r"\tsent_ok=(\d+)\tdmg_after=(\S+)\tnxt_after=(\S+)\tdisarm_ok=(\d+)")

MODE_NAME = {1: "GRAB(63f210)", 2: "HOLD(63f730)", 3: "RELEASE(63f470)"}


def main():
    path = sys.argv[1] if len(sys.argv) > 1 else "eaw-mt.log"
    rows = []
    hold_rows = []
    rel_rows = []
    last_survey = None
    last_survey_hold = None
    last_survey_rel = None
    with open(path, "r", errors="replace") as fh:
        for line in fh:
            m = LINE.search(line)
            if m:
                rows.append(m.groups())
                continue
            h = HOLD.search(line)
            if h:
                hold_rows.append(h.groups())
                continue
            rr = REL.search(line)
            if rr:
                rel_rows.append(rr.groups())
                continue
            s = SURVEY.search(line)
            if s:
                last_survey = s.groups()
                sh = SURVEY_HOLD.search(line)
                last_survey_hold = sh.groups() if sh else None
                sr = SURVEY_REL.search(line)
                last_survey_rel = sr.groups() if sr else None

    if not rows and not hold_rows and not rel_rows:
        print(f"NO DTTK LINES in {path} — was EAW_DIFFTRACE=1 set, and did a battle with a telekinesis "
              f"(Force force-grip) cast run? The behavior only interp-ticks while a unit is gripped.")
        return 1

    n = len(rows)
    bicond_ok = bicond_bad = 0
    rebase_ok = rebase_bad = 0
    interp_ok = interp_bad = 0
    comp_edges = 0
    mode_hist = {}
    entities = set()

    for (tick, ent, mode_s, mode_after_s, t_s, predc_s, actc_s,
         bicond_s, rebase_s, interp_s, val80_s, lerp_s) in rows:
        mode = int(mode_s)
        predc = predc_s == "1"
        actc = actc_s == "1"
        entities.add(ent)
        mode_hist[mode] = mode_hist.get(mode, 0) + 1

        # (1) completion biconditional — the hook's bicond flag must equal (pred_comp == act_comp).
        if bicond_s == "1" and (predc == actc):
            bicond_ok += 1
        else:
            bicond_bad += 1

        if actc:
            comp_edges += 1
            # (2) rebase bit-exact on the completion edge.
            if rebase_s == "1":
                rebase_ok += 1
            else:
                rebase_bad += 1
        else:
            # (3) interp lerp bit-exact on a non-completion tick.
            if interp_s == "1":
                interp_ok += 1
            else:
                interp_bad += 1

    # ── part 2 (HOLD, FUN_14063f730): (4) z-bob pose + (5) damage-deadline rebase ───────────────────
    hold_pose_ok = hold_pose_bad = 0
    hold_sched_ok = hold_sched_bad = 0
    hold_fires = 0
    for (htick, hent, hspin, hang, hdue, hpose, hsched, hv80, hpz, hnxt, hpnxt) in hold_rows:
        if hpose == "1":
            hold_pose_ok += 1
        else:
            hold_pose_bad += 1
        if hsched == "1":
            hold_sched_ok += 1
        else:
            hold_sched_bad += 1
        if hdue == "1":
            hold_fires += 1

    # ── part 3 (RELEASE, FUN_14063f470): (6) terminus 0 + (7) sentinel + (8) damage disarm ───────────
    rel_term_ok = rel_term_bad = 0
    rel_sent_ok = rel_sent_bad = 0
    rel_disarm_ok = rel_disarm_bad = 0
    for (rtick, rent, rmode_after, rterm, rsent, rsent_ok, rdmg, rnxt, rdisarm) in rel_rows:
        if rterm == "1":
            rel_term_ok += 1
        else:
            rel_term_bad += 1
        if rsent_ok == "1":
            rel_sent_ok += 1
        else:
            rel_sent_bad += 1
        if rdisarm == "1":
            rel_disarm_ok += 1
        else:
            rel_disarm_bad += 1

    print("DTTK oracle — Telekinesis interp-timeline core (FUN_14063f210)")
    print(f"  log: {path}")
    print(f"  telekinesis interp-ticks (logged): {n}   distinct entities: {len(entities)}")
    print()
    print("  (1) COMPLETION BICONDITIONAL  pred_comp == act_comp  (lifted t>=1 edge == binary slot+0x8 change)")
    print(f"        bicond_ok={bicond_ok}/{n}   bicond_bad={bicond_bad}")
    print()
    print("  (2) REBASE BIT-EXACT  (on completion: slot+0x24_after == now)")
    print(f"        rebase_ok={rebase_ok}/{comp_edges}   rebase_bad={rebase_bad}   (completion edges: {comp_edges})")
    print()
    print("  (3) INTERP LERP BIT-EXACT  (non-completion: entity+0x80 == lerp(slot+0x30, to, t))")
    print(f"        interp_ok={interp_ok}   interp_bad={interp_bad}")
    print()
    print("  --- part 2: HOLD spin/bob + damage scheduler (FUN_14063f730) ---")
    print(f"  HOLD ticks (logged): {len(hold_rows)}")
    print("  (4) Z-BOB POSE BIT-EXACT  (entity+0x80 == slot+0x14 + sin(spin_t*omega)*bob + height_offset)")
    print(f"        hold_pose_ok={hold_pose_ok}   hold_pose_bad={hold_pose_bad}")
    print("  (5) DAMAGE-DEADLINE REBASE BIT-EXACT  (on fire: slot+0x48_after == interval + slot+0x48)")
    print(f"        hold_sched_ok={hold_sched_ok}   hold_sched_bad={hold_sched_bad}   damage fires: {hold_fires}")
    print()
    print("  --- part 3: RELEASE teardown end-state (FUN_14063f470) ---")
    print(f"  RELEASE completion edges (logged): {len(rel_rows)}")
    print("  (6) MODE TERMINUS  (slot+0x8_after == 0 — lifecycle over, not just changed)")
    print(f"        rel_term_ok={rel_term_ok}   rel_term_bad={rel_term_bad}")
    print("  (7) SENTINEL  (slot+0x4c_after == 0x3fffff)")
    print(f"        rel_sent_ok={rel_sent_ok}   rel_sent_bad={rel_sent_bad}")
    print("  (8) DAMAGE DISARM  (slot+0x40_after == slot+0x48_after == 0 — HOLD clock off)")
    print(f"        rel_disarm_ok={rel_disarm_ok}   rel_disarm_bad={rel_disarm_bad}")
    print()
    print("  DISTRIBUTION")
    print("        mode:  " +
          "  ".join(f"{MODE_NAME.get(m, 'mode='+str(m))}:{c}" for m, c in sorted(mode_hist.items())))
    print()
    if last_survey:
        (st, sticks, sno, sg, sh, sr, si, sio, sib, sce, sbo, sbb, sro, srb) = last_survey
        extra = ""
        if last_survey_hold:
            (hpo, hpb, hso, hsb, hf) = last_survey_hold
            extra = (f" hold_pose(ok/bad)={hpo}/{hpb} hold_sched(ok/bad)={hso}/{hsb} hold_fires={hf}")
        if last_survey_rel:
            (re_, rto, rtb, rso, rsb, rdo, rdb) = last_survey_rel
            extra += (f" rel_edges={re_} rel_term(ok/bad)={rto}/{rtb} rel_sent(ok/bad)={rso}/{rsb} "
                      f"rel_disarm(ok/bad)={rdo}/{rdb}")
        print(f"  DTTKSURVEY (in-hook totals, last @tick {st}): ticks={sticks} noslot={sno} "
              f"mode(grab/hold/rel/idle)={sg}/{sh}/{sr}/{si} interp(ok/bad)={sio}/{sib} "
              f"comp_edges={sce} bicond(ok/bad)={sbo}/{sbb} rebase(ok/bad)={sro}/{srb}{extra}")
    print()
    # Part-1 PASS: every biconditional held, every observed completion rebased exactly, every interp lerp
    # was bit-exact, and we saw interp ticks. (comp_edges may legitimately be 0 if no grab completed within
    # the capture — like #1's 0x2d emit / #5's LINGER pins; that does not fail the run.)
    p1_present = len(rows) > 0
    p1_pass = (bicond_bad == 0 and rebase_bad == 0 and interp_bad == 0 and bicond_ok > 0)
    # Part-2 PASS: every HOLD z-bob pose was bit-exact, every damage-deadline rebase was bit-exact, and we
    # observed HOLD ticks. (damage fires may be 0 if no grip held long enough to cross a deadline; the pose
    # check still validates the spin/bob math every HOLD tick.)
    p2_present = len(hold_rows) > 0
    p2_pass = (hold_pose_bad == 0 and hold_sched_bad == 0 and hold_pose_ok > 0)
    # Part-3 PASS: every RELEASE completion edge hit the terminus 0, wrote the 0x3fffff sentinel, and
    # disarmed the damage fields — bit-for-bit. Needs at least one RELEASE edge to be a real PASS (a grip
    # must have been let go within the capture; HOLD without a release just leaves part 3 n/a).
    p3_present = len(rel_rows) > 0
    p3_pass = (rel_term_bad == 0 and rel_sent_bad == 0 and rel_disarm_bad == 0 and rel_term_ok > 0)
    print(f"  PART 1 (interp timeline): {'PASS' if p1_pass else 'FAIL' if p1_present else 'n/a (no interp ticks)'}")
    print(f"  PART 2 (HOLD spin+damage): {'PASS' if p2_pass else 'FAIL' if p2_present else 'n/a (no HOLD ticks)'}")
    print(f"  PART 3 (RELEASE teardown): {'PASS' if p3_pass else 'FAIL' if p3_present else 'n/a (no RELEASE edges)'}")
    # Overall PASS requires every PRESENT part to pass, and at least one part exercised.
    verdict_pass = ((not p1_present or p1_pass) and (not p2_present or p2_pass) and (not p3_present or p3_pass)
                    and (p1_present or p2_present or p3_present))
    print("  VERDICT:", "PASS" if verdict_pass else "FAIL")
    return 0 if verdict_pass else 1


if __name__ == "__main__":
    sys.exit(main())
