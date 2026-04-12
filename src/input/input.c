/**
 * input.c
 *
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

volatile UINT8 * const IKBD_control = IKBD_CTRL_ADDR;
volatile const UINT8 * const IKBD_status = IKBD_STATUS_ADDR;
volatile const SCANCODE * const IKBD_RDR = IKBD_RDR_ADDR;

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

SCANCODE read_scancode(void) {
    while ((*IKBD_status & IKBD_SR_RDRF) == 0)
        ;
    return *IKBD_RDR;
}
