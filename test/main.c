#ifdef SS_LIB_RUN_TESTS

#include <stdio.h>

#include "test_array.h"
#include "test_string.h"


#define run(F) run_test(#F, F)

int num_run = 0;

static void run_test(const char *name, void (*f)()) {
    printf("Running test '%s'\n", name);
    (*f)();
    num_run += 1;
}

static void ss_array_tests() {
    run(default_array_is_empty);
    run(create_empty_array_with_set_capacity);
    run(create_array_from_data);
    run(clearing_array_leaves_buffer_valid);
    run(append_data_to_array);
    run(append_data_to_new_array);
    run(append_array);
    run(append_array_to_new_array);
    run(array_swap);
    run(insert_element_into_array);
    run(insert_element_at_beginning);
    run(insert_element_at_end);
    run(array_partition_odd_elems);
    run(array_partition_even_elems);
    run(array_partition_no_moves);
    run(array_partition_move_all);
    run(array_partition_all_match);
    run(array_partition_pivot_is_first);
    run(array_partition_pivot_is_last);
    run(array_partition_pivot_out_of_range);
    run(array_partition_pivot_is_highest);
    run(get_reference_to_element);
    run(get_array_length);
    run(check_whether_array_is_empty);
}

static void ss_string_tests() {
    run(default_string_is_empty);
    run(create_empty_string_with_set_capacity);
    run(create_string_from_cstring);
    run(clearing_string_leaves_buffer_valid);
    run(append_cstring);
    run(append_cstring_to_new_string);
    run(append_data_to_string);
    run(append_char_to_string);
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
    ss_array_tests();
    ss_string_tests();

    printf("\nSuccessfully ran %i tests.\n", num_run);
}

#endif
