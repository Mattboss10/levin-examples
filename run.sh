#!/bin/sh
# Try a CBMC verification check locally with a 5-second limit; if it is too slow, answer it
# with Levin (which splits the formula across many machines and checks every piece's proof).
#
#   ./run.sh verification/pricing.c -DW=8
#
# Only the numbered SAT formula your checker produces is sent to Levin: no source code,
# file names or variable names. You need CBMC installed and on your PATH.
f="$1"
[ -n "$f" ] || { echo "usage: ./run.sh verification/<file>.c [-D...]"; exit 2; }
shift
BOUNDS="--function harness --unwind 34 --unwinding-assertions"
echo "1) Trying locally with a 5-second limit..."
if timeout 5 cbmc "$f" "$@" $BOUNDS; then
  echo
  echo "Finished locally in under 5 seconds. Levin was not needed."
  exit 0
fi
echo
echo "2) Too slow locally. Sending only the numbered formula to Levin (this can take up to"
echo "   a few minutes)..."
cbmc "$f" "$@" $BOUNDS --external-sat-solver ./levin-sat
