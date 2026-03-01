#include "unity.h"
#include "raster.h" /* Assuming this includes your UINT8/INT16 types and prototypes */
#include <string.h>

#define SCREEN_SIZE_BYTES 32000
#define SCREEN_WIDTH_BYTES 80

/* Mock screen buffer for testing */
static UINT8 mock_screen[SCREEN_SIZE_BYTES];

/* Sample 8-pixel wide bitmap data - using distinct bytes to track shifts */
static const UINT8 sample_bitmap_8[] = {
    0x11,  /* Row 0 */
    0x22,  /* Row 1 */
    0x33,  /* Row 2 */
    0x44,  /* Row 3 */
    0x55   /* Row 4 */
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

/* Test basic 8-pixel wide bitmap plotting
 */
void test_plot_bitmap_8_basic(void) {
    /* Plot all 3 rows of the sample bitmap at row 10, column 8 
       (col 8 = byte offset 1 since 8 pixels = 1 byte) */
    plot_bitmap_8(mock_screen, 10, 8, sample_bitmap_8, 3);

    /* Verify the 3 rows were drawn exactly where expected */
    TEST_ASSERT_EQUAL_HEX8(0x11, mock_screen[(10 * SCREEN_WIDTH_BYTES) + 1]);
    TEST_ASSERT_EQUAL_HEX8(0x22, mock_screen[(11 * SCREEN_WIDTH_BYTES) + 1]);
    TEST_ASSERT_EQUAL_HEX8(0x33, mock_screen[(12 * SCREEN_WIDTH_BYTES) + 1]);
    
    /* Verify the surrounding memory was untouched (a crucial fundamental check!) */
    TEST_ASSERT_EQUAL_HEX8(0x00, mock_screen[(9 * SCREEN_WIDTH_BYTES) + 1]);  /* Row above */
    TEST_ASSERT_EQUAL_HEX8(0x00, mock_screen[(13 * SCREEN_WIDTH_BYTES) + 1]); /* Row below */
}

/* Test 8-pixel bitmaps with different heights
 */
void test_plot_bitmap_8_various_heights(void) {
    /* We pass in height=2, even though the bitmap array has more rows */
    plot_bitmap_8(mock_screen, 20, 16, sample_bitmap_8, 2);

    /* Verify only the first 2 rows rendered at column 16 (byte offset 2) */
    TEST_ASSERT_EQUAL_HEX8(0x11, mock_screen[(20 * SCREEN_WIDTH_BYTES) + 2]);
    TEST_ASSERT_EQUAL_HEX8(0x22, mock_screen[(21 * SCREEN_WIDTH_BYTES) + 2]);
    
    /* Verify the 3rd row was NOT rendered (should still be black 0x00) */
    TEST_ASSERT_EQUAL_HEX8(0x00, mock_screen[(22 * SCREEN_WIDTH_BYTES) + 2]);
}

/* Main function to run all tests */
int main(void) {
    UNITY_BEGIN();
    
    RUN_TEST(test_plot_bitmap_8_basic);
    RUN_TEST(test_plot_bitmap_8_various_heights);
    
    return UNITY_END();
}