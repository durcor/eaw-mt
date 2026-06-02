#!/usr/bin/env python3
"""DTAST in-game oracle: validate the lifted AsteroidFieldDamage probabilistic damage trigger
(sim/asteroid_field_damage.cpp, from FUN_140437310) against real per-tick state captured from the
live binary. Behavior #6 — the SIBLING of Nebula #5 (adjacent RVA, same entity+0xf0 sub-object).

Unlike Nebula's spring (a continuous float reproduction), this behavior's deterministic core is the
sim-RNG draw sequence plus the proximity stamp at sub+0x10c. The winmm_proxy DTAST trampoline snapshots
the shared sim-RNG seed (DAT_140a13e24) and sub+0x10c immediately BEFORE and AFTER the real per-tick
call, emitting per asteroid-field-entity-tick (advanced / stamped / edge / sparse):
    DTAST  tick=I ent=HEX  seedb=HEX seeda=HEX  k=U  prob=F roll0=F apply0=B
           tickb=I ticka=I now=I  adv=B stampnow=B bicond=B lcg=B
where seedb/seeda = the global sim-RNG seed before/after the tick (the only draws between the snapshots
are THIS behavior's), k = the hook's forward-step LCG draw count, prob = DAT_140b16d64 (per-tick hit
probability), roll0 = the hook's reproduction of the first gate roll, tickb/ticka = sub+0x10c before/
after, now = the sim tick counter. Plus DTASTSURVEY periodic totals.

This analyzer independently reproduces the lifted model from the raw seeds and validates:
  (1) PROXIMITY BICONDITIONAL — every "stamp now" is preceded by >=1 sim-RNG roll and vice-versa, so
      (seed advanced) <=> (ticka == now). [the proximity-SM <-> RNG-consumption coupling PASS]
  (2) LCG RECURRENCE + DRAW COUNT — forward-step s'=s*0x41c64e6d+0xbdf from seedb; the step count to
      reach seeda must exist within the bound and equal the hook's k. The k distribution is the
      per-tick draw count: membership-dependent (varies with #asteroids hit) — the determinism-contract
      quantity (sim_rng.h). [the recurrence PASS, against the live seed transitions]
  (3) FIRST ROLL — reproduce range_f(seedb,0,1) in float32 and confirm it equals the hook's roll0
      bit-exact, and that it lies in [0,1). The apply DECISION (roll<=prob) is env (436920's self-damage
      + GOM spawn) and not externally observable, so we validate the roll VALUE + the k count, not the
      per-asteroid apply count.

Usage: tools/analyze_asteroid_oracle.py [eaw-mt.log]   (default: ./eaw-mt.log)
"""
import sys
import re
import struct

LINE = re.compile(
    r"DTAST\ttick=(-?\d+)\tent=([0-9a-fA-F]+)\tseedb=([0-9a-fA-F]+)\tseeda=([0-9a-fA-F]+)\tk=(\d+)"
    r"\tprob=(\S+)\troll0=(\S+)\tapply0=(\d+)\ttickb=(-?\d+)\tticka=(-?\d+)\tnow=(-?\d+)"
    r"\tadv=(\d+)\tstampnow=(\d+)\tbicond=(\d+)\tlcg=(\d+)")
SURVEY = re.compile(
    r"DTASTSURVEY\ttick=(-?\d+)\tticks=(\d+)\tactive=(\d+)\tclear=(\d+)\tbicond_ok=(\d+)"
    r"\tbicond_bad=(\d+)\tlcg_ok=(\d+)\tlcg_bad=(\d+)\tdraw_min=(\d+)\tdraw_max=(\d+)")


def f32(x):
    """Coerce a Python float to float32 (the binary computes in 32-bit)."""
    return struct.unpack("f", struct.pack("f", x))[0]


def lcg_step(s):
    return (s * 0x41C64E6D + 0xBDF) & 0xFFFFFFFF


def lcg_draw_count(seedb, seeda, bound=256):
    """Forward-step from seedb; return k in [1,bound] with step^k(seedb)==seeda, or None."""
    s = seedb
    for k in range(1, bound + 1):
        s = lcg_step(s)
        if s == seeda:
            return k
    return None


def range_f01(seedb):
    """Reproduce FUN_1401ffbb0(seed, 0, 1): one draw, (field/32767) in [0,1), float32 op order."""
    s = lcg_step(seedb)
    field = float((s >> 10) & 0x7FFF)
    return f32(f32(1.0 - 0.0) * f32(field / 32767.0) + 0.0)


