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

#define SCREEN_WIDTH_PIXELS 640
#define SCREEN_WIDTH_BYTES (SCREEN_WIDTH_PIXELS / 8)
#define SCREEN_HEIGHT_PIXELS 400
#define BITMAP_33_SIZE 32

/* Assume that the pixel is on the screen */
/* TODO: Might do a row lookup table, remove the computation completely */
static void plotPixel(INT16 x, INT16 y, UINT8* screen_ptr)
{
    /* offset = y * screen_width_bytes + x / 8 */
    UINT16 offset = (y * SCREEN_WIDTH_BYTES + (x >> 3));
    UINT8* byte_ptr = screen_ptr + offset;
    UINT8 byte_value = *byte_ptr;
    UINT8 pixel_in_byte = 1 << (7 - (x & 7));
    UINT8 new_value = byte_value | pixel_in_byte;
    *byte_ptr = new_value;
}

void plot_bitmap_33(INT16 x, INT16 y, UINT8* base, UINT32* bitmap)
{
    INT16 src_row;

    for (src_row = 0; src_row < BITMAP_33_SIZE; src_row++) {
        const INT16 dst_row = (INT16)(y + src_row);
        const UINT32 row_bits = bitmap[src_row];
        INT16 src_col;

        if (dst_row < 0 || dst_row >= SCREEN_HEIGHT_PIXELS) {
            continue;
        }

        for (src_col = 0; src_col < 32; src_col++) {
            const INT16 dst_col = (INT16)(x + src_col);
            const UINT8 value = (UINT8)((row_bits >> (31 - src_col)) & 1u);

            if (value != 0u && dst_col >= 0 && dst_col < SCREEN_WIDTH_PIXELS) {
                plotPixel(dst_col, dst_row, base);
            }
        }
    }
}
