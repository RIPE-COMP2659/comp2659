/*
 * sync.c
 *
 * PURPOSE: Implements synchronous (clock-triggered) event
 *          handlers, called once per frame (1/70th second).
 */

#include "sync.h"
#include "cond.h"

/*
 * on_clock_tick
 *
 * PURPOSE: Called once per frame (1/70th of a second).
 *          Updates Geo's position, camera view, and
 *          checks for collisions.
 *
 * INPUT:   world — pointer to the World object
 *          level_index - index of current Level
 *
 * OUTPUT:  None (modifies world in place)
 *
 * ASSUMPTIONS: world is not NULL
 */
void on_clock_tick(World *world, unsigned int level_index)
{
    /* 1. Update Geo's position and velocity */
    geo_update(&world->geo);

    /* 2. Update camera position and entity visibility indices */
    world_update_camera(world, level_index);

    /* 3. Check for collisions using tracked block boundaries */
    check_collisions(world, level_index);
}
