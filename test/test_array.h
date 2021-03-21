#ifndef SS_LIB_TEST_ARRAY
#define SS_LIB_TEST_ARRAY

#include "ss_array.h"
#include "ss_assert.h"

DEFINE_ARRAY(int)
GENERATE_ARRAY(int)

typedef struct S { int a; int b; } S;
GENERATE_ARRAY2(S, s)

void default_array_is_empty() {
    struct ss_array_int *int_array = ss_array_int_create();
    struct ss_array_s *s_array = ss_array_s_create();
    ss_assert(int_array != NULL && s_array != NULL);

    ss_assert(ss_array_int_is_empty(int_array) && int_array->len == 0);
    ss_assert(ss_array_s_is_empty(s_array) && s_array->len == 0);

    ss_array_int_free(int_array);
    ss_array_s_free(s_array);
}

void create_empty_array_with_set_capacity() {
    struct ss_array_int *int_array = ss_array_int_create_with_size(4);
    struct ss_array_s *s_array = ss_array_s_create_with_size(4);
    ss_assert(int_array != NULL && s_array != NULL);

    ss_assert(int_array->len == 0 && s_array->len == 0);

    ss_assert(s_array->capacity == 4 * sizeof(S));
    ss_assert(int_array->capacity == 4 * sizeof(int));

    ss_array_int_free(int_array);
    ss_array_s_free(s_array);
}

void create_array_from_data() {
    int ints[] = { 1, 2, 3, 4 };

    S a = { .a = 1, .b = 1 };
    S b = { .a = 2, .b = 2 };
    S *ss = { &a, &b };

    struct ss_array_int *int_array = ss_array_int_create_from(ints, 4);
    struct ss_array_s *s_array = ss_array_s_create_from(ss, 2);

    ss_assert(int_array->len == 4);
    ss_assert(int_array->data[0] == 1);
    ss_assert(int_array->data[1] == 2);
    ss_assert(int_array->data[2] == 3);
    ss_assert(int_array->data[3] == 4);

    ss_assert(s_array->len == 2);
    ss_assert(s_array->data[0].a == 1);
    ss_assert(s_array->data[0].b == 1);
    ss_assert(s_array->data[1].a == 2);
    ss_assert(s_array->data[1].b == 2);

    free(int_array);
    free(s_array);
}

void clearing_array_leaves_buffer_valid() {
    int ints[] = { 1, 2, 3, 4 };
    struct ss_array_int *array = ss_array_int_create_from(ints, 4);
    size_t cap = array->capacity;

    ss_assert(array->data != NULL);
    ss_array_int_clear(array);
    ss_assert(array->data != NULL);

    ss_assert(array->len == 0);
    ss_assert(array->capacity == cap);

    free(array);
}

void append_data_to_array() {
    int ints_[] = { 1, 2, 3, 4 };
    int *ints = (int*) malloc(sizeof(int) * 4);
    memcpy(ints, ints_, sizeof(int) * 4);

    struct ss_array_int *array = ss_array_int_create_from(ints, 4);
    free(ints);

    int more_ints[] = { 5, 6, 7 };
    ss_assert(ss_array_int_append_data(array, more_ints, 3));

    ss_assert(array->len == 7);
    ss_assert(array->data[0] == 1);
    ss_assert(array->data[1] == 2);
    ss_assert(array->data[2] == 3);
    ss_assert(array->data[3] == 4);
    ss_assert(array->data[4] == 5);
    ss_assert(array->data[5] == 6);
    ss_assert(array->data[6] == 7);

    ss_array_int_free(array);
}

void append_data_to_new_array() {
    int ints_[] = { 1, 2, 3 };
    int *ints = (int*) malloc(sizeof(int) * 3);
    memcpy(ints, ints_, sizeof(int) * 3);

    struct ss_array_int *array = ss_array_int_create();
    ss_assert(ss_array_int_append_data(array, ints, 3));
    free(ints);

    ss_assert(array->len == 3);
    ss_assert(array->data[0] == 1);
    ss_assert(array->data[1] == 2);
    ss_assert(array->data[2] == 3);

    ss_array_int_free(array);
}

