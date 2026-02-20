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
	Geo geo = { GEO_DDY, GEO_DX, 0, 100, 200, GEO_SIZE, GEO_SPRITE };

	levels = get_levels();

	return create_world(levels, geo, 0);
}

void world_placeholder(void) {
}
