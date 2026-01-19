# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -std=c99

# Include directories
# -I. means include from current directory (for helloworld.h)
# -Iunity means include from unity subdirectory (for unity.h)
INCLUDES = -I. -Iunity

# Source files
SRC = helloworld.c
TEST_SRC = testhelloworld.c
UNITY_FILES = unity/unity.c

# Output executable
TEST_EXEC = testhelloworld

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
	rm -f $(TEST_EXEC) helloworld.exe *.o

.PHONY: all test clean