void append_array() {
    int first[3] = { 1, 2, 3 };
    int second[3] = { 4, 5, 6 };
    struct ss_array_int *first_array = ss_array_int_create_from(first, 3);
    struct ss_array_int *second_array = ss_array_int_create_from(second, 3);

    ss_assert(ss_array_int_append(first_array, second_array));

    ss_assert(first_array->len == 6);
    ss_assert(first_array->data[0] == 1);
    ss_assert(first_array->data[1] == 2);
    ss_assert(first_array->data[2] == 3);
    ss_assert(first_array->data[3] == 4);
    ss_assert(first_array->data[4] == 5);
    ss_assert(first_array->data[5] == 6);

    ss_array_int_free(first_array);
    ss_array_int_free(second_array);
}

void append_array_to_new_array() {
    struct ss_array_int *first_array = ss_array_int_create();
    int second[3] = { 4, 5, 6 };
    struct ss_array_int *second_array = ss_array_int_create_from(second, 3);

    ss_assert(ss_array_int_append(first_array, second_array));

    ss_assert(first_array->len == 3);
    ss_assert(first_array->data[0] == 4);
    ss_assert(first_array->data[1] == 5);
    ss_assert(first_array->data[2] == 6);

    ss_array_int_free(first_array);
    ss_array_int_free(second_array);
}

void array_swap() {
    int a = 1;
    int b = 2;

    ss_swap_int_(&a, &b);
    ss_assert(a == 2);
    ss_assert(b == 1);
}

void insert_element_into_array() {
    int elems[3] = { 1, 2, 4 };
    struct ss_array_int *array = ss_array_int_create_from(elems, 3);

    int elem = 3;
    ss_assert(ss_array_int_insert(array, &elem, 2));

    ss_assert_msg(array->len == 4, "len: %li", array->len);
    ss_assert(array->data[0] == 1);
    ss_assert(array->data[1] == 2);
    ss_assert(array->data[2] == 3);
    ss_assert(array->data[3] == 4);

    ss_array_int_free(array);
}

void insert_element_at_beginning() {
    int elems[3] = { 2, 3, 4 };
    struct ss_array_int *array = ss_array_int_create_from(elems, 3);

    int elem = 1;
    ss_assert(ss_array_int_insert(array, &elem, 0));

    ss_assert(array->len == 4);
    ss_assert(array->data[0] == 1);
    ss_assert(array->data[1] == 2);
    ss_assert(array->data[2] == 3);
    ss_assert(array->data[3] == 4);

    ss_array_int_free(array);
}

void insert_element_at_end() {
    int elems[3] = { 1, 2, 3 };
    struct ss_array_int *array = ss_array_int_create_from(elems, 3);

    int elem = 4;
    ss_assert(ss_array_int_insert(array, &elem, 3));

    ss_assert(array->len == 4);
    ss_assert(array->data[0] == 1);
    ss_assert(array->data[1] == 2);
    ss_assert(array->data[2] == 3);
    ss_assert(array->data[3] == 4);

    ss_array_int_free(array);
}

bool less_eq_five(int *i) { return *i <= 5; }

void array_partition_odd_elems() {
    int elems[9] = { 1, 9, 2, 3, 8, 4, 7, 5, 6 };
    struct ss_array_int *array = ss_array_int_create_from(elems, 9);

    int *partition = ss_array_int_partition(array, &less_eq_five);

    size_t i;
    for (i = 0; &array->data[i] != partition; ++i) {
        ss_assert(array->data[i] <= 5);
    }
    for (i += 1; i < 9; ++i) {
        ss_assert(array->data[i] > 5);
    }
}

void array_partition_even_elems() {
    int elems[8] = { 9, 2, 3, 8, 4, 7, 5, 6 };
    struct ss_array_int *array = ss_array_int_create_from(elems, 9);

    int *partition = ss_array_int_partition(array, &less_eq_five);

    size_t i;
    for (i = 0; &array->data[i] != partition; ++i) {
        ss_assert(array->data[i] <= 5);
    }
    for (i += 1; i < 9; ++i) {
        ss_assert(array->data[i] > 5);
    }
}

void array_partition_no_moves() {
    int elems[9] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    struct ss_array_int *array = ss_array_int_create_from(elems, 9);

    int *partition = ss_array_int_partition(array, &less_eq_five);

    size_t i;
    for (i = 0; &array->data[i] != partition; ++i) {
        ss_assert(array->data[i] <= 5);
    }
    for (i += 1; i < 9; ++i) {
        ss_assert(array->data[i] > 5);
    }
}

