/* This Source Code Form is subject to the terms of the Mozilla Public License,
 * v. 2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "ss_string.h"
#include "ss_math.h"

#ifdef USE_SS_LIB_ASSERT
    #include "ss_assert.h"
#else
    #include <assert.h>

    #define ss_check(EXPR, MSG) assert(EXPR)
    #define ss_assert assert
    #define ss_assert_msg(EXPR, ...) assert(EXPR)
#endif

struct ss_string {
    // A valid C string
    char* str;
    // The current length of the string, including the null terminator
    size_t len;
    // The size of the string buffer
    size_t capacity;
};

struct ss_string *ss_string_create() {
    struct ss_string *s = (struct ss_string*) malloc(sizeof(struct ss_string));
    if (s == NULL) { return NULL; }

    s->str = NULL;
    s->len = 0;
    s->capacity = 0;

    return s;
}

struct ss_string *ss_string_create_with_size(size_t sz) {
    struct ss_string *s = ss_string_create();
    if (s == NULL) return NULL;

    if (sz > 0) {
        size_t cap = next_pow_of_two(sz);
        s->str = (char*) calloc(cap, sizeof(char));

        if (s->str == NULL) {
            free(s);
            s = NULL;
            return NULL;
        }

        s->capacity = cap;
    }

    return s;
}

struct ss_string *ss_string_create_from_cstring(const char *s) {
    size_t len = strlen(s) + 1;
    struct ss_string *str = ss_string_create_with_size(len);
    if (str == NULL) return NULL;

    memcpy(str->str, s, len);
    str->len = len;

    ss_assert_msg(str->str[str->len-1] == '\0',
        "Len: %i, end char is %c", str->len, str->str[str->len]);

    return str;
}

void ss_string_free(struct ss_string **s) {
    if (s == NULL || *s == NULL) return;
    free((*s)->str);
    (*s)->str = NULL;
    free(*s);
    *s = NULL;
}

void ss_string_clear(struct ss_string *s) {
    if (s == NULL || s->str == NULL) return;

    memset(s->str, '\0', s->len);
    s->len = 0;
}

// TODO: use ss_string_append_data for this.
bool ss_string_append_cstring(struct ss_string *dest, const char *src) {
    if (dest == NULL || src == NULL || *src == '\0') {
        return false;
    }

    // Adjust for empty (unallocated) strings - count the virtual terminator.
    if (dest->len == 0) { dest->len = 1; }

    size_t src_len = strlen(src);
    size_t new_len = dest->len + src_len;

    if (new_len > dest->capacity) {
        char *new_str = (char*) realloc(
            dest->str,
            next_pow_of_two(new_len * 2)
        );
        if (new_str == NULL) {
            return false;
        } else {
            dest->str = new_str;
            dest->capacity = new_len;
        }
    }

    memcpy(dest->str + dest->len - 1, src, src_len);
    dest->len = new_len;
    dest->str[new_len-1] = '\0';

    ss_assert(dest->str[dest->len-1] == '\0');

    return true;
}

bool ss_string_append_data(struct ss_string *dest, const char *src, size_t len)
{
    // TODO: We should only append a null-terminator if the data appended
    // doesn't already have one.

    if (dest == NULL || src == NULL || *src == '\0') {
        return false;
    }

    // Adjust for empty (unallocated) strings - count the virtual terminator.
    if (dest->len == 0) { dest->len = 1; }

    size_t new_len = dest->len + len;

    if (new_len > dest->capacity) {
        char *new_str = (char*) realloc(
            dest->str,
            next_pow_of_two(new_len * 2)
        );
        if (new_str == NULL) {
            return false;
        } else {
            dest->str = new_str;
            dest->capacity = new_len;
        }
    }

    memcpy(dest->str + dest->len - 1, src, len);
    dest->len = new_len;
    dest->str[new_len - 1] = '\0';

    ss_assert(dest->str[dest->len-1] == '\0');

    return true;
}

bool ss_string_append_char(struct ss_string *dest, char src) {
    if (dest == NULL || src == '\0') { return false; }

    if (dest->str == NULL) {
        // Storage was not yet allocated.
        char *new_str = (char*) calloc('\0', 16);
        if (new_str == NULL) return false;

        dest->len = 1;
        dest->str = new_str;
        dest->capacity = 16;
    } else if (dest->len == dest->capacity) {
        size_t new_cap = next_pow_of_two(2 * dest->capacity * 2);
        char *new_str = (char*) realloc(dest->str, new_cap);
        if (new_str == NULL) {
            return false;
        } else {
            dest->str = new_str;
            dest->capacity *= 2;
        }
    }

    dest->str[dest->len-1] = src;
    dest->str[dest->len] = '\0';
    dest->len += 1;

    ss_assert(dest->str[dest->len-1] == '\0');

    return true;
}

bool ss_string_append_string(
    struct ss_string *dest,
    const struct ss_string *src
) {
    if (dest == NULL || src == NULL || src->str == NULL || *src->str == '\0') {
        return false;
    }

    // Adjust for empty (unallocated) strings - count the virtual terminator.
    if (dest->len == 0) { dest->len = 1; }

    size_t new_len = dest->len + src->len - 1;

    if (new_len > dest->capacity) {
        char *new_str = (char*) realloc(
            dest->str,
            next_pow_of_two(new_len * 2)
        );
        if (new_str == NULL) {
            return false;
        } else {
            dest->str = new_str;
            dest->capacity = new_len;
        }
    }

    memcpy(dest->str + dest->len - 1, src->str, src->len);
    dest->len = new_len;

    ss_assert(dest->str[dest->len-1] == '\0');

    return true;
}

const char *ss_string_as_cstring(struct ss_string *s) {
    if (s == NULL) return NULL;
    return s->str;
}

size_t ss_string_len(const struct ss_string *s) {
    if (s == NULL || s->str == NULL) return 0;
    return s->len;
}

bool ss_string_is_empty(const struct ss_string *s) {
    return s == NULL || s->str == NULL || s->len == 0 || s->str[0] == '\0';
}

int ss_string_cmp(const struct ss_string *s1, const struct ss_string *s2) {
    return s1 == NULL && s2 == NULL ? 0
        : s1 == NULL || s2 == NULL ? -1
        : strcmp(s1->str, s2->str);
}

