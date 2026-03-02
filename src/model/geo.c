#include "geo.h"

/* TODO: Comments, refactoring, cleanup. */
const unsigned int GEO_SPRITE[GEO_SIZE][GEO_SIZE / WORD] = {
    {0x800C, 0x3001}, /* 1 */
    {0x3FC9, 0x93FC},
    {0x7FEB, 0xD7FE},
    {0x7FEB, 0xD7FE},
    {0x7FC9, 0x93FE},
    {0x7C0C, 0x303E},
    {0x79FF, 0xFF9E},
    {0x7BFF, 0xFFDE},/* 8 */
    {0x7BFF, 0xFFDE},
    {0x7BFF, 0xFFDE},
    {0x33FF, 0xFFCC},
    {0x03FF, 0xFFC0},
    {0xFFFF, 0xFFFF},
    {0x87F8, 0x1FE1},
    {0x33FB, 0xDFCC},
    {0x7BFA, 0x5FDE},/* 16 */
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
    {0x800C, 0x3001} /* 32 */
};

Geo create_geo(unsigned int x, unsigned int y, unsigned int ground_y) {
    Geo geo;

    geo.ddy = -1;
    geo.dx = 1;
    geo.dy = 0;
    geo.is_landed = FALSE;
    geo.is_dead = FALSE;
    geo.ground_y = ground_y;
    geo.x = x;
    geo.y = y;
    geo.size = GEO_SIZE;
    geo.sprite = GEO_SPRITE;

    return geo;
}

/* TODO: This will take some fine tuning on what feels right, but can't really be done until gameplay */
signed int geo_check_square_collision(Geo *geo, unsigned int object_x, unsigned int object_y, unsigned int object_size) {
    signed int geo_left = geo->x;
    signed int geo_right = geo->x + geo->size;
    signed int geo_bottom = geo->y - geo->size;
    signed int geo_top = geo->y;

    signed int object_left = object_x;
    signed int object_right = object_x + object_size;
    signed int object_top = object_y;
    signed int object_bottom = object_y - object_size;

    signed int collision_result = COLLISION_ERROR;

    /* TODO: Optimize logic after determining desired behaviour based on gameplay */
    if (geo_right < object_left || geo_left > object_right || geo_top < object_bottom || geo_bottom > object_top) {
        /* No overlap, collision impossible */
        collision_result = COLLISION_NONE;
    } else {
        /* Collision must have occured */
        unsigned int horizontal_overlap;
        unsigned int vertical_overlap;

        /* case1: geo right is less than object right, in which case difference is geo right minus object left */
        if (geo_right < object_right) {
            horizontal_overlap = geo_right - object_left;
        /* case2: geo left is greater than object left, in which case difference is geo left minus object right */
        } else {
            horizontal_overlap = object_right - geo_left;
        }

        /* case1: geo top is less than object top, in which case difference is geo top minus object bottom */
        if (geo_top < object_top) {
            vertical_overlap = geo_top - object_bottom;
        /* case2: geo bottom is greater than object bottom, in which case difference is object top minus geo bottom */
        } else {
            vertical_overlap = object_top - geo_bottom;
        }

        /* When there is more overlap vertically than there is horizontally, it's a left-collision */
        if (vertical_overlap > horizontal_overlap) {
            collision_result = COLLISION_LEFT;
        } else {
            /* Otherwise, it's either a top or bottom collision */
            /* It's a top collision when geo is above the object */
            if (geo_bottom > object_bottom) {
                collision_result = COLLISION_TOP;
            } else {
                collision_result = COLLISION_BOTTOM;
            }
        }
    }

    return collision_result;
}

void geo_update(Geo *geo) {
    geo->x += geo->dx;

    geo->dy += geo->ddy;
    geo->y += geo->dy;

    geo_update_landed(geo);
}

void geo_jump(Geo *geo) {
    if (geo->is_landed == TRUE) {
        geo->dy = 10;
    }
}

/* TODO: This currently snaps geo up to the position of landed even if they're much lower, might be weird in practice. */
void geo_update_landed(Geo *geo) {
    signed int geo_bottom = geo->y - geo->size;
    signed int ground_y = geo->ground_y;

    if (geo_bottom <= ground_y) {
        geo->is_landed = TRUE;
        geo->y = geo->ground_y + geo->size;
        geo->dy = 0;
    } else {
        geo->is_landed = FALSE;
    }
}


