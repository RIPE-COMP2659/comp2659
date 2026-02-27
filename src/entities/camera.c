#include "camera.h"

Camera create_camera(unsigned int x, unsigned int y) {
    Camera camera;

    camera.x = x;
    camera.y = y;
    camera.width = SCREEN_WIDTH;
    camera.height = SCREEN_HEIGHT;

    return camera;
}

void camera_update_coordinates(Camera *camera, unsigned int x, unsigned int y) {
    camera->x = x;
    camera->y = y;
}

signed int camera_get_relative_x(Camera *camera, unsigned int object_x) {
    return object_x - camera->x;
}

signed int camera_get_relative_y(Camera *camera, unsigned int object_y) {
    return camera->y - object_y;
}
