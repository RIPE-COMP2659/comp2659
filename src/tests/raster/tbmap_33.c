#include "unity.h"
#include "raster.h"
#include <string.h>

#define SCREEN_SIZE_BYTES 32000
#define SCREEN_WIDTH_PIXELS 640
#define SCREEN_WIDTH_BYTES (SCREEN_WIDTH_PIXELS / 8)
#define SCREEN_HEIGHT_PIXELS 400
#define BITMAP_32_SIZE 32
#define WHITE 0

#define BITMAP_33_WORD_COUNT BITMAP_32_SIZE

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

/* Read a single pixel from a bitmap */
static int get_bitmap_32_pixel(INT16 x, INT16 y, const UINT32 *bitmap)
{
    INT16 word_index = y;
    INT16 bit_shift = 31 - x;
    UINT32 value = bitmap[word_index];
    int result = (value >> bit_shift) & 1;
    return result;
}

/* Assert one-pixel border around a 32x32 bitmap draw region remains clear. */
static void assert_bitmap_32_borders(INT16 x, INT16 y, UINT8 *base)
{
    INT16 y_screen;
    INT16 x_screen;
    const INT16 left_x = x - 1;
    const INT16 right_x = x + BITMAP_32_SIZE;
    const INT16 top_y = y - 1;
    const INT16 bottom_y = y + BITMAP_32_SIZE;

    for (y_screen = y; y_screen < (INT16)(y + BITMAP_32_SIZE); y_screen++) {
        if (y_screen >= 0 && y_screen < SCREEN_HEIGHT_PIXELS) {
            if (left_x >= 0 && left_x < SCREEN_WIDTH_PIXELS) {
                TEST_ASSERT_EQUAL_INT_MESSAGE(WHITE, get_pixel(left_x, y_screen, base),
                    "Left boundary column cleared incorrectly");
            }

            if (right_x >= 0 && right_x < SCREEN_WIDTH_PIXELS) {
                TEST_ASSERT_EQUAL_INT_MESSAGE(WHITE, get_pixel(right_x, y_screen, base),
                    "Right boundary column cleared incorrectly");
            }
        }
    }

    for (x_screen = x; x_screen < (INT16)(x + BITMAP_32_SIZE); x_screen++) {
        if (x_screen >= 0 && x_screen < SCREEN_WIDTH_PIXELS) {
            if (top_y >= 0 && top_y < SCREEN_HEIGHT_PIXELS) {
                TEST_ASSERT_EQUAL_INT_MESSAGE(WHITE, get_pixel(x_screen, top_y, base),
                    "Top boundary row cleared incorrectly");
            }

            if (bottom_y >= 0 && bottom_y < SCREEN_HEIGHT_PIXELS) {
                TEST_ASSERT_EQUAL_INT_MESSAGE(WHITE, get_pixel(x_screen, bottom_y, base),
                    "Bottom boundary row cleared incorrectly");
            }
        }
    }
}

/* Verify bitmap pixels exactly match expected data, with screen bounds clipping. */
static void verify_bitmap_32_pixels(
    INT16 x,
    INT16 y,
    UINT8 *base,
    const UINT32 *bitmap
) {
    INT16 x_bitmap;
    INT16 y_bitmap;

    for (y_bitmap = 0; y_bitmap < BITMAP_32_SIZE; y_bitmap++) {
        const INT16 y_screen = y + y_bitmap;

        if (y_screen >= 0 && y_screen < SCREEN_HEIGHT_PIXELS) {
            for (x_bitmap = 0; x_bitmap < BITMAP_32_SIZE; x_bitmap++) {
                const INT16 x_screen = x + x_bitmap;

                if (x_screen >= 0 && x_screen < SCREEN_WIDTH_PIXELS) {
                    TEST_ASSERT_EQUAL_INT_MESSAGE(
                        get_bitmap_32_pixel(x_bitmap, y_bitmap, bitmap),
                        get_pixel(x_screen, y_screen, base),
                        "Bitmap pixel mismatch");
                }
            }
        }
    }
}

static void assert_and_verify_bitmap_32(
    INT16 x,
    INT16 y,
    UINT8 *base,
    const UINT32 *bitmap
)
{
    assert_bitmap_32_borders(x, y, base);
    verify_bitmap_32_pixels(x, y, base, bitmap);
}

