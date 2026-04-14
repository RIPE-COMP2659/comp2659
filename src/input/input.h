/**
 * input.h
 *
 * Authors:
 *     Riley Gramlich, rgram060@mtroyal.ca, 201762060
 *     Robert Parker Hutcheson, rhutc335@mtroyal.ca, 201762335
 *     Isaac Klein, iklei977@mtroyal.ca, 201763977
 *     Eduard Mykhailets, emykh268@mtroyal.ca, 201750268
 * Course: COMP 2659-001, Computing Machinery II, Winter 2026
 * Instructor: Nolan Shaw
 *
 * Header file for the high-level input module, providing game-independent
 * routines to process user input without OS-specific calls in the main game
 * logic.
 */
#include <osbind.h>
#include "../shared/dtypes.h"
#include "ikbd.h"

extern volatile UINT8 * const IKBD_control;
extern volatile const UINT8 * const IKBD_status;
extern volatile const SCANCODE * const IKBD_RDR;

/**
 * Checks if there is pending input to be processed.
 *
 * Params:
 *     None
 *
 * Returns:
 *     int: TRUE if input is available, FALSE otherwise.
 */
int has_input();

/**
 * Gets the next character of input to be processed.
 *
 * Params:
 *     None
 *
 * Returns:
 *     SCANCODE: The next queued scancode.
 */
SCANCODE get_input(void);

SCANCODE read_scancode(void);

void init_input(void);

void handle_ikbd_byte(void);

void restore_input(void);

/**
 * Returns the current absolute mouse X coordinate, clamped to [0, SCREEN_WIDTH-1].
 * Reads from the global updated by the IKBD ISR.
 */
int get_mouse_x(void);

/**
 * Returns the current absolute mouse Y coordinate, clamped to [0, SCREEN_HEIGHT-1].
 * Reads from the global updated by the IKBD ISR.
 */
int get_mouse_y(void);

/**
 * Returns the current mouse button state byte from the last packet header.
 * Bit 1 (0x02) = left button pressed; bit 0 (0x01) = right button pressed.
 */
UINT8 get_mouse_buttons(void);
