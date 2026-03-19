/**
 * cond.c
 * Authors:
 *     Riley Gramlich, rgram060@mtroyal.ca, 201762060
 *     Robert Parker Hutcheson, rhutc335@mtroyal.ca, 201762335
 *     Isaac Klein, iklei977@mtroyal.ca, 201763977
 *     Eduard Mykhailets, emykh268@mtroyal.ca, 201750268
 * Course: COMP 2659-001, Computing Machinery II, Winter 2026
 * Instructor: Nolan Shaw
 *
 * PURPOSE: Implements conditional (collision/state-triggered)
 *          event handlers. Checks Geo against level obstacles and
 *          determines high-level state changes (level complete, death).
 *
 * Assumptions:
 *  - `Model` and related types are available from `model` headers.
 *  - Collision semantics are handled by `geo_*` utilities and game world
 *    helpers; this module maps those checks to events.
 */
#include "cond.h"

int check_level_complete(Model *model)
{
    signed int event = EVENT_NONE;

    if (model->world.geo.x >= model->world.levels[model->world.level_index].end_x)
    {
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