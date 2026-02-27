#include "raster.h"
#include <osbind.h>
#include <stdio.h>

#define BYTES_PER_SCREEN 32000
#define BUFFER_SIZE (BYTES_PER_SCREEN + 256) /* Extra space for alignment */
#define ANIMATION_CYCLES 100

/* Four screen buffers for animation - oversized to allow for alignment */
UINT8 buffer1_raw[BUFFER_SIZE];
UINT8 buffer2_raw[BUFFER_SIZE];
UINT8 buffer3_raw[BUFFER_SIZE];
UINT8 buffer4_raw[BUFFER_SIZE];

/* Aligned buffer pointers */
UINT8 *buffer1;
UINT8 *buffer2;
UINT8 *buffer3;
UINT8 *buffer4;

/*
 * main
 *
 *      EXPERIMENTAL: Uses 4 fram buffers to create scrolling background/platfom with no additional compute
 *       aside from the cost of a more complex `clear_region()` function- the costs of which are yet to be determined.
 *                                                 ^^ @sudonym-i will have to look into this.
 *
 *      Estimated memory cost: 4 * 32000 bytes = 128KB (a bit more, for 256-byte alignment padding)
 *
 */
int main(void)
{
    UINT8 *original_screen;
    int i;
    char key;

    disable_cursor();

    /* Save original screen base */
    original_screen = (UINT8 *)Physbase();

    /* Initialize all three buffers with background patterns - done ONCE at startup */
    printf("Initializing buffers...\n");
    init_buffers();
    printf("Buffers initialized.\n\n");

    /* Display instructions */
    printf("Scrolling Background Demo\n");
    printf("4 screen buffers pre-rendered.\n");
    printf("Press any key to start.\n");
    Cnecin();

    /* Cycle through buffers to create scrolling effect */
    /* We switch the display to each buffer - no copying needed! */
    for (i = 0; i < ANIMATION_CYCLES; i++)
    {
        /* Switch display to buffer 1 */
        Setscreen(buffer1, buffer1, -1);
        Vsync();

        /* Switch display to buffer 2 */
        Setscreen(buffer2, buffer2, -1);
        Vsync();

        /* Switch display to buffer 3 */
        Setscreen(buffer3, buffer3, -1);
        Vsync();

        /* Switch display to buffer 4 */
        Setscreen(buffer4, buffer4, -1);
        Vsync();
    }

    /* Restore original screen and clear */
    Setscreen(original_screen, original_screen, -1);
    clear_screen((UINT32 *)original_screen);

    return 0;
}

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
 * get_buffer4
 *
 * PURPOSE: Get a pointer to buffer 4
 *
 * INPUT: None
 *
 * OUTPUT: Pointer to buffer 4
 */
UINT8 *get_buffer4(void)
{
    return buffer4;
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
