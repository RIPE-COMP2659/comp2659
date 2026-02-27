#include "raster.h"
#include <stdio.h>
#include <osbind.h>

void disable_cursor();
void fill_screen(UINT32 *base, char pattern);

void test_clear_screen(UINT8 *base)
{
    clear_screen((UINT32 *)base);
}

void test_clear_region(UINT8 *base)
{
    int i;

    /* Fill screen with white first */
    fill_screen((UINT32 *)base, -1);

    /* Test 1: 48x48 optimized path - word aligned */
    clear_region((UINT32 *)base, 0, 0, 48, 48);

    /* Test 2: 48x48 optimized path - different position */
    clear_region((UINT32 *)base, 50, 80, 48, 48);

    /* Test 3: Small region - unoptimized path */
    clear_region((UINT32 *)base, 10, 5, 10, 10);

    /* Test 4: Wide region - unoptimized path */
    clear_region((UINT32 *)base, 100, 50, 20, 100);

    /* Test 5: Tall narrow region */
    clear_region((UINT32 *)base, 150, 200, 50, 8);

    /* Test 6: Odd column alignment to test byte spanning */
    clear_region((UINT32 *)base, 200, 5, 15, 4);
}

void test_plot_pixel(UINT8 *base)
{
    int i;

    /* Clear screen first */
    clear_screen((UINT32 *)base);

    /* Test 1: Draw a horizontal line using plot_pixel */
    for (i = 0; i < 100; i++)
    {
        plot_pixel(base, 10, i);
    }

    /* Test 2: Draw a vertical line */
    for (i = 20; i < 120; i++)
    {
        plot_pixel(base, i, 50);
    }

    /* Test 3: Draw a diagonal line */
    for (i = 0; i < 50; i++)
    {
        plot_pixel(base, 30 + i, 100 + i);
    }

    /* Test 4: Draw a box outline using plot_pixel */
    for (i = 200; i <= 280; i++)
    {
        plot_pixel(base, 200, i); /* top */
        plot_pixel(base, 250, i); /* bottom */
    }
    for (i = 200; i <= 250; i++)
    {
        plot_pixel(base, i, 200); /* left */
        plot_pixel(base, i, 280); /* right */
    }

    /* Test 5: Test pixels at various byte boundaries */
    for (i = 0; i < 64; i += 8)
    {
        plot_pixel(base, 300, i);     /* bit 7 (MSB) */
        plot_pixel(base, 301, i + 4); /* bit 3 (middle) */
        plot_pixel(base, 302, i + 7); /* bit 0 (LSB) */
    }
}

void test_plot_horizontal_line(UINT8 *base)
{
    /* Clear screen first */
    clear_screen((UINT32 *)base);

    /* Test 1: Simple horizontal line */
    plot_horizontal_line((UINT32 *)base, 10, 10, 50, 1);

    /* Test 2: Line starting at byte boundary */
    plot_horizontal_line((UINT32 *)base, 20, 0, 100, 1);

    /* Test 3: Line starting at various bit positions */
    plot_horizontal_line((UINT32 *)base, 30, 1, 40, 1);
    plot_horizontal_line((UINT32 *)base, 31, 2, 40, 1);
    plot_horizontal_line((UINT32 *)base, 32, 3, 40, 1);
    plot_horizontal_line((UINT32 *)base, 33, 4, 40, 1);
    plot_horizontal_line((UINT32 *)base, 34, 5, 40, 1);
    plot_horizontal_line((UINT32 *)base, 35, 6, 40, 1);
    plot_horizontal_line((UINT32 *)base, 36, 7, 40, 1);

    /* Test 4: Long line spanning many bytes */
    plot_horizontal_line((UINT32 *)base, 50, 5, 300, 1);

    /* Test 5: Short lines of various lengths */
    plot_horizontal_line((UINT32 *)base, 70, 10, 1, 1);
    plot_horizontal_line((UINT32 *)base, 72, 10, 3, 1);
    plot_horizontal_line((UINT32 *)base, 74, 10, 5, 1);
    plot_horizontal_line((UINT32 *)base, 76, 10, 8, 1);
    plot_horizontal_line((UINT32 *)base, 78, 10, 10, 1);

    /* Test 6: Lines that cross multiple byte boundaries */
    plot_horizontal_line((UINT32 *)base, 100, 7, 16, 1);  /* crosses 2-3 bytes */
    plot_horizontal_line((UINT32 *)base, 102, 15, 16, 1); /* crosses 2-3 bytes */

    /* Test 7: Full width line */
    plot_horizontal_line((UINT32 *)base, 120, 0, 640, 1);

    /* Test 8: Zero length line (should do nothing) */
    plot_horizontal_line((UINT32 *)base, 130, 100, 0, 1);

    /* Test 9: Create a ladder pattern */
    plot_horizontal_line((UINT32 *)base, 150, 50, 100, 1);
    plot_horizontal_line((UINT32 *)base, 155, 60, 100, 1);
    plot_horizontal_line((UINT32 *)base, 160, 70, 100, 1);
    plot_horizontal_line((UINT32 *)base, 165, 80, 100, 1);
    plot_horizontal_line((UINT32 *)base, 170, 90, 100, 1);
}

