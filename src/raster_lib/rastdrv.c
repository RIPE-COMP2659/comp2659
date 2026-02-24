#include <stdio.h>
#include <stdlib.h>
#include <osbind.h>

/* Include the raster library header */
#include "raster.h"

#define BYTES_PER_SCREEN 32000

void disable_cursor();
void enable_cursor();
void fill_screen(UINT32 *base, char pattern);
void test_clear_screen(UINT8 *base);
void test_clear_region(UINT8 *base);
void test_plot_pixel(UINT8 *base);
void test_plot_horizontal_line(UINT8 *base);
void test_plot_vertical_line(UINT8 *base);
void test_plot_line(UINT8 *base);
void test_plot_rectangle(UINT8 *base);
void test_plot_square(UINT8 *base);
void test_plot_triangle(UINT8 *base);
void test_plot_bitmap_8(UINT8 *base);
void test_plot_bitmap_16(UINT8 *base);
void test_plot_bitmap_32(UINT8 *base);
void test_plot_character(UINT8 *base);
void test_plot_string(UINT8 *base);

int main()
{
    UINT8 *base = (UINT8 *)Physbase();

    disable_cursor();

    /* Pre-fill screen so the clear_screen test is obvious */
    fill_screen((UINT32 *)base, -1); /* fill with white/black */
    con_printf("\033H\n=== RASTER LIBRARY TEST SUITE ===\n");
    con_printf("Screen filled. Press any key to test clear_screen...\n");
    fflush(stdout);
    Cnecin();

    /* Test 1: Clear Screen */
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

    /* Cleanup and exit */
    clear_screen((UINT32 *)base);
    enable_cursor();
    return 0;
}

void test_clear_screen(UINT8 *base)
{
    clear_screen((UINT32 *)base);
    con_printf("\033H\n=== TEST 1: CLEAR SCREEN ===\n");
    con_printf("Testing module: clr_scrn.o\n");
    con_printf("The screen should now be completely clear.\n");
    fflush(stdout);
}

void test_clear_region(UINT8 *base)
{
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

    con_printf("\033H\n=== TEST 2: CLEAR REGION ===\n");
    con_printf("Testing module: clr_reg.o\n");
    con_printf("Drawing solid screen with multiple erased (cleared) cutout blocks.\n");
    fflush(stdout);
}

