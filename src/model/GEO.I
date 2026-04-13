# 1 "model/geo.c" 1
# 1 "model/geo.h" 1
# 1 "model/dtypes.h" 1
# 4 "model/geo.h" 2
# 55 "model/geo.h"
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
  const unsigned int (*sprite)[32  / 16 ];
} Geo;
# 85 "model/geo.h"
Geo create_geo(unsigned int x, unsigned int y, unsigned int ground_y);
# 105 "model/geo.h"
signed int geo_check_square_collision(Geo *geo, unsigned int object_x,
                                      unsigned int object_y,
                                      unsigned int object_size);
# 126 "model/geo.h"
signed int geo_check_lava_collision(
    Geo *geo,
    unsigned int lava_x,
    unsigned int lava_y,
    unsigned int lava_size
);

signed int geo_check_spike_collision(Geo *geo, unsigned int spike_x,
                                     unsigned int spike_y,
                                     unsigned int spike_size);
# 144 "model/geo.h"
void geo_jump(Geo *geo);
# 154 "model/geo.h"
void geo_update(Geo *geo);
# 165 "model/geo.h"
void geo_update_landed(Geo *geo);
# 1 "model/geo.c" 2




const unsigned int GEO_SPRITE[32 ][32  / 16 ] = {
    {0x800C, 0x3001},
    {0x3FC9, 0x93FC},
    {0x7FEB, 0xD7FE},
    {0x7FEB, 0xD7FE},
    {0x7FC9, 0x93FE},
    {0x7C0C, 0x303E},
    {0x79FF, 0xFF9E},
    {0x7BFF, 0xFFDE},
    {0x7BFF, 0xFFDE},
    {0x7BFF, 0xFFDE},
    {0x33FF, 0xFFCC},
    {0x03FF, 0xFFC0},
    {0xFFFF, 0xFFFF},
    {0x87F8, 0x1FE1},
    {0x33FB, 0xDFCC},
    {0x7BFA, 0x5FDE},
    {0x7BFA, 0x5FDE},
    {0x33FB, 0xDFCC},
    {0x87F8, 0x1FE1},
    {0xFFFF, 0xFFFF},
    {0x03FF, 0xFFC0},
    {0x33FF, 0xFFCC},
    {0x7BFF, 0xFFDE},
    {0x7BFF, 0xFFDE},
    {0x7BFF, 0xFFDE},
    {0x79FF, 0xFF9E},
    {0x7C0C, 0x303E},
    {0x7FC9, 0x93FE},
    {0x7FEB, 0xD7FE},
    {0x7FEB, 0xD7FE},
    {0x3FC9, 0x93FC},
    {0x800C, 0x3001}
};


Geo create_geo(unsigned int x, unsigned int y, unsigned int ground_y) {
    Geo geo;

    geo.ddy = -42 ;
    geo.dx = 5u ;
    geo.dy = 0;
    geo.is_landed = -1 ;
    geo.is_dead = -1 ;
    geo.ground_y = ground_y;
    geo.x = x;
    geo.y = y;
    geo.y_scaled = y << 6 ;
    geo.size = 32 ;
    geo.jump_buffer = 0;
    geo.sprite = GEO_SPRITE;

    return geo;
}




signed int geo_check_square_collision(
    Geo *geo,
    unsigned int object_x,
    unsigned int object_y,
    unsigned int object_size
) {
    signed int gx_left = geo->x;
    signed int gx_right = geo->x + geo->size - 1;
    signed int gy_top = geo->y;
    signed int gy_bot = geo->y - geo->size + 1;

    signed int ox_left = object_x;
    signed int ox_right = object_x + object_size - 1;
    signed int oy_top = object_y;
    signed int oy_bot = object_y - object_size + 1;


    if (gx_right < ox_left || gx_left > ox_right ||
        gy_bot > oy_top || gy_top < oy_bot) {
        return 0 ;
    }


    if (geo->dy <= 0) {


        if (gy_bot >= oy_top - 16) {
            return 1 ;
        } else {
            return -2 ;
        }
    } else {


        if (gy_top <= oy_bot + 16) {
            return -1 ;
        } else {
            return -2 ;
        }
    }
}


signed int geo_check_lava_collision(
    Geo *geo,
    unsigned int lava_x,
    unsigned int lava_y,
    unsigned int lava_size
) {
    signed int gx_left = geo->x + 2;
    signed int gx_right = geo->x + 29;
    signed int gx_mid = geo->x + 16;
    signed int gy_bot = geo->y - geo->size + 1;

    signed int lx_left = lava_x;
    signed int lx_right = lava_x + lava_size - 1;
    signed int ly_top = lava_y;



    if (gy_bot <= ly_top + 2) {
        if ((gx_left >= lx_left && gx_left <= lx_right) ||
            (gx_mid >= lx_left && gx_mid <= lx_right) ||
            (gx_right >= lx_left && gx_right <= lx_right)) {
            return -1 ;
        }
    }

    return 0 ;
}


signed int geo_check_spike_collision(
    Geo *geo,
    unsigned int spike_x,
    unsigned int spike_y,
    unsigned int spike_size
) {
    signed int gx_left = geo->x;
    signed int gx_right = geo->x + geo->size;
    signed int gy_top = geo->y;
    signed int gy_bottom = geo->y - geo->size;


    unsigned int py_top = spike_y;
    unsigned int py_bot = spike_y - (spike_size - 1);
    unsigned int px_apex1 = spike_x + 15;
    unsigned int px_apex2 = spike_x + 16;
    unsigned int px_left = spike_x;
    unsigned int px_right = spike_x + (spike_size - 1);
# 159 "model/geo.c"
    if ((px_apex1 >= gx_left && px_apex1 <= gx_right && py_top <= gy_top && py_top >= gy_bottom) ||
        (px_apex2 >= gx_left && px_apex2 <= gx_right && py_top <= gy_top && py_top >= gy_bottom)) {
        return -1 ;
    }


    if ((px_left >= gx_left && px_left <= gx_right && py_bot <= gy_top && py_bot >= gy_bottom) ||
        (px_right >= gx_left && px_right <= gx_right && py_bot <= gy_top && py_bot >= gy_bottom)) {
        return -1 ;
    }

    return 0 ;
}


void geo_jump(Geo *geo) {
    geo->jump_buffer = 8;
}


void geo_update(Geo *geo) {
    geo->x += geo->dx;
    geo->dy += geo->ddy;

    if (geo->dy < -666 ) {
        geo->dy = -666 ;
    }

    geo->y_scaled += geo->dy;
    geo->y = geo->y_scaled >> 6 ;
    geo_update_landed(geo);


    if (geo->jump_buffer > 0) {
        if (geo->is_landed == 0 ) {
            geo->dy = 640 ;
            geo->is_landed = -1 ;
            geo->jump_buffer = 0;
        } else {
            geo->jump_buffer--;
        }
    }
}




void geo_update_landed(Geo *geo) {
    signed int geo_bottom = geo->y - geo->size;
    signed int ground_y = geo->ground_y;

    if (geo_bottom <= ground_y && geo->dy <= 0) {
        geo->is_landed = 0 ;
        geo->y = geo->ground_y + geo->size;
        geo->y_scaled = geo->y << 6 ;
        geo->dy = 0;
    } else {
        geo->is_landed = -1 ;
    }
}
