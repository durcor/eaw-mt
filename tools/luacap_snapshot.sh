#!/usr/bin/env bash
# Phase 6 LUACAP galactic capture helper.
# Snapshots the distinct Lua __index method names captured so far (by mode tag) from eaw-mt.log,
# writes a sorted union to a file, and reports counts. Run it once the game is sitting in the
# galactic map with the AI pumping (a few seconds is enough — the dedup set fills fast).
#
# Usage:
#   1. just luacap=1 launch-foc-desktop &        # launches with the capture hook armed
#   2. (in the game) Load Game -> a galactic conquest save -> let the galaxy view sit ~30s
#   3. tools/luacap_snapshot.sh galactic         # snapshot + report; does NOT kill the game
#   4. just kill-game                            # when done
set -u
TAG="${1:-snap}"
ROOT="/home/ty/src/eaw-reverse-engineering"
LOG="$ROOT/eaw-mt.log"
OUT="$ROOT/luacap-$TAG.txt"
cd "$ROOT" || exit 1

if [ ! -f "$LOG" ]; then echo "no eaw-mt.log yet"; exit 1; fi

grep "LUACAP mode=" "$LOG" | sed 's/.*LUACAP //' | sort -u > "$OUT"
echo "=== LUACAP snapshot ($TAG) -> $OUT ==="
echo "distinct (mode,name): $(wc -l < "$OUT")"
echo "--- by mode ---"
grep -oE 'mode=.' "$OUT" | sort | uniq -c
echo "--- names ---"
sed -E 's/mode=. name=//' "$OUT" | sort -u
echo "--- install confirmation ---"
grep -E "LUACAP:" "$LOG" | tail -1
pgrep -x StarWarsG.exe >/dev/null && echo "[game still running — kill with: just kill-game]" || echo "[game not running]"
