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
#define NUM_LEVELS 2

/* Level one (consistently using L2 sizes for current draft) */
#define L1_BLOCKS_SIZE 400
#define L1_SPIKES_SIZE 100
#define L1_LAVA_SIZE 100
#define NUM_STAIR_STEPS 12

static void add_block_rect(
    Block *blocks,
    int *current_block,
    unsigned int x,
    unsigned int y,
    unsigned int width)
{
    unsigned int offset;

    for (offset = 0; offset < width; offset += BLOCK_SIZE)
    {
        blocks[*current_block] = create_block(x + offset, y);
        (*current_block)++;
    }
}

static void add_lava_rect(
    Lava *lava,
    int *current_lava,
    unsigned int x,
    unsigned int y,
    unsigned int width)
{
    unsigned int offset;

    for (offset = 0; offset < width; offset += LAVA_SIZE)
    {
        lava[*current_lava] = create_lava(x + offset, y);
        (*current_lava)++;
    }
}

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
    int current_block = 0;
    int current_lava = 0;
    int current_spike = 0;
    unsigned int bx, by;

    /* --- INITIALIZATION --- */
    /* Starting path: Empty floor stretch for 14 blocks (448px) */
    /* Ground line is implicitly at 32. Blocks placed at 64 represent ground. */

    /* --- SEGMENT 1: WARMUP SPIKES --- */
    /* Description: Intro Double Spike */
    level_spikes[current_spike++] = create_spike(600, 64);
    level_spikes[current_spike++] = create_spike(632, 64);

    /* Description: Intro Triple Spike */
    level_spikes[current_spike++] = create_spike(1200, 64);
    level_spikes[current_spike++] = create_spike(1232, 64);
    level_spikes[current_spike++] = create_spike(1264, 64);

    /* --- SEGMENT 2: PLATFORMING INTRODUCTION --- */
    /* Description: Low-height platforms with 1-block slide room */
    add_block_rect(level_blocks, &current_block, 1600, 96, 128); /* 4 blocks at y=96 */
    add_block_rect(level_blocks, &current_block, 1856, 96, 128); /* Next platform at y=96 */
    add_lava_rect(level_lava, &current_lava, 1728, 32, 128);   /* Lava gap */

    /* --- SEGMENT 3: THE FALL (3 RIGHT, 1 DOWN) --- */
    /* Description: Elevated platform followed by the specific fall jump */
    /* Jumped to via Section 2 platform 2 (x=1984 end) using 5-right, 1-up (160px) */
    add_block_rect(level_blocks, &current_block, 2104, 128, 128); 
    /* Gap of 3 blocks (96px) leading to lower ground: (2144+128+96 = 2368) */
    add_block_rect(level_blocks, &current_block, 2296, 96, 32); 
    add_lava_rect(level_lava, &current_lava, 2328, 32, 96);

    /* --- SEGMENT 4: THE BIG STAIRCASE ASCENSION --- */
    /* Description: 12-block long ladder up using 4-right 1-up spacing */
    /* Drift compensation: Add 1 block (32px) to the gap every 4th jump. */
    bx = 3000;
    for(i = 0; i < NUM_STAIR_STEPS + 1; i++) {
        unsigned int jump_width = 128+58;
        by = 64 + (i * 32);
        level_blocks[current_block++] = create_block(bx, by);
        if (by > 64) level_lava[current_lava++] = create_lava(bx, 32);
        bx += jump_width;
    }

    /* Description: Peak platform for a brief breather */
    add_block_rect(level_blocks, &current_block, bx, by, 320); 
    bx += 320+170;

    /* --- SEGMENT 5: STAIRCASE DESCENT --- */
    /* Description: 12-block stairway down using 5-right 1-down spacing */
    /* Calculated drift: 170px jump width for -32y jump */
    for(i = 0; i < NUM_STAIR_STEPS; i++) {
        by = by - 32;
        if (by < 64) by = 64;
        level_blocks[current_block++] = create_block(bx, by);
        if (by > 64) level_lava[current_lava++] = create_lava(bx, 32);
        bx += 170; 
    }

    /* --- SEGMENT 6: THE QUAD-PLATFORM SPIKE GAUNTLET --- */
    /* Description: 7 platforms, 4 blocks each, spike on 4th block. 
       Requires jumping off 3rd block (x+64) to clear spike + gap.
       Jump distance used: 136px (0 drift). Next platform starts at x + 64 + 136. */
    bx += 500; /* Spacing before challenge */
    by = 96;   /* Initial platform height */
    for(i = 0; i < 7; i++) {
        add_block_rect(level_blocks, &current_block, bx, by, 128);
        level_spikes[current_spike++] = create_spike(bx + 96, by + 32);
        /* Shift bx by 200 (128 width + 72 gap) to achieve 136 jump from x+64 */
        bx += 200;
        by += 32; /* Next platform is 1 block higher */
    }

    /* --- SEGMENT 7: FALL-JUMP STAIRCASE --- */
    /* Description: Alternating cycle of Falling (64px) and Jumping (170px) */
    add_block_rect(level_blocks, &current_block, bx, by, 640); /* Long bridge */
    bx += 640; /* End of bridge */
    
    while (by > 64) {
        /* Step 1: Fall 1 block */
        bx += 72;
        by -= 32; 
        level_blocks[current_block++] = create_block(bx, by);

        if (by <= 64) break;

        /* Step 2: Jump 1 block */
        bx += 170;
        by -= 32;
        level_blocks[current_block++] = create_block(bx, by);
    }
    add_lava_rect(level_lava, &current_lava, bx - 288, 32, 288);

    /* Final Spikes: 4 block gap (128px) after last ground block */
    bx += 32 + 128; 
    level_spikes[current_spike++] = create_spike(bx, 64);
    level_spikes[current_spike++] = create_spike(bx + 32, 64);
    bx += 500;
    level_spikes[current_spike++] = create_spike(bx, 64);
    level_spikes[current_spike++] = create_spike(bx + 32, 64);
    level_spikes[current_spike++] = create_spike(bx + 64, 64);

    /* --- CLEANUP & END --- */
    for(i = current_block; i < L1_BLOCKS_SIZE; i++) {
        level_blocks[i] = create_block(0, 0); 
    }
    for(i = current_spike; i < L1_SPIKES_SIZE; i++) {
        level_spikes[i] = create_spike(0, 0);
    }
    for(i = current_lava; i < L1_LAVA_SIZE; i++) {
        level_lava[i] = create_lava(0, 0);
    }

    return create_level(
        level_blocks,
        level_spikes,
        level_lava,
        current_block,
        current_spike,
        current_lava,
        11600
    );
}

