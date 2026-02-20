#include "unity.h"
#include "world.h"

World world;

void setUp(void) {
    world = get_world();
}

void tearDown(void) {
}

void test_world(void) {
    printf("Realistically nothing to test\n");
    world_placeholder();
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_world);

    return UNITY_END();
}