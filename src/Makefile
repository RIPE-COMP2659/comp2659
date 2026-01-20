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
CFLAGS = -Wall -Wextra -std=c99

# Coverage flags for gcov
# -fprofile-arcs: Generate arc coverage data
# -ftest-coverage: Generate .gcno notes files
COVERAGE_FLAGS = -fprofile-arcs -ftest-coverage -O0

# Include directories
# -I. means include from current directory (for helloworld.h)
# -Itests/unity means include from tests/unity subdirectory (for unity.h)
INCLUDES = -I. -Itests/unity

# Source files
SRC = helloworld.c
UNITY_FILES = tests/unity/unity.c

# Dynamically discover test files (all .c files in tests/ excluding unity/)
TEST_SRC_FILES = $(filter-out tests/unity/%, $(wildcard tests/*.c))
TEST_EXECS = $(TEST_SRC_FILES:tests/%.c=%$(EXE_EXT))

# Default target
all: test

# Compile and run all tests
test: $(TEST_EXECS)
	$(foreach exec,$(TEST_EXECS),$(RUN_PREFIX)$(exec) && ) $(NOOP)

# Build test executables (pattern rule)
%$(EXE_EXT): tests/%.c $(SRC) $(UNITY_FILES)
	$(CC) $(CFLAGS) $(COVERAGE_FLAGS) $(INCLUDES) -DTESTING $< $(SRC) $(UNITY_FILES) -o $@

# Build the main program
helloworld: helloworld.c
	$(CC) $(CFLAGS) helloworld.c -o helloworld$(EXE_EXT)

# Clean up unncessary files
coverage: generate-coverage clean-coverage

# Generate coverage report using gcov for all test executables
generate-coverage: clean test
	$(foreach exec,$(TEST_EXECS:$(EXE_EXT)=),$(GCOV) $(exec)-helloworld.gcno && ) $(NOOP)
	@echo Coverage report generated. Check *.gcov files for details.

# Clean only coverage files (used before coverage generation)
clean-coverage:
	-$(RM) *.gcda *.gcno

# Clean up build files
clean:
	-$(RM) *.exe *.o *.gcda *.gcno *.gcov $(TEST_EXECS)

# Commands to run tests, otherwise make may assume these are files.
.PHONY: all test clean clean-coverage coverage generate-coverage
