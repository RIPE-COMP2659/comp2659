#include "dtypes.h"

/* NOTE: Missing some documentation on the funny business with how these
 * offsets work when passed to raster functions. */

/**
 * Camera entity which will follow Geo, important to have distinct for two 
 * player mode and for vertical scrolling. Not to be confused with 
 * rendering even if it assists in computation for screen positions
 * 
 * unsigned int x:
 *     The world x coordinate of the top-left corner of the camera
 * unsigned int y:
 *     The world y coordinate of the top-left corner of the camera
 */
typedef struct {
    unsigned int x;
    unsigned int y;
    unsigned int width;
    unsigned int height;
} Camera;

/**
 * Creates a camera with the given initial position and screen bounds
 * 
 * Params:
 *     unsigned int x:
 *         The initial x coordinate of the camera in the world
 *     unsigned int y:
 *         The initial y coordinate of the camera in the world
 *     unsigned int width:
 *         The width of the camera viewport
 *     unsigned int height:
 *         The height of the camera viewport
 * 
 * Returns:
 *     Camera: The respective x, y, width, and height
 */
Camera create_camera(unsigned int x, unsigned int y);

/**
 * Converts world x to relative screen x
 * 
 * Params:
 *     Camera *camera:
 *         Pointer to the camera object
 *     unsigned int world_x:
 *         The x coordinate in the world
 * 
 * Returns:
 *     signed int: The x coordinate relative to the camera's viewport
 */
signed int camera_get_relative_x(Camera *camera, unsigned int world_x);

/**
 * Converts world y to relative screen y
 * 
 * Params:
 *     Camera *camera:
 *         Pointer to the camera object
 *     unsigned int world_y:
 *         The y coordinate in the world
 * 
 * Returns:
 *     signed int: The y coordinate relative to the camera's viewport
 */
signed int camera_get_relative_y(Camera *camera, unsigned int world_y);

/**
 * Update this camera's coordinates to the given x and y, almost exclusively
 * for following Geo. To some extent, almost worth asking if this should just
 * pass in Geo. However, it might get tricky with vertical scrolling logic
 * without the camera being aware of the world's ground level.
 * 
 * Params:
 *     Camera *camera:
 *         Pointer to the camera object to update
 *     unsigned int x:
 *        The new x coordinate of the camera in the world
 *     unsigned int y:
 *        The new y coordinate of the camera in the world
 */
void camera_update_coordinates(Camera *camera, unsigned int x, unsigned int y);
