#include "unity.h"
#include "raster.h"
#include <string.h>

#define SCREEN_SIZE_BYTES 32000
#define SCREEN_WIDTH_BYTES 80
#define SCREEN_WIDTH_PIXELS 640
#define SCREEN_HEIGHT_PIXELS 400
#define COLOR_WHITE 1
#define COLOR_BLACK 0

/* Mock screen buffer for testing */
static UINT8 mock_screen[SCREEN_SIZE_BYTES];

/* TODO: Don't hardcode the size of the sprite, can then generic for the others */
/* TODO: Optimize division and mods and such, math is slow */
/* Helper to check if a pixel at (row, col) is black (0) or white (1) */
static int get_pixel(UINT8 *base, INT16 row, INT16 col)
{
    UINT8 *byte_ptr = base + (row * SCREEN_WIDTH_BYTES) + (col / 8);
    int bit_pos = 7 - (col % 8);
    return (*byte_ptr >> bit_pos) & 1;
}

/* Setup - runs before each test */
void setUp(void)
{
    /* Initialize screen with all zeros (black) */
    memset(mock_screen, 0x00, SCREEN_SIZE_BYTES);
}

/* Teardown - runs after each test */
void tearDown(void)
{
    /* Nothing to clean up */
}

/* Test: Plot 32x32 all-white sprite at top-left */
void test_plot_bitmap_32_top_left(void)
{
    int row, col;

    /* Create a 32x32 bitmap with all bits set to 1 (white) */
    const UINT32 bitmap[32] = {
        0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
        0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
        0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
        0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
        0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
        0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
        0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
        0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF};

    /* Plot at origin (0, 0) */
    plot_bitmap_32(mock_screen, 0, 0, bitmap, 32);

    /* Verify all 32x32 pixels are white */
    for (row = 0; row < 32; row++)
    {
        for (col = 0; col < 32; col++)
        {
            TEST_ASSERT_EQUAL_INT(COLOR_WHITE, get_pixel(mock_screen, row, col));
        }
    }

    /* Verify pixels outside sprite remain black */
    TEST_ASSERT_EQUAL_INT(COLOR_BLACK, get_pixel(mock_screen, 0, 32));
    TEST_ASSERT_EQUAL_INT(COLOR_BLACK, get_pixel(mock_screen, 32, 0));
    TEST_ASSERT_EQUAL_INT(COLOR_BLACK, get_pixel(mock_screen, 32, 32));
}

/* Test: Plot 32x32 all-white sprite at top-right */
void test_plot_bitmap_32_top_right(void)
{
    int row, col;
    const INT16 start_col = SCREEN_WIDTH_PIXELS - 32;

    /* Create a 32x32 bitmap with all bits set to 1 (white) */
    const UINT32 bitmap[32] = {
        0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
        0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
        0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
        0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
        0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
        0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
        0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
        0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF};

    /* Plot at top-right */
    plot_bitmap_32(mock_screen, 0, start_col, bitmap, 32);

    /* Verify all 32x32 pixels are white */
    for (row = 0; row < 32; row++)
    {
        for (col = 0; col < 32; col++)
        {
            TEST_ASSERT_EQUAL_INT(COLOR_WHITE, get_pixel(mock_screen, row, start_col + col));
        }
    }

    /* Verify pixels just outside sprite remain black */
    TEST_ASSERT_EQUAL_INT(COLOR_BLACK, get_pixel(mock_screen, 0, start_col - 1));
    TEST_ASSERT_EQUAL_INT(COLOR_BLACK, get_pixel(mock_screen, 32, start_col));
}

/* Test: Plot 32x32 all-white sprite at bottom-left */
void test_plot_bitmap_32_bottom_left(void)
{
    int row, col;
    const INT16 start_row = SCREEN_HEIGHT_PIXELS - 32;

    /* Create a 32x32 bitmap with all bits set to 1 (white) */
    const UINT32 bitmap[32] = {
        0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
        0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
        0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
        0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
        0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
        0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
        0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
        0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF};

    /* Plot at bottom-left */
    plot_bitmap_32(mock_screen, start_row, 0, bitmap, 32);

    /* Verify all 32x32 pixels are white */
    for (row = 0; row < 32; row++)
    {
        for (col = 0; col < 32; col++)
        {
            TEST_ASSERT_EQUAL_INT(COLOR_WHITE, get_pixel(mock_screen, start_row + row, col));
        }
    }

    /* Verify pixels just outside sprite remain black */
    TEST_ASSERT_EQUAL_INT(COLOR_BLACK, get_pixel(mock_screen, start_row - 1, 0));
    TEST_ASSERT_EQUAL_INT(COLOR_BLACK, get_pixel(mock_screen, start_row, 32));
}

