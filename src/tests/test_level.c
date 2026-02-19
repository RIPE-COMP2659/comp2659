#include "unity.h"
#include "level.h"

Level level1;
Level level2;

void setUp(void) {
    level1 = create_level1();
    level2 = create_level2();
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