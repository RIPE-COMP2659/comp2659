#include "unity.h"
#include "sync.h"

Model test_model;
unsigned int original_geo_x;
signed int original_geo_y;

void setUp(void) {
    test_model = get_model();
    /* Set geo to a predictable state for testing model_update */
    test_model.world.geo.x = 100;
    test_model.world.geo.y = 100;
    test_model.world.geo.dy = -5; /* Give geo some upward velocity */
    test_model.world.geo.is_landed = FALSE;
    
    original_geo_x = test_model.world.geo.x;
    original_geo_y = test_model.world.geo.y;
}

void tearDown(void) {
}

void test_on_clock_tick_calls_model_update_changes_geo_position(void) {
    /* Call on_clock_tick which should delegate to model_update */
    on_clock_tick(&test_model);
    
    /* Verify that model_update was called by checking if geo moved */
    /* model_update should call world_update which calls geo_update */
    /* geo_update should increment x by dx (default 2) and modify y by dy */
    TEST_ASSERT_NOT_EQUAL(original_geo_x, test_model.world.geo.x);
    TEST_ASSERT_NOT_EQUAL(original_geo_y, test_model.world.geo.y);
}

void test_on_clock_tick_advances_geo_x_by_dx(void) {
    /* Verify specific behavior - geo should move by dx (typically 2) */
    on_clock_tick(&test_model);
    
    /* Check that x increased (geo moves right) */
    TEST_ASSERT_GREATER_THAN(original_geo_x, test_model.world.geo.x);
}

void test_on_clock_tick_applies_gravity_to_geo_y(void) {
    /* With negative dy (upward velocity) and gravity, y should change */
    on_clock_tick(&test_model);
    
    /* Y should have changed due to dy and gravity effects */
    TEST_ASSERT_NOT_EQUAL(original_geo_y, test_model.world.geo.y);
}

void test_on_clock_tick_multiple_calls_continue_updating(void) {
    unsigned int x_after_first_tick;
    
    /* First tick */
    on_clock_tick(&test_model);
    x_after_first_tick = test_model.world.geo.x;
    
    /* Second tick should continue moving */
    on_clock_tick(&test_model);
    
    /* Geo should continue moving right */
    TEST_ASSERT_GREATER_THAN(x_after_first_tick, test_model.world.geo.x);
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_on_clock_tick_calls_model_update_changes_geo_position);
    RUN_TEST(test_on_clock_tick_advances_geo_x_by_dx);
    RUN_TEST(test_on_clock_tick_applies_gravity_to_geo_y);
    RUN_TEST(test_on_clock_tick_multiple_calls_continue_updating);

    return UNITY_END();
}