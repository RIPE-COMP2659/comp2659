/**
 * level.c
 * Authors:
 *     Riley Gramlich, rgram060@mtroyal.ca, 201762060
 *     Robert Parker Hutcheson, rhutc335@mtroyal.ca, 201762335
 *     Isaac Klein, iklei977@mtroyal.ca, 201763977
 *     Eduard Mykhailets, emykh268@mtroyal.ca, 201750268
 * Course: COMP 2659-001, Computing Machinery II, Winter 2026
 * Instructor: Nolan Shaw
 *
 * PURPOSE: Level definitions and helpers. Provides convenience functions
 *          to create Level structures and returns a static array of
 *          predefined levels used by the game.
 */
#include "level.h"

/* Constants for level sizes */
#define NUM_LEVELS 1

/* Level one (consistently using L2 sizes for current draft) */
#define L1_BLOCKS_SIZE 200
#define L1_SPIKES_SIZE 15
#define L1_LAVA_SIZE 5

Level create_level(
    Block *blocks,
    Spike *spikes,
    Lava *lava,
    unsigned int blocks_size,
    unsigned int spikes_size,
    unsigned int lava_size,
    unsigned int end_x)
{
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

Level get_level1(void)
{
    static Block level_blocks[L1_BLOCKS_SIZE];
    static Spike level_spikes[L1_SPIKES_SIZE];
    static Lava level_lava[L1_LAVA_SIZE];
    int i;
    int current_block;
    unsigned int last_x;
    unsigned int last_y;
    unsigned int next_y;

    /* Test 1: 3-block right jump over lava (x_diff = 96px) */
    /* Starting block */
    level_blocks[0] = create_block(464, 64);
    level_blocks[1] = create_block(464, 96);
    /* 2-block wide lava pit (for a 3-right jump) */
    level_lava[0] = create_lava(496, 32);
    level_lava[1] = create_lava(528, 32);

    /* 2-high landing block (strictly 3 blocks right: 464 + 96 = 560) */
    
    level_blocks[2] = create_block(560, 64);

#define NUM_STAIR_STEPS 15

    /* Test 2: Ascending stair ladder (NUM_STAIR_STEPS steps, 4-right jumps = 128px) */
    current_block = 3;
    last_x = 928;
    last_y = 64;
    for(i = 0; i < NUM_STAIR_STEPS; i++) {
        level_blocks[current_block++] = create_block(last_x + (i * 128), last_y + (i * 32));
    }

    /* Test 3: Descending stairway (NUM_STAIR_STEPS steps, 5-right spacing = 160px) */
    /* Start descent from the final peak of Test 2 plus a 5-block gap */
    last_x = last_x + ((NUM_STAIR_STEPS - 1) * 128) + 160;
    last_y = last_y + ((NUM_STAIR_STEPS - 1) * 32);
    for(i = 0; i < NUM_STAIR_STEPS; i++) {
        /* Each step down is 1 block lower than the previous peak/step */
        next_y = last_y - ((i + 1) * 32);
        /* If we go below ground level (64), stop decreasing height */
        if (next_y < 64) next_y = 64; 
        level_blocks[current_block++] = create_block(last_x + (i * 160), next_y);
    }

    /* Fill remainder with dummy values to avoid garbage rendering */
    for(i = current_block; i < L1_BLOCKS_SIZE; i++) {
        level_blocks[i] = create_block(0, 0); 
    }
    for(i = 0; i < L1_SPIKES_SIZE; i++) {
        level_spikes[i] = create_spike(0, 0);
    }
    for(i = 2; i < L1_LAVA_SIZE; i++) {
        level_lava[i] = create_lava(0, 0);
    }

    /* Set actual sizes to matching indices to avoid dummy rendering overhead */
    return create_level(
        level_blocks,
        level_spikes,
        level_lava,
        current_block,
        0,
        2,
        6000
    );
}

Level *get_levels(void)
{
    static Level levels[NUM_LEVELS];

    levels[0] = get_level1();

    return levels;
}
