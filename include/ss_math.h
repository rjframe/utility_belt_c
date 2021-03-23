#ifndef SS_MATH_H
#define SS_MATH_H

/* This Source Code Form is subject to the terms of the Mozilla Public License,
 * v. 2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#include <stdint.h>

// If num is a power of two, returns num. Otherwise returns the next-highest
// power of two.
uint64_t next_pow_of_two(uint64_t num);

#endif

