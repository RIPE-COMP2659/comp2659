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

static void init_bitmap_33_1(UINT32 *bitmap)
{
    INT16 y;

    for (y = 0; y < BITMAP_33_SIZE; y++) {
        if ((y & 1) == 0) {
            bitmap[y * 2] = 0xAAAAAAAAu;
            bitmap[(y * 2) + 1] = 0x80000000u;
        } else {
            bitmap[y * 2] = 0x55555555u;
            bitmap[(y * 2) + 1] = 0x00000000u;
        }
    }
}

static void init_bitmap_33_2(UINT32 *bitmap)
{
    INT16 y;

    for (y = 0; y < BITMAP_33_SIZE; y++) {
        if ((y & 1) == 0) {
            bitmap[y * 2] = 0x55555555u;
            bitmap[(y * 2) + 1] = 0x00000000u;
        } else {
            bitmap[y * 2] = 0xAAAAAAAAu;
            bitmap[(y * 2) + 1] = 0x80000000u;
        }
    }
}

static int get_bitmap_33_pixel(const UINT32 *bitmap, INT16 y, INT16 x)
{
    UINT32 value;

    if (x < 32) {
        value = bitmap[y * 2];
        return (int)((value >> (31 - x)) & 1u);
    }

    value = bitmap[(y * 2) + 1];
    return (int)((value >> 31) & 1u);
}

static void verify_bitmap_33_pixels(
    UINT8 *base,
    INT16 start_y,
    INT16 start_x,
    const UINT32 *bitmap
)
{
    INT16 src_y;

    for (src_y = 0; src_y < BITMAP_33_SIZE; src_y++) {
        const INT16 dst_y = start_y + src_y;
        INT16 src_x;

        if (dst_y < 0 || dst_y >= SCREEN_HEIGHT_PIXELS) {
            continue;
        }

        for (src_x = 0; src_x < BITMAP_33_SIZE; src_x++) {
            const INT16 dst_x = start_x + src_x;

            if (dst_x >= 0 && dst_x < SCREEN_WIDTH_PIXELS) {
                const int expected = get_bitmap_33_pixel(bitmap, src_y, src_x);
                TEST_ASSERT_EQUAL_INT_MESSAGE(
                    expected,
                    get_pixel(dst_x, dst_y, base),
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

void test_plot_bitmap_33_center_1(void)
{
    UINT32 bitmap[BITMAP_33_WORD_COUNT];
    INT16 x;
    INT16 y;

    init_bitmap_33_1(bitmap);
    x = SCREEN_WIDTH_PIXELS / 2;
    y = SCREEN_HEIGHT_PIXELS / 2;

    plot_bitmap_33(x, y, mock_screen, bitmap);
    verify_bitmap_33_pixels(mock_screen, y, x, bitmap);
}

void test_plot_bitmap_33_center_2(void)
{
    UINT32 bitmap[BITMAP_33_WORD_COUNT];
    INT16 x;
    INT16 y;

    init_bitmap_33_2(bitmap);
    x = SCREEN_WIDTH_PIXELS / 2;
    y = SCREEN_HEIGHT_PIXELS / 2;

    plot_bitmap_33(x, y, mock_screen, bitmap);
    verify_bitmap_33_pixels(mock_screen, y, x, bitmap);
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_plot_bitmap_33_center_1);
    RUN_TEST(test_plot_bitmap_33_center_2);

    return UNITY_END();
}
