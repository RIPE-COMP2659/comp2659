#include "unity.h"
#include "raster.h"
#include <string.h>

#define SCREEN_SIZE_BYTES 32000
#define SCREEN_WIDTH_BYTES 80
#define SCREEN_WIDTH_PIXELS 640
#define SCREEN_HEIGHT_PIXELS 400
#define WHITE 0
#define BLACK 1

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
static void assert_borders(
    UINT8 *base,
    INT16 row,
    INT16 col,
    UINT16 length,
    UINT16 width
) {
    /* TODO: Logic is garbage, should be using ternary and cleaner loops */
    INT16 y;
    INT16 x;
    const INT16 left_col = col - 1;
    const INT16 right_col = col + width;
    const INT16 top_row = row - 1;
    const INT16 bottom_row = row + length;

    for (y = row; y < (INT16)(row + length); y++) {
        if (y >= 0 && y < SCREEN_HEIGHT_PIXELS) {
            if (left_col >= 0 && left_col < SCREEN_WIDTH_PIXELS) {
                TEST_ASSERT_EQUAL_INT_MESSAGE(WHITE, get_pixel(base, y, left_col),
                    "Left boundary column cleared incorrectly");
            }

            if (right_col >= 0 && right_col < SCREEN_WIDTH_PIXELS) {
                TEST_ASSERT_EQUAL_INT_MESSAGE(WHITE, get_pixel(base, y, right_col),
                    "Right boundary column cleared incorrectly");
            }
        }
    }

    for (x = col; x < (INT16)(col + width); x++) {
        if (x >= 0 && x < SCREEN_WIDTH_PIXELS) {
            if (top_row >= 0 && top_row < SCREEN_HEIGHT_PIXELS) {
                TEST_ASSERT_EQUAL_INT_MESSAGE(WHITE, get_pixel(base, top_row, x),
                    "Top boundary row cleared incorrectly");
            }

            if (bottom_row >= 0 && bottom_row < SCREEN_HEIGHT_PIXELS) {
                TEST_ASSERT_EQUAL_INT_MESSAGE(WHITE, get_pixel(base, bottom_row, x),
                    "Bottom boundary row cleared incorrectly");
            }
        }
    }
}

/* Read one pixel from a 32-bit bitmap row using only division/modulo math. */
static int get_bitmap_32_pixel(const UINT32 *bitmap, INT16 row, INT16 col)
{
    UINT32 value = bitmap[row];
    INT16 step;

    for (step = 0; step < (INT16)(31 - col); step++) {
        /* 2u is an unsigned int 2, avoids conversion warnings */
        value /= 2u;
    }

    return (value % 2u) ? BLACK : WHITE;
}

/* Verify bitmap pixels exactly match expected data, with screen bounds clipping. */
static void verify_bitmap_32_pixels(
    UINT8 *base,
    INT16 row,
    INT16 col,
    const UINT32 *bitmap,
    UINT16 height
) {
    INT16 src_row;
    INT16 src_col;

    for (src_row = 0; src_row < (INT16)height; src_row++) {
        const INT16 dst_row = row + src_row;

        if (dst_row >= 0 && dst_row < SCREEN_HEIGHT_PIXELS) {
            for (src_col = 0; src_col < 32; src_col++) {
                const INT16 dst_col = col + src_col;
                const int expected = get_bitmap_32_pixel(bitmap, src_row, src_col);

                if (dst_col >= 0 && dst_col < SCREEN_WIDTH_PIXELS) {
                    TEST_ASSERT_EQUAL_INT_MESSAGE(expected, get_pixel(base, dst_row, dst_col),
                        "Bitmap pixel mismatch");
                }
            }
        }
    }
}

