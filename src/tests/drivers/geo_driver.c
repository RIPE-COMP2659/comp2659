/**
 * GEO TEST DRIVER
 * 
 * Tests Geo character movement, gravity, and jumping mechanics.
 * Traces object properties with printf after each action.
 */

#include <stdio.h>
#include "../../geo.h"

void print_geo_state(Geo *geo, const char *label) {
    printf("\n%s:\n", label);
    printf("  Position: x=%u, y=%u\n", geo->x, geo->y);
    printf("  Velocity: dx=%d, dy=%d\n", geo->dx, geo->dy);
    printf("  Acceleration: ddy=%d\n", geo->ddy);
}

int main(void) {
    Geo geo1;
    Geo geo2;
    Geo geo3;
    Geo geo4;
    Geo geo5;
    int i;
    int tick;

    printf("===== GEO CHARACTER TEST DRIVER =====\n");
    printf("Testing movement, gravity, and jumping mechanics\n");

    /* Test 1: Basic gravity and movement */
    printf("\n\n--- TEST 1: Gravity and Horizontal Movement ---\n");
    /* Start well above ground so gravity can be observed */
    geo1 = create_geo(100, 200, 50);
    geo1.dx = GEO_DX;
    geo1.ddy = GEO_DDY;

    print_geo_state(&geo1, "Initial state (airborne)");
    
    printf("\nApplying 5 gravity ticks (free fall):\n");
    for (i = 1; i <= 5; i++) {
        geo_move(&geo1);
        printf("  Tick %d: x=%u, y=%u, dy=%d\n", i, geo1.x, geo1.y, geo1.dy);
    }
    print_geo_state(&geo1, "After 5 ticks of falling");

    /* Test 2: Jump from rest */
    printf("\n\n--- TEST 2: Jump From Rest ---\n");
    /* Put Geo well above ground and at rest */
    geo2 = create_geo(50, 300, 50);
    geo2.dx = GEO_DX;
    geo2.ddy = GEO_DDY;

    print_geo_state(&geo2, "Initial state before jump");
    
    /* ensure jump logic sees the player as landed before initiating jump */
    geo2.is_landed = TRUE;
    geo_jump(&geo2);
    print_geo_state(&geo2, "After jump() called (velocity set to GEO_JUMP_DY)");
    
    printf("\nApplying gravity ticks during descent:\n");
    for (i = 1; i <= 15; i++) {
        geo_move(&geo2);
        printf("  Tick %d: x=%u, y=%u, dy=%d\n", i, geo2.x, geo2.y, geo2.dy);
    }
    print_geo_state(&geo2, "After jump sequence complete");

    /* Test 3: Jump at apex */
    printf("\n\n--- TEST 3: Find Jump Apex ---\n");
    geo3 = create_geo(150, 350, 50);
    geo3.dx = GEO_DX;
    geo3.ddy = GEO_DDY;

    print_geo_state(&geo3, "Before jump");
    geo3.is_landed = TRUE;
    geo_jump(&geo3);
    
    printf("\nMoving until velocity reaches 0 (apex):\n");
    tick = 0;
    while (geo3.dy > 0) {
        geo_move(&geo3);
        tick++;
        printf("  Tick %d: dy=%d, y=%u\n", tick, geo3.dy, geo3.y);
    }
    print_geo_state(&geo3, "At apex (dy should be 0 or negative)");

    /* Test 4: Horizontal movement during jump */
    printf("\n\n--- TEST 4: Horizontal Movement During Jump ---\n");
    geo4 = create_geo(200, 400, 50);
    geo4.dx = GEO_DX;
    geo4.ddy = GEO_DDY;

    print_geo_state(&geo4, "Initial state");
    geo4.is_landed = TRUE;
    geo_jump(&geo4);
    
    printf("\nTicks with both horizontal and vertical movement:\n");
    for (i = 1; i <= 12; i++) {
        geo_move(&geo4);
        printf("  Tick %d: x=%u, y=%u, dx=%d, dy=%d\n", i, geo4.x, geo4.y, geo4.dx, geo4.dy);
    }

    /* Test 5: Continuous movement (moving right while falling) */
    printf("\n\n--- TEST 5: Continuous Movement ---\n");
    geo5 = create_geo(0, 200, 50);
    /* give initial downward velocity and horizontal speed */
    geo5.dy = -5;
    geo5.dx = GEO_DX;
    geo5.ddy = GEO_DDY;

    print_geo_state(&geo5, "Starting with initial downward velocity (dy=-5)");
    
    printf("\nContinuous ticks (falling and moving right):\n");
    for (i = 1; i <= 8; i++) {
        geo_move(&geo5);
        printf("  Tick %d: Position=(%u, %u), Velocity=(dx=%d, dy=%d)\n", 
               i, geo5.x, geo5.y, geo5.dx, geo5.dy);
    }

    printf("\n\n===== TEST DRIVER COMPLETE =====\n");
    return 0;
}
