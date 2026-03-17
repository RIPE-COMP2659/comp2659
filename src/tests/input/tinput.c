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
#include "../../model/dtypes.h"
#include <osbind.h>

int main() {
  int done = FALSE;
  char ch;

  Cconws("Input test program started. Press 'q' to quit.\r\n");

  while (done == FALSE) {
    if (has_input() == TRUE) {
      ch = get_input();

      if (ch == 'q') {
        done = TRUE;
        Cconws("Exiting...\r\n");
      } else {
        Cconws("Key pressed\r\n");
      }
    }
  }

  return 0;
}
