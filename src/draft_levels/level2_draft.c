/**
 * Draft of Level 2 (The Impossible Game - Phobos approximation)
 * To be integrated into src/model/level.c once approved.
 */

#include "../../model/level.h"

/* Level two draft sizes */
#define L2_DRAFT_BLOCKS_SIZE 25
#define L2_DRAFT_SPIKES_SIZE 15
#define L2_DRAFT_LAVA_SIZE 5

Level get_level2_draft(void) {
    static Block level_blocks[L2_DRAFT_BLOCKS_SIZE];
    static Spike level_spikes[L2_DRAFT_SPIKES_SIZE];
    static Lava level_lava[L2_DRAFT_LAVA_SIZE];

    /* Block initialization 
     * The impossible game level 2 has step-ups and floating platforms.
     * Blocks are 32x32 typically. Base ground is y=default. 
     * X spacing determines timing.
     */
    
    /* First staircase obstacle */
    level_blocks[0] = create_block(400, 32); 
    level_blocks[1] = create_block(432, 64);
    level_blocks[2] = create_block(464, 96);
    
    /* Gap, then a floating platform to jump onto */
    level_blocks[3] = create_block(600, 64);
    level_blocks[4] = create_block(632, 64);
    level_blocks[5] = create_block(664, 64);

    /* Dropping back down, series of ground pillars */
    level_blocks[6] = create_block(800, 32);
    level_blocks[7] = create_block(950, 64);
    level_blocks[8] = create_block(1100, 32);
    
    /* A long elevated bridge over lava */
    level_blocks[9] = create_block(1250, 96);
    level_blocks[10] = create_block(1282, 96);
    level_blocks[11] = create_block(1314, 96);
    level_blocks[12] = create_block(1346, 96);
    level_blocks[13] = create_block(1378, 96);
    
    /* Small stepping stones */
    level_blocks[14] = create_block(1550, 64);
    level_blocks[15] = create_block(1700, 96);
    level_blocks[16] = create_block(1850, 128);

    /* Final tunnel / flat ground stretch */
    int i, x_offset = 2000;
    for(i = 17; i < L2_DRAFT_BLOCKS_SIZE; i++) {
        level_blocks[i] = create_block(x_offset, 32);
        x_offset += 32;
    }

    /* Lava initialization 
     * Placed under the elevated bridge and between the stepping stones
     */
    level_lava[0] = create_lava(1250, 32);
    level_lava[1] = create_lava(1300, 32);
    level_lava[2] = create_lava(1350, 32);
    level_lava[3] = create_lava(1400, 32);
    level_lava[4] = create_lava(1500, 32);

    /* Spike initialization 
     * Placed periodically to force jumps
     */
    /* Spike before the first staircase */
    level_spikes[0] = create_spike(300, 32);
    
    /* Spikes on top of the floating platform */
    level_spikes[1] = create_spike(632, 96);
    
    /* Spikes immediately after pillars to force precise jumps */
    level_spikes[2] = create_spike(850, 32);
    level_spikes[3] = create_spike(1000, 32);
    
    /* Spikes on the bridge */
    level_spikes[4] = create_spike(1314, 128);

    /* Trap spike below stepping stones */
    level_spikes[5] = create_spike(1600, 32);
    level_spikes[6] = create_spike(1750, 32);

    /* Spikes on the final flat stretch */
    x_offset = 2050;
    for(i = 7; i < L2_DRAFT_SPIKES_SIZE; i++) {
        level_spikes[i] = create_spike(x_offset, 64);
        x_offset += 100; /* Spaced out fairly evenly */
    }

    /* Create and return the level */
    return create_level(
        level_blocks,
        level_spikes,
        level_lava,
        L2_DRAFT_BLOCKS_SIZE,
        L2_DRAFT_SPIKES_SIZE,
        L2_DRAFT_LAVA_SIZE,
        19000 /* Level End X */
    );
}
