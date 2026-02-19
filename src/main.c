#include <stdio.h> /* Temporary printf import */
#include "geo.h"
#include "level.h"

int main(void) {
    /* For now, just a placeholder */
    Level level = create_level1();
    Geo test_geo = { GEO_DDY, GEO_DX, 0, 100, 200, GEO_SIZE, GEO_SPRITE };

    printf("Running main!\n");

    printf("Level has %d block size \n", 
        level.blocks[0].size
    );

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