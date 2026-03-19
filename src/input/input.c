/**
 * input.c
 * Authors:
 *     Riley Gramlich, rgram060@mtroyal.ca, 201762060
 *     Robert Parker Hutcheson, rhutc335@mtroyal.ca, 201762335
 *     Isaac Klein, iklei977@mtroyal.ca, 201763977
 *     Eduard Mykhailets, emykh268@mtroyal.ca, 201750268
 * Course: COMP 2659-001, Computing Machinery II, Winter 2026
 * Instructor: Nolan Shaw
 *
 * Implementation of the high-level input module. This handles the dependency
 * injection or wrapping of OS-specific keyboard routines like Cconis and
 * Cnecin.
 */

#include "input.h"
#include <osbind.h>
int has_input() {
  int result = FALSE;

  if (Cconis() != 0) {
    result = TRUE;
  }

  return result;
}

char get_input() {
  char result = '\0';

  if (has_input() == TRUE) {
    result = (char)Cnecin();
  }

  return result;
}
