#include "world.h"

#define CAMERA_OFFSET 160

/* TODO: Levels must be in ascending x value order or logic will break */
/* Make sure levels adhere to this upon creation */
World create_world(Level* levels, Geo geo, unsigned int ground_y) {
	World world;

	world.levels = levels;
	world.geo = geo;
	world.camera = create_camera(0, SCREEN_HEIGHT);
	world.ground_y = ground_y;
    world.cam_min_bi = 0; /* block index */
    world.cam_max_bi = 0; /* block index */
    world.cam_min_si = 0; /* spike index */
    world.cam_max_si = 0; /* spike index */
    world.cam_min_li = 0; /* lava index */
    world.cam_max_li = 0; /* lava index */

    world_update_camera(&world, 0);

	return world;
}

/* TODO: World to keep track of it's own level index */
void world_update(World *world, unsigned int level_index) {
    geo_update(&world->geo);
    camera_update_coordinates(&world->camera, world->geo.x - CAMERA_OFFSET, SCREEN_HEIGHT);
    world_update_camera(world, level_index);
}

World get_world(void) {
	Level* levels = get_levels();
	unsigned int ground_y = 32;
    /* TODO: Should probably not be hard coded for starting x */
    /* TODO: Added some additional height to test the start, can be removed */
	Geo geo = create_geo(CAMERA_OFFSET, ground_y + GEO_SIZE + GEO_SIZE, ground_y);

	return create_world(levels, geo, ground_y);
}

/* Generic helper function to update camera indices for any entity type */
static void update_camera_indices(
    unsigned int *cam_min,
    unsigned int *cam_max,
    unsigned int camera_left,
    unsigned int camera_right,
    unsigned int entity_count,
    unsigned int (*get_x)(void*, unsigned int),
    unsigned int (*get_size)(void*, unsigned int),
    void *entities
) {
    unsigned int i;

    /* Find max first: iterate from current max to find last entity with left edge on screen */
    i = *cam_max;
    while (i <= entity_count && get_x(entities, i) < camera_right) {
        i++;
    }

    /* Set max to the last entity before the one that's off screen */
    *cam_max = (i > 0) ? i - 1 : 0;

    /* Find min: iterate from current min (up to new max) to find first entity with right edge on screen */
    i = *cam_min;
    while (i <= *cam_max && get_x(entities, i) + get_size(entities, i) <= camera_left) {
        i++;
    }

    *cam_min = i;
}

/* Helper functions for blocks */
static unsigned int get_block_x(void *blocks, unsigned int index) {
    return ((Block*)blocks)[index].x;
}

static unsigned int get_block_size(void *blocks, unsigned int index) {
    return ((Block*)blocks)[index].size;
}

/* Helper functions for spikes */
static unsigned int get_spike_x(void *spikes, unsigned int index) {
    return ((Spike*)spikes)[index].x;
}

static unsigned int get_spike_size(void *spikes, unsigned int index) {
    return ((Spike*)spikes)[index].size;
}

/* Helper functions for lava */
static unsigned int get_lava_x(void *lava, unsigned int index) {
    return ((Lava*)lava)[index].x;
}

static unsigned int get_lava_size(void *lava, unsigned int index) {
    return ((Lava*)lava)[index].size;
}

void world_update_camera(World *world, unsigned int level_index) {
    world_update_camera_bi(world, level_index);
    world_update_camera_si(world, level_index);
    world_update_camera_li(world, level_index);
}

void world_update_camera_bi(World *world, unsigned int level_index) {
    Level *level = &world->levels[level_index];
    unsigned int camera_left = world->camera.x;
    unsigned int camera_right = world->camera.x + world->camera.width;

    update_camera_indices(
        &world->cam_min_bi,
        &world->cam_max_bi,
        camera_left,
        camera_right,
        level->blocks_size,
        get_block_x,
        get_block_size,
        level->blocks
    );
}

void world_update_camera_si(World *world, unsigned int level_index) {
    Level *level = &world->levels[level_index];
    unsigned int camera_left = world->camera.x;
    unsigned int camera_right = world->camera.x + world->camera.width;

    update_camera_indices(
        &world->cam_min_si,
        &world->cam_max_si,
        camera_left,
        camera_right,
        level->spikes_size,
        get_spike_x,
        get_spike_size,
        level->spikes
    );
}

void world_update_camera_li(World *world, unsigned int level_index) {
    Level *level = &world->levels[level_index];
    unsigned int camera_left = world->camera.x;
    unsigned int camera_right = world->camera.x + world->camera.width;

    update_camera_indices(
        &world->cam_min_li,
        &world->cam_max_li,
        camera_left,
        camera_right,
        level->lava_size,
        get_lava_x,
        get_lava_size,
        level->lava
    );
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
