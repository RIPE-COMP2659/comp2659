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
#define BITMAP_32_SIZE 32

/* Assume that the pixel is on the screen */
/* TODO: Might do a row lookup table, remove the computation completely */
/* Assume the area is already white */
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
    INT16 x_map_i;
    INT16 y_map_i;
    INT16 y_map_min = 0;
    INT16 y_map_max = BITMAP_32_SIZE - 1;
    INT16 x_map_min = 0;
    INT16 x_map_max = BITMAP_32_SIZE - 1;

    if (y < 0) { /* If y is negative, that's the offset index */
        y_map_min = -y;
    } else if (y + BITMAP_32_SIZE > SCREEN_HEIGHT_PIXELS) { /* y can't be off bottom if off top */
        y_map_max = SCREEN_HEIGHT_PIXELS - 1 - y;
    }

    if (x < 0) { /* If x is negative, that's the offset index */
        x_map_min = -x;
    } else if (x + BITMAP_32_SIZE > SCREEN_WIDTH_PIXELS) { /* x can't be off the right if off the left */
        x_map_max = SCREEN_WIDTH_PIXELS - 1 - x;
    }

    for (y_map_i = y_map_min; y_map_i <= y_map_max; y_map_i++) {
        const INT16 y_screen = y + y_map_i;
        const UINT32 row_bits = bitmap[y_map_i];

        for (x_map_i = x_map_min; x_map_i <= x_map_max; x_map_i++) {
            const UINT8 value = (UINT8)((row_bits >> (31 - x_map_i)) & 1u);

            if (value != 0u) {
                plotPixel(x + x_map_i, y_screen, base);
            }
        }
    }
}
