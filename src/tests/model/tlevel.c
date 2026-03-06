#include "unity.h"
#include "level.h"

Level *levels;

/* Pre-test setup, runs for each test */
void setUp(void) {
    levels = get_levels();
}

/* Post-test teardown, runs for each test */
void tearDown(void) {
}

/* Unutilized test placeholder */
void test_level(void) {
    TEST_IGNORE_MESSAGE("Nothing to test");
}

/* Runs tests */
int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_level);

    return UNITY_END();
}
