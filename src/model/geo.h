#ifndef GEO_H
#define GEO_H

#include "dtypes.h"

/* The length and width of geo in pixels */
#define GEO_SIZE 32

/* The values used to scale the vertical movement of geo to have a roughly
   2 block jump height and 1 second duration during a 70 FPS game. This is
   because the original Impossible Game had roughly that exact design and it
   would allow us to steal a large amount of their level design */
#define GEO_DX 5u               /*pixels per frame (unscaled) */
#define GEO_JUMP_DY_SCALED 610 /* Necessary combo with DDY */
#define GEO_DDY_SCALED -42     /* For height and time */
#define GEO_TERMINAL_DY_SCALED -666 /* -13 pixels * 64 */
#define GEO_PHYSICS_SHIFT 6    /* Bit shifting scaled values, / or * by 64 */
/**
 * Represents the player character, Geo. Might have more than one if we do
 * two player. See the #defines for an explanation on some of the fields
 * regarding the vertical position, some less intuitive math was done to
 * avoid floating point numbers / non-integer division
 *
 * signed int ddy:
 *     The change in dy per update. Note that this is not scaled relative
 *     to vertical position, but instead it is scaled relative to a roughly
 *     2 block jump height and 1 second duration during a 70 FPS game
 * signed int dx:
 *     The change in horizontal position per update, constant horizontal
 *     scrolling.
 * signed int dy:
 *     The change in y per update. It is not scalred relative to vertical
 *     position, it is scaled for a 2 block jump height and 1 second duration
 *     during a 70 FPS game
 * signed int is_landed:
 *     Whether geo is currently landed on the ground, which affects whether
 *     they can jump or is subject to gravity
 * signed int is_dead:
 *     Whether geo is currently dead, will display a game over or another
 *     attempt will be given in the future
 * unsigned int ground_y:
 *     The y value of the ground, either a block or the ground of the level,
 *     determines if geo has landed
 * unsigned int x:
 *     The world x value of the top left of Geo
 * unsigned int y:
 *     The world y value of the top of Geo
 * signed int y_scaled:
 *     The scaled y value of the top of Geo, see header defines for details
 * unsigned int size:
 *     The length and width of Geo in pixels
 * unsigned int jump_buffer:
 *     The number of frames in which a jump can be queued even if Geo can't
 *     yet jump
 * const unsigned int (*sprite)[GEO_SIZE / WORD]:
 *     The pointer to the sprite of Geo, a 32x32 pixel image
 */
typedef struct {
  signed int ddy;
  unsigned int dx;
  signed int dy;
  signed int is_landed;
  signed int is_dead;
  unsigned int ground_y;
  unsigned int x;
  unsigned int y;
  unsigned int y_scaled;
  unsigned int size;
  unsigned int jump_buffer;
  const unsigned int (*sprite)[GEO_SIZE / WORD];
} Geo;

/**
 * Creates a Geo with the given x, y, and ground_y values, with default values
 * for the remaining fields
 *
 * Params:
 *     unsigned int x:
 *        The world x value of the top left of Geo
 *     unsigned int y:
 *        The world y value of the top of Geo
 *     unsigned int ground_y:
 *        The y value of the ground, either a block or the ground of the level
 *
 * Returns:
 *     Geo: A Geo with the given x, y, and ground_y values
 */
Geo create_geo(unsigned int x, unsigned int y, unsigned int ground_y);

/**
 * Checks if Geo is colliding with a square object
 *
 * Params:
 *     Geo *geo:
 *        The Geo to check for collision
 *     unsigned int object_x:
 *        The world x value of the top left of the object
 *     unsigned int object_y:
 *        The world y value of the top of the object
 *     unsigned int object_size:
 *        The length and width of the object in pixels
 *
 * Returns:
 *     signed int:
 *       COLLISION_NONE, COLLISION_TOP, COLLISION_BOTTOM, COLLISION_LEFT, or
 *       COLLISION_ERROR as defined in dtypes.h
 */
signed int geo_check_square_collision(Geo *geo, unsigned int object_x,
                                      unsigned int object_y,
                                      unsigned int object_size);

/**
 * Checks if Geo is colliding with a spike (triangle) object
 *
 * Params:
 *     Geo *geo:
 *        The Geo to check for collision
 *     unsigned int spike_x:
 *        The world x value of the top left of the spike bounding box
 *     unsigned int spike_y:
 *        The world y value of the top of the spike
 *     unsigned int spike_size:
 *        The length and width of the spike bounding box in pixels
 *
 * Returns:
 *     signed int:
 *       COLLISION_NONE or COLLISION_BOTTOM (representing death)
 */
signed int geo_check_spike_collision(Geo *geo, unsigned int spike_x,
                                     unsigned int spike_y,
                                     unsigned int spike_size);

/**
 * Makes Geo jump by setting the jump buffer, which will queue a jump
 *
 * Params:
 *     Geo *geo:
 *         The Geo to make jump
 */
void geo_jump(Geo *geo);

/**
 * Updates Geo's position based on dy, dx, and ddy, as well as delegating
 * to geo_update_landed.
 *
 * Params:
 *     Geo *geo:
 *         The Geo to update
 */
void geo_update(Geo *geo);

/**
 * Updates Geo's is_landed status based on their position relative to the
 * ground, snaps them to the ground level if they have landed, and dy to 0
 * if they have landed, based on geo.ground_y
 *
 * Params:
 *     Geo *geo:
 *         The Geo to update landed status for
 */
void geo_update_landed(Geo *geo);

#endif
