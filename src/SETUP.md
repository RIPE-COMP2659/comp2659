# Unity Test Setup

Parker doesn't think there are prerequisites, but we will run into issue and add them here as needed.

**TODO:** Update documentation for when new files are created.

**TODO:** Create a seperate directory for tests.

**TODO:** Get code coverage.

## Ensure that Make is installed

1. [Install for Windows](https://www.youtube.com/watch?v=5TavcolACQY&t=227s)

## How Run

```bash
cd src
make test
```

This compiles and runs all tests.

## Other Makefile Commands

```bash
make clean              # Remove compiled files
make helloworld.exe     # Build your main program
make help               # Show all available commands
```

## How It Works

1. **-I.** tells gcc to look in current directory for `helloworld.h`
2. **-Iunity** tells gcc to look in unity/ directory for `unity.h`
3. Your test includes work cleanly: `#include "unity.h"`

## Adding More Tests

Add a test function:
```c
void test_my_new_test(void) {
    TEST_ASSERT_EQUAL_INT(expected, actual);
}
```

Then add to main():
```c
int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_my_new_test);  // Add this line
    return UNITY_END();
}
```