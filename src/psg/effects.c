#include "effects.h"
#include "notes.h"
#include "../clock/clock.h"

/**
 * play_death_effect
 * Implementation for a duller, rougher crash sound.
 */
void play_death_effect(void) {
    /* Enable both tone and noise */
    enable_channel(CHANNEL_C, 1, 1);
    
    /* Low frequency tone (D# 77.78Hz) for the impact */
    set_tone(CHANNEL_C, D2S);

    /* Lowering noise frequency for a "duller" rumble (from 0x08 to 0x12) */
    set_noise(0x12);
    
    /* Use envelope mode */
    set_volume(CHANNEL_C, 0x10);
    
    /* Medium-short decay */
    set_envelope(0x00, 0x4000);
}

/**
 * play_jump_effect
 * Short, nonchalant "pop" on Channel C.
 */
void play_jump_effect(void) {
    /* Enable only tone on Channel C */
    enable_channel(CHANNEL_C, 1, 0);
    
    /* Short tone */
    set_tone(CHANNEL_C, D5);
    set_volume(CHANNEL_C, 0x10);
    
    /* Very short decay */
    set_envelope(0x00, 0x01FF);
}

/**
 * play_level_complete_effect
 * Triumphant rising arpeggio: C5 -> E5 -> G5 -> C6
 */
void play_level_complete_effect(void) {
    unsigned long wait_until;

    /* Enable only tone on Channel C */
    enable_channel(CHANNEL_C, 1, 0);
    
    /* Fast Note Sequence (Arpeggio) */
    
    /* Note 1: C5 */
    set_tone(CHANNEL_C, C5);
    set_volume(CHANNEL_C, 12);
    wait_until = get_time() + 4;
    while(get_time() < wait_until);
    
    /* Note 2: E5 */
    set_tone(CHANNEL_C, E5);
    wait_until = get_time() + 4;
    while(get_time() < wait_until);
    
    /* Note 3: G5 */
    set_tone(CHANNEL_C, G5);
    wait_until = get_time() + 4;
    while(get_time() < wait_until);
    
    /* Final Note: C6 with Decay - Shortened to prevent trailing noise */
    set_tone(CHANNEL_C, C6);
    set_volume(CHANNEL_C, 0x10);
    set_envelope(0x00, 0x4FFF);
}
