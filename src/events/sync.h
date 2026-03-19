#include "../model/model.h"

/**
 * Called once per frame (1/70th of a second), and delegates to
 * model_update() for updating the game state
 *
 * Params:
 *     Model *model:
 *         The game model to be updated, assumed not to be null.
 */
signed int on_clock_tick(Model *model);
