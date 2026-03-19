/**
 * render.c
 * Authors:
 *     Riley Gramlich, rgram060@mtroyal.ca, 201762060
 *     Robert Parker Hutcheson, rhutc335@mtroyal.ca, 201762335
 *     Isaac Klein, iklei977@mtroyal.ca, 201763977
 *     Eduard Mykhailets, emykh268@mtroyal.ca, 201750268
 * Course: COMP 2659-001, Computing Machinery II, Winter 2026
 * Instructor: Nolan Shaw
 *
 * See render.h for documentation.
 */

#include "render.h"
#include <osbind.h>

/* Screen size: 640x400 at 8-bit = 32000 bytes */
/* Extra space for 256-byte alignment */
#define SCREEN_SIZE 32000
#define BUFFER_SIZE (SCREEN_SIZE + 256)
#define NUM_BUFFERS 2

/* Buffer alignment constants */
#define ALIGNMENT_MASK 0xFFFFFF00 /* Mask to zero lower 8 bits */
#define ALIGNMENT_PADDING 255     /* Round up to next 256-byte boundary */

/**
 * Align address to 256-byte boundary.
 * Rounds up to ensure proper alignment.
 */
/* TODO: Check out this character */
#define ALIGN_TO_256(addr) \
    ((UINT8 *)(((UINT32)(addr) + ALIGNMENT_PADDING) & ALIGNMENT_MASK))

/* Two off-screen buffers - oversized for alignment */
static UINT8 buffer_0[BUFFER_SIZE];
static UINT8 buffer_1[BUFFER_SIZE];

/* Aligned buffer pointers - set at runtime */
static UINT8 *buffers[NUM_BUFFERS];

/* Current buffer indices */
static int stale_buffer = 1;
static int display_index = 0;

/**
 * Initialize buffers and set up double buffering.
 * Must be called once at program start.
 */
void init_render_buffers(void)
{
    UINT32 addr;

    /* Align buffers to 256-byte boundary */
    buffers[0] = ALIGN_TO_256(buffer_0);
    buffers[1] = ALIGN_TO_256(buffer_1);

    /* Clear both buffers */
    clear_screen((UINT32 *)buffers[0]);
    clear_screen((UINT32 *)buffers[1]);

    /* Set screen to show first buffer, start rendering to second */
    Setscreen(buffers[0], buffers[0], -1);
    stale_buffer = 1;
    display_index = 0;
}

/**
 * Get pointer to the buffer currently being rendered to.
 */
UINT8 *get_render_buffer(void) { return buffers[stale_buffer]; }

/**
 * Get pointer to the buffer currently displayed on screen.
 */
UINT8 *get_display_buffer(void) { return buffers[display_index]; }

/**
 * Mark rendering as complete and ready for display.
 */
void mark_render_complete(void)
{
    /* Nothing to do - swap happens in swap_buffers */
}

/**
 * Swap buffers - call this AFTER Vsync().
 * Switches the display to the newly rendered buffer.
 */
void swap_buffers(void)
{
    /* Swap render and display indices */
    stale_buffer = display_index;

    /* only works for 2 buffers right now- flips between 0 and 1 (XOR) */
    display_index = display_index ^ 1;

    /* Show the newly rendered buffer */
    Setscreen(buffers[display_index], buffers[display_index], -1);
}
/**
 * floor_div8 and clear_sprite_region should not exist. Clearing is bugged
 * and to save time, we're just going to clear a slightly larger region.
 */
/* Floor division by 8 using shifts, including negative values */
static int floor_div8(int x)
{
    signed int result;

    if (x >= 0)
    {
        result = x >> 3;
    }
    else
    {
        result = -(((-x) + 7) >> 3);
    }

    return result;
}
/* Clear the the bytes of the surrounding sprite region */
static void clear_sprite_region(UINT8 *base, int rel_y, int rel_x, unsigned int size)
{
    int clear_x;
    int clear_right;
    int clipped_left;
    int clipped_right;
    int clear_width;

    clear_x = floor_div8(rel_x) << 3;
    clear_right = (floor_div8(rel_x + (int)size - 1) << 3) + 7;

    /* Keep the clippings in bounds */
    clipped_left = (clear_x < 0) ? 0 : clear_x;
    clipped_right = (clear_right >= SCREEN_WIDTH) ? (SCREEN_WIDTH - 1) : clear_right;

    /* Keep horizontal clear bounds on-screen to avoid invalid writes */
    /* TODO: There's bad logic in here, this shouldn't need to happen, but it can crash */
    if (clipped_right >= clipped_left)
    {
        clear_width = clipped_right - clipped_left + 1;
        clear_region((UINT32 *)base, rel_y, clipped_left, size, clear_width);
    }
}

