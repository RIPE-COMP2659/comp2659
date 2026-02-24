/**
 * LEVEL TEST DRIVER
 * 
 * Tests Level struct creation and level data initialization.
 * Verifies that multiple levels can be created and accessed properly.
 */

#include <stdio.h>
#include "../../level.h"

void print_level_state(Level *level, const char *label) {
    printf("\n%s:\n", label);
    printf("  Blocks pointer: %p\n", (void*)level->blocks);
    printf("  Spikes pointer: %p\n", (void*)level->spikes);
    printf("  Lava pointer: %p\n", (void*)level->lava);
    printf("  Current block: %u\n", level->current_block);
    printf("  Current spike: %u\n", level->current_spike);
    printf("  Current lava: %u\n", level->current_lava);
    printf("  End X: %u\n", level->end_x);
}

void print_block_state(Block *block, const char *label) {
    printf("\n    %s:\n", label);
    printf("      Position: (%u, %u)\n", block->x, block->y);
    printf("      Size: %u\n", block->size);
    printf("      Sprite: %p\n", (void*)block->sprite);
}

void print_spike_state(Spike *spike, const char *label) {
    printf("\n    %s:\n", label);
    printf("      Position: (%u, %u)\n", spike->x, spike->y);
    printf("      Size: %u\n", spike->size);
    printf("      Sprite: %p\n", (void*)spike->sprite);
}

void print_lava_state(Lava *lava, const char *label) {
    printf("\n    %s:\n", label);
    printf("      Position: (%u, %u)\n", lava->x, lava->y);
    printf("      Size: %u\n", lava->size);
    printf("      Sprite: %p\n", (void*)lava->sprite);
}

int main(void) {
    Level level;
    Level *levels;
    Level level1;
    Level level2;
    Block blocks[2];
    Spike spikes[1];
    Lava lava_pools[1];
    int i;

    printf("===== LEVEL TEST DRIVER =====\n");
    printf("Testing Level struct initialization and data access\n");

    /* Test 1: Create simple level manually */
    printf("\n\n--- TEST 1: Manual Level Creation ---\n");
    blocks[0] = (Block){ 0, 0, BLOCK_SIZE, BLOCK_SPRITE };
    blocks[1] = (Block){ 100, 200, BLOCK_SIZE, BLOCK_SPRITE };
    spikes[0] = (Spike){ 300, 400, SPIKE_SIZE, SPIKE_SPRITE };
    lava_pools[0] = (Lava){ 500, 600, LAVA_SIZE, LAVA_SPRITE };
    
    level = create_level(blocks, spikes, lava_pools, 1000);
    print_level_state(&level, "Manually created level");
    printf("  Expected: All pointers valid, counters at 0, end_x=1000\n");

    /* Test 2: Verify block data in level */
    printf("\n\n--- TEST 2: Level Blocks Data ---\n");
    printf("Checking block data in level:\n");
    for (i = 0; i < 2; i++) {
        char buffer[50];
        sprintf(buffer, "Block %d", i);
        print_block_state(&level.blocks[i], buffer);
    }
    printf("  Status: Blocks accessible and initialized\n");

    /* Test 3: Verify spike data in level */
    printf("\n\n--- TEST 3: Level Spikes Data ---\n");
    printf("Checking spike data in level:\n");
    print_spike_state(&level.spikes[0], "Spike 0");
    printf("  Status: Spikes accessible and initialized\n");

    /* Test 4: Verify lava data in level */
    printf("\n\n--- TEST 4: Level Lava Data ---\n");
    printf("Checking lava data in level:\n");
    print_lava_state(&level.lava[0], "Lava 0");
    printf("  Status: Lava accessible and initialized\n");

    /* Test 5: Load Level 1 */
    printf("\n\n--- TEST 5: Load Level 1 (get_level1) ---\n");
    level = get_level1();
    print_level_state(&level, "Level 1");
    printf("  Status: Level 1 loaded via get_level1()\n");

    /* Test 6: Load Level 2 */
    printf("\n\n--- TEST 6: Load Level 2 (get_level2) ---\n");
    level = get_level2();
    print_level_state(&level, "Level 2");
    printf("  Status: Level 2 loaded via get_level2()\n");

    /* Test 7: Load all levels via get_levels() */
    printf("\n\n--- TEST 7: Load All Levels (get_levels) ---\n");
    levels = get_levels();
    
    if (levels != NULL) {
        printf("Levels array loaded successfully\n");
        
        printf("\nLevel 0 from array:\n");
        print_level_state(&levels[0], "levels[0]");
        
        printf("\nLevel 1 from array:\n");
        print_level_state(&levels[1], "levels[1]");
        
        printf("\n  Status: ✓ All levels accessible via get_levels()\n");
    } else {
        printf("  Status: ✗ get_levels() returned NULL\n");
    }

    /* Test 8: Verify entity counters reset */
    printf("\n\n--- TEST 8: Entity Counters ---\n");
    printf("Verifying counters for entity progression:\n");
    level = get_level1();
    printf("  current_block: %u (initialize to 0: %s)\n", 
           level.current_block, (level.current_block == 0) ? "✓" : "✗");
    printf("  current_spike: %u (initialize to 0: %s)\n",
           level.current_spike, (level.current_spike == 0) ? "✓" : "✗");
    printf("  current_lava: %u (initialize to 0: %s)\n",
           level.current_lava, (level.current_lava == 0) ? "✓" : "✗");

    /* Test 9: Placeholder function (should do nothing) */
    printf("\n\n--- TEST 9: Level Placeholder Function ---\n");
    printf("Calling level_placeholder()...\n");
    level_placeholder();
    printf("  Status: ✓ Function returned successfully\n");

    /* Test 10: Verify different levels have different content */
    printf("\n\n--- TEST 10: Level Differentiation ---\n");
    level1 = get_level1();
    level2 = get_level2();
    
    printf("Comparing Level 1 and Level 2:\n");
    printf("  Level 1 end_x: %u\n", level1.end_x);
    printf("  Level 2 end_x: %u\n", level2.end_x);
    printf("  Same end_x: %s\n", (level1.end_x == level2.end_x) ? "Yes" : "No");
    printf("  Different block arrays: %s\n", 
           (level1.blocks != level2.blocks) ? "✓ Yes (good)" : "✗ No (same reference)");

    printf("\n\n===== LEVEL TEST DRIVER COMPLETE =====\n");
    return 0;
}
