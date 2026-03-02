#include "unity.h"
#include "raster.h"
#include <string.h>

#define SCREEN_SIZE_BYTES 32000
#define SCREEN_WIDTH_BYTES 80
#define SCREEN_HEIGHT 400

/* Prototype for the assembly routine being tested */
extern void plot_clipped_bitmap(UINT8 *base, UINT16 row, UINT16 col, const void *bitmap, 
                                UINT16 height, UINT16 width, UINT16 status, UINT16 new_width);

/* Mock screen buffer for testing */
static UINT8 mock_screen[SCREEN_SIZE_BYTES];

/* Sample 16-pixel (2 byte) wide bitmap data */
static const UINT8 sample_bitmap_16[] = {
    0xAA, 0xBB,  /* Row 0: 10101010 10111011 */
    0xCC, 0xDD,  /* Row 1: 11001100 11011101 */
    0xEE, 0xFF   /* Row 2: 11101110 11111111 */
};

/* Helper to check a specific byte in the mock screen */
static UINT8 get_byte(int row, int col_byte) {
    if (row < 0 || row >= SCREEN_HEIGHT || col_byte < 0 || col_byte >= SCREEN_WIDTH_BYTES)
        return 0;
    return mock_screen[(row * SCREEN_WIDTH_BYTES) + col_byte];
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

/* Test Left Clipping (Status 1)
 * Scenario: Bitmap is 16 pixels wide (2 bytes). 
 * Col is -8 (exactly 1 byte off screen).
 * Status 1 should skip the first byte of each row and plot the second byte at col 0.
 */
void test_plot_clip_left_edge(void) {
    /* Parameters: 
       row=10, col=-8, height=3, width=16, status=1 (Left), new_width=8 
    */
    plot_clipped_bitmap(mock_screen, 10, -8, sample_bitmap_16, 3, 16, 1, 8);

    /* Verify Row 10: Byte 0 should contain 0xBB (the second byte of bitmap Row 0) */
    TEST_ASSERT_EQUAL_HEX8_MESSAGE(0xBB, get_byte(10, 0), "Left clip failed to skip first byte of bitmap");
    
    /* Verify Row 11: Byte 0 should contain 0xDD */
    TEST_ASSERT_EQUAL_HEX8(0xDD, get_byte(11, 0));

    /* Verify Row 12: Byte 0 should contain 0xFF */
    TEST_ASSERT_EQUAL_HEX8(0xFF, get_byte(12, 0));

    /* Verify that Byte 1 (col 8) is still 0x00 */
    TEST_ASSERT_EQUAL_HEX8(0x00, get_byte(10, 1));
}

/* Test Right Clipping (Status 2)
 * Scenario: Bitmap is 16 pixels wide (2 bytes).
 * Col is 632 (Byte 79). 
 * Status 2 should only plot the first byte of the bitmap and clip the rest.
 */
void test_plot_clip_right_edge(void) {
    /* Parameters: 
       row=20, col=632, height=3, width=16, status=2 (Right), new_width=8 
    */
    plot_clipped_bitmap(mock_screen, 20, 632, sample_bitmap_16, 3, 16, 2, 8);

    /* Verify Row 20: Byte 79 should contain 0xAA (the first byte of bitmap Row 0) */
    TEST_ASSERT_EQUAL_HEX8_MESSAGE(0xAA, get_byte(20, 79), "Right clip failed to truncate bitmap width");

    /* Verify Row 21: Byte 79 should contain 0xCC */
    TEST_ASSERT_EQUAL_HEX8(0xCC, get_byte(21, 79));

    /* Verify adjacent memory (start of next row) is untouched */
    TEST_ASSERT_EQUAL_HEX8(0x00, get_byte(21, 0));
}

/* Test basic clipping logic (Status 2 used for partial width in middle of screen)
 * This verifies that the 'do_plot' subroutine respects 'new_width'.
 */
void test_plot_clip_partial_width(void) {
    /* Plot only the first 8 pixels of a 16 pixel bitmap at (50, 80) */
    plot_clipped_bitmap(mock_screen, 50, 80, sample_bitmap_16, 1, 16, 2, 8);

    /* Byte offset for col 80 is 10. */
    TEST_ASSERT_EQUAL_HEX8(0xAA, get_byte(50, 10));
    /* Byte offset 11 should be empty because we clipped the width to 8 pixels */
    TEST_ASSERT_EQUAL_HEX8(0x00, get_byte(50, 11));
}

/* Main function to run all tests */
int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_plot_clip_left_edge);
    RUN_TEST(test_plot_clip_right_edge);
    RUN_TEST(test_plot_clip_partial_width);

    return UNITY_END();
}