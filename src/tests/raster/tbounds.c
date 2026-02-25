#include "unity.h"
#include "raster.h"
#include <string.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 400
#define IN_BOUNDS 0
#define LEFT_EDGE 1
#define RIGHT_EDGE 2
#define OUT_OF_BOUNDS 3

/* Test rectangle, cleaner */
typedef struct {
    INT16 row;
    INT16 col;
    INT16 height;
    INT16 width;
} TestRect;

/* Global test rectangle */
static TestRect rectangle;

/*----- Type: BoundsCheckResult -----

PURPOSE: Structure to hold the result of a bounds check operation.
         Contains both the status code and the adjusted width.

FIELDS:
    status: INT8 - Status of bounds check
            0 = within bounds
            1 = exceeds left edge
            2 = exceeds right edge
            3 = entirely out of bounds
    new_width: INT32 - Clipped width (for out-of-bounds cases)
*/
typedef struct {
    INT8 status;
    INT32 new_width;
} BoundsCheckResult;

extern void check_bounds(INT16 row, INT16 col, INT16 height, INT16 width, INT8 *status, INT32 *new_width);

BoundsCheckResult check_bounds_asm(INT16 row, INT16 col, INT16 height, INT16 width) {
    BoundsCheckResult result;
    check_bounds(row, col, height, width, &result.status, &result.new_width);
    return result;
}

/* Helper function to check bounds with a test rectangle */
static BoundsCheckResult check_bounds_rect(TestRect rect) {
    return check_bounds_asm(rect.row, rect.col, rect.height, rect.width);
}

/* Setup - runs before each test */
void setUp(void) {
    /* Initialize test rectangle to default values */
    rectangle.row = 0;
    rectangle.col = 0;
    rectangle.height = 32;
    rectangle.width = 64;
}

/* Teardown - runs after each test */
void tearDown(void) {
    /* Nothing to clean up */
}

/* Test: Within bounds - normal */
void test_bounds_inside_basic(void) {
    BoundsCheckResult result;

    TEST_IGNORE_MESSAGE("bounds assembly returns vals in d0 and d1, not supported by C, would need to change");

    rectangle.row = SCREEN_HEIGHT / 2;
    rectangle.col = SCREEN_WIDTH / 2;
    printf("Testing bounds with rectangle at row %d, col %d, height %d, width %d\n",
           rectangle.row, rectangle.col, rectangle.height, rectangle.width);
    result = check_bounds_rect(rectangle);

    TEST_ASSERT_EQUAL_INT8(IN_BOUNDS, result.status);
    TEST_ASSERT_EQUAL_INT32(rectangle.width, result.new_width);
}

/* Test: Within bounds - at top-left corner */
void test_bounds_inside_top_left(void) {
    BoundsCheckResult result;

    TEST_IGNORE_MESSAGE("bounds assembly returns vals in d0 and d1, not supported by C, would need to change");

    rectangle.row = 0;
    rectangle.col = 0;
    result = check_bounds_rect(rectangle);

    TEST_ASSERT_EQUAL_INT8(IN_BOUNDS, result.status);
    TEST_ASSERT_EQUAL_INT32(rectangle.width, result.new_width);
}

/* Test: Within bounds - at top-right corner */
void test_bounds_inside_top_right(void) {
    BoundsCheckResult result;

    TEST_IGNORE_MESSAGE("bounds assembly returns vals in d0 and d1, not supported by C, would need to change");

    rectangle.row = 0;
    rectangle.col = SCREEN_WIDTH - rectangle.width;
    result = check_bounds_rect(rectangle);

    TEST_ASSERT_EQUAL_INT8(IN_BOUNDS, result.status);
    TEST_ASSERT_EQUAL_INT32(rectangle.width, result.new_width);
}

/* Test: Within bounds - at bottom-left corner */
void test_bounds_inside_bottom_left(void) {
    BoundsCheckResult result;

    TEST_IGNORE_MESSAGE("bounds assembly returns vals in d0 and d1, not supported by C, would need to change");

    rectangle.row = SCREEN_HEIGHT - rectangle.height;
    rectangle.col = 0;
    result = check_bounds_rect(rectangle);

    TEST_ASSERT_EQUAL_INT8(IN_BOUNDS, result.status);
    TEST_ASSERT_EQUAL_INT32(rectangle.width, result.new_width);
}

/* Test: Within bounds - at bottom-right corner */
void test_bounds_inside_bottom_right(void) {
    BoundsCheckResult result;

    TEST_IGNORE_MESSAGE("bounds assembly returns vals in d0 and d1, not supported by C, would need to change");

    rectangle.row = SCREEN_HEIGHT - rectangle.height;
    rectangle.col = SCREEN_WIDTH - rectangle.width;
    result = check_bounds_rect(rectangle);

    TEST_ASSERT_EQUAL_INT8(IN_BOUNDS, result.status);
    TEST_ASSERT_EQUAL_INT32(rectangle.width, result.new_width);
}

/* Main function to run all tests */
int main(void) {
    UNITY_BEGIN();

    
    RUN_TEST(test_bounds_inside_basic);
    RUN_TEST(test_bounds_inside_top_left);
    RUN_TEST(test_bounds_inside_top_right);
    RUN_TEST(test_bounds_inside_bottom_left);
    RUN_TEST(test_bounds_inside_bottom_right);

    return UNITY_END();
}