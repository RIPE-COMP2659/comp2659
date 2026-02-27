#include "unity.h"
#include "camera.h"

/* Test cameras */
Camera camera;

/* Setup cameras for testing */
void setUp(void) {
    /* Nothing to setup */
}

void tearDown(void) {
    /* clean stuff up here */
}

/* Test camera creation with zero position */
void test_camera_create_zero(void) {
    camera = create_camera(0, 0);
    TEST_ASSERT_EQUAL_UINT(0, camera.x);
    TEST_ASSERT_EQUAL_UINT(0, camera.y);
    TEST_ASSERT_EQUAL_UINT(SCREEN_WIDTH, camera.width);
    TEST_ASSERT_EQUAL_UINT(SCREEN_HEIGHT, camera.height);
}

/* Test camera creation with non-zero position */
void test_camera_create_normal(void) {
    camera = create_camera(100, 200);
    TEST_ASSERT_EQUAL_UINT(100, camera.x);
    TEST_ASSERT_EQUAL_UINT(200, camera.y);
    TEST_ASSERT_EQUAL_UINT(SCREEN_WIDTH, camera.width);
    TEST_ASSERT_EQUAL_UINT(SCREEN_HEIGHT, camera.height);
}

/* Test camera update coordinates */
void test_camera_update_coordinates(void) {
    camera = create_camera(0, 0);
    camera_update_coordinates(&camera, 50, 75);
    TEST_ASSERT_EQUAL_UINT(50, camera.x);
    TEST_ASSERT_EQUAL_UINT(75, camera.y);
}

/* Test camera update to zero */
void test_camera_update_to_zero(void) {
    camera = create_camera(100, 200);
    camera_update_coordinates(&camera, 0, 0);
    TEST_ASSERT_EQUAL_UINT(0, camera.x);
    TEST_ASSERT_EQUAL_UINT(0, camera.y);
}

/* Test get relative x at same position */
void test_camera_get_relative_x_zero(void) {
    signed int relative_x;
    camera = create_camera(0, 0);
    relative_x = camera_get_relative_x(&camera, 0);
    TEST_ASSERT_EQUAL_INT(0, relative_x);
}

/* Test get relative x to right of the camera */
void test_camera_get_relative_x_positive(void) {
    signed int relative_x;
    camera = create_camera(0, 0);
    relative_x = camera_get_relative_x(&camera, 100);
    TEST_ASSERT_EQUAL_INT(100, relative_x);
}

/* Test get relative x to the left of the camera */
void test_camera_get_relative_x_negative(void) {
    signed int relative_x;
    camera = create_camera(100, 0);
    relative_x = camera_get_relative_x(&camera, 50);
    TEST_ASSERT_EQUAL_INT(-50, relative_x);
}

/* Test get relative y at same position */
void test_camera_get_relative_y_zero(void) {
    signed int relative_y;
    camera = create_camera(0, 0);
    relative_y = camera_get_relative_y(&camera, 0);
    TEST_ASSERT_EQUAL_INT(0, relative_y);
}

/* Test get relative y to above the camera */
void test_camera_get_relative_y_negative(void) {
    /* This can be a bit confusing, but -150 is 150 above camera*/
    signed int relative_y;
    camera = create_camera(0, 0);
    relative_y = camera_get_relative_y(&camera, 150);
    TEST_ASSERT_EQUAL_INT(-150, relative_y);
}

/* Test get relative y to below the camera */
void test_camera_get_relative_y_positive(void) {
    signed int relative_y;
    camera = create_camera(100, 100);
    relative_y = camera_get_relative_y(&camera, 50);
    TEST_ASSERT_EQUAL_INT(50, relative_y);
}

/* Test camera tracking - simulate following an object */
void test_camera_tracking_object(void) {
    unsigned int object_x = 500;
    unsigned int object_y = 300;
    signed int relative_x;
    signed int relative_y;

    camera = create_camera(0, 0);

    camera_update_coordinates(&camera, object_x, object_y);

    relative_x = camera_get_relative_x(&camera, object_x);
    relative_y = camera_get_relative_y(&camera, object_y);

    TEST_ASSERT_EQUAL_INT(0, relative_x);
    TEST_ASSERT_EQUAL_INT(0, relative_y);
}

/* Main function to run all tests */
int main(void) {
    UNITY_BEGIN();
    
    RUN_TEST(test_camera_create_zero);
    RUN_TEST(test_camera_create_normal);
    RUN_TEST(test_camera_update_coordinates);
    RUN_TEST(test_camera_update_to_zero);
    RUN_TEST(test_camera_get_relative_x_zero);
    RUN_TEST(test_camera_get_relative_x_positive);
    RUN_TEST(test_camera_get_relative_x_negative);
    RUN_TEST(test_camera_get_relative_y_zero);
    RUN_TEST(test_camera_get_relative_y_positive);
    RUN_TEST(test_camera_get_relative_y_negative);
    RUN_TEST(test_camera_tracking_object);

    return UNITY_END();
}
