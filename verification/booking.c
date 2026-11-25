// Booking check: after any OPS bookings and cancellations of room ranges, no room slot is
// booked twice (-DOPS=..., default 6; -DBUG uses an off-by-one range check). The sequence of
// requests is chosen by the solver, as any user could send it. Every slot is visited by a
// fixed loop, as booking code over a small calendar would, so no array index is symbolic.
#include <stdint.h>
#ifndef OPS
#define OPS 6
#endif
#define ROOMS 3
#define SLOTS 8
unsigned nondet_unsigned(void);
_Bool nondet_bool(void);

static uint8_t holder[ROOMS][SLOTS]; // 0 = free, else booking id
static uint8_t count[ROOMS][SLOTS];  // bookings covering each slot

static int in_range(unsigned s, unsigned start, unsigned len) {
  return s >= start && s < start + len;
}

static void book(uint8_t id, unsigned room, unsigned start, unsigned len) {
  int free = 1;
  for (unsigned r = 0; r < ROOMS; r++)
    for (unsigned s = 0; s < SLOTS; s++)
#ifdef BUG
      if (r == room && in_range(s, start, len - 1) && holder[r][s] != 0) free = 0;  // off by one
#else
      if (r == room && in_range(s, start, len) && holder[r][s] != 0) free = 0;
#endif
  if (!free) return;
  for (unsigned r = 0; r < ROOMS; r++)
    for (unsigned s = 0; s < SLOTS; s++)
      if (r == room && in_range(s, start, len)) {
        holder[r][s] = id;
        count[r][s]++;
      }
}

static void cancel(uint8_t id, unsigned room) {
  for (unsigned r = 0; r < ROOMS; r++)
    for (unsigned s = 0; s < SLOTS; s++)
      if (r == room && holder[r][s] == id) {
        holder[r][s] = 0;
        count[r][s]--;
      }
}

void harness(void) {
  for (uint8_t op = 1; op <= OPS; op++) {
    unsigned room = nondet_unsigned(), start = nondet_unsigned(), len = nondet_unsigned();
    __CPROVER_assume(room < ROOMS && start < SLOTS && len >= 1 && len <= 3);
    __CPROVER_assume(start + len <= SLOTS);
    if (nondet_bool()) {
      book(op, room, start, len);
    } else {
      unsigned which = nondet_unsigned();
      __CPROVER_assume(which >= 1 && which < op);
      cancel((uint8_t)which, room);
    }
  }
  for (unsigned r = 0; r < ROOMS; r++)
    for (unsigned s = 0; s < SLOTS; s++)
      __CPROVER_assert(count[r][s] <= 1, "no double booking");
}
