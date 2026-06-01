#!/usr/bin/env python3
"""DTNRG in-game oracle: validate the lifted EnergyPool regen ticker (sim/energy_pool.cpp,
from FUN_14056c030) against real per-tick energy updates captured from the live binary.

The winmm_proxy DTNRG trampoline emits, once per tick for one latched entity:
    DTNRG  tick=..  ent=HEX  vb=FLOAT  va=FLOAT  rate=FLOAT  base=FLOAT
           dis=INT  drain=INT  sel=INT  mode=INT
where vb/va = pool value (pool+0xf8) before/after the tick, rate = regen per tick
(template+0xddc), base = unbuffed max (template+0xdd0), dis = pool disabled flag
(pool+0x130), drain = drain-capable flag (template+0x8b), sel = locally-selected.

The lifted model is:   va = clamp(vb + rate*dt, 0, max)      (max = base for unbuffed pools)
where dt = FUN_140398010(entity,4), the per-tick game-speed/difficulty time-scale we DON'T
read directly. We instead recover it offline (the DTDMG technique): on clean regen samples
(rate!=0, not disabled, not drained, value strictly inside (0,base) so the clamp is inactive)
    dt = (va - vb) / rate.
Validation (non-circular):
  (1) dt is a SINGLE shared scalar across ALL entities & ticks at a fixed game speed — every
      pool regens by its own rate * the same dt. A tight distribution confirms the rate*dt model.
  (2) feed the recovered GLOBAL dt back through the model and reproduce va for EVERY sample,
      including clamped ones: the clamp must independently produce va==base when vb+rate*dt>base
      (upper cap) and va>=0 (lower cap). The clamped samples are the genuinely non-circular check.
  (3) disabled pools (dis!=0) must NOT change (va==vb).
  (4) energy never exceeds base+tol (no overshoot) and never drops below 0.

Usage: tools/analyze_energy_oracle.py [eaw-mt.log]   (default: ./eaw-mt.log)
"""
import sys
import re

LINE = re.compile(
    r"DTNRG\ttick=(\d+)\tent=([0-9a-fA-F]+)\tvb=(-?[\d.]+)\tva=(-?[\d.]+)"
    r"\trate=(-?[\d.]+)\tbase=(-?[\d.]+)\tdis=(\d+)\tdrain=(\d+)\tsel=(\d+)\tmode=(-?\d+)")

# Interior regen samples (latch-bypass): va guaranteed strictly inside (0, base) -> clamp inactive,
# so dt = (va-vb)/rate is exact. These are what pin the game-speed scale.
INTLINE = re.compile(
    r"DTNRGINT\ttick=(\d+)\tent=([0-9a-fA-F]+)\tvb=(-?[\d.]+)\tva=(-?[\d.]+)"
    r"\trate=(-?[\d.]+)\tbase=(-?[\d.]+)")


def clamp(v, lo, hi):
    return lo if v < lo else (hi if v > hi else v)


