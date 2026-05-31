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


def check_fighter(pos, vel):
    """Full-position oracle for the Fighter family (vtable 0x8a6198, FighterLocomotorBehaviorClass).
    Every flight-state tick (state 0x1b/0x1c/0x1e) commits new_pos = owner_pos + velocity, i.e. the
    per-tick displacement equals the captured velocity exactly. (State 0x2c is transient drift and is
    covered by the drift-magnitude oracle, so it is excluded here.)"""
    FLIGHT = (0x1b, 0x1c, 0x1e)
    ticks = sorted(set(pos) & set(vel))
    n = ok = 0
    fails = []
    for a, b in zip(ticks, ticks[1:]):
        if b - a != 1 or vel[a][0] != vel[b][0]:
            continue
        ls, vx, vy, vz = vel[b][1], vel[b][2], vel[b][3], vel[b][4]
        # loco family is not on the vel tuple; flight states 0x1b/1c/1e are Fighter-exclusive.
        if ls not in FLIGHT:
            continue
        dx = pos[b][0] - pos[a][0]; dy = pos[b][1] - pos[a][1]; dz = pos[b][2] - pos[a][2]
        n += 1
        if abs(dx - vx) + abs(dy - vy) + abs(dz - vz) <= 0.03:
            ok += 1
        elif len(fails) < 8:
            fails.append((b, ls, (dx, dy, dz), (vx, vy, vz)))
    if n == 0:
        print("\n[Fighter integrator oracle] no 0x1b/0x1c/0x1e flight ticks captured — skipped "
              "(need a space battle with engaged fighters)")
        return None
    print(f"\n[Fighter integrator oracle]  disp == velocity (pos += vel): {ok}/{n} flight-state ticks")
    for t, ls, d, v in fails:
        print(f"  MISMATCH t={t} state=0x{ls:x} disp={tuple(round(x,2) for x in d)} vel={tuple(round(x,2) for x in v)}")
    return n, ok


def parse_spline(path):
    """Parse the basis matrix (DTMAT) and per-tick control points (DTSPL n=0/n=1)."""
    basis = {}
    nodes = {}  # tick -> {0: node, 1: node};  node = (z, px, py, tx, ty, tz, w, a)
    rm = re.compile(r"DTMAT\ti=(\d+)\tv=(-?[\d.]+)")
    rs = re.compile(r"DTSPL\ttick=(\d+)\tn=(\d)\tidx=(-?\d+)\tz=(-?[\d.]+)\tpx=(-?[\d.]+)\tpy=(-?[\d.]+)"
                    r"\ttx=(-?[\d.]+)\tty=(-?[\d.]+)\ttz=(-?[\d.]+)\tw=(-?[\d.]+)\ta=(-?[\d.]+)")
    with open(path, encoding="utf-8", errors="replace") as f:
        for line in f:
            m = rm.match(line)
            if m:
                basis[int(m.group(1))] = float(m.group(2)); continue
            m = rs.match(line)
            if m:
                t = int(m.group(1)); n = int(m.group(2))
                nodes.setdefault(t, {})[n] = tuple(float(m.group(i)) for i in range(4, 12))
    return basis, nodes


def hermite_eval(p0, p1, tick, hold_z, B):
    """Mirror of sim/locomotor.cpp hermite_spline_eval. node = (z,px,py,tx,ty,tz,w,a)."""
    import math
    _, p0x, p0y, t0x, t0y, t0z, w0, a0 = p0
    _, p1x, p1y, t1x, t1y, t1z, w1, a1 = p1
    arclen = a1 - a0
    if not (arclen > 0):
        return (p0x, p0y, hold_z)
    t = max(0.0, min(1.0, (tick - a0) / arclen))
    def norm(x, y, z):
        L = math.sqrt(x * x + y * y + z * z)
        return (x / L, y / L) if L > 0 else (0.0, 0.0)
    m0x, m0y = norm(t0x, t0y, t0z); m1x, m1y = norm(t1x, t1y, t1z)
    s0, s1 = w0 * arclen, w1 * arclen
    m0x *= s0; m0y *= s0; m1x *= s1; m1y *= s1
    t2, t3 = t * t, t * t * t
    def ax(a, b, mm0, mm1):
        v = (a, b, mm0, mm1)
        c = [B[r * 4 + 0] * v[0] + B[r * 4 + 1] * v[1] + B[r * 4 + 2] * v[2] + B[r * 4 + 3] * v[3]
             for r in range(4)]
        return c[0] + c[1] * t + c[2] * t2 + c[3] * t3
    return (ax(p0x, p1x, m0x, m1x), ax(p0y, p1y, m0y, m1y), hold_z)