void array_partition_move_all() {
    int elems[9] = { 9, 8, 7, 6, 5, 4, 3, 2, 1 };
    struct ss_array_int *array = ss_array_int_create_from(elems, 9);

    int *partition = ss_array_int_partition(array, &less_eq_five);

    size_t i;
    for (i = 0; &array->data[i] != partition; ++i) {
        ss_assert(array->data[i] <= 5);
    }
    for (i += 1; i < 9; ++i) {
        ss_assert(array->data[i] > 5);
    }
}

void array_partition_all_match() {
    int elems[9] = { 5, 5, 5, 5, 5, 5, 5, 5, 5 };
    struct ss_array_int *array = ss_array_int_create_from(elems, 9);

    int *partition = ss_array_int_partition(array, &less_eq_five);

    size_t i;
    for (i = 0; &array->data[i] != partition; ++i) {
        ss_assert(array->data[i] <= 5);
    }
    for (i += 1; i < 9; ++i) {
        ss_assert(array->data[i] > 5);
    }
}

bool less_eq_one(int *i) { return *i <= 1; }

void array_partition_pivot_is_first() {
    int elems[9] = { 1, 9, 2, 3, 8, 4, 7, 5, 6 };
    struct ss_array_int *array = ss_array_int_create_from(elems, 9);

    int *partition = ss_array_int_partition(array, &less_eq_one);

    size_t i;
    for (i = 0; &array->data[i] != partition; ++i) {
        ss_assert(array->data[i] <= 1);
    }
    for (i += 1; i < 9; ++i) {
        ss_assert(array->data[i] > 1);
    }
}

bool less_eq_six(int *i) { return *i <= 6; }

void array_partition_pivot_is_last() {
    int elems[9] = { 1, 9, 2, 3, 8, 4, 7, 5, 6 };
    struct ss_array_int *array = ss_array_int_create_from(elems, 9);

    int *partition = ss_array_int_partition(array, &less_eq_six);

    size_t i;
    for (i = 0; &array->data[i] != partition; ++i) {
        ss_assert(array->data[i] <= 6);
    }
    for (i += 1; i < 9; ++i) {
        ss_assert(array->data[i] > 6);
    }
}

bool less_eq_eleven(int *i) { return *i <= 11; }

void array_partition_pivot_out_of_range() {
    int elems[9] = { 1, 9, 2, 3, 8, 4, 7, 5, 6 };
    struct ss_array_int *array = ss_array_int_create_from(elems, 9);

    int *partition = ss_array_int_partition(array, &less_eq_eleven);

    size_t i;
    for (i = 0; &array->data[i] != partition; ++i) {
        ss_assert(array->data[i] <= 11);
    }
    for (i += 1; i < 9; ++i) {
        ss_assert(array->data[i] > 11);
    }
}

bool less_eq_nine(int *i) { return *i <= 9; }

void array_partition_pivot_is_highest() {
    int elems[9] = { 1, 9, 2, 3, 8, 4, 7, 5, 6 };
    struct ss_array_int *array = ss_array_int_create_from(elems, 9);

    int *partition = ss_array_int_partition(array, &less_eq_nine);

    size_t i;
    for (i = 0; &array->data[i] != partition; ++i) {
        ss_assert(array->data[i] <= 9);
    }
    for (i += 1; i < 9; ++i) {
        ss_assert(array->data[i] > 9);
    }
}

void get_reference_to_element() {
    int elems[4] = { 1, 2, 3, 4 };
    struct ss_array_int *array = ss_array_int_create_from(elems, 4);

    int *elem = ss_array_int_get(array, 0);
    ss_assert(*elem == 1);
    elem = ss_array_int_get(array, 1);
    ss_assert(*elem == 2);
    elem = ss_array_int_get(array, 2);
    ss_assert(*elem == 3);
    elem = ss_array_int_get(array, 3);
    ss_assert(*elem == 4);
    elem = ss_array_int_get(array, 4);
    ss_assert(elem == NULL);

    ss_array_int_free(array);
}

void get_array_length() {
    struct ss_array_int *array = ss_array_int_create();
    ss_assert(ss_array_int_len(array) == 0);

    int elems[4] = { 1, 2, 3, 4 };
    ss_array_int_append_data(array, elems, 4);

    ss_assert(ss_array_int_len(array) == 4);

    ss_array_int_free(array);
}

void check_whether_array_is_empty() {
    struct ss_array_int *array = ss_array_int_create();
    ss_assert(ss_array_int_is_empty(array));

    int elems[4] = { 1, 2, 3, 4 };
    ss_array_int_append_data(array, elems, 4);

    ss_assert(! ss_array_int_is_empty(array));

    ss_array_int_free(array);
}

#endif
