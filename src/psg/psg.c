/* See psg.h for documentation */

#include "psg.h"

/* Hardware addresses */
#define REG_SELECT_PTR  ((volatile char *)0xFF8800)
#define REG_READ_PTR    ((volatile char *)0xFF8800)
#define REG_WRITE_PTR   ((volatile char *)0xFF8802)
#define CONSOLE_PTR    ((volatile char *)0x000484)

/* TODO: Non-super and non-error handling variations */

/** See psg.h for documentation */
void write_psg(unsigned int reg, UINT8 val) {
    /* Checking 0 <= val <= 255 is redundant because of data type */
    /* Checking 0 <= reg is redundant because of data type*/
    if (reg <= IO_PORT_B) {
        long old_ssp = Super(0);

        write_psg_q(reg, val);

        Super(old_ssp);
    }
};

/** See psg.h for documentation */
void write_psg_q(unsigned int reg, UINT8 val) {
    *REG_SELECT_PTR = reg;
    *REG_WRITE_PTR = val;
}

/** See psg.h for documentation */
UINT8 read_psg(unsigned int reg) {
    /* Checking 0 <= reg is redundant because of data type */
    if (reg <= IO_PORT_B) {
        long old_ssp = Super(0);
        UINT8 value;

        value = read_psg_q(reg);

        Super(old_ssp);

        return value;
    } else {
        /* There's not a graceful way to handle this, returning a value gives
           a false sense of confidence, but there's no way to give an error
           code */
        Pterm(1);
    }
};

/** See psg.h for documentation */
UINT8 read_psg_q(unsigned int reg) {
    *REG_SELECT_PTR = reg;
    return *REG_READ_PTR;
}

/** See psg.h for documentation */
void set_tone(unsigned int channel, unsigned int tuning) {
    /* Tuning is 1111 1111 1111 at max */
    if (channel <= 2 && tuning <= 0xFFF) {
        long old_ssp = Super(0);

        set_tone_q(channel, tuning);

        Super(old_ssp);
    }
}

/** See psg.h for documentation */
void set_tone_q(unsigned int channel, unsigned int tuning) {
    /* NOTE: Do not hand-modify this code, copy write_psg / write_psg_q
       This just speeds up function calls */
    *REG_SELECT_PTR = channel * 2;
    *REG_WRITE_PTR = tuning & 0xFF;
    *REG_SELECT_PTR = channel * 2 + 1;
    *REG_WRITE_PTR = (tuning >> 8) & 0x0F;
}

/** See psg.h for documentation */
void set_volume(unsigned int channel, unsigned int volume) {
    if (channel <= 2 && volume <= 15) {
        long old_ssp = Super(0);

        set_volume_q(channel, volume);

        Super(old_ssp);
    }
}

/** See psg.h for documentation */
void set_volume_q(unsigned int channel, unsigned int volume) {
    /* NOTE: Do not hand-modify this code, copy write_psg / write_psg_q
       This just speeds up function calls */
    *REG_SELECT_PTR = LEVEL_A + channel;
    *REG_WRITE_PTR = volume;
}

/** See psg.h for documentation */
void enable_channel(
    unsigned int channel,
    unsigned int tone_on,
    unsigned int noise_on
) {
    if (channel <= 2 && tone_on <= 1 && noise_on <= 1) {
        long old_ssp = Super(0);

        enable_channel_q(channel, tone_on, noise_on);

        Super(old_ssp);
    }
}

/** See psg.h for documentation */
void enable_channel_q(
    unsigned int channel,
    unsigned int tone_on,
    unsigned int noise_on
) {
    /* NOTE: Don't hand modify this code, copy function calls */
    *REG_SELECT_PTR = MIXER;
    *REG_WRITE_PTR = (*REG_READ_PTR & ~(0x09 << channel))
        | ((!noise_on << (channel + 3)) | (!tone_on << channel));
    /* Explanation:
           1. We select the register to be read as the mixer, MIXER
           2. That value is in the READ_PTR area, *REG_READ_PTR
           3. We clear the values for the channel, & (0x09 << channel)
                  - 0x09 = 0000 1001, clearing noise and tone for channel
                  - Noise upper 3, tone lower 2nd, CBACBA see manual for more
           4. We create the new values for the channel, ((!noise_on << . . .
                  - Noise and tone are set by enable on low, need to invert
                    because assignment specifications require 1 to enable
           5. We combine the cleared value and new value, either side of |
           6. We write the new value to the mixer, already by *REG_SELECT_PTR
                  - We can do this because MIXER is already set
    */
}

/** See psg.h for documentation */
void stop_sound() {
    long old_ssp = Super(0);

    stop_sound_q();

    Super(old_ssp);
}

/** See psg.h for documentation */
void stop_sound_q() {
    /* NOTE: Do not hand modify this code, copy read_psg / write_psg q */
    UINT8 mixer;
    *REG_SELECT_PTR = MIXER;
    *REG_WRITE_PTR = *REG_READ_PTR | 0x3F;
    /* Explanation:
           1. We select the register to be read as the mixer, MIXER
           2. That value is in the READ_PTR area, *REG_READ_PTR
           3. We set the bits for disabling tone/noise for channels A-C, | 0x3F
               - 0x3F = 0011 1111, leave the upper bits, disable is high
           4. We write the new value to the mixer, already by *REG_SELECT_PTR
                - We can do this because MIXER is already set
    */
}

/* TODO: Function documentation */
void toggle_keyboard_sound() {
    long old_ssp = Super(0);
    UINT8 console_byte = *CONSOLE_PTR ^ 0x01; /* Invert bit 0 to flip keyboard sound */
    *CONSOLE_PTR = console_byte;
    Super(old_ssp);
}
