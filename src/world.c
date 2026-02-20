#include "world.h"

World create_world(Level* levels, Geo geo, unsigned int ground_y) {
	World world;

	world.levels = levels;
	world.geo = geo;
	world.ground_y = ground_y;

	return world;
}

World get_world(void) {
	static Level levels[2];
	Geo geo = { GEO_DDY, GEO_DX, 0, 100, 200, GEO_SIZE, GEO_SPRITE };

	levels[0] = get_level1();
	levels[1] = get_level2();

	return create_world(levels, geo, 0);
}

void world_placeholder(void) {
}
