#include "raster.h"
#include <osbind.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 400
#define LINE_SPACING 4

/* External references to buffer pointers defined in renderer.c */
extern UINT8 *buffer1;
extern UINT8 *buffer2;
extern UINT8 *buffer3;
extern UINT8 buffer1_raw[];
extern UINT8 buffer2_raw[];
extern UINT8 buffer3_raw[];

/*
 * draw_diagonal_lines
 *
 * PURPOSE: Draw diagonal lines from top-left to bottom-right with a given offset
 *
 * INPUT:
 *   base - pointer to the screen buffer
 *   offset - horizontal offset for the diagonal pattern (0, 1, 2, or 3 pixels)
 *
 * OUTPUT: None
 */
void draw_diagonal_lines(UINT8 *base, UINT16 offset)
{
    int start_x, start_y;

    /* Draw diagonal lines that cover the entire screen */
    /* Start from top edge, moving left to right */
    for (start_x = offset; start_x < SCREEN_WIDTH; start_x += LINE_SPACING)
    {
        UINT16 x = start_x;
        UINT16 y = 0;
        UINT16 end_x, end_y;

        /* Calculate how far this diagonal can go */
        if (x + SCREEN_HEIGHT - 1 <= SCREEN_WIDTH - 1)
        {
            /* Can reach bottom edge */
            end_x = x + SCREEN_HEIGHT - 1;
            end_y = SCREEN_HEIGHT - 1;
        }
        else
        {
            /* Will reach right edge first */
            end_x = SCREEN_WIDTH - 1;
            end_y = SCREEN_WIDTH - 1 - x;
        }

        /* Safety check - ensure coordinates are in bounds */
        if (x < SCREEN_WIDTH && y < SCREEN_HEIGHT &&
            end_x < SCREEN_WIDTH && end_y < SCREEN_HEIGHT)
        {
            plot_line((UINT32 *)base, y, x, end_y, end_x);
        }
    }

    /* Draw diagonal lines starting from left edge */
    /* Apply the same offset to maintain pattern consistency */
    for (start_y = LINE_SPACING - offset; start_y < SCREEN_HEIGHT; start_y += LINE_SPACING)
    {
        UINT16 x = 0;
        UINT16 y = start_y;
        UINT16 end_x, end_y;

        /* Calculate how far this diagonal can go */
        if (y + SCREEN_WIDTH - 1 <= SCREEN_HEIGHT - 1)
        {
            /* Can reach right edge */
            end_x = SCREEN_WIDTH - 1;
            end_y = y + SCREEN_WIDTH - 1;
        }
        else
        {
            /* Will reach bottom edge first */
            end_y = SCREEN_HEIGHT - 1;
            end_x = SCREEN_HEIGHT - 1 - y;
        }

        /* Safety check - ensure coordinates are in bounds */
        if (x < SCREEN_WIDTH && y < SCREEN_HEIGHT &&
            end_x < SCREEN_WIDTH && end_y < SCREEN_HEIGHT)
        {
            plot_line((UINT32 *)base, y, x, end_y, end_x);
        }
    }
}

/*
 * align_buffers
 *
 * PURPOSE: Align the buffer pointers to 256-byte boundaries as required by Atari ST
 *
 * INPUT: None
 *
 * OUTPUT: None
 */
void align_buffers(void)
{
    UINT32 addr;

    /* Align buffer1 to 256-byte boundary */
    addr = (UINT32)buffer1_raw;
    buffer1 = (UINT8 *)((addr + 255) & 0xFFFFFF00UL);

    /* Align buffer2 to 256-byte boundary */
    addr = (UINT32)buffer2_raw;
    buffer2 = (UINT8 *)((addr + 255) & 0xFFFFFF00UL);

    /* Align buffer3 to 256-byte boundary */
    addr = (UINT32)buffer3_raw;
    buffer3 = (UINT8 *)((addr + 255) & 0xFFFFFF00UL);
}

/*
 * init_buffers
 *
 * PURPOSE: Initialize all three screen buffers with diagonal lines at different offsets
 *          to create a scrolling effect when buffers are switched
 *
 * INPUT: None
 *
 * OUTPUT: None
 */
void init_buffers(void)
{
    /* First, align the buffers to 256-byte boundaries */
    align_buffers();

    /* Clear all buffers first */
    clear_screen((UINT32 *)buffer1);
    clear_screen((UINT32 *)buffer2);
    clear_screen((UINT32 *)buffer3);

    /* Buffer 1: Diagonal lines at offset 0 */
    draw_diagonal_lines(buffer1, 0);

    /* Buffer 2: Diagonal lines shifted left by 1 pixel (offset 3 wraps around) */
    draw_diagonal_lines(buffer2, 3);

    /* Buffer 3: Diagonal lines shifted left by 2 pixels (offset 2 wraps around) */
    draw_diagonal_lines(buffer3, 2);
}
