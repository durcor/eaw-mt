#!/usr/bin/env bash
# B3.6.2 (§8.42) gate cell runner: launch a menu battle with the given pfire knobs, wait for the
# deterministic first-battle DTWORLD fingerprint (tick=2048), capture it, then kill the game.
# Usage: pfire_gate_run.sh <tag> <geomss 0|1> <reorder 0|1> [pool 0|1]
set -u
TAG="$1"; GEOMSS="$2"; REORDER="$3"; POOL="${4:-0}"
ROOT="/home/ty/src/eaw-reverse-engineering"
LOG="$ROOT/eaw-mt.log"
OUT="$ROOT/eaw-gate-$TAG.dtworld"
cd "$ROOT" || exit 1

just kill-game >/dev/null 2>&1; sleep 2
: > "$LOG" 2>/dev/null || true

just pfire=4 pfireshards=4 pfiregeomss="$GEOMSS" pfirereorder="$REORDER" pfirepool="$POOL" difftrace=1 \
    launch-foc-desktop > "$ROOT/eaw-launch-$TAG.out" 2>&1 &
LJOB=$!

# Poll up to 360s for the tick=2048 fingerprint (the second deterministic pin).
for i in $(seq 1 120); do
  if grep -q "DTWORLD.*tick=2048" "$LOG" 2>/dev/null; then break; fi
  sleep 3
done

# Give a beat for the line to flush, snapshot the fingerprints.
sleep 1
{ grep "DTWORLD" "$LOG" 2>/dev/null | sort -u; grep "PFLCG" "$LOG" 2>/dev/null | head -6; } > "$OUT"
echo "=== gate cell $TAG (geomss=$GEOMSS reorder=$REORDER pool=$POOL) ==="
cat "$OUT"
grep -E "PFIREGEOMSS|PFIREREORDER|PFIREPOOL|geom_ss_fires|shard_maxfill|PFIRE-DRAIN|rank_down|pool_threads" "$LOG" 2>/dev/null | tail -5
pgrep -x StarWarsG.exe >/dev/null && echo "[still running at capture]" || echo "[GAME EXITED before capture — possible crash]"

just kill-game >/dev/null 2>&1
wait "$LJOB" 2>/dev/null
