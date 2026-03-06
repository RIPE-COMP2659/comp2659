/*
 * cond.c
 *
 * PURPOSE: Implements conditional (collision/state-triggered)
 *          event handlers. Checks Geo against level
 *          obstacles using model functions and camera indices.
 */

#include "cond.h"
#include "../model/dtypes.h"
#include "events.h"

/*
 * check_collisions
 *
 * PURPOSE: Iterates through obstacles in the level that
 *          are currently on-screen (using camera indices)
 *          and checks for collisions with Geo.
 *
 * INPUT:   world — pointer to the World object
 *          level_index - index of current Level
 *
 * OUTPUT:  int — EVENT_DEATH if Geo hits a hazard or
 *                  side/bottom of block
 *                EVENT_LANDED if Geo lands on top of block
 *                EVENT_NONE if no collision
 */
int check_collisions(World *world, unsigned int level_index)
{
    unsigned int i;
    Level *level = &world->levels[level_index];
    Geo *geo = &world->geo;
    unsigned int geo_x = geo->x;
    unsigned int geo_right = geo_x + geo->size;
    int event = EVENT_ERROR;
    unsigned int cam_max;
    unsigned int arr_max;

    /* Check spikes - Optimized to avoid dereference, higher memory, faster speed
     */
    cam_max = world->cam_max_si;
    arr_max = level->spikes_size;

    for (i = world->cam_min_si;
         i <= cam_max && i < arr_max && event != EVENT_DEATH &&
         level->spikes[i].x <= geo_right;
         i++)
    {
        Spike spike = level->spikes[i];

        if (spike.x + spike.size >= geo_x)
        {
            world_collision_geo_spike(world, &spike);
            if (geo->is_dead == TRUE)
            {
                event = EVENT_DEATH;
            }
        }
    }

    /* Check lava - same optimized loop */
    cam_max = world->cam_max_li;
    arr_max = level->lava_size;

    for (i = world->cam_min_li;
         i <= cam_max && i < arr_max && event != EVENT_DEATH &&
         level->lava[i].x <= geo_right;
         i++)
    {
        Lava lava = level->lava[i];

        if (lava.x + lava.size >= geo_x)
        {
            world_collision_geo_lava(world, &lava);
            if (geo->is_dead == TRUE)
            {
                event = EVENT_DEATH;
            }
        }
    }

    /* Check blocks - same optimized loop */
    cam_max = world->cam_max_bi;
    arr_max = level->blocks_size;

    for (i = world->cam_min_bi;
         i <= cam_max && i < arr_max && event != EVENT_DEATH &&
         level->blocks[i].x <= geo_right;
         i++)
    {
        Block block = level->blocks[i];

        if (block.x + block.size >= geo_x)
        {
            world_collision_geo_block(world, &block);
            if (geo->is_dead == TRUE)
            {
                event = EVENT_DEATH;
            }
            else if (geo->is_landed == TRUE)
            {
                event = EVENT_LANDED;
            }
        }
    }

    /* Final resolution: check ground if no collision event was finalized */
    if (event == EVENT_ERROR || event == EVENT_NONE)
    {
        world_collision_geo_ground(world);
        if (geo->is_landed == TRUE)
        {
            event = EVENT_LANDED;
        }
        else
        {
            event = EVENT_NONE;
        }
    }

    return event;
}

/*
 * check_level_complete
 *
 * PURPOSE: Checks whether Geo has reached or passed
 *          the end-x coordinate of the level.
 *
 * INPUT:   geo   — pointer to Geo
 *          level — pointer to current Level
 *
 * OUTPUT:  int — EVENT_LEVEL_DONE or EVENT_NONE
 */
int check_level_complete(const Geo *geo, const Level *level)
{
    if (geo->x >= level->end_x)
    {
        return EVENT_LEVEL_DONE;
    }
    return EVENT_NONE;
}

/*
 * check_floor
 *
 * PURPOSE: Checks if Geo is on the ground.
 *
 * OUTPUT:  int — EVENT_LANDED if geo->is_landed is TRUE
 */
int check_floor(Geo *geo, int floor_y)
{
    if (geo->is_landed == TRUE)
    {
        return EVENT_LANDED;
    }
    (void)floor_y;
    return EVENT_NONE;
}

/* FUTURE TODO/Nice to haves (State Transitions/Triggers):
 * void on_geo_death(World *world);    -- Stop music, play boom, start reset
 * timer void on_geo_victory(World *world);  -- Play cheer, trigger menu
 * transition void on_item_collision(Geo *geo, Item *item); if we decide to
 * implement bonus coins, etc.
 */