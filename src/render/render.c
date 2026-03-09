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

    render_ground(model, base);

    for (i = model->cam_min_bi; i < model->cam_max_bi; i++) {
        render_block(&model->world.levels[0].blocks[i], cam, base);
    }

    for (i = model->cam_min_si; i < model->cam_max_si; i++) {
        render_spike(&model->world.levels[0].spikes[i], cam, base);
    }

    for (i = model->cam_min_li; i < model->cam_max_li; i++) {
        render_lava(&model->world.levels[0].lava[i], cam, base);
    }

    render_geo(&model->world.geo, cam, base);
}

/* See render.h for documentation */
void render_ground(const Model *model, UINT8 *base) {
    int rel_y = camera_get_relative_y(&model->world.camera, model->world.ground_y);
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
