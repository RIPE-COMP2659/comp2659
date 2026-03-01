#ifndef SYNCH_H
#define SYNCH_H

#include "../entities/geo.h"
#include "../entities/level.h"
#include "../entities/world.h"

/*
 * on_clock_tick
 *
 * PURPOSE: Called once per frame (1/70th of a second).
 *          Advances the game state:
 *            1. Updates Geo's position and velocity
 *            2. Updates Camera based on new position
 *            3. Checks for collisions
 *
 * INPUT:   world — pointer to the World object
 *          level_index - index of current Level
 *
 * OUTPUT:  None (modifies world in place)
 *
 * ASSUMPTIONS: world is not NULL
 */
void on_clock_tick(World *world, unsigned int level_index);

#endif
