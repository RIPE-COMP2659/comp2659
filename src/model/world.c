/**
 * world.c
 * Authors:
 *     Riley Gramlich, rgram060@mtroyal.ca, 201762060
 *     Robert Parker Hutcheson, rhutc335@mtroyal.ca, 201762335
 *     Isaac Klein, iklei977@mtroyal.ca, 201763977
 *     Eduard Mykhailets, emykh268@mtroyal.ca, 201750268
 * Course: COMP 2659-001, Computing Machinery II, Winter 2026
 * Instructor: Nolan Shaw
 *
 * PURPOSE: World lifecycle and collision handling. Manages camera
 *          offset, world updates per tick, and collision resolution
 *          between the player and level entities.
 */
#include "world.h"

#define CAMERA_X_OFFSET 160
#define CAMERA_Y_OFFSET 260

/* TODO: Level index management, currently never updated */
/* TODO: Levels must be in ascending x value order or logic will break */
/* Make sure levels adhere to this upon creation */
World create_world(Level *levels, Geo geo, unsigned int ground_y)
{
    World world;

    world.levels = levels;
    world.geo = geo;
    world.camera = create_camera(0, SCREEN_HEIGHT);
    world.ground_y = ground_y;
    world.level_index = 0;
    world.levels_size = 2; /* TODO: Hard coded */

    return world;
}

World get_world(void)
{
    Level *levels = get_levels();
    unsigned int ground_y = 32;

    /* TODO: Should probably not be hard coded for starting x */
    /* TODO: Added some additional height to test the start, can be removed */
    Geo geo = create_geo(
        CAMERA_X_OFFSET,
        ground_y + GEO_SIZE + GEO_SIZE,
        ground_y);

    return create_world(levels, geo, ground_y);
}

void world_reset_level(World *world)
{
    world->geo = create_geo(CAMERA_X_OFFSET, world->ground_y + GEO_SIZE, world->ground_y);
    world->camera = create_camera(0, SCREEN_HEIGHT);
}

void world_update(World *world)
{
    geo_update(&world->geo);
    /* TODO: Camera tracking is stuck to Geo instead of delayed vertical movement */
    camera_update_coordinates(
        &world->camera,
        world->geo.x - CAMERA_X_OFFSET, 
        world->geo.y + CAMERA_Y_OFFSET
    );
}

void world_update_collisions(
    World *world,
    unsigned int block_min,
    unsigned int block_max,
    unsigned int spike_min,
    unsigned int spike_max,
    unsigned int lava_min,
    unsigned int lava_max)
{
    Level *level = &world->levels[world->level_index];

    /* Reset is_landed before checking blocks, if Geo is on a block, this will
       be set to true again */
    world->geo.is_landed = FALSE;
    world->geo.ground_y = world->ground_y;

    world_collisions_geo_blocks(world, level->blocks, block_min, block_max);
    world_collisions_geo_spikes(world, level->spikes, spike_min, spike_max);
    world_collisions_geo_lava(world, level->lava, lava_min, lava_max);

    /* Update landed status based on final ground_y after all collisions */
    geo_update_landed(&world->geo);
}

void world_collisions_geo_blocks(
    World *world,
    Block *blocks,
    unsigned int block_min,
    unsigned int block_max)
{
    unsigned int i;
    for (i = block_min; i <= block_max; i++)
    {
        world_collision_geo_block(world, &blocks[i]);
    }
}

void world_collisions_geo_spikes(
    World *world,
    Spike *spikes,
    unsigned int spike_min,
    unsigned int spike_max)
{
    unsigned int i;
    for (i = spike_min; i <= spike_max; i++)
    {
        world_collision_geo_spike(world, &spikes[i]);
    }
}

void world_collisions_geo_lava(
    World *world,
    Lava *lava,
    unsigned int lava_min,
    unsigned int lava_max)
{
    unsigned int i;
    for (i = lava_min; i <= lava_max; i++)
    {
        world_collision_geo_lava(world, &lava[i]);
    }
}

void world_collision_geo_block(World *world, Block *block)
{
    signed int collision = geo_check_square_collision(
        &world->geo,
        block->x,
        block->y,
        block->size);

    if (collision == COLLISION_TOP)
    {
        world->geo.ground_y = block->y;
        world->geo.is_landed = TRUE;
    }
    else if (collision == COLLISION_BOTTOM || collision == COLLISION_LEFT)
    {
        world->geo.is_dead = TRUE;
    }
    else if (collision == COLLISION_NONE)
    {
        /* No collision with this block, geo is not supported */
        world->geo.is_landed = FALSE;
    }
}

/* TODO: Implement proper spike collision detection */
void world_collision_geo_spike(World *world, Spike *spike)
{
    signed int collision = geo_check_spike_collision(
        &world->geo,
        spike->x,
        spike->y,
        spike->size);

    if (collision != COLLISION_NONE && collision != COLLISION_ERROR)
    {
        world->geo.is_dead = TRUE;
    }
}

/* TODO: Implement proper lava collision detection once we refactor to better
   lava objects */
void world_collision_geo_lava(World *world, Lava *lava)
{
    signed int collision = geo_check_lava_collision(
        &world->geo,
        lava->x,
        lava->y,
        lava->size);

    if (collision != COLLISION_NONE && collision != COLLISION_ERROR)
    {
        world->geo.is_dead = TRUE;
    }
}

/* TODO: In general, the logic feels shaky and could likely use a rework */
void world_collision_geo_ground(World *world)
{
    if (world->geo.is_landed == FALSE)
    {
        world->geo.ground_y = world->ground_y;
    }
}
