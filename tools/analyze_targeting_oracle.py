#!/usr/bin/env python3
"""DTARG in-game oracle: validate the lifted Targeting game-mode DISPATCHER
(sim/targeting_dispatch.cpp, from FUN_140633a30) against the live binary. Behavior #7 (part 1).

Targeting is the AI fire-control orchestrator; 0x633a30 is its thin dispatcher: run the base
LocomotorCommonClass::vfunc_6 (a stub that only writes behavior+0x30 — touches neither the entity nor
the controller), resolve a mode int, then route mode 1 -> FUN_140634810 (individual targeting),
mode 2 -> FUN_140633ae0 (team targeting), else NoOp (return 0x650000). Mode resolution: controller =
entity->get_component(1) (vtbl +0x10); controller present -> controller->control_mode() (vtbl +0x238);
else DAT_140b15418 present ? game-speed mode (vtbl +0xe0) : default DAT_140b153fc.

The dispatcher core is a trivial integer decision, so the oracle validates it as a BICONDITIONAL
predicted == actual. The winmm_proxy DTARG hook, per targeting-entity-tick, replicates the resolution
(PREDICTED impl) and snapshots two entry-marker counters on FUN_140634810/FUN_140633ae0 around the real
call (ACTUAL impl), emitting:
    DTARG  tick=I ent=HEX  mode=I src=I pred=I act=I  d1=U d2=U  bicond=B multi=B readok=B
where mode = the resolved mode int, src = 0 controller / 1 manager game-speed / 2 default, pred =
mode==1?1:mode==2?2:0, act = which sub-impl actually ran (1/2/0 NoOp; -1 if BOTH ran = `multi`),
d1/d2 = the per-impl marker deltas. Plus DTARGSURVEY periodic totals.

This analyzer independently re-derives pred from the logged mode and validates:
  (1) DISPATCH BICONDITIONAL — pred == act on every tick: the lifted resolve+dispatch reproduces which
      sub-implementation the binary actually invoked. [the core PASS]
  (2) MUTUAL EXCLUSION — never both sub-impls in one dispatch (multi == 0): the dispatch is an
      if / else-if / else, exactly one (or no) branch per call.
  (3) RESOLUTION CONSISTENCY — pred recomputed from mode matches the hook's pred (sanity on the field).
It also reports the empirical DISTRIBUTION (mode-source mix + Mode1/Mode2/NoOp frequency) — which is the
useful intel for prioritising the deferred sub-body lifts (is team-targeting mode 2 even exercised?).

Usage: tools/analyze_targeting_oracle.py [eaw-mt.log]   (default: ./eaw-mt.log)
"""
import sys
import re

LINE = re.compile(
    r"DTARG\ttick=(-?\d+)\tent=([0-9a-fA-F]+)\tmode=(-?\d+)\tsrc=(-?\d+)\tpred=(-?\d+)\tact=(-?\d+)"
    r"\td1=(\d+)\td2=(\d+)\tbicond=(\d+)\tmulti=(\d+)\treadok=(\d+)")
SURVEY = re.compile(
    r"DTARGSURVEY\ttick=(-?\d+)\tticks=(\d+)\tpred\(m1/m2/noop\)=(\d+)/(\d+)/(\d+)"
    r"\tact\(m1/m2/noop\)=(\d+)/(\d+)/(\d+)\tsrc\(ctrl/mgr/def\)=(\d+)/(\d+)/(\d+)"
    r"\tbicond_ok=(\d+)\tbicond_bad=(\d+)\tmulti=(\d+)")

SRC_NAME = {0: "controller", 1: "manager-gamespeed", 2: "default"}
IMPL_NAME = {0: "NoOp", 1: "Mode1(634810)", 2: "Mode2(633ae0)", -1: "MULTI"}


