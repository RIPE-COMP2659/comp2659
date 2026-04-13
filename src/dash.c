#include "clock/clock.h"
#include "events/events.h"
#include "model/model.h"
#include "render/render.h"
#include "input/input.h"
#include "splash/splash.h"
#include "psg/effects.h"
#include "psg/music.h"

#define JUMP 32
#define QUIT 113

/* TODO: Death is a little bit out of sync */
int main_game(void)
{
    Model model = get_model();
    UINT8 quit = FALSE;
    UINT8 game_won = FALSE;
    signed int died_this_frame;
    signed int current_event;
    unsigned long timeThen, timeNow, timeElapsed;

    UINT8 init = splash_screen(); /* Prints splash screen and awaits user input */

    if (init == 0)
    {
        quit = TRUE; /* Early exit if exit selected */
    }
    else
    {
        /* Initialize render buffers */
        init_render_buffers();
        /* Initial render */
        render(&model, 0);

        /* Disable keyboard sound */
        toggle_keyboard_sound();

        /* Start background music (driven by update_music from the main loop) */
        start_music(SONG_MEGALO);

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

            /* NOTE: This keeps the game from crashing, no clue why, something in music timing */
            if (timeElapsed != 0)
            {

                /* Update music with elapsed VBL ticks */
                update_music(timeElapsed);

                /* Step gameplay only on odd clock ticks. */
                if ((timeNow & 1) != 0)
                {
                    died_this_frame = on_clock_tick(&model);

                    if (died_this_frame == TRUE)
                    {
                        clear_render_buffers();
                        start_music(SONG_MEGALO);
                    }

                    current_event = check_level_complete(&model);
                    if (current_event == EVENT_LEVEL_DONE)
                    {
                        stop_music();
                        play_level_complete_effect();
                        game_won = TRUE;
                    }

                    /* Initial render */
                    render(&model, 0);
                }

                /* Advance frame baseline to avoid reusing old elapsed time. */
                timeThen = timeNow;
            }
        }

        /* Renable the keyboard */
        toggle_keyboard_sound();

        /* Clean up music */
        stop_music();
    }

    return 0;
}

int main(void) { return main_game(); }