void test_plot_vertical_line(UINT8 *base)
{
    /* Clear screen first */
    clear_screen((UINT32 *)base);

    /* Test 1: Simple vertical line */
    plot_vertical_line((UINT32 *)base, 10, 10, 50, 1);

    /* Test 2: Line starting at top of screen */
    plot_vertical_line((UINT32 *)base, 0, 20, 100, 1);

    /* Test 3: Lines at different column positions (different bit positions) */
    plot_vertical_line((UINT32 *)base, 10, 1, 40, 1);
    plot_vertical_line((UINT32 *)base, 10, 9, 40, 1);
    plot_vertical_line((UINT32 *)base, 10, 17, 40, 1);
    plot_vertical_line((UINT32 *)base, 10, 25, 40, 1);
    plot_vertical_line((UINT32 *)base, 10, 33, 40, 1);
    plot_vertical_line((UINT32 *)base, 10, 41, 40, 1);
    plot_vertical_line((UINT32 *)base, 10, 49, 40, 1);

    /* Test 4: Tall line spanning many rows */
    plot_vertical_line((UINT32 *)base, 5, 80, 300, 1);

    /* Test 5: Short lines of various lengths */
    plot_vertical_line((UINT32 *)base, 10, 100, 1, 1);
    plot_vertical_line((UINT32 *)base, 20, 100, 3, 1);
    plot_vertical_line((UINT32 *)base, 30, 100, 5, 1);
    plot_vertical_line((UINT32 *)base, 40, 100, 8, 1);
    plot_vertical_line((UINT32 *)base, 50, 100, 10, 1);

    /* Test 6: Lines at byte boundaries (columns 0, 8, 16, etc.) */
    plot_vertical_line((UINT32 *)base, 100, 0, 30, 1);
    plot_vertical_line((UINT32 *)base, 100, 8, 30, 1);
    plot_vertical_line((UINT32 *)base, 100, 16, 30, 1);
    plot_vertical_line((UINT32 *)base, 100, 24, 30, 1);

    /* Test 7: Full height line */
    plot_vertical_line((UINT32 *)base, 0, 150, 400, 1);

    /* Test 8: Zero length line (should do nothing) */
    plot_vertical_line((UINT32 *)base, 100, 200, 0, 1);

    /* Test 9: Create a grid pattern combining with horizontal lines */
    plot_horizontal_line((UINT32 *)base, 150, 200, 100, 1);
    plot_horizontal_line((UINT32 *)base, 180, 200, 100, 1);
    plot_horizontal_line((UINT32 *)base, 210, 200, 100, 1);
    plot_vertical_line((UINT32 *)base, 150, 200, 60, 1);
    plot_vertical_line((UINT32 *)base, 150, 230, 60, 1);
    plot_vertical_line((UINT32 *)base, 150, 260, 60, 1);
    plot_vertical_line((UINT32 *)base, 150, 290, 60, 1);
}

void test_plot_line(UINT8 *base)
{
    /* Clear screen first */
    clear_screen((UINT32 *)base);

    /* Test 1: Horizontal line (should work like plot_horizontal_line) */
    plot_line((UINT32 *)base, 10, 10, 10, 100, 1);

    /* Test 2: Vertical line (should work like plot_vertical_line) */
    plot_line((UINT32 *)base, 20, 20, 80, 20, 1);

    /* Test 3: Diagonal line - 45 degrees down-right */
    plot_line((UINT32 *)base, 10, 120, 60, 170, 1);

    /* Test 4: Diagonal line - 45 degrees up-right */
    plot_line((UINT32 *)base, 60, 180, 10, 230, 1);

    /* Test 5: Steep line - more vertical than horizontal */
    plot_line((UINT32 *)base, 100, 50, 180, 70, 1);

    /* Test 6: Shallow line - more horizontal than vertical */
    plot_line((UINT32 *)base, 100, 100, 120, 200, 1);

    /* Test 7: Line going left (negative x direction) */
    plot_line((UINT32 *)base, 150, 250, 150, 150, 1);

    /* Test 8: Line going up (negative y direction) */
    plot_line((UINT32 *)base, 200, 50, 150, 50, 1);

    /* Test 9: Diagonal line - down-left */
    plot_line((UINT32 *)base, 150, 300, 200, 250, 1);

    /* Test 10: Diagonal line - up-left */
    plot_line((UINT32 *)base, 200, 350, 150, 300, 1);

    /* Test 11: Draw a triangle */
    plot_line((UINT32 *)base, 250, 400, 300, 350, 1);
    plot_line((UINT32 *)base, 300, 350, 300, 450, 1);
    plot_line((UINT32 *)base, 300, 450, 250, 400, 1);

    /* Test 12: Draw a star pattern */
    plot_line((UINT32 *)base, 320, 500, 360, 500, 1); /* vertical center */
    plot_line((UINT32 *)base, 340, 480, 340, 520, 1); /* horizontal center */
    plot_line((UINT32 *)base, 325, 485, 355, 515, 1); /* diagonal \ */
    plot_line((UINT32 *)base, 325, 515, 355, 485, 1); /* diagonal / */

    /* Test 13: Single pixel line (start == end) */
    plot_line((UINT32 *)base, 380, 100, 380, 100, 1);

    /* Test 14: Very long diagonal line */
    plot_line((UINT32 *)base, 10, 550, 380, 630, 1);
}

void test_plot_rectangle(UINT8 *base)
{
    /* Clear screen first */
    clear_screen((UINT32 *)base);

    /* Test 1: 48x48 optimized path - word aligned */
    plot_rectangle((UINT32 *)base, 0, 0, 48, 48);

    /* Test 2: 48x48 optimized path - different position */
    plot_rectangle((UINT32 *)base, 50, 80, 48, 48);

    plot_rectangle((UINT32 *)base, 300, 0, 2, 639);

    /* Test 3: Small rectangle - unoptimized path */
    plot_rectangle((UINT32 *)base, 10, 5, 10, 10);

    /* Test 4: Wide rectangle - unoptimized path */
    plot_rectangle((UINT32 *)base, 100, 50, 20, 100);

    /* Test 5: Tall narrow rectangle */
    plot_rectangle((UINT32 *)base, 150, 200, 50, 8);

    /* Test 6: Odd column alignment to test byte spanning */
    plot_rectangle((UINT32 *)base, 200, 5, 15, 4);

    /* Test 7: Single pixel rectangle */
    plot_rectangle((UINT32 *)base, 250, 10, 1, 1);

    /* Test 8: Various sized rectangles */
    plot_rectangle((UINT32 *)base, 10, 300, 30, 20);
    plot_rectangle((UINT32 *)base, 50, 350, 25, 35);
    plot_rectangle((UINT32 *)base, 100, 400, 40, 15);

    /* Test 9: Large rectangle */
    plot_rectangle((UINT32 *)base, 200, 300, 100, 150);
}

