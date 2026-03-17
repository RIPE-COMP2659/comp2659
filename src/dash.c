#include "clock.h"
#include "events/events.h"
#include "model/model.h"
#include "oswrap.h"
#include "render/render.h"
#include <osbind.h>

#define JUMP 32
#define QUIT 113

int main_game(void) {

  Model model = get_model();
  UINT8 quit = FALSE;
  UINT8 game_won = FALSE;
  unsigned int input;

  unsigned long timeThen, timeNow, timeElapsed;

  /* Initialize render buffers */
  init_render_buffers();

  /* Initial render */
  render(&model, 0);

  timeThen = get_time();
  while (quit != TRUE && game_won != TRUE) {
    if (os_has_input()) {
      input = os_get_input();
      switch (input) {
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

    on_clock_tick(&model);
    render(&model, 0);

    /* Wait for VBL, then swap buffers */
    Vsync();
    swap_buffers();

    timeThen = timeNow;

    os_yield();
  }

  return 0;
}

int main(void) { return main_game(); }
