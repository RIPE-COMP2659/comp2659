/**
 * tmusic.c
 * Authors:
 * Riley Gramlich, rgram060@mtroyal.ca, 201762060
 * Robert Parker Hutcheson, rhutc335@mtroyal.ca, 201762335
 * Isaac Klein, iklei977@mtroyal.ca, 201763977
 * Eduard Mykhailets, emykh268@mtroyal.ca, 201750268
 * Course: COMP 2659-001, Computing Machinery II, Winter 2026
 * Instructor: Nolan Shaw
 *
 * Integration testing for the asynchronous music module.
 */

#include <osbind.h>
#include "../../psg/music.h"
#include "../../shared/dtypes.h"
#include "../../clock/clock.h" /* To use get_time() */

/**
 * Helper function that simulates the timing of the actual game loop.
 * It will play the provided song until any key is pressed.
 */
static void test_song(SongChoice song, const char* song_name) {
    unsigned long timeThen, timeNow, timeElapsed;

    Cconws("Now Playing: ");
    Cconws(song_name);
    Cconws("\r\nPress any key to stop this track...\r\n");

    /* 1. Start the music asynchronously */
    start_music(song);

    timeThen = get_time();

    /* 2. Run a simulated game loop until a key is pressed */
    while (!Cconis()) {
        timeNow = get_time();
        timeElapsed = timeNow - timeThen;

        /* Throttle just like the real game loop */
        if (timeElapsed > 0) {
            
            /* 3. Feed the elapsed ticks to the music engine */
            update_music(timeElapsed);
            
            timeThen = timeNow;
        }
    }

    /* Clear the keyboard buffer so the next prompt works */
    Cnecin(); 

    /* 4. Stop the music gracefully */
    stop_music();
    
    Cconws("Stopped ");
    Cconws(song_name);
    Cconws(".\r\n\n");
}

int main(void) {
    /* Clear screen to make output readable */
    Cconws("\033E"); 
    Cconws("--- Music Module Integration Test ---\r\n\n");

    /* Test Track 1: Tetris */
    test_song(SONG_TETRIS, "Tetris Theme (Korobeiniki)");

    /* Test Track 2: Bach's Bourree */
    test_song(SONG_BOURREE, "Bach's Bourree in E minor");
    
    /* Test Track 3: Hot Cross Buns */
    test_song(SONG_HOT_CROSS_BUNS, "Hot Cross Buns");
    
    /* Test Track 4: Gloria */
    test_song(SONG_GLORIA, "Gloria");

    /* Test Track 5: In the Hall of the Mountain King */
    test_song(SONG_MOUNTAIN_KING, "In the Hall of the Mountain King");

    /* Test Track 6: Megalo */
    test_song(SONG_MEGALO, "Megalo");
    
    Cconws("All music tests complete.\r\n");
    Cconws("Press any key to exit to DOS...\r\n");
    Cnecin();

    return 0;
}