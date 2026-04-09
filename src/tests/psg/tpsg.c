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

#include "../../clock/clock.h"
#include "../../psg/psg.h"
#include "../../shared/dtypes.h"

/* Forward declarations for tests added later in the file */
/* TODO: Likely not necessary for forward dec */
void test_dual_channel(void);

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
  write_psg_q(LEVEL_A, 0x10);
  /* TODO: Tune the timing */
  write_psg_q(ENV_FINE, 0x60);
  write_psg_q(ENV_COARSE, 0x60);
  write_psg_q(ENV_SHAPE, 0x00);
  Cconws("test_crash (Playing Level Complete): press key to start...\r\n");
  Cnecin();

  /* Rising arpeggio: C5 -> E5 -> G5 -> C6 */
  enable_channel_q(CHANNEL_C, 1, 0);

  set_tone_q(CHANNEL_C, C5);
  set_volume_q(CHANNEL_C, 14);
  wait(50000);

  set_tone_q(CHANNEL_C, E5);
  wait(50000);

  set_tone_q(CHANNEL_C, G5);
  wait(50000);

  set_tone_q(CHANNEL_C, C6);
  set_volume_q(CHANNEL_C, 0x10);
  set_envelope_q(0x00, 0x7FFF);

  Super(old_ssp);

  Cconws("Done, press any key to stop...\r\n");
  Cnecin();
  stop_sound();
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

  set_tone_q(CHANNEL_A, D5);
  set_volume_q(CHANNEL_A, 10);
  wait(12000);
  set_tone_q(CHANNEL_A, F5);
  wait(12000);
  set_tone_q(CHANNEL_A, D6);
  wait(24000);
  set_volume_q(CHANNEL_A, 0);
  wait(24000);

  set_tone_q(CHANNEL_A, D5);
  set_volume_q(CHANNEL_A, 10);
  wait(12000);
  set_tone_q(CHANNEL_A, F5);
  wait(12000);
  set_tone_q(CHANNEL_A, D6);
  wait(24000);
  set_volume_q(CHANNEL_A, 0);
  wait(24000);

  set_tone_q(CHANNEL_A, E6);
  set_volume_q(CHANNEL_A, 10);
  wait(24000);
  set_tone_q(CHANNEL_A, F6);
  wait(16000);
  set_tone_q(CHANNEL_A, E6);
  wait(16000);
  set_tone_q(CHANNEL_A, F6);
  wait(16000);

  set_tone_q(CHANNEL_A, E6);
  set_volume_q(CHANNEL_A, 10);
  wait(12000);
  set_tone_q(CHANNEL_A, C6);
  wait(12000);
  set_tone_q(CHANNEL_A, A5);
  wait(48000);

  set_tone_q(CHANNEL_A, A5);
  set_volume_q(CHANNEL_A, 10);
  wait(12000);
  set_tone_q(CHANNEL_A, D5);
  wait(12000);
  set_tone_q(CHANNEL_A, F5);
  wait(12000);
  set_tone_q(CHANNEL_A, G5);
  wait(36000);

  set_tone_q(CHANNEL_A, A5);
  set_volume_q(CHANNEL_A, 10);
  wait(72000);

  set_tone_q(CHANNEL_A, A5);
  set_volume_q(CHANNEL_A, 10);
  wait(12000);
  set_tone_q(CHANNEL_A, D5);
  wait(12000);
  set_tone_q(CHANNEL_A, F5);
  wait(12000);
  set_tone_q(CHANNEL_A, G5);
  wait(36000);

  set_tone_q(CHANNEL_A, F5);
  set_volume_q(CHANNEL_A, 10);
  wait(72000);

  set_tone_q(CHANNEL_A, D5);
  set_volume_q(CHANNEL_A, 10);
  wait(12000);
  set_tone_q(CHANNEL_A, F5);
  wait(12000);
  set_tone_q(CHANNEL_A, D6);
  wait(24000);
  set_volume_q(CHANNEL_A, 0);
  wait(24000);

  set_tone_q(CHANNEL_A, D5);
  set_volume_q(CHANNEL_A, 10);
  wait(12000);
  set_tone_q(CHANNEL_A, F5);
  wait(12000);
  set_tone_q(CHANNEL_A, D6);
  wait(24000);
  set_volume_q(CHANNEL_A, 0);
  wait(24000);

  set_tone_q(CHANNEL_A, E6);
  set_volume_q(CHANNEL_A, 10);
  wait(24000);
  set_tone_q(CHANNEL_A, F6);
  wait(16000);
  set_tone_q(CHANNEL_A, E6);
  wait(16000);
  set_tone_q(CHANNEL_A, F6);
  wait(16000);

  set_tone_q(CHANNEL_A, E6);
  set_volume_q(CHANNEL_A, 10);
  wait(12000);
  set_tone_q(CHANNEL_A, C6);
  wait(12000);
  set_tone_q(CHANNEL_A, A5);
  wait(48000);

  set_tone_q(CHANNEL_A, A5);
  set_volume_q(CHANNEL_A, 10);
  wait(12000);
  set_tone_q(CHANNEL_A, D5);
  wait(12000);
  set_tone_q(CHANNEL_A, F5);
  wait(12000);
  set_tone_q(CHANNEL_A, G5);
  wait(36000);

  set_tone_q(CHANNEL_A, A5);
  set_volume_q(CHANNEL_A, 10);
  wait(36000);
  set_tone_q(CHANNEL_A, A5);
  wait(36000);

  set_tone_q(CHANNEL_A, D5);
  set_volume_q(CHANNEL_A, 10);
  wait(72000);
  set_tone_q(CHANNEL_A, D5);
  wait(72000);

  set_volume_q(CHANNEL_A, 0);
  wait(1);

  stop_sound_q();

  Super(old_ssp);
}

