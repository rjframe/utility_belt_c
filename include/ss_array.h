#ifndef SS_ARRAY_H
#define SS_ARRAY_H

/* This Source Code Form is subject to the terms of the Mozilla Public License,
 * v. 2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

/* Managed typesafe array type.
 *
 * When an array is first created, enough memory is allocated to store the
 * initial data. On later resizes, extra memory is allocated under the
 * assumption that an array that grows will probably continue to grow.
 *
 * Requres: ss_math.h
 */

#include <stdbool.h>
#include <string.h>

#include "ss_math.h"

#ifdef USE_SS_LIB_ASSERT
    #include "ss_assert.h"
#else
    #include <assert.h>

    #define ss_check(EXPR, MSG) assert(EXPR)
    #define ss_assert assert
    #define ss_assert_msg(EXPR, ...) assert(EXPR)
#endif


#define DECLARE_ARRAY(T) DECLARE_ARRAY2(T, T)

#define DECLARE_ARRAY2(T, LBL)                                              \
struct ss_array_##LBL;                                                      \
                                                                            \
/* Create a new, empty array.                                               \
 *                                                                          \
 * Returns NULL on failure to allocate memory.                              \
 */                                                                         \
struct ss_array_##LBL *ss_array_##LBL##_create();                           \
                                                                            \
/* Free the provided array and set its pointer to NULL. */                  \
void ss_array_##LBL##_free(struct ss_array_##LBL **array);                  \
                                                                            \
/* Create a new array with the specified capacity.                          \
 * If `num_elems` is 0, behaves like [ss_array_##LBL##_create].             \
 */                                                                         \
struct ss_array_##LBL *ss_array_##LBL##_create_with_size(size_t num_elems); \
                                                                            \
/* Create an array from the provided pointer and length.                    \
 *                                                                          \
 * The data in the original array is copied to a new memory buffer.         \
 *                                                                          \
 * Returns NULL on failure to allocated memory.                             \
 */                                                                         \
struct ss_array_##LBL *ss_array_##LBL##_create_from(T *data, size_t len);   \
                                                                            \
/* Clear the array's data without invalidating the buffer.                  \
 * The optimizer may optimize this call away.                               \
 */                                                                         \
void ss_array_##LBL##_clear(struct ss_array_##LBL *array);                  \
                                                                            \
/* Append the provided data to an array.                                    \
 *                                                                          \
 * If `data` is NULL or `num_elems` is 0, does nothing and returns `false`. \
 */                                                                         \
bool ss_array_##LBL##_append_data(                                          \
    struct ss_array_##LBL *array,                                           \
    T *data,                                                                \
    size_t num_elems                                                        \
);                                                                          \
                                                                            \
/* Append one array to another. The `src` array is left unchanged.          \
 *                                                                          \
 * If either array is invalid, does nothing and returns `false`.            \
 *                                                                          \
 * On failure to allocate, leaves `array` unchanged and returns `false`.    \
 */                                                                         \
bool ss_array_##LBL##_append(                                               \
    struct ss_array_##LBL *array,                                           \
    struct ss_array_##LBL *src                                              \
);                                                                          \
                                                                            \
/* Insert an element into the array at the specified position.              \
 *                                                                          \
 * If `pos` is outside the array bounds, does nothing and returns false.    \
 *                                                                          \
 * Returns `false` if unable to allocate memory.                            \
 */                                                                         \
bool ss_array_##LBL##_insert(                                               \
    struct ss_array_##LBL *array,                                           \
    T *elem,                                                                \
    size_t pos                                                              \
);                                                                          \
                                                                            \
/* Partition the array so that elements for which the supplied function     \
 * returns `true` precede elements for which it returns `false`.            \
 *                                                                          \
 * Returns a pointer to the first element of the second group.              \
*/                                                                          \
T *ss_array_##LBL##_partition(                                              \
    struct ss_array_##LBL *array,                                           \
    bool (*f)(T* elem)                                                      \
);                                                                          \
                                                                            \
/* Return the array's length. */                                            \
size_t ss_array_##LBL##_len(struct ss_array_##LBL *array);                  \
                                                                            \
/* Get a reference to the element at the specified position.                \
 *                                                                          \
 * If `pos` is outside the array's bounds, returns NULL.                    \
 */                                                                         \
T *ss_array_##LBL##_get(struct ss_array_##LBL *array, size_t pos);          \
                                                                            \
