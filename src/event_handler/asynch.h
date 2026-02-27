/*
 * asynch.h
 *
 * PURPOSE: Prototypes for asynchronous (user input-triggered)
 *          event handlers.
 */

#ifndef ASYNCH_H
#define ASYNCH_H

#include "../geo.h"

/*
 * on_jump_request
 *
 * PURPOSE: Handles the asynchronous jump event triggered by
 *          the spacebar. Only allows a jump when Geo is on
 *          a surface (dy == 0).
 *
 * INPUT:   geo — pointer to the player Geo struct
 *
 * OUTPUT:  None (modifies geo in place via geo_jump)
 */
void on_jump_request(Geo *geo);

#endif