void test_plot_square(UINT8 *base)
{
    /* Clear screen first */
    clear_screen((UINT32 *)base);

    /* Test 1: 48x48 optimized path - word aligned */
    plot_square((UINT32 *)base, 0, 0, 48);

    /* Test 2: 48x48 optimized path - different position */
    plot_square((UINT32 *)base, 50, 80, 48);

    /* Test 3: Small square - unoptimized path */
    plot_square((UINT32 *)base, 10, 5, 10);

    /* Test 4: Medium square */
    plot_square((UINT32 *)base, 100, 50, 25);

    /* Test 5: Tall/narrow square */
    plot_square((UINT32 *)base, 150, 200, 8);

    /* Test 6: Odd column alignment */
    plot_square((UINT32 *)base, 200, 5, 15);

    /* Test 7: Single pixel square */
    plot_square((UINT32 *)base, 250, 10, 1);

    /* Test 8: Various sized squares */
    plot_square((UINT32 *)base, 10, 300, 20);
    plot_square((UINT32 *)base, 50, 350, 30);
    plot_square((UINT32 *)base, 100, 400, 40);

    /* Test 9: Large square */
    plot_square((UINT32 *)base, 200, 300, 100);
}

void test_plot_triangle(UINT8 *base)
{
    /* Clear screen first */
    clear_screen((UINT32 *)base);

    /* Test 1: Direction 0 - top-left 90° angle */
    plot_triangle((UINT32 *)base, 10, 10, 50, 50, 0);

    /* Test 2: Direction 1 - top-right 90° angle */
    plot_triangle((UINT32 *)base, 10, 150, 50, 50, 1);

    /* Test 3: Direction 2 - bottom-left 90° angle */
    plot_triangle((UINT32 *)base, 100, 10, 50, 50, 2);

    /* Test 4: Direction 3 - bottom-right 90° angle */
    plot_triangle((UINT32 *)base, 100, 150, 50, 50, 3);

    /* Test 5: Various sizes - direction 0 */
    plot_triangle((UINT32 *)base, 150, 10, 30, 40, 0);
    plot_triangle((UINT32 *)base, 150, 60, 20, 30, 0);
    plot_triangle((UINT32 *)base, 150, 100, 40, 20, 0);

    /* Test 6: Various sizes - direction 1 */
    plot_triangle((UINT32 *)base, 200, 100, 30, 40, 1);
    plot_triangle((UINT32 *)base, 200, 150, 20, 30, 1);

    /* Test 7: Various sizes - direction 2 */
    plot_triangle((UINT32 *)base, 300, 10, 30, 40, 2);
    plot_triangle((UINT32 *)base, 300, 60, 20, 30, 2);

    /* Test 8: Various sizes - direction 3 */
    plot_triangle((UINT32 *)base, 350, 100, 30, 40, 3);
    plot_triangle((UINT32 *)base, 350, 150, 20, 30, 3);

    /* Test 9: Large triangles in all directions */
    plot_triangle((UINT32 *)base, 10, 300, 80, 100, 0);
    plot_triangle((UINT32 *)base, 10, 500, 80, 100, 1);
    plot_triangle((UINT32 *)base, 200, 300, 80, 100, 2);
    plot_triangle((UINT32 *)base, 200, 500, 80, 100, 3);
}

void disable_cursor()
{
    printf("\033f");
    fflush(stdout);
}

