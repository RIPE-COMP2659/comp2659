/**
 * tikbd.c
 *
 * Authors:
 *     Riley Gramlich, rgram060@mtroyal.ca, 201762060
 *     Robert Parker Hutcheson, rhutc335@mtroyal.ca, 201762335
 *     Isaac Klein, iklei977@mtroyal.ca, 201763977
 *     Eduard Mykhailets, emykh268@mtroyal.ca, 201750268
 * Course: COMP 2659-001, Computing Machinery II, Winter 2026
 * Instructor: Nolan Shaw
 *
 * Basic integration test for IKBD vector installation.
 */

#include <osbind.h>
#include "../../input/ikbd.h"

static void print_hex32(UINT32 value) {
  int shift;

  Cconws("0x");
  for (shift = 28; shift >= 0; shift -= 4) {
    UINT8 nibble = (UINT8)((value >> shift) & 0x0FUL);
    if (nibble < 10U) {
      Cconout((int)('0' + nibble));
    } else {
      Cconout((int)('A' + (nibble - 10U)));
    }
  }
}

static void print_vector_line(const char *label, Vector vec) {
  Cconws(label);
  print_hex32((UINT32)vec);
  Cconws("\r\n");
}

int main(void) {
  long old_ssp;
  Vector original;
  Vector installed;
  volatile const UINT32 *vector_table;

  Cconws("IKBD vector install test started.\r\n");

  old_ssp = Super(0);
  original = installVector(IKBD_ISR, ikbdisr);
  vector_table = (volatile const UINT32 *)0x0UL;
  installed = (Vector)vector_table[IKBD_ISR];
  installVector(IKBD_ISR, original);
  Super(old_ssp);

  print_vector_line("Original IKBD vector:  ", original);
  print_vector_line("Installed IKBD vector: ", installed);

  if (original != (Vector)0L) {
    Cconws("Vector install/restore succeeded.\r\n");
  } else {
    Cconws("Original vector was null.\r\n");
  }

  Cconws("Press any key to exit...\r\n");
  Cnecin();

  return 0;
}
