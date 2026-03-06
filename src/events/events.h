/*
 * events.h
 *
 * PURPOSE: Umbrella header for the event handler module.
 *          Includes all three event submodules and defines
 *          shared event return codes.
 */

#ifndef EVENTS_H
#define EVENTS_H

/* Event return codes used by conditional event handlers */
#define EVENT_ERROR -1
#define EVENT_NONE 0
#define EVENT_DEATH 1
#define EVENT_LANDED 2
#define EVENT_LEVEL_DONE 3
#define EVENT_OFF_PLATFORM 4

#include "async.h"
#include "sync.h"
#include "cond.h"
#include "sync.h"

#endif
