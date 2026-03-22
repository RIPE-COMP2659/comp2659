/**
 * model.c
 * Authors:
 *     Riley Gramlich, rgram060@mtroyal.ca, 201762060
 *     Robert Parker Hutcheson, rhutc335@mtroyal.ca, 201762335
 *     Isaac Klein, iklei977@mtroyal.ca, 201763977
 *     Eduard Mykhailets, emykh268@mtroyal.ca, 201750268
 * Course: COMP 2659-001, Computing Machinery II, Winter 2026
 * Instructor: Nolan Shaw
 *
 * PURPOSE: Top-level model operations which coordinate camera updates,
 *          world updates and collision indexing. Exposes `get_model`
 *          and `model_update` used by the synchronous event handlers.
 */
#include "model.h"

Model get_model(void)
{
    Model model;
    model.cam_min_bi = 0; /* block index, camera */
    model.cam_max_bi = 0; /* block index, camera */
    model.cam_min_si = 0; /* spike index, camera */
    model.cam_max_si = 0; /* spike index, camera */
    model.cam_min_li = 0; /* lava index, camera */
    model.cam_max_li = 0; /* lava index, camera */
    model.cam_min_bi_prev = 0; /* block index, camera, previous update */
    model.cam_max_bi_prev = 0; /* block index, camera, previous update */
    model.cam_min_si_prev = 0; /* spike index, camera, previous update */
    model.cam_max_si_prev = 0; /* spike index, camera, previous update */
    model.cam_min_li_prev = 0; /* lava index, camera, previous update */
    model.cam_max_li_prev = 0; /* lava index, camera, previous update */
    model.col_min_bi = 0; /* block index, collision */
    model.col_max_bi = 0; /* block index, collision */
    model.col_min_si = 0; /* spike index, collision */
    model.col_max_si = 0; /* spike index, collision */
    model.col_min_li = 0; /* lava index, collision */
    model.col_max_li = 0; /* lava index, collision */
    model.world = get_world();
    model.prev_cam = model.world.camera;
    model.prev_geo = model.world.geo;
    return model;
}

static void model_reset_indices(Model *model)
{
    /* TODO: The cam_*_prev don't need to happen, no time to resolve bug */
    /* Ideally the indices would not need the previous state */
    /* Instead something like rendering might increase the min index */
    model->cam_min_bi = 0;
    model->cam_max_bi = 0;
    model->cam_min_si = 0;
    model->cam_max_si = 0;
    model->cam_min_li = 0;
    model->cam_max_li = 0;
    model->cam_min_bi_prev = 0;
    model->cam_max_bi_prev = 0;
    model->cam_min_si_prev = 0;
    model->cam_max_si_prev = 0;
    model->cam_min_li_prev = 0;
    model->cam_max_li_prev = 0;
    model->col_min_bi = 0;
    model->col_max_bi = 0;
    model->col_min_si = 0;
    model->col_max_si = 0;
    model->col_min_li = 0;
    model->col_max_li = 0;
}

signed int model_update(Model *model)
{
    signed int reset_occurred;

    model->prev_cam = model->world.camera;
    model->prev_geo = model->world.geo;
    model->cam_min_bi_prev = model->cam_min_bi;
    model->cam_max_bi_prev = model->cam_max_bi;
    model->cam_min_si_prev = model->cam_min_si;
    model->cam_max_si_prev = model->cam_max_si;
    model->cam_min_li_prev = model->cam_min_li;
    model->cam_max_li_prev = model->cam_max_li;

    world_update(&model->world);
    model_update_collision(model);
    world_update_collisions(&model->world, model->col_min_bi, model->col_max_bi,
                            model->col_min_si, model->col_max_si,
                            model->col_min_li, model->col_max_li);
    reset_occurred = model_check_death(model);
    model_update_camera(model);

    return reset_occurred;
}

signed int model_check_death(Model *model)
{
    signed int return_value;

    if (model->world.geo.is_dead == TRUE)
    {
        world_reset_level(&model->world);
        model_reset_indices(model);
        return_value = TRUE;
    } else {
        return_value = FALSE;
    }

    return return_value;
}

void model_update_collision(Model *model)
{
    model_update_collision_bi(model);
    model_update_collision_si(model);
    model_update_collision_li(model);
}