void test_plot_bitmap_8(UINT8 *base)
{
    /* Test bitmaps - 8 pixels wide, 1 byte per row */

    /* Small checkerboard pattern (8x8) */
    const UINT8 checkerboard[] = {
        0xAA, /* 10101010 */
        0x55, /* 01010101 */
        0xAA, /* 10101010 */
        0x55, /* 01010101 */
        0xAA, /* 10101010 */
        0x55, /* 01010101 */
        0xAA, /* 10101010 */
        0x55  /* 01010101 */
    };

    /* Solid block (8x8) */
    const UINT8 solid_block[] = {
        0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF};

    /* Arrow pointing down (8x16) */
    const UINT8 arrow_down[] = {
        0x18, /* 00011000 */
        0x18, /* 00011000 */
        0x18, /* 00011000 */
        0x18, /* 00011000 */
        0x18, /* 00011000 */
        0x18, /* 00011000 */
        0xFF, /* 11111111 */
        0x7E, /* 01111110 */
        0x3C, /* 00111100 */
        0x18, /* 00011000 */
        0x00, /* 00000000 */
        0x00, /* 00000000 */
        0x00, /* 00000000 */
        0x00, /* 00000000 */
        0x00, /* 00000000 */
        0x00  /* 00000000 */
    };

    /* Smiley face (8x8) */
    const UINT8 smiley[] = {
        0x3C, /* 00111100 */
        0x42, /* 01000010 */
        0xA5, /* 10100101 */
        0x81, /* 10000001 */
        0xA5, /* 10100101 */
        0x99, /* 10011001 */
        0x42, /* 01000010 */
        0x3C  /* 00111100 */
    };

    /* Cross/Plus pattern (8x8) */
    const UINT8 cross[] = {
        0x18, /* 00011000 */
        0x18, /* 00011000 */
        0x18, /* 00011000 */
        0xFF, /* 11111111 */
        0xFF, /* 11111111 */
        0x18, /* 00011000 */
        0x18, /* 00011000 */
        0x18  /* 00011000 */
    };

    /* Diagonal pattern (8x8) */
    const UINT8 diagonal[] = {
        0x80, /* 10000000 */
        0x40, /* 01000000 */
        0x20, /* 00100000 */
        0x10, /* 00010000 */
        0x08, /* 00001000 */
        0x04, /* 00000100 */
        0x02, /* 00000010 */
        0x01  /* 00000001 */
    };

    /* Clear screen first */
    clear_screen((UINT32 *)base);

    /* Test 1: Word-aligned, even height (optimized path) */
    plot_bitmap_8(base, 10, 0, checkerboard, 8);

    /* Test 2: Word-aligned, even height at different position */
    plot_bitmap_8(base, 10, 16, solid_block, 8);

    /* Test 3: Odd column position (byte copy path) */
    plot_bitmap_8(base, 10, 25, smiley, 8);

    /* Test 4: Various positions across the screen */
    plot_bitmap_8(base, 30, 0, cross, 8);
    plot_bitmap_8(base, 30, 10, diagonal, 8);
    plot_bitmap_8(base, 30, 20, checkerboard, 8);
    plot_bitmap_8(base, 30, 30, smiley, 8);

    /* Test 5: Larger bitmap (even height, optimized) */
    plot_bitmap_8(base, 50, 8, arrow_down, 16);

    /* Test 6: Multiple bitmaps in a row */
    plot_bitmap_8(base, 80, 0, solid_block, 8);
    plot_bitmap_8(base, 80, 8, checkerboard, 8);
    plot_bitmap_8(base, 80, 16, cross, 8);
    plot_bitmap_8(base, 80, 24, diagonal, 8);
    plot_bitmap_8(base, 80, 32, smiley, 8);
    plot_bitmap_8(base, 80, 40, solid_block, 8);

    /* Test 7: Multiple bitmaps in a grid */
    plot_bitmap_8(base, 100, 0, checkerboard, 8);
    plot_bitmap_8(base, 100, 10, smiley, 8);
    plot_bitmap_8(base, 100, 20, cross, 8);
    plot_bitmap_8(base, 110, 0, diagonal, 8);
    plot_bitmap_8(base, 110, 10, solid_block, 8);
    plot_bitmap_8(base, 110, 20, checkerboard, 8);
    plot_bitmap_8(base, 120, 0, cross, 8);
    plot_bitmap_8(base, 120, 10, diagonal, 8);
    plot_bitmap_8(base, 120, 20, smiley, 8);

    /* Test 8: Odd height bitmap (forces byte copy path) */
    plot_bitmap_8(base, 140, 0, arrow_down, 15);

    /* Test 9: Various positions testing alignment */
    plot_bitmap_8(base, 170, 1, smiley, 8);  /* odd col */
    plot_bitmap_8(base, 170, 11, smiley, 8); /* odd col */
    plot_bitmap_8(base, 170, 21, smiley, 8); /* odd col */
    plot_bitmap_8(base, 170, 31, smiley, 8); /* odd col */

    /* Test 10: Bounds checking - Left edge clipping */
    plot_bitmap_8(base, 20, -7, solid_block, 8);
    plot_bitmap_8(base, 30, -4, checkerboard, 8);
    plot_bitmap_8(base, 40, -2, cross, 8);
    plot_bitmap_8(base, 50, -1, diagonal, 8);
    plot_bitmap_8(base, 60, 0, smiley, 8);

    /* Test 11: Bounds checking - Right edge clipping */
    plot_bitmap_8(base, 80, 633, solid_block, 8);
    plot_bitmap_8(base, 90, 635, checkerboard, 8);
    plot_bitmap_8(base, 100, 637, cross, 8);
    plot_bitmap_8(base, 110, 638, diagonal, 8);
    plot_bitmap_8(base, 120, 639, smiley, 8);

    /* Test 14: Bounds checking - Entirely out of bounds (should not crash) */

    plot_bitmap_8(base, -20, 100, smiley, 8);
    plot_bitmap_8(base, 450, 100, smiley, 8);
    plot_bitmap_8(base, 100, -20, smiley, 8);
    plot_bitmap_8(base, 100, 650, smiley, 8);

    Cnecin();
}

