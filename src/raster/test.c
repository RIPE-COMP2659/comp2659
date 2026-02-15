#include "raster.h"
#include <stdio.h>
#include <osbind.h>

#define BYTES_PER_SCREEN 32000

void disable_cursor();
void fill_screen(UINT32 *base, char pattern);
void test_clear_screen(UINT8 *base);
void test_clear_region(UINT8 *base);
void test_plot_pixel(UINT8 *base);
void test_plot_horizontal_line(UINT8 *base);
void test_plot_vertical_line(UINT8 *base);

int main()
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

    return 0;
}

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
    plot_horizontal_line((UINT32 *)base, 10, 10, 50);

    /* Test 2: Line starting at byte boundary */
    plot_horizontal_line((UINT32 *)base, 20, 0, 100);

    /* Test 3: Line starting at various bit positions */
    plot_horizontal_line((UINT32 *)base, 30, 1, 40);
    plot_horizontal_line((UINT32 *)base, 31, 2, 40);
    plot_horizontal_line((UINT32 *)base, 32, 3, 40);
    plot_horizontal_line((UINT32 *)base, 33, 4, 40);
    plot_horizontal_line((UINT32 *)base, 34, 5, 40);
    plot_horizontal_line((UINT32 *)base, 35, 6, 40);
    plot_horizontal_line((UINT32 *)base, 36, 7, 40);

    /* Test 4: Long line spanning many bytes */
    plot_horizontal_line((UINT32 *)base, 50, 5, 300);

    /* Test 5: Short lines of various lengths */
    plot_horizontal_line((UINT32 *)base, 70, 10, 1);
    plot_horizontal_line((UINT32 *)base, 72, 10, 3);
    plot_horizontal_line((UINT32 *)base, 74, 10, 5);
    plot_horizontal_line((UINT32 *)base, 76, 10, 8);
    plot_horizontal_line((UINT32 *)base, 78, 10, 10);

    /* Test 6: Lines that cross multiple byte boundaries */
    plot_horizontal_line((UINT32 *)base, 100, 7, 16);  /* crosses 2-3 bytes */
    plot_horizontal_line((UINT32 *)base, 102, 15, 16); /* crosses 2-3 bytes */

    /* Test 7: Full width line */
    plot_horizontal_line((UINT32 *)base, 120, 0, 640);

    /* Test 8: Zero length line (should do nothing) */
    plot_horizontal_line((UINT32 *)base, 130, 100, 0);

    /* Test 9: Create a ladder pattern */
    plot_horizontal_line((UINT32 *)base, 150, 50, 100);
    plot_horizontal_line((UINT32 *)base, 155, 60, 100);
    plot_horizontal_line((UINT32 *)base, 160, 70, 100);
    plot_horizontal_line((UINT32 *)base, 165, 80, 100);
    plot_horizontal_line((UINT32 *)base, 170, 90, 100);
}

void test_plot_vertical_line(UINT8 *base)
{
    /* Clear screen first */
    clear_screen((UINT32 *)base);

    /* Test 1: Simple vertical line */
    plot_vertical_line((UINT32 *)base, 10, 10, 50);

    /* Test 2: Line starting at top of screen */
    plot_vertical_line((UINT32 *)base, 0, 20, 100);

    /* Test 3: Lines at different column positions (different bit positions) */
    plot_vertical_line((UINT32 *)base, 10, 1, 40);
    plot_vertical_line((UINT32 *)base, 10, 9, 40);
    plot_vertical_line((UINT32 *)base, 10, 17, 40);
    plot_vertical_line((UINT32 *)base, 10, 25, 40);
    plot_vertical_line((UINT32 *)base, 10, 33, 40);
    plot_vertical_line((UINT32 *)base, 10, 41, 40);
    plot_vertical_line((UINT32 *)base, 10, 49, 40);

    /* Test 4: Tall line spanning many rows */
    plot_vertical_line((UINT32 *)base, 5, 80, 300);

    /* Test 5: Short lines of various lengths */
    plot_vertical_line((UINT32 *)base, 10, 100, 1);
    plot_vertical_line((UINT32 *)base, 20, 100, 3);
    plot_vertical_line((UINT32 *)base, 30, 100, 5);
    plot_vertical_line((UINT32 *)base, 40, 100, 8);
    plot_vertical_line((UINT32 *)base, 50, 100, 10);

    /* Test 6: Lines at byte boundaries (columns 0, 8, 16, etc.) */
    plot_vertical_line((UINT32 *)base, 100, 0, 30);
    plot_vertical_line((UINT32 *)base, 100, 8, 30);
    plot_vertical_line((UINT32 *)base, 100, 16, 30);
    plot_vertical_line((UINT32 *)base, 100, 24, 30);

    /* Test 7: Full height line */
    plot_vertical_line((UINT32 *)base, 0, 150, 400);

    /* Test 8: Zero length line (should do nothing) */
    plot_vertical_line((UINT32 *)base, 100, 200, 0);

    /* Test 9: Create a grid pattern combining with horizontal lines */
    plot_horizontal_line((UINT32 *)base, 150, 200, 100);
    plot_horizontal_line((UINT32 *)base, 180, 200, 100);
    plot_horizontal_line((UINT32 *)base, 210, 200, 100);
    plot_vertical_line((UINT32 *)base, 150, 200, 60);
    plot_vertical_line((UINT32 *)base, 150, 230, 60);
    plot_vertical_line((UINT32 *)base, 150, 260, 60);
    plot_vertical_line((UINT32 *)base, 150, 290, 60);
}

void disable_cursor()
{
    printf("\033f");
    fflush(stdout);
}

void fill_screen(UINT32 *base, char pattern)
{
    register int i = 0;
    register UINT32 *loc = base;

    while (i++ < BYTES_PER_SCREEN / 4)
        *(loc++) = pattern;
}
