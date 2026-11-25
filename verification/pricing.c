// Refactor check from a billing system: price * (quantity + extra) rewritten as
// price * quantity + price * extra, in W-bit arithmetic that wraps around (-DW=..., default
// 12). Distributivity over multiplication is a classic hard case for solvers.
#include <stdint.h>
#ifndef W
#define W 12
#endif
#define MASK ((1u << W) - 1u)
uint32_t nondet_u32(void);

void harness(void) {
  uint32_t price = nondet_u32() & MASK, quantity = nondet_u32() & MASK;
  uint32_t extra = nondet_u32() & MASK;
  uint32_t before = (price * ((quantity + extra) & MASK)) & MASK;
  uint32_t after = ((price * quantity) & MASK) + ((price * extra) & MASK);
  __CPROVER_assert(before == (after & MASK), "same total");
}
