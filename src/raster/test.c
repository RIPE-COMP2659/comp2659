#include "raster.h"
#include <stdio.h>
#include <osbind.h>

#define BYTES_PER_SCREEN 32000

void disable_cursor();
void fill_screen(UINT32 *base, char pattern);

int main() {
    UINT32 *base = (UINT32 *)Physbase();
    
    disable_cursor(); /* hide blinking text cursor */
    
    /* Fill screen with all black to start */
    fill_screen(base, -1);
    Cnecin(); /* wait for key press */
    
    /* Test 1: Small region - 4 bytes wide (1 long register), 10 rows */
    /* Clears from row 10, col 5 bytes, height 10 rows, width 4 bytes */
    clear_region(base, 10, 5, 10, 4);
    Cnecin();
    
    /* Refill screen */
    fill_screen(base, -1);
    
    /* Test 2: Medium region - 20 bytes wide (5 long registers), 15 rows */
    /* Clears from row 50, col 10 bytes, height 15 rows, width 20 bytes */
    clear_region(base, 50, 10, 15, 20);
    Cnecin();
    
    /* Refill screen */
    fill_screen(base, -1);
    
    /* Test 3: Larger region - 52 bytes wide (13 long registers), 20 rows */
    /* Clears from row 100, col 0 bytes, height 20 rows, width 52 bytes */
    clear_region(base, 100, 0, 20, 52);
    Cnecin();
    
    /* Refill screen */
    fill_screen(base, -1);
    
    /* Test 4: Very large region - 100 bytes wide (requires multiple movem), 30 rows */
    /* Clears from row 200, col 0 bytes, height 30 rows, width 100 bytes */
    clear_region(base, 200, 0, 30, 100);
    Cnecin();
    
    /* Refill screen */
    fill_screen(base, -1);
    
    /* Test 5: Region with odd width - 13 bytes (3 longs + 1 byte), 5 rows */
    /* Tests partial byte handling */
    clear_region(base, 300, 10, 5, 13);
    Cnecin();
    
    /* Refill screen */
    fill_screen(base, -1);
    
    /* Test 6: Region with word alignment - 18 bytes (4 longs + 1 word), 8 rows */
    /* Tests word remainder handling */
    clear_region(base, 350, 20, 8, 18);
    Cnecin();
    
    /* Refill screen */
    fill_screen(base, -1);
    
    /* Test 7: Tiny region - 3 bytes (no full longs), 5 rows */
    /* Tests no_movem_needed path */
    clear_region(base, 380, 30, 5, 3);
    Cnecin();
    
    /* Clear screen at the end */
    fill_screen(base, 0);
    
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