Level get_level2(void)
{
    static Block level_blocks[L1_BLOCKS_SIZE];
    static Spike level_spikes[L1_SPIKES_SIZE];
    static Lava level_lava[L1_LAVA_SIZE];
    int current_block = 0;
    int current_lava = 0;
    int current_spike = 0;
    int i;

    add_block_rect(level_blocks, &current_block, 0, 64, 512);
    add_lava_rect(level_lava, &current_lava, 512, 32, 96);
    level_spikes[current_spike++] = create_spike(640, 64);
    level_spikes[current_spike++] = create_spike(672, 64);
    add_block_rect(level_blocks, &current_block, 800, 96, 160);
    add_lava_rect(level_lava, &current_lava, 960, 32, 160);
    add_block_rect(level_blocks, &current_block, 1200, 64, 256);

    for (i = current_block; i < L1_BLOCKS_SIZE; i++) {
        level_blocks[i] = create_block(0, 0);
    }
    for (i = current_spike; i < L1_SPIKES_SIZE; i++) {
        level_spikes[i] = create_spike(0, 0);
    }
    for (i = current_lava; i < L1_LAVA_SIZE; i++) {
        level_lava[i] = create_lava(0, 0);
    }

    return create_level(
        level_blocks,
        level_spikes,
        level_lava,
        current_block,
        current_spike,
        current_lava,
        2200
    );
}

Level *get_levels(void)
{
    static Level levels[NUM_LEVELS];

    levels[0] = get_level1();
    levels[1] = get_level2();

    return levels;
}