def main():
    path = sys.argv[1] if len(sys.argv) > 1 else "eaw-mt.log"
    rows = []
    interior = []
    with open(path, "r", errors="replace") as f:
        for line in f:
            mi = INTLINE.search(line)
            if mi:
                interior.append(dict(
                    tick=int(mi.group(1)), ent=mi.group(2),
                    vb=float(mi.group(3)), va=float(mi.group(4)),
                    rate=float(mi.group(5)), base=float(mi.group(6))))
                continue
            m = LINE.search(line)
            if not m:
                continue
            rows.append(dict(
                tick=int(m.group(1)), ent=m.group(2),
                vb=float(m.group(3)), va=float(m.group(4)),
                rate=float(m.group(5)), base=float(m.group(6)),
                dis=int(m.group(7)), drain=int(m.group(8)),
                sel=int(m.group(9)), mode=int(m.group(10))))

    if not rows:
        print(f"NO DTNRG lines found in {path}")
        print("  (need a profile build with EAW_DIFFTRACE=1, in a battle with energy-pool units)")
        return 1

    ents = {r["ent"] for r in rows} | {r["ent"] for r in interior}
    print(f"DTNRG oracle — {path}")
    print(f"  latched samples  : {len(rows)}   interior(bypass): {len(interior)}   "
          f"distinct entities: {len(ents)}")

    # (1) recover dt from INTERIOR samples — va is guaranteed strictly inside (0, base) so the clamp
    # is inactive and dt = (va-vb)/rate is exact. (Latched samples are dominated by pools that refill
    # to the cap each interval, giving only a one-sided dt bound.) Fall back to any interior-looking
    # latched rows if the bypass stream is empty.
    EPS = 1e-3
    clean = list(interior)
    if not clean:
        clean = [r for r in rows
                 if not r["dis"] and not r["drain"] and r["rate"] > EPS
                 and EPS < r["vb"] and r["vb"] < r["base"] - EPS
                 and EPS < r["va"] and r["va"] < r["base"] - EPS
                 and abs(r["va"] - r["vb"]) > 1e-4]
    dts = [(r["va"] - r["vb"]) / r["rate"] for r in clean if r["rate"] > EPS]
    if dts:
        dt_mean = sum(dts) / len(dts)
        dt_min, dt_max = min(dts), max(dts)
        var = sum((d - dt_mean) ** 2 for d in dts) / len(dts)
        print(f"  clean regen      : {len(clean)} samples")
        print(f"  recovered dt     : mean={dt_mean:.6f} min={dt_min:.6f} max={dt_max:.6f} "
              f"stdev={var ** 0.5:.6f}")
        dt_global = dt_mean
        dt_consistent = (dt_max - dt_min) <= 5e-3 * (1.0 + abs(dt_mean))
    else:
        print("  clean regen      : 0 samples (no unclamped regen ticks captured)")
        dt_global = None
        dt_consistent = False

    # (3) disabled must not change
    dis_rows = [r for r in rows if r["dis"]]
    dis_held = sum(1 for r in dis_rows if abs(r["va"] - r["vb"]) <= 1e-4)
    if dis_rows:
        print(f"  disabled no-op   : {dis_held}/{len(dis_rows)} (va==vb)")

    # (4) bounds: never < 0, never > base (+tol for possible buffs)
    below0 = sum(1 for r in rows if r["va"] < -1e-3)
    overbase = sum(1 for r in rows if r["va"] > r["base"] + 1e-2)
    print(f"  out-of-bounds    : va<0: {below0}   va>base(+buff?): {overbase}")

    # (2) reproduce va for every non-drain sample using the GLOBAL recovered dt
    repro_ok = repro_tot = clamp_hi = clamp_lo = 0
    fails = []
    if dt_global is not None:
        for r in rows:
            if r["drain"]:
                continue                      # drain disposition not observable from fields
            repro_tot += 1
            if r["dis"]:
                model = r["vb"]               # disabled -> no write
            else:
                raw = r["vb"] + r["rate"] * dt_global
                model = clamp(raw, 0.0, r["base"])
                if raw > r["base"]:
                    clamp_hi += 1
                elif raw < 0.0:
                    clamp_lo += 1
            tol = 1e-3 * (1.0 + abs(r["va"]))
            if abs(model - r["va"]) <= tol:
                repro_ok += 1
            elif len(fails) < 15:
                fails.append((r["tick"], r["ent"], r["vb"], r["va"], model,
                              r["rate"], r["base"]))
        print(f"  model reproduces : {repro_ok}/{repro_tot} "
              f"(upper-clamp ticks: {clamp_hi}, lower-clamp ticks: {clamp_lo})")
    if fails:
        print("  first mismatches (tick ent vb va model rate base):")
        for t, e, vb, va, mo, ra, ba in fails:
            print(f"    t={t} ent={e} vb={vb:.4f} va={va:.4f} model={mo:.4f} "
                  f"rate={ra:.4f} base={ba:.4f}")

    ok = (dt_global is not None and dt_consistent and repro_tot > 0
          and repro_ok == repro_tot and below0 == 0
          and (not dis_rows or dis_held == len(dis_rows)))
    print("  RESULT           : " + (
        "PASS — lifted EnergyPool regen reproduces the live binary"
        if ok else "INCOMPLETE/FAIL — see above"))
    return 0 if ok else 1


if __name__ == "__main__":
    sys.exit(main())
