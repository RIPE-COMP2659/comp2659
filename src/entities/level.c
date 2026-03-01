#include "level.h"

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
    static Block level_blocks[12] = {
        {288, 64, BLOCK_SIZE, BLOCK_SPRITE},
        {352, 96, BLOCK_SIZE, BLOCK_SPRITE},
        {416, 96, BLOCK_SIZE, BLOCK_SPRITE},
        {480, 64, BLOCK_SIZE, BLOCK_SPRITE},
        {608, 96, BLOCK_SIZE, BLOCK_SPRITE},
        {640, 96, BLOCK_SIZE, BLOCK_SPRITE},
        {672, 96, BLOCK_SIZE, BLOCK_SPRITE},
        {704, 96, BLOCK_SIZE, BLOCK_SPRITE},
        {736, 96, BLOCK_SIZE, BLOCK_SPRITE},
        {768, 96, BLOCK_SIZE, BLOCK_SPRITE},
        {800, 96, BLOCK_SIZE, BLOCK_SPRITE},
        {832, 96, BLOCK_SIZE, BLOCK_SPRITE}
    };
    static Spike level_spikes[2] = {
        {256, 64, SPIKE_SIZE, SPIKE_SPRITE},
        {512, 64, SPIKE_SIZE, SPIKE_SPRITE}
    };
    static Lava level_lava[5] = {
        {320, 32, LAVA_SIZE, LAVA_SPRITE},
        {352, 32, LAVA_SIZE, LAVA_SPRITE},
        {384, 32, LAVA_SIZE, LAVA_SPRITE},
        {416, 32, LAVA_SIZE, LAVA_SPRITE},
        {448, 32, LAVA_SIZE, LAVA_SPRITE}
    };
    unsigned int blocks_size = (sizeof(level_blocks) / sizeof(level_blocks[0]));
    unsigned int spikes_size = (sizeof(level_spikes) / sizeof(level_spikes[0]));
    unsigned int lava_size = (sizeof(level_lava) / sizeof(level_lava[0]));

    return create_level(
        level_blocks,
        level_spikes,
        level_lava,
        blocks_size,
        spikes_size,
        lava_size,
        1000
    );
}

Level get_level2(void) {
    static Block level_blocks[2] = {
        {0, 0, BLOCK_SIZE, BLOCK_SPRITE},
        {100, 200, BLOCK_SIZE, BLOCK_SPRITE}
    };
    static Spike level_spikes[1] = {
        {300, 400, SPIKE_SIZE, SPIKE_SPRITE}
    };
    static Lava level_lava[1] = {
        {500, 600, LAVA_SIZE, LAVA_SPRITE}
    };
    unsigned int blocks_size = (sizeof(level_blocks) / sizeof(level_blocks[0])) - 1;
    unsigned int spikes_size = (sizeof(level_spikes) / sizeof(level_spikes[0])) - 1;
    unsigned int lava_size = (sizeof(level_lava) / sizeof(level_lava[0])) - 1;

    return create_level(
        level_blocks,
        level_spikes,
        level_lava,
        blocks_size,
        spikes_size,
        lava_size,
        1000
    );
}

Level* get_levels(void) {
    static Level levels[2];

    levels[0] = get_level1();
    levels[1] = get_level2();

    return levels;
}

void level_placeholder(void) {
}
