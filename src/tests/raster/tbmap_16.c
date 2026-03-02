#include "unity.h"
#include "raster.h"
#include <string.h>

#define SCREEN_SIZE_BYTES 32000
#define SCREEN_WIDTH_BYTES 80
#define SCREEN_HEIGHT 400

/* Mock screen buffer for testing */
static UINT8 mock_screen[SCREEN_SIZE_BYTES];

/* Sample 16-pixel wide bitmap data */
static const UINT16 sample_bitmap_16[] = {
    0xFFFF,  /* Row 1 */
    0xFFFF,  /* Row 2 */
    0xFFFF,  /* Row 3 */
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

/* Implement test_plot_bitmap_16_basic
 * Test basic 16-pixel wide bitmap plotting
 */
void test_plot_bitmap_16_basic(void) {
    /* Plot all 3 rows of the sample bitmap at row 10, column 16 
       (col 16 = byte offset 2 since 16 pixels = 2 bytes) */
    plot_bitmap_16(mock_screen, 10, 16, sample_bitmap_16, 3);

    /* Verify the 3 rows were drawn exactly where expected.
       Because it's a 16-bit bitmap, we check BOTH bytes for each row (offsets 2 and 3). */
    
    /* Row 10 */
    TEST_ASSERT_EQUAL_HEX8(0xFF, mock_screen[(10 * SCREEN_WIDTH_BYTES) + 2]);
    TEST_ASSERT_EQUAL_HEX8(0xFF, mock_screen[(10 * SCREEN_WIDTH_BYTES) + 3]);
    
    /* Row 11 */
    TEST_ASSERT_EQUAL_HEX8(0xFF, mock_screen[(11 * SCREEN_WIDTH_BYTES) + 2]);
    TEST_ASSERT_EQUAL_HEX8(0xFF, mock_screen[(11 * SCREEN_WIDTH_BYTES) + 3]);
    
    /* Row 12 */
    TEST_ASSERT_EQUAL_HEX8(0xFF, mock_screen[(12 * SCREEN_WIDTH_BYTES) + 2]);
    TEST_ASSERT_EQUAL_HEX8(0xFF, mock_screen[(12 * SCREEN_WIDTH_BYTES) + 3]);
    
    /* Verify the surrounding memory was untouched (fundamental boundary check) */
    TEST_ASSERT_EQUAL_HEX8(0x00, mock_screen[(9 * SCREEN_WIDTH_BYTES) + 2]);  /* Row above */
    TEST_ASSERT_EQUAL_HEX8(0x00, mock_screen[(13 * SCREEN_WIDTH_BYTES) + 2]); /* Row below */
}

/* Implement test_plot_bitmap_16_various_heights
 * Test 16-pixel bitmaps with different heights
 */
void test_plot_bitmap_16_various_heights(void) {
    /* Pass in height=2, deliberately truncating the 3-row array */
    plot_bitmap_16(mock_screen, 20, 32, sample_bitmap_16, 2);

    /* Verify only the first 2 rows rendered at column 32 (byte offset 4 and 5) */
    
    /* Row 20 */
    TEST_ASSERT_EQUAL_HEX8(0xFF, mock_screen[(20 * SCREEN_WIDTH_BYTES) + 4]);
    TEST_ASSERT_EQUAL_HEX8(0xFF, mock_screen[(20 * SCREEN_WIDTH_BYTES) + 5]);
    
    /* Row 21 */
    TEST_ASSERT_EQUAL_HEX8(0xFF, mock_screen[(21 * SCREEN_WIDTH_BYTES) + 4]);
    TEST_ASSERT_EQUAL_HEX8(0xFF, mock_screen[(21 * SCREEN_WIDTH_BYTES) + 5]);
    
    /* Verify the 3rd row was NOT rendered (should remain completely black 0x00) */
    TEST_ASSERT_EQUAL_HEX8(0x00, mock_screen[(22 * SCREEN_WIDTH_BYTES) + 4]);
    TEST_ASSERT_EQUAL_HEX8(0x00, mock_screen[(22 * SCREEN_WIDTH_BYTES) + 5]);
}

/* Test Top Clipping (Partially off the top) */
void test_plot_bitmap_16_clip_top(void) {
    /* Plot at row -2. The first two rows of the 3-row bitmap should be skipped.
       Only the 3rd row should render, and it should land exactly at row 0. */
    plot_bitmap_16(mock_screen, -2, 16, sample_bitmap_16, 3);

    /* Verify row 0 got the 3rd row of the bitmap */
    TEST_ASSERT_EQUAL_HEX8(0xFF, mock_screen[(0 * SCREEN_WIDTH_BYTES) + 2]);
    TEST_ASSERT_EQUAL_HEX8(0xFF, mock_screen[(0 * SCREEN_WIDTH_BYTES) + 3]);

    /* Verify row 1 is black */
    TEST_ASSERT_EQUAL_HEX8(0x00, mock_screen[(1 * SCREEN_WIDTH_BYTES) + 2]);
    TEST_ASSERT_EQUAL_HEX8(0x00, mock_screen[(1 * SCREEN_WIDTH_BYTES) + 3]);
}

/* Test Bottom Clipping (Partially off the bottom) */
void test_plot_bitmap_16_clip_bottom(void) {
    /* Plot at row 398. The screen is 400 rows high (0-399).
       Row 1 -> 398, Row 2 -> 399, Row 3 -> clipped. */
    plot_bitmap_16(mock_screen, 398, 16, sample_bitmap_16, 3);

    /* Verify row 398 and 399 got rendered */
    TEST_ASSERT_EQUAL_HEX8(0xFF, mock_screen[(398 * SCREEN_WIDTH_BYTES) + 2]);
    TEST_ASSERT_EQUAL_HEX8(0xFF, mock_screen[(398 * SCREEN_WIDTH_BYTES) + 3]);
    TEST_ASSERT_EQUAL_HEX8(0xFF, mock_screen[(399 * SCREEN_WIDTH_BYTES) + 2]);
    TEST_ASSERT_EQUAL_HEX8(0xFF, mock_screen[(399 * SCREEN_WIDTH_BYTES) + 3]);
    
    /* The OS/Unity will catch memory corruption if it tries to write to row 400 */
}

/* Test Completely Off Screen (Top and Bottom) */
void test_plot_bitmap_16_completely_off_screen(void) {
    /* Plot way above the screen */
    plot_bitmap_16(mock_screen, -10, 16, sample_bitmap_16, 3);
    
    /* Plot way below the screen */
    plot_bitmap_16(mock_screen, 410, 16, sample_bitmap_16, 3);

    /* Verify that random spots at the top and bottom remain 0x00, 
       proving the assembly successfully aborted the draw. */
    TEST_ASSERT_EQUAL_HEX8(0x00, mock_screen[(0 * SCREEN_WIDTH_BYTES) + 2]);
    TEST_ASSERT_EQUAL_HEX8(0x00, mock_screen[(399 * SCREEN_WIDTH_BYTES) + 2]);
}

/* Main function to run all tests */
int main(void) {
    UNITY_BEGIN();
    
    RUN_TEST(test_plot_bitmap_16_basic);
    RUN_TEST(test_plot_bitmap_16_various_heights);
    RUN_TEST(test_plot_bitmap_16_clip_top);
    RUN_TEST(test_plot_bitmap_16_clip_bottom);
    RUN_TEST(test_plot_bitmap_16_completely_off_screen);
    
    return UNITY_END();
}
