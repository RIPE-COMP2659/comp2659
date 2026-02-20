#include "geo.h"

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

    geo.ddy = GEO_DDY;
    geo.dx = GEO_DX;
    geo.dy = GEO_DY;
    geo.is_landed = FALSE;
    geo.ground_y = ground_y;
    geo.x = x;
    geo.y = y;
    geo.size = GEO_SIZE;
    geo.sprite = GEO_SPRITE;

    return geo;
}

void geo_move(Geo *geo) {
    geo->x += geo->dx;

    geo->dy += geo->ddy;
    geo->y += geo->dy;
}

/* TODO: Should probably check geo is on the ground, coming later */
void geo_jump(Geo *geo) {
    geo->dy = GEO_JUMP_DY;
}