void model_update_camera(Model *model) {
    model_update_camera_bi(model);
    model_update_camera_si(model);
    model_update_camera_li(model);
}

/* Generic helper function to update entity indices within a given range */
static void update_entity_indices(
    unsigned int *min, unsigned int *max, unsigned int range_left,
    unsigned int range_right, unsigned int entity_count,
    unsigned int (*get_x)(void *, unsigned int),
    unsigned int (*get_size)(void *, unsigned int), void *entities)
{
    unsigned int i;

    /* Find max first: iterate from current max to find last entity with left edge
     * within range */
    i = *max;
    while (i < entity_count && get_x(entities, i) < range_right)
    {
        i++;
    }

    /* Set max to the last entity before the one that's out of range */
    *max = (i > 0) ? i - 1 : 0;

    /* Find min: iterate from current min (up to new max) to find first entity
     * with right edge within range */
    i = *min;
    while (i <= *max &&
           get_x(entities, i) + get_size(entities, i) <= range_left)
    {
        i++;
    }

    *min = i;
}

/* Helper functions for blocks */
static unsigned int get_block_x(void *blocks, unsigned int index)
{
    return ((Block *)blocks)[index].x;
}

static unsigned int get_block_size(void *blocks, unsigned int index)
{
    return ((Block *)blocks)[index].size;
}

/* Helper functions for spikes */
static unsigned int get_spike_x(void *spikes, unsigned int index)
{
    return ((Spike *)spikes)[index].x;
}

static unsigned int get_spike_size(void *spikes, unsigned int index)
{
    return ((Spike *)spikes)[index].size;
}

/* Helper functions for lava */
static unsigned int get_lava_x(void *lava, unsigned int index)
{
    return ((Lava *)lava)[index].x;
}

static unsigned int get_lava_size(void *lava, unsigned int index)
{
    return ((Lava *)lava)[index].size;
}

void model_update_camera_bi(Model *model)
{
    Level *level = &model->world.levels[model->world.level_index];
    unsigned int camera_left = model->world.camera.x;
    unsigned int camera_right = model->world.camera.x + model->world.camera.width;

    update_entity_indices(&model->cam_min_bi, &model->cam_max_bi, camera_left,
                          camera_right, level->blocks_size, get_block_x,
                          get_block_size, level->blocks);
}

void model_update_camera_si(Model *model)
{
    Level *level = &model->world.levels[model->world.level_index];
    unsigned int camera_left = model->world.camera.x;
    unsigned int camera_right = model->world.camera.x + model->world.camera.width;

    update_entity_indices(&model->cam_min_si, &model->cam_max_si, camera_left,
                          camera_right, level->spikes_size, get_spike_x,
                          get_spike_size, level->spikes);
}

void model_update_camera_li(Model *model)
{
    Level *level = &model->world.levels[model->world.level_index];
    unsigned int camera_left = model->world.camera.x;
    unsigned int camera_right = model->world.camera.x + model->world.camera.width;

    update_entity_indices(&model->cam_min_li, &model->cam_max_li, camera_left,
                          camera_right, level->lava_size, get_lava_x,
                          get_lava_size, level->lava);
}

void model_update_collision_bi(Model *model)
{
    Level *level = &model->world.levels[model->world.level_index];
    unsigned int collision_left = model->world.geo.x;
    unsigned int collision_right = model->world.geo.x + model->world.geo.size;

    update_entity_indices(&model->col_min_bi, &model->col_max_bi, collision_left,
                          collision_right, level->blocks_size, get_block_x,
                          get_block_size, level->blocks);
}

void model_update_collision_si(Model *model)
{
    Level *level = &model->world.levels[model->world.level_index];
    unsigned int collision_left = model->world.geo.x;
    unsigned int collision_right = model->world.geo.x + model->world.geo.size;

    update_entity_indices(&model->col_min_si, &model->col_max_si, collision_left,
                          collision_right, level->spikes_size, get_spike_x,
                          get_spike_size, level->spikes);
}

void model_update_collision_li(Model *model)
{
    Level *level = &model->world.levels[model->world.level_index];
    unsigned int collision_left = model->world.geo.x;
    unsigned int collision_right = model->world.geo.x + model->world.geo.size;

    update_entity_indices(&model->col_min_li, &model->col_max_li, collision_left,
                          collision_right, level->lava_size, get_lava_x,
                          get_lava_size, level->lava);
}
