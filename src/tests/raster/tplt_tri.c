#include "unity.h"
#include "raster.h"
#include <string.h>

#define SCREEN_SIZE_BYTES 32000
#define SCREEN_WIDTH_BYTES 80
#define SCREEN_HEIGHT 400

/* Mock screen buffer for testing */
static UINT32 mock_screen[SCREEN_SIZE_BYTES / 4];

/* Helper to check if a single pixel at (row, col) is black (0) or white (1) */
static int get_pixel(UINT32 *base, INT16 row, INT16 col)
{
    UINT8 *byte_ptr = (UINT8 *)base + (row * SCREEN_WIDTH_BYTES) + (col / 8);
    int bit_pos = 7 - (col % 8);
    return (*byte_ptr >> bit_pos) & 1;
}

/* Setup - runs before each test */
void setUp(void) {
    /* Initialize screen with all zeros (black) */
    memset(mock_screen, 0x00, SCREEN_SIZE_BYTES);
}

/* Teardown - runs after each test */
void tearDown(void) {
    /* Nothing to clean up */
}

/* Implement test_plot_triangle_basic
 * Test basic triangle drawing (Direction 0: Top-Left)
 */
void test_plot_triangle_basic(void) {
    /* 90-degree angle at (10, 10), base 10, height 10, Direction 0 */
    plot_triangle(mock_screen, 10, 10, 10, 10, 0);

    /* Verify the tip (first row) has a minimal width */
    /* Formula: (10 * (0 + 1)) / 10 = 1 pixel */
    TEST_ASSERT_EQUAL_INT(1, get_pixel(mock_screen, 10, 10));
    TEST_ASSERT_EQUAL_INT(0, get_pixel(mock_screen, 10, 11));

    /* Verify the base (last row, offset 9) has full width */
    /* Formula: (10 * (9 + 1)) / 10 = 10 pixels */
    TEST_ASSERT_EQUAL_INT(1, get_pixel(mock_screen, 19, 10));
    TEST_ASSERT_EQUAL_INT(1, get_pixel(mock_screen, 19, 19));
    TEST_ASSERT_EQUAL_INT(0, get_pixel(mock_screen, 19, 20));
}

/* Implement test_plot_triangle_all_directions
 * Test triangles in all 4 directions to ensure coordinate expansion logic is correct
 */
void test_plot_triangle_all_directions(void) {
    /* 1. Top-Right (Direction 1): Expands left and down */
    plot_triangle(mock_screen, 30, 50, 10, 10, 1);
    TEST_ASSERT_EQUAL_INT_MESSAGE(1, get_pixel(mock_screen, 30, 49), "Top-Right expansion failed");
    TEST_ASSERT_EQUAL_INT(1, get_pixel(mock_screen, 39, 40));

    /* 2. Bottom-Left (Direction 2): Expands right and up */
    plot_triangle(mock_screen, 100, 10, 10, 10, 2);
    TEST_ASSERT_EQUAL_INT_MESSAGE(1, get_pixel(mock_screen, 100, 10), "Bottom-Left start failed");
    TEST_ASSERT_EQUAL_INT(1, get_pixel(mock_screen, 91, 10));

    /* 3. Bottom-Right (Direction 3): Expands left and up */
    plot_triangle(mock_screen, 200, 50, 10, 10, 3);
    TEST_ASSERT_EQUAL_INT_MESSAGE(1, get_pixel(mock_screen, 200, 49), "Bottom-Right expansion failed");
    TEST_ASSERT_EQUAL_INT(1, get_pixel(mock_screen, 191, 40));
}

/* Main function to run all tests */
int main(void) {
    UNITY_BEGIN();
    
    RUN_TEST(test_plot_triangle_basic);
    RUN_TEST(test_plot_triangle_all_directions);
    
    return UNITY_END();
}
