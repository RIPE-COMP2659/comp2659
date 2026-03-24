/* See psg.h for documentation */

#include "psg.h"

/** Constants */
/* PSG Registers */
#define A_FINE     0   /* Channel A fine tune */
#define A_COARSE   1   /* Channel A coarse tune */
#define B_FINE     2   /* Channel B fine tune */
#define B_COARSE   3   /* Channel B coarse tune */
#define C_FINE     4   /* Channel C fine tune */
#define C_COARSE   5   /* Channel C coarse tune */
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

/* Hardware addresses */
#define REG_SELECT_PTR  ((volatile char *)0xFF8800)
#define REG_READ_PTR    ((volatile char *)0xFF8800)
#define REG_WRITE_PTR   ((volatile char *)0xFF8802)

/* TODO: Non-super and non-error handling variations */

/** See psg.h for documentation */
void write_psg(unsigned int reg, UINT8 val) {
    /* Checking 0 <= val <= 255 is redundant because of data type */
    /* Checking 0 <= reg is redundant because of data type*/
    if (reg <= IO_PORT_B) {
        long old_ssp = Super(0);

        *REG_SELECT_PTR = reg;
        *REG_WRITE_PTR = val;

        Super(old_ssp);
    }
};

/** See psg.h for documentation */
UINT8 read_psg(unsigned int reg) {
    /* Checking 0 <= reg is redundant because of data type */
    if (reg <= IO_PORT_B) {
        long old_ssp = Super(0);
        UINT8 value;

        *REG_SELECT_PTR = reg;
        value = *REG_READ_PTR;

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
void set_tone(unsigned int channel, unsigned int tuning) {
    /* Tuning is 1111 1111 1111 at max */
    if (channel <= 2 && tuning <= 0xFFF) {
        write_psg(channel * 2, tuning & 0xFF); /* Fine for lower 8 bits */
        write_psg(channel * 2 + 1, (tuning >> 8) & 0x0F); /* Coarse for upper 4 */
    }
}

/** See psg.h for documentation */
void set_volume(unsigned int channel, unsigned int volume) {
    if (channel <= 2 && volume <= 15) {
        write_psg(LEVEL_A + channel, volume);
    }
}

/* TODO: Below this point needs love, same with accompanying tests */
/** See psg.h for documentation */
void enable_channel(
    unsigned int channel,
    unsigned int tone_on,
    unsigned int noise_on
) {
    /* NOTE: We could remove some variables for slight speed increase at
       the cost of readability */
    if (channel <= 2 && tone_on <= 1 && noise_on <= 1) {
        /* Flip bits because it's enabled on low */
        /* Importantly, it goes CBACBA for the lower six bits, where top 3 
           are the noise bits and bottom 3 are the tone bits */
        UINT8 mask = (!noise_on << (channel + 3)) | (!tone_on << channel);
        UINT8 current_mix = read_psg(MIXER);

        /* Clear the bits for the channel we're changing */
        current_mix = current_mix & ~(0x09 << channel);
        /* Then set the new values */
        current_mix = current_mix | mask;

        write_psg(MIXER, current_mix);
    }
}

/** See psg.h for documentation */
void stop_sound() {
    UINT8 mixer;

    set_volume(0, 0);
    set_volume(1, 0);
    set_volume(2, 0);

    mixer = read_psg(MIXER);
    mixer = (UINT8)(mixer | 0x3F); /* disable tone/noise for channels A-C */
    write_psg(MIXER, mixer);
}
