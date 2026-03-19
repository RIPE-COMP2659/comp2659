/*
 * cond.h
 *
 * PURPOSE: Prototypes for conditional (collision/state-triggered)
 *          event handlers. These check object interactions
 *          each frame and return event codes.
 */
#include "../model/model.h"

#define EVENT_ERROR -1
#define EVENT_NONE 0
#define EVENT_DEATH 1
#define EVENT_LEVEL_DONE 2

/**
 * Checks if Geo has reached the end of the level.
 *
 * Params:
 *     Model *model:
 *         Pointer to the game model containing world and level data
 *
 * Returns:
 *     signed int:
 *         EVENT_LEVEL_DONE if Geo's x >= level.end_x, or EVENT_NONE otherwise
 */
signed int check_level_complete(Model *model);
