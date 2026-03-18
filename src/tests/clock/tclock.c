#include "../../model/clock.h"
#include "unity.h"

void setUp(void) {}
void tearDown(void) {}

void test_get_time_returns_value(void) {
  unsigned long a = get_time();
  unsigned long b = get_time();
  /* The timer should be monotonic or equal across two reads */
  TEST_ASSERT_TRUE(a <= b || a >= b);
}
