# Unity Test Setup

Testing is done via Make, the Unity testing framework, and gcov. It has automated test discovery and code coverage support.

## Prerequisites

Ensure the following is installed:
- **GCC Compiler**: For compiling C code
- **Make**: For running the Makefile build system
  - Linux/Mac: Usually pre-installed
  - Windows: [Installation Guide](https://www.youtube.com/watch?v=5TavcolACQY&t=227s)
- **gcov**: For code coverage reports (typically included with GCC)

The easiest way to get going on Windows is to use WSL.

## Running Tests

Navigate to your source directory and run tests:

```bash
cd src
make test
# OR
make
```

The command will:
1. Automatically discover all test files in `tests/` directory
2. Compile each test with Unity and your source code
3. Run all tests and display results

## Code Coverage

```bash
make coverage
```
This creates `.gcov` files showing which lines of code were executed during testing. Look up the documentation for GCov, the official documentation isn't the best, but you should be able to figure out what is going on using other online resources.

## Cleanup
```bash
make clean               # Remove all build artifacts (executables, coverage files)
make clean-coverage      # Remove only coverage files (.gcda, .gcno, .gcov)
```

## How It Works

### Included Paths
The Makefile configures GCC to find headers in multiple locations:
- **-I.**: Looks in the current directory for your project headers (e.g., `helloworld.h`)
- **-Itests/unity**: Looks in the Unity framework directory for `unity.h`

This means your test files can cleanly include:
```c
#include "unity.h"
#include "helloworld.h"
```

### Test Discovery
The Makefile automatically finds and compiles all `.c` files in the `tests/` directory (excluding the `tests/unity/` subdirectory). Each test file is compiled into its own executable and run independently.

## Writing Tests

### Test File Structure
Create a new file in `tests/` directory (e.g., `tests/test_myfeature.c`):

```c
#include "unity.h"
#include "helloworld.h"

// Required setup function (runs before each test)
void setUp(void) {
    // Initialize test fixtures here
}

// Required teardown function (runs after each test)
void tearDown(void) {
    // Clean up after tests here
}

// Example test function
void test_addition_should_work(void) {
    TEST_ASSERT_EQUAL_INT(4, 2 + 2);
}

void test_myfunction_returns_expected_value(void) {
    int result = my_function(5);
    TEST_ASSERT_EQUAL_INT(10, result);
}

// Main function - required for each test file
int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_addition_should_work);
    RUN_TEST(test_myfunction_returns_expected_value);
    return UNITY_END();
}
```

Look into the [Unity documentation](https://github.com/ThrowTheSwitch/Unity) for complete information.

### Common Unity Assertions
```c
TEST_ASSERT_EQUAL_INT(expected, actual);
TEST_ASSERT_EQUAL(expected, actual);
TEST_ASSERT_TRUE(condition);
TEST_ASSERT_FALSE(condition);
TEST_ASSERT_NULL(pointer);
TEST_ASSERT_NOT_NULL(pointer);
TEST_ASSERT_EQUAL_STRING(expected, actual);
TEST_ASSERT_EQUAL_FLOAT(expected, actual, delta);
```

## Resources

- [Unity Testing Framework](https://github.com/ThrowTheSwitch/Unity)
- [Unity Documentation](https://github.com/ThrowTheSwitch/Unity/blob/master/docs/UnityGettingStartedGuide.md)
- [GCC Coverage Documentation](https://gcc.gnu.org/onlinedocs/gcc/Gcov.html)