/** Play Bourree in E minor (Right hand melody, Half Tempo) */
void test_bourree(void) {
  long old_ssp;

  /* Doubled U from 2000 to 4000 to exactly halve the playback speed */
  unsigned int U = 4000;

  Cconws("test_bourree: Prepared, press any key to continue...\r\n");
  Cnecin();
  wait(30000);

  old_ssp = Super(0);

  /* Volumes */
  set_volume_q(CHANNEL_A, 10);
  set_volume_q(CHANNEL_B, 0);
  set_volume_q(CHANNEL_C, 0);

  /* Channels: only use A for this test */
  enable_channel_q(CHANNEL_A, 1, 0);
  enable_channel_q(CHANNEL_B, 0, 0);
  enable_channel_q(CHANNEL_C, 0, 0);

  /* Pickup (8th notes) */
  set_tone_q(CHANNEL_A, E5);
  set_volume_q(CHANNEL_A, 10);
  wait(8 * U);
  set_tone_q(CHANNEL_A, F5S);
  set_volume_q(CHANNEL_A, 10);
  wait(8 * U);

  /* Bar 1 */
  set_tone_q(CHANNEL_A, G5);
  set_volume_q(CHANNEL_A, 10);
  wait(8 * U);
  set_tone_q(CHANNEL_A, F5S);
  set_volume_q(CHANNEL_A, 10);
  wait(8 * U);
  set_tone_q(CHANNEL_A, G5);
  set_volume_q(CHANNEL_A, 10);
  wait(8 * U);
  set_tone_q(CHANNEL_A, A5);
  set_volume_q(CHANNEL_A, 10);
  wait(8 * U);
  /* The "Pauses" (Quarter notes) */
  set_tone_q(CHANNEL_A, B5);
  set_volume_q(CHANNEL_A, 10);
  wait(16 * U);
  set_tone_q(CHANNEL_A, E5);
  set_volume_q(CHANNEL_A, 10);
  wait(16 * U);

  /* Bar 2 (All 8th notes) */
  set_tone_q(CHANNEL_A, F5S);
  set_volume_q(CHANNEL_A, 10);
  wait(8 * U);
  set_tone_q(CHANNEL_A, G5);
  set_volume_q(CHANNEL_A, 10);
  wait(8 * U);
  set_tone_q(CHANNEL_A, A5);
  set_volume_q(CHANNEL_A, 10);
  wait(8 * U);
  set_tone_q(CHANNEL_A, B5);
  set_volume_q(CHANNEL_A, 10);
  wait(8 * U);
  set_tone_q(CHANNEL_A, C6);
  set_volume_q(CHANNEL_A, 10);
  wait(8 * U);
  set_tone_q(CHANNEL_A, B5);
  set_volume_q(CHANNEL_A, 10);
  wait(8 * U);
  set_tone_q(CHANNEL_A, C6);
  set_volume_q(CHANNEL_A, 10);
  wait(8 * U);
  set_tone_q(CHANNEL_A, D6);
  set_volume_q(CHANNEL_A, 10);
  wait(8 * U);

  /* Bar 3 (Descending scale, all 8th notes) */
  set_tone_q(CHANNEL_A, E6);
  set_volume_q(CHANNEL_A, 10);
  wait(8 * U);
  set_tone_q(CHANNEL_A, D6);
  set_volume_q(CHANNEL_A, 10);
  wait(8 * U);
  set_tone_q(CHANNEL_A, C6);
  set_volume_q(CHANNEL_A, 10);
  wait(8 * U);
  set_tone_q(CHANNEL_A, B5);
  set_volume_q(CHANNEL_A, 10);
  wait(8 * U);
  set_tone_q(CHANNEL_A, A5);
  set_volume_q(CHANNEL_A, 10);
  wait(8 * U);
  set_tone_q(CHANNEL_A, G5);
  set_volume_q(CHANNEL_A, 10);
  wait(8 * U);
  set_tone_q(CHANNEL_A, F5S);
  set_volume_q(CHANNEL_A, 10);
  wait(8 * U);
  set_tone_q(CHANNEL_A, E5);
  set_volume_q(CHANNEL_A, 10);
  wait(8 * U);

  /* Bar 4 (Quarter notes and a Half note for a long pause) */
  set_tone_q(CHANNEL_A, D5S);
  set_volume_q(CHANNEL_A, 10);
  wait(16 * U);
  set_tone_q(CHANNEL_A, F5S);
  set_volume_q(CHANNEL_A, 10);
  wait(16 * U);
  set_tone_q(CHANNEL_A, B4);
  set_volume_q(CHANNEL_A, 10);
  wait(32 * U);

  /* Bar 5 (Repeat of Pickup & Bar 1) */
  set_tone_q(CHANNEL_A, E5);
  set_volume_q(CHANNEL_A, 10);
  wait(8 * U);
  set_tone_q(CHANNEL_A, F5S);
  set_volume_q(CHANNEL_A, 10);
  wait(8 * U);
  set_tone_q(CHANNEL_A, G5);
  set_volume_q(CHANNEL_A, 10);
  wait(8 * U);
  set_tone_q(CHANNEL_A, F5S);
  set_volume_q(CHANNEL_A, 10);
  wait(8 * U);
  set_tone_q(CHANNEL_A, G5);
  set_volume_q(CHANNEL_A, 10);
  wait(8 * U);
  set_tone_q(CHANNEL_A, A5);
  set_volume_q(CHANNEL_A, 10);
  wait(8 * U);
  set_tone_q(CHANNEL_A, B5);
  set_volume_q(CHANNEL_A, 10);
  wait(16 * U);
  set_tone_q(CHANNEL_A, E5);
  set_volume_q(CHANNEL_A, 10);
  wait(16 * U);

  /* Bar 6 (Repeat of Bar 2) */
  set_tone_q(CHANNEL_A, F5S);
  set_volume_q(CHANNEL_A, 10);
  wait(8 * U);
  set_tone_q(CHANNEL_A, G5);
  set_volume_q(CHANNEL_A, 10);
  wait(8 * U);
  set_tone_q(CHANNEL_A, A5);
  set_volume_q(CHANNEL_A, 10);
  wait(8 * U);
  set_tone_q(CHANNEL_A, B5);
  set_volume_q(CHANNEL_A, 10);
  wait(8 * U);
  set_tone_q(CHANNEL_A, C6);
  set_volume_q(CHANNEL_A, 10);
  wait(8 * U);
  set_tone_q(CHANNEL_A, B5);
  set_volume_q(CHANNEL_A, 10);
  wait(8 * U);
  set_tone_q(CHANNEL_A, C6);
  set_volume_q(CHANNEL_A, 10);
  wait(8 * U);
  set_tone_q(CHANNEL_A, D6);
  set_volume_q(CHANNEL_A, 10);
  wait(8 * U);

  /* Bar 7 */
  set_tone_q(CHANNEL_A, G5);
  set_volume_q(CHANNEL_A, 10);
  wait(8 * U);
  set_tone_q(CHANNEL_A, A5);
  set_volume_q(CHANNEL_A, 10);
  wait(8 * U);
  set_tone_q(CHANNEL_A, B5);
  set_volume_q(CHANNEL_A, 10);
  wait(8 * U);
  set_tone_q(CHANNEL_A, A5);
  set_volume_q(CHANNEL_A, 10);
  wait(8 * U);
  set_tone_q(CHANNEL_A, G5);
  set_volume_q(CHANNEL_A, 10);
  wait(8 * U);
  set_tone_q(CHANNEL_A, F5S);
  set_volume_q(CHANNEL_A, 10);
  wait(8 * U);
  set_tone_q(CHANNEL_A, E5);
  set_volume_q(CHANNEL_A, 10);
  wait(8 * U);
  set_tone_q(CHANNEL_A, D5S);
  set_volume_q(CHANNEL_A, 10);
  wait(8 * U);

  /* Bar 8 final */
  set_tone_q(CHANNEL_A, E5);
  set_volume_q(CHANNEL_A, 10);
  wait(48 * U);

  set_volume_q(CHANNEL_A, 0);
  wait(16 * U);

  stop_sound_q();

  Super(old_ssp);
}

