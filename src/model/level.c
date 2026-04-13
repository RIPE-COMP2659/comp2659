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
#define L1_SPIKES_SIZE 20
#define L1_LAVA_SIZE 100

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
    int current_lava;
    int current_spike;
    unsigned int last_y;
    unsigned int next_y;
    unsigned int bx;
    unsigned int by;
    unsigned int jump_width;
    int k;

    /* Test 1: Triple Spike (3 spikes in a row) */
    level_spikes[0] = create_spike(352, 64);
    level_spikes[1] = create_spike(384, 64);
    level_spikes[2] = create_spike(416, 64);

    current_block = 0;
    current_lava = 0;
    current_spike = 3;

#define NUM_STAIR_STEPS 15

    /* Test 2: Ascending stair ladder with drift compensation (every 4th jump wider) */
    bx = 928;
    last_y = 64;
    for(i = 0; i < NUM_STAIR_STEPS; i++) {
        by = last_y + (i * 32);
        level_blocks[current_block++] = create_block(bx, by);

        /* Fill lava directly under the block if it's hovering */
        if (by > 64) {
             level_lava[current_lava++] = create_lava(bx, 32);
        }

        /* If there's another step, fill the gap with lava */
        if (i < NUM_STAIR_STEPS - 1) {
            jump_width = 128;
            if ((i + 1) % 4 == 0) {
                jump_width += 32; /* +1 block width every 4th jump */
            }
            
            if (jump_width > 32) {
                level_lava[current_lava++] = create_lava_rect(bx + 32, 32, jump_width - 32);
            }
            bx += jump_width;
        }
    }

    /* Connection Gap between Ascension and Descent (160px gap) - Pooled */
    level_lava[current_lava++] = create_lava_rect(bx + 32, 32, 128); /* 160 - 32 = 128 */
    bx += 160;
    last_y = by; /* Start descent from the peak height */

    /* Test 3: Descending stairway with drift compensation (1st jump then every 3rd) */
    for(i = 0; i < NUM_STAIR_STEPS; i++) {
        next_y = last_y - ((i + 1) * 32);
        if (next_y < 64) next_y = 64; 
        
        level_blocks[current_block++] = create_block(bx, next_y);

        if (next_y > 64) {
             level_lava[current_lava++] = create_lava(bx, 32);
        }

        if (i < NUM_STAIR_STEPS - 1) {
            jump_width = 160;
            if (i == 0 || (i % 3 == 0)) {
                jump_width += 32; /* 1st jump (+1), then every 3rd jump (+1) */
            }
            
            if (jump_width > 32) {
                level_lava[current_lava++] = create_lava_rect(bx + 32, 32, jump_width - 32);
            }
            bx += jump_width;
        }
    }

    /* Fill remainder with dummy values to avoid garbage rendering */
    for(i = current_block; i < L1_BLOCKS_SIZE; i++) {
        level_blocks[i] = create_block(0, 0); 
    }
    for(i = current_spike; i < L1_SPIKES_SIZE; i++) {
        level_spikes[i] = create_spike(0, 0);
    }
    for(i = current_lava; i < L1_LAVA_SIZE; i++) {
        level_lava[i] = create_lava(0, 0);
    }

    /* Set actual sizes to matching indices to avoid dummy rendering overhead */
    return create_level(
        level_blocks,
        level_spikes,
        level_lava,
        current_block,
        current_spike,
        current_lava,
        30000
    );
}

Level *get_levels(void)
{
    static Level levels[NUM_LEVELS];

    levels[0] = get_level1();

    return levels;
}
