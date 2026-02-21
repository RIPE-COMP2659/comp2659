#ifndef LEVEL_H
#define LEVEL_H

#include "block.h"
#include "lava.h"
#include "spike.h"


/* TODO: Can turn some of this into constants, somehow */
typedef struct {
  Block *blocks;
  Spike *spikes;
  Lava *lava;
  unsigned int current_block;
  unsigned int current_spike;
  unsigned int current_lava;
  unsigned int end_x;
} Level;

Level create_level(Block* blocks, Spike* spikes, Lava* lava, unsigned int end_x);
Level get_level1(void);
Level get_level2(void);
Level* get_levels(void);
void level_placeholder(void);

#endif /* LEVEL_H */
