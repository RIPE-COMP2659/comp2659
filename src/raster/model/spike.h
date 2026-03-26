#ifndef SPIKE_H
#define SPIKE_H

#include "../shared/dtypes.h"

/* The size of a spike, length and width, in pixels */
#define SPIKE_SIZE 32

/**
 * A spike in the game, a single triangle, that causes death upon touching it.
 * Notably, this is currently treated as a square despite appearing as a 
 * triangle until we implement better collision detection
 * 
 * unsigned int x:
 *     The world x coordinate for the top-left of the spike "square"
 * unsigned int y:
 *     The world y coordinate for the top-left of the spike "square"
 * unsigned int size:
 *     The size of the spike, in pixels
 * unsigned int (*sprite)[SPIKE_SIZE / WORD]:
 *     A pointer to the sprite for the spike, all spikes share the same sprite
 */
typedef struct {
  unsigned int x;
  unsigned int y;
  unsigned int size;
  const unsigned int (*sprite)[SPIKE_SIZE / WORD];
} Spike;

/**
 * Creates a spike with the default size and sprite
 * 
 * Params:
 *     unsigned int x:
 *         The world x coordinate for the top-left of the spike "square"
 *     unsigned int y:
 *         The world y coordinate for the top-left of the spike "square"
 * 
 * Returns:
 *     Spike:
 *         A spike struct with the given x and y, and default size and sprite
 */
Spike create_spike(unsigned int x, unsigned int y);

#endif
