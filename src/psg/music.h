#ifndef MUSIC_H
#define MUSIC_H

/* Use project-wide fixed-width types */
#include "../shared/dtypes.h"

#define CHANNEL_A 0
#define CHANNEL_B 1
#define CHANNEL_C 2

typedef struct {
    unsigned int pitch;    /* Note frequency divider (0 = Rest) */
    unsigned int duration; /* Duration in 70Hz VBL ticks */
} Note;

/* Enum to easily swap songs in start_music() */
typedef enum {
    SONG_BOURREE,
    SONG_TETRIS
    , SONG_HOT_CROSS_BUNS
    , SONG_GLORIA
} SongChoice;

/* Updated to take a SongChoice enum */
void start_music(SongChoice song);

/* Advances the notes based on VBL ticks elapsed */
void update_music(UINT32 time_elapsed);

/* Halts the PSG output */
void stop_music(void);

#endif