/**
 * tinput.c
 * Authors:
 *     Riley Gramlich, rgram060@mtroyal.ca, 201762060
 *     Robert Parker Hutcheson, rhutc335@mtroyal.ca, 201762335
 *     Isaac Klein, iklei977@mtroyal.ca, 201763977
 *     Eduard Mykhailets, emykh268@mtroyal.ca, 201750268
 * Course: COMP 2659-001, Computing Machinery II, Winter 2026
 * Instructor: Nolan Shaw
 *
 * Basic testing for the PSG module for compilation as a .TOS program.
 */

#include "../../psg/psg.h"
#include "../../shared/dtypes.h"

static void print_uint(unsigned int value) {
    char digits[10];
    int i;

    if (value == 0) {
        Cconout('0');
        return;
    }

    i = 0;
    while (value > 0 && i < 10) {
        digits[i] = (char)('0' + (value % 10));
        value /= 10;
        i++;
    }

    while (i > 0) {
        i--;
        Cconout(digits[i]);
    }
}

static void wait(unsigned int cycles) {
    volatile unsigned int i;
    for (i = 0; i < cycles; i++) {
        /* Do nothing, just waste time */
    }
}

void test_crash(void) {
    long old_ssp = Super(0);

    wait(30000); /* Make sure keyboard sound stopped */

    Cconws("test_crash: Playing crash, "
           "press any key to continue...\r\n");

    enable_channel_q(CHANNEL_A, 0, 1);
    enable_channel_q(CHANNEL_B, 0, 0);
    enable_channel_q(CHANNEL_C, 0, 0);
    set_volume_q(CHANNEL_A, 15);
    write_psg_q(NOISE_FREQ, 0x1F);
    write_psg_q(LEVEL_A,    0x10);
    /* TODO: Tune the timing */
    write_psg_q(ENV_FINE,   0xFF);
    write_psg_q(ENV_COARSE, 0xFF);
    write_psg_q(ENV_SHAPE,  0x00);

    Super(old_ssp);

    Cnecin();
    stop_sound();
    Cconws("test_crash: Sound stopped, "
           "press any key to continue...\r\n");
    Cnecin();
}

void test_write_psg(void) {
    write_psg(A_FINE, 248); /* set channel A fine tune = 248 */
    write_psg(A_COARSE, 1); /* set channel A coarse tune = 1 */
    write_psg(MIXER, 0x3E); /* enable channel A on mixer */
    write_psg(LEVEL_A, 11); /* set channel A volume = 11 */
    Cconws("test_write_psg: Tone should be playing, "
           "press any key to stop...\r\n");
    Cnecin();

    write_psg(LEVEL_A, 0); /* set channel A volume = 0 */
    Cconws("test_write_psg: Tone should have stopped, "
           "press any key to continue...\r\n");
    Cnecin();

    write_psg(IO_PORT_B + 1, 0);
    Cconws("test_write_psg: Invalid register write should have no change, "
           "press any key to continue...\r\n");
    Cnecin();
}

void test_read_psg(void) {
    UINT8 fine;
    UINT8 coarse;

    wait(10000); /* Make sure keyboard sound stopped */

    write_psg(A_FINE, 248); /* set channel A fine tune = 248 */
    write_psg(A_COARSE, 1); /* set channel A coarse tune = 1 */

    fine = read_psg(A_FINE);
    if (fine == 248) {
        Cconws("test_read_psg: Fine test passed...\r\n");
    } else {
        Cconws("test_read_psg: Fine test failed, expected 248, got ");
        print_uint((unsigned int)fine);
        Cconws("\r\n");
    }

    coarse = read_psg(A_COARSE);
    if (coarse == 1) {
        Cconws("test_read_psg: Coarse test passed...\r\n");
    } else {
        Cconws("test_read_psg: Coarse test failed, expected 1, got ");
        print_uint((unsigned int)coarse);
        Cconws("\r\n");
    }

    Cconws("Press any key to continue...\r\n");
    Cnecin();
}

