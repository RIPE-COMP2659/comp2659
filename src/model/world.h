#ifndef WORLD_H
#define WORLD_H

#include "camera.h"
#include "geo.h"
#include "level.h"

/**
 * The game world, which keeps track of entities, their interactions, and
 * their states. However, it does not manage the larger game state or logic
 * like collision indices, which would live in the model.
 *
 * Level *levels:
 *     Array of levels in the game, which contain blocks, spikes, and lava
 * Geo geo:
 *     The player character
 * Camera camera:
 *     The camera that follows the player character
 * unsigned int ground_y:
 *     The y coordinate of the ground, which the player can land on without
 *     dying, and is used as the default ground when not landing on blocks
 * unsigned int level_index:
 *     The index of the current level in the levels array
 * unsigned int levels_size:
 *     The size of the levels array, used for bounds checking
 */
typedef struct {
    /* TODO: Turn these into constants somehow */
    Level *levels;
    Geo geo;
    unsigned int ground_y;
    Camera camera;
    unsigned int level_index;
    unsigned int levels_size;
} World;

/**
 * Creates a world with the given levels, geo, and ground_y, with default
 * values for the camera, level_index, and levels_size
 *
 * Params:
 *     Level *levels:
 *        The levels in the world
 *    Geo geo:
 *       The player character in the world
 *    unsigned int ground_y:
 *      The y coordinate of the ground in the world
 */
World create_world(Level *levels, Geo geo, unsigned int ground_y);

/**
 * Get the default world
 *
 * Params:
 *     None
 *
 * Returns
 *     World: The default world with default levels, geo, and ground_y
 */
World get_world(void);

/**
 * Resets the current level by resetting geo's position to the start of the
 * level and setting is_dead to false. Note that this does not reset the
 * camera, as it will be updated on the next world_update call, and does not
 * reset any blocks, spikes, or lava, as they are static in our current levels
 *
 * Params:
 *     World *world:
 *         The world to reset the level in
 */
void world_reset_level(World *world);

/**
 * Updates the world by updating the geo and camera coordinates, does not
 * check for collisions, just physics for moving the camera and geo.
 * Notably, we need to check for collisions before checking with the ground
 * so the procedure is to update positions, check collisions, then update
 * the ground
 *
 * Params:
 *     World *world: The world to update
 */
void world_update(World *world);

/**
 * Updates the world for collisions by checking the geo against all blocks,
 * spikes, and lava in the current level, and updating geo's state. Note that
 * min and max values are not checked for bounds and should be set prior to
 * calling
 *
 * Params:
 *     World *world:
 *         The world to update collisions for
 *     unsigned int block_min:
 *         The minimum index of blocks to check for collisions
 *     unsigned int block_max:
 *         The maximum index of blocks to check for collisions
 *     unsigned int spike_min:
 *         The minimum index of spikes to check for collisions
 *     unsigned int spike_max:
 *         The maximum index of spikes to check for collisions
 *     unsigned int lava_min:
 *         The minimum index of lava to check for collisions
 *     unsigned int lava_max:
 *         The maximum index of lava to check for collisions
 */
void world_update_collisions(
    World *world,
    unsigned int block_min,
    unsigned int block_max,
    unsigned int spike_min,
    unsigned int spike_max,
    unsigned int lava_min,
    unsigned int lava_max
);

/**
 * Helper function to check for collisions between geo and blocks.
 * Will update Geo's state accordingly. Note, that min and max are intended
 * to have been set according to the array size ahead of time, and will not
 * be checked for bounds within this function
 *
 * Params:
 *     World *world:
 *         The world to check for collisions in
 *     Block *blocks:
 *         The block array to check for collisions with
 *     unsigned int block_min:
 *         The minimum index of blocks to check for collisions
 *     unsigned int block_max:
 *         The maximum index of blocks to check for collisions
 */
void world_collisions_geo_blocks(
    World *world,
    Block *blocks,
    unsigned int block_min,
    unsigned int block_max
);

/**
 * Helper function to check for collisions between geo and spikes.
 * Will update Geo's state accordingly. Note, that min and max are intended
 * to have been set according to the array size ahead of time, and will not
 * be checked for bounds within this function
 *
 * Params:
 *     World *world:
 *         The world to check for collisions in
 *     Spike *spikes:
 *         The spike array to check for collisions with
 *     unsigned int spike_min:
 *         The minimum index of spikes to check for collisions
 *     unsigned int spike_max:
 *         The maximum index of spikes to check for collisions
 */
void world_collisions_geo_spikes(
    World *world,
    Spike *spikes,
    unsigned int spike_min,
    unsigned int spike_max
);

/**
 * Helper function to check for collisions between geo and lava.
 * Will update Geo's state accordingly. Note, that min and max are intended
 * to have been set according to the array size ahead of time, and will not
 * be checked for bounds within this function
 *
 * Params:
 *     World *world:
 *         The world to check for collisions in
 *     Lava *lava:
 *         The lava array to check for collisions with
 *     unsigned int lava_min:
 *         The minimum index of lava to check for collisions
 *     unsigned int lava_max:
 *         The maximum index of lava to check for collisions
 */
void world_collisions_geo_lava(
    World *world,
    Lava *lava,
    unsigned int lava_min,
    unsigned int lava_max
);

/**
 * Helper function to check for collisions between geo and a single block.
 * Will update Geo's state accordingly.
 *
 * Params:
 *     World *world:
 *         The world to check for collisions in
 *     Block *block:
 *         The block to check for collisions with
 */
void world_collision_geo_block(World *world, Block *block);

/**
 * Helper function to check for collisions between geo and a single spike.
 * Will update Geo's state accordingly.
 *
 * Params:
 *     World *world:
 *         The world to check for collisions in
 *     Spike *spike:
 *         The spike to check for collisions with
 */
void world_collision_geo_spike(World *world, Spike *spike);

/**
 * Helper function to check for collisions between geo and a single lava.
 * Will update Geo's state accordingly.
 *
 * Params:
 *     World *world:
 *         The world to check for collisions in
 *     Lava *lava:
 *         The lava to check for collisions with
 */
void world_collision_geo_lava(World *world, Lava *lava);

/**
 * Helper function to check for collisions between geo and the ground.
 * Will update Geo's state accordingly.
 *
 * Params:
 *     World *world:
 *         The world to check for collisions in
 */
void world_collision_geo_ground(World *world);

#endif
