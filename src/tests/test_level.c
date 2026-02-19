#include "unity.h"
#include "level.h"

Level level;
static Block level_blocks[1] = {
    { 0, 0, BLOCK_SIZE, BLOCK_SPRITE }
};
static Spike level_spikes[1] = {
    { 0, 0, SPIKE_SIZE, SPIKE_SPRITE }
};
static Lava level_lava[1] = {
    { 0, 0, LAVA_SIZE, LAVA_SPRITE }
};

void setUp(void) {
    level = create_level(level_blocks, level_spikes, level_lava, 1000);
}

void tearDown(void) {
}

void test_level(void) {
    printf("Realistically nothing to test\n");
    level_placeholder();
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_level);

    return UNITY_END();
}