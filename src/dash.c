#include "clock/clock.h"
#include "events/events.h"
#include "model/model.h"
#include "render/render.h"
#include "input/input.h"
#include "splash/splash.h"
#include "psg/effects.h"
#include "psg/music.h"

#define JUMP SC_SPACE
#define QUIT SC_Q

/* TODO: Death is a little bit out of sync */
int main_game(void)
{
    Model model = get_model();
    UINT8 quit = FALSE;
    UINT8 game_won = FALSE;
    UINT8 init;
    signed int died_this_frame;
    unsigned int input;
    signed int current_event;
    unsigned long timeThen, timeNow, timeElapsed;

    init = splash_screen(); /* Prints splash screen and awaits user input */
    init_input(); /* Initializes input handling and installs IKBD ISR */

    if (init == 0)
    {
        quit = TRUE; /* Early exit if exit selected */
    }
    else
    {
        UINT8 jumping = FALSE;
        SCANCODE sc;

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
            /* Silly-while consumes both make and breaks properly
               otherwise jumping logic becomes far more verbose */
            /* Not ideal, but necessary for time constraints */
            while ((sc = get_input()) != 0) {
                switch (sc) {
                    case JUMP:
                        jumping = TRUE;
                        break;
                    case JUMP | BREAK_BIT:
                        jumping = FALSE;
                        break;
                    case QUIT:
                        quit = TRUE;
                        break;
                    default:
                        break;
                }
            }

            if (jumping == TRUE) {
                on_jump_request(&model);
            }

            timeNow = get_time();
            timeElapsed = timeNow - timeThen;
            timeThen = timeNow; /* update here, before on_clock_tick's death wait */

            /* NOTE: This keeps the game from crashing, no clue why, something in music timing */
            if (timeElapsed == 0)
            {
                timeElapsed = 1;
            }

            /* Update music with elapsed VBL ticks */
            update_music(timeElapsed);

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

        /* Renable the keyboard */
        toggle_keyboard_sound();

        /* Clean up music */
        stop_music();

        restore_input();

        return 0;
    }

    restore_input(); /* Restore original IKBD vector */

    return 0;
}

int main(void) { return main_game(); }
