#include "unity.h"
#include "raster.h"
#include <string.h>

#define WHITE 0
#define BLACK 1

#define SCREEN_SIZE_BYTES 32000
#define SCREEN_SIZE_LONGS (SCREEN_SIZE_BYTES / 4)
#define SCREEN_WIDTH_BYTES 80
#define SCREEN_HEIGHT 400

/* Mock screen buffer for testing */
static UINT32 mock_screen[SCREEN_SIZE_LONGS];

extern void clear_region(UINT32 *base, INT16 row, INT16 col, UINT16 length, UINT16 width);

/* Helper to check if a pixel at (row, col) is white (0) or black (1) */
static int get_pixel(UINT8 *base, INT16 row, INT16 col) {
    UINT8 *byte_ptr = base + (row * SCREEN_WIDTH_BYTES) + (col / 8);
    int bit_pos = 7 - (col % 8);
    return (*byte_ptr >> bit_pos) & 1;
}

/* Helper to set a pixel at (row, col) to 0 or 1 */
static void set_pixel(UINT8 *base, INT16 row, INT16 col, int value) {
    UINT8 *byte_ptr = base + (row * SCREEN_WIDTH_BYTES) + (col / 8);
    UINT8 bit_mask = (UINT8)(1u << (7 - (col % 8)));

    if (value) {
        *byte_ptr |= bit_mask;
    } else {
        *byte_ptr &= (UINT8)(~bit_mask);
    }
}

/* Seed border, clear region, and verify clear/boundary behavior. */
static void clear_region_with_black_border_assertion(
    UINT8 *base,
    INT16 row,
    INT16 col,
    UINT16 length,
    UINT16 width
) {
    INT16 y;
    INT16 x;

    for (y = row - 1; y < (INT16)(row + length + 1); y++) {
        for (x = col - 1; x < (INT16)(col + width + 1); x++) {
            set_pixel(base, y, x, BLACK);
        }
    }

    clear_region(mock_screen, row, col, length, width);

    for (y = row; y < (INT16)(row + length); y++) {
        TEST_ASSERT_EQUAL_INT_MESSAGE(BLACK, get_pixel(base, y, col - 1),
            "Left boundary column cleared incorrectly");
        TEST_ASSERT_EQUAL_INT_MESSAGE(BLACK, get_pixel(base, y, col + width),
            "Right boundary column cleared incorrectly");
    }

    for (x = col; x < (INT16)(col + width); x++) {
        TEST_ASSERT_EQUAL_INT_MESSAGE(BLACK, get_pixel(base, row - 1, x),
            "Top boundary row cleared incorrectly");
        TEST_ASSERT_EQUAL_INT_MESSAGE(BLACK, get_pixel(base, row + length, x),
            "Bottom boundary row cleared incorrectly");
    }

    for (y = row; y < (INT16)(row + length); y++) {
        for (x = col; x < (INT16)(col + width); x++) {
            TEST_ASSERT_EQUAL_INT_MESSAGE(WHITE, get_pixel(base, y, x),
                "Pixel in clear remained");
        }
    }
}

/* Setup - runs before each test */
void setUp(void) {
    /* Initialize screen with all black */
    memset(mock_screen, 0xFF, SCREEN_SIZE_BYTES);
}

/* Teardown - runs after each test */
void tearDown(void) {
    /* Nothing to clean up */
}

/* Test clearing a simple region at origin */
void test_clear_region_at_origin(void) {
    UINT8 *base = (UINT8 *)mock_screen;
    
    /* Clear a 16x16 region at (0,0) */
    clear_region(mock_screen, 0, 0, 16, 16);
    
    /* Verify the region is white */
    TEST_ASSERT_EQUAL_INT(WHITE, get_pixel(base, 0, 0));
    TEST_ASSERT_EQUAL_INT(WHITE, get_pixel(base, 0, 15));
    TEST_ASSERT_EQUAL_INT(WHITE, get_pixel(base, 15, 0));
    TEST_ASSERT_EQUAL_INT(WHITE, get_pixel(base, 15, 15));
    
    /* Verify pixels outside the region are still black */
    TEST_ASSERT_EQUAL_INT(BLACK, get_pixel(base, 0, 16));
    TEST_ASSERT_EQUAL_INT(BLACK, get_pixel(base, 16, 0));
}

/* Test clearing a region with odd dimensions */
void test_clear_region_odd_dimensions(void) {
    UINT8 *base = (UINT8 *)mock_screen;
    
    /* Clear a 13x7 region at (10,5) */
    clear_region(mock_screen, 10, 5, 13, 7);
    
    /* Check corners of cleared region */
    TEST_ASSERT_EQUAL_INT(WHITE, get_pixel(base, 10, 5));
    TEST_ASSERT_EQUAL_INT(WHITE, get_pixel(base, 10, 11));
    TEST_ASSERT_EQUAL_INT(WHITE, get_pixel(base, 22, 5));
    TEST_ASSERT_EQUAL_INT(WHITE, get_pixel(base, 22, 11));
    
    /* Check pixels just outside the region */
    TEST_ASSERT_EQUAL_INT(BLACK, get_pixel(base, 9, 5));   /* Above */
    TEST_ASSERT_EQUAL_INT(BLACK, get_pixel(base, 10, 4));  /* Left */
    TEST_ASSERT_EQUAL_INT(BLACK, get_pixel(base, 10, 12)); /* Right */
    TEST_ASSERT_EQUAL_INT(BLACK, get_pixel(base, 23, 5));  /* Below */
}

