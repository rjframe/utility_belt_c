# Utility sources for C

This is a collection of my commonly-used C types and functions.

Everything should be compatible with GCC and Clang on Windows and Linux, but I
typically only build and test with GCC on Linux. Some features may only be
present for specific environments (namely, GCC on Linux).

No attempt is made to ensure safe sharing across threads.


## Table of Contents

* [License](#license)
* [Running Tests](#running-tests)
* [Using](#using)
    * [Standalone Sources](#standalone-source-files)
    * [xmake Package](#xmake-package)
* [Source Overview](#source-overview)
    * [Array](#array)
    * [Assert](#assert)
    * [Math](#math)
    * [String](#string)
* [Contributing](#contributing)
    * [Code Styles](#code-styles)
* [Contact](#contact)


## License

All source code is licensed under the terms of the
[MPL 2.0 license](LICENSE.txt).


## Running Tests

Requires [xmake](https://xmake.io) to run tests:

```bash
$ xmake -w && xmake run test
```


## Using

### Standalone Source Files

Most source files are independent so you can simply drop the source and header
files you want into your current project. The header files document any
dependencies that a given module may have.

Most files have an optional dependency on `ss_assert.h`.


### xmake Package

You can create an xmake package:

```bash
$ xmake package ss_utils -o /my/xmake/packages
```

And then use it in your xmake.lua file:

```lua
add_rules("mode.debug", "mode.release")

add_packagedirs("/my/xmake/packages")

target("my-program")
    set_kind("binary")
    add_packages("ss_utils")
    add_defines("USE_SS_LIB_ASSERT")
    if is_mode("debug")
        -- This is needed for backtraces on failed assertions
        add_ldflags("-rdynamic")
    end
```


## Source Overview

The headers contain API documentation and usage notes.


### Array

`ss_array` is a header-only, typesafe, dynamically-sized array type that manages
its own memory. There is a partition function but no sort function.

Use the `GENERATE_ARRAY` macro to create an array for a given type, or the
`GENERATE_ARRAY2` macro if you need an array named differently than the type:

```c
GENERATE_ARRAY(int)
struct ss_array_int *a1 = ss_array_int_create();

GENERATE_ARRAY2(int*, intptr)
struct ss_array_intptr *a2 = ss_array_intptr_create();
```

The `DECLARE_ARRAY` and `DECLARE_ARRAY2` macros provide equivalent struct and
function declarations so you can create an opaque type in a header and avoid
exposing the private swap function.


#### Dependencies

Required: `ss_math.h` for `next_pow_of_two`

Optional: `ss_assert.h`


### Assert

`ss_assert.h` provides an alternative assert function, optionally with a
message.

The other source files only use `ss_assert` if `USE_SS_LIB_ASSERT` is defined;
otherwise they use the `assert` function from `assert.h`.

With Glib, `ss_assert` provides a short backtrace, or a full backtrace if
`SS_FULL_BACKTRACE` is defined.

`ss_assert.h` also provides an `ss_check` macro that prints a message but does
not abort if a condition is false. This can be helpful during printf-style
debugging to avoid outputting noise when the environment state is as expected.


### Math

The math module currently only contains a function to calculate the nearest
power of two of a number.


### String

`ss_string` is a true string type that manages its own memory. `ss_string`s are
guaranteed to be NULL-terminated, so you can obtain the pointer to the
underlying string via `ss_string_as_cstring` and pass it to any function
expecting a string. Such a function *must not* modify the length of the string.


#### Dependencies

Required: `ss_math.h` for `next_pow_of_two`

Optional: `ss_assert.h`


## Contributing

The official repository is at
[https://git.sr.ht/~rjframe/utility_belt_c](https://git.sr.ht/~rjframe/utility_belt_c);
please send any bug reports, patches, or other communications to
[https://lists.sr.ht/~rjframe/public](https://lists.sr.ht/~rjframe/public) as I
likely will not be checking the Github mirror often.


### Code Styles

* Postfix the names of private members in public headers with an underscore.
* Do not typedef the `struct` keyword away. That's a decision for user code.
* Use fixed integral types rather than `int`, `long`, etc.
* Use `size_t` for index or positional values.
* Use a hard 80-column line width and spaces for indentation.


## Contact

- Email: code@ryanjframe.com
- Website: [www.ryanjframe.com](https://www.ryanjframe.com)
- diaspora*: rjframe@diasp.org
