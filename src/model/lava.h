#ifndef LAVA_H
#define LAVA_H

#include "dtypes.h"

/* TODO: Lava is an individual block and should be a big object to optimize
   space */

/* The length and width of a lava object */
#define LAVA_SIZE 32

/**
 * Represents a single lava tile, should be optimized later to take up less 
 * and properly use width, but is currently sufficient
 *
 * unsigned int x:
 *     The world x value of the top left of the lava
 * unsigned int y:
 *     The world y value of the top left of the lava
 * unsigned int size:
 *     The size of the lava tile, defaults to 32 like other objects
 * const unsigned int (*sprite)[LAVA_SIZE / WORD]:
 *     The sprite pointers for the lava tile
 */
typedef struct {
    unsigned int x;
    unsigned int y;
    unsigned int size;
    const unsigned int (*sprite)[LAVA_SIZE / WORD];
} Lava;

/**
 * Creates a lava tile with the given x and y values, with default values for
 * remaining fields.
 *
 * Params:
 *     unsigned int x:
 *       The world x value of the top left of the lava
 *    unsigned int y:
 *      The world y value of the top left of the lava
 *
 * Returns:
 *     Lava: A lava tile with the given x and y values
 */
Lava create_lava(unsigned int x, unsigned int y);

#endif
