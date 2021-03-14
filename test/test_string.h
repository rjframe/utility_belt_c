#ifndef SS_LIB_TEST_STRING
#define SS_LIB_TEST_STRING

#include <stdlib.h>
#include <string.h>

#include "ss_assert.h"
#include "ss_string.h"

// Test note: Define the opaque type.
// TODO: Move this to an inner/impl file to include rather than duplicate the
// definition.
struct ss_string {
    // A valid C string
    char* str;
    // The current length of the string, including the null terminator
    size_t len;
    // The size of the string buffer
    size_t capacity;
};

void default_string_is_empty() {
    struct ss_string *s = ss_string_create();
    ss_assert(s != NULL && s->str == NULL);
    ss_assert(ss_string_is_empty(s) && s->len == 0);

    ss_string_free(s);
}

void create_empty_string_with_set_capacity() {
    struct ss_string *s = ss_string_create_with_size(8);
    ss_assert(s->len == 0);
    ss_assert(s->capacity == 8);

    ss_string_free(s);
}

void create_string_from_cstring() {
    struct ss_string *s = ss_string_create_from_cstring("test string");
    ss_assert(s->len == 12);
    ss_assert(strcmp(s->str, "test string") == 0);

    ss_string_free(s);
}

void clearing_string_leaves_buffer_valid() {
    struct ss_string *s = ss_string_create_from_cstring("test string");
    size_t cap = s->capacity;

    ss_assert(s->str != NULL);
    ss_string_clear(s);
    ss_assert(s->str != NULL);

    ss_assert_msg(s->len == 0, "len is %i", s->len);
    ss_assert(s->capacity = cap);
    ss_assert(strcmp(s->str, "\0\0\0\0\0\0\0\0\0\0\0\0") == 0);

    ss_string_free(s);
}

void append_cstring() {
    struct ss_string *s = ss_string_create_from_cstring("a");
    ss_string_append_cstring(s, "bcd");

    ss_assert_msg(s->len == 5, "len is %li", s->len);
    ss_assert(strcmp(s->str, "abcd") == 0);

    ss_string_free(s);
}

void append_cstring_to_new_string() {
    struct ss_string *s = ss_string_create();
    ss_string_append_cstring(s, "asdf");

    ss_assert_msg(s->len == 5, "len is %li", s->len);
    ss_assert_msg(strncmp(s->str, "asdf", 4) == 0,
        "value is '%s'; strcmp: %i", s->str
    );

    ss_string_free(s);
}

void append_data() {
    struct ss_string *s = ss_string_create_from_cstring("asdf");
    const char *data = "abc";

    ss_string_append_data(s, data, 3);

    ss_assert_msg(s->len == 8, "len is %li", s->len);
    ss_assert(strcmp(s->str, "asdfabc") == 0);

    ss_string_free(s);
}

void append_char() {
    struct ss_string *s = ss_string_create_from_cstring("a");
    ss_string_append_char(s, 'b');

    ss_assert_msg(s->len == 3, "len is %li", s->len);
    ss_assert_msg(strcmp(s->str, "ab") == 0, "str is '%s'", s->str);

    ss_string_free(s);
}

void append_char_to_new_string() {
    struct ss_string *s = ss_string_create();
    ss_string_append_char(s, 'a');

    ss_assert_msg(s->len == 2, "len is %li", s->len);
    ss_assert_msg(strcmp(s->str, "a") == 0, "str is '%s'", s->str);

    ss_string_free(s);
}

void append_char_to_empty_string_with_one_null() {
    struct ss_string *s = ss_string_create_from_cstring("\0");
    ss_string_append_char(s, 'a');

    ss_assert_msg(s->len == 2, "len is %li", s->len);
    ss_assert_msg(strcmp(s->str, "a") == 0, "str is '%s'", s->str);

    ss_string_append_char(s, '\0');
    ss_assert(s->len == 2);

    ss_string_free(s);
}

void append_string() {
    struct ss_string *s1 = ss_string_create_from_cstring("ab");
    struct ss_string *s2 = ss_string_create_from_cstring("cd");
    ss_string_append_string(s1, s2);

    ss_assert_msg(s1->len == 5, "len is %li", s1->len);
    ss_assert_msg(strcmp(s1->str, "abcd") == 0, "str is '%s'", s1->str);

    ss_string_free(s1);
    ss_string_free(s2);
}

void append_string_to_new_string() {
    struct ss_string *s1 = ss_string_create();
    struct ss_string *s2 = ss_string_create_from_cstring("ab");

    ss_string_append_string(s1, s2);

    ss_assert_msg(s1->len == 3, "len is %li", s1->len);
    ss_assert(strcmp(s1->str, s2->str) == 0);

    ss_string_free(s1);
    ss_string_free(s2);
}

void get_const_reference_to_inner_cstring() {
    struct ss_string *s = ss_string_create_from_cstring("abc");

    ss_assert(strcmp(ss_string_as_cstring(s), "abc") == 0);

    ss_string_free(s);
}

void get_string_length() {
    struct ss_string *s = ss_string_create_from_cstring("abc");
    ss_assert(ss_string_len(s) == 4);

    ss_string_clear(s);
    ss_assert(ss_string_len(s) == 0);

    ss_string_free(s);
}

void check_whether_string_is_empty() {
    struct ss_string *s = ss_string_create_from_cstring("");

    ss_assert_msg(s->len == 1, "len is %li", s->len);
    ss_assert(ss_string_is_empty(s));

    ss_string_append_cstring(s, "a");
    ss_assert(! ss_string_is_empty(s));

    ss_string_free(s);
}

void compare_strings() {
    struct ss_string *s1 = ss_string_create_from_cstring("ab");
    struct ss_string *s2 = ss_string_create_from_cstring("ab");
    struct ss_string *s3 = ss_string_create_from_cstring("ac");

    ss_assert(ss_string_cmp(s1, s2) == 0);
    ss_assert(ss_string_cmp(s1, s3) != 0);

    ss_string_free(s1);
    ss_string_free(s2);
    ss_string_free(s3);
}

#endif
