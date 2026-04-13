# 1 "model/camera.c" 1
# 1 "model/camera.h" 1
# 1 "model/dtypes.h" 1
# 4 "model/camera.h" 2
# 19 "model/camera.h"
typedef struct {
    unsigned int x;
    unsigned int y;
    unsigned int width;
    unsigned int height;
} Camera;
# 42 "model/camera.h"
Camera create_camera(unsigned int x, unsigned int y);
# 56 "model/camera.h"
signed int camera_get_relative_x(Camera *camera, unsigned int world_x);
# 70 "model/camera.h"
signed int camera_get_relative_y(Camera *camera, unsigned int world_y);
# 86 "model/camera.h"
void camera_update_coordinates(Camera *camera, unsigned int x, unsigned int y);
# 1 "model/camera.c" 2


Camera create_camera(unsigned int x, unsigned int y) {
    Camera camera;

    camera.x = x;
    camera.y = y;
    camera.width = 640 ;
    camera.height = 400 ;

    return camera;
}

signed int camera_get_relative_x(Camera *camera, unsigned int object_x) {
    return object_x - camera->x;
}

signed int camera_get_relative_y(Camera *camera, unsigned int object_y) {
    return camera->y - object_y;
}

void camera_update_coordinates(Camera *camera, unsigned int x, unsigned int y) {
    camera->x = x;
    camera->y = y;
}
