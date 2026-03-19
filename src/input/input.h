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
#include "../model/dtypes.h"

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
 *     char: The character that was pressed by the user.
 */
char get_input();
