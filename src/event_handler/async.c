/*
 * async.c
 *
 * PURPOSE: Implements asynchronous (user input-triggered)
 *          event handlers. Currently handles jump requests.
 */

#include "async.h"

/*
 * on_jump_request
 *
 * PURPOSE: Handles the asynchronous jump event triggered by
 *          the spacebar. Only allows a jump when Geo is on
 *          a surface (dy == 0), preventing mid-air jumps.
 *
 * INPUT:   geo — pointer to the player Geo struct
 *
 * OUTPUT:  None (modifies geo in place via geo_jump)
 *
 * ASSUMPTIONS: geo is not NULL
 */
void on_jump_request(Geo *geo) { geo_jump(geo); }

/* FUTURE TODO (User Actions):
 * void on_pause_request(World *world);
 * void on_quit_request(World *world);
 */