void test_plot_pixel(UINT8 *base)
{
    int i;
    clear_screen((UINT32 *)base);
    
    con_printf("\033H\n=== TEST 3: PLOT PIXEL ===\n");
    con_printf("Testing module: plt_pxl.o\n");
    con_printf("Drawing individual pixels to form lines, boxes, and bit-boundary tests.\n");
    fflush(stdout);

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
    clear_screen((UINT32 *)base);
    con_printf("\033H\n=== TEST 4: HORIZONTAL LINES ===\n");
    con_printf("Testing module: plt_hor.o\n");
    con_printf("Drawing rapid horizontal lines crossing byte boundaries and a ladder pattern.\n");
    fflush(stdout);

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
    clear_screen((UINT32 *)base);
    con_printf("\033H\n=== TEST 5: VERTICAL LINES ===\n");
    con_printf("Testing module: plt_vert.o\n");
    con_printf("Drawing vertical lines across rows, including a grid pattern.\n");
    fflush(stdout);

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
    clear_screen((UINT32 *)base);
    con_printf("\033H\n=== TEST 6: BRESENHAM LINES ===\n");
    con_printf("Testing module: plt_line.o\n");
    con_printf("Drawing lines in all 8 directional octants, plus a triangle and star.\n");
    fflush(stdout);

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
    clear_screen((UINT32 *)base);
    con_printf("\033H\n=== TEST 7: RECTANGLES ===\n");
    con_printf("Testing module: plt_rec.o\n");
    con_printf("Drawing filled rectangles across boundaries and optimized paths.\n");
    fflush(stdout);

    /* Test 1: 48x48 optimized path - word aligned */
    plot_rectangle((UINT32 *)base, 0, 0, 48, 48);

    /* Test 2: 48x48 optimized path - different position */
    plot_rectangle((UINT32 *)base, 50, 80, 48, 48);

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
    clear_screen((UINT32 *)base);
    con_printf("\033H\n=== TEST 8: SQUARES ===\n");
    con_printf("Testing module: plt_sqr.o\n");
    con_printf("Drawing perfect uniform squares.\n");
    fflush(stdout);

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
    clear_screen((UINT32 *)base);
    con_printf("\033H\n=== TEST 9: TRIANGLES ===\n");
    con_printf("Testing module: plt_tri.o\n");
    con_printf("Drawing multiple right-angle triangles in 4 orientations.\n");
    fflush(stdout);

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

void test_plot_bitmap_8(UINT8 *base)
{
    /* Test bitmaps - 8 pixels wide, 1 byte per row */
    const UINT8 checkerboard[] = { 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55 };
    const UINT8 solid_block[] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
    const UINT8 arrow_down[] = { 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0xFF, 0x7E, 0x3C, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    const UINT8 smiley[] = { 0x3C, 0x42, 0xA5, 0x81, 0xA5, 0x99, 0x42, 0x3C };
    const UINT8 cross[] = { 0x18, 0x18, 0x18, 0xFF, 0xFF, 0x18, 0x18, 0x18 };
    const UINT8 diagonal[] = { 0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01 };

    clear_screen((UINT32 *)base);
    con_printf("\033H\n=== TEST 10: BITMAPS (8px) & CLIPPING ===\n");
    con_printf("Testing modules: bmap_8.o, bounds.o, plt_clip.o\n");
    con_printf("Drawing 8px sprites. Testing edge collision safety.\n");
    fflush(stdout);

    /* Normal Drawings */
    plot_bitmap_8(base, 10, 0, checkerboard, 8);
    plot_bitmap_8(base, 10, 16, solid_block, 8);
    plot_bitmap_8(base, 10, 25, smiley, 8);
    plot_bitmap_8(base, 30, 0, cross, 8);
    plot_bitmap_8(base, 30, 10, diagonal, 8);
    plot_bitmap_8(base, 30, 20, checkerboard, 8);
    plot_bitmap_8(base, 30, 30, smiley, 8);
    plot_bitmap_8(base, 50, 8, arrow_down, 16);
    plot_bitmap_8(base, 80, 0, solid_block, 8);
    plot_bitmap_8(base, 80, 8, checkerboard, 8);
    plot_bitmap_8(base, 80, 16, cross, 8);
    plot_bitmap_8(base, 80, 24, diagonal, 8);
    plot_bitmap_8(base, 80, 32, smiley, 8);
    plot_bitmap_8(base, 80, 40, solid_block, 8);
    
    plot_bitmap_8(base, 100, 0, checkerboard, 8);
    plot_bitmap_8(base, 100, 10, smiley, 8);
    plot_bitmap_8(base, 100, 20, cross, 8);
    plot_bitmap_8(base, 110, 0, diagonal, 8);
    plot_bitmap_8(base, 110, 10, solid_block, 8);
    plot_bitmap_8(base, 110, 20, checkerboard, 8);
    plot_bitmap_8(base, 120, 0, cross, 8);
    plot_bitmap_8(base, 120, 10, diagonal, 8);
    plot_bitmap_8(base, 120, 20, smiley, 8);

    plot_bitmap_8(base, 140, 0, arrow_down, 15);
    plot_bitmap_8(base, 170, 1, smiley, 8); 
    plot_bitmap_8(base, 170, 11, smiley, 8);
    plot_bitmap_8(base, 170, 21, smiley, 8);
    plot_bitmap_8(base, 170, 31, smiley, 8);

    /* Wait for input before clipping tests */
    Cnecin();
    clear_screen((UINT32 *)base);

    con_printf("\033H\n--- CLIPPING TESTS (bmap_8) ---\n");
    con_printf("Drawing shapes OFF the edges. Screen MUST NOT crash.\n");
    fflush(stdout);

    plot_string(base, 5, 20, "Left Edge Clipping:");
    plot_bitmap_8(base, 20, -4, solid_block, 8); 
    plot_bitmap_8(base, 30, -2, checkerboard, 8);
    plot_bitmap_8(base, 40, 0, cross, 8); 

    plot_string(base, 60, 20, "Right Edge Clipping:");
    plot_bitmap_8(base, 80, 636, solid_block, 8); 
    plot_bitmap_8(base, 90, 638, checkerboard, 8);
    plot_bitmap_8(base, 100, 632, cross, 8); 

    /* Bounds checking - Entirely out of bounds (should not crash) */
    plot_bitmap_8(base, -20, 100, smiley, 8);
    plot_bitmap_8(base, 450, 100, smiley, 8);
    plot_bitmap_8(base, 100, -20, smiley, 8);
    plot_bitmap_8(base, 100, 650, smiley, 8);
}

void test_plot_bitmap_16(UINT8 *base)
{
    /* Test bitmaps - 16 pixels wide, 2 bytes per row */
    const UINT16 checkerboard_16[] = { 0xAAAA, 0x5555, 0xAAAA, 0x5555, 0xAAAA, 0x5555, 0xAAAA, 0x5555 };
    const UINT16 solid_block_16[] = { 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF };
    const UINT16 smiley_16[] = { 0x0FF0, 0x3FFC, 0x7FFE, 0x7E7E, 0xF3CF, 0xF3CF, 0xFFFF, 0xFFFF, 0xFFFF, 0xF3CF, 0xF3CF, 0x7FFE, 0x7FFE, 0x3FFC, 0x0FF0, 0x0000 };
    const UINT16 frame_16[] = { 0xFFFF, 0xFFFF, 0xC003, 0xC003, 0xC003, 0xC003, 0xC003, 0xC003, 0xC003, 0xC003, 0xC003, 0xC003, 0xC003, 0xC003, 0xFFFF, 0xFFFF };
    const UINT16 diamond_16[] = { 0x0180, 0x03C0, 0x07E0, 0x0FF0, 0x1FF8, 0x3FFC, 0x7FFE, 0xFFFF, 0xFFFF, 0x7FFE, 0x3FFC, 0x1FF8, 0x0FF0, 0x07E0, 0x03C0, 0x0180 };

    clear_screen((UINT32 *)base);
    con_printf("\033H\n=== TEST 11: BITMAPS (16px) & CLIPPING ===\n");
    con_printf("Testing modules: bmap_16.o, bounds.o, plt_clip.o\n");
    con_printf("Drawing 16px sprites on aligned and unaligned offsets.\n");
    fflush(stdout);

    /* Normal tests */
    plot_bitmap_16(base, 10, 0, checkerboard_16, 8);
    plot_bitmap_16(base, 10, 32, solid_block_16, 8);
    plot_bitmap_16(base, 10, 48, smiley_16, 16);
    plot_bitmap_16(base, 30, 0, frame_16, 16);
    plot_bitmap_16(base, 30, 20, diamond_16, 16);
    plot_bitmap_16(base, 30, 40, checkerboard_16, 8);
    plot_bitmap_16(base, 60, 0, solid_block_16, 8);
    plot_bitmap_16(base, 60, 16, checkerboard_16, 8);
    plot_bitmap_16(base, 60, 32, frame_16, 16);
    plot_bitmap_16(base, 60, 48, diamond_16, 16);
    plot_bitmap_16(base, 100, 0, checkerboard_16, 8);
    plot_bitmap_16(base, 100, 20, smiley_16, 16);
    plot_bitmap_16(base, 100, 40, frame_16, 16);
    plot_bitmap_16(base, 110, 0, diamond_16, 16);
    plot_bitmap_16(base, 110, 20, solid_block_16, 8);
    plot_bitmap_16(base, 110, 40, checkerboard_16, 8);
    plot_bitmap_16(base, 140, 0, smiley_16, 15);
    plot_bitmap_16(base, 170, 0, diamond_16, 16);
    plot_bitmap_16(base, 170, 24, diamond_16, 16);
    plot_bitmap_16(base, 170, 48, diamond_16, 16);

    /* Wait for input before clipping tests */
    Cnecin();
    clear_screen((UINT32 *)base);

    con_printf("\033H\n--- CLIPPING TESTS (bmap_16) ---\n");
    con_printf("Testing safe edge clipping logic for 16px rendering.\n");
    fflush(stdout);

    plot_string(base, 5, 20, "Left Edge Clipping (16px):");
    plot_bitmap_16(base, 20, -8, solid_block_16, 8); 
    plot_bitmap_16(base, 30, -4, checkerboard_16, 8); 
    plot_bitmap_16(base, 40, 0, frame_16, 16); 

    plot_string(base, 60, 20, "Right Edge Clipping (16px):");
    plot_bitmap_16(base, 80, 632, solid_block_16, 8); 
    plot_bitmap_16(base, 90, 636, checkerboard_16, 8); 
    plot_bitmap_16(base, 100, 624, frame_16, 16); 

    plot_string(base, 120, 20, "Top Edge Clipping (16px):");
    plot_bitmap_16(base, -8, 100, solid_block_16, 8); 
    plot_bitmap_16(base, -4, 120, checkerboard_16, 8); 
    plot_bitmap_16(base, 0, 140, frame_16, 16); 

    plot_string(base, 340, 20, "Bottom Edge Clipping (16px):");
    plot_bitmap_16(base, 392, 100, smiley_16, 16); 
    plot_bitmap_16(base, 396, 120, checkerboard_16, 8); 
    plot_bitmap_16(base, 384, 140, frame_16, 16); 

    plot_bitmap_16(base, -30, 100, smiley_16, 16);
    plot_bitmap_16(base, 450, 100, smiley_16, 16);
    plot_bitmap_16(base, 100, -30, smiley_16, 16);
    plot_bitmap_16(base, 100, 650, smiley_16, 16);
}

void test_plot_bitmap_32(UINT8 *base)
{
    /* Test bitmaps - 32 pixels wide, 4 bytes per row */
    const UINT32 checkerboard_32[] = { 
        0xAAAAAAAA, 0x55555555, 0xAAAAAAAA, 0x55555555, 0xAAAAAAAA, 0x55555555, 0xAAAAAAAA, 0x55555555,
        0xAAAAAAAA, 0x55555555, 0xAAAAAAAA, 0x55555555, 0xAAAAAAAA, 0x55555555, 0xAAAAAAAA, 0x55555555,
        0xAAAAAAAA, 0x55555555, 0xAAAAAAAA, 0x55555555, 0xAAAAAAAA, 0x55555555, 0xAAAAAAAA, 0x55555555,
        0xAAAAAAAA, 0x55555555, 0xAAAAAAAA, 0x55555555, 0xAAAAAAAA, 0x55555555, 0xAAAAAAAA, 0x55555555
    };
    const UINT32 solid_block_32[] = { 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF };
    const UINT32 smiley_32[] = {
        0x00FFFF00, 0x03FFFF80, 0x07FFFFC0, 0x0FF00FF0, 0x1FE00FFC, 0x3FC003FE, 0x7F8001FF, 0x7F8001FF,
        0xFF0000FF, 0xFF0000FF, 0xFF0000FF, 0x7F8001FF, 0x3FC003FE, 0x1FE00FFC, 0x0FF00FF0, 0x03FFFF80
    };
    const UINT32 frame_32[] = {
        0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xC0000003, 0xC0000003, 0xC0000003, 0xC0000003, 0xC0000003,
        0xC0000003, 0xC0000003, 0xC0000003, 0xC0000003, 0xC0000003, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF
    };
    const UINT32 arrow_32[] = {
        0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
        0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
        0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
        0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
        0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF
    };

    clear_screen((UINT32 *)base);
    con_printf("\033H\n=== TEST 12: BITMAPS (32px) & CLIPPING ===\n");
    con_printf("Testing modules: bmap_32.o, bounds.o, plt_clip.o\n");
    con_printf("Drawing full 32px-wide characters/blocks.\n");
    fflush(stdout);

    /* Normal Drawings */
    plot_bitmap_32(base, 10, 0, checkerboard_32, 32);
    plot_bitmap_32(base, 10, 64, solid_block_32, 8);
    plot_bitmap_32(base, 10, 100, smiley_32, 16);
    plot_bitmap_32(base, 30, 0, frame_32, 16);
    plot_bitmap_32(base, 30, 40, arrow_32, 16);
    plot_bitmap_32(base, 30, 80, checkerboard_32, 8);
    plot_bitmap_32(base, 60, 0, solid_block_32, 8);
    plot_bitmap_32(base, 60, 32, checkerboard_32, 8);
    plot_bitmap_32(base, 60, 64, frame_32, 16);
    plot_bitmap_32(base, 60, 96, arrow_32, 16);
    plot_bitmap_32(base, 100, 0, checkerboard_32, 8);
    plot_bitmap_32(base, 100, 40, smiley_32, 16);
    plot_bitmap_32(base, 100, 80, frame_32, 16);
    plot_bitmap_32(base, 110, 0, arrow_32, 16);
    plot_bitmap_32(base, 110, 40, solid_block_32, 8);
    plot_bitmap_32(base, 110, 80, checkerboard_32, 8);
    plot_bitmap_32(base, 140, 0, smiley_32, 15);
    plot_bitmap_32(base, 170, 0, arrow_32, 37);
    plot_bitmap_32(base, 170, 48, arrow_32, 37);
    plot_bitmap_32(base, 170, 96, arrow_32, 37);

    Cnecin();
    clear_screen((UINT32 *)base);

    con_printf("\033H\n--- CLIPPING TESTS (bmap_32) ---\n");
    con_printf("Verify large 32px bitmaps trim correctly at screen limits.\n");
    fflush(stdout);

    plot_string(base, 5, 20, "Left Edge Clipping (32px):");
    plot_bitmap_32(base, 20, -16, solid_block_32, 8);
    plot_bitmap_32(base, 30, -8, checkerboard_32, 8);
    plot_bitmap_32(base, 40, 0, frame_32, 16); 

    plot_string(base, 70, 20, "Right Edge Clipping (32px):");
    plot_bitmap_32(base, 90, 624, solid_block_32, 8); 
    plot_bitmap_32(base, 100, 632, checkerboard_32, 8); 
    plot_bitmap_32(base, 110, 608, frame_32, 16); 

    plot_string(base, 140, 20, "Top Edge Clipping (32px):");
    plot_bitmap_32(base, -8, 100, solid_block_32, 8); 
    plot_bitmap_32(base, -4, 140, checkerboard_32, 8); 
    plot_bitmap_32(base, 0, 180, frame_32, 16); 

    plot_string(base, 320, 20, "Bottom Edge Clipping (32px):");
    plot_bitmap_32(base, 384, 100, frame_32, 16); 
    plot_bitmap_32(base, 392, 140, checkerboard_32, 8); 
    plot_bitmap_32(base, 376, 180, smiley_32, 16); 

    /* Bounds checking - Entirely out of bounds */
    plot_bitmap_32(base, -50, 100, smiley_32, 16); 
    plot_bitmap_32(base, 450, 100, smiley_32, 16); 
    plot_bitmap_32(base, 100, -50, smiley_32, 16); 
    plot_bitmap_32(base, 100, 650, smiley_32, 16); 
}

void test_plot_character(UINT8 *base)
{
    int i;
    clear_screen((UINT32 *)base);
    con_printf("\033H\n=== TEST 13: CHARACTERS ===\n");
    con_printf("Testing module: plt_char.o\n");
    con_printf("Drawing raw individual font glyphs to frame buffer.\n");
    fflush(stdout);

    /* Test 1: Plot individual characters */
    plot_character(base, 40, 10, 'A');
    plot_character(base, 40, 20, 'B');
    plot_character(base, 40, 30, 'C');
    plot_character(base, 40, 40, 'D');

    /* Test 2: Plot digits */
    plot_character(base, 60, 10, '0');
    plot_character(base, 60, 20, '1');
    plot_character(base, 60, 30, '2');
    plot_character(base, 60, 40, '3');
    plot_character(base, 60, 50, '4');

    /* Test 3: Plot special characters */
    plot_character(base, 80, 10, '!');
    plot_character(base, 80, 20, '@');
    plot_character(base, 80, 30, '#');
    plot_character(base, 80, 40, '$');
    plot_character(base, 80, 50, '%');

    /* Test 4: Plot all uppercase letters */
    for (i = 0; i < 26; i++)
        plot_character(base, 100, 10 + (i * 8), 'A' + i);

    /* Test 5: Plot all lowercase letters */
    for (i = 0; i < 26; i++)
        plot_character(base, 120, 10 + (i * 8), 'a' + i);

    /* Test 6: Plot all digits */
    for (i = 0; i < 10; i++)
        plot_character(base, 140, 10 + (i * 8), '0' + i);

    /* Test 7: Plot various symbols */
    plot_character(base, 160, 10, '(');
    plot_character(base, 160, 20, ')');
    plot_character(base, 160, 30, '[');
    plot_character(base, 160, 40, ']');
    plot_character(base, 160, 50, '{');
    plot_character(base, 160, 60, '}');
    plot_character(base, 160, 70, '<');
    plot_character(base, 160, 80, '>');
    plot_character(base, 160, 90, '/');
    plot_character(base, 160, 100, '\\');

    /* Test 8: Plot at different vertical positions */
    for (i = 0; i < 10; i++)
        plot_character(base, 190 + (i * 10), 10, '*');
}

void test_plot_string(UINT8 *base)
{
    clear_screen((UINT32 *)base);
    con_printf("\033H\n=== TEST 14: STRINGS ===\n");
    con_printf("Testing module: plt_str.o\n");
    con_printf("Drawing full sentences, aligning spacing and offsets.\n");
    fflush(stdout);

    /* Test 1: Simple string */
    plot_string(base, 40, 10, "Hello, World!");

    /* Test 2: Alphabet and Casing */
    plot_string(base, 60, 10, "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
    plot_string(base, 70, 10, "abcdefghijklmnopqrstuvwxyz");

    /* Test 3: Numbers and Punctuation */
    plot_string(base, 90, 10, "Numbers: 0123456789");
    plot_string(base, 100, 10, "Chars: !@#$%^&*()_+-=[]{}|;':,.<>/?");

    /* Test 4: Multi-line simulation (manual vertical offset) */
    plot_string(base, 130, 100, "Line 1: The quick brown fox");
    plot_string(base, 140, 100, "Line 2: Jumps over the lazy dog.");

    /* Test 5: Off-center and boundary checks */
    plot_string(base, 200, 320, "Centered (approx) message.");
    plot_string(base, 380, 0, "Bottom Edge Text");
    
    /* Test 6: Empty string (should do nothing/not crash) */
    plot_string(base, 250, 10, "");

    con_printf("\nString tests complete. Press any key to finish.");
    fflush(stdout);
}