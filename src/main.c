#include <stdio.h> /* Temporary printf import */
#include <osbind.h>
#include "entities/world.h"
#include "raster/raster.h"

#define BYTES_PER_SCREEN 32000

/* Only works on Atari ST, comment out starting here */
/* End commenting out here */

void test_rendering(UINT32 *base, World world)
{
    unsigned int i;
    unsigned int w_iter;

    for (w_iter = 0; w_iter < 200; w_iter++) {
        world_update(&world, 0);

        clear_screen(base);

        /* Test Plotting Ground */
        /* TODO: These are getting incompatible pointer type warnings, left for another time, it works */
        /* TODO: Without hardcoding 0, this causes a crash. */
        plot_rectangle((UINT32 *)base, camera_get_relative_y(&world.camera, world.ground_y), 0, 4, SCREEN_WIDTH);

        /* Test Blocks */
        for (i = world.cam_min_bi; i < world.cam_max_bi; i++) {
            Block block = world.levels[0].blocks[i];
            plot_bitmap_32((UINT8 *)base, camera_get_relative_y(&world.camera, block.y), camera_get_relative_x(&world.camera, block.x), block.sprite, block.size);
        }

        /* Test Spikes */
        for (i = world.cam_min_si; i < world.cam_max_si; i++) {
            Spike spike = world.levels[0].spikes[i];
            plot_bitmap_32((UINT8 *)base, camera_get_relative_y(&world.camera, spike.y), camera_get_relative_x(&world.camera, spike.x), spike.sprite, spike.size);
        }

        /* Test Lava */
        for (i = world.cam_min_li; i < world.cam_max_li; i++) {
            Lava lava = world.levels[0].lava[i];
            plot_bitmap_32((UINT8 *)base, camera_get_relative_y(&world.camera, lava.y), camera_get_relative_x(&world.camera, lava.x), lava.sprite, lava.size);
        }

        /* Test Geo */
        plot_bitmap_32((UINT8 *)base, camera_get_relative_y(&world.camera, world.geo.y), camera_get_relative_x(&world.camera, world.geo.x), world.geo.sprite, world.geo.size);

        printf("Geo x: %d\n", world.geo.x);
        Cnecin();
    }
}

void disable_cursor()
{
    printf("\033f");
    fflush(stdout);
}

void fill_screen(UINT32 *base, char pattern)
{
    register int i = 0;
    register UINT32 *loc = base;

    while (i++ < BYTES_PER_SCREEN / 4)
        *(loc++) = pattern;
}

void test_clear_screen(UINT8 *base)
{
    clear_screen((UINT32 *)base);
}

void test_clear_region(UINT8 *base)
{
    int i;

    /* Fill screen with white first */
    fill_screen((UINT32 *)base, -1);

    /* Test 1: 48x48 optimized path - word aligned */
    clear_region((UINT32 *)base, 0, 0, 48, 48);

    /* Test 2: 48x48 optimized path - different position */
    clear_region((UINT32 *)base, 50, 80, 48, 48);

    /* Test 3: Small region - unoptimized path */
    clear_region((UINT32 *)base, 10, 5, 10, 10);

    /* Test 4: Wide region - unoptimized path */
    clear_region((UINT32 *)base, 100, 50, 20, 100);

    /* Test 5: Tall narrow region */
    clear_region((UINT32 *)base, 150, 200, 50, 8);

    /* Test 6: Odd column alignment to test byte spanning */
    clear_region((UINT32 *)base, 200, 5, 15, 4);
}

int main(void) {
    /* For now, just a placeholder */
    UINT8 *base = (UINT8 *)Physbase();
    World world = get_world();

    test_rendering((UINT32 *)base, world);

    disable_cursor();

    /* Test 1: Clear Screen */
    fill_screen((UINT32 *)base, -1); /* fill with white */
    Cnecin();
    test_clear_screen(base);
    Cnecin();

    /* Test 2: Clear Region - various sizes and positions */
    test_clear_region(base);
    Cnecin();

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

    geo_update(&world.geo);
    printf("Geo is_landed: %d\n", world.geo.is_landed);
    geo_update(&world.geo);
    printf("Geo is_landed: %d\n", world.geo.is_landed);
    geo_update(&world.geo);

    printf("Geo x: %d\n", world.geo.x);
    printf("Geo y: %d\n", world.geo.y);
    printf("Geo dx: %d\n", world.geo.dx);
    printf("Geo dy: %d\n", world.geo.dy);
    printf("Geo ddy: %d\n", world.geo.ddy);
    printf("Geo is_landed: %d\n", world.geo.is_landed);

    geo_jump(&world.geo);
    geo_update(&world.geo);

    printf("Geo x after jump and move: %d\n", world.geo.x);
    printf("Geo y after jump and move: %d\n", world.geo.y);
    printf("Geo dx: %d\n", world.geo.dx);
    printf("Geo dy: %d\n", world.geo.dy);
    printf("Geo ddy: %d\n", world.geo.ddy);
    printf("Geo is_landed: %d\n", world.geo.is_landed);

    geo_jump(&world.geo);
    geo_jump(&world.geo);
    geo_jump(&world.geo);
    geo_update(&world.geo);

    printf("Geo x after move: %d\n", world.geo.x);
    printf("Geo y after move: %d\n", world.geo.y);
    printf("Geo is_landed: %d\n", world.geo.is_landed);

    return 0;
}