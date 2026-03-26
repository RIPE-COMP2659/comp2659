/**
 * block.c
 * Authors:
 * Riley Gramlich, rgram060@mtroyal.ca, 201762060
 * Robert Parker Hutcheson, rhutc335@mtroyal.ca, 201762335
 * Isaac Klein, iklei977@mtroyal.ca, 201763977
 * Eduard Mykhailets, emykh268@mtroyal.ca, 201750268
 * Course: COMP 2659-001, Computing Machinery II, Winter 2026
 * Instructor: Nolan Shaw
 *
 * PURPOSE: Block entity implementation and sprite data. Provides a
 * factory for creating `Block` instances used by levels.
 */
#include "block.h"

/* Global sprite, each Block will point here */
static const unsigned int BLOCK_SPRITE[BLOCK_SIZE][BLOCK_SIZE / WORD] = {
    /* Solid Top Border */
    {0xFFFF, 0xFFFF}, /* 1 */
    {0xFFFF, 0xFFFF},
    
    /* 75% Dark Dither */
    {0xEEEE, 0xEEEF},
    {0xFBBB, 0xBBBB},
    {0xEEEE, 0xEEEF},
    {0xFBBB, 0xBBBB},
    {0xEEEE, 0xEEEF},
    {0xFBBB, 0xBBBB}, /* 8 */
    
    /* 50% Mid-tone Dither (Checkerboard) */
    {0xEAAA, 0xAAAB},
    {0xD555, 0x5557},
    {0xEAAA, 0xAAAB},
    {0xD555, 0x5557},
    {0xEAAA, 0xAAAB},
    {0xD555, 0x5557},
    {0xEAAA, 0xAAAB},
    {0xD555, 0x5557}, /* 16 */
    
    /* 25% Light Dither */
    {0xC888, 0x888B},
    {0xE222, 0x2223},
    {0xC888, 0x888B},
    {0xE222, 0x2223},
    {0xC888, 0x888B},
    {0xE222, 0x2223},
    {0xC888, 0x888B},
    {0xE222, 0x2223}, /* 24 */
    
    /* 12.5% to 0% (Highlight) */
    {0xC000, 0x0003},
    {0xC808, 0x080B},
    {0xC000, 0x0003},
    {0xC808, 0x080B},
    {0xC000, 0x0003},
    {0xC000, 0x0003},
    
    /* Solid Bottom Border */
    {0xFFFF, 0xFFFF},
    {0xFFFF, 0xFFFF} /* 32 */
};

Block create_block(unsigned int x, unsigned int y)
{
    Block block;
    block.x = x;
    block.y = y;
    block.size = BLOCK_SIZE;
    block.sprite = BLOCK_SPRITE;
    return block;
};
