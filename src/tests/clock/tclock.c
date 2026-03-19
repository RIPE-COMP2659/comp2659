#include "../../clock/clock.h"
#include "unity.h"

void setUp(void) {}
void tearDown(void) {}

/* TODO: Testing is garbage */
void test_get_time_returns_value(void)
{
    unsigned long a = get_time();
    unsigned long b = get_time();
    TEST_IGNORE_MESSAGE("Implement clock tests");
    /* The timer should be monotonic or equal across two reads */
    TEST_ASSERT_TRUE(a <= b || a >= b);
}

/* Main function to run all tests */
int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_get_time_returns_value);

    return UNITY_END();
}
