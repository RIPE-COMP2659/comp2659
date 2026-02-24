#include "unity.h"
#include "raster.h"
#include <string.h>

#define SCREEN_SIZE_BYTES 32000
#define SCREEN_WIDTH_BYTES 80
#define SCREEN_HEIGHT 400

/* Mock screen buffer for testing */
static UINT8 mock_screen[SCREEN_SIZE_BYTES];

/* Helper to check if a pixel at (row, col) is black (0) or white (1) */
static int get_pixel(UINT8 *base, INT16 row, INT16 col) {
    UINT8 *byte_ptr = base + (row * SCREEN_WIDTH_BYTES) + (col / 8);
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

/* Test plotting a pixel at origin */
void test_plot_pixel_at_origin(void) {
    plot_pixel(mock_screen, 0, 0);
    
    /* Verify pixel is white */
    TEST_ASSERT_EQUAL_INT(1, get_pixel(mock_screen, 0, 0));
    
    /* Verify adjacent pixels are still black */
    TEST_ASSERT_EQUAL_INT(0, get_pixel(mock_screen, 0, 1));
    TEST_ASSERT_EQUAL_INT(0, get_pixel(mock_screen, 1, 0));
}

/* Test plotting pixels at different positions */
void test_plot_pixel_various_positions(void) {
    plot_pixel(mock_screen, 10, 20);
    plot_pixel(mock_screen, 100, 200);
    plot_pixel(mock_screen, 399, 639);
    
    /* Verify all plotted pixels are white */
    TEST_ASSERT_EQUAL_INT(1, get_pixel(mock_screen, 10, 20));
    TEST_ASSERT_EQUAL_INT(1, get_pixel(mock_screen, 100, 200));
    TEST_ASSERT_EQUAL_INT(1, get_pixel(mock_screen, 399, 639));
    
    /* Verify adjacent pixels remain black */
    TEST_ASSERT_EQUAL_INT(0, get_pixel(mock_screen, 10, 21));
    TEST_ASSERT_EQUAL_INT(0, get_pixel(mock_screen, 100, 201));
}

/* Test plotting pixels in a byte boundary */
void test_plot_pixel_byte_boundary(void) {
    /* Plot pixels at bit positions 0-7 in the same byte */
    plot_pixel(mock_screen, 50, 80);  /* bit 0 of byte 10 */
    plot_pixel(mock_screen, 50, 81);  /* bit 1 of byte 10 */
    plot_pixel(mock_screen, 50, 82);  /* bit 2 of byte 10 */
    plot_pixel(mock_screen, 50, 87);  /* bit 7 of byte 10 */
    
    /* Verify all plotted pixels are white */
    TEST_ASSERT_EQUAL_INT(1, get_pixel(mock_screen, 50, 80));
    TEST_ASSERT_EQUAL_INT(1, get_pixel(mock_screen, 50, 81));
    TEST_ASSERT_EQUAL_INT(1, get_pixel(mock_screen, 50, 82));
    TEST_ASSERT_EQUAL_INT(1, get_pixel(mock_screen, 50, 87));
    
    /* Verify unplotted pixels in same byte are black */
    TEST_ASSERT_EQUAL_INT(0, get_pixel(mock_screen, 50, 83));
    TEST_ASSERT_EQUAL_INT(0, get_pixel(mock_screen, 50, 86));
}

/* Test plotting same pixel multiple times is idempotent */
void test_plot_pixel_idempotent(void) {
    plot_pixel(mock_screen, 25, 50);
    plot_pixel(mock_screen, 25, 50);
    plot_pixel(mock_screen, 25, 50);
    
    /* Should still be white */
    TEST_ASSERT_EQUAL_INT(1, get_pixel(mock_screen, 25, 50));
}

/* Test plotting a diagonal line of pixels */
void test_plot_pixel_diagonal_pattern(void) {
    int i;
    
    /* Plot diagonal from (0,0) to (9,9) */
    for (i = 0; i < 10; i++) {
        plot_pixel(mock_screen, i, i);
    }
    
    /* Verify diagonal pixels are white */
    for (i = 0; i < 10; i++) {
        TEST_ASSERT_EQUAL_INT(1, get_pixel(mock_screen, i, i));
    }
    
    /* Verify off-diagonal pixels are black */
    TEST_ASSERT_EQUAL_INT(0, get_pixel(mock_screen, 0, 1));
    TEST_ASSERT_EQUAL_INT(0, get_pixel(mock_screen, 1, 0));
    TEST_ASSERT_EQUAL_INT(0, get_pixel(mock_screen, 5, 6));
}

/* Main function to run all tests */
int main(void) {
    UNITY_BEGIN();
    
    RUN_TEST(test_plot_pixel_at_origin);
    RUN_TEST(test_plot_pixel_various_positions);
    RUN_TEST(test_plot_pixel_byte_boundary);
    RUN_TEST(test_plot_pixel_idempotent);
    RUN_TEST(test_plot_pixel_diagonal_pattern);
    
    return UNITY_END();
}