void test_set_tone(void) {
    /* 504 = 0001 1111 1000 */
    /* 1111 1000 = 248 Fine */
    /* 0001 = 1 Coarse */
    Cconws("test_set_tone: Setting channel A to same tune as test_write, "
           "press any key to continue...\r\n");
    set_tone(CHANNEL_A, 504);
    write_psg(LEVEL_A, 11);
    Cnecin();
}

void test_set_volume(void) {
    Cconws("test_set_volume: Setting channel A volume to 15, "
           "press any key to continue...\r\n");
    set_volume(CHANNEL_A, 15);
    Cnecin();

    Cconws("test_set_volume: Setting channel A volume to 0, "
           "press any key to continue...\r\n");
    set_volume(CHANNEL_A, 0);
    Cnecin();
}

void test_enable_channel(void) {
    UINT8 mixer_state;

    Cconws("test_enable_channel: Enabling channel A tone, "
           "press any key to continue...\r\n");
    enable_channel(CHANNEL_A, 1, 1);
    set_tone(CHANNEL_A, 504);
    set_volume(CHANNEL_A, 11);
    Cnecin();

    Cconws("test_enable_channel: Disabling channel A tone, "
           "press any key to continue...\r\n");
    enable_channel(CHANNEL_A, 0, 0);
    Cnecin();

    wait(10000); /* Make sure keyboard sound stopped */

    Cconws("test_enable_channel: Enabling all channels tone, "
           "press any key to continue...\r\n");
    enable_channel(CHANNEL_A, 1, 1);
    set_volume(CHANNEL_A, 10);
    enable_channel(CHANNEL_B, 1, 1);
    set_volume(CHANNEL_B, 10);
    enable_channel(CHANNEL_C, 1, 1);
    set_volume(CHANNEL_C, 10);

    wait(30000); /* Can't rely on keyboard */

    Cconws("test_enable_channel: Disabling channel A tone...\r\n");
    enable_channel(CHANNEL_A, 0, 0);

    wait(30000); /* Can't rely on keyboard */

    Cconws("test_enable_channel: Disabling channel C tone...\r\n");
    enable_channel(CHANNEL_C, 0, 0);

    wait(30000); /* Can't rely on keyboard */

    Cconws("test_enable_channel: Disabling channel B tone, press any key to "
           "continue...\r\n");
    enable_channel(CHANNEL_B, 0, 0);
    Cnecin();
}

void test_stop_sound(void) {
    Cconws("test_stop_sound: Enabling all channels tone...\r\n");
    enable_channel(CHANNEL_A, 1, 1);
    set_volume(CHANNEL_A, 10);
    enable_channel(CHANNEL_B, 1, 1);
    set_volume(CHANNEL_B, 10);
    enable_channel(CHANNEL_C, 1, 1);
    set_volume(CHANNEL_C, 10);

    wait(30000); /* Can't rely on keyboard */

    Cconws("test_stop_sound: Stopping all sound, "
           "press any key to continue...\r\n");
    stop_sound();
    Cnecin();
}

