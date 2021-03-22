#ifndef SS_LIB_ASSERT_H
#define SS_LIB_ASSERT_H

/* Custom assertion functions.
 *
 * Backtraces are only supported with the GCC compiler.
 *
 * Recognized macro definitions:
 *
 * - SS_FULL_BACKTRACE: provide a full backtrace rather than the default short
 *   backtrace.
 */

#include <stdbool.h>
#include <stdlib.h>

// Check an expression and print a message if false (this is assert() without
// the abort).
#define ss_check(EXPR, MSG) ss_do_assert_(!!(EXPR), MSG, \
    __FILE__, __LINE__, false, #EXPR)

// Abort if the given expression fails.
#define ss_assert(EXPR) ss_do_assert_(!!(EXPR), NULL, \
    __FILE__, __LINE__, true, #EXPR)

// Abort with a message if the given expression fails.
#define ss_assert_msg(EXPR, MSG, ...) ss_do_assert_(!!(EXPR), MSG, \
    __FILE__, __LINE__, true, #EXPR, __VA_ARGS__)

void ss_do_assert_(
    int condition,
    const char *message,
    const char *file,
    size_t line,
    bool abort_on_false,
    const char *expr,
    ...
);

#endif
