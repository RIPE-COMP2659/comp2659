#include "unity.h"
#include "raster.h"
#include <string.h>

#define SCREEN_SIZE_BYTES 32000
#define SCREEN_SIZE_LONGS (SCREEN_SIZE_BYTES / 4)
#define SCREEN_WIDTH_BYTES 80
#define SCREEN_HEIGHT 400

/* Mock screen buffer for testing */
static UINT32 mock_screen[SCREEN_SIZE_LONGS];

/* Helper to check if a pixel at (row, col) is black (0) or white (1) */
static int get_pixel(UINT8 *base, INT16 row, INT16 col) {
    UINT8 *byte_ptr = base + (row * SCREEN_WIDTH_BYTES) + (col / 8);
    int bit_pos = 7 - (col % 8);
    return (*byte_ptr >> bit_pos) & 1;
}

/* Setup - runs before each test */
void setUp(void) {
    /* Initialize screen with all white */
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
    
    /* Verify the region is black */
    TEST_ASSERT_EQUAL_INT(0, get_pixel(base, 0, 0));
    TEST_ASSERT_EQUAL_INT(0, get_pixel(base, 0, 15));
    TEST_ASSERT_EQUAL_INT(0, get_pixel(base, 15, 0));
    TEST_ASSERT_EQUAL_INT(0, get_pixel(base, 15, 15));
    
    /* Verify pixels outside the region are still white */
    TEST_ASSERT_EQUAL_INT(1, get_pixel(base, 0, 16));
    TEST_ASSERT_EQUAL_INT(1, get_pixel(base, 16, 0));
}

/* Test clearing a region with odd dimensions */
void test_clear_region_odd_dimensions(void) {
    UINT8 *base = (UINT8 *)mock_screen;
    
    /* Clear a 13x7 region at (10,5) */
    clear_region(mock_screen, 10, 5, 13, 7);
    
    /* Check corners of cleared region */
    TEST_ASSERT_EQUAL_INT(0, get_pixel(base, 10, 5));
    TEST_ASSERT_EQUAL_INT(0, get_pixel(base, 10, 11));
    TEST_ASSERT_EQUAL_INT(0, get_pixel(base, 22, 5));
    TEST_ASSERT_EQUAL_INT(0, get_pixel(base, 22, 11));
    
    /* Check pixels just outside the region */
    TEST_ASSERT_EQUAL_INT(1, get_pixel(base, 9, 5));   /* Above */
    TEST_ASSERT_EQUAL_INT(1, get_pixel(base, 10, 4));  /* Left */
    TEST_ASSERT_EQUAL_INT(1, get_pixel(base, 10, 12)); /* Right */
    TEST_ASSERT_EQUAL_INT(1, get_pixel(base, 23, 5));  /* Below */
}

/* Test clearing a 48x48 optimized region (word-aligned) */
void test_clear_region_48x48_optimized(void) {
    UINT8 *base = (UINT8 *)mock_screen;
    
    /* Clear a 48x48 region at word-aligned position (0,0) */
    clear_region(mock_screen, 0, 0, 48, 48);
    
    /* Verify corners are black */
    TEST_ASSERT_EQUAL_INT(0, get_pixel(base, 0, 0));
    TEST_ASSERT_EQUAL_INT(0, get_pixel(base, 0, 47));
    TEST_ASSERT_EQUAL_INT(0, get_pixel(base, 47, 0));
    TEST_ASSERT_EQUAL_INT(0, get_pixel(base, 47, 47));
    
    /* Verify outside is white */
    TEST_ASSERT_EQUAL_INT(1, get_pixel(base, 0, 48));
    TEST_ASSERT_EQUAL_INT(1, get_pixel(base, 48, 0));
}

/* Test clearing a single pixel region */
void test_clear_region_single_pixel(void) {
    UINT8 *base = (UINT8 *)mock_screen;
    
    /* Clear a 1x1 region at (50,100) */
    clear_region(mock_screen, 50, 100, 1, 1);
    
    /* Verify the pixel is black */
    TEST_ASSERT_EQUAL_INT(0, get_pixel(base, 50, 100));
    
    /* Verify adjacent pixels are white */
    TEST_ASSERT_EQUAL_INT(1, get_pixel(base, 50, 99));
    TEST_ASSERT_EQUAL_INT(1, get_pixel(base, 50, 101));
    TEST_ASSERT_EQUAL_INT(1, get_pixel(base, 49, 100));
    TEST_ASSERT_EQUAL_INT(1, get_pixel(base, 51, 100));
}

/* Main function to run all tests */
int main(void) {
    UNITY_BEGIN();
    
    RUN_TEST(test_clear_region_at_origin);
    RUN_TEST(test_clear_region_odd_dimensions);
    RUN_TEST(test_clear_region_48x48_optimized);
    RUN_TEST(test_clear_region_single_pixel);
    
    return UNITY_END();
}
