# include "block.h"
# include "spike.h"
# include "lava.h"

/* TODO: Can turn some of this into constants, somehow */
typedef struct {
    Block* blocks;
    Spike* spikes;
    Lava* lava;
    unsigned int current_block;
    unsigned int current_spike;
    unsigned int current_lava;
    unsigned int end_x;
} Level;

Level create_level(Block* blocks, Spike* spikes, Lava* lava, unsigned int end_x);
Level create_level1(void);
Level create_level2(void);
void level_placeholder(void);
