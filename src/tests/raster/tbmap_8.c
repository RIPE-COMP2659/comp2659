#include "unity.h"
#include "raster.h"
#include <string.h>

#define SCREEN_SIZE_BYTES 32000
#define SCREEN_WIDTH_BYTES 80
#define SCREEN_HEIGHT 400

/* Mock screen buffer for testing */
static UINT8 mock_screen[SCREEN_SIZE_BYTES];

/* Sample 8-pixel wide bitmap data (using distinct bytes to track rows) */
static const UINT8 sample_bitmap_8[] = {
    0x11,  /* Row 1 */
    0x22,  /* Row 2 */
    0x33,  /* Row 3 */
};

/* Setup - runs before each test */
void setUp(void) {
    /* Initialize screen with all zeros (black) */
    memset(mock_screen, 0x00, SCREEN_SIZE_BYTES);
}

/* Teardown - runs after each test */
void tearDown(void) {
    /* Nothing to clean up */
}

/* Test basic 8-pixel wide bitmap plotting */
void test_plot_bitmap_8_basic(void) {
    /* Plot all 3 rows of the sample bitmap at row 10, column 8 
       (col 8 = byte offset 1 since 8 pixels = 1 byte) */
    plot_bitmap_8(mock_screen, 10, 8, sample_bitmap_8, 3);

    /* Verify the 3 rows were drawn exactly where expected */
    TEST_ASSERT_EQUAL_HEX8(0x11, mock_screen[(10 * SCREEN_WIDTH_BYTES) + 1]);
    TEST_ASSERT_EQUAL_HEX8(0x22, mock_screen[(11 * SCREEN_WIDTH_BYTES) + 1]);
    TEST_ASSERT_EQUAL_HEX8(0x33, mock_screen[(12 * SCREEN_WIDTH_BYTES) + 1]);
    
    /* Verify the surrounding memory was untouched (fundamental boundary check) */
    TEST_ASSERT_EQUAL_HEX8(0x00, mock_screen[(9 * SCREEN_WIDTH_BYTES) + 1]);  /* Row above */
    TEST_ASSERT_EQUAL_HEX8(0x00, mock_screen[(13 * SCREEN_WIDTH_BYTES) + 1]); /* Row below */
}

/* Test 8-pixel bitmaps with different heights */
void test_plot_bitmap_8_various_heights(void) {
    /* Pass in height=2, deliberately truncating the 3-row array */
    plot_bitmap_8(mock_screen, 20, 16, sample_bitmap_8, 2);

    /* Verify only the first 2 rows rendered at column 16 (byte offset 2) */
    TEST_ASSERT_EQUAL_HEX8(0x11, mock_screen[(20 * SCREEN_WIDTH_BYTES) + 2]);
    TEST_ASSERT_EQUAL_HEX8(0x22, mock_screen[(21 * SCREEN_WIDTH_BYTES) + 2]);
    
    /* Verify the 3rd row was NOT rendered (should remain completely black 0x00) */
    TEST_ASSERT_EQUAL_HEX8(0x00, mock_screen[(22 * SCREEN_WIDTH_BYTES) + 2]);
}

/* Test Top Clipping (Partially off the top) */
void test_plot_bitmap_8_clip_top(void) {
    /* Plot at row -2. The first two rows (0x11, 0x22) should be skipped.
       Only the 3rd row (0x33) should render exactly at row 0. */
    plot_bitmap_8(mock_screen, -2, 8, sample_bitmap_8, 3);

    /* Verify row 0 got the 3rd row of the bitmap */
    TEST_ASSERT_EQUAL_HEX8(0x33, mock_screen[(0 * SCREEN_WIDTH_BYTES) + 1]);

    /* Verify row 1 is black */
    TEST_ASSERT_EQUAL_HEX8(0x00, mock_screen[(1 * SCREEN_WIDTH_BYTES) + 1]);
}

/* Test Bottom Clipping (Partially off the bottom) */
void test_plot_bitmap_8_clip_bottom(void) {
    /* Plot at row 398. The screen is 400 rows high (0-399).
       Row 1 -> 398, Row 2 -> 399, Row 3 -> clipped. */
    plot_bitmap_8(mock_screen, 398, 8, sample_bitmap_8, 3);

    /* Verify row 398 and 399 got rendered */
    TEST_ASSERT_EQUAL_HEX8(0x11, mock_screen[(398 * SCREEN_WIDTH_BYTES) + 1]);
    TEST_ASSERT_EQUAL_HEX8(0x22, mock_screen[(399 * SCREEN_WIDTH_BYTES) + 1]);
}

/* Test Completely Off Screen (Top and Bottom) */
void test_plot_bitmap_8_completely_off_screen(void) {
    /* Plot way above the screen */
    plot_bitmap_8(mock_screen, -10, 8, sample_bitmap_8, 3);
    
    /* Plot way below the screen */
    plot_bitmap_8(mock_screen, 410, 8, sample_bitmap_8, 3);

    /* Verify that random spots remain 0x00, proving abortion of draw */
    TEST_ASSERT_EQUAL_HEX8(0x00, mock_screen[(0 * SCREEN_WIDTH_BYTES) + 1]);
    TEST_ASSERT_EQUAL_HEX8(0x00, mock_screen[(399 * SCREEN_WIDTH_BYTES) + 1]);
}

/* Main function to run all tests */
int main(void) {
    UNITY_BEGIN();
    
    RUN_TEST(test_plot_bitmap_8_basic);
    RUN_TEST(test_plot_bitmap_8_various_heights);
    RUN_TEST(test_plot_bitmap_8_clip_top);
    RUN_TEST(test_plot_bitmap_8_clip_bottom);
    RUN_TEST(test_plot_bitmap_8_completely_off_screen);
    
    return UNITY_END();
}
