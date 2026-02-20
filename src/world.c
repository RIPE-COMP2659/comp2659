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
	unsigned int ground_y = 0;
	Geo geo = create_geo(100, 200, ground_y);

	levels = get_levels();

	return create_world(levels, geo, ground_y);
}

void world_placeholder(void) {
}