/* Setup - runs before each test */
void setUp(void)
{
    /* Initialize screen with all white */
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
    const UINT32 bitmap[32] = {
        0xAAAAAAAA, 0xAAAAAAAA, 0xAAAAAAAA, 0xAAAAAAAA,
        0x55555555, 0x55555555, 0x55555555, 0x55555555,
        0xAAAAAAAA, 0xAAAAAAAA, 0xAAAAAAAA, 0xAAAAAAAA,
        0x55555555, 0x55555555, 0x55555555, 0x55555555,
        0xAAAAAAAA, 0xAAAAAAAA, 0xAAAAAAAA, 0xAAAAAAAA,
        0x55555555, 0x55555555, 0x55555555, 0x55555555,
        0xAAAAAAAA, 0xAAAAAAAA, 0xAAAAAAAA, 0xAAAAAAAA,
        0x55555555, 0x55555555, 0x55555555, 0x55555555
    };

    /* Plot at origin (0, 0) */
    plot_bitmap_32(mock_screen, 0, 0, bitmap, 32);
    assert_borders((UINT8 *)mock_screen, 0, 0, 32, 32);
    verify_bitmap_32_pixels((UINT8 *)mock_screen, 0, 0, bitmap, 32);
}

/* Test: Plot 32x32 all-white sprite at top-right */
void test_plot_bitmap_32_top_right(void)
{
    const INT16 start_col = SCREEN_WIDTH_PIXELS - 32;

    const UINT32 bitmap[32] = {
        0xAAAAAAAA, 0xAAAAAAAA, 0xAAAAAAAA, 0xAAAAAAAA,
        0x55555555, 0x55555555, 0x55555555, 0x55555555,
        0xAAAAAAAA, 0xAAAAAAAA, 0xAAAAAAAA, 0xAAAAAAAA,
        0x55555555, 0x55555555, 0x55555555, 0x55555555,
        0xAAAAAAAA, 0xAAAAAAAA, 0xAAAAAAAA, 0xAAAAAAAA,
        0x55555555, 0x55555555, 0x55555555, 0x55555555,
        0xAAAAAAAA, 0xAAAAAAAA, 0xAAAAAAAA, 0xAAAAAAAA,
        0x55555555, 0x55555555, 0x55555555, 0x55555555
    };

    /* Plot at top-right */
    plot_bitmap_32(mock_screen, 0, start_col, bitmap, 32);
    assert_borders((UINT8 *)mock_screen, 0, start_col, 32, 32);
    verify_bitmap_32_pixels((UINT8 *)mock_screen, 0, start_col, bitmap, 32);
}

/* Test: Plot 32x32 all-white sprite at bottom-left */
void test_plot_bitmap_32_bottom_left(void)
{
    const INT16 start_row = SCREEN_HEIGHT_PIXELS - 32;

    const UINT32 bitmap[32] = {
        0xAAAAAAAA, 0xAAAAAAAA, 0xAAAAAAAA, 0xAAAAAAAA,
        0x55555555, 0x55555555, 0x55555555, 0x55555555,
        0xAAAAAAAA, 0xAAAAAAAA, 0xAAAAAAAA, 0xAAAAAAAA,
        0x55555555, 0x55555555, 0x55555555, 0x55555555,
        0xAAAAAAAA, 0xAAAAAAAA, 0xAAAAAAAA, 0xAAAAAAAA,
        0x55555555, 0x55555555, 0x55555555, 0x55555555,
        0xAAAAAAAA, 0xAAAAAAAA, 0xAAAAAAAA, 0xAAAAAAAA,
        0x55555555, 0x55555555, 0x55555555, 0x55555555
    };

    /* Plot at bottom-left */
    plot_bitmap_32(mock_screen, start_row, 0, bitmap, 32);
    assert_borders((UINT8 *)mock_screen, start_row, 0, 32, 32);
    verify_bitmap_32_pixels((UINT8 *)mock_screen, start_row, 0, bitmap, 32);
}

