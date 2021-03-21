#include <stdint.h>

#include "ss_math.h"

// If num is a power of two, returns num. Otherwise returns the next-highest
// power of two.
uint64_t next_pow_of_two(uint64_t num) {
    uint64_t n = num > 0 ? num - 1 : 0;

    n |= n >> 1;
    n |= n >> 2;
    n |= n >> 4;
    n |= n >> 8;
    n |= n >> 16;
    n |= n >> 32;
    n += 1;

    return n;
}
