#ifndef MODEL_H
#define MODEL_H

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
    Camera old_cam;   /* camera from last model_update, for computing camera deltas */
    Camera prev_cam;  /* camera from 2 updates ago (intermediate shift) */
    Camera stale_cam; /* camera from 2 updates ago used to clear the stale buffer */
    Geo old_geo;      /* geo from last model_update */
    Geo prev_geo;     /* geo from 2 updates ago (intermediate shift) */
    Geo stale_geo;    /* geo from 2 updates ago used to clear the stale buffer */
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
 * Updates the model by delegating to model_update_camera, world_update,
 * model_update_collision, world_update_collisions, model_check_death in
 * order. The main game world updating loop
 *
 * Params:
 *     Model *model:
 *         Pointer to the model to update
 */
signed int model_update(Model *model);

/**
 * Checks if Geo is dead, and if so, resets the level. Called each frame in
 * model_update after updating the world and collisions
 */
signed int model_check_death(Model *model);

/**
 * Updates the camera indices within the model, delegating to each of the
 * model_update_camera_* functions for each entity type
 *
 * Params:
 *     Model *model:
 *         Pointer to the model to update
 */
void model_update_camera(Model *model);

/**
 * Updates the camera indices within the model, for blocks, cam_min_bi and
 * cam_max_bi, keeps tracks of the minimum and maximum indices to be
 * incremented as geo moves through the level
 * 
 * Params:
 *     Model *model:
 *         Pointer to the model to update
 */
void model_update_camera_bi(Model *model);

/**
 * Updates the camera indices within the model, for spikes, cam_min_si and
 * cam_max_si, keeps tracks of the minimum and maximum indices to be
 * incremented as geo moves through the level
 * 
 * Params:
 *     Model *model:
 *         Pointer to the model to update
 */
void model_update_camera_si(Model *model);

/**
 * Updates the camera indices within the model, for lava, cam_min_li and
 * cam_max_li, keeps tracks of the minimum and maximum indices to be
 * incremented as geo moves through the level
 * 
 * Params:
 *     Model *model:
 *         Pointer to the model to update
 */
void model_update_camera_li(Model *model);

/**
 * Updates the collision indices within the model, delegating to each of the
 * model_update_collision_* functions for each entity type
 *
 * Params:
 *     Model *model:
 *         Pointer to the model to update
 */
void model_update_collision(Model *model);

/**
 * Updates the collision indices within the model, for blocks, col_min_bi and
 * col_max_bi, keeps tracks of the minimum and maximum indices to be
 * incremented as geo moves through the level
 * 
 * Params:
 *     Model *model:
 *         Pointer to the model to update
 */
void model_update_collision_bi(Model *model);

/**
 * Updates the collision indices within the model, for spikes, col_min_si and
 * col_max_si, keeps tracks of the minimum and maximum indices to be
 * incremented as geo moves through the level
 * 
 * Params:
 *     Model *model:
 *         Pointer to the model to update
 */
void model_update_collision_si(Model *model);

/**
 * Updates the collision indices within the model, for lava, col_min_li and
 * col_max_li, keeps tracks of the minimum and maximum indices to be
 * incremented as geo moves through the level
 * 
 * Params:
 *     Model *model:
 *         Pointer to the model to update
 */
void model_update_collision_li(Model *model);

#endif
