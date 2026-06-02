#!/usr/bin/env python3
"""DTNEB in-game oracle: validate the lifted Nebula effect-ramp ticker (sim/nebula.cpp, from
FUN_140437b60) against real per-tick state captured from the live binary. Behavior #5 — the
"clean float integrator": its STAGE 1 is a semi-implicit DAMPED HARMONIC OSCILLATOR that runs
unconditionally, so the deterministic core is continuously observable and reproducible BIT-EXACT.

The winmm_proxy DTNEB trampoline emits, per nebula-entity-tick (spring-active ticks + edges + pins +
a sparse sample):
    DTNEB  tick=I ent=HEX  dt=F freq=F  vb=F eqb=F velb=F etb=I   (BEFORE: value/eq/velocity/enter_tick)
           va=F eqa=F vela=F eta=I  pv=F pvel=F  spring_ok=B pin=B  enter=B leave=B
where dt = DAT_140b0a344 (per-tick game delta), freq = sub+0x12c, vb/eqb/velb = value/equilibrium/
velocity before, etb = enter_tick before; va/eqa/vela/eta = the same AFTER the real tick; pv/pvel = the
hook's own inline spring reproduction; pin = a LINGER-pin tick (STAGE 2 clobbered value->1, vel->0,
eq->1, only when eq<1); enter/leave = membership edges. Plus DTNEBSURVEY periodic totals.

The lifted model (nebula_spring_step), reproduced here in float32 from the BEFORE state + dt:
    w=freq*2; wdt=w*dt; dx=vb-eqb; inv=1/(1+wdt+0.48*wdt^2+0.235*wdt^3); tmp=(dx*w+velb)*dt;
    value'=(tmp+dx)*inv+eqb;  velocity'=(velb-tmp*w)*inv
must equal the captured va/vela EXACTLY — except on LINGER-pin ticks (STAGE 2 overwrote value).

Validation:
  (1) SPRING (bit-exact): on every non-pin tick, the float32 reproduction == va and == vela exactly.
      [the deterministic-core PASS — the analog of DTNRG's `va` / DTABIL's `ta`]
  (2) LINGER pins: the pin invariant holds (eqb<1 & va==1 & vela==0 & eqa==1). Spring n/a (clobbered).
  (3) MEMBERSHIP edges: on an enter edge (etb==-1 -> eta!=-1) eqa==1; on a leave edge (-> -1) eqa==0.
  (4) dt is a single per-frame global (report the distinct values; the spring PASS pins it as correct).

Usage: tools/analyze_nebula_oracle.py [eaw-mt.log]   (default: ./eaw-mt.log)
"""
import sys
import re
import struct

LINE = re.compile(
    r"DTNEB\ttick=(-?\d+)\tent=([0-9a-fA-F]+)\tdt=(\S+)\tfreq=(\S+)"
    r"\tvb=(\S+)\teqb=(\S+)\tvelb=(\S+)\tetb=(-?\d+)"
    r"\tva=(\S+)\teqa=(\S+)\tvela=(\S+)\teta=(-?\d+)"
    r"\tpv=(\S+)\tpvel=(\S+)\tspring_ok=(\d+)\tpin=(\d+)\tenter=(\d+)\tleave=(\d+)")


def f32(x):
    """Round a Python float to IEEE-754 single precision (matches C `float`)."""
    return struct.unpack("f", struct.pack("f", x))[0]


def spring_step(vb, eqb, velb, freq, dt):
    """Reproduce nebula_spring_step in float32, op-order-identical to sim/nebula.cpp.
    Every binary op is coerced back to float32, mirroring SSE scalar-single arithmetic."""
    vb, eqb, velb, freq, dt = map(f32, (vb, eqb, velb, freq, dt))
    w   = f32(freq * f32(2.0))
    wdt = f32(w * dt)
    dx  = f32(vb - eqb)
    denom = f32(f32(f32(f32(wdt * f32(0.48)) * wdt) + wdt) + f32(1.0))
    denom = f32(denom + f32(f32(f32(wdt * f32(0.235)) * wdt) * wdt))
    inv = f32(f32(1.0) / denom)
    tmp = f32(f32(f32(dx * w) + velb) * dt)
    pv   = f32(f32(f32(tmp + dx) * inv) + eqb)
    pvel = f32(f32(f32(velb - f32(tmp * w)) * inv))
    return pv, pvel


