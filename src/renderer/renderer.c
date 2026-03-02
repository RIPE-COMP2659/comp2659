#include "renderer.h"

void render_world(const World *world, UINT32 *base) {
    int i;
    const Level *level = &world->level;

    /* 1. Clear the screen or relevant region if not handled by caller */
    clear_screen(base);

    /* 2. Render all blocks */
    for (i = 0; i < level->num_blocks; i++) {
        render_block(&level->blocks[i], base);
    }

    /* 3. Render all spikes */
    for (i = 0; i < level->num_spikes; i++) {
        render_spike(&level->spikes[i], base);
    }

    /* 4. Render Geo (the player character) */
    render_geo(&world->geo, base);

    /* 5. Render Lava */
    for (i = 0; i < level->num_lava; i++) {
        render_lava(&level->lava[level->num_lava], base);
    }
}

void render_block(const Block *block, UINT32 *base) {
    /* Assuming blocks are 32x32 based on typical geometry dash clones, 
       or using the dimensions in the block struct if available. 
       We use plot_bitmap_32 as the standard block size. */
    plot_bitmap_32(base, block->x, block->y, block->bitmap, 32);
}

void render_spike(const Spike *spike, UINT32 *base) {
    /* Spikes are usually smaller or handled as 32x32 bitmaps with transparency */
    plot_bitmap_32(base, spike->x, spike->y, spike->bitmap, 32);
}

void render_geo(const Geo *geo, UINT32 *base) {
    /* Geo is the player, typically 32x32 */
    plot_bitmap_32(base, geo->x, geo->y, geo->bitmap, 32);
}

void render_lava(const Lava *lava, UINT32 *base) {
    /* Lava might be plotted as blocks or horizontal strips */
    plot_bitmap_32(base, lava->x, lava->y, lava->bitmap, 32);
}