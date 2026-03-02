#include "unity.h"
#include "raster.h"
#include <string.h>

#define SCREEN_SIZE_BYTES 32000
#define SCREEN_WIDTH_BYTES 80
#define SCREEN_WIDTH_PIXELS 640
#define SCREEN_HEIGHT_PIXELS 400

extern void plot_square(UINT32 *base, INT16 row, INT16 col, UINT16 side);

/* Mock screen buffer for testing */
static UINT32 mock_screen[SCREEN_SIZE_BYTES / 4];

/* Helper to check the exact hex value of a byte in memory */
static UINT8 get_byte(UINT32 *base, INT16 row, INT16 col_byte_offset) {
    UINT8 *byte_ptr = (UINT8 *)base + (row * SCREEN_WIDTH_BYTES) + col_byte_offset;
    return *byte_ptr;
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

/* Test basic square drawing (using the 48x48 optimized path) */
void test_plot_square_basic_48(void) {
    int r;
    
    /* Plot a 48x48 square at row 10, col 16 (byte offset 2) */
    plot_square(mock_screen, 10, 16, 48);

    /* Verify the 48 rows (10 through 57) have 6 solid bytes (0xFF) */
    for (r = 10; r < 58; r++) {
        TEST_ASSERT_EQUAL_HEX8(0xFF, get_byte(mock_screen, r, 2));
        TEST_ASSERT_EQUAL_HEX8(0xFF, get_byte(mock_screen, r, 3));
        TEST_ASSERT_EQUAL_HEX8(0xFF, get_byte(mock_screen, r, 4));
        TEST_ASSERT_EQUAL_HEX8(0xFF, get_byte(mock_screen, r, 5));
        TEST_ASSERT_EQUAL_HEX8(0xFF, get_byte(mock_screen, r, 6));
        TEST_ASSERT_EQUAL_HEX8(0xFF, get_byte(mock_screen, r, 7));
        
        /* Ensure the bounds on the sides remain untouched */
        TEST_ASSERT_EQUAL_HEX8(0x00, get_byte(mock_screen, r, 1));
        TEST_ASSERT_EQUAL_HEX8(0x00, get_byte(mock_screen, r, 8));
    }
}

/* Test squares with different side lengths (forces the unoptimized path) */
void test_plot_square_various_sizes(void) {
    int r;
    
    /* Plot a 16x16 square at row 20, col 32 (byte offset 4) */
    plot_square(mock_screen, 20, 32, 16);

    /* Verify the 16 rows (20 through 35) have 2 solid bytes (0xFF) */
    for (r = 20; r < 36; r++) {
        TEST_ASSERT_EQUAL_HEX8(0xFF, get_byte(mock_screen, r, 4));
        TEST_ASSERT_EQUAL_HEX8(0xFF, get_byte(mock_screen, r, 5));
        
        /* Ensure the bounds on the sides remain untouched */
        TEST_ASSERT_EQUAL_HEX8(0x00, get_byte(mock_screen, r, 3));
        TEST_ASSERT_EQUAL_HEX8(0x00, get_byte(mock_screen, r, 6));
    }
}

/* Test all bounds clipping limits mathematically */
void test_plot_square_clipping(void) {
    /* 1. Top Clip: Plot 48x48 at row -10, col 0. 
       Should clip the top 10 rows, leaving a 48x38 rectangle starting at row 0. 
       Because width is still 48, it uses the optimized path! */
    plot_square(mock_screen, -10, 0, 48);
    TEST_ASSERT_EQUAL_HEX8(0xFF, get_byte(mock_screen, 0, 0));
    TEST_ASSERT_EQUAL_HEX8(0xFF, get_byte(mock_screen, 37, 0));
    TEST_ASSERT_EQUAL_HEX8(0x00, get_byte(mock_screen, 38, 0)); /* Row 38 remains black */

    /* 2. Bottom Clip: Plot 16x16 at row 390, col 16.
       Should clip the bottom 6 rows, leaving a 16x10 rectangle. */
    plot_square(mock_screen, 390, 16, 16);
    TEST_ASSERT_EQUAL_HEX8(0xFF, get_byte(mock_screen, 390, 2));
    TEST_ASSERT_EQUAL_HEX8(0xFF, get_byte(mock_screen, 399, 2));

    /* 3. Left Clip: Plot 48x48 at row 100, col -16.
       Should clip 2 bytes (16 pixels) off the left. 
       Because width is now 32, it will seamlessly fall back to the UNOPTIMIZED path! */
    plot_square(mock_screen, 100, -16, 48);
    TEST_ASSERT_EQUAL_HEX8(0xFF, get_byte(mock_screen, 100, 0)); /* Byte offset 0 (cols 0-7) is drawn */
    TEST_ASSERT_EQUAL_HEX8(0xFF, get_byte(mock_screen, 100, 3)); /* Byte offset 3 (cols 24-31) is drawn */
    TEST_ASSERT_EQUAL_HEX8(0x00, get_byte(mock_screen, 100, 4)); /* Byte offset 4 (cols 32-39) remains black */

    /* 4. Right Clip: Plot 16x16 at row 200, col 632.
       Should clip the rightmost 8 pixels (1 byte). */
    plot_square(mock_screen, 200, 632, 16);
    TEST_ASSERT_EQUAL_HEX8(0xFF, get_byte(mock_screen, 200, 79)); /* Byte 79 (last byte on screen) is drawn */
    
    /* Note: If the code tried to write to byte 80, it would wrap around to the next line 
       or cause a fault. We verify the first byte of the NEXT row is completely black. */
    TEST_ASSERT_EQUAL_HEX8(0x00, get_byte(mock_screen, 201, 0));
}

/* Main function to run all tests */
int main(void) {
    UNITY_BEGIN();
    
    RUN_TEST(test_plot_square_basic_48);
    RUN_TEST(test_plot_square_various_sizes);
    RUN_TEST(test_plot_square_clipping);
    
    return UNITY_END();
}
