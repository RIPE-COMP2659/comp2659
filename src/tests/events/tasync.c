#include "unity.h"
#include "async.h"

Model test_model;

void setUp(void) {
    test_model = get_model();
    /* Place Geo on the ground so geo_update_landed keeps is_landed true. */
    test_model.world.geo.ground_y = test_model.world.ground_y;
    test_model.world.geo.y = test_model.world.geo.ground_y + test_model.world.geo.size;
    test_model.world.geo.y_scaled = test_model.world.geo.y << GEO_PHYSICS_SHIFT;
    test_model.world.geo.dy = 0;
    test_model.world.geo.is_landed = TRUE;
}

void tearDown(void) {
}

void test_on_jump_request_calls_geo_jump(void) {
    signed int expected_jump_dy = GEO_JUMP_DY_SCALED;

    on_jump_request(&test_model);
    model_update(&test_model); /* Update the model to process the jump buffer */

    TEST_ASSERT_EQUAL_INT(expected_jump_dy, test_model.world.geo.dy);
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_on_jump_request_calls_geo_jump);

    return UNITY_END();
}