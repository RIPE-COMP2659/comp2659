#include "raster.h"
#include <osbind.h>
#include <stdio.h>

#define BYTES_PER_SCREEN 32000
#define BUFFER_SIZE (BYTES_PER_SCREEN + 256) /* Extra space for alignment */

/* Three screen buffers for animation - oversized to allow for alignment */
UINT8 buffer1_raw[BUFFER_SIZE];
UINT8 buffer2_raw[BUFFER_SIZE];
UINT8 buffer3_raw[BUFFER_SIZE];

/* Aligned buffer pointers */
UINT8 *buffer1;
UINT8 *buffer2;
UINT8 *buffer3;

/*
 * get_buffer1
 *
 * PURPOSE: Get a pointer to buffer 1
 *
 * INPUT: None
 *
 * OUTPUT: Pointer to buffer 1
 */
UINT8 *get_buffer1(void)
{
    return buffer1;
}

/*
 * get_buffer2
 *
 * PURPOSE: Get a pointer to buffer 2
 *
 * INPUT: None
 *
 * OUTPUT: Pointer to buffer 2
 */
UINT8 *get_buffer2(void)
{
    return buffer2;
}

/*
 * get_buffer3
 *
 * PURPOSE: Get a pointer to buffer 3
 *
 * INPUT: None
 *
 * OUTPUT: Pointer to buffer 3
 */
UINT8 *get_buffer3(void)
{
    return buffer3;
}

/*
 * disable_cursor
 *
 * PURPOSE: Disable the cursor on the screen
 *
 * INPUT: None
 *
 * OUTPUT: None
 */
void disable_cursor(void)
{
    printf("\033f");
    fflush(stdout);
}

/*
 * wait_vblanks
 *
 * PURPOSE: Wait for a specified number of vertical blank intervals
 *
 * INPUT:
 *   count - number of vertical blanks to wait for
 *
 * OUTPUT: None
 */
void wait_vblanks(int count)
{
    int i;
    for (i = 0; i < count; i++)
    {
        Vsync();
    }
}

/*
 * main
 *
 * PURPOSE: Demonstrate the scrolling background effect by cycling through
 *          the three pre-rendered buffers using Setscreen() to switch between them
 *
 * INPUT: None
 *
 * OUTPUT: Returns 0 on successful completion
 */
int main(void)
{
    UINT8 *original_screen;
    int i;
    char key;

    disable_cursor();

    /* Save original screen base */
    original_screen = (UINT8 *)Physbase();

    /* Initialize all three buffers with vertical lines - done ONCE at startup */
    printf("Initializing buffers...\n");
    init_buffers();
    printf("Buffers initialized.\n\n");

    /* Display instructions */
    printf("Scrolling Background Demo\n");
    printf("3 screen buffers pre-rendered with shifted vertical lines.\n");
    printf("Press any key to cycle through buffers...\n");
    printf("Press 'q' to quit\n\n");
    Cnecin();

    /* Cycle through buffers to create scrolling effect */
    /* We switch the display to each buffer - no copying needed! */
    for (i = 0; i < 100; i++) /* Show 20 cycles */
    {
        /* Switch display to buffer 1 */
        Setscreen(buffer1, buffer1, -1);
        wait_vblanks(2);
        /* Switch display to buffer 2 */
        Setscreen(buffer2, buffer2, -1);
        wait_vblanks(2);
        /* Switch display to buffer 3 */
        Setscreen(buffer3, buffer3, -1);
        wait_vblanks(2);
    }

    /* Restore original screen and clear */
    Setscreen(original_screen, original_screen, -1);
    clear_screen((UINT32 *)original_screen);

    return 0;
}
