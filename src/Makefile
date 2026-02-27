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
# -Ientities means include from entities subdirectory (for entity headers)
INCLUDES = -I. -Itests/unity -Ientities

# Source files - automatically discover all .c files in src directory (excluding tests/)
SRC = $(filter-out tests/%, $(wildcard *.c entities/*.c))
SRC_BASENAMES = $(basename $(SRC))
UNITY_FILES = tests/unity/unity.c
TEST_SUPPORT_SRC = $(filter-out main.c,$(SRC))

# Dynamically discover test files (all .c files in tests/entities/)
TEST_SRC_FILES = $(wildcard tests/entities/*.c)
TEST_EXECS = $(TEST_SRC_FILES:tests/entities/%.c=%$(EXE_EXT))

# Main program sources
MAIN_EXE = main$(EXE_EXT)
MAIN_SRC = main.c entities/block.c entities/spike.c entities/lava.c entities/geo.c entities/level.c entities/world.c

# Default target
all: main

# Default command prefix for running tests (can be overridden)
RUN_CMD = $(RUN_PREFIX)

# Compile and run all tests
test: $(TEST_EXECS)
	$(foreach exec,$(TEST_EXECS),$(RUN_CMD)$(exec) && ) $(NOOP)

# Build test executables (pattern rule)
# Each test compiles with its corresponding source file: test_NAME.c compiles with NAME.c
#%$(EXE_EXT): tests/entities/%.c $(UNITY_FILES) $(TEST_SUPPORT_SRC)
%$(EXE_EXT): tests/entities/%.c $(UNITY_FILES) $(TEST_SUPPORT_SRC)
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
