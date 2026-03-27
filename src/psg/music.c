#include "music.h"
#include "notes.h"
#include "psg.h"
#include <osbind.h>

#define EIGHTH 20
#define QUARTER 40
#define HALF 80
#define REST 0

/* --- BOURREE TRACK DATA --- */
static const Note bourree_melody[] = {
    /* Pickup */
    {E5, EIGHTH}, {F5S, EIGHTH},
    /* Bar 1 */
    {G5, EIGHTH}, {F5S, EIGHTH}, {G5, EIGHTH}, {A5, EIGHTH}, {B5, QUARTER}, {E5, QUARTER},
    /* Bar 2 */
    {F5S, EIGHTH}, {G5, EIGHTH}, {A5, EIGHTH}, {B5, EIGHTH}, {C6, EIGHTH}, {B5, EIGHTH}, {C6, EIGHTH}, {D6, EIGHTH},
    /* Bar 3 */
    {E6, EIGHTH}, {D6, EIGHTH}, {C6, EIGHTH}, {B5, EIGHTH}, {A5, EIGHTH}, {G5, EIGHTH}, {F5S, EIGHTH}, {E5, EIGHTH},
    /* Bar 4 */
    {D5S, QUARTER}, {F5S, QUARTER}, {B4, HALF},
    /* Bar 5 (Repeat) */
    {E5, EIGHTH}, {F5S, EIGHTH},
    {G5, EIGHTH}, {F5S, EIGHTH}, {G5, EIGHTH}, {A5, EIGHTH}, {B5, QUARTER}, {E5, QUARTER},
    /* Bar 6 */
    {F5S, EIGHTH}, {G5, EIGHTH}, {A5, EIGHTH}, {B5, EIGHTH}, {C6, EIGHTH}, {B5, EIGHTH}, {C6, EIGHTH}, {D6, EIGHTH},
    /* Bar 7 */
    {G5, EIGHTH}, {A5, EIGHTH}, {B5, EIGHTH}, {A5, EIGHTH}, {G5, EIGHTH}, {F5S, EIGHTH}, {E5, EIGHTH}, {D5S, EIGHTH},
    /* Bar 8 */
    {E5, HALF + QUARTER}, {REST, QUARTER}
};
static const int BOURREE_LEN = sizeof(bourree_melody) / sizeof(Note);

/* --- TETRIS (Korobeiniki) TRACK DATA --- */
static const Note tetris_melody[] = {
    /* Bar 1 */
    {E5, QUARTER}, {B4, EIGHTH}, {C5, EIGHTH}, {D5, QUARTER}, {C5, EIGHTH}, {B4, EIGHTH},
    /* Bar 2 */
    {A4, QUARTER}, {A4, EIGHTH}, {C5, EIGHTH}, {E5, QUARTER}, {D5, EIGHTH}, {C5, EIGHTH},
    /* Bar 3 */
    {B4, QUARTER}, {B4, EIGHTH}, {C5, EIGHTH}, {D5, QUARTER}, {E5, QUARTER},
    /* Bar 4 */
    {C5, QUARTER}, {A4, QUARTER}, {A4, QUARTER}, {REST, QUARTER},
    /* Bar 5 */
    {D5, QUARTER}, {D5, EIGHTH}, {F5, EIGHTH}, {A5, QUARTER}, {G5, EIGHTH}, {F5, EIGHTH},
    /* Bar 6 */
    {C5, QUARTER}, {C5, EIGHTH}, {E5, EIGHTH}, {G5, QUARTER}, {F5, EIGHTH}, {E5, EIGHTH},
    /* Bar 7 */
    {D5, QUARTER}, {B4, EIGHTH}, {C5, EIGHTH}, {D5, QUARTER}, {E5, QUARTER},
    /* Bar 8 */
    {C5, QUARTER}, {A4, QUARTER}, {A4, QUARTER}, {REST, QUARTER}
};
static const int TETRIS_LEN = sizeof(tetris_melody) / sizeof(Note);

/* --- MODULE STATE --- */
static int is_playing = 0;
static int chA_index = 0;
static int chA_ticks_remaining = 0;

/* Active song pointers */
static const Note* current_song = 0;
static int current_song_len = 0;


/* --- FUNCTIONS --- */

void start_music(SongChoice song) {
    /* Route the song choice to the correct array */
    switch (song) {
        case SONG_BOURREE:
            current_song = bourree_melody;
            current_song_len = BOURREE_LEN;
            break;
        case SONG_TETRIS:
            current_song = tetris_melody;
            current_song_len = TETRIS_LEN;
            break;
        default:
            return; /* Invalid choice */
    }

    /* Reset state */
    chA_index = 0;
    chA_ticks_remaining = current_song[0].duration;
    is_playing = 1;

    /* Initialize PSG channels securely */
    {
        long old_ssp = Super(0);
        enable_channel_q(CHANNEL_A, 1, 0);
        enable_channel_q(CHANNEL_B, 0, 0);
        enable_channel_q(CHANNEL_C, 0, 0);

        /* Load first note */
        set_volume_q(CHANNEL_A, 10);
        if (current_song[0].pitch == REST) {
            set_volume_q(CHANNEL_A, 0);
        } else {
            set_tone_q(CHANNEL_A, current_song[0].pitch);
        }
        Super(old_ssp);
    }
}

void update_music(UINT32 time_elapsed) {
    if (!is_playing || time_elapsed == 0 || current_song_len == 0) return;

    /* --- UPDATE CHANNEL A (MELODY) --- */
    chA_ticks_remaining -= time_elapsed;
    
    if (chA_ticks_remaining <= 0) {
        /* Advance circular array */
        chA_index = (chA_index + 1) % current_song_len;
        
        /* Carry over negative ticks to keep exact timing */
        chA_ticks_remaining += current_song[chA_index].duration;

        if (current_song[chA_index].pitch == REST) {
            long old_ssp = Super(0);
            set_volume_q(CHANNEL_A, 0);
            Super(old_ssp);
        } else {
            /* Reset volume to emulate articulation */
            long old_ssp = Super(0);
            set_volume_q(CHANNEL_A, 0);
            set_volume_q(CHANNEL_A, 10);
            set_tone_q(CHANNEL_A, current_song[chA_index].pitch);
            Super(old_ssp);
        }
    }
}

void stop_music(void) {
    is_playing = 0;
    {
        long old_ssp = Super(0);
        set_volume_q(CHANNEL_A, 0);
        Super(old_ssp);
    }
}