#include <stdio.h> /* Temporary printf import */
#include "world.h"

int main(void) {
    /* For now, just a placeholder */
    World world = get_world();

    printf("Running main!\n");

    printf("Level has %u block size \n", 
        world.levels[0].blocks[0].size
    );

    printf("Geo x: %d\n", world.geo.x);
    printf("Geo y: %d\n", world.geo.y);
    printf("Geo dx: %d\n", world.geo.dx);
    printf("Geo dy: %d\n", world.geo.dy);
    printf("Geo ddy: %d\n", world.geo.ddy);
    printf("Geo is_landed: %d\n", world.geo.is_landed);

    geo_move(&world.geo);
    printf("Geo is_landed: %d\n", world.geo.is_landed);
    geo_move(&world.geo);
    printf("Geo is_landed: %d\n", world.geo.is_landed);
    geo_move(&world.geo);

    printf("Geo x: %d\n", world.geo.x);
    printf("Geo y: %d\n", world.geo.y);
    printf("Geo dx: %d\n", world.geo.dx);
    printf("Geo dy: %d\n", world.geo.dy);
    printf("Geo ddy: %d\n", world.geo.ddy);
    printf("Geo is_landed: %d\n", world.geo.is_landed);

    geo_jump(&world.geo);
    geo_move(&world.geo);

    printf("Geo x after jump and move: %d\n", world.geo.x);
    printf("Geo y after jump and move: %d\n", world.geo.y);
    printf("Geo dx: %d\n", world.geo.dx);
    printf("Geo dy: %d\n", world.geo.dy);
    printf("Geo ddy: %d\n", world.geo.ddy);
    printf("Geo is_landed: %d\n", world.geo.is_landed);

    geo_jump(&world.geo);
    geo_jump(&world.geo);
    geo_jump(&world.geo);
    geo_move(&world.geo);

    printf("Geo x after move: %d\n", world.geo.x);
    printf("Geo y after move: %d\n", world.geo.y);
    printf("Geo is_landed: %d\n", world.geo.is_landed);

    return 0;
}