void test_plot_bitmap_16(UINT8 *base)
{
    /* Test bitmaps - 16 pixels wide, 2 bytes per row */

    /* Checkerboard pattern (16x8) */
    const UINT16 checkerboard_16[] = {
        0xAAAA, /* 1010101010101010 */
        0x5555, /* 0101010101010101 */
        0xAAAA, /* 1010101010101010 */
        0x5555, /* 0101010101010101 */
        0xAAAA, /* 1010101010101010 */
        0x5555, /* 0101010101010101 */
        0xAAAA, /* 1010101010101010 */
        0x5555  /* 0101010101010101 */
    };

    /* Solid block (16x8) */
    const UINT16 solid_block_16[] = {
        0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
        0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF};

    /* Large smiley (16x16) */
    const UINT16 smiley_16[] = {
        0x0FF0, /* 0000111111110000 */
        0x3FFC, /* 0011111111111100 */
        0x7FFE, /* 0111111111111110 */
        0x7E7E, /* 0111111001111110 */
        0xF3CF, /* 1111001111001111 */
        0xF3CF, /* 1111001111001111 */
        0xFFFF, /* 1111111111111111 */
        0xFFFF, /* 1111111111111111 */
        0xFFFF, /* 1111111111111111 */
        0xF3CF, /* 1111001111001111 */
        0xF3CF, /* 1111001111001111 */
        0x7FFE, /* 0111111111111110 */
        0x7FFE, /* 0111111111111110 */
        0x3FFC, /* 0011111111111100 */
        0x0FF0, /* 0000111111110000 */
        0x0000  /* 0000000000000000 */
    };

    /* Border frame (16x16) */
    const UINT16 frame_16[] = {
        0xFFFF, /* 1111111111111111 */
        0xFFFF, /* 1111111111111111 */
        0xC003, /* 1100000000000011 */
        0xC003, /* 1100000000000011 */
        0xC003, /* 1100000000000011 */
        0xC003, /* 1100000000000011 */
        0xC003, /* 1100000000000011 */
        0xC003, /* 1100000000000011 */
        0xC003, /* 1100000000000011 */
        0xC003, /* 1100000000000011 */
        0xC003, /* 1100000000000011 */
        0xC003, /* 1100000000000011 */
        0xC003, /* 1100000000000011 */
        0xC003, /* 1100000000000011 */
        0xFFFF, /* 1111111111111111 */
        0xFFFF  /* 1111111111111111 */
    };

    /* Diamond pattern (16x16) */
    const UINT16 diamond_16[] = {
        0x0180, /* 0000000110000000 */
        0x03C0, /* 0000001111000000 */
        0x07E0, /* 0000011111100000 */
        0x0FF0, /* 0000111111110000 */
        0x1FF8, /* 0001111111111000 */
        0x3FFC, /* 0011111111111100 */
        0x7FFE, /* 0111111111111110 */
        0xFFFF, /* 1111111111111111 */
        0xFFFF, /* 1111111111111111 */
        0x7FFE, /* 0111111111111110 */
        0x3FFC, /* 0011111111111100 */
        0x1FF8, /* 0001111111111000 */
        0x0FF0, /* 0000111111110000 */
        0x07E0, /* 0000011111100000 */
        0x03C0, /* 0000001111000000 */
        0x0180  /* 0000000110000000 */
    };

    /* Clear screen first */
    clear_screen((UINT32 *)base);

    /* Test 1: Long-aligned, even height (optimized path) */
    plot_bitmap_16(base, 10, 0, checkerboard_16, 8);

    /* Test 2: Long-aligned, even height at different position */
    plot_bitmap_16(base, 10, 32, solid_block_16, 8);

    /* Test 3: Word-aligned but not long-aligned */
    plot_bitmap_16(base, 10, 48, smiley_16, 16);

    /* Test 4: Various positions across the screen */
    plot_bitmap_16(base, 30, 0, frame_16, 16);
    plot_bitmap_16(base, 30, 20, diamond_16, 16);
    plot_bitmap_16(base, 30, 40, checkerboard_16, 8);

    /* Test 5: Multiple bitmaps in a row */
    plot_bitmap_16(base, 60, 0, solid_block_16, 8);
    plot_bitmap_16(base, 60, 16, checkerboard_16, 8);
    plot_bitmap_16(base, 60, 32, frame_16, 16);
    plot_bitmap_16(base, 60, 48, diamond_16, 16);

    /* Test 6: Multiple bitmaps in a grid */
    plot_bitmap_16(base, 100, 0, checkerboard_16, 8);
    plot_bitmap_16(base, 100, 20, smiley_16, 16);
    plot_bitmap_16(base, 100, 40, frame_16, 16);
    plot_bitmap_16(base, 110, 0, diamond_16, 16);
    plot_bitmap_16(base, 110, 20, solid_block_16, 8);
    plot_bitmap_16(base, 110, 40, checkerboard_16, 8);

    /* Test 7: Odd height bitmap (forces word copy path) */
    plot_bitmap_16(base, 140, 0, smiley_16, 15);

    /* Test 8: Various positions testing alignment */
    plot_bitmap_16(base, 170, 0, diamond_16, 16);
    plot_bitmap_16(base, 170, 24, diamond_16, 16);
    plot_bitmap_16(base, 170, 48, diamond_16, 16);

    /* Test 9: Bounds checking - LEFT EDGE offsets (safe vertical: y=50-100) */
    clear_screen((UINT32 *)base);
    plot_bitmap_16(base, 50, -16, solid_block_16, 8);  /* x=-16: 16px off */
    plot_bitmap_16(base, 60, -14, checkerboard_16, 8); /* x=-14: 14px off */
    plot_bitmap_16(base, 70, -12, frame_16, 16);       /* x=-12: 12px off */
    plot_bitmap_16(base, 80, -10, smiley_16, 16);      /* x=-10: 10px off */
    plot_bitmap_16(base, 90, -8, solid_block_16, 8);   /* x=-8: 8px off */
    plot_bitmap_16(base, 50, -6, checkerboard_16, 8);  /* x=-6: 6px off */
    plot_bitmap_16(base, 60, -4, frame_16, 16);        /* x=-4: 4px off */
    plot_bitmap_16(base, 70, -2, diamond_16, 16);      /* x=-2: 2px off */
    plot_bitmap_16(base, 80, 0, smiley_16, 16);        /* x=0: at left edge */
    plot_bitmap_16(base, 90, 8, solid_block_16, 8);    /* x=8: fully onscreen */
    plot_bitmap_16(base, 50, 16, checkerboard_16, 8);  /* x=16: fully onscreen */
    plot_bitmap_16(base, 60, 32, diamond_16, 16);      /* x=32: fully onscreen */

    /* Test 10: Bounds checking - RIGHT EDGE offsets (safe vertical: y=110-160) */
    plot_bitmap_16(base, 110, 632, solid_block_16, 8);  /* x=632: 8px off */
    plot_bitmap_16(base, 120, 630, checkerboard_16, 8); /* x=630: 10px off */
    plot_bitmap_16(base, 130, 628, frame_16, 16);       /* x=628: 12px off */
    plot_bitmap_16(base, 140, 626, smiley_16, 16);      /* x=626: 14px off */
    plot_bitmap_16(base, 50, 624, solid_block_16, 8);   /* x=624: fully clipped */
    plot_bitmap_16(base, 60, 622, checkerboard_16, 8);  /* x=622: fully clipped */
    plot_bitmap_16(base, 70, 620, frame_16, 16);        /* x=620: fully clipped */
    plot_bitmap_16(base, 80, 616, diamond_16, 16);      /* x=616: fully onscreen */
    plot_bitmap_16(base, 90, 608, smiley_16, 16);       /* x=608: fully onscreen */
    plot_bitmap_16(base, 110, 600, solid_block_16, 8);  /* x=600: fully onscreen */
    plot_bitmap_16(base, 120, 592, checkerboard_16, 8); /* x=592: fully onscreen */
    plot_bitmap_16(base, 130, 584, diamond_16, 16);     /* x=584: fully onscreen */

    /* Test 11-12: Vertical bounds (safe - never off top/bottom) */
    plot_bitmap_16(base, 0, 140, frame_16, 16);   /* y=0: at top edge */
    plot_bitmap_16(base, 384, 140, frame_16, 16); /* y=384: at bottom edge */

    /* Test 13: Completely out of bounds (should not crash) */
    plot_bitmap_16(base, -30, 100, smiley_16, 16);
    plot_bitmap_16(base, 450, 100, smiley_16, 16);
    plot_bitmap_16(base, 100, -30, smiley_16, 16);
    plot_bitmap_16(base, 100, 650, smiley_16, 16);

    Cnecin();
}

