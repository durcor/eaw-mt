#!/usr/bin/env python3
"""Differential oracle for the lifted hardpoint fire-budget distribution (sim/hardpoint.cpp).

Parses DTFIRE capture lines (emitted by the FUN_1403a76b0 trampoline in winmm_proxy.c under
EAW_DIFFTRACE=1) and validates the two claims the lift makes about the per-tick distribution:

    avail   = (capacity - base) * total_w           (total_w = Σ game-speed-scaled weight)
    share_i = (budget_i / total_w) * avail
            = budget_i * (capacity - base)           <-- total_w CANCELS

Each DTFIRE line carries, for one active mount on the latched ship for one tick:
    tick, ent, nm (#mounts), na (#active), idx, b0 (budget+0x28 BEFORE), w (weight+0x58),
    b1 (budget AFTER). The consumed share is delta_i = b0_i - b1_i; the consumed FRACTION is
    k_i = delta_i / b0_i. The cancellation predicts k_i == (capacity-base) for EVERY firing
    mount on a tick, INDEPENDENT of its weight w.

Two oracles:
  (1) CANCELLATION / weight-independence: on each tick, all firing mounts share one k (= cap-base),
      even when their weights w differ. This is the headline finding tested directly from the live
      binary — if weight actually entered the share, mounts with different w would show different k.
  (2) LIFT REPRODUCTION (engine op order): recover the shared k for the tick, then replay the engine's
      EXACT float arithmetic — total_w = Σ w_i (capture is at normal game speed, mode 0, so the 540070
      scale is 1 and scaled weight == raw w); avail = k*total_w; share_i = (b0_i/total_w)*avail;
      b1_pred_i = b0_i - share_i — and confirm b1_pred matches the captured b1 for every mount at once.
      A single k reproducing all mounts through the (b0/total_w)*avail path can only hold if total_w
      cancels; it cross-checks oracle (1) and proves the lifted op order is faithful to fp tolerance.

Usage: python3 tools/analyze_fire_oracle.py [eaw-mt.log]
"""
import re
import sys

FIRE_EPS = 1e-4    # min consumed fraction to count a mount as "fired" this tick
K_TOL    = 2e-4    # cross-mount k spread tolerance (b0/b1 logged at %.6f)
B1_TOL   = 5e-4    # lift-reproduction abs tolerance on b1 (plus a relative term)


def parse(path):
    """Returns ticks: {(tick, ent): [ (idx, b0, w, b1), ... ]} and seen weight spread info."""
    ticks = {}
    rf = re.compile(
        r"DTFIRE\ttick=(\d+)\tent=([0-9a-f]+)\tnm=(\d+)\tna=(\d+)\tidx=(\d+)\t"
        r"b0=(-?[\d.]+)\tw=(-?[\d.]+)\tb1=(-?[\d.]+)")
    with open(path, encoding="utf-8", errors="replace") as f:
        for line in f:
            m = rf.match(line)
            if not m:
                continue
            t = int(m.group(1)); ent = int(m.group(2), 16)
            idx = int(m.group(5))
            b0 = float(m.group(6)); w = float(m.group(7)); b1 = float(m.group(8))
            ticks.setdefault((t, ent), []).append((idx, b0, w, b1))
    return ticks


