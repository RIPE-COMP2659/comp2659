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
 * A basic test loop for testing the input module. Compiled as a .tos program.
 */

#include "../../input/input.h"
#include "../../shared/dtypes.h"
#include <osbind.h>

void test_has_get_input(void)
{
    int done = FALSE;
    char ch;

    Cconws("Input test program started. Press 'q' to quit.\r\n");

    while (done == FALSE)
    {
        if (has_input() == TRUE)
        {
            ch = get_input();

            if (ch == 'q')
            {
                done = TRUE;
                Cconws("Exiting...\r\n");
            }
            else
            {
                Cconws("Key pressed\r\n");
            }
        }
    }
}

void test_read_scancode(void)
{
    long old_ssp;
    SCANCODE sc;
    char ascii;
    char *scancode_2_ascii;
    int done = FALSE;

    /* TODO: THIS IS TOS, REPLACE WITH CUSTOM TABLE OR SIMILAR, MAYBE MATH? */
    scancode_2_ascii = (char *)((Keytbl(-1, -1, -1))->unshift);

    Cconws("Press keys to echo. Press ESC to exit.\r\n");

    old_ssp = Super(0);
    *IKBD_control = IKBD_POLLING_MODE;

    while (done == FALSE) {
        sc = read_scancode();

        if (sc == (ESC_SCANCODE | BREAK_BIT)) {
            done = TRUE;
        } else if ((sc & BREAK_BIT) == 0) {
            ascii = scancode_2_ascii[sc];
            if (ascii != '\0') {
                Cconout(ascii);
            }
        }
    }

    *IKBD_control = IKBD_RESTORE_MODE;
    Super(old_ssp);

    Cconws("\r\nExiting...\r\n");
}

int main()
{
    test_read_scancode();
    test_has_get_input();
    return 0;
}
