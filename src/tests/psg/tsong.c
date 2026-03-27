/**
 * tsong.c
 * Integration test that starts the song and drives it from the 70Hz clock.
 */

#include "../../psg/song.h"
#include "../../shared/dtypes.h"
#include <osbind.h>

static void wait_busy(unsigned int cycles) {
    volatile unsigned int i;
    for (i = 0; i < cycles; i++) { }
}

int main(void) {
    Cconws("tsong: Starting song, press any key to stop...\r\n");
    start_music();

    /* drive the song by calling update_music(1) repeatedly. Use Cconis to
     * detect keypresses so we don't need to link the input module. This
     * approximates advancing one vertical-blank tick per loop iteration.
     */
    Cconws("tsong: playing bourree demo (synchronous)\r\n");
    play_bourree();

    Cconws("tsong: Bourree demo complete, press any key to continue...\r\n");
    Cnecin();

    stop_music();
    Cconws("tsong: Song stopped.\r\n");
    return 0;
}
