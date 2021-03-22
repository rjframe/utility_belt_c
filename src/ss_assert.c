#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>

// TODO: Maybe use https://github.com/ianlancetaylor/libbacktrace for more
// portable backtraces.

// This is portable enough. Covers Clang, GCC, Intel, MSVC, Oracle.
#ifdef _MSC_VER
    #define NOINLINE __noinline
#else
    #define NOINLINE __attribute__((__noinline__))
#endif

#if defined __GLIBC__ && defined DEBUG
    #define SS_BACKTRACE

    #include <execinfo.h>
    #include <string.h>

    #ifdef SS_FULL_BACKTRACE
        #define NUM_FUNCS 200
    # else
        #define NUM_FUNCS 12
    #endif

    #if defined _WIN32 || defined _WIN64
        #define SEP '\\'
    #else
        #define SEP '/'
    #endif

    void NOINLINE ss_print_backtrace() {
        void *calls[NUM_FUNCS];

        const int size = backtrace(calls, NUM_FUNCS);
        char **symbols = backtrace_symbols(calls, NUM_FUNCS);

        if (symbols != NULL) {
            printf("\nBacktrace:\n\n");

            // We skip the assert and backtrace function calls.
            for (int i = 2; i < size; ++i) {
                char *symbol = strrchr(symbols[i], SEP);

                if (symbol == NULL) {
                    printf("  %i: %s\n", size - i, symbols[i]);
                } else {
                    printf("  %i: %s\n", size - i, symbol + 1);
                }
            }

            free(symbols);
        }
    }
#endif

#include "ss_assert.h"

void NOINLINE ss_do_assert_(
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

    if (abort_on_false) {
#if defined SS_BACKTRACE
        ss_print_backtrace();
#endif
        abort();
    }
}
