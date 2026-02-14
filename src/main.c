#include <stdio.h> /* Temporary printf import */
#include "block.h"
#include "spike.h"
#include "lava.h"
#include "geo.h"

int main(void) {
    /* For now, just a placeholder */
    Block test_block = {100, 200, BLOCK_SIZE, BLOCK_SPRITE};
    Spike test_spike = {300, 400, SPIKE_SIZE, SPIKE_SPRITE};
    Lava test_lava = {500, 600, LAVA_SIZE, LAVA_SPRITE};
    Geo test_geo = { GEO_DDY, GEO_DX, 0, 100, 200, GEO_SIZE, GEO_SPRITE };

    printf("Running main!\n");

    printf("Geo x: %d\n", test_geo.x);
    printf("Geo y: %d\n", test_geo.y);
    printf("Geo dx: %d\n", test_geo.dx);
    printf("Geo dy: %d\n", test_geo.dy);
    printf("Geo ddy: %d\n", test_geo.ddy);

    geo_move(&test_geo);

    printf("Geo x: %d\n", test_geo.x);
    printf("Geo y: %d\n", test_geo.y);
    printf("Geo dx: %d\n", test_geo.dx);
    printf("Geo dy: %d\n", test_geo.dy);
    printf("Geo ddy: %d\n", test_geo.ddy);

    geo_jump(&test_geo);
    geo_move(&test_geo);

    printf("Geo x after jump and move: %d\n", test_geo.x);
    printf("Geo y after jump and move: %d\n", test_geo.y);
    printf("Geo dx: %d\n", test_geo.dx);
    printf("Geo dy: %d\n", test_geo.dy);
    printf("Geo ddy: %d\n", test_geo.ddy);

    geo_move(&test_geo);

    printf("Geo x after move: %d\n", test_geo.x);
    printf("Geo y after move: %d\n", test_geo.y);

    return 0;
}