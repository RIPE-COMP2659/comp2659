#include "level.h"

/* Constants for level sizes */
#define NUM_LEVELS 1

/* Level one (consistently using L2 sizes for current draft) */
#define L1_BLOCKS_SIZE 25
#define L1_SPIKES_SIZE 15
#define L1_LAVA_SIZE 5

Level create_level(
    Block* blocks,
    Spike* spikes,
    Lava* lava,
    unsigned int blocks_size,
    unsigned int spikes_size,
    unsigned int lava_size,
    unsigned int end_x
) {
    Level level;

    level.blocks = blocks;
    level.spikes = spikes;
    level.lava = lava;
    level.blocks_size = blocks_size;
    level.spikes_size = spikes_size;
    level.lava_size = lava_size;
    level.end_x = end_x;

    return level;
}

Level get_level1(void) {
    static Block level_blocks[L1_BLOCKS_SIZE];
    static Spike level_spikes[L1_SPIKES_SIZE];
    static Lava level_lava[L1_LAVA_SIZE];
    int i;

    /* Test 1: 4-block lava jump from 1-high to 2-high (Shifted +64px) */
    /* 1-high starting block */
    level_blocks[0] = create_block(464, 64);

    /* 4-block wide lava pit */
    level_lava[0] = create_lava(496, 32);
    level_lava[1] = create_lava(528, 32);
    level_lava[2] = create_lava(560, 32);
    level_lava[3] = create_lava(592, 32);

    /* 2-high landing block */
    level_blocks[1] = create_block(624, 64);
    level_blocks[2] = create_block(624, 96);

    /* Test 2: Ascending stair ladder (5 steps, 4-block gap, Shifted +160px) */
    /* Step 1: 800 + 160 = 960 */
    level_blocks[3] = create_block(960, 64);
    /* Step 2: 960 + 160 = 1120 */
    level_blocks[4] = create_block(1120, 96);
    /* Step 3: 1120 + 160 = 1280 */
    level_blocks[5] = create_block(1280, 128);
    /* Step 4: 1280 + 160 = 1440 */
    level_blocks[6] = create_block(1440, 160);
    /* Step 5: 1440 + 160 = 1600 */
    level_blocks[7] = create_block(1600, 192);

    /* Test 3: Descending stairway (5 steps, 5-block spacing, Shifted +160px) */
    /* First block down: 1600 + 160 = 1760 */
    level_blocks[8] = create_block(1760, 160);
    level_blocks[9] = create_block(1920, 128);
    level_blocks[10] = create_block(2080, 96);
    level_blocks[11] = create_block(2240, 64);
    level_blocks[12] = create_block(2400, 64);

    /* Fill remainder with dummy values to avoid garbage rendering */
    for(i = 13; i < L1_BLOCKS_SIZE; i++) {
        level_blocks[i] = create_block(0, 0); 
    }
    for(i = 0; i < L1_SPIKES_SIZE; i++) {
        level_spikes[i] = create_spike(0, 0);
    }
    for(i = 4; i < L1_LAVA_SIZE; i++) {
        level_lava[i] = create_lava(0, 0);
    }

    /* Set actual sizes to matching indices to avoid dummy rendering overhead */
    return create_level(
        level_blocks,
        level_spikes,
        level_lava,
        13,
        0,
        4,
        19000
    );
}

Level* get_levels(void) {
    static Level levels[NUM_LEVELS];

    levels[0] = get_level1();

    return levels;
}
