#include "renderer.h"
#include "model/camera.h"

/*
 * Function: render
 * ----------------
 * Renders the entire world state to the provided framebuffer.
 * Only renders objects within the camera's active indices.
 */
void render(const World *world, UINT8 *base) {
    unsigned int i;
    const Camera *cam = &world->camera;

    /* 1. Render static ground */
    render_ground(world, base);

    /* 2. Render blocks within current camera view */
    for (i = world->cam_min_bi; i < world->cam_max_bi; i++) {
        render_block(&world->levels[0].blocks[i], cam, base);
    }

    /* 3. Render spikes within current camera view */
    for (i = world->cam_min_si; i < world->cam_max_si; i++) {
        render_spike(&world->levels[0].spikes[i], cam, base);
    }

    /* 4. Render lava within current camera view */
    for (i = world->cam_min_li; i < world->cam_max_li; i++) {
        render_lava(&world->levels[0].lava[i], cam, base);
    }

    /* 5. Render Geo (Player) */
    render_geo(&world->geo, cam, base);
}

void render_ground(const World *world, UINT8 *base) {
    int rel_y = camera_get_relative_y(&world->camera, world->ground_y);
    /* Draw a 4-pixel tall rectangle across the screen width (640 px) */
    plot_rectangle((UINT32 *)base, rel_y, 0, 4, 640);
}

void render_block(const Block *block, const Camera *camera, UINT8 *base) {
    int rel_x = camera_get_relative_x(camera, block->x);
    int rel_y = camera_get_relative_y(camera, block->y);
    plot_bitmap_32(base, rel_y, rel_x, block->sprite, block->size);
}

void render_spike(const Spike *spike, const Camera *camera, UINT8 *base) {
    int rel_x = camera_get_relative_x(camera, spike->x);
    int rel_y = camera_get_relative_y(camera, spike->y);
    plot_bitmap_32(base, rel_y, rel_x, spike->sprite, spike->size);
}

void render_lava(const Lava *lava, const Camera *camera, UINT8 *base) {
    int rel_x = camera_get_relative_x(camera, lava->x);
    int rel_y = camera_get_relative_y(camera, lava->y);
    plot_bitmap_32(base, rel_y, rel_x, lava->sprite, lava->size);
}

void render_geo(const Geo *geo, const Camera *camera, UINT8 *base) {
    int rel_x = camera_get_relative_x(camera, geo->x);
    int rel_y = camera_get_relative_y(camera, geo->y);
    plot_bitmap_32(base, rel_y, rel_x, geo->sprite, geo->size);
}