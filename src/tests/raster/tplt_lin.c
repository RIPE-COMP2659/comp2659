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
extern void plot_line(UINT32 *base, INT16 start_row, INT16 start_col, INT16 end_row, INT16 end_col, UINT16 color);

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

/* Test line drawing for horizontal lines */
void test_plot_line_horizontal(void) {
    int i;
    
    /* Plot a horizontal line across row 10, from col 15 to 25 */
    plot_line(mock_screen, 10, 15, 10, 25, 1);

    /* Verify the exact line is drawn */
    for (i = 15; i <= 25; i++) {
        TEST_ASSERT_EQUAL_INT_MESSAGE(1, get_pixel(mock_screen, 10, i), "Horizontal line pixel missing");
    }

    /* Verify the pixels immediately before and after remain untouched */
    TEST_ASSERT_EQUAL_INT(0, get_pixel(mock_screen, 10, 14));
    TEST_ASSERT_EQUAL_INT(0, get_pixel(mock_screen, 10, 26));
}

/* Test line drawing for vertical lines */
void test_plot_line_vertical(void) {
    int i;
    
    /* Plot a vertical line down column 40, from row 5 to 15 */
    plot_line(mock_screen, 5, 40, 15, 40, 1);

    /* Verify the exact line is drawn */
    for (i = 5; i <= 15; i++) {
        TEST_ASSERT_EQUAL_INT_MESSAGE(1, get_pixel(mock_screen, i, 40), "Vertical line pixel missing");
    }

    /* Verify the pixels immediately above and below remain untouched */
    TEST_ASSERT_EQUAL_INT(0, get_pixel(mock_screen, 4, 40));
    TEST_ASSERT_EQUAL_INT(0, get_pixel(mock_screen, 16, 40));
}

/* Test line drawing for diagonal lines (Bresenham algorithm) */
void test_plot_line_diagonal(void) {
    int i;
    
    /* Plot a perfect 45-degree diagonal line from (20, 20) to (25, 25) */
    plot_line(mock_screen, 20, 20, 25, 25, 1);

    /* Verify the exact diagonal pixels are drawn */
    for (i = 0; i <= 5; i++) {
        TEST_ASSERT_EQUAL_INT_MESSAGE(1, get_pixel(mock_screen, 20 + i, 20 + i), "Diagonal line pixel missing");
    }

    /* Verify surrounding offset pixels are untouched (checking Bresenham accuracy) */
    TEST_ASSERT_EQUAL_INT(0, get_pixel(mock_screen, 20, 21));
    TEST_ASSERT_EQUAL_INT(0, get_pixel(mock_screen, 21, 20));
}

/* Test clipping for lines that go out of bounds */
void test_plot_line_clipping(void) {
    int i;

    /* 1. Line partially off the left edge (col -5 to 5 at row 30) */
    plot_line(mock_screen, 30, -5, 30, 5, 1);

    /* Verify only the visible portion (cols 0 to 5) was drawn */
    for (i = 0; i <= 5; i++) {
        TEST_ASSERT_EQUAL_INT(1, get_pixel(mock_screen, 30, i));
    }
    /* Verify pixel just past the end of the line is black */
    TEST_ASSERT_EQUAL_INT(0, get_pixel(mock_screen, 30, 6));


    /* 2. Line entirely off the bottom of the screen (row 410 to 420) */
    plot_line(mock_screen, 410, 100, 420, 100, 1);

    /* Verify the bottom-most visible row directly above it is completely untouched */
    TEST_ASSERT_EQUAL_INT(0, get_pixel(mock_screen, SCREEN_HEIGHT_PIXELS - 1, 100));
}

/* Main function to run all tests */
int main(void) {
    UNITY_BEGIN();
    
    RUN_TEST(test_plot_line_horizontal);
    RUN_TEST(test_plot_line_vertical);
    RUN_TEST(test_plot_line_diagonal);
    RUN_TEST(test_plot_line_clipping);
    
    return UNITY_END();
}