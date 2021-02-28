#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

#include "ss_assert.h"

void ss_do_assert_(
    int condition,
    const char *message,
    const char *file,
    size_t line,
    bool abort_on_false,
    const char *expr,
    ...
) {
    if (condition) { return; }

    if (message == NULL) {
        printf("%s (%ld): assertion failed: %s\n", file, line, expr);
    } else {
        va_list args;
        va_start(args, expr);

        vprintf(message, args);
        printf("\tin %s at line %ld\n", file, line);

        va_end(args);
    }
    if (abort_on_false) { abort(); }
}
