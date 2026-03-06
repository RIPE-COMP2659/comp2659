/*
 * sync.c
 *
 * PURPOSE: Implements synchronous (clock-triggered) event
 *          handlers, called once per frame (1/70th second).
 */
#include "sync.h"

void on_clock_tick(Model *model) { model_update(model); }
