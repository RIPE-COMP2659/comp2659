#include "clock.h"
#include "events/events.h"
#include "model/model.h"
#include "oswrap.h"
#include "render/render.h"

#define JUMP 32
#define QUIT 113

int main_game(void) {

  /* Initialize the game models and variables */
  Model model = get_model();
  UINT8 quit = FALSE;
  UINT8 game_won = FALSE;
  unsigned int input; /* this maybe larger than it needs to be- I was just being
  careful so that nothing breaks */

  unsigned long timeThen, timeNow, timeElapsed;
  unsigned char *base = os_framebuffer_base();

  /* Initial render */
  render(&model, base);

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

    on_clock_tick(&model); /* Update the game model state */
    /* event-driven checks */
    /* rendering triggered on clock tick */
    render(&model, base);
    timeThen = timeNow;

    os_yield();
  }

  return 0;
}

/* Expose C main entrypoint expected by build */
int main(void) { return main_game(); }
