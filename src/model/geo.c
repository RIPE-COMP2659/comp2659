#include "geo.h"

/* NOTE: This is only useful if we have different sprites for multiplayer */
/* The global sprite for geo */
const unsigned int GEO_SPRITE[GEO_SIZE][GEO_SIZE / WORD] = {
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

/** See geo.h for documentation */
Geo create_geo(unsigned int x, unsigned int y, unsigned int ground_y) {
    Geo geo;

    geo.ddy = GEO_DDY_SCALED;
    geo.dx = GEO_DX;
    geo.dy = 0;
    geo.is_landed = FALSE;
    geo.is_dead = FALSE;
    geo.ground_y = ground_y;
    geo.x = x;
    geo.y = y;
    geo.y_scaled = y << GEO_PHYSICS_SHIFT;
    geo.size = GEO_SIZE;
    geo.jump_buffer = 0;
    geo.sprite = GEO_SPRITE;

    return geo;
}

/* TODO: This will take some fine tuning on what feels right, but can't
   really be done until gameplay */
/** See header file for documentation */
signed int geo_check_square_collision(
    Geo *geo,
    unsigned int object_x,
    unsigned int object_y,
    unsigned int object_size
) {
    signed int geo_left = geo->x;
    signed int geo_right = geo->x + geo->size;
    signed int geo_bottom = geo->y - geo->size;
    signed int geo_top = geo->y;

    signed int object_left = object_x;
    signed int object_right = object_x + object_size;
    signed int object_top = object_y;
    signed int object_bottom = object_y - object_size;

    signed int collision_result = COLLISION_ERROR;

    /* TODO: Optimize logic after determining desired behaviour based on
       gameplay */
    if (geo_right < object_left || geo_left > object_right ||
        geo_top < object_bottom || geo_bottom > object_top) {
        collision_result = COLLISION_NONE;
    } else {
        /* penetration_depth: how many pixels Geo is 'sunken' into the block vertically.
           We use the 'Half-Block Rule' (16px) to decide between landing and crashing. */
        signed int penetration_depth = object_top - geo_bottom;

        if (geo->dy <= 0) {
            /* FALLING or STANDING: If we are in the top half of the block, prioritize landing.
               This allows sliding off edges safely and 'soft' corner landings. */
            if (penetration_depth <= 16) {
                collision_result = COLLISION_TOP;
            } else {
                collision_result = COLLISION_LEFT;
            }
        } else {
            /* JUMPING: Only trigger a death (BOTTOM collision) if our feet are physically 
               below the block's bottom. This prevents 'phantom ceiling' deaths. */
            if (geo_bottom < object_bottom) {
                collision_result = COLLISION_BOTTOM;
            } else {
                collision_result = COLLISION_NONE;
            }
        }
    }

    return collision_result;
}

/** See header file for documentation */
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

    /* Check 4 points of the spike against Geo's bounding box:
       middle 2 at the top, and bottom left/right.
       Since sy is the top, py is sy at the top and sy - 31 at the bottom. */

    /* Top peaks (checking both pixels for the apex) */
    signed int py = spike_y;
    signed int px1 = spike_x + 15;
    signed int px2 = spike_x + 16;

    if ((px1 >= gx_left && px1 <= gx_right && py <= gy_top && py >= gy_bottom) ||
        (px2 >= gx_left && px2 <= gx_right && py <= gy_top && py >= gy_bottom)) {
        return COLLISION_BOTTOM;
    }

    /* Bottom corners */
    py = spike_y - spike_size + 1;
    px1 = spike_x;
    px2 = spike_x + spike_size - 1;

    if ((px1 >= gx_left && px1 <= gx_right && py <= gy_top && py >= gy_bottom) ||
        (px2 >= gx_left && px2 <= gx_right && py <= gy_top && py >= gy_bottom)) {
        return COLLISION_BOTTOM;
    }

    return COLLISION_NONE;
}

/** See header file for documentation */
void geo_jump(Geo *geo) {
    geo->jump_buffer = 8; /* Buffer the jump for 8 frames*/
}

/** See header file for documentation */
void geo_update(Geo *geo) {
    geo->x += geo->dx;
    geo->dy += geo->ddy;

    if (geo->dy < GEO_TERMINAL_DY_SCALED) {
        geo->dy = GEO_TERMINAL_DY_SCALED;
    }

    geo->y_scaled += geo->dy;
    geo->y = geo->y_scaled >> GEO_PHYSICS_SHIFT;
    geo_update_landed(geo);

    /* Jump Buffering Logic: */
    if (geo->jump_buffer > 0) {
        if (geo->is_landed == TRUE) {
            geo->dy = GEO_JUMP_DY_SCALED;
            geo->is_landed = FALSE;
            geo->jump_buffer = 0;
        } else {
            geo->jump_buffer--;
        }
    }
}

/* TODO: This currently snaps geo up to the position of landed even if
   they're much lower, might be weird in practice. */
/** See header file for documentation */
void geo_update_landed(Geo *geo) {
    signed int geo_bottom = geo->y - geo->size;
    signed int ground_y = geo->ground_y;

    if (geo_bottom <= ground_y && geo->dy <= 0) {
        geo->is_landed = TRUE;
        geo->y = geo->ground_y + geo->size;
        geo->y_scaled = geo->y << GEO_PHYSICS_SHIFT;
        geo->dy = 0;
    } else {
        geo->is_landed = FALSE;
    }
}
