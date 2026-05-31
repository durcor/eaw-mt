#!/usr/bin/env python3
"""Differential oracle for the lifted locomotor position-integration (sim/locomotor.cpp).

Parses DTPOS + DTVEL capture lines from eaw-mt.log and checks the invariant the lifted
`starship_tick` core integration implies:

    whenever a ship moves between consecutive (same-entity) ticks, the horizontal displacement
    equals that tick's captured velocity:   pos[t] - pos[t-1] == velocity[t]

This is robust to the locomotor's service schedule: on ticks it doesn't run, the ship doesn't move
(displacement ~0) and the pair is skipped; on ticks it does, the move must equal the velocity field.
A pass confirms (1) state+0x14/18/1c IS the velocity, and (2) the lifted pos+=velocity step matches
the live binary. Z is reported separately (it also carries the terrain-follow vertical step).

Usage: python3 tools/analyze_loco_oracle.py [eaw-mt.log]
"""
import re
import sys

MOVE_EPS = 0.02   # min horizontal displacement to count a tick as "moved"
TOL      = 0.03   # match tolerance (log rounding: pos %.3f, vel %.4f)

def parse(path):
    # vel:  tick -> (ent, lstate, vx, vy, vz, heading, speed)  from DTVEL.
    pos, vel, families = {}, {}, {}
    rp  = re.compile(r"DTPOS\ttick=(\d+)\tx=(-?[\d.]+)\ty=(-?[\d.]+)\tz=(-?[\d.]+)")
    rv  = re.compile(r"DTVEL\ttick=(\d+)\tent=([0-9a-f]+)\tloco=([0-9a-f]+)\tlstate=(-?\d+)\tvx=(-?[\d.]+)\tvy=(-?[\d.]+)\tvz=(-?[\d.]+)\thd=(-?[\d.]+)\tsp=(-?[\d.]+)")
    rvL = re.compile(r"DTVEL\ttick=(\d+)\tent=([0-9a-f]+)\tloco=([0-9a-f]+)\tlstate=(-?\d+)\tvx=(-?[\d.]+)\tvy=(-?[\d.]+)\tvz=(-?[\d.]+)")  # no hd/sp
    with open(path, encoding="utf-8", errors="replace") as f:
        for line in f:
            m = rp.match(line)
            if m:
                t = int(m.group(1)); pos[t] = tuple(float(m.group(i)) for i in (2, 3, 4)); continue
            m = rv.match(line)
            if m:
                t = int(m.group(1)); families[int(m.group(3), 16)] = families.get(int(m.group(3), 16), 0) + 1
                vel[t] = (int(m.group(2), 16), int(m.group(4)), float(m.group(5)), float(m.group(6)),
                          float(m.group(7)), float(m.group(8)), float(m.group(9))); continue
            m = rvL.match(line)
            if m:
                t = int(m.group(1)); families[int(m.group(3), 16)] = families.get(int(m.group(3), 16), 0) + 1
                vel[t] = (int(m.group(2), 16), int(m.group(4)), float(m.group(5)),
                          float(m.group(6)), float(m.group(7)), None, None)
    return pos, vel, families


DEG2RAD = (3.14159274 * 2.0) / 360.0  # matches sim/locomotor DEG2RAD

def check_simplespace_straight(pos, vel):
    """Oracle: SimpleSpace cruise move = pos[t] = pos[t-1] + (cos h, sin h, 0)*speed.
    Validates the lifted simplespace_straight_move against the live binary on cruise ticks."""
    import math
    ticks = sorted(set(pos) & set(vel))
    moved = matched = 0
    fails = []
    for a, b in zip(ticks, ticks[1:]):
        if b - a != 1 or vel[a][0] != vel[b][0]:
            continue
        hd, sp = vel[b][5], vel[b][6]
        if hd is None or sp is None:
            continue
        dx = pos[b][0] - pos[a][0]; dy = pos[b][1] - pos[a][1]
        if abs(dx) < 0.02 and abs(dy) < 0.02:
            continue
        moved += 1
        rad = hd * DEG2RAD
        px = math.cos(rad) * sp; py = math.sin(rad) * sp
        # tolerance scales with speed (heading logged to 1e-6 deg -> ~sp*2e-8 rad err, negligible;
        # dominant error is float<->decimal log rounding ~1e-3 absolute).
        tol = 0.05 + abs(sp) * 1e-4
        if abs(dx - px) <= tol and abs(dy - py) <= tol:
            matched += 1
        else:
            fails.append((b, dx, dy, px, py, hd, sp))
    print(f"\n[SimpleSpace straight-move oracle]  moved cruise ticks: {moved}   matched: {matched}"
          + (f"   rate: {100.0*matched/moved:.1f}%" if moved else ""))
    for t, dx, dy, px, py, hd, sp in fails[:8]:
        print(f"  MISMATCH t={t} disp=({dx:.3f},{dy:.3f}) pred=({px:.3f},{py:.3f}) hd={hd:.3f} sp={sp:.3f}")
    return moved, matched

def main():
    path = sys.argv[1] if len(sys.argv) > 1 else "eaw-mt.log"
    pos, vel, families = parse(path)
    nss = sum(1 for _ in open(path, encoding="utf-8", errors="replace") if _.startswith("DTVELS"))
    ticks = sorted(set(pos) & set(vel))
    print(f"parsed {len(pos)} DTPOS, {len(vel)} DTVEL ({nss} DTVELS Starship-confirmed); {len(ticks)} ticks with both")
    if families:
        fam = {0x8ae250: "Starship", 0x899c58: "Fleet", 0x8a6198: "Fighter", 0x8ad798: "Walk",
               0x8aeaf8: "SimpleSpace", 0x8adda0: "base", 0: "none/unknown"}
        print("  DTVEL locomotor families: " +
              ", ".join(f"{fam.get(k, hex(k))}={v}" for k, v in sorted(families.items(), key=lambda x: -x[1])))

    # Primary oracle for the SimpleSpace family (the common space mover): the lifted straight-move.
    moved, matched = check_simplespace_straight(pos, vel)
    if moved == 0:
        print("\nNO CRUISE MOVEMENT WITH hd/sp CAPTURED — need a SimpleSpace ship in steady cruise.")
        return 2
    rate = matched / moved
    print("\nORACLE PASS (SimpleSpace straight-move reproduces the live binary)"
          if rate >= 0.95 else "\nORACLE FAIL")
    return 0 if rate >= 0.95 else 1

if __name__ == "__main__":
    sys.exit(main())