/* Test: Plot 32x32 all-white sprite at bottom-right */
void test_plot_bitmap_32_bottom_right(void)
{
    const INT16 start_row = SCREEN_HEIGHT_PIXELS - 32;
    const INT16 start_col = SCREEN_WIDTH_PIXELS - 32;

    const UINT32 bitmap[32] = {
        0xAAAAAAAA, 0xAAAAAAAA, 0xAAAAAAAA, 0xAAAAAAAA,
        0x55555555, 0x55555555, 0x55555555, 0x55555555,
        0xAAAAAAAA, 0xAAAAAAAA, 0xAAAAAAAA, 0xAAAAAAAA,
        0x55555555, 0x55555555, 0x55555555, 0x55555555,
        0xAAAAAAAA, 0xAAAAAAAA, 0xAAAAAAAA, 0xAAAAAAAA,
        0x55555555, 0x55555555, 0x55555555, 0x55555555,
        0xAAAAAAAA, 0xAAAAAAAA, 0xAAAAAAAA, 0xAAAAAAAA,
        0x55555555, 0x55555555, 0x55555555, 0x55555555
    };

    /* Plot at bottom-right */
    plot_bitmap_32(mock_screen, start_row, start_col, bitmap, 32);
    assert_borders((UINT8 *)mock_screen, start_row, start_col, 32, 32);
    verify_bitmap_32_pixels((UINT8 *)mock_screen, start_row, start_col, bitmap, 32);
}

/* Test: Plot 32x32 sprite partially off left edge */
void test_plot_bitmap_32_off_left_edge(void)
{
    const INT16 start_col = -16; /* Half off screen to the left */

    const UINT32 bitmap[32] = {
        0xAAAAAAAA, 0xAAAAAAAA, 0xAAAAAAAA, 0xAAAAAAAA,
        0x55555555, 0x55555555, 0x55555555, 0x55555555,
        0xAAAAAAAA, 0xAAAAAAAA, 0xAAAAAAAA, 0xAAAAAAAA,
        0x55555555, 0x55555555, 0x55555555, 0x55555555,
        0xAAAAAAAA, 0xAAAAAAAA, 0xAAAAAAAA, 0xAAAAAAAA,
        0x55555555, 0x55555555, 0x55555555, 0x55555555,
        0xAAAAAAAA, 0xAAAAAAAA, 0xAAAAAAAA, 0xAAAAAAAA,
        0x55555555, 0x55555555, 0x55555555, 0x55555555
    };

    /* Plot sprite starting at col -16 (16 pixels off screen) */
    plot_bitmap_32(mock_screen, 0, start_col, bitmap, 32);
    assert_borders((UINT8 *)mock_screen, 0, start_col, 32, 32);
    verify_bitmap_32_pixels((UINT8 *)mock_screen, 0, start_col, bitmap, 32);
}

/* Test: Plot 32x32 sprite partially off right edge byte aligned */
void test_plot_bitmap_32_off_right_edge_byte_aligned(void)
{
    const INT16 start_col = SCREEN_WIDTH_PIXELS - 16; /* Half off screen to the right */

    const UINT32 bitmap[32] = {
        0xAAAAAAAA, 0xAAAAAAAA, 0xAAAAAAAA, 0xAAAAAAAA,
        0x55555555, 0x55555555, 0x55555555, 0x55555555,
        0xAAAAAAAA, 0xAAAAAAAA, 0xAAAAAAAA, 0xAAAAAAAA,
        0x55555555, 0x55555555, 0x55555555, 0x55555555,
        0xAAAAAAAA, 0xAAAAAAAA, 0xAAAAAAAA, 0xAAAAAAAA,
        0x55555555, 0x55555555, 0x55555555, 0x55555555,
        0xAAAAAAAA, 0xAAAAAAAA, 0xAAAAAAAA, 0xAAAAAAAA,
        0x55555555, 0x55555555, 0x55555555, 0x55555555
    };

    /* Plot sprite starting at col SCREEN_WIDTH_PIXELS - 16 (16 pixels off screen) */
    plot_bitmap_32(mock_screen, 0, start_col, bitmap, 32);
    assert_borders((UINT8 *)mock_screen, 0, start_col, 32, 32);
    verify_bitmap_32_pixels((UINT8 *)mock_screen, 0, start_col, bitmap, 32);
}

