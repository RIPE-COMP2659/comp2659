#include "unity.h"
#include "async.h"

Model test_model;

void setUp(void) {
    test_model = get_model();
    test_model.world.geo.dy = -10; /* Set to a non-jump value */
    test_model.world.geo.is_landed = TRUE; /* Ensure geo can jump */
}

void tearDown(void) {
}

void test_on_jump_request_calls_geo_jump(void) {
    signed int expected_jump_dy = 10; /* Based on geo_jump implementation */
    
    on_jump_request(&test_model);
    
    TEST_ASSERT_EQUAL_INT(expected_jump_dy, test_model.world.geo.dy);
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_on_jump_request_calls_geo_jump);

    return UNITY_END();
}