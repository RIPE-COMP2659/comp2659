/*
 * async.c
 *
 * PURPOSE: Implements asynchronous (user input-triggered)
 *          event handlers. Currently handles jump requests.
 */
#include "async.h"

void on_jump_request(Model *model) {geo_jump(&model->world.geo); }

/* FUTURE TODO (User Actions):
 * void on_pause_request(World *world);
 * void on_quit_request(World *world);
 */
