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
    # vel:  tick -> (ent, lstate, vx, vy, vz)   from DTVELS (Starship) preferred, else DTVEL.
    # NOTE: DTPOS samples the first-folded ship; DTVELS may be a DIFFERENT ship, so a Starship pos
    # stream is only available when the first-folded ship IS the Starship (loco=8ae250 on DTVEL).
    pos, vel, families = {}, {}, {}
    rp  = re.compile(r"DTPOS\ttick=(\d+)\tx=(-?[\d.]+)\ty=(-?[\d.]+)\tz=(-?[\d.]+)")
    rv  = re.compile(r"DTVEL\ttick=(\d+)\tent=([0-9a-f]+)\tloco=([0-9a-f]+)\tlstate=(-?\d+)\tvx=(-?[\d.]+)\tvy=(-?[\d.]+)\tvz=(-?[\d.]+)")
    rvo = re.compile(r"DTVEL\ttick=(\d+)\tent=([0-9a-f]+)\tlstate=(-?\d+)\tvx=(-?[\d.]+)\tvy=(-?[\d.]+)\tvz=(-?[\d.]+)")  # legacy, no loco
    with open(path, encoding="utf-8", errors="replace") as f:
        for line in f:
            m = rp.match(line)
            if m:
                t = int(m.group(1)); pos[t] = tuple(float(m.group(i)) for i in (2, 3, 4)); continue
            m = rv.match(line)
            if m:
                t = int(m.group(1)); families[int(m.group(3), 16)] = families.get(int(m.group(3), 16), 0) + 1
                vel[t] = (int(m.group(2), 16), int(m.group(4)),
                          float(m.group(5)), float(m.group(6)), float(m.group(7))); continue
            m = rvo.match(line)
            if m:
                t = int(m.group(1))
                vel[t] = (int(m.group(2), 16), int(m.group(3)),
                          float(m.group(4)), float(m.group(5)), float(m.group(6)))
    return pos, vel, families

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

    moved = matched = zmatch = 0
    fails = []
    for a, b in zip(ticks, ticks[1:]):
        if b - a != 1:                       # only adjacent sim ticks
            continue
        ent_a, ent_b = vel[a][0], vel[b][0]
        if ent_a != ent_b:                   # same tracked ship across the pair
            continue
        dx = pos[b][0] - pos[a][0]
        dy = pos[b][1] - pos[a][1]
        dz = pos[b][2] - pos[a][2]
        if abs(dx) < MOVE_EPS and abs(dy) < MOVE_EPS:
            continue                          # not serviced this tick (no horizontal move)
        moved += 1
        vx, vy, vz = vel[b][2], vel[b][3], vel[b][4]
        ok_xy = abs(dx - vx) <= TOL and abs(dy - vy) <= TOL
        if ok_xy:
            matched += 1
        else:
            fails.append((b, dx, dy, vx, vy))
        if abs(dz - vz) <= TOL:
            zmatch += 1

    print(f"moved ticks: {moved}   xy-matched pos+=vel: {matched}   z also matched: {zmatch}")
    if moved:
        print(f"xy match rate: {100.0*matched/moved:.1f}%")
    for t, dx, dy, vx, vy in fails[:10]:
        print(f"  MISMATCH tick={t}  disp=({dx:.4f},{dy:.4f})  vel=({vx:.4f},{vy:.4f})")
    if moved == 0:
        print("NO MOVEMENT CAPTURED — need a battle with ships under move orders.")
        return 2
    rate = matched / moved
    print("\nORACLE PASS" if rate >= 0.95 else "\nORACLE FAIL")
    return 0 if rate >= 0.95 else 1

if __name__ == "__main__":
    sys.exit(main())
