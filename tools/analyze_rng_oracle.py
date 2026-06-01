#!/usr/bin/env python3
"""DTRNG in-game oracle: validate the lifted global sim RNG (sim/sim_rng.cpp, from
FUN_1401ffb40) against real draws captured from the live binary.

The winmm_proxy DTRNG trampoline emits, for a sampled subset of RNG draws:
    DTRNG  tick=..  s_in=HEX  a=INT  b=INT  ret=INT  s_out=HEX
where s_in/s_out are the 32-bit LCG state word (DAT_140a13e24) before/after the call.

For each tuple we replay range_i(a, b) seeded at s_in and assert BOTH:
  - the returned int == ret             (constants + masking correct)
  - the final state  == s_out           (rejection draw COUNT correct)
A single mismatch means the lifted RNG would desync the Phase-3/4 replay.

Usage: tools/analyze_rng_oracle.py [eaw-mt.log]   (default: ./eaw-mt.log)
"""
import sys
import re

MULT = 0x41c64e6d
INC  = 0xbdf
MASK32 = 0xffffffff


def step(s):
    return (s * MULT + INC) & MASK32


def range_i(state, a, b):
    """Mirror of SimRng::range_i (sim/sim_rng.cpp). Returns (ret, final_state, draws)."""
    hi, lo = (b, a) if a <= b else (a, b)
    if lo == hi:
        return lo, state, 0
    rng = (hi - lo) & MASK32
    bit = 0x4000
    if rng < 0x7fff:
        t = rng & 0x4000
        while t == 0:
            bit >>= 1
            t = rng & bit
    else:
        rng = 0x7ffe
    mask = (bit * 2 - 1) & MASK32
    draws = 0
    while True:
        state = step(state)
        draws += 1
        r = (state >> 10) & 0x7fff & mask
        # signed compare in the binary; both are non-negative here
        if not (rng < r):
            break
    # ret is an int; lo can be negative -> emulate 32-bit wrap then interpret signed
    ret = (r + (lo & MASK32)) & MASK32
    if ret >= 0x80000000:
        ret -= 0x100000000
    return ret, state, draws


LINE = re.compile(
    r"DTRNG\ttick=(\d+)\ts_in=([0-9a-fA-F]+)\ta=(-?\d+)\tb=(-?\d+)\tret=(-?\d+)\ts_out=([0-9a-fA-F]+)")


def main():
    path = sys.argv[1] if len(sys.argv) > 1 else "eaw-mt.log"
    total = ret_fail = state_fail = 0
    fails = []
    draw_hist = {}
    a_eq_b = 0
    span_min, span_max = None, None
    with open(path, "r", errors="replace") as f:
        for line in f:
            m = LINE.search(line)
            if not m:
                continue
            total += 1
            tick = int(m.group(1))
            s_in = int(m.group(2), 16)
            a = int(m.group(3)); b = int(m.group(4))
            ret = int(m.group(5))
            s_out = int(m.group(6), 16)

            got, st, draws = range_i(s_in, a, b)
            draw_hist[draws] = draw_hist.get(draws, 0) + 1
            if a == b:
                a_eq_b += 1
            span = abs(b - a)
            span_min = span if span_min is None else min(span_min, span)
            span_max = span if span_max is None else max(span_max, span)

            ok_ret = (got == ret)
            ok_state = (st == s_out)
            if not ok_ret:
                ret_fail += 1
            if not ok_state:
                state_fail += 1
            if (not ok_ret or not ok_state) and len(fails) < 20:
                fails.append((tick, s_in, a, b, ret, got, s_out, st))

    if total == 0:
        print(f"NO DTRNG lines found in {path}")
        print("  (need a profile build with EAW_DIFFTRACE=1, in a battle that draws the RNG)")
        return 1

    print(f"DTRNG oracle — {path}")
    print(f"  samples          : {total}")
    print(f"  span (|b-a|)     : min={span_min} max={span_max}   a==b draws: {a_eq_b}")
    print(f"  draw-count hist  : " +
          ", ".join(f"{k}:{draw_hist[k]}" for k in sorted(draw_hist)))
    print(f"  ret matches      : {total - ret_fail}/{total}")
    print(f"  state matches    : {total - state_fail}/{total}")
    if fails:
        print("  first mismatches (tick s_in a b ret got s_out st):")
        for fr in fails:
            t, si, a, b, ret, got, so, st = fr
            print(f"    t={t} s_in={si:08x} a={a} b={b} ret={ret} got={got} "
                  f"s_out={so:08x} st={st:08x}")
    ok = (ret_fail == 0 and state_fail == 0)
    print("  RESULT           : " + ("PASS — lifted RNG is bit-exact" if ok else "FAIL"))
    return 0 if ok else 1


if __name__ == "__main__":
    sys.exit(main())
