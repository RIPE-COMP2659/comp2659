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

void test_write_psg(void) {
    write_psg(0, 248); /* set channel A fine tune = 248 */
    write_psg(1, 1); /* set channel A coarse tune = 1 */
    write_psg(7, 0x3E); /* enable channel A on mixer */
    write_psg(8, 11); /* set channel A volume = 11 */
    Cconws("test_write_psg: Tone should be playing, "
           "press any key to stop...\r\n");
    Cnecin();

    write_psg(8, 0); /* set channel A volume = 0 */
    Cconws("test_write_psg: Tone should have stopped, "
           "press any key to continue...\r\n");
    Cnecin();

    write_psg(16, 0);
    Cconws("test_write_psg: Invalid register write should have no change, "
           "press any key to continue...\r\n");
    Cnecin();
}

void test_read_psg(void) {
    UINT8 fine;
    UINT8 coarse;

    write_psg(0, 248); /* set channel A fine tune = 248 */
    write_psg(1, 1); /* set channel A coarse tune = 1 */

    fine = read_psg(0);
    if (fine == 248) {
        Cconws("test_read_psg: Fine passed...\r\n");
    } else {
        Cconws("test_read_psg: Fine failed, expected 248, got ");
        print_uint((unsigned int)fine);
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
    set_tone(0, 504);
    write_psg(8, 11);
    Cnecin();
}

int main() {
    Cconws("PSG test started...\r\n");

    test_write_psg();
    test_read_psg();
    test_set_tone();

    return 0;
}
