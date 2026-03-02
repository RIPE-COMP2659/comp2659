/*
 * cond.h
 *
 * PURPOSE: Prototypes for conditional (collision/state-triggered)
 *          event handlers. These check object interactions
 *          each frame and return event codes.
 */

#ifndef COND_H
#define COND_H

#include "../model/geo.h"
#include "../model/level.h"
#include "../model/world.h"

/* Default ground height (bottom of the screen) */
#define GROUND_Y 0

/*
 * check_collisions
 *
 * PURPOSE: Checks Geo against all obstacles in the level
 *          (blocks, spikes, lava). Determines whether a
 *          collision results in death or a safe landing.
 *          Uses camera indices for block tracking/optimisation.
 *
 * INPUT:   world — pointer to the World object
 *          level_index - index of current Level
 *
 * OUTPUT:  int — EVENT_DEATH if Geo hits a hazard or
 *                  side/bottom of block
 *                EVENT_LANDED if Geo lands on top of block
 *                EVENT_NONE if no collision
 */
int check_collisions(World *world, unsigned int level_index);

/*
 * check_level_complete
 *
 * PURPOSE: Checks if Geo has reached the end of the level.
 *
 * INPUT:   geo   — pointer to Geo
 *          level — pointer to current Level
 *
 * OUTPUT:  int — EVENT_LEVEL_DONE if Geo's x >= level end_x,
 *                EVENT_NONE otherwise
 */
int check_level_complete(const Geo *geo, const Level *level);

/*
 * check_floor
 *
 * PURPOSE: Checks if Geo has fallen to or below the given
 *          floor height. If so, clamps y to the floor and
 *          sets dy to 0 (Geo has landed).
 *
 * INPUT:   geo     — pointer to Geo (may be modified)
 *          floor_y — current floor height (ground or
 *                    top of a block)
 *
 * OUTPUT:  int — EVENT_LANDED if Geo was at or below
 *                  floor and was clamped,
 *                EVENT_NONE otherwise
 */
int check_floor(Geo *geo, int floor_y);

#endif