/** Play Bourree in E minor (Right hand melody, Half Tempo) */
void test_bourree(void) {
    long old_ssp;
    
    /* Doubled U from 2000 to 4000 to exactly halve the playback speed */
    unsigned int U = 4000; 

    Cconws("test_bourree: Prepared, press any key to continue...\r\n");
    Cnecin();
    wait(30000);

    old_ssp = Super(0);

    /* Volumes */
    set_volume_q(CHANNEL_A, 10);
    set_volume_q(CHANNEL_B, 0);
    set_volume_q(CHANNEL_C, 0);

    /* Channels: only use A for this test */
    enable_channel_q(CHANNEL_A, 1, 0);
    enable_channel_q(CHANNEL_B, 0, 0);
    enable_channel_q(CHANNEL_C, 0, 0);

    /* Pickup (8th notes) */
    set_tone_q(CHANNEL_A, E5);  set_volume_q(CHANNEL_A, 10); wait(8 * U);
    set_tone_q(CHANNEL_A, F5S); set_volume_q(CHANNEL_A, 10); wait(8 * U);

    /* Bar 1 */
    set_tone_q(CHANNEL_A, G5);  set_volume_q(CHANNEL_A, 10); wait(8 * U);
    set_tone_q(CHANNEL_A, F5S); set_volume_q(CHANNEL_A, 10); wait(8 * U);
    set_tone_q(CHANNEL_A, G5);  set_volume_q(CHANNEL_A, 10); wait(8 * U);
    set_tone_q(CHANNEL_A, A5);  set_volume_q(CHANNEL_A, 10); wait(8 * U);
    /* The "Pauses" (Quarter notes) */
    set_tone_q(CHANNEL_A, B5);  set_volume_q(CHANNEL_A, 10); wait(16 * U); 
    set_tone_q(CHANNEL_A, E5);  set_volume_q(CHANNEL_A, 10); wait(16 * U); 

    /* Bar 2 (All 8th notes) */
    set_tone_q(CHANNEL_A, F5S); set_volume_q(CHANNEL_A, 10); wait(8 * U);
    set_tone_q(CHANNEL_A, G5);  set_volume_q(CHANNEL_A, 10); wait(8 * U);
    set_tone_q(CHANNEL_A, A5);  set_volume_q(CHANNEL_A, 10); wait(8 * U);
    set_tone_q(CHANNEL_A, B5);  set_volume_q(CHANNEL_A, 10); wait(8 * U);
    set_tone_q(CHANNEL_A, C6);  set_volume_q(CHANNEL_A, 10); wait(8 * U);
    set_tone_q(CHANNEL_A, B5);  set_volume_q(CHANNEL_A, 10); wait(8 * U);
    set_tone_q(CHANNEL_A, C6);  set_volume_q(CHANNEL_A, 10); wait(8 * U);
    set_tone_q(CHANNEL_A, D6);  set_volume_q(CHANNEL_A, 10); wait(8 * U);

    /* Bar 3 (Descending scale, all 8th notes) */
    set_tone_q(CHANNEL_A, E6);  set_volume_q(CHANNEL_A, 10); wait(8 * U);
    set_tone_q(CHANNEL_A, D6);  set_volume_q(CHANNEL_A, 10); wait(8 * U);
    set_tone_q(CHANNEL_A, C6);  set_volume_q(CHANNEL_A, 10); wait(8 * U);
    set_tone_q(CHANNEL_A, B5);  set_volume_q(CHANNEL_A, 10); wait(8 * U);
    set_tone_q(CHANNEL_A, A5);  set_volume_q(CHANNEL_A, 10); wait(8 * U);
    set_tone_q(CHANNEL_A, G5);  set_volume_q(CHANNEL_A, 10); wait(8 * U);
    set_tone_q(CHANNEL_A, F5S); set_volume_q(CHANNEL_A, 10); wait(8 * U);
    set_tone_q(CHANNEL_A, E5);  set_volume_q(CHANNEL_A, 10); wait(8 * U);

    /* Bar 4 (Quarter notes and a Half note for a long pause) */
    set_tone_q(CHANNEL_A, D5S); set_volume_q(CHANNEL_A, 10); wait(16 * U); 
    set_tone_q(CHANNEL_A, F5S); set_volume_q(CHANNEL_A, 10); wait(16 * U); 
    set_tone_q(CHANNEL_A, B4);  set_volume_q(CHANNEL_A, 10); wait(32 * U); 

    /* Bar 5 (Repeat of Pickup & Bar 1) */
    set_tone_q(CHANNEL_A, E5);  set_volume_q(CHANNEL_A, 10); wait(8 * U);
    set_tone_q(CHANNEL_A, F5S); set_volume_q(CHANNEL_A, 10); wait(8 * U);
    set_tone_q(CHANNEL_A, G5);  set_volume_q(CHANNEL_A, 10); wait(8 * U);
    set_tone_q(CHANNEL_A, F5S); set_volume_q(CHANNEL_A, 10); wait(8 * U);
    set_tone_q(CHANNEL_A, G5);  set_volume_q(CHANNEL_A, 10); wait(8 * U);
    set_tone_q(CHANNEL_A, A5);  set_volume_q(CHANNEL_A, 10); wait(8 * U);
    set_tone_q(CHANNEL_A, B5);  set_volume_q(CHANNEL_A, 10); wait(16 * U); 
    set_tone_q(CHANNEL_A, E5);  set_volume_q(CHANNEL_A, 10); wait(16 * U); 

    /* Bar 6 (Repeat of Bar 2) */
    set_tone_q(CHANNEL_A, F5S); set_volume_q(CHANNEL_A, 10); wait(8 * U);
    set_tone_q(CHANNEL_A, G5);  set_volume_q(CHANNEL_A, 10); wait(8 * U);
    set_tone_q(CHANNEL_A, A5);  set_volume_q(CHANNEL_A, 10); wait(8 * U);
    set_tone_q(CHANNEL_A, B5);  set_volume_q(CHANNEL_A, 10); wait(8 * U);
    set_tone_q(CHANNEL_A, C6);  set_volume_q(CHANNEL_A, 10); wait(8 * U);
    set_tone_q(CHANNEL_A, B5);  set_volume_q(CHANNEL_A, 10); wait(8 * U);
    set_tone_q(CHANNEL_A, C6);  set_volume_q(CHANNEL_A, 10); wait(8 * U);
    set_tone_q(CHANNEL_A, D6);  set_volume_q(CHANNEL_A, 10); wait(8 * U);

    /* Bar 7 */
    set_tone_q(CHANNEL_A, G5);  set_volume_q(CHANNEL_A, 10); wait(8 * U);
    set_tone_q(CHANNEL_A, A5);  set_volume_q(CHANNEL_A, 10); wait(8 * U);
    set_tone_q(CHANNEL_A, B5);  set_volume_q(CHANNEL_A, 10); wait(8 * U);
    set_tone_q(CHANNEL_A, A5);  set_volume_q(CHANNEL_A, 10); wait(8 * U);
    set_tone_q(CHANNEL_A, G5);  set_volume_q(CHANNEL_A, 10); wait(8 * U);
    set_tone_q(CHANNEL_A, F5S); set_volume_q(CHANNEL_A, 10); wait(8 * U);
    set_tone_q(CHANNEL_A, E5);  set_volume_q(CHANNEL_A, 10); wait(8 * U);
    set_tone_q(CHANNEL_A, D5S); set_volume_q(CHANNEL_A, 10); wait(8 * U);

    /* Bar 8 final */
    set_tone_q(CHANNEL_A, E5);  set_volume_q(CHANNEL_A, 10); wait(48 * U);
    
    set_volume_q(CHANNEL_A, 0); 
    wait(16 * U);

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
    test_dual_channel();
    test_crash();
    test_bourree();

  return 0;
}

