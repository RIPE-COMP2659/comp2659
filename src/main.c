#include "events/events.h"
#include "input.h"
#include "model/model.h"
#include "raster/raster.h"
#include "render/render.h"
#include <osbind.h>
#include <stdio.h> /* Temporary printf import */


#define BYTES_PER_SCREEN 32000

/* Only works on Atari ST, comment out starting here */
/* End commenting out here */

void test_rendering(UINT8 *base, Model *model) {
    World *world = &model->world;
    char input_char;
    signed int current_event;
    int is_running = TRUE;

    /* Start on Level 1 */
    world->level_index = 0;

    /* Run continuously until 'q' or 'Q' is pressed */
    while (is_running == TRUE) {
        /* Vsync first to align with screen refresh and stabilize frame time */
        Vsync();

        if (has_input() == TRUE) {
            input_char = get_input();
            if (input_char == 'q' || input_char == 'Q') {
                is_running = FALSE;
            } else if (input_char == ' ') {
                on_jump_request(model);
            }
        }

        if (is_running == TRUE) {
            on_clock_tick(model);

            /* Handle game state based on events */
            current_event = check_level_complete(model);
            if (current_event == EVENT_LEVEL_DONE) {
                /* Level Complete! Exit or go to next level */
                is_running = FALSE;
            }

            if (world->geo.is_dead == TRUE) {
                /* Restart on death */
                world_reset_level(world);
            }

            render(model, base);
        }
    }
}

void disable_cursor() {
    printf("\033f");
    fflush(stdout);
}

void fill_screen(UINT32 *base, char pattern) {
    register int i = 0;
    register UINT32 *loc = base;

    while (i++ < BYTES_PER_SCREEN / 4)
        *(loc++) = pattern;
}

void test_clear_screen(UINT8 *base) { clear_screen((UINT32 *)base); }

void test_clear_region(UINT8 *base) {
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
    UINT8 *base = (UINT8 *)Physbase();
    Model model = get_model();

    test_rendering(base, &model);

    return 0;
}
