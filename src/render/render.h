#include "../model/model.h"
#include "../raster/raster.h"

/* * Master render function: Renders a single frame based on the world snapshot. */
void render(const World *world, UINT8 *base);

void render_geo(const Geo *geo, const Camera *camera, UINT8 *base);
void render_block(const Block *block, const Camera *camera, UINT8 *base);
void render_spike(const Spike *spike, const Camera *camera, UINT8 *base);
void render_lava(const Lava *lava, const Camera *camera, UINT8 *base);
void render_ground(const World *world, UINT8 *base);