/* Test: Plot 32x32 sprite partially off right edge not byte aligned */
void test_plot_bitmap_32_off_right_edge_non_byte_aligned(void)
{
    const INT16 start_col = SCREEN_WIDTH_PIXELS - 7; /* Half off screen to the right */

    const UINT32 bitmap[32] = {
        0xAAAAAAAA, 0xAAAAAAAA, 0xAAAAAAAA, 0xAAAAAAAA,
        0x55555555, 0x55555555, 0x55555555, 0x55555555,
        0xAAAAAAAA, 0xAAAAAAAA, 0xAAAAAAAA, 0xAAAAAAAA,
        0x55555555, 0x55555555, 0x55555555, 0x55555555,
        0xAAAAAAAA, 0xAAAAAAAA, 0xAAAAAAAA, 0xAAAAAAAA,
        0x55555555, 0x55555555, 0x55555555, 0x55555555,
        0xAAAAAAAA, 0xAAAAAAAA, 0xAAAAAAAA, 0xAAAAAAAA,
        0x55555555, 0x55555555, 0x55555555, 0x55555555
    };

    /* Plot sprite starting at col SCREEN_WIDTH_PIXELS - 7 (7 pixels off screen) */
    plot_bitmap_32(mock_screen, 0, start_col, bitmap, 32);
    assert_borders((UINT8 *)mock_screen, 0, start_col, 32, 32);
    verify_bitmap_32_pixels((UINT8 *)mock_screen, 0, start_col, bitmap, 32);
}

/* Test: Plot 32x32 sprite partially off top edge */
void test_plot_bitmap_32_off_top_edge(void)
{
    const INT16 start_row = -16; /* Half off screen to the top */

    const UINT32 bitmap[32] = {
        0xAAAAAAAA, 0xAAAAAAAA, 0xAAAAAAAA, 0xAAAAAAAA,
        0x55555555, 0x55555555, 0x55555555, 0x55555555,
        0xAAAAAAAA, 0xAAAAAAAA, 0xAAAAAAAA, 0xAAAAAAAA,
        0x55555555, 0x55555555, 0x55555555, 0x55555555,
        0xAAAAAAAA, 0xAAAAAAAA, 0xAAAAAAAA, 0xAAAAAAAA,
        0x55555555, 0x55555555, 0x55555555, 0x55555555,
        0xAAAAAAAA, 0xAAAAAAAA, 0xAAAAAAAA, 0xAAAAAAAA,
        0x55555555, 0x55555555, 0x55555555, 0x55555555
    };;

    /* Plot sprite starting at row -16 (top 16 pixels clipped) */
    plot_bitmap_32(mock_screen, start_row, 0, bitmap, 32);
    assert_borders((UINT8 *)mock_screen, start_row, 0, 32, 32);
    verify_bitmap_32_pixels((UINT8 *)mock_screen, start_row, 0, bitmap, 32);
}

/* Test: Plot 32x32 sprite partially off bottom edge */
void test_plot_bitmap_32_off_bottom_edge(void)
{
    const INT16 start_row = SCREEN_HEIGHT_PIXELS - 16; /* Half off screen to the bottom */

    const UINT32 bitmap[32] = {
        0xAAAAAAAA, 0xAAAAAAAA, 0xAAAAAAAA, 0xAAAAAAAA,
        0x55555555, 0x55555555, 0x55555555, 0x55555555,
        0xAAAAAAAA, 0xAAAAAAAA, 0xAAAAAAAA, 0xAAAAAAAA,
        0x55555555, 0x55555555, 0x55555555, 0x55555555,
        0xAAAAAAAA, 0xAAAAAAAA, 0xAAAAAAAA, 0xAAAAAAAA,
        0x55555555, 0x55555555, 0x55555555, 0x55555555,
        0xAAAAAAAA, 0xAAAAAAAA, 0xAAAAAAAA, 0xAAAAAAAA,
        0x55555555, 0x55555555, 0x55555555, 0x55555555
    };

    /* Plot sprite starting at row SCREEN_HEIGHT_PIXELS - 16 (bottom 16 pixels clipped) */
    plot_bitmap_32(mock_screen, start_row, 0, bitmap, 32);
    assert_borders((UINT8 *)mock_screen, start_row, 0, 32, 32);
    verify_bitmap_32_pixels((UINT8 *)mock_screen, start_row, 0, bitmap, 32);
}

