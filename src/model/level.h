#include "block.h"
#include "lava.h"
#include "spike.h"

/* TODO: Investigate turning this into constants, same goes for other
   objects */

/**
 * Represents a single level of the game comprised of Blocks, Spikes, and Lava
 * 
 * Block *blocks:
 *     An array of blocks in the level
 * Spike *spikes:
 *     An array of spikes in the level
 * Lava *lava:
 *     An array of lava in the level
 * unsigned int blocks_size:
 *     The number of blocks in the level
 * unsigned int spikes_size:
 *     The number of spikes in the level
 * unsigned int lava_size:
 *     The number of lava in the level
 * unsigned int end_x:
 *     The x-coordinate of the end of the level, once reached the player wins
 */
typedef struct {
    Block *blocks;
    Spike *spikes;
    Lava *lava;
    unsigned int blocks_size;
    unsigned int spikes_size;
    unsigned int lava_size;
    unsigned int end_x;
} Level;

/**
 * Creates a level with the given blocks, spikes, lava, and end_x
 * 
 * Params:
 *     Block *blocks:
 *         An array of blocks in the level
 *     Spike *spikes:
 *         An array of spikes in the level
 *     Lava *lava:
 *         An array of lava in the level
 *     unsigned int blocks_size:
 *         The number of blocks in the level
 *     unsigned int spikes_size:
 *         The number of spikes in the level
 *     unsigned int lava_size:
 *         The number of lava in the level
 *     unsigned int end_x:
 *         The x-coordinate of the end of the level, once reached the player wins
 * 
 * Returns:
 *     Level: A level struct with the given blocks, spikes, lava, and end_x
 */
Level create_level(
    Block *blocks,
    Spike *spikes,
    Lava *lava,
    unsigned int blocks_size,
    unsigned int spikes_size,
    unsigned int lava_size,
    unsigned int end_x
);

/**
 * Returns the first level of the game
 * 
 * Returns:
 *     Level: The first level of the game
 */
Level get_level1(void);

/**
 * Returns the second level of the game
 * 
 * Returns:
 *     Level: The second level of the game
 */
Level get_level2(void);

/**
 * Returns the array of levels
 * 
 * Returns:
 *    Level*: The array of levels
 */
Level* get_levels(void);
