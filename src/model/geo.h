#include "dtypes.h"

/* The length and width of geo in pixels */
#define GEO_SIZE 32

/**
 * Represents the player character, Geo. Might have more than one if we do
 * two player
 * 
 * signed int ddy:
 *     The change in vertical speed per update, effectively gravity
 * signed int dx:
 *     The change in horizontal position per update, constant horiztonal
 *     scrolling
 * signed int dy:
 *     The change in vertical position per update, affected by jumping and
 *     landing. Set to their respective values on the given events
 * signed int is_landed:
 *     Whether geo is currently landed on the ground, which affects whether
 *     they can jump or is subject to gravity
 * signed int is_dead:
 *     Whether geo is currently dead, will display a game over or another
 *     attempt will be given in the future
 * unsigned int ground_y
 *     The y value of the ground, either a block or the ground of the level,
 *     determines if geo has landed
 * unsigned int x:
 *     The world x value of the top left of Geo
 * unsigned int y:
 *     The world y value of the top of Geo
 * unsigned int size:
 *     The length and width of Geo in pixels
 * const unsigned int (*sprite)[GEO_SIZE / WORD]:
 *     The pointer to the sprite of Geo, a 32x32 pixel image
 */
typedef struct {
    signed int ddy;
    signed int dx;
    signed int dy;
    signed int is_landed;
    signed int is_dead;
    unsigned int ground_y;
    unsigned int x;
    unsigned int y;
    unsigned int size;
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
signed int geo_check_square_collision(
    Geo *geo,
    unsigned int object_x,
    unsigned int object_y,
    unsigned int object_size
);

/**
 * Makes Geo jump by setting dy to a constant value if Geo is currently landed
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
