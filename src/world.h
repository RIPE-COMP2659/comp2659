# include "level.h"
# include "geo.h"

/* TODO: Turn these into constants somehow */
typedef struct {
    Level* levels;
    Geo geo;
    unsigned int ground_y;
} World;

World create_world(Level* levels, Geo geo, unsigned int ground_y);
World get_world(void);
void world_collision_geo_block(World *world, Block *block);
void world_collision_geo_spike(World *world, Spike *spike);
void world_collision_geo_lava(World *world, Lava *lava);\
void world_collision_geo_ground(World *world);
