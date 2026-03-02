#include "world.h"

/**
 * Model wrapper entity for the game's world state
 *
 * World world:
 *     The wrapped World object containing levels, geo, camera, and
 *     collision-related indices
 */
typedef struct {
    World world;
} Model;

/**
 * Creates a model with the default world state, thin wrapper by assignment
 * specifications
 *
 * Params:
 *     None
 *
 * Returns:
 *    Model: Constructed object containing a default-initialized world
 */
Model get_model(void);

/**
 * Updates the model by delegating to world_update with level index 0
 *
 * Params:
 *     Model *model:
 *         Pointer to the model to update
 *
 * Returns:
 *    None
 */
void model_update(Model *model);
