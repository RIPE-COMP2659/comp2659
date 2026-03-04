#include "world.h"

#define CAMERA_OFFSET 160

/* TODO: Level index management */
/* TODO: Levels must be in ascending x value order or logic will break */
/* Make sure levels adhere to this upon creation */
World create_world(Level* levels, Geo geo, unsigned int ground_y) {
    World world;

    world.levels = levels;
    world.geo = geo;
    world.camera = create_camera(0, SCREEN_HEIGHT);
    world.ground_y = ground_y;
    world.level_index = 0;
    world.levels_size = 2;

    return world;
}

/* TODO: World to keep track of it's own level index */
void world_update(World *world) {
    geo_update(&world->geo);
    camera_update_coordinates(&world->camera, world->geo.x - CAMERA_OFFSET, SCREEN_HEIGHT);
}

World get_world(void) {
    Level* levels = get_levels();
    unsigned int ground_y = 32;

    /* TODO: Should probably not be hard coded for starting x */
    /* TODO: Added some additional height to test the start, can be removed */
    Geo geo = create_geo(CAMERA_OFFSET, ground_y + GEO_SIZE + GEO_SIZE, ground_y);

    return create_world(levels, geo, ground_y);
}

void world_update_collisions(
    World *world,
    unsigned int block_min,
    unsigned int block_max,
    unsigned int spike_min,
    unsigned int spike_max,
    unsigned int lava_min,
    unsigned int lava_max
) {
    Level *level = &world->levels[world->level_index];

    /* Reset is_landed before checking blocks to determine if geo has support */
    world->geo.is_landed = FALSE;
    world_collision_geo_ground(world);

    world_collisions_geo_blocks(world, level->blocks, block_min, block_max);
    world_collisions_geo_spikes(world, level->spikes, spike_min, spike_max);
    world_collisions_geo_lava(world, level->lava, lava_min, lava_max);

    /* Update landed status based on final ground_y after all collisions */
    geo_update_landed(&world->geo);
}


/* TODO: Move these to a generic helper */
void world_collisions_geo_blocks(World *world, Block *blocks, unsigned int col_min_bi, unsigned int col_max_bi) {
    unsigned int i;
    for (i = col_min_bi; i <= col_max_bi; i++) {
        world_collision_geo_block(world, &blocks[i]);
    }
}

void world_collisions_geo_spikes(World *world, Spike *spikes, unsigned int col_min_si, unsigned int col_max_si) {
    unsigned int i;
    for (i = col_min_si; i <= col_max_si; i++) {
        world_collision_geo_spike(world, &spikes[i]);
    }
}

void world_collisions_geo_lava(World *world, Lava *lava, unsigned int col_min_li, unsigned int col_max_li) {
    unsigned int i;
    for (i = col_min_li; i <= col_max_li; i++) {
        world_collision_geo_lava(world, &lava[i]);
    }
}

void world_collision_geo_block(World *world, Block *block) {
    signed int collision = geo_check_square_collision(
        &world->geo,
        block->x,
        block->y,
        block->size
    );

    if (collision == COLLISION_TOP) {
        world->geo.ground_y = block->y;
        world->geo.is_landed = TRUE;
    } else if (collision == COLLISION_BOTTOM || collision == COLLISION_LEFT) {
        world->geo.is_dead = TRUE;
    } else if (collision == COLLISION_NONE) {
        /* No collision with this block, geo is not supported */
        world->geo.is_landed = FALSE;
    }
}

/* TODO: Implement proper spike collision detection */
void world_collision_geo_spike(World *world, Spike *spike) {
    signed int collision = geo_check_square_collision(
        &world->geo,
        spike->x,
        spike->y,
        spike->size
    );

    if (collision != COLLISION_NONE && collision != COLLISION_ERROR) {
        world->geo.is_dead = TRUE;
    }
}

/* TODO: Implement proper lava collision detection once we refactor to better lava objects */
void world_collision_geo_lava(World *world, Lava *lava) {
    signed int collision = geo_check_square_collision(
        &world->geo,
        lava->x,
        lava->y,
        lava->size
    );

    if (collision != COLLISION_NONE && collision != COLLISION_ERROR) {
        world->geo.is_dead = TRUE;
    }
}

void world_collision_geo_ground(World *world) {
    if (world->geo.is_landed == FALSE) {
        world->geo.ground_y = world->ground_y;
    }
}