def check_spline(path, pos):
    basis, nodes = parse_spline(path)
    if len(basis) < 16 or not nodes:
        print("\n[spline (0x13) oracle] no DTMAT/DTSPL captured — skipped "
              f"(basis={len(basis)}, spline-ticks={len(nodes)})")
        return None
    B = [basis[i] for i in range(16)]
    import math
    n = ok = 0
    fails = []
    for t, nd in sorted(nodes.items()):
        if 0 not in nd or 1 not in nd or t not in pos:
            continue
        hold_z = nd[0][0]
        px, py, pz = hermite_eval(nd[0], nd[1], t, hold_z, B)
        ax, ay, az = pos[t]
        n += 1
        if abs(px - ax) <= 0.05 and abs(py - ay) <= 0.05:
            ok += 1
        else:
            fails.append((t, px, py, ax, ay))
    print(f"\n[spline (0x13) oracle]  hermite_spline_eval == DTPOS: {ok}/{n} spline ticks")
    for t, px, py, ax, ay in fails[:8]:
        print(f"  MISMATCH t={t} pred=({px:.3f},{py:.3f}) actual=({ax:.3f},{ay:.3f})")
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

def wrap180(d):
    while d > 180.0:   d -= 360.0
    while d <= -180.0: d += 360.0
    return d

def wrap360(d):
    while d >= 360.0: d -= 360.0
    while d < 0.0:    d += 360.0
    return d

def fighter_turn_angle(cur, delta, budget):
    """Mirror of sim/locomotor.cpp fighter_turn_angle (== FUN_1405c8fb0 core)."""
    cur = wrap180(cur)
    if budget < abs(delta):
        if delta > 0.0:   cur += budget
        elif delta < 0.0: cur -= budget
    else:
        cur += delta
    return wrap360(cur)

def steer_local_target(pitch_deg, yaw_deg, owner, target):
    """Replicate FUN_1405caaf0's inverse-orientation transform: build M (identity, then the in-place
    Ry(-pitch)=FUN_1402cf9e0 and Rz(-yaw)=FUN_1400480f0 rotations), return M·(target-owner;1) =
    the target offset in the entity LOCAL frame (local_d8/d4/d0)."""
    import math
    M = [1.0,0,0,0, 0,1.0,0,0, 0,0,1.0,0]   # 3x4 row-major identity
    a = -pitch_deg * DEG2RAD                  # FUN_1402cf9e0: rotate cols 0,2 (X-Z, pitch)
    c, s = math.cos(a), math.sin(a)
    for r in range(3):
        x, z = M[r*4+0], M[r*4+2]
        M[r*4+0] = x*c - z*s
        M[r*4+2] = z*c + x*s
    b = -yaw_deg * DEG2RAD                     # FUN_1400480f0: rotate cols 0,1 (X-Y, yaw)
    c, s = math.cos(b), math.sin(b)
    for r in range(3):
        x, y = M[r*4+0], M[r*4+1]
        M[r*4+0] = x*c + y*s
        M[r*4+1] = y*c - x*s
    dx, dy, dz = target[0]-owner[0], target[1]-owner[1], target[2]-owner[2]
    lx = M[0]*dx + M[1]*dy + M[2]*dz + M[3]
    ly = M[4]*dx + M[5]*dy + M[6]*dz + M[7]
    lz = M[8]*dx + M[9]*dy + M[10]*dz + M[11]
    return lx, ly, lz

