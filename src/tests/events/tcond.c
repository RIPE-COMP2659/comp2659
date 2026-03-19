#include "unity.h"
#include "cond.h"

Model test_model;

void setUp(void) {
    test_model = get_model();
    /* Set up a test level with specific end_x value */
    test_model.world.levels[0].end_x = 500; /* Test level ends at x=500 */
    test_model.world.level_index = 0;
}

void tearDown(void) {
}

void test_check_level_complete_returns_none_when_not_at_end(void) {
    signed int result;

    /* Geo is before the end of the level */
    test_model.world.geo.x = 400;

    result = check_level_complete(&test_model);

    TEST_ASSERT_EQUAL_INT(EVENT_NONE, result);
}

void test_check_level_complete_returns_level_done_when_at_end(void) {
    signed int result;

    /* Geo is exactly at the end of the level */
    test_model.world.geo.x = 500;

    result = check_level_complete(&test_model);

    TEST_ASSERT_EQUAL_INT(EVENT_LEVEL_DONE, result);
}

void test_check_level_complete_returns_level_done_when_past_end(void) {
    signed int result;

    /* Geo is past the end of the level */
    test_model.world.geo.x = 600;

    result = check_level_complete(&test_model);

    TEST_ASSERT_EQUAL_INT(EVENT_LEVEL_DONE, result);
}

void test_check_level_complete_uses_current_level_index(void) {
    signed int result;

    /* Set up a second level with different end_x */
    test_model.world.levels[1].end_x = 800;
    test_model.world.level_index = 1; /* Use second level */
    test_model.world.geo.x = 600; /* Past first level end, but before second */

    result = check_level_complete(&test_model);

    TEST_ASSERT_EQUAL_INT(EVENT_NONE, result);
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_check_level_complete_returns_none_when_not_at_end);
    RUN_TEST(test_check_level_complete_returns_level_done_when_at_end);
    RUN_TEST(test_check_level_complete_returns_level_done_when_past_end);
    RUN_TEST(test_check_level_complete_uses_current_level_index);

    return UNITY_END();
}