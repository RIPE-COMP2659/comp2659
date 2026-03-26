/**
 * camera.c
 * Authors:
 *     Riley Gramlich, rgram060@mtroyal.ca, 201762060
 *     Robert Parker Hutcheson, rhutc335@mtroyal.ca, 201762335
 *     Isaac Klein, iklei977@mtroyal.ca, 201763977
 *     Eduard Mykhailets, emykh268@mtroyal.ca, 201750268
 * Course: COMP 2659-001, Computing Machinery II, Winter 2026
 * Instructor: Nolan Shaw
 *
 * PURPOSE: Camera utilities. Implements creation and coordinate
 *          transformation helpers used by the renderer and model.
 */
#include "camera.h"

Camera create_camera(unsigned int x, unsigned int y)
{
    Camera camera;

    camera.x = x;
    camera.y = y;
    camera.width = SCREEN_WIDTH;
    camera.height = SCREEN_HEIGHT;

    return camera;
}

signed int camera_get_relative_x(Camera *camera, unsigned int object_x)
{
    return object_x - camera->x;
}

signed int camera_get_relative_y(Camera *camera, unsigned int object_y)
{
    return camera->y - object_y;
}

void camera_update_coordinates(Camera *camera, unsigned int x, unsigned int y)
{
    camera->x = x;
    camera->y = y;
}