void test_plot_bitmap_32(UINT8 *base)
{
    /* Test bitmaps - 32 pixels wide, 4 bytes per row */

    /* Checkerboard pattern (32x8) */
    const UINT32 checkerboard_32[] = {
        0xAAAAAAAA, /* 10101010101010101010101010101010 */
        0x55555555, /* 01010101010101010101010101010101 */
        0xAAAAAAAA, /* 10101010101010101010101010101010 */
        0x55555555, /* 01010101010101010101010101010101 */
        0xAAAAAAAA, /* 10101010101010101010101010101010 */
        0x55555555, /* 01010101010101010101010101010101 */
        0xAAAAAAAA, /* 10101010101010101010101010101010 */
        0x55555555, /* 01010101010101010101010101010101 */
        0xAAAAAAAA, /* 10101010101010101010101010101010 */
        0x55555555, /* 01010101010101010101010101010101 */
        0xAAAAAAAA, /* 10101010101010101010101010101010 */
        0x55555555, /* 01010101010101010101010101010101 */
        0xAAAAAAAA, /* 10101010101010101010101010101010 */
        0x55555555, /* 01010101010101010101010101010101 */
        0xAAAAAAAA, /* 10101010101010101010101010101010 */
        0x55555555, /* 01010101010101010101010101010101 */
        0xAAAAAAAA, /* 10101010101010101010101010101010 */
        0x55555555, /* 01010101010101010101010101010101 */
        0xAAAAAAAA, /* 10101010101010101010101010101010 */
        0x55555555, /* 01010101010101010101010101010101 */
        0xAAAAAAAA, /* 10101010101010101010101010101010 */
        0x55555555, /* 01010101010101010101010101010101 */
        0xAAAAAAAA, /* 10101010101010101010101010101010 */
        0x55555555, /* 01010101010101010101010101010101 */
        0xAAAAAAAA, /* 10101010101010101010101010101010 */
        0x55555555, /* 01010101010101010101010101010101 */
        0xAAAAAAAA, /* 10101010101010101010101010101010 */
        0x55555555, /* 01010101010101010101010101010101 */
        0xAAAAAAAA, /* 10101010101010101010101010101010 */
        0x55555555, /* 01010101010101010101010101010101 */
        0xAAAAAAAA, /* 10101010101010101010101010101010 */
        0x55555555  /* 01010101010101010101010101010101 */
    };

    /* Solid block (32x8) */
    const UINT32 solid_block_32[] = {
        0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
        0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF};

    /* Large smiley (32x16) */
    const UINT32 smiley_32[] = {
        0x00FFFF00, /* 00000000111111111111111100000000 */
        0x03FFFF80, /* 00000011111111111111111110000000 */
        0x07FFFFC0, /* 00000111111111111111111111000000 */
        0x0FF00FF0, /* 00001111111100000000111111110000 */
        0x1FE00FFC, /* 00011111111000000000111111111100 */
        0x3FC003FE, /* 00111111110000000000001111111110 */
        0x7F8001FF, /* 01111111100000000000000111111111 */
        0x7F8001FF, /* 01111111100000000000000111111111 */
        0xFF0000FF, /* 11111111000000000000000011111111 */
        0xFF0000FF, /* 11111111000000000000000011111111 */
        0xFF0000FF, /* 11111111000000000000000011111111 */
        0x7F8001FF, /* 01111111100000000000000111111111 */
        0x3FC003FE, /* 00111111110000000000001111111110 */
        0x1FE00FFC, /* 00011111111000000000111111111100 */
        0x0FF00FF0, /* 00001111111100000000111111110000 */
        0x03FFFF80  /* 00000011111111111111111110000000 */
    };

    /* Border frame (32x16) */
    const UINT32 frame_32[] = {
        0xFFFFFFFF, /* 11111111111111111111111111111111 */
        0xFFFFFFFF, /* 11111111111111111111111111111111 */
        0xFFFFFFFF, /* 11111111111111111111111111111111 */
        0xC0000003, /* 11000000000000000000000000000011 */
        0xC0000003, /* 11000000000000000000000000000011 */
        0xC0000003, /* 11000000000000000000000000000011 */
        0xC0000003, /* 11000000000000000000000000000011 */
        0xC0000003, /* 11000000000000000000000000000011 */
        0xC0000003, /* 11000000000000000000000000000011 */
        0xC0000003, /* 11000000000000000000000000000011 */
        0xC0000003, /* 11000000000000000000000000000011 */
        0xC0000003, /* 11000000000000000000000000000011 */
        0xC0000003, /* 11000000000000000000000000000011 */
        0xFFFFFFFF, /* 11111111111111111111111111111111 */
        0xFFFFFFFF, /* 11111111111111111111111111111111 */
        0xFFFFFFFF  /* 11111111111111111111111111111111 */
    };

    /* Arrow pattern (32x37) */
    const UINT32 arrow_32[] = {
        0xFFFFFFFF, /* 11111111111111111111111111111111 */
        0xFFFFFFFF, /* 11111111111111111111111111111111 */
        0xFFFFFFFF, /* 11111111111111111111111111111111 */
        0xFFFFFFFF, /* 11111111111111111111111111111111 */
        0xFFFFFFFF, /* 11111111111111111111111111111111 */
        0xFFFFFFFF, /* 11111111111111111111111111111111 */
        0xFFFFFFFF, /* 11111111111111111111111111111111 */
        0xFFFFFFFF, /* 11111111111111111111111111111111 */
        0xFFFFFFFF, /* 11111111111111111111111111111111 */
        0xFFFFFFFF, /* 11111111111111111111111111111111 */
        0xFFFFFFFF, /* 11111111111111111111111111111111 */
        0xFFFFFFFF, /* 11111111111111111111111111111111 */
        0xFFFFFFFF, /* 11111111111111111111111111111111 */
        0xFFFFFFFF, /* 11111111111111111111111111111111 */
        0xFFFFFFFF, /* 11111111111111111111111111111111 */
        0xFFFFFFFF, /* 11111111111111111111111111111111 */
        0xFFFFFFFF, /* 11111111111111111111111111111111 */
        0xFFFFFFFF, /* 11111111111111111111111111111111 */
        0xFFFFFFFF, /* 11111111111111111111111111111111 */
        0xFFFFFFFF, /* 11111111111111111111111111111111 */
        0xFFFFFFFF, /* 11111111111111111111111111111111 */
        0xFFFFFFFF, /* 11111111111111111111111111111111 */
        0xFFFFFFFF, /* 11111111111111111111111111111111 */
        0xFFFFFFFF, /* 11111111111111111111111111111111 */
        0xFFFFFFFF, /* 11111111111111111111111111111111 */
        0xFFFFFFFF, /* 11111111111111111111111111111111 */
        0xFFFFFFFF, /* 11111111111111111111111111111111 */
        0xFFFFFFFF, /* 11111111111111111111111111111111 */
        0xFFFFFFFF, /* 11111111111111111111111111111111 */
        0xFFFFFFFF, /* 11111111111111111111111111111111 */
        0xFFFFFFFF, /* 11111111111111111111111111111111 */
        0xFFFFFFFF, /* 11111111111111111111111111111111 */
        0xFFFFFFFF, /* 11111111111111111111111111111111 */
        0xFFFFFFFF, /* 11111111111111111111111111111111 */
        0xFFFFFFFF, /* 11111111111111111111111111111111 */
        0xFFFFFFFF, /* 11111111111111111111111111111111 */
        0xFFFFFFFF  /* 11111111111111111111111111111111 */
    };

    /* Clear screen first */
    clear_screen((UINT32 *)base);

    /* Test 1: Long-aligned, even height (optimized path with movem) */
    plot_bitmap_32(base, 10, 0, checkerboard_32, 32);

    /* Test 2: Long-aligned, even height at different position */
    plot_bitmap_32(base, 10, 64, solid_block_32, 8);

    /* Test 3: Odd column position (long copy path) */
    plot_bitmap_32(base, 10, 100, smiley_32, 16);

    /* Test 6: Multiple bitmaps in a grid */

    /* Test 7: Odd height bitmap (forces long copy path) */
    plot_bitmap_32(base, 140, 0, smiley_32, 15);

    /* Test 8: Various positions testing alignment */
    plot_bitmap_32(base, 170, 0, arrow_32, 37);
    plot_bitmap_32(base, 170, 48, arrow_32, 37);
    plot_bitmap_32(base, 170, 96, arrow_32, 37);

    /* Test 9: Bounds checking - LEFT EDGE offsets (safe vertical: y=50-100) */
    clear_screen((UINT32 *)base);
    plot_bitmap_32(base, 50, -32, solid_block_32, 8);  /* x=-32: fully off left */
    plot_bitmap_32(base, 60, -30, checkerboard_32, 8); /* x=-30: 30px off */
    plot_bitmap_32(base, 70, -28, frame_32, 16);       /* x=-28: 28px off */
    plot_bitmap_32(base, 80, -24, smiley_32, 16);      /* x=-24: 24px off */
    plot_bitmap_32(base, 90, -20, solid_block_32, 8);  /* x=-20: 20px off */
    plot_bitmap_32(base, 50, -16, checkerboard_32, 8); /* x=-16: 16px off */
    plot_bitmap_32(base, 60, -12, frame_32, 16);       /* x=-12: 12px off */
    plot_bitmap_32(base, 70, -8, frame_32, 16);        /* x=-8: 8px off */
    plot_bitmap_32(base, 80, -4, smiley_32, 16);       /* x=-4: 4px off */

    /* Test 10: Bounds checking - RIGHT EDGE offsets (safe vertical: y=110-160) */
    plot_bitmap_32(base, 120, 624, solid_block_32, 8);  /* x=624: 16px off */
    plot_bitmap_32(base, 130, 620, checkerboard_32, 8); /* x=620: 20px off */
    plot_bitmap_32(base, 140, 622, frame_32, 16);       /* x=616: 24px off */
    plot_bitmap_32(base, 50, 638, smiley_32, 16);       /* x=608: fully clipped */
    plot_bitmap_32(base, 60, 632, solid_block_32, 8);   /* x=600: fully clipped */
    plot_bitmap_32(base, 70, 624, checkerboard_32, 8);  /* x=592: fully clipped */
    plot_bitmap_32(base, 80, 639, frame_32, 16);        /* x=584: fully onscreen */
    plot_bitmap_32(base, 90, 638, smiley_32, 16);       /* x=576: fully onscreen */
    plot_bitmap_32(base, 110, 633, arrow_32, 16);       /* x=568: fully onscreen */
    plot_bitmap_32(base, 120, 610, solid_block_32, 8);  /* x=560: fully onscreen */
    plot_bitmap_32(base, 130, 608, checkerboard_32, 8); /* x=552: fully onscreen */
    plot_bitmap_32(base, 140, 544, arrow_32, 16);       /* x=544: fully onscreen */

    /* Test 11-12: Vertical bounds (safe - never off top/bottom) */
    plot_string(base, 170, 10, "Vertical Bounds:");
    plot_bitmap_32(base, 0, 140, frame_32, 16);   /* y=0: at top edge */
    plot_bitmap_32(base, 384, 140, frame_32, 16); /* y=384: at bottom edge */

    /* Test 13: Completely out of bounds (should not crash) */
    plot_bitmap_32(base, -50, 100, smiley_32, 16);
    plot_bitmap_32(base, 450, 100, smiley_32, 16);
    plot_bitmap_32(base, 100, -50, smiley_32, 16);
    plot_bitmap_32(base, 100, 650, smiley_32, 16);

    Cnecin();
}

