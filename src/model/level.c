#include "level.h"

/* Constants for level sizes */
#define NUM_LEVELS 2

/* Level one */
#define L1_BLOCKS_SIZE 12
#define L1_SPIKES_SIZE 2
#define L1_LAVA_SIZE 5

/* Level two */
#define L2_BLOCKS_SIZE 2
#define L2_SPIKES_SIZE 1
#define L2_LAVA_SIZE 1

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
    static Lava level_lava[L1_LAVA_SIZE];
    static Spike level_spikes[L1_SPIKES_SIZE];

    /* Block initialization */
    level_blocks[0] = create_block(288, 64);
    level_blocks[1] = create_block(352, 96);
    level_blocks[2] = create_block(416, 96);
    level_blocks[3] = create_block(480, 64);
    level_blocks[4] = create_block(608, 96);
    level_blocks[5] = create_block(640, 96);
    level_blocks[6] = create_block(672, 96);
    level_blocks[7] = create_block(704, 96);
    level_blocks[8] = create_block(736, 96);
    level_blocks[9] = create_block(768, 96);
    level_blocks[10] = create_block(800, 96);
    level_blocks[11] = create_block(832, 96);

    /* Lava initialization */
    level_lava[0] = create_lava(320, 32);
    level_lava[1] = create_lava(352, 32);
    level_lava[2] = create_lava(384, 32);
    level_lava[3] = create_lava(416, 32);
    level_lava[4] = create_lava(448, 32);

    /* Spike initialization */
    level_spikes[0] = create_spike(256, 64);
    level_spikes[1] = create_spike(512, 64);

    return create_level(
        level_blocks,
        level_spikes,
        level_lava,
        L1_BLOCKS_SIZE,
        L1_SPIKES_SIZE,
        L1_LAVA_SIZE,
        1000
    );
}

Level get_level2(void) {
    static Block level_blocks[L2_BLOCKS_SIZE];
    static Spike level_spikes[L2_SPIKES_SIZE];
    static Lava level_lava[L2_LAVA_SIZE];

    /* Block initialization */
    level_blocks[0] = create_block(100, 100);
    level_blocks[1] = create_block(200, 200);

    /* Lava initialization */
    level_lava[0] = create_lava(500, 600);

    /* Spike initialization */
    level_spikes[0] = create_spike(300, 400);

    return create_level(
        level_blocks,
        level_spikes,
        level_lava,
        L2_BLOCKS_SIZE,
        L2_SPIKES_SIZE,
        L2_LAVA_SIZE,
        1000
    );
}

Level* get_levels(void) {
    static Level levels[NUM_LEVELS];

    levels[0] = get_level1();
    levels[1] = get_level2();

    return levels;
}
