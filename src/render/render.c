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

/* Screen buffer size - 32000 bytes for 640x400 at 8-bit */
/* Extra 256 bytes for alignment */
#define BUFFER_SIZE 32000
#define NUM_BUFFERS 2
#define ALIGN_MASK 0xFF

/* Two off-screen buffers for double buffering (oversized for alignment) */
static UINT8 buffer_0[BUFFER_SIZE];
static UINT8 buffer_1[BUFFER_SIZE];

/* Aligned buffer pointers - initialized in init_buffers() */
static UINT8 *buffers[NUM_BUFFERS];

/* Index of buffer currently being rendered to */
static int render_index = 1;

/* Index of buffer currently displayed on screen */
static int display_index = 0;

/* Flag: true when render is complete and ready for buffer swap */
static int render_complete = FALSE;

/**
 * Initialize buffers with proper alignment.
 * Must be called once before rendering.
 */
void init_render_buffers(void) {
  /* Align buffer pointers to 256-byte boundaries */
  buffers[0] = (UINT8 *)(((UINT32)buffer_0 + ALIGN_MASK) & ~ALIGN_MASK);
  buffers[1] = (UINT8 *)(((UINT32)buffer_1 + ALIGN_MASK) & ~ALIGN_MASK);

  /* Point hardware to our first buffer so we see something on screen */
  Setscreen(buffers[0], buffers[0], -1);
}

/**
 * Get the buffer currently being rendered to.
 * Call this in render() to get the target buffer.
 */
UINT8 *get_render_buffer(void) { return buffers[render_index]; }

/**
 * Get the buffer currently displayed on screen.
 * Call this in VBL handler to get the buffer to flip to.
 */
UINT8 *get_display_buffer(void) { return buffers[display_index]; }

/**
 * Mark rendering as complete. Call after render() finishes.
 * The actual buffer swap should happen in VBL interrupt.
 */
void mark_render_complete(void) { render_complete = TRUE; }

/**
 * Swap buffers - call this in VBL interrupt handler.
 * Swaps render_index and display_index, then updates video base.
 */
void swap_buffers(void) {
  int temp_index;

  if (!render_complete) {
    return;
  }

  /* Swap render and display indices */
  temp_index = render_index;
  render_index = display_index;
  display_index = temp_index;

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
  UINT8 *render_buf = get_render_buffer();

  (void)base; /* base is ignored - we render to our own buffer */

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

  /* Mark render complete - swap happens in VBL */
  mark_render_complete();
}

/* See render.h for documentation */
void render_ground(const Model *model, UINT8 *base) {
  int rel_y =
      camera_get_relative_y(&model->world.camera, model->world.ground_y);
  /* Draw a 4-pixel tall rectangle across the screen width (640 px) */
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