static void init_bitmap_32_v1(UINT32 *bitmap)
{
    INT16 y;

    for (y = 0; y < BITMAP_32_SIZE; y++) {
        bitmap[y] = ((y & 1) == 0) ? 0xAAAAAAAAu : 0x55555555u;
    }
}

static void init_bitmap_32_v2(UINT32 *bitmap)
{
    INT16 y;

    for (y = 0; y < BITMAP_32_SIZE; y++) {
        bitmap[y] = ((y & 1) == 0) ? 0x55555555u : 0xAAAAAAAAu;
    }
}

void setUp(void)
{
    memset(mock_screen, 0x00, SCREEN_SIZE_BYTES);
}

void tearDown(void)
{
}

void test_plot_bitmap_33_center_v1(void)
{
    UINT32 bitmap[BITMAP_33_WORD_COUNT];
    INT16 x = SCREEN_WIDTH_PIXELS >> 2;
    INT16 y = SCREEN_HEIGHT_PIXELS >> 2;

    init_bitmap_32_v1(bitmap);
    plot_bitmap_33(x, y, mock_screen, bitmap);

    assert_and_verify_bitmap_32(x, y, mock_screen, bitmap);
}

void test_plot_bitmap_33_center_v2(void)
{
    UINT32 bitmap[BITMAP_33_WORD_COUNT];
    INT16 x = SCREEN_WIDTH_PIXELS >> 2;
    INT16 y = SCREEN_HEIGHT_PIXELS >> 2;

    init_bitmap_32_v2(bitmap);
    plot_bitmap_33(x, y, mock_screen, bitmap);

    assert_and_verify_bitmap_32(x, y, mock_screen, bitmap);
}

void test_plot_bitmap_33_off_left_non_byte_v1(void)
{
    UINT32 bitmap[BITMAP_33_WORD_COUNT];
    INT16 x = -33;
    INT16 y = SCREEN_HEIGHT_PIXELS >> 2;

    init_bitmap_32_v1(bitmap);
    plot_bitmap_33(x, y, mock_screen, bitmap);

    assert_and_verify_bitmap_32(x, y, mock_screen, bitmap);
}

void test_plot_bitmap_33_off_left_non_byte_v2(void)
{
    UINT32 bitmap[BITMAP_33_WORD_COUNT];
    INT16 x = -33;
    INT16 y = SCREEN_HEIGHT_PIXELS >> 2;

    init_bitmap_32_v2(bitmap);
    plot_bitmap_33(x, y, mock_screen, bitmap);

    assert_and_verify_bitmap_32(x, y, mock_screen, bitmap);
}

void test_plot_bitmap_33_off_left_byte_v1(void)
{
    UINT32 bitmap[BITMAP_33_WORD_COUNT];
    INT16 x = -32;
    INT16 y = SCREEN_HEIGHT_PIXELS >> 2;

    init_bitmap_32_v1(bitmap);
    plot_bitmap_33(x, y, mock_screen, bitmap);

    assert_and_verify_bitmap_32(x, y, mock_screen, bitmap);
}

void test_plot_bitmap_33_off_left_byte_v2(void)
{
    UINT32 bitmap[BITMAP_33_WORD_COUNT];
    INT16 x = -32;
    INT16 y = SCREEN_HEIGHT_PIXELS >> 2;

    init_bitmap_32_v2(bitmap);
    plot_bitmap_33(x, y, mock_screen, bitmap);

    assert_and_verify_bitmap_32(x, y, mock_screen, bitmap);
}

void test_plot_bitmap_33_partial_left_non_byte_v1(void)
{
    UINT32 bitmap[BITMAP_33_WORD_COUNT];
    INT16 x = -7;
    INT16 y = SCREEN_HEIGHT_PIXELS >> 2;

    init_bitmap_32_v1(bitmap);
    plot_bitmap_33(x, y, mock_screen, bitmap);

    assert_and_verify_bitmap_32(x, y, mock_screen, bitmap);
}

