#include "model/model.h"
#include "render/render.h"
#include "events/events.h"
#include "clock.h"
#include "oswrap.h"

#define JUMP 32
#define QUIT 113

int main_game(void)
{
    Model model = get_model();
    int quit = FALSE;
    unsigned int input;
    unsigned long timeThen, timeNow, timeElapsed;
    unsigned char *base = os_framebuffer_base();

    /* Initial render */
    render(&model, base);

    timeThen = get_time();
    while (quit != TRUE)
    {
        if (os_has_input())
        {
            input = os_get_input();
            switch (input)
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
        }

        timeNow = get_time();
        timeElapsed = timeNow - timeThen;
        if (timeElapsed > 0)
        {
            on_clock_tick(&model);
            /* event-driven checks */
            /* rendering triggered on clock tick */
            render(&model, base);
            timeThen = timeNow;
        }

        os_yield();
    }

    return 0;
}

/* Expose C main entrypoint expected by build */
int main(void) { return main_game(); }
