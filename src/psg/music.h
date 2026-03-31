/**
 * music.h
 * Authors:
 *     Riley Gramlich, rgram060@mtroyal.ca, 201762060
 *     Robert Parker Hutcheson, rhutc335@mtroyal.ca, 201762335
 *     Isaac Klein, iklei977@mtroyal.ca, 201763977
 *     Eduard Mykhailets, emykh268@mtroyal.ca, 201750268
 * Course: COMP 2659-001, Computing Machinery II, Winter 2026
 * Instructor: Nolan Shaw
 *
 * The music management for the game. Namely, the collection of notes and
 * timing
 */

/* TODO: Function definition template */

#include "../shared/dtypes.h"
#include "psg.h"
#include <osbind.h>

typedef struct {
    unsigned int pitch;    /* Note frequency divider (0 = Rest) */
    unsigned int duration; /* Duration in 70Hz VBL ticks */
} Note;

/* Enum to easily swap songs in start_music() */
typedef enum {
    SONG_BOURREE,
<<<<<<< HEAD
    SONG_TETRIS,
    SONG_HOT_CROSS_BUNS,
    SONG_GLORIA
=======
    SONG_TETRIS
    , SONG_HOT_CROSS_BUNS
    , SONG_GLORIA
    , SONG_MOUNTAIN_KING
    , SONG_MEGALO
>>>>>>> 865e22f (add megalo tune)
} SongChoice;

/* Updated to take a SongChoice enum */
void start_music(SongChoice song);

/* Advances the notes based on VBL ticks elapsed */
void update_music(UINT32 time_elapsed);

/* Halts the PSG output */
void stop_music(void);
