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

void test_write_psg(void) {
    write_psg(0, 248); /* set channel A fine tune = 248 */
    write_psg(1, 50); /* set channel A coarse tune = 50 */
    write_psg(7, 0x3E); /* enable channel A on mixer */
    write_psg(8, 11); /* set channel A volume = 11 */

    Cconws("test_write_psg: Tone should be playing, "
           "press any key to stop...\r\n");
    while (!Cconis())
        ;

    write_psg(8, 0); /* set channel A volume = 0 */

    Cconws("test_write_psg: Tone should have stopped, "
           "press any key to continue...\r\n");
    while (!Cconis())
        ;
}

int main() {
    Cconws("PSG test started...\r\n");

    test_write_psg();

    return 0;
}
