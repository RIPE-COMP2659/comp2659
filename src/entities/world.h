#include "camera.h"
#include "geo.h"
#include "level.h"

/* TODO: Turn these into constants somehow */
typedef struct {
  Level *levels;
  Geo geo;
  unsigned int ground_y;
  Camera camera;
  unsigned int cam_min_bi;
  unsigned int cam_max_bi;
  unsigned int cam_min_si; /* spike index */
  unsigned int cam_max_si; /* spike index */
  unsigned int cam_min_li; /* lava index */
  unsigned int cam_max_li; /* lava index */
} World;

World create_world(Level *levels, Geo geo, unsigned int ground_y);
World get_world(void);
void world_update_camera(World *world, unsigned int level_index);
void world_update_camera_bi(World *world, unsigned int level_index);
void world_update_camera_si(World *world, unsigned int level_index);
void world_update_camera_li(World *world, unsigned int level_index);
void world_collision_geo_block(World *world, Block *block);
void world_collision_geo_spike(World *world, Spike *spike);
void world_collision_geo_lava(World *world, Lava *lava);
void world_collision_geo_ground(World *world);