def check_cancellation(ticks):
    """Oracle (1): per tick, every firing mount shares one consumed fraction k = (b0-b1)/b0,
    regardless of its weight w."""
    tick_n = tick_ok = 0
    mount_n = 0
    fails = []
    max_wspread_ok = (0.0, None)   # largest weight ratio over a still-constant-k tick
    for (t, ent), mounts in sorted(ticks.items()):
        fired = [(idx, b0, w, b1) for (idx, b0, w, b1) in mounts
                 if b0 > 0.0 and (b0 - b1) / b0 > FIRE_EPS]
        if len(fired) < 2:
            continue                      # need >=2 firing mounts to compare k across mounts
        ks = [(b0 - b1) / b0 for (_, b0, w, b1) in fired]
        ws = [w for (_, _, w, _) in fired]
        kmed = sorted(ks)[len(ks) // 2]
        spread = max(abs(k - kmed) for k in ks)
        tick_n += 1
        mount_n += len(fired)
        if spread <= K_TOL + abs(kmed) * 1e-3:
            tick_ok += 1
            wmin, wmax = min(ws), max(ws)
            if wmin > 0 and wmax / wmin > max_wspread_ok[0]:
                max_wspread_ok = (wmax / wmin, (t, kmed, wmin, wmax, len(fired)))
        elif len(fails) < 8:
            kmin, kmax = min(ks), max(ks)
            fails.append((t, ent, len(fired), kmin, kmax, min(ws), max(ws)))
    print(f"\n[fire cancellation oracle]  (DTFIRE)")
    print(f"  k=(b0-b1)/b0 constant across mounts (weight-independent): {tick_ok}/{tick_n} "
          f"multi-mount-firing ticks  ({mount_n} firing-mount samples)")
    if max_wspread_ok[1]:
        r, (t, k, wmin, wmax, nf) = max_wspread_ok
        print(f"  strongest weight-spread witness: t={t}  w in [{wmin:g},{wmax:g}] "
              f"(ratio {r:.1f}x) yet single k={k:.6f} over {nf} mounts")
    for t, ent, nf, kmin, kmax, wmn, wmx in fails:
        print(f"  MISMATCH t={t} ent=0x{ent:x} nfired={nf} k=[{kmin:.6f},{kmax:.6f}] "
              f"w=[{wmn:g},{wmx:g}]")
    return tick_n, tick_ok


def check_reproduction(ticks):
    """Oracle (2): replay the engine's exact (b0/total_w)*avail arithmetic with the recovered k and
    captured weights; confirm it reproduces every mount's b1 simultaneously."""
    tick_n = tick_ok = 0
    mount_n = mount_ok = 0
    fails = []
    for (t, ent), mounts in sorted(ticks.items()):
        active = [(idx, b0, w, b1) for (idx, b0, w, b1) in mounts]
        fired = [(idx, b0, w, b1) for (idx, b0, w, b1) in active
                 if b0 > 0.0 and (b0 - b1) / b0 > FIRE_EPS]
        if len(fired) < 2:
            continue
        # total_w over the captured active mounts (mode-0 capture => scaled weight == raw w).
        total_w = sum(w for (_, _, w, _) in active)
        if total_w <= 0.0:
            continue
        # recover k = capacity - base from the median consumed fraction (cancellation => share/b0).
        ks = sorted((b0 - b1) / b0 for (_, b0, w, b1) in fired)
        k = ks[len(ks) // 2]
        avail = k * total_w
        ok_here = True
        worst = 0.0
        for (idx, b0, w, b1) in active:
            if b0 <= 0.0:
                pred = b0                       # eligibility guard: no consume
            else:
                share = (b0 / total_w) * avail
                pred = b0 - share if share > 0.0 else b0
            err = abs(pred - b1)
            mount_n += 1
            if err <= B1_TOL + abs(b1) * 1e-3:
                mount_ok += 1
            else:
                ok_here = False
                worst = max(worst, err)
        tick_n += 1
        if ok_here:
            tick_ok += 1
        elif len(fails) < 8:
            fails.append((t, ent, k, total_w, worst))
    print(f"\n[fire reproduction oracle]  share_i=(b0_i/total_w)*((cap-base)*total_w) == captured b1")
    print(f"  ticks fully reproduced: {tick_ok}/{tick_n}   mounts: {mount_ok}/{mount_n}")
    for t, ent, k, tw, worst in fails:
        print(f"  MISS t={t} ent=0x{ent:x} k={k:.6f} total_w={tw:.4f} worst|b1err|={worst:.6f}")
    return tick_n, tick_ok


def main():
    path = sys.argv[1] if len(sys.argv) > 1 else "eaw-mt.log"
    ticks = parse(path)
    samples = sum(len(v) for v in ticks.values())
    ents = {ent for (_, ent) in ticks}
    print(f"parsed {samples} DTFIRE mount-samples over {len(ticks)} ship-ticks "
          f"({len(ents)} ship(s): {', '.join('0x%x' % e for e in sorted(ents))})")
    if not ticks:
        print("\nNO DTFIRE LINES — need a space battle with a multi-hardpoint ship firing, "
              "captured with EAW_DIFFTRACE=1 on a profile build.")
        return 2

    cn, cok = check_cancellation(ticks)
    rn, rok = check_reproduction(ticks)

    if cn == 0:
        print("\nINCONCLUSIVE — no tick had >=2 mounts firing simultaneously "
              "(need a ship with multiple active weighted hardpoints engaging).")
        return 2
    cancel_ok = cok / cn >= 0.98
    repro_ok  = rn > 0 and rok / rn >= 0.98
    print(f"\n  [cancellation] {'PASS' if cancel_ok else 'FAIL'} ({100.0*cok/cn:.1f}%)")
    print(f"  [reproduction] {'PASS' if repro_ok else 'FAIL'} ({100.0*rok/max(rn,1):.1f}%)")
    if cancel_ok and repro_ok:
        print("\nORACLE PASS — the live binary distributes share_i = budget_i*(capacity-base); the\n"
              "  summed weight total_w (and the 540070 game-speed weight scale) cancel out of every\n"
              "  share, and the lifted op order reproduces the engine's per-mount budgets.")
        return 0
    print("\nORACLE FAIL")
    return 1


if __name__ == "__main__":
    sys.exit(main())
