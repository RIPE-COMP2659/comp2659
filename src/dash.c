#include "clock/clock.h"
#include "events/events.h"
#include "model/model.h"
#include "render/render.h"
#include "input/input.h"
#include <osbind.h>

#define JUMP 32
#define QUIT 113

/* TODO: Death is a little bit out of sync */
int main_game(void)
{

    Model model = get_model();
    UINT8 quit = FALSE;
    UINT8 game_won = FALSE;
    unsigned int input;
    signed int current_event;
    unsigned long timeThen, timeNow, timeElapsed;

    /* Initialize render buffers */
    init_render_buffers();

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

        on_clock_tick(&model);

        current_event = check_level_complete(&model);
        if (current_event == EVENT_LEVEL_DONE) {
            printf("Level Complete!\n"); /* TODO: Level complete handling gracefully */
        }

        render(&model, 0);

        /* I know its a weird order, but for some reason this way works,
         * and the way that makes sense in my head does not.
         */
        swap_buffers();
        Vsync();

        timeThen = timeNow;
    }
    return 0;
}

int main(void) { return main_game(); }
