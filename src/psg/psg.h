/**
 * psg.h
 * Authors:
 *     Riley Gramlich, rgram060@mtroyal.ca, 201762060
 *     Robert Parker Hutcheson, rhutc335@mtroyal.ca, 201762335
 *     Isaac Klein, iklei977@mtroyal.ca, 201763977
 *     Eduard Mykhailets, emykh268@mtroyal.ca, 201750268
 * Course: COMP 2659-001, Computing Machinery II, Winter 2026
 * Instructor: Nolan Shaw
 *
 * The Programmable Sound Generator, PSG, provides functions to control the
 * PSG hardware for sound generation, including setting tones, volumes, and
 * enabling or disabling channels.
 */
#include "../shared/dtypes.h"
#include "notes.h"
#include <osbind.h>  /* for Super() */

/* PSG Registers */
#define A_FINE          0   /* Channel A fine tune */
#define A_COARSE        1   /* Channel A coarse tune */
#define B_FINE          2   /* Channel B fine tune */
#define B_COARSE        3   /* Channel B coarse tune */
#define C_FINE          4   /* Channel C fine tune */
#define C_COARSE        5   /* Channel C coarse tune */
#define NOISE_FREQ      6   /* Noise frequency */
#define MIXER           7   /* Mixer and I/O control */
#define LEVEL_A         8   /* Channel A level */
#define LEVEL_B         9   /* Channel B level */
#define LEVEL_C         10  /* Channel C level */
#define ENV_FINE        11  /* Envelope fine tune */
#define ENV_COARSE      12  /* Envelope coarse tune */
#define ENV_SHAPE       13  /* Envelope shape */
#define IO_PORT_A       14  /* I/O port A data */
#define IO_PORT_B       15  /* I/O port B data */

/* Channel Constants */
#define CHANNEL_A       0
#define CHANNEL_B       1
#define CHANNEL_C       2

/**
 * Writes the given byte value (0-255) to the given PSG register (0-15). This
 * is a helper routine to be used by the other functions in this module
 *
 * Params:
 *     unsigned int reg:
 *         The PSG register to write to (0-15)
 *     UINT8 val:
 *         The value to write to the PSG register (0-255)
 */
void write_psg(unsigned int reg, UINT8 val);

/**
 * Useful for testing purposes, but unlikely to be used in either the music
 * or effects modules. Do not use this function in rapid succession, because
 * there are timing issues with the reading. Otherwise the register doesn't
 * have time to update
 *
 * Params:
 *     unsigned int reg:
 *         The PSG register to read from (0-15)
 *
 * Returns:
 *     UINT8: The value read from the PSG register (0-255)
 */
UINT8 read_psg(unsigned int reg);

/**
 * Loads the tone registers (coarse and fine) for the given channel (0=A, 1=B,
 * 2=C) with the given 12-bit tuning
 *
 * Params:
 *     unsigned int reg:
 *         The channel the tone will play from, 0=A, 1=B, 2=C
 *     unsigned int tuning:
 *         The 12-bit tuning value to load into the tone registers, the fine
 *         register gets the lower 8 bits, and the coarse register gets the
 *         upper 4 bits
 */
void set_tone(unsigned int channel, unsigned int tuning);

/**
 * Loads the volume register for the given channel
 *
 * Params:
 *     unsigned int channel:
 *         The channel to set the volume for, 0=A, 1=B, 2=C
 *     unsigned int volume:
 *         The volume level to set (0-15)
 */
void set_volume(unsigned int channel, unsigned int volume);

/**
 * Turns the given channel’s tone/noise signals on/off (0=off, 1=on)
 *
 * Params:
 *     unsigned int channel:
 *         The channel to enable/disable, 0=A, 1=B, 2=C
 *     unsigned int tone_on:
 *         Whether to enable the tone signal (0=off, 1=on)
 *     unsigned int noise_on:
 *         Whether to enable the noise signal (0=off, 1=on)
 */
void enable_channel(
    unsigned int channel,
    unsigned int tone_on,
    unsigned int noise_on
);

/**
 * Silences all PSG sound production
 */
void stop_sound();

/**
 * The following functions are the same as the above, without input validation
 * or super calls. The above functions all wrap their quick counterpart. So if
 * they work, then so do their quick counterparts.
 */
void write_psg_q(unsigned int reg, UINT8 val);
UINT8 read_psg_q(unsigned int reg);
void set_tone_q(unsigned int channel, unsigned int tuning);
void set_volume_q(unsigned int channel, unsigned int volume);
void enable_channel_q(
    unsigned int channel,
    unsigned int tone_on,
    unsigned int noise_on
);
void stop_sound_q();
