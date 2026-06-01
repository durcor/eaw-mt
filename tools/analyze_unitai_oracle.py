#!/usr/bin/env python3
"""DTUAI in-game oracle: validate the lifted UnitAI fog/sensor-visibility ticker
(sim/unit_ai.cpp, from FUN_1404f6070) against real per-tick visibility updates captured from the
live binary. Behavior #4 — the first CROSS-ENTITY behavior (it writes a shared sensor grid), so the
deterministic-core part that can be reproduced BIT-EXACT is narrower than #1-3:

The winmm_proxy DTUAI trampoline emits, per observer-tick (refresh ticks + a sparse sample):
    DTUAI  tick=.. ent=HEX  ndb=I nda=I now=I  init=I chg=B cacheok=B
           vcnt=I vb=I va=I  e01=I e10=I  fog=B sen=B
where ndb/nda = next_due (ai+0x20) before/after, now = game[2], init = ai+0x18 before,
chg = (observer transform obj+0x248..+0x274 != cache ai+0x68..+0x94), cacheok = the hook's own check
that the cache landed where the lifted core says it should (recached to transform iff an advance
happened AND chg, else untouched), vcnt = ai+0x60 (#objects), vb/va = #set visibility bits before/
after (ai+0x58, capped scan), e01/e10 = visibility EDGES this tick (= the number of became_visible /
became_hidden notifies the core pushes into the shared grid), fog = sweep-vs-clear gate, sen = sensor
mgr present. Plus DTUAISURVEY periodic totals.

The lifted model (unit_ai_tick):
  * THROTTLE (bit-exact): if next_due <= now and sensor → next_due += 300 (FIXED phase, not now+300).
    So every tick where next_due CHANGED must show nda == ndb + 300, and ndb <= now, and sen=1.
  * TRANSFORM CACHE (bit-exact): on a refresh tick, cache := transform iff chg else untouched — the
    hook folds this into `cacheok` (computed against the live post-tick cache), so cacheok must be 1.
  * VISIBILITY MATRIX (characterised, not reproduced — the bit VALUE is the env LOS predicate): the
    edges e01/e10 are the cross-boundary notify count; under the clear path (fog=0) the array must end
    empty (va == 0) and produce no 0->1 edges.
Validation:
  (1) every advance tick (nda != ndb): nda == ndb + 300, ndb <= now, sen == 1.   [the throttle PASS]
  (2) cacheok == 1 on every sample.                                              [the cache PASS]
  (3) fog==0 ticks: va == 0 and e01 == 0.                                        [clear-path]
  (4) sanity: e01/e10 only where the array exists (vcnt>0); va/vb within [0,vcnt].

Usage: tools/analyze_unitai_oracle.py [eaw-mt.log]   (default: ./eaw-mt.log)
"""
import sys
import re

LINE = re.compile(
    r"DTUAI\ttick=(\d+)\tent=([0-9a-fA-F]+)\tndb=(-?\d+)\tnda=(-?\d+)\tnow=(-?\d+)"
    r"\tinit=(-?\d+)\tchg=(\d+)\tcacheok=(\d+)\tvcnt=(-?\d+)\tvb=(-?\d+)\tva=(-?\d+)"
    r"\te01=(-?\d+)\te10=(-?\d+)\tfog=(\d+)\tsen=(\d+)")


def main():
    path = sys.argv[1] if len(sys.argv) > 1 else "eaw-mt.log"
    rows = []
    with open(path, "r", errors="replace") as f:
        for line in f:
            m = LINE.search(line)
            if m:
                g = m.groups()
                rows.append(dict(
                    tick=int(g[0]), ent=g[1], ndb=int(g[2]), nda=int(g[3]), now=int(g[4]),
                    init=int(g[5]), chg=int(g[6]), cacheok=int(g[7]), vcnt=int(g[8]),
                    vb=int(g[9]), va=int(g[10]), e01=int(g[11]), e10=int(g[12]),
                    fog=int(g[13]), sen=int(g[14])))

    if not rows:
        print(f"NO DTUAI lines found in {path}")
        print("  (need a profile build with EAW_DIFFTRACE=1, in a battle with moving units)")
        return 1

    ents = {r["ent"] for r in rows}
    print(f"DTUAI oracle — {path}")
    print(f"  samples: {len(rows)}   distinct observers: {len(ents)}")

    adv = [r for r in rows if r["nda"] != r["ndb"]]
    # (1) throttle: every advance is exactly +300, only when due and sensor present
    bad_adv = [r for r in adv if not (r["nda"] == r["ndb"] + 300 and r["ndb"] <= r["now"] and r["sen"] == 1)]
    print(f"  throttle  : {len(adv)} advances; +300-exact & due & sensor: {len(adv) - len(bad_adv)}/{len(adv)}")
    for r in bad_adv[:8]:
        print(f"    BAD ndb={r['ndb']} nda={r['nda']} now={r['now']} sen={r['sen']} (want nda=ndb+300, ndb<=now)")

    # (2) transform cache post-condition (hook-computed)
    cache_bad = [r for r in rows if r["cacheok"] != 1]
    print(f"  cache     : cacheok==1 on {len(rows) - len(cache_bad)}/{len(rows)} samples")
    for r in cache_bad[:8]:
        print(f"    BAD tick={r['tick']} ent={r['ent']} chg={r['chg']} nda-ndb={r['nda']-r['ndb']}")

    # (3) clear-path: fog off => empty array, no 0->1 edges
    fogoff = [r for r in rows if r["fog"] == 0]
    clear_bad = [r for r in fogoff if r["va"] != 0 or r["e01"] != 0]
    if fogoff:
        print(f"  clear-path: {len(fogoff)} fog-off ticks; va==0 & e01==0: "
              f"{len(fogoff) - len(clear_bad)}/{len(fogoff)}")
        for r in clear_bad[:8]:
            print(f"    BAD tick={r['tick']} va={r['va']} e01={r['e01']}")

    # (4) bounds sanity
    bnd = [r for r in rows if not (0 <= r["vb"] <= max(r["vcnt"], 0) and 0 <= r["va"] <= max(r["vcnt"], 0))
           or ((r["e01"] or r["e10"]) and r["vcnt"] <= 0)]
    e01 = sum(r["e01"] for r in rows); e10 = sum(r["e10"] for r in rows)
    print(f"  vis matrix: edges 0->1={e01}  1->0={e10}   (= shared-grid notify count)   bounds-bad: {len(bnd)}")

    passed = (len(adv) > 0 and not bad_adv and not cache_bad and not clear_bad and not bnd)
    print("  RESULT    : " + (
        "PASS — lifted UnitAI reproduces the throttle + transform cache; vis matrix characterised"
        if passed else "INCOMPLETE/FAIL — see above"))
    if len(adv) == 0:
        print("    (no throttle advances captured yet — keep the battle running ~300+ ticks per unit)")
    return 0 if passed else 1


if __name__ == "__main__":
    sys.exit(main())
