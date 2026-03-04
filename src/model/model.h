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
    unsigned int cam_min_bi; /* block index, camera */
    unsigned int cam_max_bi; /* block index, camera */
    unsigned int cam_min_si; /* spike index, camera */
    unsigned int cam_max_si; /* spike index, camera */
    unsigned int cam_min_li; /* lava index, camera */
    unsigned int cam_max_li; /* lava index, camera */
    unsigned int col_min_bi; /* block index, collision */
    unsigned int col_max_bi; /* block index, collision */
    unsigned int col_min_si; /* spike index, collision */
    unsigned int col_max_si; /* spike index, collision */
    unsigned int col_min_li; /* lava index, collision */
    unsigned int col_max_li; /* lava index, collision */
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
void model_update_camera(Model *model);

void model_update_camera_bi(Model *model);
void model_update_camera_si(Model *model);
void model_update_camera_li(Model *model);

void model_update_collision_bi(Model *model);
void model_update_collision_si(Model *model);
void model_update_collision_li(Model *model);
