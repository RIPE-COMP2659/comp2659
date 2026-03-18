#include "unity.h"
#include "geo.h"

Geo geo;
const signed int INITIAL_X = 64;
const signed int INITIAL_Y = 32;
const signed int INITIAL_GROUND_Y = 32;

/* TODO: Add tests for update_landed */
/* TODO: geo_check_spike_collision tests missing */

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
    geo.y_scaled = geo.y << GEO_PHYSICS_SHIFT;

    geo_update(&geo);
    TEST_ASSERT_EQUAL_INT(current_dy + current_ddy, geo.dy);
    current_dy = geo.dy;

    geo_update(&geo);
    TEST_ASSERT_EQUAL_INT(current_dy + current_ddy, geo.dy);
}

void test_geo_move_decreases_y_by_constant_ddy(void) {
    signed int current_dy = geo.dy;
    signed int current_ddy = geo.ddy;
    unsigned int current_y_scaled;
    /* Need to make sure geo is above the ground */
    unsigned int offset = current_dy >= 0 ? current_dy * 3 : (-current_dy) * 3;

    geo.y = geo.ground_y + geo.size + offset;
    geo.y_scaled = geo.y << GEO_PHYSICS_SHIFT;
    current_y_scaled = geo.y_scaled;

    current_dy += current_ddy;
    current_y_scaled += current_dy;
    geo_update(&geo);
    TEST_ASSERT_EQUAL_INT(current_dy, geo.dy);
    TEST_ASSERT_EQUAL_INT(current_y_scaled >> GEO_PHYSICS_SHIFT, geo.y);

    current_dy += current_ddy;
    current_y_scaled += current_dy;
    geo_update(&geo);
    TEST_ASSERT_EQUAL_INT(current_dy, geo.dy);
    TEST_ASSERT_EQUAL_INT(current_y_scaled >> GEO_PHYSICS_SHIFT, geo.y);
}

/* TODO: Add test to make sure can't jump if not landed */
void test_geo_jump_sets_dy_to_constant(void) {
    geo.dy = -20; /* Set to something other than the jump value to make sure it's being set, not added to */
    geo_update(&geo); /* Move to update landed status based on initial position and ground_y */
    geo_jump(&geo);
    TEST_ASSERT_EQUAL_INT(GEO_JUMP_DY_SCALED, geo.dy);
}

void test_geo_update_landed_false_when_above_ground(void) {
    geo.y = geo.ground_y + geo.size + 10;
    geo.y_scaled = geo.y << GEO_PHYSICS_SHIFT;
    geo.is_landed = TRUE;

    geo_update_landed(&geo);

    TEST_ASSERT_EQUAL_INT(FALSE, geo.is_landed);
    TEST_ASSERT_EQUAL_INT(geo.ground_y + geo.size + 10, geo.y);
}

void test_geo_update_landed_true_when_at_ground(void) {
    geo.y = geo.ground_y + geo.size;
    geo.y_scaled = geo.y << GEO_PHYSICS_SHIFT;
    geo.is_landed = FALSE;

    geo_update_landed(&geo);

    TEST_ASSERT_EQUAL_INT(TRUE, geo.is_landed);
}

void test_geo_update_landed_true_when_below_ground(void) {
    geo.y = geo.ground_y + geo.size - 5;
    geo.y_scaled = geo.y << GEO_PHYSICS_SHIFT;
    geo.is_landed = FALSE;

    geo_update_landed(&geo);

    TEST_ASSERT_EQUAL_INT(TRUE, geo.is_landed);
    TEST_ASSERT_EQUAL_INT(geo.ground_y + geo.size, geo.y);
}

/* Validate jump apex and total air time are within tuning targets. */
void test_geo_jump_works_with_move_until_apex_and_back(void) {
    unsigned int start_y;
    unsigned int highest_y;
    unsigned int apex_height;
    unsigned int frame_count;
    unsigned int apex_frame;

    geo_update(&geo); /* Update landed status */
    geo_jump(&geo);

    start_y = geo.y;
    highest_y = geo.y;
    frame_count = 0;
    apex_frame = 0;

    do {
        geo_update(&geo);
        frame_count++;

        if (geo.y > highest_y) {
            highest_y = geo.y;
            apex_frame = frame_count;
        }

        TEST_ASSERT_LESS_THAN_UINT_MESSAGE(
            200,
            frame_count,
            "Jump went on for too long"
        );
    } while (geo.is_landed != TRUE);

    apex_height = highest_y - start_y;

    /* Jump height approximately two blocks */
    TEST_ASSERT_GREATER_OR_EQUAL_UINT(64, apex_height);
    TEST_ASSERT_LESS_OR_EQUAL_UINT(70, apex_height);

    /* Total air time approximately one second */
    TEST_ASSERT_GREATER_OR_EQUAL_UINT(65, frame_count);
    TEST_ASSERT_LESS_OR_EQUAL_UINT(75, frame_count);

    TEST_ASSERT_GREATER_OR_EQUAL_UINT(30, apex_frame);
    TEST_ASSERT_LESS_OR_EQUAL_UINT(40, apex_frame);
}

void test_geo_check_square_collision_no_collision(void) {
    signed int collision_result;
    unsigned int object_size = 32;
    geo.x = 100;
    geo.y = 100;
    geo.y_scaled = geo.y << GEO_PHYSICS_SHIFT;

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
    geo.y_scaled = geo.y << GEO_PHYSICS_SHIFT;
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
    geo.y_scaled = geo.y << GEO_PHYSICS_SHIFT;
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
    geo.y_scaled = geo.y << GEO_PHYSICS_SHIFT;

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
