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

/* See render.h for documentation */
void render(const Model *model, UINT8 *base) {
    unsigned int i;
    const Camera *cam = &model->world.camera;
    int level_i = model->world.level_index;
    Level level = model->world.levels[level_i];

    clear_screen((UINT32 *)base);

    render_ground(model, base);

    for (i = model->cam_min_bi; i <= model->cam_max_bi; i++) {
        render_block(&level.blocks[i], cam, base);
    }

    for (i = model->cam_min_si; i <= model->cam_max_si; i++) {
        render_spike(&level.spikes[i], cam, base);
    }

    for (i = model->cam_min_li; i <= model->cam_max_li; i++) {
        render_lava(&level.lava[i], cam, base);
    }

    render_geo(&model->world.geo, cam, base);
}

/* See render.h for documentation */
void render_ground(const Model *model, UINT8 *base) {
    int rel_y = camera_get_relative_y(&model->world.camera, model->world.ground_y);
    const Camera *cam = &model->world.camera;
    const Level *lvl = &model->world.levels[model->world.level_index];
    int first_x, last_x;

    /* VERTICAL CULL: If ground is off-screen, don't draw it */
    if (rel_y < -2 || rel_y >= 400) return;

    /* POOL LOGIC: If no lava is visible, draw the full line and return. */
    if (lvl->lava_size == 0 || model->cam_min_li > model->cam_max_li || model->cam_min_li >= lvl->lava_size) {
        plot_rectangle((UINT32 *)base, rel_y, 0, 2, 640);
        return;
    }

    /* Find where the lava pool starts and ends on screen */
    first_x = camera_get_relative_x(cam, lvl->lava[model->cam_min_li].x);
    last_x = camera_get_relative_x(cam, lvl->lava[model->cam_max_li].x) + 32;

    /* Draw segment BEFORE the lava pool */
    if (first_x > 0) {
        plot_rectangle((UINT32 *)base, rel_y, 0, 2, (first_x > 640 ? 640 : first_x));
    }

    /* Draw segment AFTER the lava pool */
    if (last_x < 640) {
        plot_rectangle((UINT32 *)base, rel_y, (last_x < 0 ? 0 : last_x), 2, (last_x < 0 ? 640 : 640 - last_x));
    }
}

/* See render.h for documentation */
void render_block(const Block *block, const Camera *camera, UINT8 *base) {
    int rel_y = camera_get_relative_y(camera, block->y);
    int rel_x;

    /* VERTICAL CULL: Don't call plotting if entity is vertically off-screen */
    if (rel_y < -32 || rel_y >= 400) return;

    rel_x = camera_get_relative_x(camera, block->x);
    plot_bitmap_32(base, rel_y, rel_x, block->sprite, block->size);
}

/* See render.h for documentation */
void render_spike(const Spike *spike, const Camera *camera, UINT8 *base) {
    int rel_y = camera_get_relative_y(camera, spike->y);
    int rel_x;

    if (rel_y < -32 || rel_y >= 400) return;

    rel_x = camera_get_relative_x(camera, spike->x);
    plot_bitmap_32(base, rel_y, rel_x, spike->sprite, spike->size);
}

/* See render.h for documentation */
void render_lava(const Lava *lava, const Camera *camera, UINT8 *base) {
    int rel_y = camera_get_relative_y(camera, lava->y);
    int rel_x;
    
    if (rel_y < -32 || rel_y >= 400) return;

    rel_x = camera_get_relative_x(camera, lava->x);
    
    /* OPTIMIZATION: Only render the top 4 pixels (the hazard surface). 
       The bottom 28 pixels match the background, so we skip them entirely. 
       This makes the heavy lava sections performant on the 68000. */
    plot_bitmap_32(base, rel_y, rel_x, lava->sprite, 4);
}

/* See render.h for documentation */
void render_geo(const Geo *geo, const Camera *camera, UINT8 *base) {
    int rel_x = camera_get_relative_x(camera, geo->x);
    int rel_y = camera_get_relative_y(camera, geo->y);
    plot_bitmap_32(base, rel_y, rel_x, geo->sprite, geo->size);
}
