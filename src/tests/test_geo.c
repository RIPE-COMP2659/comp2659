#include "unity.h"
#include "geo.h"

Geo geo;

void setUp(void) {
    geo = (Geo){ GEO_DDY, GEO_DX, -20, 100, 200, GEO_SIZE, GEO_SPRITE };
}

void tearDown(void) {
}

void test_geo_init(void) {
    TEST_ASSERT_EQUAL_INT(100, geo.x);
    TEST_ASSERT_EQUAL_INT(200, geo.y);
}

void test_geo_init_size(void) {
    TEST_ASSERT_EQUAL_INT(GEO_SIZE, geo.size);
}

void test_geo_init_sprite(void) {
    TEST_ASSERT_EQUAL_PTR(GEO_SPRITE, geo.sprite);
}

void test_geo_sprite_size(void) {
    int num_rows = sizeof(GEO_SPRITE) / sizeof(GEO_SPRITE[0]);
    TEST_ASSERT_EQUAL_INT(geo.size, num_rows);

    int bits_per_row = (GEO_SIZE / 16) * 16;
    TEST_ASSERT_EQUAL_INT(geo.size, bits_per_row);
}

void test_geo_move_increases_x_by_constant_dx(void) {
    unsigned int current_x = geo.x;

    geo_move(&geo);
    TEST_ASSERT_EQUAL_INT(current_x + GEO_DX, geo.x);
    current_x = geo.x;

    geo_move(&geo);
    TEST_ASSERT_EQUAL_INT(current_x + GEO_DX, geo.x);
}

void test_geo_move_decreases_dy_by_constant_ddy(void) {
    signed int current_dy = geo.dy;

    geo_move(&geo);
    TEST_ASSERT_EQUAL_INT(current_dy + GEO_DDY, geo.dy);
    current_dy = geo.dy;

    geo_move(&geo);
    TEST_ASSERT_EQUAL_INT(current_dy + GEO_DDY, geo.dy);
}

void test_geo_move_decreases_y_by_constant_ddy(void) {
    signed int current_y = geo.y;
    signed int current_dy = geo.dy;

    geo_move(&geo);
    TEST_ASSERT_EQUAL_INT(current_y + current_dy + GEO_DDY, geo.y);
    current_y = geo.y;
    current_dy = geo.dy;

    geo_move(&geo);
    TEST_ASSERT_EQUAL_INT(current_y + current_dy + GEO_DDY, geo.y);
}

void test_geo_jump_sets_dy_to_constant(void) {
    geo.dy = -100; /* Set to something other than the jump value to make sure it's being set, not added to */
    geo_jump(&geo);
    TEST_ASSERT_EQUAL_INT(GEO_JUMP_DY, geo.dy);
}

/* Make sure falling after jumping works properly, ideally to the point of an apex, using move */
void test_geo_jump_works_with_move_until_apex_and_back(void) {
    geo.y = 0;

    geo_jump(&geo);

    signed int current_y = geo.y;
    signed int current_dy = geo.dy;
    signed int iterations_to_apex = GEO_JUMP_DY / -GEO_DDY;

    for (int i = 0; i < iterations_to_apex; i++) {
        geo_move(&geo);

        TEST_ASSERT_EQUAL_INT(current_dy + GEO_DDY, geo.dy);
        TEST_ASSERT_EQUAL_INT(current_y + current_dy + GEO_DDY, geo.y);

        current_y = geo.y;
        current_dy = geo.dy;
    }

    TEST_ASSERT_EQUAL_INT(0, geo.dy);

    geo_move(&geo);
    TEST_ASSERT_EQUAL_INT(current_dy + GEO_DDY, geo.dy);
    TEST_ASSERT_EQUAL_INT(current_y + current_dy + GEO_DDY, geo.y);
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_geo_init);
    RUN_TEST(test_geo_init_size);
    RUN_TEST(test_geo_init_sprite);
    RUN_TEST(test_geo_sprite_size);
    RUN_TEST(test_geo_move_increases_x_by_constant_dx);
    RUN_TEST(test_geo_move_decreases_dy_by_constant_ddy);
    RUN_TEST(test_geo_move_decreases_y_by_constant_ddy);
    RUN_TEST(test_geo_jump_sets_dy_to_constant);
    RUN_TEST(test_geo_jump_works_with_move_until_apex_and_back);

    return UNITY_END();
}