void test_plot_character(UINT8 *base)
{
    int i;

    /* Clear screen first */
    clear_screen((UINT32 *)base);

    /* Test 1: Plot individual characters */
    plot_character(base, 10, 10, 'A');
    plot_character(base, 10, 20, 'B');
    plot_character(base, 10, 30, 'C');
    plot_character(base, 10, 40, 'D');

    /* Test 2: Plot digits */
    plot_character(base, 30, 10, '0');
    plot_character(base, 30, 20, '1');
    plot_character(base, 30, 30, '2');
    plot_character(base, 30, 40, '3');
    plot_character(base, 30, 50, '4');

    /* Test 3: Plot special characters */
    plot_character(base, 50, 10, '!');
    plot_character(base, 50, 20, '@');
    plot_character(base, 50, 30, '#');
    plot_character(base, 50, 40, '$');
    plot_character(base, 50, 50, '%');

    /* Test 4: Plot all uppercase letters */
    for (i = 0; i < 26; i++)
    {
        plot_character(base, 80, 10 + (i * 8), 'A' + i);
    }

    /* Test 5: Plot all lowercase letters */
    for (i = 0; i < 26; i++)
    {
        plot_character(base, 100, 10 + (i * 8), 'a' + i);
    }

    /* Test 6: Plot all digits */
    for (i = 0; i < 10; i++)
    {
        plot_character(base, 120, 10 + (i * 8), '0' + i);
    }

    /* Test 7: Plot various symbols */
    plot_character(base, 140, 10, '(');
    plot_character(base, 140, 20, ')');
    plot_character(base, 140, 30, '[');
    plot_character(base, 140, 40, ']');
    plot_character(base, 140, 50, '{');
    plot_character(base, 140, 60, '}');
    plot_character(base, 140, 70, '<');
    plot_character(base, 140, 80, '>');
    plot_character(base, 140, 90, '/');
    plot_character(base, 140, 100, '\\');

    /* Test 8: Plot at different vertical positions */
    for (i = 0; i < 10; i++)
    {
        plot_character(base, 170 + (i * 10), 10, '*');
    }
}

