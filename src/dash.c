#include "clock/clock.h"
#include "events/events.h"
#include "model/model.h"
#include "render/render.h"
#include "input/input.h"
#include "psg/effects.h"
#include <osbind.h>
#include <stdio.h>
#include "psg/music.h"

#define JUMP 32

void main_game(void);

int main()
{
    main_game();
    return 0;
}

void main_game(void)
{
    Model model;
    int quit;
    int game_won;
    UINT32 timeThen;
    UINT32 timeNow;
    UINT32 timeElapsed;
    int died_this_frame;
    signed int current_event;
    int inputChar;

    /* Initialize all buffers and pointers at start */
    init_render_buffers();

    /* Perform all assignments after declarations (C89 Style) */
    model = get_model();
    quit = FALSE;
    game_won = FALSE;
    died_this_frame = FALSE;

    /* Disable keyboard sound */
    toggle_keyboard_sound();

    /* Start background music (driven by update_music from the main loop) */
    start_music(SONG_GLORIA);

    timeThen = get_time();

    while (quit != TRUE && game_won != TRUE)
    {
        /* --- 1. Polling Input (Every iteration for responsiveness) --- */
        if (Cconis() != 0)
        {
            inputChar = (int)Cnecin();
            if (inputChar == JUMP)
            {
                on_jump_request(&model);
            }
            else if (inputChar == 'q' || inputChar == 'Q')
            {
                quit = TRUE;
            }
        }

        timeNow = get_time();
        timeElapsed = timeNow - timeThen;

        /* --- 2. 35 FPS Logic Block (Execute every 2 ticks @ 70Hz) --- */
        if (timeElapsed >= 2)
        {
            /* Tick 1 Physics */
            update_music(1);
            died_this_frame = on_clock_tick(&model);
            if (died_this_frame == TRUE) {
                clear_render_buffers();
                start_music(SONG_GLORIA);
            }

            /* Tick 2 Physics */
            update_music(1);
            died_this_frame = on_clock_tick(&model);
            if (died_this_frame == TRUE) {
                clear_render_buffers();
                start_music(SONG_GLORIA);
            }

            /* Check level completion */
            current_event = check_level_complete(&model);
            if (current_event == EVENT_LEVEL_DONE) {
                stop_music();
                play_level_complete_effect();
                printf("Level Complete!\n");
                game_won = TRUE;
            }

            /* --- 3. Render visuals once per 35 FPS cycle --- */
            render(&model, NULL);

            timeThen = timeNow;
        }
    }

    /* Renable the keyboard */
    toggle_keyboard_sound();
    stop_sound();
}
