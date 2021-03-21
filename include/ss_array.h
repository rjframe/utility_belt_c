#ifndef SS_ARRAY_H
#define SS_ARRAY_H

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


#define DEFINE_ARRAY(T) DEFINE_ARRAY2(T, T)

#define DEFINE_ARRAY2(T, LBL)                                               \
struct ss_array_##LBL;                                                      \
struct ss_array_##LBL *ss_array_##LBL_create();                             \
void ss_array_##LBL##_free(struct ss_array_##LBL *array);                   \
struct ss_array_##LBL *ss_array_##LBL##_create_with_size(size_t num_elems); \
struct ss_array_##LBL *ss_array_##LBL##_create_from(T *data, size_t len);   \
/* Clear the array's data without invalidating the buffer. */               \
void ss_array_##LBL##_clear(struct ss_array_##LBL *array);                  \
bool ss_array_##LBL##_append_data(                                          \
    struct ss_array_##LBL *array,                                           \
    T *data,                                                                \
    size_t num_elems                                                        \
);                                                                          \
bool ss_array_##LBL##_append(                                               \
    struct ss_array_##LBL *array,                                           \
    struct ss_array_##LBL *src                                              \
);                                                                          \
bool ss_array_##LBL##_insert(                                               \
    struct ss_array_##LBL *array,                                           \
    T *elem,                                                                \
    size_t pos                                                              \
);                                                                          \
/* Partition the array so that elements for which the supplied function     \
   returns `true` precede elements for which it returns `false`.            \
                                                                            \
   Returns a pointer to the first element of the second group.              \
*/                                                                          \
T *ss_array_##LBL##_partition(                                              \
    struct ss_array_##LBL *array,                                           \
    bool (*f)(T* elem)                                                      \
);                                                                          \
size_t ss_array_##LBL##_len(struct ss_array_##LBL *array);                  \
T *ss_array_##LBL##_get(struct ss_array_##LBL *array, size_t pos);          \
const T* ss_array_##LBL##_ptr(struct ss_array_##LBL *array);                \
/* Point `out` to the array's internal data and free all other memory       \
   associated with the array (give up ownership of the data) and return     \
   the array's length.                                                      \
*/                                                                          \
size_t ss_array_##LBL##_dissolve(struct ss_array_##LBL *array, T **out);    \
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
void ss_array_##LBL##_free(struct ss_array_##LBL *array) {                     \
    if (array == NULL) return;                                                 \
    free(array->data);                                                         \
    free(array);                                                               \
}                                                                              \
                                                                               \
struct ss_array_##LBL *ss_array_##LBL##_create_with_size(size_t num_elems) {   \
    struct ss_array_##LBL *array = ss_array_##LBL##_create();                  \
    if (array == NULL) return NULL;                                            \
                                                                               \
    array->data = (T*) malloc(num_elems * sizeof(T));                          \
    if (array->data == NULL) {                                                 \
        free(array);                                                           \
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
        ss_array_##LBL##_free(array);                                          \
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
size_t ss_array_##LBL##_dissolve(struct ss_array_##LBL *array, T **out) {      \
    if (array == NULL) return NULL;                                            \
    size_t len = array->len;                                                   \
    *out = array->data;                                                        \
    free(array);                                                               \
    return len;                                                                \
}                                                                              \
                                                                               \
bool ss_array_##LBL##_is_empty(struct ss_array_##LBL *array) {                 \
    return array == NULL || array->data == NULL || array->len == 0;            \
}

#endif