/** Simple test that plays tones on both channel A and B simultaneously */
void test_dual_channel(void) {
  long old_ssp;

  Cconws("test_dual_channel: Prepared, press any key to start...\r\n");
  Cnecin();
  wait(10000);

  old_ssp = Super(0);

  /* Enable A and B, silence C */
  set_volume_q(CHANNEL_A, 12);
  set_volume_q(CHANNEL_B, 10);
  set_volume_q(CHANNEL_C, 0);

  enable_channel_q(CHANNEL_A, 1, 0);
  enable_channel_q(CHANNEL_B, 1, 0);
  enable_channel_q(CHANNEL_C, 0, 0);

  /* Play a sustained interval for a short time */
  set_tone_q(CHANNEL_A, E4); /* melody-ish */
  set_tone_q(CHANNEL_B, E2); /* bass-ish */
  wait(24000);

  /* Change the bass to ensure B updates */
  set_tone_q(CHANNEL_B, B2);
  wait(24000);

  stop_sound_q();
  Super(old_ssp);
}

    /** Simple test that plays tones on both channel A and B simultaneously */
    void test_dual_channel(void) {
        long old_ssp;

        Cconws("test_dual_channel: Prepared, press any key to start...\r\n");
        Cnecin();
        wait(10000);

        old_ssp = Super(0);

        /* Enable A and B, silence C */
        set_volume_q(CHANNEL_A, 12);
        set_volume_q(CHANNEL_B, 10);
        set_volume_q(CHANNEL_C, 0);

        enable_channel_q(CHANNEL_A, 1, 0);
        enable_channel_q(CHANNEL_B, 1, 0);
        enable_channel_q(CHANNEL_C, 0, 0);

        /* Play a sustained interval for a short time */
        set_tone_q(CHANNEL_A, E4); /* melody-ish */
        set_tone_q(CHANNEL_B, E2); /* bass-ish */
        wait(24000);

        /* Change the bass to ensure B updates */
        set_tone_q(CHANNEL_B, B2);
        wait(24000);

        stop_sound_q();
        Super(old_ssp);
    }