def main():
    path = sys.argv[1] if len(sys.argv) > 1 else "eaw-mt.log"
    rows = []
    with open(path, "r", errors="replace") as fh:
        for line in fh:
            m = LINE.search(line)
            if m:
                g = m.groups()
                rows.append(dict(
                    tick=int(g[0]), ent=g[1], dt=float(g[2]), freq=float(g[3]),
                    vb=float(g[4]), eqb=float(g[5]), velb=float(g[6]), etb=int(g[7]),
                    va=float(g[8]), eqa=float(g[9]), vela=float(g[10]), eta=int(g[11]),
                    pv=float(g[12]), pvel=float(g[13]), spring_ok=int(g[14]),
                    pin=int(g[15]), enter=int(g[16]), leave=int(g[17])))

    if not rows:
        print(f"NO DTNEB lines found in {path}")
        print("  (need a profile build with EAW_DIFFTRACE=1, in a battle that CONTAINS A NEBULA so")
        print("   ships enter it and the effect-ramp spring becomes non-trivial)")
        return 1

    ents = {r["ent"] for r in rows}
    print(f"DTNEB oracle — {path}")
    print(f"  samples: {len(rows)}   distinct nebula entities: {len(ents)}")

    pins = [r for r in rows if r["pin"]]
    nonpin = [r for r in rows if not r["pin"]]

    # (1) SPRING bit-exact on non-pin ticks
    spring_bad = []
    for r in nonpin:
        pv, pvel = spring_step(r["vb"], r["eqb"], r["velb"], r["freq"], r["dt"])
        if not (pv == f32(r["va"]) and pvel == f32(r["vela"])):
            r["_pv"], r["_pvel"] = float(pv), float(pvel)
            spring_bad.append(r)
    print(f"  spring    : float32 reproduction == (value',velocity') on "
          f"{len(nonpin) - len(spring_bad)}/{len(nonpin)} non-pin ticks")
    for r in spring_bad[:8]:
        print(f"    BAD tick={r['tick']} ent={r['ent']} vb={r['vb']:.9g} eqb={r['eqb']:.9g} "
              f"velb={r['velb']:.9g} freq={r['freq']:.9g} dt={r['dt']:.9g} -> "
              f"got va={r['va']:.9g} vela={r['vela']:.9g}  want pv={r['_pv']:.9g} pvel={r['_pvel']:.9g}")

    # cross-check: the hook's own inline pv/pvel should agree with ours on non-pin ticks
    hook_disagree = sum(1 for r in nonpin
                        if not (f32(r["pv"]) == spring_step(r["vb"], r["eqb"], r["velb"], r["freq"], r["dt"])[0]))
    if hook_disagree:
        print(f"    note: hook inline pv disagreed with analyzer on {hook_disagree} ticks (printf rounding?)")

    # (2) LINGER pin invariant
    pin_bad = [r for r in pins if not (r["eqb"] < 1.0 and r["va"] == 1.0 and r["vela"] == 0.0 and r["eqa"] == 1.0)]
    print(f"  linger    : {len(pins)} pin ticks; invariant (eqb<1 & va=1 & vela=0 & eqa=1): "
          f"{len(pins) - len(pin_bad)}/{len(pins)}")
    for r in pin_bad[:8]:
        print(f"    BAD tick={r['tick']} eqb={r['eqb']:.9g} va={r['va']:.9g} vela={r['vela']:.9g} eqa={r['eqa']:.9g}")

    # (3) membership edges
    enters = [r for r in rows if r["enter"]]
    leaves = [r for r in rows if r["leave"]]
    enter_bad = [r for r in enters if not (r["etb"] == -1 and r["eta"] != -1 and r["eqa"] == 1.0)]
    leave_bad = [r for r in leaves if not (r["etb"] != -1 and r["eta"] == -1 and r["eqa"] == 0.0)]
    print(f"  membership: {len(enters)} enters (eqa->1 ok: {len(enters) - len(enter_bad)}/{len(enters)})   "
          f"{len(leaves)} leaves (eqa->0 ok: {len(leaves) - len(leave_bad)}/{len(leaves)})")
    for r in (enter_bad + leave_bad)[:8]:
        print(f"    BAD tick={r['tick']} etb={r['etb']} eta={r['eta']} eqa={r['eqa']:.9g} "
              f"enter={r['enter']} leave={r['leave']}")

    # (4) dt distinctness
    dts = sorted({round(r["dt"], 9) for r in rows})
    active = [r for r in rows if r["eqb"] != 0.0 or r["vb"] != 0.0]
    print(f"  dt        : {len(dts)} distinct value(s): {dts[:6]}{' ...' if len(dts) > 6 else ''}")
    print(f"  activity  : {len(active)}/{len(rows)} ticks spring-active (eq!=0 or value!=0)")

    passed = (len(nonpin) > 0 and not spring_bad and not pin_bad and not enter_bad and not leave_bad
              and len(active) > 0)
    print("  RESULT    : " + (
        "PASS — lifted Nebula spring reproduces (value',velocity') bit-exact; linger/membership characterised"
        if passed else "INCOMPLETE/FAIL — see above"))
    if len(active) == 0:
        print("    (no spring-active ticks captured — load a battle that CONTAINS A NEBULA and fly a ship in)")
    return 0 if passed else 1


if __name__ == "__main__":
    sys.exit(main())
