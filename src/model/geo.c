#include "geo.h"

#define VO_TOLERANCE ((-1 * GEO_TERMINAL_DY_SCALED) >> 6)

/* NOTE: This is only useful if we have different sprites for multiplayer */
/* The global sprite for geo */
const unsigned int GEO_SPRITE[GEO_SIZE][GEO_SIZE / WORD] = {
    {0x7FE3, 0xC7FE},
    {0xC036, 0x6403},
    {0x9FD5, 0xA5F9},
    {0xBFD5, 0xA5FD},
    {0xB836, 0x661D},
    {0xB3E3, 0xC3CD},
    {0xB600, 0x006D},
    {0xB400, 0x002D},
    {0xB400, 0x002D},
    {0xB400, 0x002D},
    {0xCC00, 0x0033},
    {0x7800, 0x001E},
    {0x0000, 0x0000},
    {0x7807, 0xE01E},
    {0xCC04, 0x2033},
    {0xB405, 0xA02D},
    {0xB405, 0xA02D},
    {0xCC04, 0x2033},
    {0x7807, 0xE01E},
    {0x0000, 0x0000},
    {0x7800, 0x001E},
    {0xCC00, 0x0033},
    {0xB400, 0x002D},
    {0xB400, 0x002D},
    {0xB400, 0x002D},
    {0xB600, 0x006D},
    {0xB3E3, 0xC7CD},
    {0xB836, 0x6C1D},
    {0xBFD5, 0xABFD},
    {0x9FD5, 0xABF9},
    {0xC036, 0x6C03},
    {0x7FE3, 0xC7FE}
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

/** See header file for documentation */
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

    /* AABB Quick Check */
    if (gx_right < ox_left || gx_left > ox_right ||
        gy_bot > oy_top || gy_top < oy_bot) {
        return COLLISION_NONE;
    }

    /* POINT-BASED PRIORITY: Check Geo's corners against the block */
    if (geo->dy <= 0) {
        /* Moving Down/Still: Check bottom corners for landing */
        /* Use 16px threshold (Half-Block) for landing vs crashing into sides */
        if (gy_bot >= oy_top - 16) {
            return COLLISION_TOP;
        } else {
            return COLLISION_LEFT;
        }
    } else {
        /* Moving Up: Check top corners for head-strike */
        /* Only die if we are physically hitting the bottom of the block from underneath */
        if (gy_top <= oy_bot + 16) {
            return COLLISION_BOTTOM;
        } else {
            return COLLISION_LEFT;
        }
    }
}

signed int geo_check_lava_collision(
    Geo *geo,
    unsigned int lava_x,
    unsigned int lava_y,
    unsigned int lava_size
) {
    signed int gx_left = geo->x + 2;   /* Inset feet slightly */
    signed int gx_right = geo->x + 29;
    signed int gx_mid = geo->x + 16;
    signed int gy_bot = geo->y - geo->size + 1;

    signed int lx_left = lava_x;
    signed int lx_right = lava_x + lava_size - 1;
    signed int ly_top = lava_y;

    /* Optimization: Only check 3 points on the bottom surface against the top of lava */
    /* Add a 2-pixel 'surface' threshold so standing ON lava kills you */
    if (gy_bot <= ly_top + 2) {
        if ((gx_left >= lx_left && gx_left <= lx_right) ||
            (gx_mid >= lx_left && gx_mid <= lx_right) ||
            (gx_right >= lx_left && gx_right <= lx_right)) {
            return COLLISION_BOTTOM; /* Death */
        }
    }

    return COLLISION_NONE;
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

    /* Points to check: Apex (tip) and the two bottom corners */
    unsigned int py_top = spike_y;
    unsigned int py_bot = spike_y - (spike_size - 1);
    unsigned int px_apex1 = spike_x + 15;
    unsigned int px_apex2 = spike_x + 16;
    unsigned int px_left = spike_x;
    unsigned int px_right = spike_x + (spike_size - 1);

    /* Check Tip */
    if ((px_apex1 >= gx_left && px_apex1 <= gx_right && py_top <= gy_top && py_top >= gy_bottom) ||
        (px_apex2 >= gx_left && px_apex2 <= gx_right && py_top <= gy_top && py_top >= gy_bottom)) {
        return COLLISION_BOTTOM;
    }

    /* Check Bottom Corners (to catch the sides/base) */
    if ((px_left >= gx_left && px_left <= gx_right && py_bot <= gy_top && py_bot >= gy_bottom) ||
        (px_right >= gx_left && px_right <= gx_right && py_bot <= gy_top && py_bot >= gy_bottom)) {
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

  /* TODO: Revisit clamping, might be able to simplify physics */
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
      play_jump_effect();
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
