/**
 * LAVA TEST DRIVER
 * 
 * Tests lava pool placement and properties.
 * Verifies lava hazard state and positioning.
 * C89 compatible - all variables declared at start of main().
 */

#include <stdio.h>
#include "../../lava.h"

void print_lava_state(Lava *lava, const char *label) {
    printf("\n%s:\n", label);
    printf("  Position: x=%u, y=%u\n", lava->x, lava->y);
    printf("  Size: %u pixels\n", lava->size);
    printf("  Sprite pointer: %p\n", (void*)lava->sprite);
}

int main(void) {
    Lava lava1;
    Lava lava2;
    Lava lava_pools[3];
    Lava lava_pit;
    Lava scattered[4];
    Lava left_wall;
    Lava right_wall;
    Lava trench;
    Lava test_lava;
    
    unsigned int heights[3];
    unsigned int scatter_x[4];
    unsigned int scatter_y[4];
    
    int i;
    int match;
    int sprite_mismatch;

    printf("===== LAVA TEST DRIVER =====\n");
    printf("Testing lava pool placement and state tracking\n");

    /* Test 1: Create lava at origin */
    printf("\n\n--- TEST 1: Lava at Origin ---\n");
    lava1 = (Lava){ 0, 0, LAVA_SIZE, LAVA_SPRITE };
    print_lava_state(&lava1, "Lava1 at (0, 0)");
    printf("  Expected: Position (0,0), Size %u\n", LAVA_SIZE);

    /* Test 2: Create lava at arbitrary position */
    printf("\n\n--- TEST 2: Lava at Arbitrary Position ---\n");
    lava2 = (Lava){ 100, 100, LAVA_SIZE, LAVA_SPRITE };
    print_lava_state(&lava2, "Lava2 at (100, 100)");
    printf("  Expected: Position (100,100), Size %u\n", LAVA_SIZE);

    /* Test 3: Lava at different heights */
    printf("\n\n--- TEST 3: Lava at Different Heights (Platforms) ---\n");
    printf("Creating lava pools at various vertical positions:\n");
    
    heights[0] = 100;
    heights[1] = 160;
    heights[2] = 200;
    
    for (i = 0; i < 3; i++) {
        lava_pools[i] = (Lava){ 50, heights[i], LAVA_SIZE, LAVA_SPRITE };
        printf("  Lava %d: y=%u\n", i, lava_pools[i].y);
    }

    printf("\nVerifying all lava pools are independent:\n");
    for (i = 0; i < 3; i++) {
        printf("  Lava %d: y=%u (expected %u) %s\n",
               i, lava_pools[i].y, heights[i],
               (lava_pools[i].y == heights[i]) ? "✓" : "✗");
    }

    /* Test 4: Lava pit at bottom */
    printf("\n\n--- TEST 4: Lava Pit at Bottom of Screen ---\n");
    /* Typical game height is 224 pixels */
    lava_pit = (Lava){ 0, 192, LAVA_SIZE, LAVA_SPRITE };
    print_lava_state(&lava_pit, "Bottom lava pit (y=192)");
    printf("  Expected: Safe zone above at y < 192\n");
    printf("  Expected: Dangerous zone at y >= 192\n");

    /* Test 5: Multiple lava pools across screen */
    printf("\n\n--- TEST 5: Scattered Lava Pools ---\n");
    scatter_x[0] = 50;
    scatter_x[1] = 120;
    scatter_x[2] = 200;
    scatter_x[3] = 280;
    scatter_y[0] = 180;
    scatter_y[1] = 160;
    scatter_y[2] = 180;
    scatter_y[3] = 160;
    
    printf("Creating 4 lava pools in staggered pattern:\n");
    for (i = 0; i < 4; i++) {
        scattered[i] = (Lava){ scatter_x[i], scatter_y[i], LAVA_SIZE, LAVA_SPRITE };
        printf("  Lava %d: Position (%u, %u)\n", i, scattered[i].x, scattered[i].y);
    }

    /* Test 6: Lava corridor */
    printf("\n\n--- TEST 6: Lava Corridor (Narrow Path) ---\n");
    printf("Creating lava that forces narrow safe passage:\n");
    
    left_wall = (Lava){ 0, 180, LAVA_SIZE, LAVA_SPRITE };
    right_wall = (Lava){ 288, 180, LAVA_SIZE, LAVA_SPRITE };
    
    print_lava_state(&left_wall, "Left wall lava");
    print_lava_state(&right_wall, "Right wall lava");
    printf("\n  Safe passage: x between 32 and 288, y < 180\n");

    /* Test 7: Lava trench */
    printf("\n\n--- TEST 7: Lava Trench (Must Jump Over) ---\n");
    printf("Creating wide lava trench to jump over:\n");
    
    trench = (Lava){ 100, 200, LAVA_SIZE, LAVA_SPRITE };
    print_lava_state(&trench, "Lava trench at (100, 200)");
    printf("  Expected: Geo must be at y < 200 to be safe\n");
    printf("  Expected: 32-pixel width means jump needed\n");

    /* Test 8: Verify sprite consistency */
    printf("\n\n--- TEST 8: Sprite Pointer Consistency ---\n");
    printf("All lava pools should point to same global LAVA_SPRITE:\n");
    sprite_mismatch = 0;
    for (i = 0; i < 4; i++) {
        match = (scattered[i].sprite == LAVA_SPRITE);
        printf("  Lava %d: Sprite pointer %s global LAVA_SPRITE %s\n",
               i, (match ? "==" : "!="), (match ? "✓" : "✗"));
        if (!match) sprite_mismatch++;
    }
    if (sprite_mismatch == 0) {
        printf("  All sprites consistent!\n");
    }

    /* Test 9: Memory layout verification */
    printf("\n\n--- TEST 9: Lava Structure Memory Layout ---\n");
    test_lava = (Lava){ 10, 20, LAVA_SIZE, LAVA_SPRITE };
    printf("Lava struct at address: %p\n", (void*)&test_lava);
    printf("  x (offset 0): %u bytes, value=%u\n", (unsigned int)sizeof(test_lava.x), test_lava.x);
    printf("  y (offset %u): %u bytes, value=%u\n", (unsigned int)sizeof(test_lava.x), (unsigned int)sizeof(test_lava.y), test_lava.y);
    printf("  size: %u\n", test_lava.size);
    printf("  sprite pointer: %p\n", (void*)test_lava.sprite);

    printf("\n\n===== TEST DRIVER COMPLETE =====\n");
    return 0;
}
