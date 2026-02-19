#include "unity.h"
#include "world.h"

World world;

void setUp(void) {
    world = (World) {
        (Level[]) {
            (Level) {
                (Block[]) {
                    (Block){ 0, 0, BLOCK_SIZE, BLOCK_SPRITE }
                },
                (Spike[]) {
                    (Spike){ 0, 0, SPIKE_SIZE, SPIKE_SPRITE }
                },
                (Lava[]) {
                    (Lava){ 0, 0, LAVA_SIZE, LAVA_SPRITE }
                },
                0,
                0,
                0,
                1000
            }
        },
        (Geo) {
            GEO_DDY, GEO_DX, -20, 100, 200, GEO_SIZE, GEO_SPRITE
        },
        300 // ground_y
    };
}

void tearDown(void) {
}

void test_world(void) {
    world_placeholder();
    printf("Realistically nothing to test\n");
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_world);

    return UNITY_END();
}