/* Test clearing a 48x48 optimized region (word-aligned) */
void test_clear_region_48x48_optimized(void) {
    UINT8 *base = (UINT8 *)mock_screen;
    
    /* Clear a 48x48 region at word-aligned position (0,0). */
    clear_region(mock_screen, 0, 0, 48, 48);
    
    /* Verify corners are white */
    TEST_ASSERT_EQUAL_INT(WHITE, get_pixel(base, 0, 0));
    TEST_ASSERT_EQUAL_INT(WHITE, get_pixel(base, 0, 47));
    TEST_ASSERT_EQUAL_INT(WHITE, get_pixel(base, 47, 0));
    TEST_ASSERT_EQUAL_INT(WHITE, get_pixel(base, 47, 47));
    
    /* Verify outside is black */
    TEST_ASSERT_EQUAL_INT(BLACK, get_pixel(base, 0, 48));
    TEST_ASSERT_EQUAL_INT(BLACK, get_pixel(base, 48, 0));
}

/* Test clearing a single pixel region */
void test_clear_region_single_pixel(void) {
    UINT8 *base = (UINT8 *)mock_screen;
    
    /* Clear a 1x1 region at (50,100) */
    clear_region(mock_screen, 50, 100, 1, 1);
    
    /* Verify the pixel is white */
    TEST_ASSERT_EQUAL_INT(WHITE, get_pixel(base, 50, 100));
    
    /* Verify adjacent pixels are black */
    TEST_ASSERT_EQUAL_INT(BLACK, get_pixel(base, 50, 99));
    TEST_ASSERT_EQUAL_INT(BLACK, get_pixel(base, 50, 101));
    TEST_ASSERT_EQUAL_INT(BLACK, get_pixel(base, 49, 100));
    TEST_ASSERT_EQUAL_INT(BLACK, get_pixel(base, 51, 100));
}

/**
 * This test ensures a previous bug is caught. When region was 32 pixels wide
 * it would assume that it was also byte aligned, and it would not properly
 * clear the trailing pixels / ones to the right.
 */
void test_clear_region_width32_non_byte_aligned_column(void) {
    UINT8 *base = (UINT8 *)mock_screen;
    const INT16 region_row = 20;
    const INT16 region_col = 1;
    const UINT16 region_length = 32;
    const UINT16 region_width = 32;

    clear_region_with_black_border_assertion(base, region_row, region_col, region_length, region_width);
}

/* Test all bounds clipping limits mathematically */
void test_clear_region_clipping(void) {
    UINT8 *base = (UINT8 *)mock_screen;

    /* 1. Top Clip: Clear 48x48 at row -10, col 0. 
       Should clip the top 10 rows, leaving a 38x48 region starting at row 0. 
       Because width is still 48, it seamlessly stays in the optimized path! */
    clear_region(mock_screen, -10, 0, 48, 48);
    TEST_ASSERT_EQUAL_INT(WHITE, get_pixel(base, 0, 0));
    TEST_ASSERT_EQUAL_INT(WHITE, get_pixel(base, 37, 0));
    TEST_ASSERT_EQUAL_INT(BLACK, get_pixel(base, 38, 0)); /* Row 38 remains black */

    /* 2. Bottom Clip: Clear 20x16 at row 390, col 16.
       Should clip the bottom 10 rows, leaving a 10x16 region. */
    clear_region(mock_screen, 390, 16, 20, 16);
    TEST_ASSERT_EQUAL_INT(WHITE, get_pixel(base, 390, 16));
    TEST_ASSERT_EQUAL_INT(WHITE, get_pixel(base, 399, 16));

    /* 3. Left Clip: Clear 10x48 at row 100, col -16.
       Should clip 16 pixels off the left, leaving a 32-pixel wide region.
       It dynamically adjusts width to 32 and uses the unoptimized generic routine. */
    clear_region(mock_screen, 100, -16, 10, 48);
    TEST_ASSERT_EQUAL_INT(WHITE, get_pixel(base, 100, 0));  /* Start of screen is cleared */
    TEST_ASSERT_EQUAL_INT(WHITE, get_pixel(base, 100, 31)); /* Last column of new width 32 */
    TEST_ASSERT_EQUAL_INT(BLACK, get_pixel(base, 100, 32)); /* Pixel 32 remains black */

    /* 4. Right Clip: Clear 10x48 at row 200, col 624.
       Should clip the rightmost 32 pixels, leaving a 16-pixel wide region. */
    clear_region(mock_screen, 200, 624, 10, 48);
    TEST_ASSERT_EQUAL_INT(WHITE, get_pixel(base, 200, 624)); 
    TEST_ASSERT_EQUAL_INT(WHITE, get_pixel(base, 200, 639)); /* Last pixel on screen is cleared */
    
    /* Ensure the bounds on the next line are totally untouched, proving no overflow */
    TEST_ASSERT_EQUAL_INT(BLACK, get_pixel(base, 201, 0));
}

/* Main function to run all tests */
int main(void) {
    UNITY_BEGIN();
    
    RUN_TEST(test_clear_region_at_origin);
    RUN_TEST(test_clear_region_odd_dimensions);
    RUN_TEST(test_clear_region_48x48_optimized);
    RUN_TEST(test_clear_region_single_pixel);
    RUN_TEST(test_clear_region_width32_non_byte_aligned_column);
    /* RUN_TEST(test_clear_region_clipping); */
    
    return UNITY_END();
}
