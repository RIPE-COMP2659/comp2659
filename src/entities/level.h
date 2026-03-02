#include "block.h"
#include "lava.h"
#include "spike.h"

/* TODO: Can turn some of this into constants, somehow */
typedef struct {
  Block *blocks;
  Spike *spikes;
  Lava *lava;
  unsigned int blocks_size;
  unsigned int spikes_size;
  unsigned int lava_size;
  unsigned int end_x;
} Level;

Level create_level(
    Block *blocks,
    Spike *spikes,
    Lava *lava,
    unsigned int blocks_size,
    unsigned int spikes_size,
    unsigned int lava_size,
    unsigned int end_x
);
Level get_level1(void);
Level get_level2(void);
Level *get_levels(void);
void level_placeholder(void);
