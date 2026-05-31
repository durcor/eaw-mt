#!/usr/bin/env python3
"""Recover the FUN_1405caaf0 hard-turn-snap thresholds from a DTSTEER capture.

The snap latch (state+0x1d4) in decomp/5caaf0.c lines 82-98 is hysteretic:
  - line 82   `DAT_1408007ec <= |yaw_err|`   — the RELEASE comparison (stay-engaged floor)
  - line 88   `|yaw_err| <= DAT_1408a650c`   — the ENGAGE entry gate (with state!=0x1c, gate>0)
ht[t] captured by DTSTEER is the latch BEFORE the controller runs at tick t; for consecutive
ticks of the same entity, ht[t+1] is the latch the controller WROTE at tick t. So each
consecutive pair (ht_in=ht[t], ht_out=ht[t+1], yaw_err computed at t) is one observation of
the latch transition. Bracket the two thresholds from the observed transitions."""
import sys, importlib.util, os

HERE = os.path.dirname(os.path.abspath(__file__))
spec = importlib.util.spec_from_file_location("loco", os.path.join(HERE, "analyze_loco_oracle.py"))
loco = importlib.util.module_from_spec(spec)
spec.loader.exec_module(loco)

path = sys.argv[1] if len(sys.argv) > 1 else "eaw-mt.log.dtsteer"
steer = loco.parse_steer(path)
ticks = sorted(steer)

obs = []  # (yaw_err_abs, ht_in, ht_out, state)
for t in ticks:
    if (t + 1) not in steer:
        continue
    a, b = steer[t], steer[t + 1]
    if a["ent"] != b["ent"]:
        continue
    lx, ly, lz = loco.steer_local_target(a["p0"], a["y0"], a["owner"], a["target"])
    az, _el = loco.target_bearing((lx, ly, lz))
    ye = abs(loco.wrap180(az))
    obs.append((ye, a["ht"], b["ht"], a["state"]))

print(f"observations (consecutive same-entity pairs): {len(obs)}")

# Transition classes:
#   stay-engaged: ht_in=1, ht_out=1  -> |ye| >= RELEASE held
#   release:      ht_in=1, ht_out=0  -> |ye| <  RELEASE  (line-82 false -> clear -> entry fails)
#   engage:       ht_in=0, ht_out=1  -> entry passed: |ye| > ENGAGE & state!=0x1c & gate>0
#   stay-off:     ht_in=0, ht_out=0
stay_eng = [o[0] for o in obs if o[1] == 1 and o[2] == 1]
release  = [o[0] for o in obs if o[1] == 1 and o[2] == 0]
engage   = [o[0] for o in obs if o[1] == 0 and o[2] == 1]
stay_off = [o[0] for o in obs if o[1] == 0 and o[2] == 0]

def stat(name, xs):
    if not xs:
        print(f"  {name:10s} n=0")
        return
    xs = sorted(xs)
    print(f"  {name:10s} n={len(xs):5d}  min={xs[0]:8.3f}  max={xs[-1]:8.3f}  "
          f"median={xs[len(xs)//2]:8.3f}")

print("\n|yaw_err| by transition class:")
stat("stay_eng", stay_eng)
stat("release",  release)
stat("engage",   engage)
stat("stay_off", stay_off)

# RELEASE threshold: separates stay-engaged (>=) from release (<). The boundary lies between
# max(release) and min(stay_eng).
if release and stay_eng:
    print(f"\nRELEASE (DAT_1408007ec) bracket: ({max(release):.3f}, {min(stay_eng):.3f}]")
# ENGAGE threshold: engage requires |ye| > ENGAGE; stay_off with high |ye| but no engage would
# be blocked by state==0x1c or gate<=0, so use the engage class lower edge as the ceiling.
if engage:
    off_high = [o[0] for o in obs if o[1] == 0 and o[2] == 0 and o[3] != 0x1c]
    print(f"ENGAGE  (DAT_1408a650c) bracket: engage min={min(engage):.3f}; "
          f"non-0x1c stay_off max={max(off_high) if off_high else float('nan'):.3f}")


# ---- Full latch + pitch-override model verification --------------------------
def wrap180(d): return loco.wrap180(d)
def wrap360(d): return loco.wrap360(d)

RELEASE, ENGAGE = 90.0, 170.0
def snap_latch(ht_in, ye_abs, state, gate_pos=True):
    if ye_abs >= RELEASE:
        if ht_in == 1:
            return 1
        return 1 if (state != 0x1c and ye_abs > ENGAGE and gate_pos) else 0
    return 1 if (state != 0x1c and ye_abs > ENGAGE and gate_pos) else 0

def fighter_turn_angle(cur_deg, delta_deg, budget):
    cur = wrap180(cur_deg)
    if budget < abs(delta_deg):
        if delta_deg > 0: cur += budget
        elif delta_deg < 0: cur -= budget
    else:
        cur += delta_deg
    return wrap360(cur)

latch_ok = latch_n = 0
pitch_ok = pitch_n = 0
miss = []
for t in ticks:
    if (t + 1) not in steer: continue
    a, b = steer[t], steer[t + 1]
    if a["ent"] != b["ent"]: continue
    lx, ly, lz = loco.steer_local_target(a["p0"], a["y0"], a["owner"], a["target"])
    az, el = loco.target_bearing((lx, ly, lz))
    ye = abs(wrap180(az)); pe = wrap180(el)
    pred_ht = snap_latch(a["ht"], ye, a["state"])
    latch_n += 1
    if pred_ht == b["ht"]: latch_ok += 1
    else:
        if len(miss) < 12: miss.append((t, a["ht"], b["ht"], pred_ht, round(ye,3), a["state"]))
    # pitch override fires this tick exactly when the controller took the snap path == ht_out 1
    if b["ht"] == 1:
        cmd = 180.0 if pe > 0 else -180.0
        p1pred = fighter_turn_angle(a["p0"], cmd, a["pb"])
        pitch_n += 1
        if abs(wrap180(p1pred - a["p1"])) <= 0.05: pitch_ok += 1

print(f"\n[snap latch] reproduced ht[t+1]: {latch_ok}/{latch_n}")
if miss: print("  misses:", miss)
print(f"[snap pitch override] turn(p0, sign(pe)*180, pb)==p1 on snap ticks: {pitch_ok}/{pitch_n}")
