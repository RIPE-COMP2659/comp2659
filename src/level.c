#include "level.h"

Level create_level(Block* blocks, Spike* spikes, Lava* lava, unsigned int end_x) {
	Level level;

	level.blocks = blocks;
	level.spikes = spikes;
	level.lava = lava;
	level.current_block = 0;
	level.current_spike = 0;
	level.current_lava = 0;
	level.end_x = end_x;

	return level;
}

Level get_level1(void) {
    Block level_blocks[2] = {
        {0, 0, BLOCK_SIZE, BLOCK_SPRITE},
        {100, 200, BLOCK_SIZE, BLOCK_SPRITE}
    };
    Spike level_spikes[1] = {
        {300, 400, SPIKE_SIZE, SPIKE_SPRITE}
    };
    Lava level_lava[1] = {
        {500, 600, LAVA_SIZE, LAVA_SPRITE}
    };
    return create_level(level_blocks, level_spikes, level_lava, 1000);
}

Level get_level2(void) {
    Block level_blocks[2] = {
        {0, 0, BLOCK_SIZE, BLOCK_SPRITE},
        {100, 200, BLOCK_SIZE, BLOCK_SPRITE}
    };
    Spike level_spikes[1] = {
        {300, 400, SPIKE_SIZE, SPIKE_SPRITE}
    };
    Lava level_lava[1] = {
        {500, 600, LAVA_SIZE, LAVA_SPRITE}
    };
    return create_level(level_blocks, level_spikes, level_lava, 1000);
}

void level_placeholder(void) {
}
