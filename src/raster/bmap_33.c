/**
 * bmap_33.c
 * Authors:
 *     Riley Gramlich, rgram060@mtroyal.ca, 201762060
 *     Robert Parker Hutcheson, rhutc335@mtroyal.ca, 201762335
 *     Isaac Klein, iklei977@mtroyal.ca, 201763977
 *     Eduard Mykhailets, emykh268@mtroyal.ca, 201750268
 * Course: COMP 2659-001, Computing Machinery II, Winter 2026
 * Instructor: Nolan Shaw
 *
 * Plots a fixed 32x32 bitmap to the screen given by the top left pixel of the
 * bitmap. It's an interation on bmap_32
 */

#include "raster.h"
#include <osbind.h>

#define SCREEN_WIDTH_BYTES 80
#define SCREEN_WIDTH_PIXELS 640
#define SCREEN_HEIGHT_PIXELS 400
#define BITMAP_33_SIZE 33

static void write_pixel(UINT8 *base, INT16 row, INT16 col, UINT8 value)
{
    UINT8 *byte_ptr;
    UINT8 mask;

    byte_ptr = base + (row * SCREEN_WIDTH_BYTES) + (col >> 3);
    mask = (UINT8)(1u << (7 - (col & 7)));

    if (value != 0u) {
        *byte_ptr |= mask;
    } else {
        *byte_ptr &= (UINT8)(~mask);
    }
}

void plot_bitmap_33(INT16 x, INT16 y, UINT8 *base, UINT32 *bitmap)
{
    INT16 src_row;

    Cconws("Called bmap_33...\r\n");

    for (src_row = 0; src_row < BITMAP_33_SIZE; src_row++) {
        const INT16 dst_row = (INT16)(x + src_row);
        const UINT32 left_32 = bitmap[src_row * 2];
        const UINT32 right_1 = bitmap[(src_row * 2) + 1];
        INT16 src_col;

        if (dst_row < 0 || dst_row >= SCREEN_HEIGHT_PIXELS) {
            continue;
        }

        for (src_col = 0; src_col < 32; src_col++) {
            const INT16 dst_col = (INT16)(y + src_col);
            const UINT8 value = (UINT8)((left_32 >> (31 - src_col)) & 1u);

            if (dst_col >= 0 && dst_col < SCREEN_WIDTH_PIXELS) {
                write_pixel(base, dst_row, dst_col, value);
            }
        }

        {
            const INT16 dst_col = (INT16)(y + 32);
            const UINT8 value = (UINT8)((right_1 >> 31) & 1u);

            if (dst_col >= 0 && dst_col < SCREEN_WIDTH_PIXELS) {
                write_pixel(base, dst_row, dst_col, value);
            }
        }
    }
}
