#ifndef BLOCK_H
#define BLOCK_H

#include "../shared/dtypes.h"

/* Global size of all blocks and their sprites */
#define BLOCK_SIZE 32

/**
 * Block entity which Geo can land on, or will die if colliding with the 
 * left or the bottom
 * 
 * unsigned int x:
 *     The x coordinate of the block in the world
 * unsigned int y:
 *     The y coordinate of the block in the world
 * unsigned int size:
 *     The size of the side of a block as a square and sprite
 * const unsigned int (*sprite)[BLOCK_SIZE / WORD]:
 *     A pointer to the block's sprite
*/
typedef struct {
    unsigned int x;
    unsigned int y;
    unsigned int size;  /* This is height */
    unsigned int width; /* This is horizontal width */
    const unsigned int (*sprite)[BLOCK_SIZE / WORD];
} Block;

/**
 * Creates a block with the given x and y coordinates, with the default size
 * and sprite
 * 
 * Params:
 *     unsigned int x:
 *         The x coordinate of the block in the world
 *     unsigned int y:
 *         The y coordinate of the block in the world
 * 
 * Returns:
 *    Block: Constructed object with respective x, y, default size, and sprite
 */
Block create_block(unsigned int x, unsigned int y);
Block create_block_rect(unsigned int x, unsigned int y, unsigned int width);

#endif
