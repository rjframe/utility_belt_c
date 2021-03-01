#ifdef SS_LIB_RUN_TESTS

#include <stdio.h>

#include "test_string.h"

#define run(F) run_test(#F, F)

int num_run = 0;

static void run_test(const char *name, void (*f)()) {
    printf("Running test '%s'\n", name);
    (*f)();
    num_run += 1;
}

static void ss_string_tests() {
    run(default_string_is_empty);
    run(create_empty_string_with_set_capacity);
    run(create_string_from_cstring);
    run(clearing_string_leaves_buffer_valid);
    run(append_cstring);
    run(append_cstring_to_new_string);
    run(append_char);
    run(append_char_to_new_string);
    run(append_char_to_empty_string_with_one_null);
    run(append_string);
    run(append_string_to_new_string);
    run(get_const_reference_to_inner_cstring);
    run(get_string_length);
    run(check_whether_string_is_empty);
    run(compare_strings);
}

int main() {
    ss_string_tests();

    printf("\nSuccessfully ran %i tests.\n", num_run);
}

#endif
