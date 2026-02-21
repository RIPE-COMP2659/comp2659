/*
 * synch.c
 *
 * PURPOSE: Implements synchronous (clock-triggered) event
 *          handlers, called once per frame (1/70th second).
 */

#include "synch.h"

/*
 * on_clock_tick
 *
 * PURPOSE: Called once per frame (1/70th of a second).
 *          Advances Geo's position by delegating to
 *          geo_move(), which updates:
 *            x  += dx   (constant horizontal movement)
 *            dy += ddy  (gravity applied to vertical velocity)
 *            y  += dy   (vertical position updated)
 *
 * INPUT:   geo   — pointer to the player Geo struct
 *          level — pointer to the current Level
 *
 * OUTPUT:  None (modifies geo in place)
 *
 * ASSUMPTIONS: geo and level are not NULL
 *
 * NOTE:    The level parameter is included for future use
 *          (e.g., scrolling obstacles into view based on
 *          Geo's world-x position).
 */
void on_clock_tick(Geo *geo, Level *level) {
  geo_move(geo);
  (void)level; /* suppress unused-parameter warning for now */
}

/* FUTURE TODO (Frame-based Updates):
 * void update_animations(World *world); geo rotation, coin rotation, etc.
 * void update_camera(World *world); camera follows geo
 * void update_scrolling_background(World *world); background scrolls with geo or objects move towards geo
 */
