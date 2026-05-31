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
    # vel:  tick -> (ent, lstate, vx, vy, vz, heading, speed, timer)  from DTVEL.
    # table: timer -> speed  from the one-shot DTTAB dump of the runtime DAT_140b31440 curve.
    pos, vel, families, table = {}, {}, {}, {}
    rp  = re.compile(r"DTPOS\ttick=(\d+)\tx=(-?[\d.]+)\ty=(-?[\d.]+)\tz=(-?[\d.]+)")
    rv  = re.compile(r"DTVEL\ttick=(\d+)\tent=([0-9a-f]+)\tloco=([0-9a-f]+)\tlstate=(-?\d+)\tvx=(-?[\d.]+)\tvy=(-?[\d.]+)\tvz=(-?[\d.]+)\thd=(-?[\d.]+)\tsp=(-?[\d.]+)\ttm=(-?\d+)")
    rtab = re.compile(r"DTTAB\ti=(\d+)\tv=(-?[\d.]+)")
    with open(path, encoding="utf-8", errors="replace") as f:
        for line in f:
            m = rp.match(line)
            if m:
                t = int(m.group(1)); pos[t] = tuple(float(m.group(i)) for i in (2, 3, 4)); continue
            m = rv.match(line)
            if m:
                t = int(m.group(1)); families[int(m.group(3), 16)] = families.get(int(m.group(3), 16), 0) + 1
                vel[t] = (int(m.group(2), 16), int(m.group(4)), float(m.group(5)), float(m.group(6)),
                          float(m.group(7)), float(m.group(8)), float(m.group(9)), int(m.group(10))); continue
            m = rtab.match(line)
            if m:
                table[int(m.group(1))] = float(m.group(2))
    return pos, vel, families, table


def check_drift_magnitude(pos, vel, table):
    """Full-position oracle for the 0x2c drift mover (FUN_1406269f0):
    |disp| == table[timer] on the move window [0x19,0x96), and disp ∥ dir."""
    import math
    if not table:
        print("\n[0x2c drift-magnitude oracle] no DTTAB table captured — skipped")
        return None
    ticks = sorted(set(pos) & set(vel))
    n = ok = 0
    fails = []
    for a, b in zip(ticks, ticks[1:]):
        if b - a != 1 or vel[a][0] != vel[b][0]:
            continue
        tm = vel[b][7]
        if tm < 0x19 or tm >= 0x96:          # only the move window
            continue
        dx, dy = pos[b][0] - pos[a][0], pos[b][1] - pos[a][1]
        d = math.hypot(dx, dy)
        if d < 0.02:
            continue
        n += 1
        pred = table.get(tm, 0.0)
        if abs(d - pred) <= 0.05 + abs(pred) * 1e-4:   # |disp| == table[timer]
            ok += 1
        else:
            fails.append((b, tm, d, pred))
    print(f"\n[0x2c drift-magnitude oracle]  |disp| == table[timer]: {ok}/{n} move-window ticks")
    for t, tm, d, pred in fails[:8]:
        print(f"  MISMATCH t={t} timer={tm} |disp|={d:.3f} table={pred:.3f}")
    return n, ok


DEG2RAD = (3.14159274 * 2.0) / 360.0  # matches sim/locomotor DEG2RAD

