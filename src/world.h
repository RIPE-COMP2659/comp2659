#ifndef WORLD_H
#define WORLD_H

#include "geo.h"
#include "level.h"


/* TODO: Turn these into constants somehow */
typedef struct {
  Level *levels;
  Geo geo;
  unsigned int ground_y;
} World;

World create_world(Level *levels, Geo geo, unsigned int ground_y);
World get_world(void);
void world_placeholder(void);

#endif /* WORLD_H */
