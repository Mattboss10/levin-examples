// Refactor check: a hand-written shift-and-add multiply equals the * operator on W-bit
// numbers (W set with -DW=..., default 10). Multiplier circuits are a classic hard case.
#include <stdint.h>
#ifndef W
#define W 10
#endif
#define MASK ((1u << W) - 1u)
uint32_t nondet_u32(void);

static uint32_t mul_shift_add(uint32_t a, uint32_t b) {
  uint32_t product = 0;
  for (int i = 0; i < W; i++)
    if ((b >> i) & 1u) product += a << i;
  return product & ((1u << (2 * W)) - 1u);
}

void harness(void) {
  uint32_t a = nondet_u32() & MASK, b = nondet_u32() & MASK;
  __CPROVER_assert(mul_shift_add(a, b) == ((a * b) & ((1u << (2 * W)) - 1u)), "same product");
}
