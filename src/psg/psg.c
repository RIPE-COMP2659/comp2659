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
#define REG_WRITE_PTR   ((volatile char *)0xFF8802)

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

int main()
{
    write_psg(A_FINE, 248);		/* set channel A fine tune = 248 */
    write_psg(A_COARSE, 50);	/* set channel A coarse tune = 50 */
    write_psg(MIXER, 0x3E);	/* enable channel A on mixer */
    write_psg(LEVEL_A, 11);	/* set channel A volume = 11 */

	while (!Cconis())		/* tone now playing, await key */
		;

    write_psg(LEVEL_A, 0);	/* set channel A volume = 0 */

	Cnecin();
	return 0;
}
