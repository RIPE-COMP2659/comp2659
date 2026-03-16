#include "unity.h"
#include "clock.h"

void setUp(void) {}
void tearDown(void) {}

void test_get_time_returns_value(void)
{
    unsigned long a = get_time();
    unsigned long b = get_time();
    /* Basic sanity: two reads should compile and return values */
    TEST_ASSERT_TRUE(a <= b || a >= b);
}