/** A demo of a not working perfectly song, but close enough to demo */
void test_song_of_storms(void) {
    long old_ssp;

    Cconws("test_song_of_storms: Prepared, "
           "press any key to continue...\r\n");
    Cnecin();
    wait(30000);

    old_ssp = Super(0);

    /* Volumes */
    set_volume_q(CHANNEL_A, 10);
    set_volume_q(CHANNEL_B, 0);
    set_volume_q(CHANNEL_C, 0);

    /* Channels */
    enable_channel_q(CHANNEL_A, 1, 0);
    enable_channel_q(CHANNEL_B, 0, 0);
    enable_channel_q(CHANNEL_C, 0, 0);

    set_tone_q(CHANNEL_A, D5); set_volume_q(CHANNEL_A, 10); wait(12000);
    set_tone_q(CHANNEL_A, F5); wait(12000);
    set_tone_q(CHANNEL_A, D6); wait(24000);
    set_volume_q(CHANNEL_A, 0); wait(24000);

    set_tone_q(CHANNEL_A, D5); set_volume_q(CHANNEL_A, 10); wait(12000);
    set_tone_q(CHANNEL_A, F5); wait(12000);
    set_tone_q(CHANNEL_A, D6); wait(24000);
    set_volume_q(CHANNEL_A, 0); wait(24000);

    set_tone_q(CHANNEL_A, E6); set_volume_q(CHANNEL_A, 10); wait(24000);
    set_tone_q(CHANNEL_A, F6); wait(16000);
    set_tone_q(CHANNEL_A, E6); wait(16000);
    set_tone_q(CHANNEL_A, F6); wait(16000);

    set_tone_q(CHANNEL_A, E6); set_volume_q(CHANNEL_A, 10); wait(12000);
    set_tone_q(CHANNEL_A, C6); wait(12000);
    set_tone_q(CHANNEL_A, A5); wait(48000);

    set_tone_q(CHANNEL_A, A5); set_volume_q(CHANNEL_A, 10); wait(12000);
    set_tone_q(CHANNEL_A, D5); wait(12000);
    set_tone_q(CHANNEL_A, F5); wait(12000);
    set_tone_q(CHANNEL_A, G5); wait(36000);

    set_tone_q(CHANNEL_A, A5); set_volume_q(CHANNEL_A, 10); wait(72000);

    set_tone_q(CHANNEL_A, A5); set_volume_q(CHANNEL_A, 10); wait(12000);
    set_tone_q(CHANNEL_A, D5); wait(12000);
    set_tone_q(CHANNEL_A, F5); wait(12000);
    set_tone_q(CHANNEL_A, G5); wait(36000);

    set_tone_q(CHANNEL_A, F5); set_volume_q(CHANNEL_A, 10); wait(72000);

    set_tone_q(CHANNEL_A, D5); set_volume_q(CHANNEL_A, 10); wait(12000);
    set_tone_q(CHANNEL_A, F5); wait(12000);
    set_tone_q(CHANNEL_A, D6); wait(24000);
    set_volume_q(CHANNEL_A, 0); wait(24000);

    set_tone_q(CHANNEL_A, D5); set_volume_q(CHANNEL_A, 10); wait(12000);
    set_tone_q(CHANNEL_A, F5); wait(12000);
    set_tone_q(CHANNEL_A, D6); wait(24000);
    set_volume_q(CHANNEL_A, 0); wait(24000);

    set_tone_q(CHANNEL_A, E6); set_volume_q(CHANNEL_A, 10); wait(24000);
    set_tone_q(CHANNEL_A, F6); wait(16000);
    set_tone_q(CHANNEL_A, E6); wait(16000);
    set_tone_q(CHANNEL_A, F6); wait(16000);

    set_tone_q(CHANNEL_A, E6); set_volume_q(CHANNEL_A, 10); wait(12000);
    set_tone_q(CHANNEL_A, C6); wait(12000);
    set_tone_q(CHANNEL_A, A5); wait(48000);

    set_tone_q(CHANNEL_A, A5); set_volume_q(CHANNEL_A, 10); wait(12000);
    set_tone_q(CHANNEL_A, D5); wait(12000);
    set_tone_q(CHANNEL_A, F5); wait(12000);
    set_tone_q(CHANNEL_A, G5); wait(36000);

    set_tone_q(CHANNEL_A, A5); set_volume_q(CHANNEL_A, 10); wait(36000);
    set_tone_q(CHANNEL_A, A5); wait(36000);

    set_tone_q(CHANNEL_A, D5); set_volume_q(CHANNEL_A, 10); wait(72000);
    set_tone_q(CHANNEL_A, D5); wait(72000);

    set_volume_q(CHANNEL_A, 0); wait(1);

    stop_sound_q();

    Super(old_ssp);
}

int main() {
    Cconws("PSG test started...\r\n");

    test_crash();
    test_write_psg();
    test_read_psg();
    test_set_tone();
    test_set_volume();
    test_enable_channel();
    test_stop_sound();
    test_song_of_storms();

    return 0;
}
