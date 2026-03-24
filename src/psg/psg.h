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
#include <osbind.h>  /* for Super() */

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
 * they work, then so do their quick counterparts
 */
void write_psg_q(unsigned int reg, UINT8 val);
UINT8 read_psg_q(unsigned int reg);
void set_tone_q(unsigned int channel, unsigned int tuning);
void set_volume_q(unsigned int channel, unsigned int volume);
