# CS2002 Makefile for 'Shadow Breaker' practical.
# This file can be used to make the following executables:
#   * shadowbreaker - your program
#   * timing - a program to demonstrate 'time' on the machine
#   * create_examples - to generate input for your program.
# Relevant make directives include:
# > make all  -- makes all of above
# > make release -- same as 'all'
# > make debug   -- 'all' with debug info for debugger
# > make clean   -- deletes all the object files and dependencies,
#                   but not the executables!


# IGNORE 'tsan' lines since not installed on lab machines
# use gcc with -fsanitize=thread b/c clang was not tested
# this means we also need -ltsan in the link step. 
# libtsan must be installed

OBJS_DIR = .objs

# define all the student executables
EX_SHADOW_BREAKER=shadow_breaker
EX_SHADOW_ALL=$(EX_SHADOW_BREAKER) 

# list object file dependencies for each
OBJS_ALL=libs/utils.o libs/output_helpers.o libs/queue_r.o
OBJS_SHADOW_BREAKER=$(OBJS_ALL) $(EX_SHADOW_BREAKER)_main.o $(EX_SHADOW_BREAKER).o

# define the provided code executables and deps
EX_EXMPLS=create_examples
OBJS_EXMPLS=tools/$(EX_EXMPLS).o
EXES_PROVIDED=$(EX_EXMPLS)

# optional things
EX_TIMING=timing
OBJS_TIMING=tools/$(EX_TIMING).o libs/utils.o
EXES_OPTIONAL=$(EX_TIMING)

# set up compiler
CC = gcc
WARNINGS = -Wall -Wextra -Wno-error=format-truncation -Wno-error=unused-parameter
INC=-Ilibs/
CFLAGS_COMMON = $(WARNINGS) $(INC) -std=c99 -c -MMD -MP -D_GNU_SOURCE -pthread
CFLAGS_RELEASE = $(CFLAGS_COMMON) -O2
CFLAGS_DEBUG = $(CFLAGS_COMMON) -O0 -g -DDEBUG

# tsan needs some funky flags
CFLAGS_TSAN    = $(CFLAGS_DEBUG)
CFLAGS_TSAN    += -fsanitize=thread -DSANITIZE_THREADS -fPIC

# TSAN without the debug flag
CFLAGS_TSAN = $(CFLAGS_TSAN_DEBUG) -UDEBUG

# set up linker
# LD = gcc
# LDFLAGS = -lcrypt -lrt -pthread -Llibs/ -lprovided -lm
# LDFLAGS_TSAN = $(LDFLAGS) -ltsan
# set up linker
LD = gcc
PROVIDED_LIBRARIES:=$(shell find libs -type f -name '*.a' 2>/dev/null)
PROVIDED_LIBRARIES:=$(PROVIDED_LIBRARIES:libs/lib%.a=%)
LDFLAGS = -Llibs/ $(foreach lib,$(PROVIDED_LIBRARIES),-l$(lib)) -lm
LDFLAGS += -lcrypt -lrt -pthread
LDFLAGS += -pthread
LDFLAGS_TSAN = -ltsan $(LDFLAGS)

.PHONY: all
all: release

# build types
# run clean before building debug so that all 
# of the release executables disappear
.PHONY: debug
.PHONY: release
.PHONY: tsan

release: $(EX_SHADOW_ALL) $(EXES_PROVIDED)
debug:   clean $(EX_SHADOW_ALL:%=%-debug) $(EXES_PROVIDED)
debug-tsan:    clean $(EXES_STUDENT:%=%-debug-tsan) $(EXES_PROVIDED)
tsan:    clean $(EX_SHADOW_ALL:%=%-tsan) $(EXES_PROVIDED)

# include dependencies
-include $(OBJS_DIR)/*.d

$(OBJS_DIR):
	@mkdir -p $(OBJS_DIR)

# patterns to create objects
# keep the debug and release postfix for object files 
# so that we can always separate them correctly
$(OBJS_DIR)/%-debug.o: %.c | $(OBJS_DIR)
	@mkdir -p $(basename $@)
	$(CC) $(CFLAGS_DEBUG) $< -o $@

$(OBJS_DIR)/%-debug-tsan.o: %.c | $(OBJS_DIR)
	@mkdir -p $(basename $@)
	$(CC) $(CFLAGS_TSAN_DEBUG) $< -o $@

$(OBJS_DIR)/%-tsan.o: %.c | $(OBJS_DIR)
	@mkdir -p $(basename $@)
	$(CC) $(CFLAGS_TSAN) $< -o $@

$(OBJS_DIR)/%-release.o: %.c | $(OBJS_DIR)
	@mkdir -p $(basename $@)
	$(CC) $(CFLAGS_RELEASE) $< -o $@

# executables
# needs a quad of exe and exe-debug and exe-tsan and exe-tsan-debug
# for each executable (other than provided executables)
$(EX_SHADOW_BREAKER): $(OBJS_SHADOW_BREAKER:%.o=$(OBJS_DIR)/%-release.o)
	$(LD) $^ $(LDFLAGS) -o $@

$(EX_SHADOW_BREAKER)-debug: $(OBJS_SHADOW_BREAKER:%.o=$(OBJS_DIR)/%-debug.o)
	$(LD) $^ $(LDFLAGS) -o $@

$(EX_SHADOW_BREAKER)-debug-tsan: $(OBJS_SHADOW_BREAKER:%.o=$(OBJS_DIR)/%-debug-tsan.o)
	$(LD) $^ $(LDFLAGS_TSAN) -o $@

$(EX_SHADOW_BREAKER)-tsan: $(OBJS_SHADOW_BREAKER:%.o=$(OBJS_DIR)/%-tsan.o)
	$(LD) $^ $(LDFLAGS_TSAN) -o $@

$(EX_EXMPLS): $(OBJS_EXMPLS:%.o=$(OBJS_DIR)/%-release.o)
	$(LD) $^ $(LDFLAGS) -o $@

$(EX_TIMING): $(OBJS_TIMING:%.o=$(OBJS_DIR)/%-release.o)
	$(LD) $^ $(LDFLAGS) -o $@

.PHONY: clean
clean:
	-rm -rf .objs $(EX_SHADOW_ALL)\
	$(EX_SHADOW_ALL:%=%-tsan)\
	$(EX_SHADOW_ALL:%=%-debug)\
	$(EX_SHADOW_ALL:%=%-debug-tsan)\
	$(EXES_PROVIDED)\
	$(EXES_OPTIONAL)
