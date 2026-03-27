#include "unity.h"
#include "raster.h"
#include <string.h>

#define SCREEN_SIZE_BYTES 32000
#define SCREEN_WIDTH_PIXELS 640
#define SCREEN_WIDTH_BYTES (SCREEN_WIDTH_PIXELS / 8)
#define SCREEN_HEIGHT_PIXELS 400
#define BITMAP_33_SIZE 32

#define BITMAP_33_WORD_COUNT (BITMAP_33_SIZE * 2)

static UINT8 mock_screen[SCREEN_SIZE_BYTES];

static int get_pixel(INT16 x, INT16 y, UINT8* screen_ptr)
{
    UINT16 offset = (y * SCREEN_WIDTH_BYTES + (x >> 3));
    UINT8* byte_ptr = screen_ptr + offset;
    UINT8 pxl_shift = 7 - (x & 7);
    /* & 1 is required here to remove the upper bits */
    UINT8 masked_value = (*byte_ptr >> pxl_shift) & 1;
    return masked_value;
}

static void init_bitmap_33(UINT32 *bitmap)
{
    INT16 row;

    for (row = 0; row < BITMAP_33_SIZE; row++) {
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

    for (src_row = 0; src_row < BITMAP_33_SIZE; src_row++) {
        const INT16 dst_row = start_row + src_row;
        INT16 src_col;

        if (dst_row < 0 || dst_row >= SCREEN_HEIGHT_PIXELS) {
            continue;
        }

        for (src_col = 0; src_col < BITMAP_33_SIZE; src_col++) {
            const INT16 dst_col = start_col + src_col;

            if (dst_col >= 0 && dst_col < SCREEN_WIDTH_PIXELS) {
                const int expected = get_bitmap_33_pixel(bitmap, src_row, src_col);
                TEST_ASSERT_EQUAL_INT_MESSAGE(
                    expected,
                    get_pixel(dst_col, dst_row, base),
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
    plot_bitmap_33(0, 0, mock_screen, bitmap); /* x=col=0, y=row=0 */

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

    plot_bitmap_33(col, row, mock_screen, bitmap); /* x=col, y=row */
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

    plot_bitmap_33(col, row, mock_screen, bitmap); /* x=col, y=row */
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
