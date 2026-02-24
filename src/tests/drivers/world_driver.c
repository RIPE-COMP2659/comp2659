/**
 * WORLD TEST DRIVER
 * 
 * Tests World struct creation and collision detection.
 * Verifies that geo collisions with blocks, spikes, and lava
 * are properly detected and handled.
 */

#include <stdio.h>
#include "../../world.h"

void print_world_state(World *world, const char *label) {
    printf("\n%s:\n", label);
    printf("  Geo Position: x=%u, y=%u\n", world->geo.x, world->geo.y);
    printf("  Geo Velocity: dx=%d, dy=%d\n", world->geo.dx, world->geo.dy);
    printf("  Geo Dead: %d\n", world->geo.is_dead);
    printf("  Ground Y: %u\n", world->ground_y);
    printf("  Levels pointer: %p\n", (void*)world->levels);
}

int main(void) {
    World world;
    Block test_block;
    Spike test_spike;
    Lava test_lava;
    int i;

    printf("===== WORLD TEST DRIVER =====\n");
    printf("Testing World struct and collision detection\n");

    /* Test 1: Create world and verify structure */
    printf("\n\n--- TEST 1: World Creation ---\n");
    world = get_world();
    print_world_state(&world, "World created via get_world()");
    printf("  Expected: Geo at valid position, not dead, ground_y=32\n");

    /* Test 2: Verify Geo initial state */
    printf("\n\n--- TEST 2: Initial Geo State ---\n");
    printf("Geo should be at ground level, not dead:\n");
    printf("  x=%u (expected ~100)\n", world.geo.x);
    printf("  y=%u (expected ~64)\n", world.geo.y);
    printf("  is_dead=%d (expected TRUE (%d))\n", world.geo.is_dead, TRUE);
    printf("  ground_y=%u (expected 32)\n", world.ground_y);

    /* Test 3: Block collision - landing on top */
    printf("\n\n--- TEST 3: Block Collision - Landing on Top ---\n");
    test_block = (Block){ 50, 100, BLOCK_SIZE, BLOCK_SPRITE };
    world.geo.x = 50;
    world.geo.y = 100 - GEO_SIZE;
    world.geo.dy = -5;
    print_world_state(&world, "Before block collision");
    
    world_collision_geo_block(&world, &test_block);
    print_world_state(&world, "After collision - should update ground_y");
    printf("  Expected: ground_y updated to 100 (block y position)\n");

    /* Test 4: Block collision - hit bottom (death) */
    printf("\n\n--- TEST 4: Block Collision - Hit Bottom (Death) ---\n");
    world.geo.is_dead = FALSE;
    test_block = (Block){ 100, 200, BLOCK_SIZE, BLOCK_SPRITE };
    world.geo.x = 100;
    world.geo.y = 200 + BLOCK_SIZE;
    world.geo.dy = 5;
    print_world_state(&world, "Before hitting block from below");
    
    world_collision_geo_block(&world, &test_block);
    print_world_state(&world, "After collision - should be dead");
    printf("  Expected: is_dead should be TRUE (%d)\n", TRUE);

    /* Test 5: Spike collision */
    printf("\n\n--- TEST 5: Spike Collision ---\n");
    world.geo.is_dead = FALSE;
    test_spike = (Spike){ 150, 250, SPIKE_SIZE, SPIKE_SPRITE };
    world.geo.x = 150;
    world.geo.y = 250;
    print_world_state(&world, "Before spike collision");
    
    world_collision_geo_spike(&world, &test_spike);
    print_world_state(&world, "After spike collision - should be dead");
    printf("  Expected: is_dead should be TRUE (%d)\n", TRUE);

    /* Test 6: Lava collision */
    printf("\n\n--- TEST 6: Lava Collision ---\n");
    world.geo.is_dead = FALSE;
    test_lava = (Lava){ 200, 300, LAVA_SIZE, LAVA_SPRITE };
    world.geo.x = 200;
    world.geo.y = 300;
    print_world_state(&world, "Before lava collision");
    
    world_collision_geo_lava(&world, &test_lava);
    print_world_state(&world, "After lava collision - should be dead");
    printf("  Expected: is_dead should be TRUE (%d)\n", TRUE);

    /* Test 7: Multiple collisions with safety margin */
    printf("\n\n--- TEST 7: Multiple Safe Positions ---\n");
    printf("Testing positions with safe distance from hazards:\n");
    test_block = (Block){ 100, 100, BLOCK_SIZE, BLOCK_SPRITE };
    
    for (i = 0; i < 3; i++) {
        world.geo.is_dead = FALSE;
        world.geo.x = 100;
        world.geo.y = 50 + (i * 30);
        printf("  Position %d: (%u, %u) - ", i, world.geo.x, world.geo.y);
        world_collision_geo_block(&world, &test_block);
        printf("is_dead=%d\n", world.geo.is_dead);
    }

    /* Test 8: Verify levels are accessible */
    printf("\n\n--- TEST 8: Levels Accessibility ---\n");
    printf("Verifying world has access to level data:\n");
    printf("  Levels pointer: %p\n", (void*)world.levels);
    if (world.levels != NULL) {
        printf("  Level 0 blocks pointer: %p\n", (void*)world.levels[0].blocks);
        printf("  Level 0 spikes pointer: %p\n", (void*)world.levels[0].spikes);
        printf("  Level 0 lava pointer: %p\n", (void*)world.levels[0].lava);
        printf("  Status: ✓ All levels accessible\n");
    } else {
        printf("  Status: ✗ Levels pointer is NULL\n");
    }

    printf("\n\n===== WORLD TEST DRIVER COMPLETE =====\n");
    return 0;
}
