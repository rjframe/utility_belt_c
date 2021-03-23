#ifndef SS_LIB_STRING_H
#define SS_LIB_STRING_H

/* This Source Code Form is subject to the terms of the Mozilla Public License,
 * v. 2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

/* Managed string type.
 *
 * When a new string is created, allocated memory is sized to the new string. On
 * later resizes, extra capacity is allocated.
 *
 * This provides decent general-purpose behavior:
 *
 * - Many strings will not take more memory than necessary.
 * - We reduce allocations for strings that are being modified.
 *
 *  Known issues:
 *
 *  - Not fully compatible with multi-byte chars (only a problem on
 *    architectures without a byte type).
 *
 *  Requires:
 *
 *  ss_math.h
 */

// TODO: Add a split function.

#include <stdbool.h>
#include <stddef.h>

// A string type that manages its own memory and tracks its length.
struct ss_string;

// Create a new, empty string.
//
// The returned pointer will be NULL on failure to allocate.
struct ss_string *ss_string_create();

// Create an empty string with the specified buffer size, in number of chars.
//
// The returned pointer will be NULL on failure to allocate.
//
// If sz is 0, behaves like [ss_string_create].
struct ss_string *ss_string_create_with_size(size_t sz);

// Create a string from the provided C string.
//
// The returned pointer will be NULL on failure to allocate.
struct ss_string *ss_string_create_from_cstring(const char *s);

// Free the provided string and set its pointer to NULL.
void ss_string_free(struct ss_string **s);

// Clear the string's data, but leave the underlying memory buffer unchanged.
//
// `ss_string_clear` only clears the memory contained by the string, not the
// entire memory buffer.
//
// The optimizer may optimize the entire call to `ss_string_clear` away.
// See [ss_string_secure_clear] to guarantee that the buffer is cleared.
void ss_string_clear(struct ss_string *s);

// Append the provided C string to an ss_string.
//
// Returns:
//
// Returns true on success.
//
// If either dest or src are null or invalid or if src is an empty string, does
// nothing and returns false.
//
// On failure to allocate, returns false and leaves dest unchanged.
bool ss_string_append_cstring(struct ss_string *dest, const char *src);

// Append the provided char data to an ss_string.
//
// The input data dies not need to be null-terminated.
//
// `len` must be the number of elements to insert and must not overflow the
// input array.
//
// Returns:
//
// Returns true on success.
//
// If either dest or src are null or invalid or if src is an empty string, does
// nothing and returns false.
//
// On failure to allocate, returns false and leaves dest unchanged.
bool ss_string_append_data(struct ss_string *dest, const char *src, size_t len);

// Append the given char to the string.
//
// Returns:
//
// Returns true on success.
//
// If dest is null or invalid or if src is an empty string, does nothing and
// returns false.
//
// On failure to allocate, returns false and leaves dest unchanged.
bool ss_string_append_char(struct ss_string *dest, char src);

// Append one ss_string to another.
//
// Returns:
//
// Returns true on success.
//
// If either dest or src are null or invalid, does nothing and returns false.
//
// On failure to allocate, returns false and leaves dest unchanged.
bool ss_string_append_string(
    struct ss_string *dest,
    const struct ss_string *src
);

// Get a constant reference to the underlying C string.
const char *ss_string_as_cstring(struct ss_string *s);

// Get the length of this string.
size_t ss_string_len(const struct ss_string *s);

// Check whether the string is empty.
//
// A string containing only '\0' is empty, so it is possible for a string to
// have a length of one and still be empty:
//
// ```
// struct ss_string *s = ss_string_create();
// ss_assert(ss_string_is_empty(s));
// ss_assert(ss_string_len(s) == 0);
// ss_string_free(&s);
//
// s = ss_string_from_cstring("");
// ss_assert(ss_string_is_empty(s));
// ss_assert(ss_string_len(s) == 1);
// ```
bool ss_string_is_empty(const struct ss_string *s);

// Compare two strings
//
// This function has the same semantics as strcmp, except when only one of the
// strings is NULL (invalid), in which case ss_string_cmp returns -1.
int ss_string_cmp(const struct ss_string *s1, const struct ss_string *s2);

#endif
