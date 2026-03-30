# Detect OS for cross-platform compatibility
ifeq ($(OS),Windows_NT)
    EXE_EXT = .exe
    RM = del /Q
    RUN_PREFIX =
    NOOP = exit /b 0
else
    EXE_EXT =
    RM = rm -f
    RUN_PREFIX = ./
    NOOP = true
endif

CC = gcc
GCOV = gcov

# Compiler flags
CFLAGS = -Wall -Wextra -std=c89

# Coverage flags for gcov
# -fprofile-arcs: Generate arc coverage data
# -ftest-coverage: Generate .gcno notes files
COVERAGE_FLAGS = -fprofile-arcs -ftest-coverage -O0

# Include directories
# -I. means include from current directory (for helloworld.h)
# -Itests/unity means include from tests/unity subdirectory (for unity.h)
# -Imodel means include from model subdirectory (for entity headers)
# -Ishared means include from shared subdirectory (for shared constants)
# -Ievents means include from events subdirectory (for event headers)
# -Ios_wrap means include from os_wrap subdirectory (for oswrap headers)
OSWRAP_FOLDER = os_wrap
INCLUDES = -I. -Itests/unity -Imodel -Ishared -Ievents -I$(OSWRAP_FOLDER)

# Source files - automatically discover all .c files in src directory (excluding tests/)
SRC = $(filter-out tests/%, $(wildcard *.c model/*.c events/*.c))
SRC_BASENAMES = $(basename $(SRC))
UNITY_FILES = tests/unity/unity.c
TEST_SUPPORT_SRC = $(filter-out dash.c,$(SRC))

# Dynamically discover test files (all .c files in tests/model/ and tests/events/)
TEST_MODEL_FILES = $(wildcard tests/model/*.c)
TEST_EVENTS_FILES = $(wildcard tests/events/*.c)
TEST_MODEL_EXECS = $(TEST_MODEL_FILES:tests/model/%.c=%$(EXE_EXT))
TEST_EVENTS_EXECS = $(TEST_EVENTS_FILES:tests/events/%.c=%$(EXE_EXT))
TEST_EXECS = $(TEST_MODEL_EXECS) $(TEST_EVENTS_EXECS)

# Main program sources
MAIN_EXE = dash$(EXE_EXT)
MAIN_SRC = dash.c model/clock.c os_wrap/oswrap.c model/model.c model/block.c model/spike.c model/lava.c model/geo.c model/camera.c model/level.c model/world.c events/async.c events/cond.c events/sync.c render/render.c

# Default target
all: main

# Default command prefix for running tests (can be overridden)
RUN_CMD = $(RUN_PREFIX)

# Compile and run all tests
test: $(TEST_EXECS)
	$(foreach exec,$(TEST_EXECS),$(RUN_CMD)$(exec) && ) $(NOOP)

# Build test executables (pattern rules)
# Each test compiles with its corresponding source file: test_NAME.c compiles with NAME.c
%$(EXE_EXT): tests/model/%.c $(UNITY_FILES) $(TEST_SUPPORT_SRC)
	$(CC) $(CFLAGS) $(COVERAGE_FLAGS) $(INCLUDES) -DTESTING $< $(TEST_SUPPORT_SRC) $(UNITY_FILES) -o $@

%$(EXE_EXT): tests/events/%.c $(UNITY_FILES) $(TEST_SUPPORT_SRC)
	$(CC) $(CFLAGS) $(COVERAGE_FLAGS) $(INCLUDES) -DTESTING $< $(TEST_SUPPORT_SRC) $(UNITY_FILES) -o $@

# Build the main program
main: clean $(MAIN_EXE)

$(MAIN_EXE): $(MAIN_SRC)
	$(CC) $(CFLAGS) $(INCLUDES) $(MAIN_SRC) -o $(MAIN_EXE)

# Clean up unncessary files
coverage: generate-coverage clean-coverage

# Generate coverage report using gcov for all test executables
# Each test only reports coverage for its corresponding source file
generate-coverage: clean test
	$(foreach exec,$(TEST_EXECS:$(EXE_EXT)=),$(GCOV) $(exec)-$(patsubst test_%,%,$(exec)).gcno && ) $(NOOP)
	@echo Coverage report generated. Check *.gcov files for details.

# Clean only coverage files (used before coverage generation)
clean-coverage:
	-$(RM) *.gcda *.gcno

# Clean up build files
clean:
	-$(RM) *.exe *.o *.gcda *.gcno *.gcov *.ut *.tos $(TEST_EXECS)

# Run tests with Valgrind memory checking
valgrind: $(TEST_EXECS)
	$(MAKE) test RUN_CMD="valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes $(RUN_PREFIX)"

# Commands to run tests, otherwise make may assume these are files.
.PHONY: all main test clean clean-coverage coverage generate-coverage valgrind
