# include "level.h"
# include "geo.h"

/* TODO: Turn these into constants somehow */
typedef struct {
    Level* levels;
    Geo geo;
    unsigned int ground_y;
} World;

void world_placeholder(void);
