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
    INT16 x_bitmap;
    INT16 y_bitmap;

    for (y_bitmap = 0; y_bitmap < BITMAP_32_SIZE; y_bitmap++) {
        const INT16 y_screen = y + y_bitmap;

        if (y_screen >= 0 && y_screen < SCREEN_HEIGHT_PIXELS) {
            const UINT32 row_bits = bitmap[y_bitmap];

            for (x_bitmap = 0; x_bitmap < BITMAP_32_SIZE; x_bitmap++) {
                const INT16 x_screen = x + x_bitmap;

                if (x_screen >= 0 && x_screen < SCREEN_WIDTH_PIXELS) {
                    const UINT8 value = (UINT8)((row_bits >> (31 - x_bitmap)) & 1u);

                    /* Assume the area is already white */
                    if (value != 0u) {
                        plotPixel(x_screen, y_screen, base);
                    }
                }
            }
        }
    }
}
