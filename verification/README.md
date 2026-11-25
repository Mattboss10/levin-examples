# The checks

Each file has a `harness` function that states a property to prove, and is run with
`--function harness --unwind 34 --unwinding-assertions`. All four are **true** (no bug);
CBMC turns each into a SAT formula that is too large to solve quickly on one machine, and
Levin returns a certified "no bug" (every piece's proof checked by a formally verified
checker).

| File | What it proves | Command |
|---|---|---|
| `pricing.c` | A billing refactor is exact: `p*(q+e) == p*q + p*e`, 8-bit | `./run.sh verification/pricing.c -DW=8` |
| `mul_loop.c` | A hand-written multiply equals `*`, 10-bit | `./run.sh verification/mul_loop.c -DW=10` |
| `fmix.c` | A hash finalizer has no collisions, 16 bits | `./run.sh verification/fmix.c -DB=16` |
| `booking.c` | No room is ever double-booked, 10 requests | `./run.sh verification/booking.c -DOPS=10` |

These are small, self-contained C files adapted from Levin's own benchmark
(`bench/cbmc/`). They contain no real product code.
