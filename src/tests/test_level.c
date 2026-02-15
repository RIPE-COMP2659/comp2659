#include "unity.h"
#include "level.h"

Level level;

void setUp(void) {
    level = (Level){
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
    };
}

void tearDown(void) {
}

void test_level(void) {
    printf("Realistically nothing to test\n");
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_level);

    return UNITY_END();
}