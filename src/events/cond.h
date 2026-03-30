#ifndef COND_H
#define COND_H

#include "../model/model.h"
#include "../psg/effects.h"

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

/**
 * Triggered when Geo dies. Plays the death sound effect.
 */
void on_geo_death(Model *model);

#endif
