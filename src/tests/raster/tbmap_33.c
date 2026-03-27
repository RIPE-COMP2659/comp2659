#include "unity.h"
#include "raster.h"
#include <string.h>

#define SCREEN_SIZE_BYTES 32000
#define SCREEN_WIDTH_BYTES 80
#define SCREEN_WIDTH_PIXELS 640
#define SCREEN_HEIGHT_PIXELS 400

#define BITMAP_33_WIDTH 33
#define BITMAP_33_HEIGHT 33
#define BITMAP_33_WORDS_PER_ROW 2
#define BITMAP_33_WORD_COUNT (BITMAP_33_HEIGHT * BITMAP_33_WORDS_PER_ROW)

static UINT8 mock_screen[SCREEN_SIZE_BYTES];

static int get_pixel(UINT8 *base, INT16 row, INT16 col)
{
    UINT8 *byte_ptr;
    int bit_pos;

    byte_ptr = base + (row * SCREEN_WIDTH_BYTES) + (col / 8);
    bit_pos = 7 - (col % 8);
    return (*byte_ptr >> bit_pos) & 1;
}

static void init_bitmap_33(UINT32 *bitmap)
{
    INT16 row;

    for (row = 0; row < BITMAP_33_HEIGHT; row++) {
        if ((row & 1) == 0) {
            bitmap[row * 2] = 0xAAAAAAAAu;
            bitmap[(row * 2) + 1] = 0x80000000u;
        } else {
            bitmap[row * 2] = 0x55555555u;
            bitmap[(row * 2) + 1] = 0x00000000u;
        }
    }
}

static int get_bitmap_33_pixel(const UINT32 *bitmap, INT16 row, INT16 col)
{
    UINT32 value;

    if (col < 32) {
        value = bitmap[row * 2];
        return (int)((value >> (31 - col)) & 1u);
    }

    value = bitmap[(row * 2) + 1];
    return (int)((value >> 31) & 1u);
}

static void verify_bitmap_33_pixels(
    UINT8 *base,
    INT16 start_row,
    INT16 start_col,
    const UINT32 *bitmap
)
{
    INT16 src_row;

    for (src_row = 0; src_row < BITMAP_33_HEIGHT; src_row++) {
        const INT16 dst_row = start_row + src_row;
        INT16 src_col;

        if (dst_row < 0 || dst_row >= SCREEN_HEIGHT_PIXELS) {
            continue;
        }

        for (src_col = 0; src_col < BITMAP_33_WIDTH; src_col++) {
            const INT16 dst_col = start_col + src_col;

            if (dst_col >= 0 && dst_col < SCREEN_WIDTH_PIXELS) {
                const int expected = get_bitmap_33_pixel(bitmap, src_row, src_col);
                TEST_ASSERT_EQUAL_INT_MESSAGE(
                    expected,
                    get_pixel(base, dst_row, dst_col),
                    "Bitmap pixel mismatch"
                );
            }
        }
    }
}

void setUp(void)
{
    memset(mock_screen, 0x00, SCREEN_SIZE_BYTES);
}

void tearDown(void)
{
}

void test_plot_bitmap_33_top_left(void)
{
    UINT32 bitmap[BITMAP_33_WORD_COUNT];

    init_bitmap_33(bitmap);
    plot_bitmap_33(0, 0, mock_screen, bitmap);

    verify_bitmap_33_pixels(mock_screen, 0, 0, bitmap);
}

void test_plot_bitmap_33_center_non_byte_aligned(void)
{
    UINT32 bitmap[BITMAP_33_WORD_COUNT];
    INT16 row;
    INT16 col;

    init_bitmap_33(bitmap);
    row = 101;
    col = 203;

    plot_bitmap_33(row, col, mock_screen, bitmap);
    verify_bitmap_33_pixels(mock_screen, row, col, bitmap);
}

void test_plot_bitmap_33_clipped_top_right(void)
{
    UINT32 bitmap[BITMAP_33_WORD_COUNT];
    INT16 row;
    INT16 col;

    init_bitmap_33(bitmap);
    row = -7;
    col = SCREEN_WIDTH_PIXELS - 20;

    plot_bitmap_33(row, col, mock_screen, bitmap);
    verify_bitmap_33_pixels(mock_screen, row, col, bitmap);
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_plot_bitmap_33_top_left);
    RUN_TEST(test_plot_bitmap_33_center_non_byte_aligned);
    RUN_TEST(test_plot_bitmap_33_clipped_top_right);

    return UNITY_END();
}