/* See render.h for documentation */
void render(const Model *model, UINT8 *base)
{
    unsigned int i;
    const Camera *cam = &model->world.camera;
    const Camera *old_cam = &model->old_cam;
    int level_i = model->world.level_index;
    Level level = model->world.levels[level_i];
    int input;
    UINT8 *render_buf;

    /* TODO: Refactor function contract to remove base */
    (void)base;

    /*    clear_ground(base, old_cam, model->world.ground_y);
        clear_blocks(base, old_cam, level.blocks, model->cam_min_bi, model->cam_max_bi);
        clear_spikes(base, old_cam, level.spikes, model->cam_min_si, model->cam_max_si);
        clear_lava(base, old_cam, level.lava, model->cam_min_li, model->cam_max_li);
        clear_geo(base, old_cam, &model->old_geo);  Needed because geo's position isn't static */

    /* Get the back buffer to render to */
    render_buf = buffers[stale_buffer];

    clear_screen((UINT32 *)render_buf); /* TODO: Get rid of */

    render_ground(model, render_buf);

    for (i = model->cam_min_bi; i <= model->cam_max_bi; i++)
    {
        render_block(&level.blocks[i], cam, render_buf);
    }

    for (i = model->cam_min_si; i <= model->cam_max_si; i++)
    {
        render_spike(&level.spikes[i], cam, render_buf);
    }

    for (i = model->cam_min_li; i <= model->cam_max_li; i++)
    {
        render_lava(&level.lava[i], cam, render_buf);
    }

    render_geo(&model->world.geo, cam, render_buf);
}

void clear_geo(UINT8 *base, const Camera *camera, const Geo *geo)
{
    int rel_x = camera_get_relative_x(camera, geo->x);
    int rel_y = camera_get_relative_y(camera, geo->y);
    clear_sprite_region(base, rel_y, rel_x, geo->size);
}

void clear_lava(UINT8 *base, const Camera *camera, const Lava *lava, unsigned int min_li, unsigned int max_li)
{
    unsigned int i;
    for (i = min_li; i <= max_li; i++)
    {
        int rel_x = camera_get_relative_x(camera, lava[i].x);
        int rel_y = camera_get_relative_y(camera, lava[i].y);
        clear_sprite_region(base, rel_y, rel_x, lava[i].size);
    }
}

void clear_spikes(UINT8 *base, const Camera *camera, const Spike *spikes, unsigned int min_si, unsigned int max_si)
{
    unsigned int i;
    for (i = min_si; i <= max_si; i++)
    {
        int rel_x = camera_get_relative_x(camera, spikes[i].x);
        int rel_y = camera_get_relative_y(camera, spikes[i].y);
        clear_sprite_region(base, rel_y, rel_x, spikes[i].size);
    }
}

void clear_blocks(UINT8 *base, const Camera *camera, const Block *blocks, unsigned int min_bi, unsigned int max_bi)
{
    unsigned int i;
    for (i = min_bi; i <= max_bi; i++)
    {
        int rel_x = camera_get_relative_x(camera, blocks[i].x);
        int rel_y = camera_get_relative_y(camera, blocks[i].y);
        clear_sprite_region(base, rel_y, rel_x, blocks[i].size);
    }
}

void clear_ground(UINT8 *base, const Camera *camera, unsigned int ground_y)
{
    int rel_y = camera_get_relative_y(camera, ground_y);
    /* Clear a 4-pixel tall rectangle across the screen width (640 px) */
    clear_region((UINT32 *)base, rel_y, 0, 4, 640);
}

/* See render.h for documentation */
void render_ground(const Model *model, UINT8 *base)
{
    int rel_y =
        camera_get_relative_y(&model->world.camera, model->world.ground_y);
    plot_rectangle((UINT32 *)base, rel_y, 0, 4, 640);
}

/* See render.h for documentation */
void render_block(const Block *block, const Camera *camera, UINT8 *base)
{
    int rel_x = camera_get_relative_x(camera, block->x);
    int rel_y = camera_get_relative_y(camera, block->y);
    plot_bitmap_32(base, rel_y, rel_x, block->sprite, block->size);
}

/* See render.h for documentation */
void render_spike(const Spike *spike, const Camera *camera, UINT8 *base)
{
    int rel_x = camera_get_relative_x(camera, spike->x);
    int rel_y = camera_get_relative_y(camera, spike->y);
    plot_bitmap_32(base, rel_y, rel_x, spike->sprite, spike->size);
}

/* See render.h for documentation */
void render_lava(const Lava *lava, const Camera *camera, UINT8 *base)
{
    int rel_x = camera_get_relative_x(camera, lava->x);
    int rel_y = camera_get_relative_y(camera, lava->y);
    plot_bitmap_32(base, rel_y, rel_x, lava->sprite, lava->size);
}

/* See render.h for documentation */
void render_geo(const Geo *geo, const Camera *camera, UINT8 *base)
{
    int rel_x = camera_get_relative_x(camera, geo->x);
    int rel_y = camera_get_relative_y(camera, geo->y);
    plot_bitmap_32(base, rel_y, rel_x, geo->sprite, geo->size);
}
