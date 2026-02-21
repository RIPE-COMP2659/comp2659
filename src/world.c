#include "world.h"

World create_world(Level* levels, Geo geo, unsigned int ground_y) {
	World world;

	world.levels = levels;
	world.geo = geo;
	world.ground_y = ground_y;

	return world;
}

World get_world(void) {
	Level* levels;
	unsigned int ground_y = 32;
	Geo geo = create_geo(100, ground_y + GEO_SIZE, ground_y);

	levels = get_levels();

	return create_world(levels, geo, ground_y);
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
    } else if (collision == COLLISION_BOTTOM || collision == COLLISION_LEFT) {
        world->geo.is_dead = TRUE;
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
