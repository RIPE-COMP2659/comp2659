#include "unity.h"
#include "geo.h"

Geo geo;
const signed int INITIAL_X = 64;
const signed int INITIAL_Y = 32;
const signed int INITIAL_GROUND_Y = 32;

/* TODO: Add tests for update_landed */

void setUp(void) {
    geo = create_geo(INITIAL_X, INITIAL_Y, INITIAL_GROUND_Y);
    geo.dy = -20;
}

void tearDown(void) {
}

void test_geo_init(void) {
    TEST_ASSERT_EQUAL_INT(INITIAL_X, geo.x);
    TEST_ASSERT_EQUAL_INT(INITIAL_Y, geo.y);
    TEST_ASSERT_EQUAL_INT(FALSE, geo.is_landed);
}

void test_geo_init_size(void) {
    TEST_ASSERT_EQUAL_INT(GEO_SIZE, geo.size);
}

void test_geo_move_increases_x_by_constant_dx(void) {
    unsigned int current_x = geo.x;
    unsigned int current_dx = geo.dx;

    geo_update(&geo);
    TEST_ASSERT_EQUAL_INT(current_x + current_dx, geo.x);
    current_x = geo.x;

    geo_update(&geo);
    TEST_ASSERT_EQUAL_INT(current_x + current_dx, geo.x);
}

void test_geo_move_decreases_dy_by_constant_ddy(void) {
    signed int current_dy = geo.dy;
    signed int current_ddy = geo.ddy;
    /* Need to make sure geo is above the ground */
    unsigned int offset = current_dy >= 0 ? current_dy * 3 : (-current_dy) * 3;
    geo.y = geo.ground_y + geo.size + offset;

    geo_update(&geo);
    TEST_ASSERT_EQUAL_INT(current_dy + current_ddy, geo.dy);
    current_dy = geo.dy;

    geo_update(&geo);
    TEST_ASSERT_EQUAL_INT(current_dy + current_ddy, geo.dy);
}

void test_geo_move_decreases_y_by_constant_ddy(void) {
    signed int current_dy = geo.dy;
    signed int current_ddy = geo.ddy;
    unsigned int current_y;
    /* Need to make sure geo is above the ground */
    unsigned int offset = current_dy >= 0 ? current_dy * 3 : (-current_dy) * 3;

    geo.y = geo.ground_y + geo.size + offset;
    current_y = geo.y;

    geo_update(&geo);
    TEST_ASSERT_EQUAL_INT(current_y + current_dy + current_ddy, geo.y);
    current_y = geo.y;
    current_dy = geo.dy;

    geo_update(&geo);
    TEST_ASSERT_EQUAL_INT(current_y + current_dy + current_ddy, geo.y);
}

/* TODO: Add test to make sure can't jump if not landed */
void test_geo_jump_sets_dy_to_constant(void) {
    geo.dy = -20; /* Set to something other than the jump value to make sure it's being set, not added to */
    geo_update(&geo); /* Move to update landed status based on initial position and ground_y */
    geo_jump(&geo);
    /* NOTE: This will need to be adjusted if the jump value changes */
    TEST_ASSERT_EQUAL_INT(10, geo.dy);
}

void test_geo_update_landed_false_when_above_ground(void) {
    geo.y = geo.ground_y + geo.size + 10;
    geo.is_landed = TRUE;

    geo_update_landed(&geo);

    TEST_ASSERT_EQUAL_INT(FALSE, geo.is_landed);
    TEST_ASSERT_EQUAL_INT(geo.ground_y + geo.size + 10, geo.y);
}

void test_geo_update_landed_true_when_at_ground(void) {
    geo.y = geo.ground_y + geo.size;
    geo.is_landed = FALSE;

    geo_update_landed(&geo);

    TEST_ASSERT_EQUAL_INT(TRUE, geo.is_landed);
}

void test_geo_update_landed_true_when_below_ground(void) {
    geo.y = geo.ground_y + geo.size - 5;
    geo.is_landed = FALSE;

    geo_update_landed(&geo);

    TEST_ASSERT_EQUAL_INT(TRUE, geo.is_landed);
    TEST_ASSERT_EQUAL_INT(geo.ground_y + geo.size, geo.y);
}

/* Make sure falling after jumping works properly, ideally to the point of an apex, using move */
void test_geo_jump_works_with_move_until_apex_and_back(void) {
    signed int current_y;
    signed int current_dy;
    signed int current_ddy = geo.ddy;
    unsigned int iterations_to_apex;
    unsigned int i;

    geo_update(&geo); /* Move to update landed status based on initial position and ground_y */
    geo_jump(&geo);

    current_y = geo.y;
    current_dy = geo.dy;
    /* NOTE: This will need to be adjust if jump dy changes */
    iterations_to_apex = 10 / (-1 * current_ddy);

    for (i = 0; i < iterations_to_apex; i++) {
        geo_update(&geo);

        TEST_ASSERT_EQUAL_INT(current_dy + current_ddy, geo.dy);
        TEST_ASSERT_EQUAL_INT(current_y + current_dy + current_ddy, geo.y);

        current_y = geo.y;
        current_dy = geo.dy;
    }

    TEST_ASSERT_EQUAL_INT(0, geo.dy);

    geo_update(&geo);
    TEST_ASSERT_EQUAL_INT(current_dy + current_ddy, geo.dy);
    TEST_ASSERT_EQUAL_INT(current_y + current_dy + current_ddy, geo.y);
}

