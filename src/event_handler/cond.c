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
int check_collisions(World *world, unsigned int level_index) {
  unsigned int i;
  Level *level = &world->levels[level_index];
  Geo *geo = &world->geo;
  unsigned int geo_right = geo->x + geo->size;

  /* Check spikes - any collision is death */
  for (i = world->cam_min_si; i <= world->cam_max_si && i < level->spikes_size;
       i++) {
    /* Optimization: If obstacle is past Geo's reach, stop checking (sorted
     * list) */
    if (level->spikes[i].x > geo_right) {
      break;
    }
    /* Optimization: If obstacle is already behind Geo, skip it */
    if (level->spikes[i].x + level->spikes[i].size < geo->x) {
      continue;
    }

    world_collision_geo_spike(world, &level->spikes[i]);
    if (geo->is_dead == TRUE) {
      return EVENT_DEATH;
    }
  }

  /* Check lava - any collision is death */
  for (i = world->cam_min_li; i <= world->cam_max_li && i < level->lava_size;
       i++) {
    if (level->lava[i].x > geo_right) {
      break;
    }
    if (level->lava[i].x + level->lava[i].size < geo->x) {
      continue;
    }

    world_collision_geo_lava(world, &level->lava[i]);
    if (geo->is_dead == TRUE) {
      return EVENT_DEATH;
    }
  }

  /* Check blocks - top is safe/land, side/bottom is death */
  for (i = world->cam_min_bi; i <= world->cam_max_bi && i < level->blocks_size;
       i++) {
    if (level->blocks[i].x > geo_right) {
      break;
    }
    if (level->blocks[i].x + level->blocks[i].size < geo->x) {
      continue;
    }

    world_collision_geo_block(world, &level->blocks[i]);
    if (geo->is_dead == TRUE) {
      return EVENT_DEATH;
    }
    if (geo->is_landed == TRUE) {
      return EVENT_LANDED;
    }
  }

  /* Also check ground if not landed on block */
  world_collision_geo_ground(world);
  if (geo->is_landed == TRUE) {
    return EVENT_LANDED;
  }

  return EVENT_NONE;
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
int check_level_complete(const Geo *geo, const Level *level) {
  if (geo->x >= level->end_x) {
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
int check_floor(Geo *geo, int floor_y) {
  if (geo->is_landed == TRUE) {
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