def main():
    path = sys.argv[1] if len(sys.argv) > 1 else "eaw-mt.log"
    rows = []
    last_survey = None
    with open(path, "r", errors="replace") as fh:
        for line in fh:
            m = LINE.search(line)
            if m:
                rows.append(m.groups())
                continue
            s = SURVEY.search(line)
            if s:
                last_survey = s.groups()

    if not rows:
        print(f"NO DTAST LINES in {path} — was EAW_DIFFTRACE=1 set, and did a ship fly through an "
              f"asteroid field on the map?")
        return 1

    n = len(rows)
    bicond_ok = bicond_bad = 0
    adv_cnt = stamp_cnt = clear_cnt = 0
    lcg_ok = lcg_bad = 0
    k_mismatch = 0
    roll_ok = roll_bad = 0
    roll_range_bad = 0
    apply_pred = 0
    k_hist = {}
    probs = set()
    entities = set()

    for (tick, ent, seedb_s, seeda_s, k_s, prob_s, roll0_s, apply0_s,
         tickb_s, ticka_s, now_s, adv_s, stamp_s, bicond_s, lcg_s) in rows:
        seedb = int(seedb_s, 16)
        seeda = int(seeda_s, 16)
        k_hook = int(k_s)
        prob = float(prob_s)
        roll0 = float(roll0_s)
        ticka = int(ticka_s)
        now = int(now_s)
        adv = adv_s == "1"
        stampnow = stamp_s == "1"
        entities.add(ent)
        probs.add(round(prob, 9))

        # (1) proximity biconditional — recompute independently from the raw seeds + ticka/now.
        adv_calc = seeda != seedb
        stamp_calc = ticka == now
        if adv_calc == stamp_calc:
            bicond_ok += 1
        else:
            bicond_bad += 1
        if adv_calc:
            adv_cnt += 1
        if stamp_calc:
            stamp_cnt += 1
        if ticka == -1:
            clear_cnt += 1

        # (2) LCG recurrence + draw count.
        if adv_calc:
            k = lcg_draw_count(seedb, seeda)
            if k is None:
                lcg_bad += 1
            else:
                lcg_ok += 1
                k_hist[k] = k_hist.get(k, 0) + 1
                if k != k_hook:
                    k_mismatch += 1
            # (3) first roll reproduction (only meaningful when a draw happened).
            r = range_f01(seedb)
            if struct.pack("f", r) == struct.pack("f", f32(roll0)):
                roll_ok += 1
            else:
                roll_bad += 1
            # range_f(0,1) = field/32767 with field in [0,32767], so 1.0 is ATTAINABLE (field==32767):
            # the range is closed [0,1], not half-open. roll==1.0 is valid (and never applies vs prob<1).
            if not (0.0 <= r <= 1.0):
                roll_range_bad += 1
            if r <= prob:
                apply_pred += 1

    print(f"DTAST oracle — AsteroidFieldDamage probabilistic damage trigger (FUN_140437310)")
    print(f"  log: {path}")
    print(f"  asteroid-field-entity-ticks (logged): {n}   distinct entities: {len(entities)}")
    print(f"  per-tick hit probability DAT_140b16d64: {sorted(probs)}")
    print()
    print(f"  (1) PROXIMITY BICONDITIONAL  (seed advanced) <=> (ticka == now)")
    print(f"        bicond_ok={bicond_ok}/{n}   bicond_bad={bicond_bad}")
    print(f"        advanced(=stamped now)={adv_cnt}   cleared(ticka==-1)={clear_cnt}")
    print()
    print(f"  (2) LCG RECURRENCE + DRAW COUNT  (forward-step s*0x41c64e6d+0xbdf)")
    print(f"        reproduced={lcg_ok}/{adv_cnt} advanced ticks   not-reachable={lcg_bad}")
    print(f"        k vs hook k mismatches: {k_mismatch}")
    if k_hist:
        hist = "  ".join(f"k={k}:{c}" for k, c in sorted(k_hist.items()))
        print(f"        per-tick draw-count histogram (membership-dependent): {hist}")
    print()
    print(f"  (3) FIRST GATE ROLL  range_f(seedb,0,1) reproduction")
    print(f"        roll bit-exact vs hook roll0: {roll_ok}/{adv_cnt}   mismatch={roll_bad}")
    print(f"        rolls out of [0,1): {roll_range_bad}")
    print(f"        predicted applies (roll0<=prob) on asteroid 0: {apply_pred}/{adv_cnt} (env-only)")
    print()
    if last_survey:
        (st, sticks, sactive, sclear, sbo, sbb, slo, slb, dmin, dmax) = last_survey
        print(f"  DTASTSURVEY (in-hook totals, last @tick {st}): ticks={sticks} active={sactive} "
              f"clear={sclear} bicond_ok={sbo} bicond_bad={sbb} lcg_ok={slo} lcg_bad={slb} "
              f"draw_min={dmin} draw_max={dmax}")
    print()
    verdict_pass = (bicond_bad == 0 and lcg_bad == 0 and k_mismatch == 0 and
                    roll_bad == 0 and roll_range_bad == 0 and lcg_ok > 0)
    print("  VERDICT:", "PASS" if verdict_pass else "FAIL")
    return 0 if verdict_pass else 1


if __name__ == "__main__":
    sys.exit(main())
