#include "dtypes.h"

typedef struct {
    /* Camera position (top-left corner of viewport in world coordinates) */
    unsigned int x;
    unsigned int y;
    unsigned int width;
    unsigned int height;
} Camera;

/* Create camera with initial position and screen bounds */
Camera create_camera(unsigned int x, unsigned int y);

/* Update camera position, for following Geo */
void camera_update_coordinates(Camera *camera, unsigned int x, unsigned int y);

/* Convert world x to relative screen x */
signed int camera_get_relative_x(Camera *camera, unsigned int world_x);

/* Convert world y to relative screen y */
signed int camera_get_relative_y(Camera *camera, unsigned int world_y);