/* Test: Plot 32x32 sprite completely off screen (top and bottom) */
void test_plot_bitmap_32_completely_off_screen(void)
{
    const UINT32 bitmap[32] = {
        0xAAAAAAAA, 0xAAAAAAAA, 0xAAAAAAAA, 0xAAAAAAAA,
        0x55555555, 0x55555555, 0x55555555, 0x55555555,
        0xAAAAAAAA, 0xAAAAAAAA, 0xAAAAAAAA, 0xAAAAAAAA,
        0x55555555, 0x55555555, 0x55555555, 0x55555555,
        0xAAAAAAAA, 0xAAAAAAAA, 0xAAAAAAAA, 0xAAAAAAAA,
        0x55555555, 0x55555555, 0x55555555, 0x55555555,
        0xAAAAAAAA, 0xAAAAAAAA, 0xAAAAAAAA, 0xAAAAAAAA,
        0x55555555, 0x55555555, 0x55555555, 0x55555555
    };

    /* Plot way above the top edge */
    plot_bitmap_32(mock_screen, -40, 0, bitmap, 32);
    assert_borders((UINT8 *)mock_screen, -40, 0, 32, 32);
    
    /* Plot way below the bottom edge */
    plot_bitmap_32(mock_screen, SCREEN_HEIGHT_PIXELS + 10, 0, bitmap, 32);
    assert_borders((UINT8 *)mock_screen, SCREEN_HEIGHT_PIXELS + 10, 0, 32, 32);
}

void test_plot_bitmap_32_centered(void) {
    const UINT32 bitmap[32] = {
        0xAAAAAAAA, 0xAAAAAAAA, 0xAAAAAAAA, 0xAAAAAAAA,
        0x55555555, 0x55555555, 0x55555555, 0x55555555,
        0xAAAAAAAA, 0xAAAAAAAA, 0xAAAAAAAA, 0xAAAAAAAA,
        0x55555555, 0x55555555, 0x55555555, 0x55555555,
        0xAAAAAAAA, 0xAAAAAAAA, 0xAAAAAAAA, 0xAAAAAAAA,
        0x55555555, 0x55555555, 0x55555555, 0x55555555,
        0xAAAAAAAA, 0xAAAAAAAA, 0xAAAAAAAA, 0xAAAAAAAA,
        0x55555555, 0x55555555, 0x55555555, 0x55555555
    };
    int row = ((SCREEN_HEIGHT_PIXELS - 32) / 2) + 1;
    int col = ((SCREEN_WIDTH_PIXELS - 32) / 2) + 1;

    /* Plot sprite centered on the screen, but shouldn't be byte aligned */
    plot_bitmap_32(mock_screen, row, col, bitmap, 32);

    verify_bitmap_32_pixels((UINT8 *)mock_screen, row, col, bitmap, 32);
    assert_borders((UINT8 *)mock_screen, row, col, 32, 32);
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
    RUN_TEST(test_plot_bitmap_32_off_right_edge_byte_aligned);
    RUN_TEST(test_plot_bitmap_32_off_right_edge_non_byte_aligned);
    RUN_TEST(test_plot_bitmap_32_off_top_edge);
    RUN_TEST(test_plot_bitmap_32_off_bottom_edge);
    RUN_TEST(test_plot_bitmap_32_completely_off_screen);
    RUN_TEST(test_plot_bitmap_32_centered);

    return UNITY_END();
}
