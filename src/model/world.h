#include "camera.h"
#include "geo.h"
#include "level.h"

/* TODO: Turn these into constants somehow */
typedef struct {
    Level *levels;
    Geo geo;
    unsigned int ground_y;
    Camera camera;
    unsigned int level_index;
    unsigned int levels_size;
} World;

World create_world(Level *levels, Geo geo, unsigned int ground_y);
World get_world(void);
void world_update(World *world);
void world_update_collisions(
    World *world,
    unsigned int block_min,
    unsigned int block_max,
    unsigned int spike_min,
    unsigned int spike_max,
    unsigned int lava_min,
    unsigned int lava_max
);
void world_collisions_geo_blocks(World *world, Block *blocks, unsigned int col_min_bi, unsigned int col_max_bi);
void world_collisions_geo_spikes(World *world, Spike *spikes, unsigned int col_min_si, unsigned int col_max_si);
void world_collisions_geo_lava(World *world, Lava *lava, unsigned int col_min_li, unsigned int col_max_li);
void world_collision_geo_block(World *world, Block *block);
void world_collision_geo_spike(World *world, Spike *spike);
void world_collision_geo_lava(World *world, Lava *lava);
void world_collision_geo_ground(World *world);
