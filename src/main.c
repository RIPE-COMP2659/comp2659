#include <stdio.h> /* Temporary printf import */
#include <osbind.h>
#include "model/model.h"
#include "raster/raster.h"
#include "renderer/renderer.h"

#define BYTES_PER_SCREEN 32000

/* Only works on Atari ST, comment out starting here */
/* End commenting out here */

void test_rendering(UINT8 *base, Model *model)
{
    World *world = &model->world;
    unsigned int w_iter;

    for (w_iter = 0; w_iter < 200; w_iter++) {
        /* 1. Update Model Logic */
        model_update(model);

        /* 2. Clear Buffer */
        clear_screen((UINT32 *)base);

        /* 3. Render World State using the new renderer module */
        render(world, base);

        /* Debug Info */
        printf("Geo x: %d (Frame: %d)\n", world->geo.x, w_iter);
        
        /* Wait for input to step through frames (optional) */
        /* Cnecin(); */
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
    Model model = get_model();
    World *world = &model.world;

    /* Run the rendering test loop (now properly encapsulated) */
    test_rendering(base, &model);
    
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
        world->levels[0].blocks[0].size
    );

    printf("Geo x: %d\n", world->geo.x);
    printf("Geo y: %d\n", world->geo.y);
    printf("Geo dx: %d\n", world->geo.dx);
    printf("Geo dy: %d\n", world->geo.dy);
    printf("Geo ddy: %d\n", world->geo.ddy);
    printf("Geo is_landed: %d\n", world->geo.is_landed);

    geo_update(&world->geo);
    printf("Geo is_landed: %d\n", world->geo.is_landed);
    geo_update(&world->geo);
    printf("Geo is_landed: %d\n", world->geo.is_landed);
    geo_update(&world->geo);

    printf("Geo x: %d\n", world->geo.x);
    printf("Geo y: %d\n", world->geo.y);
    printf("Geo dx: %d\n", world->geo.dx);
    printf("Geo dy: %d\n", world->geo.dy);
    printf("Geo ddy: %d\n", world->geo.ddy);
    printf("Geo is_landed: %d\n", world->geo.is_landed);

    geo_jump(&world->geo);
    geo_update(&world->geo);

    printf("Geo x after jump and move: %d\n", world->geo.x);
    printf("Geo y after jump and move: %d\n", world->geo.y);
    printf("Geo dx: %d\n", world->geo.dx);
    printf("Geo dy: %d\n", world->geo.dy);
    printf("Geo ddy: %d\n", world->geo.ddy);
    printf("Geo is_landed: %d\n", world->geo.is_landed);

    geo_jump(&world->geo);
    geo_jump(&world->geo);
    geo_jump(&world->geo);
    geo_update(&world->geo);

    printf("Geo x after move: %d\n", world->geo.x);
    printf("Geo y after move: %d\n", world->geo.y);
    printf("Geo is_landed: %d\n", world->geo.is_landed);

    return 0;
}