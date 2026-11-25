// Hash check: MurmurHash3's 32-bit finalizer never maps two different inputs to the same
// output (it is meant to be a bijection), here on inputs that differ only in the low B bits
// (-DB=..., default 32).
#include <stdint.h>
#ifndef B
#define B 32
#endif
uint32_t nondet_u32(void);

static uint32_t fmix32(uint32_t h) {
  h ^= h >> 16;
  h *= 0x85EBCA6Bu;
  h ^= h >> 13;
  h *= 0xC2B2AE35u;
  h ^= h >> 16;
  return h;
}

void harness(void) {
  uint32_t x = nondet_u32(), y = nondet_u32();
  uint32_t low = B == 32 ? 0xFFFFFFFFu : ((1u << B) - 1u);
  __CPROVER_assume((x & ~low) == (y & ~low));
  __CPROVER_assert(x == y || fmix32(x) != fmix32(y), "no collision");
}
