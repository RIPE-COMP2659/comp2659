# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -std=c99

# Include directories
# -I. means include from current directory (for helloworld.h)
# -Itests/unity means include from tests/unity subdirectory (for unity.h)
INCLUDES = -I. -Itests/unity

# Source files
SRC = helloworld.c
TEST_SRC = tests/testhelloworld.c
UNITY_FILES = tests/unity/unity.c

# Output executable
TEST_EXEC = testhelloworld.exe

# Default target
all: test

# Compile and run tests
test: $(TEST_EXEC)
	./$(TEST_EXEC)

# Build the test executable
$(TEST_EXEC): $(TEST_SRC) $(SRC) $(UNITY_FILES)
	$(CC) $(CFLAGS) $(INCLUDES) -DTESTING $(TEST_SRC) $(SRC) $(UNITY_FILES) -o $(TEST_EXEC)

# Build the main program
helloworld: helloworld.c
	$(CC) $(CFLAGS) helloworld.c -o helloworld.exe

# Clean up build files
clean:
	-del /Q *.exe *.o

.PHONY: all test clean
