#include "raster.h"
#include <stdio.h>
#include <osbind.h>

#define BYTES_PER_SCREEN 32000

void disable_cursor();
void fill_screen(UINT32 *base, char pattern);
void test_clear_screen(UINT8 *base);
void test_clear_region(UINT8 *base);
void test_plot_pixel(UINT8 *base);

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