def predict(mode):
    return 1 if mode == 1 else 2 if mode == 2 else 0


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
        print(f"NO DTARG LINES in {path} — was EAW_DIFFTRACE=1 set, and did a battle with combat units "
              f"run (the Targeting behavior ticks for every weapon-carrying unit)?")
        return 1

    n = len(rows)
    bicond_ok = bicond_bad = 0
    multi_cnt = 0
    pred_recompute_bad = 0
    readok_bad = 0
    src_hist = {}
    mode_hist = {}
    pred_hist = {0: 0, 1: 0, 2: 0}
    act_hist = {0: 0, 1: 0, 2: 0, -1: 0}
    entities = set()

    for (tick, ent, mode_s, src_s, pred_s, act_s, d1_s, d2_s,
         bicond_s, multi_s, readok_s) in rows:
        mode = int(mode_s)
        src = int(src_s)
        pred = int(pred_s)
        act = int(act_s)
        multi = multi_s == "1"
        readok = readok_s == "1"
        entities.add(ent)

        # (3) recompute pred from the logged mode — sanity that the hook's pred field is consistent.
        if predict(mode) != pred:
            pred_recompute_bad += 1

        # (1) dispatch biconditional pred == act (and the row's own bicond flag agrees).
        bok = (act >= 0) and (pred == act) and (bicond_s == "1")
        if bok:
            bicond_ok += 1
        else:
            bicond_bad += 1

        # (2) mutual exclusion.
        if multi:
            multi_cnt += 1
        if not readok:
            readok_bad += 1

        src_hist[src] = src_hist.get(src, 0) + 1
        mode_hist[mode] = mode_hist.get(mode, 0) + 1
        if pred in pred_hist:
            pred_hist[pred] += 1
        if act in act_hist:
            act_hist[act] += 1

    print("DTARG oracle — Targeting game-mode dispatcher (FUN_140633a30)")
    print(f"  log: {path}")
    print(f"  targeting-entity-ticks (logged): {n}   distinct entities: {len(entities)}")
    print()
    print("  (1) DISPATCH BICONDITIONAL  pred == act  (lifted resolve+dispatch == binary's branch)")
    print(f"        bicond_ok={bicond_ok}/{n}   bicond_bad={bicond_bad}")
    print()
    print("  (2) MUTUAL EXCLUSION  (never both sub-impls in one dispatch)")
    print(f"        multi (both ran)={multi_cnt}   read_ok failures={readok_bad}")
    print()
    print("  (3) RESOLUTION CONSISTENCY  (pred recomputed from mode == hook pred)")
    print(f"        mismatches={pred_recompute_bad}")
    print()
    print("  DISTRIBUTION")
    print("        mode source:  " +
          "  ".join(f"{SRC_NAME.get(s, s)}:{c}" for s, c in sorted(src_hist.items())))
    print("        mode int:     " +
          "  ".join(f"mode={m}:{c}" for m, c in sorted(mode_hist.items())))
    print("        predicted:    " +
          "  ".join(f"{IMPL_NAME[i]}:{pred_hist[i]}" for i in (1, 2, 0)))
    print("        actual:       " +
          "  ".join(f"{IMPL_NAME[i]}:{act_hist[i]}" for i in (1, 2, 0, -1)))
    print()
    if last_survey:
        (st, sticks, pm1, pm2, pno, am1, am2, ano, sc, sm, sd, sbo, sbb, smu) = last_survey
        print(f"  DTARGSURVEY (in-hook totals, last @tick {st}): ticks={sticks} "
              f"pred(m1/m2/noop)={pm1}/{pm2}/{pno} act(m1/m2/noop)={am1}/{am2}/{ano} "
              f"src(ctrl/mgr/def)={sc}/{sm}/{sd} bicond_ok={sbo} bicond_bad={sbb} multi={smu}")
    print()
    verdict_pass = (bicond_bad == 0 and multi_cnt == 0 and pred_recompute_bad == 0 and
                    readok_bad == 0 and bicond_ok > 0)
    print("  VERDICT:", "PASS" if verdict_pass else "FAIL")
    return 0 if verdict_pass else 1


if __name__ == "__main__":
    sys.exit(main())