/* Get a constant reference to the underlying data. */                      \
const T* ss_array_##LBL##_ptr(struct ss_array_##LBL *array);                \
                                                                            \
/* Convert the array to its underlying C-style array.                       \
 *                                                                          \
 * Returns the data in `out`, frees all other memory associated with the    \
 * array, and returns the array's length.                                   \
 *                                                                          \
 * The pointer to the array will be set to NULL. Managing the data becomes  \
 * the caller's responsibility.                                             \
*/                                                                          \
size_t ss_array_##LBL##_dissolve(struct ss_array_##LBL **array, T **out);   \
                                                                            \
/* Returns `true` if the array is empty; otherwise, returns `false`. */     \
bool ss_array_##LBL##_is_empty(struct ss_array_##LBL *array);


#define GENERATE_ARRAY(T) GENERATE_ARRAY2(T, T)

// Use label for cases when type spans multiple words, is a pointer, etc.
#define GENERATE_ARRAY2(T, LBL)                                                \
struct ss_array_##LBL {                                                        \
    T *data;                                                                   \
    /* len is elements */                                                      \
    size_t len;                                                                \
    /* capacity is bytes */                                                    \
    size_t capacity;                                                           \
};                                                                             \
                                                                               \
struct ss_array_##LBL *ss_array_##LBL##_create() {                             \
    struct ss_array_##LBL *array =                                             \
        (struct ss_array_##LBL*) malloc(sizeof(struct ss_array_##LBL));        \
    if (array == NULL) return NULL;                                            \
                                                                               \
    array->data = NULL;                                                        \
    array->len = 0;                                                            \
    array->capacity = 0;                                                       \
                                                                               \
    return array;                                                              \
}                                                                              \
                                                                               \
void ss_array_##LBL##_free(struct ss_array_##LBL **array) {                    \
    if (array == NULL || *array == NULL) return;                               \
    free((*array)->data);                                                      \
    (*array)->data = NULL;                                                     \
    free(*array);                                                              \
    *array = NULL;                                                             \
}                                                                              \
                                                                               \
struct ss_array_##LBL *ss_array_##LBL##_create_with_size(size_t num_elems) {   \
    struct ss_array_##LBL *array = ss_array_##LBL##_create();                  \
    if (array == NULL) return NULL;                                            \
                                                                               \
    array->data = (T*) malloc(num_elems * sizeof(T));                          \
    if (array->data == NULL) {                                                 \
        free(array);                                                           \
        array = NULL;                                                          \
        return NULL;                                                           \
    }                                                                          \
    array->capacity = num_elems * sizeof(T);                                   \
                                                                               \
    return array;                                                              \
}                                                                              \
                                                                               \
struct ss_array_##LBL *ss_array_##LBL##_create_from(T *data, size_t len) {     \
    struct ss_array_##LBL *array = ss_array_##LBL##_create_with_size(len);     \
    if (array == NULL) return NULL;                                            \
                                                                               \
    size_t len_bytes = len * sizeof(T);                                        \
    T *buf = malloc(len_bytes);                                                \
    if (buf == NULL) {                                                         \
        ss_array_##LBL##_free(&array);                                         \
        return NULL;                                                           \
    }                                                                          \
                                                                               \
    memcpy(buf, data, len_bytes);                                              \
                                                                               \
    array->data = buf;                                                         \
    array->len = len;                                                          \
    return array;                                                              \
}                                                                              \
                                                                               \
void ss_array_##LBL##_clear(struct ss_array_##LBL *array) {                    \
    if (array == NULL) return;                                                 \
                                                                               \
    memset(array->data, '\0', array->len * sizeof(T));                         \
    array->len = 0;                                                            \
}                                                                              \
                                                                               \
bool ss_array_##LBL##_append_data(                                             \
    struct ss_array_##LBL *array,                                              \
    T *data,                                                                   \
    size_t num_elems                                                           \
) {                                                                            \
    if (array == NULL || data == NULL || num_elems == 0) return false;         \
                                                                               \
    size_t old_len_bytes = array->len * sizeof(T);                             \
    size_t new_len_bytes = old_len_bytes + num_elems * sizeof(T);              \
                                                                               \
    if (new_len_bytes > array->capacity) {                                     \
        size_t new_cap = next_pow_of_two(new_len_bytes);                       \
        T *buf = (T*) realloc(array->data, new_cap);                           \
        if (buf == NULL) return false;                                         \
                                                                               \
        array->data = buf;                                                     \
        array->capacity = new_cap;                                             \
    }                                                                          \
                                                                               \
    ss_assert(array->capacity >= (array->len + num_elems) * sizeof(T));        \
    memcpy(&array->data[array->len], data, num_elems * sizeof(T));             \
    array->len += num_elems;                                                   \
                                                                               \
    return true;                                                               \
}                                                                              \
                                                                               \
