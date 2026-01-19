# Detect OS for cross-platform compatibility
ifeq ($(OS),Windows_NT)
    EXE_EXT = .exe
    RM = del /Q
else
    EXE_EXT =
    RM = rm -f
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
TEST_SRC = tests/testhelloworld.c
UNITY_FILES = tests/unity/unity.c

# Output executable
TEST_EXEC = testhelloworld$(EXE_EXT)

# Default target
all: test

# Compile and run tests
test: $(TEST_EXEC)
	./$(TEST_EXEC)

# Build the test executable
$(TEST_EXEC): $(TEST_SRC) $(SRC) $(UNITY_FILES)
	$(CC) $(CFLAGS) $(COVERAGE_FLAGS) $(INCLUDES) -DTESTING $(TEST_SRC) $(SRC) $(UNITY_FILES) -o $(TEST_EXEC)

# Build the main program
helloworld: helloworld.c
	$(CC) $(CFLAGS) helloworld.c -o helloworld$(EXE_EXT)

# Clean up unncessary files
coverage: generate-coverage clean-coverage

# Generate coverage report using gcov
generate-coverage: clean test
	$(GCOV) $(TEST_EXEC:$(EXE_EXT)=)-helloworld.gcno
	@echo Coverage report generated. Check *.gcov files for details.

# Clean only coverage files (used before coverage generation)
clean-coverage:
	-$(RM) *.gcda *.gcno

# Clean up build files
clean:
	-$(RM) *.exe *.o *.gcda *.gcno *.gcov testhelloworld

# Commands to run tests, otherwise make may assume these are files.
.PHONY: all test clean clean-coverage coverage generate-coverage
