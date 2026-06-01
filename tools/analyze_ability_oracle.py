#!/usr/bin/env python3
"""DTABIL in-game oracle: validate the lifted AbilityCountdown ticker (sim/ability_countdown.cpp,
from FUN_14042f910) against real per-tick ability-cooldown updates captured from the live binary.

The winmm_proxy DTABIL trampoline emits, per active slot of one latched entity per tick:
    DTABIL  tick=..  ent=HEX  slot=N  mode=M  tb=INT  ta=INT  tgt=INT  delta=INT
where tb/ta = the slot timer (mgr+0x8+slot*4) before/after the tick, mode = mgr+0x270+slot
(0=countdown / !=0=chargeup), tgt = chargeup target (mgr+0x13c+slot*4), delta = tick - last_tick.
Plus, from ANY entity (latch-bypass), the rare informative cooldown→0 edge:
    DTABILEDGE  tick=..  ent=HEX  slot=N  tb=INT  delta=INT
(the exact tick a countdown slot reaches 0 = the sig-0x2c emit trigger).

The lifted model is PURE INTEGER (no scalar to recover, unlike DTNRG's dt):
    COUNTDOWN (mode==0):  if tb>0:  ta == max(tb - delta, 0)
    CHARGEUP  (mode!=0):  if tb<tgt: ta == min(tb + delta, tgt)   else ta == tb
Plus the deterministic DECISION to invoke the two env-modeled side acts (the core OWNS the decision
but deliberately does NOT reproduce their interiors — they live behind AbilityCountdownEnv):
  * countdown→0 EDGE : on the tick a cooldown reaches 0 → emit sig 0x2c (gated by ability_ready).
  * chargeup→target  : on the tick a chargeup reaches its target → invoke charge_complete (42f460),
                       which RECYCLES the slot into a cooldown — mode flips 1→0, the timer is reset
                       to a new (larger) cooldown value, the target field is updated. So on a
                       complete tick the OBSERVED ta is the env callback's recycle write (> tgt), NOT
                       the core's clamp-to-target — uncheckable from the pure core, exactly as DTNRG
                       excludes drain ticks. The following countdown IS reproduced bit-exact.
Validation:
  (1) reproduce ta bit-exact for every PURE-CORE sample (all countdown + chargeup below target).
  (2) CHARGE-COMPLETE ticks (chargeup reaching target, tb+delta>=tgt): verify the trigger the model
      fires on, and that the slot recycled (ta != tgt). EXCLUDED from (1) — env owns the post value.
  (3) the edge stream confirms the 0x2c trigger fires exactly when a countdown slot lands on 0.
  (4) sanity: ta never negative; chargeup ta exceeds tgt ONLY on a charge-complete recycle.

Usage: tools/analyze_ability_oracle.py [eaw-mt.log]   (default: ./eaw-mt.log)
"""
import sys
import re

LINE = re.compile(
    r"DTABIL\ttick=(\d+)\tent=([0-9a-fA-F]+)\tslot=(\d+)\tmode=(\d+)"
    r"\ttb=(-?\d+)\tta=(-?\d+)\ttgt=(-?\d+)\tdelta=(-?\d+)")

EDGE = re.compile(
    r"DTABILEDGE\ttick=(\d+)\tent=([0-9a-fA-F]+)\tslot=(\d+)\ttb=(-?\d+)\tdelta=(-?\d+)")


def model(mode, tb, tgt, delta):
    """The lifted ability_countdown_tick per-slot recurrence (integer)."""
    if mode == 0:                       # countdown
        if tb > 0:
            t = tb - delta
            return 0 if t < 1 else t
        return tb                       # already 0 → skipped (no write)
    # chargeup
    if tb < tgt:
        v = tb + delta
        return v if v < tgt else tgt
    return tb


