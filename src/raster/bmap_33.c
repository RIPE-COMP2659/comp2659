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

void plot_bitmap_33(INT16 x, INT16 y, UINT8* base, UINT32* bitmap)
{
    INT16 y_map_i;
    INT16 y_map_min = 0;
    INT16 y_map_max = BITMAP_32_SIZE - 1;
    INT16 x_map_min = 0;
    INT16 x_map_max = BITMAP_32_SIZE - 1;
    UINT8* dst;
    INT16 x_screen;
    INT16 x_byte;
    INT16 bit_offset;
    INT16 num_bytes;
    UINT8 left_mask;
    UINT8 right_mask;

    /* Only relevant if the bitmap is partially off-screen */
    if (y < 0) {
        y_map_min = -y;
    } else if (y + BITMAP_32_SIZE > SCREEN_HEIGHT_PIXELS) {
        y_map_max = SCREEN_HEIGHT_PIXELS - 1 - y;
    }
    if (x < 0) {
        x_map_min = -x;
    } else if (x + BITMAP_32_SIZE > SCREEN_WIDTH_PIXELS) {
        x_map_max = SCREEN_WIDTH_PIXELS - 1 - x;
    }

    /* Almost all only relevant if partially off screen, otherwise much simpler */
    x_screen  = x + x_map_min; /* The first x value of the bitmap */
    /* Throwaway value above*/
    x_byte    = x_screen >> 3; /* X / 8 */
    /* Throwaway value above once after variables */
    bit_offset = x_screen & 7;
    /* Important above */
    num_bytes  = ((x + x_map_max) >> 3) - x_byte + 1;
    /* Important above */

    left_mask  = 0xFF >> bit_offset;
    right_mask = 0xFF << (8 - bit_offset);

    dst = base + (y + y_map_min) * SCREEN_WIDTH_BYTES + x_byte;

    for (y_map_i = y_map_min; y_map_i <= y_map_max; y_map_i++) {
        const UINT32 row_bits = bitmap[y_map_i] << x_map_min;
        const UINT8 b0 = (UINT8)(row_bits >> 24);
        const UINT8 b1 = (UINT8)(row_bits >> 16);
        const UINT8 b2 = (UINT8)(row_bits >> 8);
        const UINT8 b3 = (UINT8)(row_bits);

        if (bit_offset == 0) {
            switch (num_bytes) {
                case 4: dst[3] = b3;
                case 3: dst[2] = b2;
                case 2: dst[1] = b1;
                case 1: dst[0] = b0;
            }
        } else {
            switch (num_bytes) {
                case 5: dst[4] = (dst[4] & ~right_mask) | (b3 << (8-bit_offset));
                case 4: dst[3] = (b2 << (8-bit_offset)) | (b3 >> bit_offset);
                case 3: dst[2] = (b1 << (8-bit_offset)) | (b2 >> bit_offset);
                case 2: dst[1] = (b0 << (8-bit_offset)) | (b1 >> bit_offset);
                case 1: dst[0] = (dst[0] & ~left_mask)  | (b0 >> bit_offset);
            }
        }

        dst += SCREEN_WIDTH_BYTES;
    }
}
