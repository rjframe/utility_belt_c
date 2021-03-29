#ifndef SS_LIB_ASSERT_H
#define SS_LIB_ASSERT_H

/* This Source Code Form is subject to the terms of the Mozilla Public License,
 * v. 2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

/* Custom assertion functions.
 *
 * Backtraces are only supported with Glibc.
 *
 * Recognized macro definitions:
 *
 * - SS_FULL_BACKTRACE: provide a full backtrace rather than the default short
 *   backtrace.
 */

#include <stdlib.h>

// Check an expression and print a message if false (this is assert() without
// the abort).
//
// This can help when doing printf-style debugging by only printing messages
// when a condition is not as expected, reducing noise in the output.
#define ss_check(EXPR, MSG) ss_do_assert_(!!(EXPR), MSG, \
    __FILE__, __LINE__, 0, #EXPR)

// Abort if the given expression fails.
#define ss_assert(EXPR) ss_do_assert_(!!(EXPR), NULL, \
    __FILE__, __LINE__, 1, #EXPR)

// Abort with a formatted message if the given expression fails.
#define ss_assert_msg(EXPR, MSG, ...) ss_do_assert_(!!(EXPR), MSG, \
    __FILE__, __LINE__, 1, #EXPR, __VA_ARGS__)

void ss_do_assert_(
    int condition,
    const char *message,
    const char *file,
    size_t line,
    int abort_on_false,
    const char *expr,
    ...
);

#endif
