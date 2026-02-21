/*
 * cond.c
 *
 * PURPOSE: Implements conditional (collision/state-triggered)
 *          event handlers. Checks Geo against all level
 *          obstacles each frame using model functions.
 */

#include "cond.h"
#include "../dtypes.h"
#include "events.h"

/*
 * check_collisions
 *
 * PURPOSE: Iterates through all obstacles in the level
 *          and checks for collisions with Geo using the
 *          model's geo_check_square_collision logic.
 *
 * INPUT:   geo   — pointer to Geo
 *          level — pointer to current Level
 *
 * OUTPUT:  int — EVENT_DEATH (hazard or side collision)
 *                EVENT_LANDED (top of block collision)
 *                EVENT_NONE (no collision)
 */
int check_collisions(Geo *geo, const Level *level) {
  unsigned int i;
  signed int collision;

  /* Check spikes - any collision is death */
  for (i = 0; i < level->current_spike; i++) {
    collision = geo_check_square_collision(
        geo, level->spikes[i].x, level->spikes[i].y, level->spikes[i].size);
    if (collision != COLLISION_NONE) {
      return EVENT_DEATH;
    }
  }

  /* Check lava - any collision is death */
  for (i = 0; i < level->current_lava; i++) {
    collision = geo_check_square_collision(
        geo, level->lava[i].x, level->lava[i].y, level->lava[i].size);
    if (collision != COLLISION_NONE) {
      return EVENT_DEATH;
    }
  }

  /* Check blocks - top is safe/land, side/bottom is death */
  for (i = 0; i < level->current_block; i++) {
    collision = geo_check_square_collision(
        geo, level->blocks[i].x, level->blocks[i].y, level->blocks[i].size);

    if (collision == COLLISION_TOP) {
      /* Snap Geo to top of block and zero dy */
      geo->y = level->blocks[i].y + level->blocks[i].size;
      geo->dy = 0;
      geo->is_landed = TRUE;
      return EVENT_LANDED;
    } else if (collision == COLLISION_LEFT || collision == COLLISION_BOTTOM) {
      return EVENT_DEATH;
    }
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
  return EVENT_NONE;
  (void)floor_y; /* Ignore for now as model handles clamping */
}

/* FUTURE TODO/Nice to haves (State Transitions/Triggers):
 * void on_geo_death(World *world);    -- Stop music, play boom, start reset
 * timer void on_geo_victory(World *world);  -- Play cheer, trigger menu
 * transition void on_item_collision(Geo *geo, Item *item); if we decide to implement bonus coins, etc.
 */
