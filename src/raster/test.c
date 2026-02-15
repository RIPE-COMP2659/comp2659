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

int main()
{
    UINT8 *base = (UINT8 *)Physbase();

    disable_cursor();

    /* Test 1: Clear Screen */
    printf("Test 1: Fill screen then clear it\n");
    fill_screen((UINT32 *)base, -1); /* fill with white */
    Cnecin();
    test_clear_screen(base);
    Cnecin();

    /* Test 2: Clear Region - various sizes and positions */
    printf("\nTest 2: Clear region tests\n");
    test_clear_region(base);
    Cnecin();

    /* Test 3: Plot Pixel - draw patterns */
    printf("\nTest 3: Plot pixel tests\n");
    test_plot_pixel(base);
    Cnecin();

    /* Test 4: Plot Horizontal Line */
    printf("\nTest 4: Plot horizontal line tests\n");
    test_plot_horizontal_line(base);
    Cnecin();

    printf("\nAll tests complete!\n");
    return 0;
}

void test_clear_screen(UINT8 *base)
{
    printf("Clearing entire screen...\n");
    clear_screen((UINT32 *)base);
    printf("Screen cleared.\n");
}

void test_clear_region(UINT8 *base)
{
    int i;

    /* Fill screen with white first */
    fill_screen((UINT32 *)base, -1);

    /* Test 1: 48x48 optimized path - word aligned */
    printf("  - 48x48 region at (0,0) - optimized path\n");
    clear_region((UINT32 *)base, 0, 0, 48, 48);

    /* Test 2: 48x48 optimized path - different position */
    printf("  - 48x48 region at (50,80) - optimized path\n");
    clear_region((UINT32 *)base, 50, 80, 48, 48);

    /* Test 3: Small region - unoptimized path */
    printf("  - 10x10 region at (10,5) - unoptimized path\n");
    clear_region((UINT32 *)base, 10, 5, 10, 10);

    /* Test 4: Wide region - unoptimized path */
    printf("  - 20x100 region at (100,50) - unoptimized path\n");
    clear_region((UINT32 *)base, 100, 50, 20, 100);

    /* Test 5: Tall narrow region */
    printf("  - 50x8 region at (150,200) - unoptimized path\n");
    clear_region((UINT32 *)base, 150, 200, 50, 8);

    /* Test 6: Odd column alignment to test byte spanning */
    printf("  - 15x4 region at (200,5) - tests byte spanning\n");
    clear_region((UINT32 *)base, 200, 5, 15, 4);

    printf("All clear_region tests complete.\n");
}

void test_plot_pixel(UINT8 *base)
{
    int i;

    /* Clear screen first */
    clear_screen((UINT32 *)base);

    /* Test 1: Draw a horizontal line using plot_pixel */
    printf("  - Drawing horizontal line at row 10\n");
    for (i = 0; i < 100; i++)
    {
        plot_pixel(base, 10, i);
    }

    /* Test 2: Draw a vertical line */
    printf("  - Drawing vertical line at col 50\n");
    for (i = 20; i < 120; i++)
    {
        plot_pixel(base, i, 50);
    }

    /* Test 3: Draw a diagonal line */
    printf("  - Drawing diagonal line\n");
    for (i = 0; i < 50; i++)
    {
        plot_pixel(base, 30 + i, 100 + i);
    }

    /* Test 4: Draw a box outline using plot_pixel */
    printf("  - Drawing box outline (200,200) to (250,280)\n");
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
    printf("  - Testing pixels at byte boundaries\n");
    for (i = 0; i < 64; i += 8)
    {
        plot_pixel(base, 300, i);     /* bit 7 (MSB) */
        plot_pixel(base, 301, i + 4); /* bit 3 (middle) */
        plot_pixel(base, 302, i + 7); /* bit 0 (LSB) */
    }

    printf("All plot_pixel tests complete.\n");
}

void test_plot_horizontal_line(UINT8 *base)
{
    /* Clear screen first */
    clear_screen((UINT32 *)base);

    /* Test 1: Simple horizontal line */
    printf("  - Drawing horizontal line at (10,10) length 50\n");
    plot_horizontal_line((UINT32 *)base, 10, 10, 50);

    /* Test 2: Line starting at byte boundary */
    printf("  - Line at (20,0) length 100 - starts at byte boundary\n");
    plot_horizontal_line((UINT32 *)base, 20, 0, 100);

    /* Test 3: Line starting at various bit positions */
    printf("  - Lines at different bit positions (30-37, col 1-7)\n");
    plot_horizontal_line((UINT32 *)base, 30, 1, 40);
    plot_horizontal_line((UINT32 *)base, 31, 2, 40);
    plot_horizontal_line((UINT32 *)base, 32, 3, 40);
    plot_horizontal_line((UINT32 *)base, 33, 4, 40);
    plot_horizontal_line((UINT32 *)base, 34, 5, 40);
    plot_horizontal_line((UINT32 *)base, 35, 6, 40);
    plot_horizontal_line((UINT32 *)base, 36, 7, 40);

    /* Test 4: Long line spanning many bytes */
    printf("  - Long line at (50,5) length 300\n");
    plot_horizontal_line((UINT32 *)base, 50, 5, 300);

    /* Test 5: Short lines of various lengths */
    printf("  - Short lines (1-10 pixels)\n");
    plot_horizontal_line((UINT32 *)base, 70, 10, 1);
    plot_horizontal_line((UINT32 *)base, 72, 10, 3);
    plot_horizontal_line((UINT32 *)base, 74, 10, 5);
    plot_horizontal_line((UINT32 *)base, 76, 10, 8);
    plot_horizontal_line((UINT32 *)base, 78, 10, 10);

    /* Test 6: Lines that cross multiple byte boundaries */
    printf("  - Lines crossing byte boundaries\n");
    plot_horizontal_line((UINT32 *)base, 100, 7, 16);  /* crosses 2-3 bytes */
    plot_horizontal_line((UINT32 *)base, 102, 15, 16); /* crosses 2-3 bytes */

    /* Test 7: Full width line */
    printf("  - Full width line at (120,0) length 640\n");
    plot_horizontal_line((UINT32 *)base, 120, 0, 640);

    /* Test 8: Zero length line (should do nothing) */
    printf("  - Zero length line at (130,100) - should do nothing\n");
    plot_horizontal_line((UINT32 *)base, 130, 100, 0);

    /* Test 9: Create a ladder pattern */
    printf("  - Ladder pattern\n");
    plot_horizontal_line((UINT32 *)base, 150, 50, 100);
    plot_horizontal_line((UINT32 *)base, 155, 60, 100);
    plot_horizontal_line((UINT32 *)base, 160, 70, 100);
    plot_horizontal_line((UINT32 *)base, 165, 80, 100);
    plot_horizontal_line((UINT32 *)base, 170, 90, 100);

    printf("All plot_horizontal_line tests complete.\n");
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
