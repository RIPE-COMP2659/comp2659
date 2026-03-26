#include "clock/clock.h"
#include "events/events.h"
#include "model/model.h"
#include "render/render.h"
#include "input/input.h"
#include "splash/splash.h"
#include <osbind.h>

#define JUMP 32
#define QUIT 113

/* TODO: Death is a little bit out of sync */
int main_game(void)
{

    Model model = get_model();
    UINT8 quit = FALSE;
    UINT8 game_won = FALSE;
    signed int died_this_frame;
    unsigned int input;
    signed int current_event;
    unsigned long timeThen, timeNow, timeElapsed;

    UINT8 init = splash_screen();

    /* Initialize render buffers */
    init_render_buffers();

    if (init == 0)
    {
        return 0;
    }

    /* Initial render */
    render(&model, 0);

    timeThen = get_time();
    while (quit != TRUE && game_won != TRUE)
    {
        switch (get_input())
        {
        case JUMP:
            on_jump_request(&model);
            break;
        case QUIT:
            quit = TRUE;
            break;
        default:
            break;
        }

        timeNow = get_time();
        timeElapsed = timeNow - timeThen;

        died_this_frame = on_clock_tick(&model);

        if (died_this_frame == TRUE)
        {
            clear_render_buffers();
        }

        current_event = check_level_complete(&model);
        if (current_event == EVENT_LEVEL_DONE)
        {
            printf("Level Complete!\n"); /* TODO: Level complete handling gracefully */
        }

        render(&model, 0);

        timeThen = timeNow;
    }
    return 0;
}

int main(void) { return main_game(); }
