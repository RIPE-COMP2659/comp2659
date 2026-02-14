#include "raster.h"
#include <stdio.h>
#include <osbind.h>

#define BYTES_PER_SCREEN 32000

void disable_cursor();
void fill_screen(UINT32 *base, char pattern);

int main()
{
    UINT32 *base = (UINT32 *)Physbase();

    /* disable_cursor(); hide blinking text cursor */

    /* Fill screen with all black to start
    fill_screen(base, -1);
    Cnecin();  wait for key press */

    /* Test 1: Small region - 4 bytes wide (1 long register), 10 rows */
    /* Clears from row 10, col 5 bytes, height 10 rows, width 4 bytes */
    clear_region(base, 0, 0, 48, 48);
    Cnecin();
    return 0;
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
