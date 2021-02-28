# Assumes GNU Make and GCC.

.PHONY: clean

DIRS = src include
HEADERS = $(foreach dir, $(DIRS), $(wildcard $(dir)/*.h))
SOURCES = $(foreach dir, $(DIRS), $(wildcard $(dir)/*.c))

BIN_DIR = build
RULE_DIR = $(BIN_DIR)/rules
OBJ_DIR = $(BIN_DIR)/objs

STATIC_OBJS = $(subst src,$(OBJ_DIR)/static,$(SOURCES:.c=.o))

TEST_HEADERS = $(wildcard test/*.h)
TEST_SOURCES = $(wildcard test/*.c)

CC = gcc
# TODO: remove pie on libraries?
CFLAGS = -std=c17 -Wall -Wextra -Wpedantic -Werror=return-type \
	-Werror=implicit-function-declaration -Werror=incompatible-pointer-types \
	-Wformat-security -Wundef -Wshadow -Wcast-align -Wwrite-strings \
	-Wcast-qual -Wconversion \
	-Wl,-z,defs -fstack-clash-protection -fstack-protector-strong \
	-fasynchronous-unwind-tables \
	$(SANITIZER) $(DEBUGFLAGS) -D USE_SS_LIB_ASSERT -Iinclude

SANITIZER = -fsanitize=undefined -fsanitize=leak \
	-fsanitize-address-use-after-scope

DEBUGFLAGS = -g -grecord-gcc-switches -D SS_DEBUG
RELEASEFLAGS = -O2
TESTFLAGS = -D SS_LIB_RUN_TESTS -Itest

# Auto-generated dependencies
RULES = $(subst src,$(RULE_DIR),$(SOURCES:.c=.d))


all: static

static: $(BIN_DIR)/ss_utils.a

# TODO: shared library w/ -fPIC option

test: tests
	./$@s


$(BIN_DIR)/ss_utils.a: $(HEADERS) $(SOURCES) $(STATIC_OBJS)
	@mkdir -p $(@D)
	ar rcs $(BIN_DIR)/ss_utils.a $(STATIC_OBJS)

$(OBJ_DIR)/static/%.o: src/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(DEBUGFLAGS) -c $^ -o $@

$(RULE_DIR)/%.d: src/%.c
	@set -e; \
	mkdir -p $(@D); \
	rm -f $@; \
	$(CC) -MM $(CFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; rm -f $@.$$$$

include $(subst src,$(RULE_DIR),$(SOURCES:.c=.d))

tests: $(HEADERS) $(SOURCES) $(TEST_HEADERS) $(TEST_SOURCES)
	$(CC) $(CFLAGS) $(DEBUGFLAGS) $(TESTFLAGS) $^ -o $@

clean:
	rm -f $(OBJ_DIR)/static/*
	rm -f $(OBJ_DIR)/shared/*
	rm -f $(RULE_DIR)/*
	rm -f $(BIN_DIR)/ss_utils.a
