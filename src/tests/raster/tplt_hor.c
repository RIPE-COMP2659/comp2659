#include "unity.h"
#include "raster.h"
#include <string.h>

#define SCREEN_SIZE_BYTES 32000
#define SCREEN_WIDTH_BYTES 80
#define SCREEN_WIDTH_PIXELS 640
#define SCREEN_HEIGHT_PIXELS 400

/* Prototype matching your assembly routine. 
 * Note: Using INT16 to allow passing negative coordinates for clipping tests!
 */
extern void plot_horizontal_line(UINT32 *base, INT16 row, INT16 col, UINT16 length, UINT16 color);

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

/* Implement test_plot_horizontal_line_basic
 * Test basic horizontal line at origin going right
 */
void test_plot_horizontal_line_basic(void) {
    int i;
    
    /* Plot a horizontal line across row 15, starting at col 10, length 5 */
    plot_horizontal_line(mock_screen, 15, 10, 5, 1);

    /* Verify the exact 5 pixels are drawn (cols 10 through 14) */
    for (i = 10; i < 15; i++) {
        TEST_ASSERT_EQUAL_INT_MESSAGE(1, get_pixel(mock_screen, 15, i), "Horizontal line pixel missing");
    }

    /* Verify the pixels immediately before and after remain untouched (boundary check) */
    TEST_ASSERT_EQUAL_INT(0, get_pixel(mock_screen, 15, 9));
    TEST_ASSERT_EQUAL_INT(0, get_pixel(mock_screen, 15, 15));
}

/* Implement test_plot_horizontal_line_with_color
 * Test horizontal line with color parameter (black/white)
 */
void test_plot_horizontal_line_with_color(void) {
    /* First draw a white line of length 5 */
    plot_horizontal_line(mock_screen, 20, 25, 5, 1);
    
    /* Then overwrite the middle 3 pixels with a black line */
    plot_horizontal_line(mock_screen, 20, 26, 3, 0);

    /* Verify the "sandwich" effect: White, Blackx3, White */
    TEST_ASSERT_EQUAL_INT(1, get_pixel(mock_screen, 20, 25));
    TEST_ASSERT_EQUAL_INT(0, get_pixel(mock_screen, 20, 26));
    TEST_ASSERT_EQUAL_INT(0, get_pixel(mock_screen, 20, 27));
    TEST_ASSERT_EQUAL_INT(0, get_pixel(mock_screen, 20, 28));
    TEST_ASSERT_EQUAL_INT(1, get_pixel(mock_screen, 20, 29));
}

/* Test clipping logic to ensure memory bounds are protected */
void test_plot_horizontal_line_clipping(void) {
    /* 1. Left clip: Start at col -3, length 5.
       The first 3 pixels should be skipped. Cols 0 and 1 should be drawn. */
    plot_horizontal_line(mock_screen, 30, -3, 5, 1);
    TEST_ASSERT_EQUAL_INT(1, get_pixel(mock_screen, 30, 0));
    TEST_ASSERT_EQUAL_INT(1, get_pixel(mock_screen, 30, 1));
    TEST_ASSERT_EQUAL_INT(0, get_pixel(mock_screen, 30, 2)); /* Col 2 remains black */

    /* 2. Right clip: Start at col 638, length 5. 
       The last 3 pixels should be skipped. Cols 638 and 639 should be drawn. */
    plot_horizontal_line(mock_screen, 40, 638, 5, 1);
    TEST_ASSERT_EQUAL_INT(1, get_pixel(mock_screen, 40, 638));
    TEST_ASSERT_EQUAL_INT(1, get_pixel(mock_screen, 40, 639));
    /* If col 640 was written to, it would bleed into row 41! */
    TEST_ASSERT_EQUAL_INT(0, get_pixel(mock_screen, 41, 0));

    /* 3. Entirely off top edge (row -5) */
    plot_horizontal_line(mock_screen, -5, 50, 10, 1);
    
    /* 4. Entirely off bottom edge (row 405) */
    plot_horizontal_line(mock_screen, 405, 50, 10, 1);
    
    /* 5. Entirely off left edge (col -10, length 5) */
    plot_horizontal_line(mock_screen, 50, -10, 5, 1);

    /* 6. Entirely off right edge (col 650, length 5) */
    plot_horizontal_line(mock_screen, 50, 650, 5, 1);

    /* Verify a random spot to ensure no wild memory writes occurred from the out-of-bounds tests */
    TEST_ASSERT_EQUAL_INT(0, get_pixel(mock_screen, 0, 50));
}

/* Main function to run all tests */
int main(void) {
    UNITY_BEGIN();
    
    RUN_TEST(test_plot_horizontal_line_basic);
    RUN_TEST(test_plot_horizontal_line_with_color);
    RUN_TEST(test_plot_horizontal_line_clipping);
    
    return UNITY_END();
}