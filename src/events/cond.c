/*
 * cond.c
 *
 * PURPOSE: Implements conditional (collision/state-triggered)
 *          event handlers. Checks Geo against level
 *          obstacles using model functions and camera indices.
 */
#include "cond.h"

int check_level_complete(Model *model)
{
    signed int event = EVENT_NONE;

    if (model->world.geo.x >= model->world.levels[model->world.level_index].end_x) {
        event = EVENT_LEVEL_DONE;
    }

    return event;
}

/* FUTURE TODO/Nice to haves (State Transitions/Triggers):
 * void on_geo_death(World *world);    -- Stop music, play boom, start reset
 * timer void on_geo_victory(World *world);  -- Play cheer, trigger menu
 * transition void on_item_collision(Geo *geo, Item *item); if we decide to
 * implement bonus coins, etc.
 */