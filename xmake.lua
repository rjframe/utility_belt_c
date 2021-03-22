add_rules("mode.debug", "mode.release")

target("ss_utils")
    set_kind("static")
    add_languages("c17")
    set_warnings("allextra")
    add_cflags("-Wpedantic", "-Werror=return-type",
        "-Werror=implicit-function-declaration",
        "-Werror=incompatible-pointer-types", "-Wformat-security", "-Wundef",
        "-Wshadow", "-Wcast-align", "-Wwrite-strings", "-Wcast-qual",
        "-Wconversion"
    )
    if is_mode("debug") then
        add_cflags("-g", "-grecord-gcc-switches")
    else
        add_cflags("-O2")
    end
    add_defines("USE_SS_LIB_ASSERT")
    add_ldflags("-rdynamic")
    add_includedirs("include", {public = true})
    add_headerfiles("include/*.h")
    add_files("src/*.c")


target("test")
    set_kind("binary")
    add_deps("ss_utils")
    add_languages("c17")
    set_warnings("allextra")
    add_cflags("-Wpedantic", "-Werror=return-type",
        "-Werror=implicit-function-declaration",
        "-Werror=incompatible-pointer-types", "-Wformat-security", "-Wundef",
        "-Wshadow", "-Wcast-align", "-Wwrite-strings", "-Wcast-qual",
        "-Wconversion"
    )
    add_cflags("-fstack-clash-protection", "-fstack-protector-strong",
        "-fasynchronous-unwind-tables"
    )
    add_cflags("-fsanitize=leak", "-fsanitize-address-use-after-scope"
        -- Linker error: "-fsanitize=undefined"
    )
    add_cflags("-g", "-grecord-gcc-switches")
    add_defines("DEBUG", "SS_DEBUG", "SS_LIB_RUN_TESTS", "USE_SS_LIB_ASSERT")
    add_ldflags("-rdynamic")
    add_includedirs("test", "include")
    add_files("src/*.c", "test/*.c")
