#!/usr/bin/env python3
"""DTLURE in-game oracle: validate the lifted shared 3D squared-distance primitive
(sim/geom_distance.cpp, from FUN_140397640) against the live binary. Behavior #9 (Lure) numeric core.

LureBehaviorClass::vfunc_6 (0x62b4c0) is — like UnitAI #4 / Targeting #7 — predominantly a cross-entity
orchestration seam (scan the lure's target squads, apply the per-target lure to each member in range). Its
ONE embedded, pure, bit-matchable numeric core is the RADIUS GATE's squared distance, computed by the
shared leaf FUN_140397640:
    dx = entity.x - q.x   dy = entity.y - q.y   dz = entity.z - q.z         (float32)
    dist² = (double)(dx*dx) + (double)(dy*dy) + (double)(dz*dz)             grouped ((dx²+dy²)+dz²)
Each square is float32 (mulss) promoted to double (cvtps2pd); the sum accumulates in double (addsd).

FUN_140397640 is a 62-byte pure leaf (no intrinsic calls) with its own stable entry, reused by range
checks across many behaviors — so the winmm_proxy DTLURE hook oracles it DIRECTLY: snapshot entity.xyz
(rcx+0x78/0x7c/0x80) + query.xyz (rdx[0..2]) before the trampoline, run the real function once (returns
dist² in xmm0), recompute the prediction with the binary's exact float32-square / double-sum grouping, and
compare BIT-FOR-BIT (memcmp on the 8 double bytes). It emits per-call (on mismatch / first-40 / periodic):
    DTLURE tick=I dist2(a/p)=A/A ok=B          (A = %a hex float, full bit precision)
and a periodic rollup:
    DTLURESURVEY tick=I ticks=U dist2(ok/bad)=U/U zero=U max=A

This analyzer aggregates and validates: dist² is reproduced bit-for-bit on every call (bad == 0) with at
least one call exercised. It also reports coverage (zero-distance count, max dist² seen).
"""
import re
import sys

SURVEY = re.compile(
    r"DTLURESURVEY\ttick=(-?\d+)\tticks=(\d+)\tdist2\(ok/bad\)=(\d+)/(\d+)\tzero=(\d+)\tmax=(\S+)")
LINE = re.compile(r"DTLURE\ttick=(-?\d+)\tdist2\(a/p\)=(\S+)/(\S+)\tok=(\d+)")


def main():
    path = sys.argv[1] if len(sys.argv) > 1 else "eaw-mt.log"
    try:
        text = open(path, encoding="utf-8", errors="replace").read()
    except FileNotFoundError:
        print(f"no log at {path}")
        return 2

    surveys = [m.groups() for m in SURVEY.finditer(text)]
    lines = [m.groups() for m in LINE.finditer(text)]

    print(f"DTLURE — Lure shared squared-distance oracle (FUN_140397640)   [{path}]")
    print(f"  DTLURE lines: {len(lines)}   DTLURESURVEY rollups: {len(surveys)}")

    line_bad = sum(1 for r in lines if r[3] == "0")
    if lines:
        print(f"  emitted-line mismatches: {line_bad}")
        for r in lines[:6]:
            print(f"    tick={r[0]} dist2 a={r[1]} p={r[2]} ok={r[3]}")

    if not surveys:
        print("  no DTLURESURVEY rollups — distance primitive not exercised (no range checks this "
              "capture? — unlikely once a battle is loaded)")
        return 1

    (tick, ticks, ok, bad, zero, mx) = surveys[-1]
    tick, ticks, ok, bad, zero = int(tick), int(ticks), int(ok), int(bad), int(zero)
    print(f"  final survey @tick={tick}: ticks={ticks}")
    print(f"    dist2 (ok/bad) = {ok}/{bad}")
    print(f"    coverage: zero-distance calls = {zero}   max dist² (hex) = {mx}")
    print()

    present = ticks > 0
    passed = present and bad == 0 and ok > 0
    print("  SQUARED DISTANCE (dist² bit-exact):",
          "PASS" if passed else "FAIL" if present else "n/a (no calls)")
    print("  VERDICT:", "PASS" if passed else "FAIL")
    return 0 if passed else 1


if __name__ == "__main__":
    sys.exit(main())