/* Test: Plot 32x32 all-white sprite at bottom-right */
void test_plot_bitmap_32_bottom_right(void)
{
    int row, col;
    const INT16 start_row = SCREEN_HEIGHT_PIXELS - 32;
    const INT16 start_col = SCREEN_WIDTH_PIXELS - 32;

    /* Create a 32x32 bitmap with all bits set to 1 (white) */
    const UINT32 bitmap[32] = {
        0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
        0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
        0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
        0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
        0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
        0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
        0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
        0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF};

    /* Plot at bottom-right */
    plot_bitmap_32(mock_screen, start_row, start_col, bitmap, 32);

    /* Verify all 32x32 pixels are white */
    for (row = 0; row < 32; row++)
    {
        for (col = 0; col < 32; col++)
        {
            TEST_ASSERT_EQUAL_INT(COLOR_WHITE, get_pixel(mock_screen, start_row + row, start_col + col));
        }
    }

    /* Verify pixels just outside sprite remain black */
    TEST_ASSERT_EQUAL_INT(COLOR_BLACK, get_pixel(mock_screen, start_row - 1, start_col));
    TEST_ASSERT_EQUAL_INT(COLOR_BLACK, get_pixel(mock_screen, start_row, start_col - 1));
}

/* Test: Plot 32x32 sprite partially off left edge */
void test_plot_bitmap_32_off_left_edge(void)
{
    int row, col;
    const INT16 start_col = -16; /* Half off screen to the left */

    /* Create a 32x32 bitmap with all bits set to 1 (white) */
    const UINT32 bitmap[32] = {
        0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
        0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
        0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
        0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
        0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
        0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
        0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
        0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF};

    /* Plot sprite starting at col -16 (16 pixels off screen) */
    plot_bitmap_32(mock_screen, 0, start_col, bitmap, 32);

    /* Verify only the right 16 pixels (columns 0-15) are drawn */
    for (row = 0; row < 32; row++)
    {
        for (col = 0; col < 16; col++)
        {
            TEST_ASSERT_EQUAL_INT(COLOR_WHITE, get_pixel(mock_screen, row, col));
        }
    }

    /* Verify pixels beyond visible portion remain black */
    for (row = 0; row < 32; row++)
    {
        TEST_ASSERT_EQUAL_INT(COLOR_BLACK, get_pixel(mock_screen, row, 16));
    }

    TEST_ASSERT_EQUAL_INT(COLOR_BLACK, get_pixel(mock_screen, 32, 0));
}

/* Test: Plot 32x32 sprite partially off right edge */
void test_plot_bitmap_32_off_right_edge(void)
{
    int row, col;
    const INT16 start_col = SCREEN_WIDTH_PIXELS - 16; /* Half off screen to the right */

    /* Create a 32x32 bitmap with all bits set to 1 (white) */
    const UINT32 bitmap[32] = {
        0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
        0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
        0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
        0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
        0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
        0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
        0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
        0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF};

    /* Plot sprite starting at col SCREEN_WIDTH_PIXELS - 16 (16 pixels off screen) */
    plot_bitmap_32(mock_screen, 0, start_col, bitmap, 32);

    /* Verify only the left 16 pixels are drawn (from start_col to start_col+15) */
    for (row = 0; row < 32; row++)
    {
        for (col = 0; col < 16; col++)
        {
            TEST_ASSERT_EQUAL_INT(1, get_pixel(mock_screen, row, start_col + col));
        }
    }

    /* Verify pixels before the sprite remain black */
    for (row = 0; row < 32; row++)
    {
        TEST_ASSERT_EQUAL_INT(COLOR_BLACK, get_pixel(mock_screen, row, start_col - 1));
    }

    TEST_ASSERT_EQUAL_INT(COLOR_BLACK, get_pixel(mock_screen, 32, start_col));
}

/* Main function to run all tests */
int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_plot_bitmap_32_top_left);
    RUN_TEST(test_plot_bitmap_32_top_right);
    RUN_TEST(test_plot_bitmap_32_bottom_left);
    RUN_TEST(test_plot_bitmap_32_bottom_right);
    RUN_TEST(test_plot_bitmap_32_off_left_edge);
    RUN_TEST(test_plot_bitmap_32_off_right_edge);

    return UNITY_END();
}