void test_plot_bitmap_33_partial_left_non_byte_v2(void)
{
    UINT32 bitmap[BITMAP_33_WORD_COUNT];
    INT16 x = -7;
    INT16 y = SCREEN_HEIGHT_PIXELS >> 2;

    init_bitmap_32_v2(bitmap);
    plot_bitmap_33(x, y, mock_screen, bitmap);

    assert_and_verify_bitmap_32(x, y, mock_screen, bitmap);
}

void test_plot_bitmap_33_partial_left_byte_v1(void)
{
    UINT32 bitmap[BITMAP_33_WORD_COUNT];
    INT16 x = -8;
    INT16 y = SCREEN_HEIGHT_PIXELS >> 2;

    init_bitmap_32_v1(bitmap);
    plot_bitmap_33(x, y, mock_screen, bitmap);

    assert_and_verify_bitmap_32(x, y, mock_screen, bitmap);
}

void test_plot_bitmap_33_partial_left_byte_v2(void)
{
    UINT32 bitmap[BITMAP_33_WORD_COUNT];
    INT16 x = -8;
    INT16 y = SCREEN_HEIGHT_PIXELS >> 2;

    init_bitmap_32_v2(bitmap);
    plot_bitmap_33(x, y, mock_screen, bitmap);

    assert_and_verify_bitmap_32(x, y, mock_screen, bitmap);
}

void test_plot_bitmap_33_on_left_non_byte_v1(void)
{
    UINT32 bitmap[BITMAP_33_WORD_COUNT];
    INT16 x = 1;
    INT16 y = SCREEN_HEIGHT_PIXELS >> 2;

    init_bitmap_32_v1(bitmap);
    plot_bitmap_33(x, y, mock_screen, bitmap);

    assert_and_verify_bitmap_32(x, y, mock_screen, bitmap);
}

void test_plot_bitmap_33_on_left_non_byte_v2(void)
{
    UINT32 bitmap[BITMAP_33_WORD_COUNT];
    INT16 x = 1;
    INT16 y = SCREEN_HEIGHT_PIXELS >> 2;

    init_bitmap_32_v2(bitmap);
    plot_bitmap_33(x, y, mock_screen, bitmap);

    assert_and_verify_bitmap_32(x, y, mock_screen, bitmap);
}

void test_plot_bitmap_33_on_left_byte_v1(void)
{
    UINT32 bitmap[BITMAP_33_WORD_COUNT];
    INT16 x = 0;
    INT16 y = SCREEN_HEIGHT_PIXELS >> 2;

    init_bitmap_32_v1(bitmap);
    plot_bitmap_33(x, y, mock_screen, bitmap);

    assert_and_verify_bitmap_32(x, y, mock_screen, bitmap);
}

void test_plot_bitmap_33_on_left_byte_v2(void)
{
    UINT32 bitmap[BITMAP_33_WORD_COUNT];
    INT16 x = 0;
    INT16 y = SCREEN_HEIGHT_PIXELS >> 2;

    init_bitmap_32_v2(bitmap);
    plot_bitmap_33(x, y, mock_screen, bitmap);

    assert_and_verify_bitmap_32(x, y, mock_screen, bitmap);
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_plot_bitmap_33_center_v1);
    RUN_TEST(test_plot_bitmap_33_center_v2);
    RUN_TEST(test_plot_bitmap_33_off_left_non_byte_v1);
    RUN_TEST(test_plot_bitmap_33_off_left_non_byte_v2);
    RUN_TEST(test_plot_bitmap_33_off_left_byte_v1);
    RUN_TEST(test_plot_bitmap_33_off_left_byte_v2);
    RUN_TEST(test_plot_bitmap_33_partial_left_non_byte_v1);
    RUN_TEST(test_plot_bitmap_33_partial_left_non_byte_v2);
    RUN_TEST(test_plot_bitmap_33_partial_left_byte_v1);
    RUN_TEST(test_plot_bitmap_33_partial_left_byte_v2);
    RUN_TEST(test_plot_bitmap_33_on_left_non_byte_v1);
    RUN_TEST(test_plot_bitmap_33_on_left_non_byte_v2);
    RUN_TEST(test_plot_bitmap_33_on_left_byte_v1);
    RUN_TEST(test_plot_bitmap_33_on_left_byte_v2);

    return UNITY_END();
}
