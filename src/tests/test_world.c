#include "unity.h"
#include "world.h"

World world;

void setUp(void) {
    Level levels[1];
    Geo geo = { GEO_DDY, GEO_DX, 0, 100, 200, GEO_SIZE, GEO_SPRITE };

    levels[0] = get_level1();
    world.levels = levels;
    world.geo = geo;
    world.ground_y = 0;
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