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

#define SCREEN_SIZE 32000
#define BUFFER_SIZE (SCREEN_SIZE + 512)
#define NUM_BUFFERS 2

/* Off-screen buffers - oversized for 256-byte alignment */
static UINT8 buffer_0[BUFFER_SIZE];
static UINT8 buffer_1[BUFFER_SIZE];

/* Buffer pointers - initialized at compile time to avoid NULL */
static UINT8 *buffers[NUM_BUFFERS] = {buffer_0, buffer_1};

/* Index of buffer currently being rendered to */
static int render_index = 0;

/* Index of buffer currently displayed on screen */
static int display_index = 0;

/* Flag: true when render is complete and ready for buffer swap */
static int render_complete = FALSE;

/* Flag: true after init_render_buffers() has been called */
static int buffers_initialized = FALSE;

/**
 * Initialize buffers with proper alignment.
 * Must be called once before rendering.
 */
void init_render_buffers(void) {
  /* Align buffer pointers to 256-byte boundaries */
  buffers[0] = (UINT8 *)(((UINT32)buffer_0 + 255) & 0xFFFFFF00);
  buffers[1] = (UINT8 *)(((UINT32)buffer_1 + 255) & 0xFFFFFF00);

  /* Screen shows buffer 0 */
  display_index = 0;
  /* We render to buffer 1 */
  render_index = 1;

  /* Point hardware to show buffer 0 */
  Setscreen(buffers[display_index], buffers[display_index], -1);

  /* Mark as initialized */
  buffers_initialized = TRUE;
}

/**
 * Get the buffer currently being rendered to.
 */
UINT8 *get_render_buffer(void) { return buffers[render_index]; }

/**
 * Get the buffer currently displayed on screen.
 */
UINT8 *get_display_buffer(void) { return buffers[display_index]; }

/**
 * Mark rendering as complete.
 */
void mark_render_complete(void) { render_complete = TRUE; }

/**
 * Swap buffers - call after Vsync.
 * Swaps render_index and display_index, then updates video base.
 */
void swap_buffers(void) {
  int temp;

  if (!render_complete) {
    return;
  }

  /* Swap indices */
  temp = render_index;
  render_index = display_index;
  display_index = temp;

  /* Update video base to point to new display buffer */
  Setscreen(buffers[display_index], buffers[display_index], -1);

  /* Reset flag */
  render_complete = FALSE;
}

/* See render.h for documentation */
void render(const Model *model, UINT8 *base) {
  unsigned int i;
  const Camera *cam = &model->world.camera;
  int level_i = model->world.level_index;
  Level level = model->world.levels[level_i];
  UINT8 *render_buf;

  (void)base;

  /* For now, always render directly to screen using Physbase */
  render_buf = (UINT8 *)Physbase();

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

  /* Mark render complete - swap happens after Vsync */
  if (buffers_initialized) {
    mark_render_complete();
  }
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
