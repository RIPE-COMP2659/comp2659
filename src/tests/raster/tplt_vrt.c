#include "unity.h"
#include "raster.h"
#include <string.h>

#define SCREEN_SIZE_BYTES 32000
#define SCREEN_WIDTH_BYTES 80
#define SCREEN_WIDTH_PIXELS 640
#define SCREEN_HEIGHT_PIXELS 400

/* Prototype matching assembly routine. 
 * Note: Using INT16 to allow passing negative coordinates for clipping tests!
 */
extern void plot_vertical_line(UINT32 *base, INT16 row, INT16 col, UINT16 length, UINT16 color);

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

/* Test basic vertical line drawing */
void test_plot_vertical_line_basic(void) {
    int i;
    
    /* Plot a vertical line down column 15, starting at row 10, length 5 */
    plot_vertical_line(mock_screen, 10, 15, 5, 1);

    /* Verify the exact 5 pixels are drawn (rows 10 through 14) */
    for (i = 10; i < 15; i++) {
        TEST_ASSERT_EQUAL_INT_MESSAGE(1, get_pixel(mock_screen, i, 15), "Vertical line pixel missing");
    }

    /* Verify the pixels immediately above and below remain untouched (boundary fundamental) */
    TEST_ASSERT_EQUAL_INT(0, get_pixel(mock_screen, 9, 15));
    TEST_ASSERT_EQUAL_INT(0, get_pixel(mock_screen, 15, 15));
}

/* Test vertical line with color parameter (black/white) */
void test_plot_vertical_line_with_color(void) {
    /* First draw a white line of length 5 */
    plot_vertical_line(mock_screen, 10, 25, 5, 1);
    
    /* Then overwrite the middle 3 pixels with a black line */
    plot_vertical_line(mock_screen, 11, 25, 3, 0);

    /* Verify the "sandwich" effect: White, Blackx3, White */
    TEST_ASSERT_EQUAL_INT(1, get_pixel(mock_screen, 10, 25));
    TEST_ASSERT_EQUAL_INT(0, get_pixel(mock_screen, 11, 25));
    TEST_ASSERT_EQUAL_INT(0, get_pixel(mock_screen, 12, 25));
    TEST_ASSERT_EQUAL_INT(0, get_pixel(mock_screen, 13, 25));
    TEST_ASSERT_EQUAL_INT(1, get_pixel(mock_screen, 14, 25));
}

/* Test clipping logic to ensure memory bounds are protected */
void test_plot_vertical_line_clipping(void) {
    /* 1. Top clip: Start at row -3, length 5.
       The first 3 pixels should be skipped. Rows 0 and 1 should be drawn. */
    plot_vertical_line(mock_screen, -3, 20, 5, 1);
    TEST_ASSERT_EQUAL_INT(1, get_pixel(mock_screen, 0, 20));
    TEST_ASSERT_EQUAL_INT(1, get_pixel(mock_screen, 1, 20));
    TEST_ASSERT_EQUAL_INT(0, get_pixel(mock_screen, 2, 20)); /* Row 2 remains black */

    /* 2. Bottom clip: Start at row 398, length 5. 
       The last 3 pixels should be skipped. Rows 398 and 399 should be drawn. */
    plot_vertical_line(mock_screen, 398, 30, 5, 1);
    TEST_ASSERT_EQUAL_INT(1, get_pixel(mock_screen, 398, 30));
    TEST_ASSERT_EQUAL_INT(1, get_pixel(mock_screen, 399, 30));
    /* If row 400 was written to, Unity/OS will often throw a memory fault here */

    /* 3. Entirely off left edge (col -5) */
    plot_vertical_line(mock_screen, 50, -5, 10, 1);
    
    /* 4. Entirely off right edge (col 645) */
    plot_vertical_line(mock_screen, 50, 645, 10, 1);
    
    /* 5. Entirely off bottom edge (row 410) */
    plot_vertical_line(mock_screen, 410, 40, 10, 1);

    /* Verify a random spot to ensure no wild memory writes occurred from the out-of-bounds tests */
    TEST_ASSERT_EQUAL_INT(0, get_pixel(mock_screen, 50, 0));
}

/* Main function to run all tests */
int main(void) {
    UNITY_BEGIN();
    
    RUN_TEST(test_plot_vertical_line_basic);
    RUN_TEST(test_plot_vertical_line_with_color);
    RUN_TEST(test_plot_vertical_line_clipping);
    
    return UNITY_END();
}
