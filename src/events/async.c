/**
 * async.c
 * Authors:
 *     Riley Gramlich, rgram060@mtroyal.ca, 201762060
 *     Robert Parker Hutcheson, rhutc335@mtroyal.ca, 201762335
 *     Isaac Klein, iklei977@mtroyal.ca, 201763977
 *     Eduard Mykhailets, emykh268@mtroyal.ca, 201750268
 * Course: COMP 2659-001, Computing Machinery II, Winter 2026
 * Instructor: Nolan Shaw
 *
 * PURPOSE: Implements asynchronous (user input-triggered)
 *          event handlers for the game. Currently handles jump
 *          requests originating from input wrappers.
 *
 * Assumptions:
 *  - `Model` and related types are declared in the game `model` headers.
 *  - This file should contain only small, single-purpose handlers.
 */
#include "async.h"

void on_jump_request(Model *model) { geo_jump(&model->world.geo); }

/* FUTURE TODO (User Actions):
 * void on_pause_request(World *world);
 * void on_quit_request(World *world);
 */