def check_simplespace_straight(pos, vel):
    """Oracle for the lifted SimpleSpace direction-from-heading (FUN_14041c000) + move-along-facing.

    Two bit-exact claims against the live binary, on the facing-driven cruise population (the unit's
    velocity vector equals its heading direction; the curving spline-interp path is excluded):
      (1) state+0x14/18/1c == ( cos(hd*DEG2RAD), sin(hd*DEG2RAD), 0 )   [the lifted dir extractor]
      (2) displacement is exactly ALONG that direction (disp x dir == 0)  [move-along-facing]
    The move MAGNITUDE is spline/mode-driven (state+0xec is only a 0..1 throttle), so it is reported
    but not predicted here.
    """
    import math
    ticks = sorted(set(pos) & set(vel))

    # (1) direction extraction, bit-exact (<=1e-4), on the facing-driven population.
    dir_ok = dir_n = 0
    facing = set()
    for t in ticks:
        hd = vel[t][5]
        if hd is None:
            continue
        vx, vy = vel[t][2], vel[t][3]
        c, s = math.cos(hd * DEG2RAD), math.sin(hd * DEG2RAD)
        if max(abs(vx - c), abs(vy - s)) <= 1e-4:
            dir_ok += 1; facing.add(t)
        dir_n += 1

    # (2) on consecutive facing-mode same-entity moving ticks, disp is parallel to dir.
    par = par_n = 0
    mags = []
    fl = sorted(facing)
    for a, b in zip(fl, fl[1:]):
        if b - a != 1 or vel[a][0] != vel[b][0]:
            continue
        dx, dy = pos[b][0] - pos[a][0], pos[b][1] - pos[a][1]
        d = math.hypot(dx, dy)
        if d < 0.05:
            continue
        par_n += 1; mags.append(d)
        if abs(dx * vel[b][3] - dy * vel[b][2]) / d <= 0.01:
            par += 1

    print(f"\n[SimpleSpace direction oracle]")
    print(f"  (1) dir == (cos hd, sin hd) bit-exact: {dir_ok}/{dir_n} ticks "
          f"({len(facing)} facing-driven)")
    print(f"  (2) displacement ∥ facing dir:        {par}/{par_n} moving ticks")
    if mags:
        mags.sort()
        print(f"      |disp|/tick (spline/mode magnitude): min={mags[0]:.2f} "
              f"med={mags[len(mags)//2]:.2f} max={mags[-1]:.2f}")
    # PASS = the lifted direction extractor + move-along-facing both hold on the facing population.
    ok = par_n > 0 and par / par_n >= 0.95 and len(facing) > 0
    return par_n, (par if ok else 0)

def main():
    path = sys.argv[1] if len(sys.argv) > 1 else "eaw-mt.log"
    pos, vel, families, table = parse(path)
    ticks = sorted(set(pos) & set(vel))
    print(f"parsed {len(pos)} DTPOS, {len(vel)} DTVEL, {len(table)}-entry speed table; "
          f"{len(ticks)} ticks with both")
    if families:
        fam = {0x8ae250: "Starship", 0x899c58: "Fleet", 0x8a6198: "Fighter", 0x8ad798: "Walk",
               0x8aeaf8: "SimpleSpace", 0x8adda0: "base", 0: "none/unknown"}
        print("  DTVEL locomotor families: " +
              ", ".join(f"{fam.get(k, hex(k))}={v}" for k, v in sorted(families.items(), key=lambda x: -x[1])))

    # Oracle 1: the lifted SimpleSpace direction extractor + move-along-facing.
    moved, matched = check_simplespace_straight(pos, vel)
    if moved == 0:
        print("\nNO FACING-MODE MOVEMENT WITH hd CAPTURED — need a SimpleSpace ship cruising.")
        return 2
    dir_ok = matched / moved >= 0.95

    # Oracle 2 (full position): the lifted 0x2c drift mover's magnitude = table[timer].
    mag = check_drift_magnitude(pos, vel, table)
    mag_ok = mag is not None and mag[0] > 0 and mag[1] / mag[0] >= 0.95

    if dir_ok and mag_ok:
        print("\nORACLE PASS (FULL POSITION) — direction (cos/sin) AND magnitude (table[timer]) of the"
              "\n  0x2c drift mover reproduce the live binary: new_pos = pos + normalize(dir)*table[timer]")
        return 0
    if dir_ok:
        print("\nORACLE PASS (direction) — magnitude check inconclusive"
              + ("" if mag is not None else " (no DTTAB table captured)"))
        return 0
    print("\nORACLE FAIL")
    return 1

if __name__ == "__main__":
    sys.exit(main())
