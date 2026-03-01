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
extern void plot_rectangle(UINT32 *base, INT16 row, INT16 col, UINT16 length, UINT16 width);

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

/* Test basic rectangle drawing (using the 48-width optimized path) */
void test_plot_rectangle_basic(void) {
    int r;
    
    /* Plot a 10(height) x 48(width) rectangle at row 5, col 16 (byte offset 2) */
    plot_rectangle(mock_screen, 5, 16, 10, 48);

    /* Verify the 10 rows (5 through 14) have 6 solid bytes (0xFF) */
    for (r = 5; r < 15; r++) {
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

/* Test rectangle where length equals width (square shape, unoptimized path) */
void test_plot_rectangle_square_shape(void) {
    int r;
    
    /* Plot a 16x16 square at row 20, col 32 (byte offset 4) */
    plot_rectangle(mock_screen, 20, 32, 16, 16);

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
void test_plot_rectangle_clipping(void) {
    /* 1. Top Clip: Plot 20x48 at row -10, col 0. 
       Should clip the top 10 rows, leaving a 10x48 rectangle starting at row 0. 
       Because width is still 48, it uniquely maintains the optimized path! */
    plot_rectangle(mock_screen, -10, 0, 20, 48);
    TEST_ASSERT_EQUAL_HEX8(0xFF, get_byte(mock_screen, 0, 0));
    TEST_ASSERT_EQUAL_HEX8(0xFF, get_byte(mock_screen, 9, 0));
    TEST_ASSERT_EQUAL_HEX8(0x00, get_byte(mock_screen, 10, 0)); /* Row 10 remains black */

    /* 2. Bottom Clip: Plot 20x16 at row 390, col 16.
       Should clip the bottom 10 rows, leaving a 10x16 rectangle. */
    plot_rectangle(mock_screen, 390, 16, 20, 16);
    TEST_ASSERT_EQUAL_HEX8(0xFF, get_byte(mock_screen, 390, 2));
    TEST_ASSERT_EQUAL_HEX8(0xFF, get_byte(mock_screen, 399, 2));

    /* 3. Left Clip: Plot 10x48 at row 100, col -16.
       Should clip 2 bytes (16 pixels) off the left. 
       Because width is now 32, it seamlessly falls back to the UNOPTIMIZED path! */
    plot_rectangle(mock_screen, 100, -16, 10, 48);
    TEST_ASSERT_EQUAL_HEX8(0xFF, get_byte(mock_screen, 100, 0)); /* Byte offset 0 (cols 0-7) is drawn */
    TEST_ASSERT_EQUAL_HEX8(0xFF, get_byte(mock_screen, 100, 3)); /* Byte offset 3 (cols 24-31) is drawn */
    TEST_ASSERT_EQUAL_HEX8(0x00, get_byte(mock_screen, 100, 4)); /* Byte offset 4 (cols 32-39) remains black */

    /* 4. Right Clip: Plot 10x32 at row 200, col 624.
       Should clip the rightmost 16 pixels (2 bytes). */
    plot_rectangle(mock_screen, 200, 624, 10, 32);
    TEST_ASSERT_EQUAL_HEX8(0xFF, get_byte(mock_screen, 200, 78)); /* Byte 78 is drawn */
    TEST_ASSERT_EQUAL_HEX8(0xFF, get_byte(mock_screen, 200, 79)); /* Byte 79 (last byte on screen) is drawn */
    
    /* Ensure the bounds on the next line are totally untouched, proving no overflow */
    TEST_ASSERT_EQUAL_HEX8(0x00, get_byte(mock_screen, 201, 0));
}

/* Main function to run all tests */
int main(void) {
    UNITY_BEGIN();
    
    RUN_TEST(test_plot_rectangle_basic);
    RUN_TEST(test_plot_rectangle_square_shape);
    RUN_TEST(test_plot_rectangle_clipping);
    
    return UNITY_END();
}