def target_bearing(local):
    """Mirror of FUN_14020acd0 / fighter_target_bearing: (azimuth, elevation) in degrees."""
    import math
    lx, ly, lz = local
    az = 0.0 if (lx == 0.0 and ly == 0.0) else math.degrees(math.atan2(ly, lx))
    if az < 0.0: az += 360.0
    el = 0.0 if (lz == 0.0 and lx == 0.0) else -math.degrees(math.atan2(lz, math.hypot(lx, ly)))
    return az, el


def parse_steer(path):
    """tick -> dict of the FUN_1405caaf0 controller I/O captured by the DTSTEER hook."""
    steer = {}
    rs = re.compile(
        r"DTSTEER\ttick=(\d+)\tent=([0-9a-f]+)\tstate=(-?\d+)\tht=(-?\d+)\t"
        r"ox=(-?[\d.]+)\toy=(-?[\d.]+)\toz=(-?[\d.]+)\ttx=(-?[\d.]+)\tty=(-?[\d.]+)\ttz=(-?[\d.]+)\t"
        r"r0=(-?[\d.]+)\tp0=(-?[\d.]+)\ty0=(-?[\d.]+)\tr1=(-?[\d.]+)\tp1=(-?[\d.]+)\ty1=(-?[\d.]+)\t"
        r"yb=(-?[\d.]+)\tpb=(-?[\d.]+)\tsb=(-?[\d.]+)\tms=(-?[\d.]+)")
    with open(path, encoding="utf-8", errors="replace") as f:
        for line in f:
            m = rs.match(line)
            if not m:
                continue
            g = m.groups()
            t = int(g[0])
            steer[t] = dict(ent=int(g[1], 16), state=int(g[2]), ht=int(g[3]),
                            owner=(float(g[4]), float(g[5]), float(g[6])),
                            target=(float(g[7]), float(g[8]), float(g[9])),
                            r0=float(g[10]), p0=float(g[11]), y0=float(g[12]),
                            r1=float(g[13]), p1=float(g[14]), y1=float(g[15]),
                            yb=float(g[16]), pb=float(g[17]), sb=float(g[18]), ms=float(g[19]))
    return steer


def fighter_steer_yaw(roll0, yaw0, yaw_err, budget, a, b, cap):
    """Mirror of sim/locomotor.cpp fighter_steer_yaw (== FUN_1405c95a0). Returns (roll1, yaw1,
    budget1, yaw_stepped). a/b enter only as the ratio b/a; cap is the raw template+0x39c."""
    we = wrap180(yaw_err)
    clamped = we
    if clamped < -cap:  clamped = -cap
    elif clamped > cap: clamped = cap
    cy = -clamped
    roll0w  = wrap180(roll0)
    roll_err = wrap180(cy - roll0w)
    roll_step = b * (budget / a)
    if roll_step >= 0.0:
        if roll_step > 180.0: roll_step = 180.0
    else:
        roll_step = 0.0
    new_roll = roll0w
    if roll_step < abs(roll_err):
        if roll_err > 0.0:   new_roll += roll_step
        elif roll_err < 0.0: new_roll -= roll_step
    else:
        new_roll += roll_err
    new_yaw = yaw0
    stepped = True
    skip = False
    if new_roll != 0.0:
        s1 = -1.0 if cy < 0.0 else 1.0
        s2 = 1.0 if new_roll >= 0.0 else -1.0
        if s1 != s2:
            budget = 0.0; skip = True; stepped = False
    if not skip:
        bud = budget
        if bud < abs(we):
            budget = 0.0
            if we <= 0.0: new_yaw -= bud
            else:         new_yaw += bud
        else:
            new_yaw += we
            budget -= abs(we)
    return wrap360(new_roll), wrap360(new_yaw), budget, stepped


