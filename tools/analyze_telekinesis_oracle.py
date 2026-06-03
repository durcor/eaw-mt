#!/usr/bin/env python3
"""DTTK in-game oracle: validate the lifted TELEKINESIS interp-timeline core
(sim/telekinesis_target.cpp, from FUN_14063f210) against the live binary. Behavior #8 (part 1).

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

MODE_NAME = {1: "GRAB(63f210)", 2: "HOLD(63f730)", 3: "RELEASE(63f470)"}


def main():
    path = sys.argv[1] if len(sys.argv) > 1 else "eaw-mt.log"
    rows = []
    last_survey = None
    with open(path, "r", errors="replace") as fh:
        for line in fh:
            m = LINE.search(line)
            if m:
                rows.append(m.groups())
                continue
            s = SURVEY.search(line)
            if s:
                last_survey = s.groups()

    if not rows:
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
    print("  DISTRIBUTION")
    print("        mode:  " +
          "  ".join(f"{MODE_NAME.get(m, 'mode='+str(m))}:{c}" for m, c in sorted(mode_hist.items())))
    print()
    if last_survey:
        (st, sticks, sno, sg, sh, sr, si, sio, sib, sce, sbo, sbb, sro, srb) = last_survey
        print(f"  DTTKSURVEY (in-hook totals, last @tick {st}): ticks={sticks} noslot={sno} "
              f"mode(grab/hold/rel/idle)={sg}/{sh}/{sr}/{si} interp(ok/bad)={sio}/{sib} "
              f"comp_edges={sce} bicond(ok/bad)={sbo}/{sbb} rebase(ok/bad)={sro}/{srb}")
    print()
    # PASS: every biconditional held, every observed completion rebased exactly, every interp lerp was
    # bit-exact, and we actually saw interp ticks. (comp_edges may legitimately be 0 if no grab completed
    # within the capture — like #1's 0x2d emit / #5's LINGER pins; that does not fail the run.)
    verdict_pass = (bicond_bad == 0 and rebase_bad == 0 and interp_bad == 0 and bicond_ok > 0)
    print("  VERDICT:", "PASS" if verdict_pass else "FAIL")
    return 0 if verdict_pass else 1


if __name__ == "__main__":
    sys.exit(main())
