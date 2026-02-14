#include "geo.h"

void geo_move(Geo *geo) {
    geo->x += geo->dx;

    geo->dy += geo->ddy;
    geo->y += geo->dy;
}

/* TODO: Should probably check geo is on the ground, coming later */
void geo_jump(Geo *geo) {
    geo->dy = GEO_JUMP_DY;
}