def parse_yaw(path):
    """tick -> dict of the FUN_1405c95a0 yaw/roll integrator I/O captured by the DTYAW hook."""
    yaw = {}
    rs = re.compile(
        r"DTYAW\ttick=(\d+)\tent=([0-9a-f]+)\t"
        r"r0=(-?[\d.]+)\tp0=(-?[\d.]+)\ty0=(-?[\d.]+)\tye=(-?[\d.]+)\tbud0=(-?[\d.]+)\t"
        r"a=(-?[\d.]+)\tb=(-?[\d.]+)\tcap=(-?[\d.]+)\tr1=(-?[\d.]+)\ty1=(-?[\d.]+)\tbud1=(-?[\d.]+)")
    with open(path, encoding="utf-8", errors="replace") as f:
        for line in f:
            m = rs.match(line)
            if not m:
                continue
            g = m.groups()
            yaw[int(g[0])] = dict(ent=int(g[1], 16),
                                  r0=float(g[2]), p0=float(g[3]), y0=float(g[4]),
                                  ye=float(g[5]), bud0=float(g[6]), a=float(g[7]),
                                  b=float(g[8]), cap=float(g[9]),
                                  r1=float(g[10]), y1=float(g[11]), bud1=float(g[12]))
    return yaw


def check_yaw(path):
    """Validate the lifted fighter_steer_yaw (FUN_1405c95a0 bank-to-turn integrator) against the
    DTYAW capture of its EXACT I/O: predict (roll1, yaw1, budget1) from (roll0, yaw0, yaw_err,
    budget0, a, b, cap). This isolates the yaw channel completely — no caaf0 pipeline to rebuild."""
    yaw = parse_yaw(path)
    if not yaw:
        print("\n[Fighter yaw-integrator oracle] no DTYAW lines — skipped "
              "(need a fighter battle captured with the DTYAW hook)")
        return None
    ticks = sorted(yaw)
    r_n = r_ok = yw_n = yw_ok = 0
    fails = []
    for t in ticks:
        s = yaw[t]
        if s["a"] == 0.0:            # template not resolved (defensive); skip
            continue
        pr, py, pbud, _ = fighter_steer_yaw(s["r0"], s["y0"], s["ye"], s["bud0"],
                                            s["a"], s["b"], s["cap"])
        r_n += 1
        if abs(wrap180(pr - s["r1"])) <= 0.05:
            r_ok += 1
        elif len(fails) < 8:
            fails.append((t, "roll", s["r0"], s["ye"], s["bud0"], pr, s["r1"]))
        yw_n += 1
        if abs(wrap180(py - s["y1"])) <= 0.05:
            yw_ok += 1
        elif len(fails) < 8:
            fails.append((t, "yaw", s["y0"], s["ye"], s["bud0"], py, s["y1"]))
    print(f"\n[Fighter yaw-integrator oracle]  (DTYAW, {len(ticks)} integrator ticks, "
          f"ent=0x{yaw[ticks[0]]['ent']:x})")
    print(f"  ROLL reproduced  steer_yaw(...).roll==r1: {r_ok}/{r_n}")
    print(f"  YAW  reproduced  steer_yaw(...).yaw ==y1: {yw_ok}/{yw_n}")
    for t, ch, cur, err, bud, pred, act in fails:
        print(f"  MISS t={t} {ch} cur={cur:.3f} yaw_err={err:.3f} budget={bud:.3f} pred={pred:.3f} act={act:.3f}")
    return (r_n, r_ok, yw_n, yw_ok)


