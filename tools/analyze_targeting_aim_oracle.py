#!/usr/bin/env python3
"""DTARG2 in-game oracle: validate the lifted Targeting team-targeting AIM GEOMETRY core
(sim/targeting_aim.cpp, from FUN_14020acd0) against the live binary. Behavior #7 (part 2).

Targeting (#7) is the AI fire-control orchestrator. Part 1 lifted the thin DISPATCHER (0x633a30); the
two ~2-3 KB sub-bodies (FUN_140634810 mode 1, FUN_140633ae0 mode 2) are target-selection / fire-control
orchestration FSMs — the deferred Phase-B emit seam. The ONE embedded, pure, bit-matchable numeric core
in the dominant in-combat path (633ae0; DTARG showed 100% controller-sourced mode 2) is the AIM GEOMETRY
FUN_14020acd0: direction(self->tgt) -> Euler {roll=0, pitch, yaw} (deg):
    delta = tgt - self
    yaw   = (dx==0 && dy==0) ? 0 : normalize360( atan2(dy, dx) * rad2deg )      rad2deg = 360/(PI*2.0)
    pitch = (dz==0 && dx==0) ? 0 : -( atan2(dz, sqrt(dy*dy + dx*dx)) * rad2deg )
    roll  = 0

FUN_14020acd0 has its own stable entry point and is a pure leaf-ish function (reads only its 3 pointer
args + .rdata constants, calls engine atan2/sqrt), so the winmm_proxy DTARG2 hook oracles it directly:
snapshot self.xyz + tgt.xyz BEFORE the trampoline, run the real function once, then recompute the
prediction in-process calling the engine's OWN atan2 (FUN_14078437c) + sqrt (FUN_140776d48) with PI/
2.0/360.0 read from .rdata — so rad2deg's float32 division is the binary's runtime op — and compare the
output {roll,pitch,yaw} BIT-FOR-BIT (memcmp). It emits per-call (on mismatch / first-40 / periodic):
    DTARG2 tick=I path=I yaw(a/p)=F/F pitch(a/p)=F/F roll(a/p)=F/F yaw_ok=B pitch_ok=B roll_ok=B
and a periodic rollup:
    DTARG2SURVEY tick=I ticks=U yaw(ok/bad)=U/U pitch(ok/bad)=U/U roll(ok/bad)=U/U
                 path(deg/pitch/yaw/full)=U/U/U/U
  path = (yaw_computed<<1)|pitch_computed : 0=both degenerate, 1=pitch-only, 2=yaw-only, 3=both computed.

This analyzer aggregates and validates: yaw, pitch, and roll are each reproduced bit-for-bit on every
call (yaw_bad == pitch_bad == roll_bad == 0), with at least one call exercised. It also reports the
branch distribution (which degenerate paths were hit) as coverage intel.
"""
import re
import sys

SURVEY = re.compile(
    r"DTARG2SURVEY\ttick=(-?\d+)\tticks=(\d+)\tyaw\(ok/bad\)=(\d+)/(\d+)\t"
    r"pitch\(ok/bad\)=(\d+)/(\d+)\troll\(ok/bad\)=(\d+)/(\d+)\t"
    r"path\(deg/pitch/yaw/full\)=(\d+)/(\d+)/(\d+)/(\d+)")
LINE = re.compile(
    r"DTARG2\ttick=(-?\d+)\tpath=(\d+)\tyaw\(a/p\)=(\S+)/(\S+)\tpitch\(a/p\)=(\S+)/(\S+)\t"
    r"roll\(a/p\)=(\S+)/(\S+)\tyaw_ok=(\d+)\tpitch_ok=(\d+)\troll_ok=(\d+)")


def main():
    path = sys.argv[1] if len(sys.argv) > 1 else "eaw-mt.log"
    try:
        text = open(path, encoding="utf-8", errors="replace").read()
    except FileNotFoundError:
        print(f"no log at {path}")
        return 2

    surveys = [m.groups() for m in SURVEY.finditer(text)]
    lines = [m.groups() for m in LINE.finditer(text)]

    print(f"DTARG2 — Targeting aim-geometry oracle (FUN_14020acd0)   [{path}]")
    print(f"  DTARG2 lines: {len(lines)}   DTARG2SURVEY rollups: {len(surveys)}")

    # Per-line mismatches (the authoritative bit-exact check is computed in-process; we re-tally here).
    line_yaw_bad = sum(1 for r in lines if r[8] == "0")
    line_pitch_bad = sum(1 for r in lines if r[9] == "0")
    line_roll_bad = sum(1 for r in lines if r[10] == "0")
    if lines:
        print(f"  emitted-line mismatches: yaw={line_yaw_bad} pitch={line_pitch_bad} roll={line_roll_bad}")
        for r in lines[:6]:
            print(f"    tick={r[0]} path={r[1]} yaw={r[2]}/{r[3]} pitch={r[4]}/{r[5]} "
                  f"roll={r[6]}/{r[7]} ok(y/p/r)={r[8]}/{r[9]}/{r[10]}")

    if not surveys:
        print("  no DTARG2SURVEY rollups — aim-geometry path not exercised (no targeting fire-control "
              "this capture?)")
        return 1

    # Last survey holds the cumulative totals.
    (tick, ticks, yok, ybad, pok, pbad, rok, rbad, p0, p1, p2, p3) = (int(x) for x in surveys[-1])
    print(f"  final survey @tick={tick}: ticks={ticks}")
    print(f"    yaw  (ok/bad) = {yok}/{ybad}")
    print(f"    pitch(ok/bad) = {pok}/{pbad}")
    print(f"    roll (ok/bad) = {rok}/{rbad}")
    print(f"    branch dist path(both-deg/pitch-only/yaw-only/both)={p0}/{p1}/{p2}/{p3}")
    print()

    present = ticks > 0
    passed = present and ybad == 0 and pbad == 0 and rbad == 0 and yok > 0 and rok > 0
    print("  AIM GEOMETRY (yaw/pitch/roll bit-exact):",
          "PASS" if passed else "FAIL" if present else "n/a (no aim ticks)")
    print("  VERDICT:", "PASS" if passed else "FAIL")
    return 0 if passed else 1


if __name__ == "__main__":
    sys.exit(main())
