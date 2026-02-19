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

void level_placeholder(void) {
}