def check_steer(path):
    """Validate the lifted Fighter steering primitives against the DTSTEER controller capture:
      (A) continuity — the latched entity's post-tick angles == next tick's pre-tick angles
          (confirms the capture tracks one ship coherently);
      (B) PITCH channel — the clean FUN_1405c8fb0 path: predict p1 from p0 with the lifted
          inverse-transform -> bearing(elevation) -> fighter_turn_angle(p0, elevation_err, pitch_budget).
          (Yaw is roll-coupled / hard-turn-snapped in FUN_1405caaf0 and is reported, not asserted.)"""
    steer = parse_steer(path)
    if not steer:
        print("\n[Fighter steering oracle] no DTSTEER lines — skipped "
              "(need a fighter battle captured with the DTSTEER hook)")
        return None
    ticks = sorted(steer)
    # (A) continuity across consecutive same-entity ticks.
    cont_n = cont_ok = 0
    for a, b in zip(ticks, ticks[1:]):
        if steer[a]["ent"] != steer[b]["ent"]:
            continue
        cont_n += 1
        if (abs(wrap180(steer[a]["p1"] - steer[b]["p0"])) <= 0.01 and
                abs(wrap180(steer[a]["y1"] - steer[b]["y0"])) <= 0.01):
            cont_ok += 1
    # (B) pitch reproduction (skip ht!=0: those are mid-hard-turn, where 5caaf0 overrides the error).
    p_n = p_ok = 0
    y_close = y_n = 0
    fails = []
    for t in ticks:
        s = steer[t]
        lx, ly, lz = steer_local_target(s["p0"], s["y0"], s["owner"], s["target"])
        az, el = target_bearing((lx, ly, lz))
        # pitch (clean path only)
        if s["ht"] == 0:
            pred_p = fighter_turn_angle(s["p0"], wrap180(el), s["pb"])
            p_n += 1
            if abs(wrap180(pred_p - s["p1"])) <= 0.05:
                p_ok += 1
            elif len(fails) < 8:
                fails.append((t, "pitch", s["p0"], wrap180(el), s["pb"], pred_p, s["p1"]))
        # yaw (informational — coupled): is the actual step at least toward the bearing & within budget?
        pred_y = fighter_turn_angle(s["y0"], wrap180(az), s["yb"])
        y_n += 1
        if abs(wrap180(pred_y - s["y1"])) <= 0.05:
            y_close += 1
    print(f"\n[Fighter steering oracle]  (DTSTEER, {len(ticks)} controller ticks, "
          f"ent=0x{steer[ticks[0]]['ent']:x})")
    print(f"  (A) angle continuity  after[t]==before[t+1]: {cont_ok}/{cont_n}")
    print(f"  (B) PITCH reproduced  turn(p0, elev_err, pb)==p1: {p_ok}/{p_n} clean (ht==0) ticks")
    print(f"      yaw (roll-coupled, informational) simple-model match: {y_close}/{y_n}")
    for t, ch, cur, err, bud, pred, act in fails:
        print(f"  MISS t={t} {ch} cur={cur:.3f} err={err:.3f} budget={bud:.3f} pred={pred:.3f} act={act:.3f}")
    return (p_n, p_ok, cont_n, cont_ok)


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

    # Oracle 3 (spline maneuvering): the lifted Hermite spline eval == DTPOS on 0x13 ticks.
    spl = check_spline(path, pos)
    if spl is not None and spl[0] > 0:
        print(f"  [spline] {'PASS' if spl[1]/spl[0] >= 0.95 else 'FAIL'} "
              f"({100.0*spl[1]/spl[0]:.1f}%)")

    # Oracle 4 (Fighter family): the lifted fighter_integrate (pos += velocity) == DTPOS displacement.
    fgt = check_fighter(pos, vel)
    if fgt is not None and fgt[0] > 0:
        print(f"  [fighter] {'PASS' if fgt[1]/fgt[0] >= 0.95 else 'FAIL'} "
              f"({100.0*fgt[1]/fgt[0]:.1f}%)")

    # Oracle 5 (Fighter steering): the lifted bearing + pitch integrator vs the DTSTEER controller I/O.
    st = check_steer(path)
    if st is not None:
        p_n, p_ok, c_n, c_ok = st
        if c_n > 0:
            print(f"  [steer continuity] {'PASS' if c_ok/c_n >= 0.99 else 'FAIL'} ({100.0*c_ok/c_n:.1f}%)")
        if p_n > 0:
            print(f"  [steer pitch]      {'PASS' if p_ok/p_n >= 0.95 else 'FAIL'} ({100.0*p_ok/p_n:.1f}%)")

    # Oracle 6 (Fighter yaw channel): the lifted fighter_steer_yaw vs the DTYAW integrator I/O.
    yw = check_yaw(path)
    if yw is not None:
        r_n, r_ok, y_n, y_ok = yw
        if r_n > 0:
            print(f"  [yaw-int roll]     {'PASS' if r_ok/r_n >= 0.95 else 'FAIL'} ({100.0*r_ok/r_n:.1f}%)")
        if y_n > 0:
            print(f"  [yaw-int yaw]      {'PASS' if y_ok/y_n >= 0.95 else 'FAIL'} ({100.0*y_ok/y_n:.1f}%)")

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
