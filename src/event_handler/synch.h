/*
 * synch.h
 *
 * PURPOSE: Prototypes for synchronous (clock-triggered)
 *          event handlers, called once per frame.
 */

#ifndef SYNCH_H
#define SYNCH_H

#include "../geo.h"
#include "../level.h"

/*
 * on_clock_tick
 *
 * PURPOSE: Called once per frame (1/70th of a second).
 *          Advances Geo's position and applies gravity
 *          by delegating to geo_move().
 *
 * INPUT:   geo   — pointer to the player Geo struct
 *          level — pointer to the current Level (for
 *                  future obstacle scrolling logic)
 *
 * OUTPUT:  None (modifies geo in place)
 *
 * ASSUMPTIONS: geo and level are not NULL.
 */
void on_clock_tick(Geo *geo, Level *level);

#endif
