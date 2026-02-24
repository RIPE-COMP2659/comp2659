/**
 * SPIKE TEST DRIVER
 * 
 * Tests spike placement and properties.
 * Verifies spike initialization and state tracking.
 * C89 compatible - all variables declared at start of main().
 */

#include <stdio.h>
#include "../../spike.h"

void print_spike_state(Spike *spike, const char *label) {
    printf("\n%s:\n", label);
    printf("  Position: x=%u, y=%u\n", spike->x, spike->y);
    printf("  Size: %u pixels\n", spike->size);
    printf("  Sprite pointer: %p\n", (void*)spike->sprite);
}

int main(void) {
    Spike spike1;
    Spike spike2;
    Spike spikes[4];
    Spike spike_top_left;
    Spike spike_top_right;
    Spike spike_bottom_left;
    Spike spike_bottom_right;
    Spike pattern_spikes[5];
    
    unsigned int min_x;
    unsigned int max_x;
    unsigned int min_y;
    unsigned int max_y;
    unsigned int spike_width;
    unsigned int spike_height;
    unsigned int pattern_spacing;
    
    int positions[4][2];
    int i;
    int match;
    int sprite_mismatch;
    
    printf("===== SPIKE TEST DRIVER =====\n");
    printf("Testing spike placement and state tracking\n");

    /* Test 1: Create spike at origin */
    printf("\n\n--- TEST 1: Spike at Origin ---\n");
    spike1 = (Spike){ 0, 0, SPIKE_SIZE, SPIKE_SPRITE };
    print_spike_state(&spike1, "Spike1 at (0, 0)");
    printf("  Expected: Position (0,0), Size %u\n", SPIKE_SIZE);

    /* Test 2: Create spike at arbitrary position */
    printf("\n\n--- TEST 2: Spike at Arbitrary Position ---\n");
    spike2 = (Spike){ 150, 200, SPIKE_SIZE, SPIKE_SPRITE };
    print_spike_state(&spike2, "Spike2 at (150, 200)");
    printf("  Expected: Position (150,200), Size %u\n", SPIKE_SIZE);

    /* Test 3: Multiple independent spikes */
    printf("\n\n--- TEST 3: Multiple Independent Spikes ---\n");
    positions[0][0] = 50;   positions[0][1] = 100;
    positions[1][0] = 100;  positions[1][1] = 150;
    positions[2][0] = 200;  positions[2][1] = 75;
    positions[3][0] = 300;  positions[3][1] = 250;
    
    printf("Creating 4 spikes at different positions:\n");
    for (i = 0; i < 4; i++) {
        spikes[i] = (Spike){ positions[i][0], positions[i][1], SPIKE_SIZE, SPIKE_SPRITE };
        printf("  Spike %d: x=%u, y=%u\n", i, spikes[i].x, spikes[i].y);
    }
    
    printf("\nVerifying all spikes maintain independent positions:\n");
    for (i = 0; i < 4; i++) {
        printf("  Spike %d: x=%u (expected %u), y=%u (expected %u) %s\n",
               i, spikes[i].x, positions[i][0], spikes[i].y, positions[i][1],
               (spikes[i].x == positions[i][0] && spikes[i].y == positions[i][1]) ? "✓" : "✗");
    }

    /* Test 4: Spike boundary positions */
    printf("\n\n--- TEST 4: Boundary Positions ---\n");
    
    /* Game bounds are typically 320x224 for Atari ST */
    min_x = 0;
    max_x = 320;
    min_y = 0;
    max_y = 224;
    spike_width = 32;
    spike_height = 32;
    
    spike_top_left = (Spike){ min_x, min_y, SPIKE_SIZE, SPIKE_SPRITE };
    spike_top_right = (Spike){ max_x - spike_width, min_y, SPIKE_SIZE, SPIKE_SPRITE };
    spike_bottom_left = (Spike){ min_x, max_y - spike_height, SPIKE_SIZE, SPIKE_SPRITE };
    spike_bottom_right = (Spike){ max_x - spike_width, max_y - spike_height, SPIKE_SIZE, SPIKE_SPRITE };
    
    printf("Screen bounds: 0-320 (x), 0-224 (y)\n");
    print_spike_state(&spike_top_left, "Spike at top-left corner");
    print_spike_state(&spike_top_right, "Spike at top-right boundary");
    print_spike_state(&spike_bottom_left, "Spike at bottom-left boundary");
    print_spike_state(&spike_bottom_right, "Spike at bottom-right corner");

    /* Test 5: Spike pattern (obstacle course) */
    printf("\n\n--- TEST 5: Spike Pattern (Obstacle Course) ---\n");
    printf("Placing spikes in a pattern across screen:\n");
    
    pattern_spacing = 50;
    for (i = 0; i < 5; i++) {
        pattern_spikes[i] = (Spike){ i * pattern_spacing + 30, 180, SPIKE_SIZE, SPIKE_SPRITE };
        printf("  Spike %d: Position (%u, 180)\n", i, pattern_spikes[i].x);
    }

    /* Test 6: Verify sprite consistency */
    printf("\n\n--- TEST 6: Sprite Pointer Consistency ---\n");
    printf("All spikes should point to same global SPIKE_SPRITE:\n");
    sprite_mismatch = 0;
    for (i = 0; i < 4; i++) {
        match = (spikes[i].sprite == SPIKE_SPRITE);
        printf("  Spike %d: Sprite pointer %s global SPIKE_SPRITE %s\n",
               i, (match ? "==" : "!="), (match ? "✓" : "✗"));
        if (!match) sprite_mismatch++;
    }
    if (sprite_mismatch == 0) {
        printf("  All sprites consistent!\n");
    }

    printf("\n\n===== TEST DRIVER COMPLETE =====\n");
    return 0;
}