void test_geo_check_square_collision_no_collision(void) {
    signed int collision_result;
    unsigned int object_size = 32;
    geo.x = 100;
    geo.y = 100;

    /* No overlap, square to the right */
    collision_result = geo_check_square_collision(&geo, geo.x + geo.size + object_size, geo.y, object_size);
    TEST_ASSERT_EQUAL_INT(COLLISION_NONE, collision_result);

    /* No overlap, square to the left */
    collision_result = geo_check_square_collision(&geo, geo.x - (object_size + object_size), geo.y, object_size);
    TEST_ASSERT_EQUAL_INT(COLLISION_NONE, collision_result);

    /* No overlap, square above */
    collision_result = geo_check_square_collision(&geo, geo.x, geo.y + object_size + object_size, object_size);
    TEST_ASSERT_EQUAL_INT(COLLISION_NONE, collision_result);

    /* No overlap, square below */
    collision_result = geo_check_square_collision(&geo, geo.x, geo.y - (object_size + object_size), object_size);
    TEST_ASSERT_EQUAL_INT(COLLISION_NONE, collision_result);
}

void test_geo_check_square_collision_top_collision(void) {
    signed int collision_result;
    unsigned int object_size = 32;
    unsigned int big_offset = object_size / 2;
    unsigned int small_offset = object_size / 4;
    geo.x = 100;
    geo.y = 100;
    geo.dy = -1;

    /* Object below geo and to the right */
    collision_result = geo_check_square_collision(&geo, geo.x + big_offset, geo.y - geo.size + small_offset, object_size);
    TEST_ASSERT_EQUAL_INT(COLLISION_TOP, collision_result);

    /* Object directly below geo with */
    collision_result = geo_check_square_collision(&geo, geo.x, geo.y - geo.size + small_offset, object_size);
    TEST_ASSERT_EQUAL_INT(COLLISION_TOP, collision_result);

    /* Object below geo and to the left */
    collision_result = geo_check_square_collision(&geo, geo.x - big_offset, geo.y - geo.size + small_offset, object_size);
    TEST_ASSERT_EQUAL_INT(COLLISION_TOP, collision_result);
}

void test_geo_check_square_collision_bottom_collision(void) {
    signed int collision_result;
    unsigned int object_size = 32;
    unsigned int big_offset = object_size / 2;
    unsigned int small_offset = object_size / 4;
    geo.x = 100;
    geo.y = 100;
    geo.dy = 1;

    /* Object above geo and to the right */
    collision_result = geo_check_square_collision(&geo, geo.x + big_offset, geo.y + object_size - small_offset, object_size);
    TEST_ASSERT_EQUAL_INT(COLLISION_BOTTOM, collision_result);

    /* Object directly above geo */
    collision_result = geo_check_square_collision(&geo, geo.x, geo.y + object_size - small_offset, object_size);
    TEST_ASSERT_EQUAL_INT(COLLISION_BOTTOM, collision_result);

    /* Object above geo and to the left */
    collision_result = geo_check_square_collision(&geo, geo.x - big_offset, geo.y + object_size - small_offset, object_size);
    TEST_ASSERT_EQUAL_INT(COLLISION_BOTTOM, collision_result);
}

void test_geo_check_square_collision_left_collision(void) {
    signed int collision_result;
    unsigned int object_size = 32;
    unsigned int small_offset = object_size / 4;
    geo.x = 100;
    geo.y = 100;

    /* Object to the right and geo slightly above  */
    collision_result = geo_check_square_collision(&geo, geo.x + geo.size - small_offset, geo.y - small_offset, object_size);
    TEST_ASSERT_EQUAL_INT(COLLISION_LEFT, collision_result);

    /* Object directly to the right of geo */
    collision_result = geo_check_square_collision(&geo, geo.x + geo.size - small_offset, geo.y, object_size);
    TEST_ASSERT_EQUAL_INT(COLLISION_LEFT, collision_result);

    /* Object to the right and geo slightly below */
    collision_result = geo_check_square_collision(&geo, geo.x + geo.size - small_offset, geo.y + small_offset, object_size);
    TEST_ASSERT_EQUAL_INT(COLLISION_LEFT, collision_result);
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_geo_init);
    RUN_TEST(test_geo_init_size);
    RUN_TEST(test_geo_move_increases_x_by_constant_dx);
    RUN_TEST(test_geo_move_decreases_dy_by_constant_ddy);
    RUN_TEST(test_geo_move_decreases_y_by_constant_ddy);
    RUN_TEST(test_geo_jump_sets_dy_to_constant);
    RUN_TEST(test_geo_update_landed_false_when_above_ground);
    RUN_TEST(test_geo_update_landed_true_when_at_ground);
    RUN_TEST(test_geo_update_landed_true_when_below_ground);
    RUN_TEST(test_geo_jump_works_with_move_until_apex_and_back);
    RUN_TEST(test_geo_check_square_collision_no_collision);
    RUN_TEST(test_geo_check_square_collision_top_collision);
    RUN_TEST(test_geo_check_square_collision_bottom_collision);
    RUN_TEST(test_geo_check_square_collision_left_collision);

    return UNITY_END();
}
