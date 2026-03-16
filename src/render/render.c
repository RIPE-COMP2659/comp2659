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

/* See render.h for documentation */
void render(const Model *model, UINT8 *base) {
    unsigned int i;
    const Camera *cam = &model->world.camera;
    const Camera *old_cam = &model->old_cam;
    int level_i = model->world.level_index;
    Level level = model->world.levels[level_i];
    int input;

    clear_ground(base, old_cam, model->world.ground_y);
    clear_blocks(base, old_cam, level.blocks, model->cam_min_bi, model->cam_max_bi);
    clear_spikes(base, old_cam, level.spikes, model->cam_min_si, model->cam_max_si);
    clear_lava(base, old_cam, level.lava, model->cam_min_li, model->cam_max_li);
    clear_geo(base, old_cam, &model->world.geo);

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

clear_geo(UINT8 *base, const Camera *camera, const Geo *geo) {
    int rel_x = camera_get_relative_x(camera, geo->x);
    int rel_y = camera_get_relative_y(camera, geo->y);
    clear_region((UINT32 *)base, rel_y, rel_x, geo->size, geo->size);
}

clear_lava(UINT8 *base, const Camera *camera, const Lava *lava, unsigned int min_li, unsigned int max_li) {
    unsigned int i;
    for (i = min_li; i <= max_li; i++) {
        int rel_x = camera_get_relative_x(camera, lava[i].x);
        int rel_y = camera_get_relative_y(camera, lava[i].y);
        clear_region((UINT32 *)base, rel_y, rel_x, lava[i].size, lava[i].size);
    }
}

clear_spikes(UINT8 *base, const Camera *camera, const Spike *spikes, unsigned int min_si, unsigned int max_si) {
    unsigned int i;
    for (i = min_si; i <= max_si; i++) {
        int rel_x = camera_get_relative_x(camera, spikes[i].x);
        int rel_y = camera_get_relative_y(camera, spikes[i].y);
        clear_region((UINT32 *)base, rel_y, rel_x, spikes[i].size, spikes[i].size);
    }
}

clear_blocks(UINT8 *base, const Camera *camera, const Block *blocks, unsigned int min_bi, unsigned int max_bi) {
    unsigned int i;
    for (i = min_bi; i <= max_bi; i++) {
        int rel_x = camera_get_relative_x(camera, blocks[i].x);
        int rel_y = camera_get_relative_y(camera, blocks[i].y);
        clear_region((UINT32 *)base, rel_y, rel_x, blocks[i].size, blocks[i].size);
    }
}

clear_ground(UINT8 *base, const Camera *camera, unsigned int ground_y) {
    int rel_y = camera_get_relative_y(camera, ground_y);
    /* Clear a 4-pixel tall rectangle across the screen width (640 px) */
    clear_region((UINT32 *)base, rel_y, 0, 4, 640);
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