bool ss_array_##LBL##_append(                                                  \
    struct ss_array_##LBL *array,                                              \
    struct ss_array_##LBL *src                                                 \
) {                                                                            \
    if (array == NULL || src == NULL) return false;                            \
    return ss_array_##LBL##_append_data(array, src->data, src->len);           \
}                                                                              \
                                                                               \
void ss_swap_##LBL##_(T *a, T *b) {                                            \
    T tmp = *a;                                                                \
    *a = *b;                                                                   \
    *b = tmp;                                                                  \
}                                                                              \
                                                                               \
bool ss_array_##LBL##_insert(struct ss_array_##LBL *array, T *elem, size_t pos)\
{                                                                              \
    if (array == NULL || elem == NULL || pos > array->len) return false;       \
                                                                               \
    if (! ss_array_##LBL##_append_data(array, &array->data[array->len-1], 1)) {\
        return false;                                                          \
    }                                                                          \
                                                                               \
    T *tmp = elem;                                                             \
    for (size_t i = pos; i < array->len; ++i) {                                \
        ss_swap_##LBL##_(tmp, &array->data[i]);                                \
    }                                                                          \
                                                                               \
    return true;                                                               \
}                                                                              \
                                                                               \
T *ss_array_##LBL##_partition(                                                 \
    struct ss_array_##LBL *array,                                              \
    bool (*f)(T* elem)                                                         \
) {                                                                            \
    if (array == NULL || f == NULL) return NULL;                               \
    /* Hoare's partition algorithm */                                          \
    T *low = array->data;                                                      \
    T *high = &array->data[array->len-1];                                      \
                                                                               \
    while (true) {                                                             \
        while (low < high && f(low)) {                                         \
            low += 1;                                                          \
        }                                                                      \
        while (low < high && !f(high)) {                                       \
            high -= 1;                                                         \
        }                                                                      \
                                                                               \
        if (low < high) {                                                      \
            ss_swap_##LBL##_(low, high);                                       \
            low += 1;                                                          \
            high -= 1;                                                         \
        } else {                                                               \
            if (f(low)) {                                                      \
                low += 1;                                                      \
            }                                                                  \
            return low;                                                        \
        }                                                                      \
    }                                                                          \
}                                                                              \
                                                                               \
size_t ss_array_##LBL##_len(struct ss_array_##LBL *array) {                    \
    if (array == NULL) return 0;                                               \
    return array->len;                                                         \
}                                                                              \
                                                                               \
T *ss_array_##LBL##_get(struct ss_array_##LBL *array, size_t pos) {            \
    if (array == NULL || pos >= array->len) return NULL;                       \
    return &array->data[pos];                                                  \
}                                                                              \
                                                                               \
const T* ss_array_##LBL##_ptr(struct ss_array_##LBL *array) {                  \
    if (array == NULL) return NULL;                                            \
    return array->data;                                                        \
}                                                                              \
                                                                               \
size_t ss_array_##LBL##_dissolve(struct ss_array_##LBL **array, T **out) {     \
    if (array == NULL || *out != NULL) return 0;                               \
    size_t len = (*array)->len;                                                \
                                                                               \
    T *buf = NULL;                                                             \
    if (len > 0) {                                                             \
        buf = (T*) realloc((*array)->data, sizeof(T) * len);                   \
        if (buf == NULL) {                                                     \
            /* Shrinking; should be impossible. */                             \
            ss_assert(false);                                                  \
        }                                                                      \
    } else {                                                                   \
        buf = (*array)->data;                                                  \
    }                                                                          \
                                                                               \
    *out = buf;                                                                \
    return len;                                                                \
}                                                                              \
                                                                               \
bool ss_array_##LBL##_is_empty(struct ss_array_##LBL *array) {                 \
    return array == NULL || array->data == NULL || array->len == 0;            \
}

#endif