def main():
    path = sys.argv[1] if len(sys.argv) > 1 else "eaw-mt.log"
    rows, edges = [], []
    with open(path, "r", errors="replace") as f:
        for line in f:
            m = LINE.search(line)
            if m:
                rows.append(dict(tick=int(m.group(1)), ent=m.group(2), slot=int(m.group(3)),
                                 mode=int(m.group(4)), tb=int(m.group(5)), ta=int(m.group(6)),
                                 tgt=int(m.group(7)), delta=int(m.group(8))))
                continue
            me = EDGE.search(line)
            if me:
                edges.append(dict(tick=int(me.group(1)), ent=me.group(2), slot=int(me.group(3)),
                                  tb=int(me.group(4)), delta=int(me.group(5))))

    if not rows and not edges:
        print(f"NO DTABIL lines found in {path}")
        print("  (need a profile build with EAW_DIFFTRACE=1, in a battle with units that have "
              "abilities on cooldown / charging)")
        return 1

    ents = {r["ent"] for r in rows} | {e["ent"] for e in edges}
    print(f"DTABIL oracle — {path}")
    print(f"  routine samples : {len(rows)}   edges(bypass): {len(edges)}   "
          f"distinct entities: {len(ents)}")

    cd = [r for r in rows if r["mode"] == 0]
    cu = [r for r in rows if r["mode"] != 0]
    # A chargeup tick whose new value reaches/passes its target is a CHARGE-COMPLETE event: the core
    # clamps to tgt then invokes charge_complete (42f460), which RECYCLES the slot (env-owned write);
    # the observed ta is that recycle, not the pure-core value, so it is excluded from (1).
    def is_complete(r):
        return r["mode"] != 0 and 0 < r["tgt"] <= r["tb"] + r["delta"] and r["tb"] <= r["tgt"]
    complete = [r for r in rows if is_complete(r)]
    core = [r for r in rows if not is_complete(r)]
    moved = [r for r in rows if r["ta"] != r["tb"]]
    print(f"  by mode         : countdown={len(cd)}  chargeup={len(cu)}   moved(ta!=tb)={len(moved)}")

    # (1) reproduce ta bit-exact for the PURE-CORE samples (env-callback recycle ticks excluded)
    ok = tot = 0
    fails = []
    for r in core:
        tot += 1
        pred = model(r["mode"], r["tb"], r["tgt"], r["delta"])
        if pred == r["ta"]:
            ok += 1
        elif len(fails) < 15:
            fails.append(r)
    print(f"  model reproduces: {ok}/{tot} ta bit-exact (pure core)")
    if fails:
        print("  first mismatches (tick slot mode tb ta model tgt delta):")
        for r in fails:
            print(f"    t={r['tick']} slot={r['slot']} mode={r['mode']} tb={r['tb']} "
                  f"ta={r['ta']} model={model(r['mode'], r['tb'], r['tgt'], r['delta'])} "
                  f"tgt={r['tgt']} delta={r['delta']}")

    # (2) charge-complete events: the model fires charge_complete here; verify the slot recycled
    # (ta != the clamp target). Their post-value is env-owned (uncheckable) but the trigger is the
    # deterministic core decision, and the recycle write proves the callback ran.
    cc_recycled = sum(1 for r in complete if r["ta"] != r["tgt"])
    if complete:
        print(f"  charge-complete : {len(complete)} events; recycled(ta!=tgt): {cc_recycled} "
              f"(charge_complete invoked → slot reset to cooldown, env-owned)")

    # (4) bounds: ta never negative; chargeup over-target ONLY on a recycle tick.
    neg = sum(1 for r in rows if r["ta"] < 0)
    over = sum(1 for r in cu if r["tgt"] > 0 and r["ta"] > r["tgt"] and not is_complete(r))
    print(f"  bounds          : ta<0: {neg}   chargeup ta>tgt off a recycle: {over}")

    # (3) edges — each should be a countdown slot that lands exactly on 0 (tb-delta <= 0)
    edge_ok = sum(1 for e in edges if e["tb"] > 0 and e["tb"] - e["delta"] <= 0)
    if edges:
        print(f"  edges consistent: {edge_ok}/{len(edges)} (tb>0 and tb-delta<=0 → lands on 0)")

    passed = (tot > 0 and ok == tot and neg == 0 and over == 0
              and (not complete or cc_recycled == len(complete))
              and (not edges or edge_ok == len(edges)))
    print("  RESULT          : " + (
        "PASS — lifted AbilityCountdown reproduces the live binary"
        if passed else "INCOMPLETE/FAIL — see above"))
    return 0 if passed else 1


if __name__ == "__main__":
    sys.exit(main())