void test_plot_string(UINT8 *base)
{
    /* Clear screen first */
    clear_screen((UINT32 *)base);

    /* Test 1: Simple string */
    plot_string(base, 10, 10, "Hello, World!");

    /* Test 2: Alphabet */
    plot_string(base, 30, 10, "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
    plot_string(base, 50, 10, "abcdefghijklmnopqrstuvwxyz");

    /* Test 3: Numbers */
    plot_string(base, 70, 10, "0123456789");

    /* Test 4: Special characters */
    plot_string(base, 90, 10, "!@#$%^&*()_+-=[]{}|;:'\",.<>?/");

    /* Test 5: Multiple lines */
    plot_string(base, 110, 10, "Line 1: Testing plot_string");
    plot_string(base, 130, 10, "Line 2: Second line of text");
    plot_string(base, 150, 10, "Line 3: If youre reading this... Its behind you.");

    /* Test 6: Different starting positions */
    plot_string(base, 180, 50, "Offset text");
    plot_string(base, 200, 100, "More offset");
    plot_string(base, 220, 150, "Even more");

    /* Test 7: Short strings */
    plot_string(base, 250, 10, "A");
    plot_string(base, 250, 30, "Hi");
    plot_string(base, 250, 60, "OK");
    plot_string(base, 250, 90, "Yes");

    /* Test 8: Information display */
    plot_string(base, 280, 10, "Atari ST Raster Library");
    plot_string(base, 300, 10, "I see you ;)");

    /* Test 9: Centered text (approximate) */
    plot_string(base, 330, 240, "CENTERED");

    /* Test 10: Empty string (should do nothing) */
    plot_string(base, 350, 10, "");
}

void fill_screen(UINT32 *base, char pattern)
{
    register int i = 0;
    register UINT32 *loc = base;

    while (i++ < BYTES_PER_SCREEN / 4)
        *(loc++) = pattern;
}

int main(void)
{
    UINT8 *base = (UINT8 *)Physbase();

    disable_cursor();

    /* Test 1: Clear Screen */
    fill_screen((UINT32 *)base, -1); /* fill with white */
    Cnecin();
    test_clear_screen(base);
    Cnecin();

    /* Test 2: Clear Region - various sizes and positions */
    test_clear_region(base);
    Cnecin();

    /* Test 3: Plot Pixel - draw patterns */
    test_plot_pixel(base);
    Cnecin();

    /* Test 4: Plot Horizontal Line */
    test_plot_horizontal_line(base);
    Cnecin();

    /* Test 5: Plot Vertical Line */
    test_plot_vertical_line(base);
    Cnecin();

    /* Test 6: Plot Line - arbitrary lines */
    test_plot_line(base);
    Cnecin();

    /* Test 7: Plot Rectangle - filled rectangles */
    test_plot_rectangle(base);
    Cnecin();

    /* Test 8: Plot Square - filled squares */
    test_plot_square(base);
    Cnecin();

    /* Test 9: Plot Triangle - filled triangles */
    test_plot_triangle(base);
    Cnecin();

    /* Test 10: Plot Bitmap 8 - 8-pixel wide bitmaps */
    test_plot_bitmap_8(base);
    Cnecin();

    /* Test 11: Plot Bitmap 16 - 16-pixel wide bitmaps */
    test_plot_bitmap_16(base);
    Cnecin();

    /* Test 12: Plot Bitmap 32 - 32-pixel wide bitmaps */
    test_plot_bitmap_32(base);
    Cnecin();

    /* Test 13: Plot Character - individual characters */
    test_plot_character(base);
    Cnecin();

    /* Test 14: Plot String - text strings */
    test_plot_string(base);

    Cnecin();

    return 0;
}
