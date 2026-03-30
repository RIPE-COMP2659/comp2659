#ifndef ASYNC_H
#define ASYNC_H

#include "../model/model.h"

/**
 * Handles the asynchronous jump event triggered by the spacebar. Delegates
 * to geo_jump
 *
 * Params:
 *     Model *model:
 *         The model that contains geo to be passed to geo_jump, assumed to
 *         be non-null
 */
void on_jump_request(Model *model);

#endif
