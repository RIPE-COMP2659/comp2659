#include "music.h"
#include "notes.h"
#include "psg.h" /* Assuming your set_tone_q / set_volume_q are here */

#define EIGHTH 20
#define QUARTER 40
#define HALF 80
#define REST 0

/* --- BOURREE TRACK DATA (1 Channel) --- */

/* Channel A: Right Hand Melody */
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
    {D5S, QUARTER}, {F5S, QUARTER}, {B4, HALF}
};
static const int MELODY_LEN = sizeof(bourree_melody) / sizeof(Note);


/* --- MODULE STATE --- */
static int is_playing = 0;
static int chA_index = 0;
static int chA_ticks_remaining = 0;


/* --- FUNCTIONS --- */

void start_music(void) {
    /* Reset state */
    chA_index = 0;
    chA_ticks_remaining = bourree_melody[0].duration;
    is_playing = 1;

    /* Initialize PSG channels (Only A enabled) using quick writes inside a
       short supervisor window to avoid repeated Super() calls while ensuring
       safe hardware access. */
    {
        long old_ssp = Super(0);
        enable_channel_q(CHANNEL_A, 1, 0);
        enable_channel_q(CHANNEL_B, 0, 0);
        enable_channel_q(CHANNEL_C, 0, 0);

        /* Load first note */
        set_volume_q(CHANNEL_A, 10);
        if (bourree_melody[0].pitch == REST) {
            set_volume_q(CHANNEL_A, 0);
        } else {
            set_tone_q(CHANNEL_A, bourree_melody[0].pitch);
        }
        Super(old_ssp);
    }
}

void update_music(UINT32 time_elapsed) {
    if (!is_playing || time_elapsed == 0) return;

    /* --- UPDATE CHANNEL A (MELODY) --- */
    chA_ticks_remaining -= time_elapsed;
    if (chA_ticks_remaining <= 0) {
        /* Advance circular array */
        chA_index = (chA_index + 1) % MELODY_LEN;
        
        /* Carry over negative ticks to keep exact timing */
        chA_ticks_remaining += bourree_melody[chA_index].duration;

        if (bourree_melody[chA_index].pitch == REST) {
            long old_ssp = Super(0);
            set_volume_q(CHANNEL_A, 0);
            Super(old_ssp);
        } else {
            /* Reset volume to emulate articulation (a new "strike") using
               quick writes inside supervisor to make the whole update atomic */
            long old_ssp = Super(0);
            set_volume_q(CHANNEL_A, 0);
            set_volume_q(CHANNEL_A, 10);
            set_tone_q(CHANNEL_A, bourree_melody[chA_index].pitch);
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