#!/usr/bin/env python3
"""DTREVEAL in-game oracle: validate the lifted Reveal fog re-reveal MOVE-THRESHOLD GATE
(sim/reveal_gate.cpp, from RevealBehaviorClass::vfunc_6 FUN_1405373c0) against the live binary. #10.

RevealBehaviorClass is the passive fog-of-war revealer on ~every mobile unit. Each tick it throttles
its global object re-scan (the deferred Phase-B emit seam) on movement: it only re-reveals once the unit
has moved far enough from where it last revealed. The embedded pure numeric core is that gate — a 2D
float32 squared move distance:
    dx = entity.x - last.x   dy = entity.y - last.y                          (float32 subss)
    moved_sq = dx*dx + dy*dy                                                 (float32 mulss/mulss/ADDSS)
    skip_rescan = moved_sq < threshold                                       (comiss/jb)
Unlike #9's leaf, this stays entirely in float32 (no double promotion) and is MID-FUNCTION — the binary
never persists moved_sq. So the winmm_proxy DTREVEAL hook trampolines at the function entry (0x5373c0),
snapshots cur (entity +0x78/+0x7c) and last/threshold (behavior +0x40/+0x44/+0x48) BEFORE the call (the
rescan path overwrites +0x40/+0x44), then compares the lifted float32 expression against an INLINE-ASM
transcription of the gate's exact SSE sequence (subss/subss/mulss/mulss/addss) BIT-FOR-BIT (memcmp on the
4 float bytes). This confirms the lift reproduces the binary's instructions (no FMA / double-promotion /
reassociation) across the live in-game input distribution. It emits per-call (mismatch / first-40 /
periodic):
    DTREVEAL tick=I moved2(a/p)=A/A thr=A skip=B ok=B          (A = %a hex float, full bit precision)
and a periodic rollup:
    DTREVEALSURVEY tick=I ticks=U moved2(ok/bad)=U/U zero=U skip/rescan=U/U max=A

This analyzer aggregates and validates: moved_sq is reproduced bit-for-bit on every call (bad == 0) with
at least one call exercised. It also reports coverage (stationary count, skip vs rescan split, max moved²).
"""
import re
import sys

SURVEY = re.compile(
    r"DTREVEALSURVEY\ttick=(-?\d+)\tticks=(\d+)\tmoved2\(ok/bad\)=(\d+)/(\d+)"
    r"\tzero=(\d+)\tskip/rescan=(\d+)/(\d+)\tmax=(\S+)")
LINE = re.compile(r"DTREVEAL\ttick=(-?\d+)\tmoved2\(a/p\)=(\S+)/(\S+)\tthr=(\S+)\tskip=(\d+)\tok=(\d+)")


def main():
    path = sys.argv[1] if len(sys.argv) > 1 else "eaw-mt.log"
    try:
        text = open(path, encoding="utf-8", errors="replace").read()
    except FileNotFoundError:
        print(f"no log at {path}")
        return 2

    surveys = [m.groups() for m in SURVEY.finditer(text)]
    lines = [m.groups() for m in LINE.finditer(text)]

    print(f"DTREVEAL — Reveal move-threshold gate oracle (FUN_1405373c0)   [{path}]")
    print(f"  DTREVEAL lines: {len(lines)}   DTREVEALSURVEY rollups: {len(surveys)}")

    line_bad = sum(1 for r in lines if r[5] == "0")
    if lines:
        print(f"  emitted-line mismatches: {line_bad}")
        for r in lines[:6]:
            print(f"    tick={r[0]} moved2 a={r[1]} p={r[2]} thr={r[3]} skip={r[4]} ok={r[5]}")

    if not surveys:
        print("  no DTREVEALSURVEY rollups — gate not exercised (no RevealBehavior ticks this capture? "
              "— unlikely once any battle with moving units is loaded)")
        return 1

    (tick, ticks, ok, bad, zero, skip, rescan, mx) = surveys[-1]
    tick, ticks, ok, bad, zero, skip, rescan = (
        int(tick), int(ticks), int(ok), int(bad), int(zero), int(skip), int(rescan))
    print(f"  final survey @tick={tick}: ticks={ticks}")
    print(f"    moved_sq (ok/bad) = {ok}/{bad}")
    print(f"    coverage: stationary calls = {zero}   skip/rescan = {skip}/{rescan}   max moved² (hex) = {mx}")
    print()

    present = ticks > 0
    passed = present and bad == 0 and ok > 0
    print("  MOVE-THRESHOLD GATE (moved_sq bit-exact):",
          "PASS" if passed else "FAIL" if present else "n/a (no calls)")
    print("  VERDICT:", "PASS" if passed else "FAIL")
    return 0 if passed else 1


if __name__ == "__main__":
    sys.exit(main())
