#ifndef EFFECTS_H
#define EFFECTS_H

#include "../shared/dtypes.h"
#include "psg.h"

/**
 * Plays the death (crash) sound effect using the PSG envelope generator.
 * This is triggered by a synchronous event (collision).
 */
void play_death_effect(void);

/**
 * Plays the level complete sound effect using the PSG envelope generator.
 */
void play_level_complete_effect(void);
void play_jump_effect(void);

#endif
