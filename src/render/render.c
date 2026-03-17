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

/* Two off-screen buffers - oversized for alignment */
static UINT8 buffer_0[BUFFER_SIZE];
static UINT8 buffer_1[BUFFER_SIZE];

/* Aligned buffer pointers - set at runtime */
static UINT8 *buffers[NUM_BUFFERS];

/* Current buffer index (the one we're rendering to) */
static int current_buffer = 0;

/**
 * Initialize buffers and set up double buffering.
 * Must be called once at program start.
 */
void init_render_buffers(void) {
  UINT32 addr;

  /* Align buffer 0 to 256-byte boundary */
  addr = (UINT32)buffer_0;
  buffers[0] = (UINT8 *)((addr + 255) & 0xFFFFFF00UL);

  /* Align buffer 1 to 256-byte boundary */
  addr = (UINT32)buffer_1;
  buffers[1] = (UINT8 *)((addr + 255) & 0xFFFFFF00UL);

  /* Clear both buffers */
  clear_screen((UINT32 *)buffers[0]);
  clear_screen((UINT32 *)buffers[1]);

  /* Set screen to show first buffer */
  Setscreen(buffers[0], buffers[0], -1);

  /* Start rendering to the other buffer */
  current_buffer = 1;
}

/**
 * Get pointer to the buffer currently being rendered to.
 */
UINT8 *get_render_buffer(void) { return buffers[current_buffer]; }

/**
 * Get pointer to the buffer currently displayed on screen.
 */
UINT8 *get_display_buffer(void) { return buffers[1 - current_buffer]; }

/**
 * Mark rendering as complete and ready for display.
 */
void mark_render_complete(void) {
  /* Nothing to do - swap happens in swap_buffers */
}

/**
 * Swap buffers - call this AFTER Vsync().
 * Switches the display to the newly rendered buffer.
 */
void swap_buffers(void) {
  /* Switch to the other buffer (the one we just rendered to) */
  current_buffer = 1 - current_buffer;

  /* Update hardware to show the new buffer */
  Setscreen(buffers[current_buffer], buffers[current_buffer], -1);
}

/* See render.h for documentation */
void render(const Model *model, UINT8 *base) {
  unsigned int i;
  const Camera *cam = &model->world.camera;
  int level_i = model->world.level_index;
  Level level = model->world.levels[level_i];
  UINT8 *render_buf;

  (void)base;

  /* Get the back buffer to render to */
  render_buf = buffers[current_buffer];

  clear_screen((UINT32 *)render_buf);

  render_ground(model, render_buf);

  for (i = model->cam_min_bi; i <= model->cam_max_bi; i++) {
    render_block(&level.blocks[i], cam, render_buf);
  }

  for (i = model->cam_min_si; i <= model->cam_max_si; i++) {
    render_spike(&level.spikes[i], cam, render_buf);
  }

  for (i = model->cam_min_li; i <= model->cam_max_li; i++) {
    render_lava(&level.lava[i], cam, render_buf);
  }

  render_geo(&model->world.geo, cam, render_buf);
}

/* See render.h for documentation */
void render_ground(const Model *model, UINT8 *base) {
  int rel_y =
      camera_get_relative_y(&model->world.camera, model->world.ground_y);
  plot_rectangle((UINT32 *)base, rel_y, 0, 4, 640);
}

/* See render.h for documentation */
void render_block(const Block *block, const Camera *camera, UINT8 *base) {
  int rel_x = camera_get_relative_x(camera, block->x);
  int rel_y = camera_get_relative_y(camera, block->y);
  plot_bitmap_32(base, rel_y, rel_x, block->sprite, block->size);
}

/* See render.h for documentation */
void render_spike(const Spike *spike, const Camera *camera, UINT8 *base) {
  int rel_x = camera_get_relative_x(camera, spike->x);
  int rel_y = camera_get_relative_y(camera, spike->y);
  plot_bitmap_32(base, rel_y, rel_x, spike->sprite, spike->size);
}

/* See render.h for documentation */
void render_lava(const Lava *lava, const Camera *camera, UINT8 *base) {
  int rel_x = camera_get_relative_x(camera, lava->x);
  int rel_y = camera_get_relative_y(camera, lava->y);
  plot_bitmap_32(base, rel_y, rel_x, lava->sprite, lava->size);
}

/* See render.h for documentation */
void render_geo(const Geo *geo, const Camera *camera, UINT8 *base) {
  int rel_x = camera_get_relative_x(camera, geo->x);
  int rel_y = camera_get_relative_y(camera, geo->y);
  plot_bitmap_32(base, rel_y, rel_x, geo->sprite, geo->size);
}
