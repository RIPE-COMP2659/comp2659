#include <stdio.h> /* Temporary printf import */
#include "world.h"

int main(void) {
    /* For now, just a placeholder */
    Level levels[1];
    Geo geo = { GEO_DDY, GEO_DX, 0, 100, 200, GEO_SIZE, GEO_SPRITE };
    World world;

    levels[0] = get_level1();
    world.levels = levels;
    world.geo = geo;
    world.ground_y = 0;

    printf("Running main!\n");

    printf("Level has %d block size \n", 
        world.levels[0].blocks[0].size
    );

    printf("Geo x: %d\n", world.geo.x);
    printf("Geo y: %d\n", world.geo.y);
    printf("Geo dx: %d\n", world.geo.dx);
    printf("Geo dy: %d\n", world.geo.dy);
    printf("Geo ddy: %d\n", world.geo.ddy);

    geo_move(&world.geo);

    printf("Geo x: %d\n", world.geo.x);
    printf("Geo y: %d\n", world.geo.y);
    printf("Geo dx: %d\n", world.geo.dx);
    printf("Geo dy: %d\n", world.geo.dy);
    printf("Geo ddy: %d\n", world.geo.ddy);

    geo_jump(&world.geo);
    geo_move(&world.geo);

    printf("Geo x after jump and move: %d\n", world.geo.x);
    printf("Geo y after jump and move: %d\n", world.geo.y);
    printf("Geo dx: %d\n", world.geo.dx);
    printf("Geo dy: %d\n", world.geo.dy);
    printf("Geo ddy: %d\n", world.geo.ddy);

    geo_move(&world.geo);

    printf("Geo x after move: %d\n", world.geo.x);
    printf("Geo y after move: %d\n", world.geo.y);

    return 0;
}