#include "unity.h"
#include "world.h"

Block block;
Spike spike;
Lava lava;
World world;

void setUp(void) {
    world = get_world();
    block = (Block){world.geo.x, world.ground_y + BLOCK_SIZE, BLOCK_SIZE, BLOCK_SPRITE};
    spike = (Spike){world.geo.x, world.ground_y + SPIKE_SIZE, SPIKE_SIZE, SPIKE_SPRITE};
    lava = (Lava){world.geo.x, world.ground_y + LAVA_SIZE, LAVA_SIZE, LAVA_SPRITE};
}

void tearDown(void) {
}

void test_world_collision_geo_block_top(void) {
    block.y = world.geo.y;
    world.geo.y = block.y + block.size;

    /* Test that geo is landing on a block above and to the left */
    block.x = world.geo.x - block.size / 2;
    world_collision_geo_block(&world, &block);
    TEST_ASSERT_EQUAL_INT(block.y, world.geo.ground_y);
    TEST_ASSERT_EQUAL_INT(FALSE, world.geo.is_dead);

    /* Test that geo is landing on a block from directly above */
    block.x = world.geo.x;
    world_collision_geo_block(&world, &block);
    TEST_ASSERT_EQUAL_INT(block.y, world.geo.ground_y);
    TEST_ASSERT_EQUAL_INT(FALSE, world.geo.is_dead);

    /* Test that geo is landing on a block above and to the right */
    block.x = world.geo.x + block.size / 2;
    world_collision_geo_block(&world, &block);
    TEST_ASSERT_EQUAL_INT(block.y, world.geo.ground_y);
    TEST_ASSERT_EQUAL_INT(FALSE, world.geo.is_dead);
}

void test_world_collision_geo_block_bottom(void) {
    block.y = world.geo.y + world.geo.size;
    world.geo.y = block.y - block.size;

    /* Test that geo is colliding with a block above it and to the left */
    block.x = world.geo.x - block.size / 2;
    world_collision_geo_block(&world, &block);
    TEST_ASSERT_EQUAL_INT(TRUE, world.geo.is_dead);

    /* Test that geo is colliding with a block directly above */
    block.x = world.geo.x;
    world_collision_geo_block(&world, &block);
    TEST_ASSERT_EQUAL_INT(TRUE, world.geo.is_dead);

    /* Test that geo is colliding with a block above it and to the right */
    block.x = world.geo.x + block.size / 2;
    world_collision_geo_block(&world, &block);
    TEST_ASSERT_EQUAL_INT(TRUE, world.geo.is_dead);
}

void test_world_collision_geo_block_left(void) {
    block.x = world.geo.x + world.geo.size;

    /* Test that geo is colliding with a block to the right and slightly above */
    block.y = world.geo.y + block.size / 2;
    world_collision_geo_block(&world, &block);
    TEST_ASSERT_EQUAL_INT(TRUE, world.geo.is_dead);

    /* Test that geo is colliding with block directly to the right */
    block.y = world.geo.y;
    world_collision_geo_block(&world, &block);
    TEST_ASSERT_EQUAL_INT(TRUE, world.geo.is_dead);

    /* Test that geo is colliding with a block to the right and slightly below */
    block.y = world.geo.y - block.size / 2;
    world_collision_geo_block(&world, &block);
    TEST_ASSERT_EQUAL_INT(TRUE, world.geo.is_dead);
}

void test_world_collision_geo_block_none(void) {
    block.x = world.geo.x + world.geo.size + 1;
    world.geo.is_dead = FALSE;
    unsigned int original_ground_y = world.geo.ground_y;

    /* Test that geo is not colliding with a block to the right and above */
    block.y = world.geo.y + block.size / 2;
    world_collision_geo_block(&world, &block);
    TEST_ASSERT_EQUAL_INT(FALSE, world.geo.is_dead);
    TEST_ASSERT_EQUAL_INT(original_ground_y, world.geo.ground_y);

    /* Test that geo is not colliding with block directly to the right */
    block.y = world.geo.y;
    world_collision_geo_block(&world, &block);
    TEST_ASSERT_EQUAL_INT(FALSE, world.geo.is_dead);
    TEST_ASSERT_EQUAL_INT(original_ground_y, world.geo.ground_y);

    /* Test that geo is not colliding with a block to the right and below */
    block.y = world.geo.y - block.size / 2;
    world_collision_geo_block(&world, &block);
    TEST_ASSERT_EQUAL_INT(FALSE, world.geo.is_dead);
    TEST_ASSERT_EQUAL_INT(original_ground_y, world.geo.ground_y);

    /* Test that geo is not colliding with a block directly above */
    block.x = world.geo.x;
    block.y = world.geo.y + block.size + 1;
    world_collision_geo_block(&world, &block);
    TEST_ASSERT_EQUAL_INT(FALSE, world.geo.is_dead);

    /* Test that geo is not colliding with a block directly below */
    block.y = world.geo.y - world.geo.size - 1;
    world_collision_geo_block(&world, &block);
    TEST_ASSERT_EQUAL_INT(FALSE, world.geo.is_dead);
    TEST_ASSERT_EQUAL_INT(original_ground_y, world.geo.ground_y);

    /* Test that geo is not colliding with a block to the left and above */
    block.x = world.geo.x - block.size - 1;
    block.y = world.geo.y + block.size / 2;
    world_collision_geo_block(&world, &block);
    TEST_ASSERT_EQUAL_INT(FALSE, world.geo.is_dead);
    TEST_ASSERT_EQUAL_INT(original_ground_y, world.geo.ground_y);

    /* Test that geo is not colliding with a block directly left */
    block.y = world.geo.y;
    world_collision_geo_block(&world, &block);
    TEST_ASSERT_EQUAL_INT(FALSE, world.geo.is_dead);
    TEST_ASSERT_EQUAL_INT(original_ground_y, world.geo.ground_y);

    /* Test that geo is not colliding with a block to the left and below */
    block.y = world.geo.y - block.size / 2;
    world_collision_geo_block(&world, &block);
    TEST_ASSERT_EQUAL_INT(FALSE, world.geo.is_dead);
    TEST_ASSERT_EQUAL_INT(original_ground_y, world.geo.ground_y);
}

void test_world_collision_geo_spike_top(void) {
    spike.y = world.geo.y;
    world.geo.y = spike.y + spike.size;

    /* Test that geo is landing on a spike above and to the left */
    spike.x = world.geo.x - spike.size / 2;
    world.geo.is_dead = FALSE;
    world_collision_geo_spike(&world, &spike);
    TEST_ASSERT_EQUAL_INT(TRUE, world.geo.is_dead);

    /* Test that geo is landing on a spike from directly above */
    spike.x = world.geo.x;
    world.geo.is_dead = FALSE;
    world_collision_geo_spike(&world, &spike);
    TEST_ASSERT_EQUAL_INT(TRUE, world.geo.is_dead);

    /* Test that geo is landing on a spike above and to the right */
    spike.x = world.geo.x + spike.size / 2;
    world.geo.is_dead = FALSE;
    world_collision_geo_spike(&world, &spike);
    TEST_ASSERT_EQUAL_INT(TRUE, world.geo.is_dead);
}

void test_world_collision_geo_spike_bottom(void) {
    spike.y = world.geo.y + world.geo.size;
    world.geo.y = spike.y - spike.size;

    /* Test that geo is colliding with a spike above it and to the left */
    spike.x = world.geo.x - spike.size / 2;
    world.geo.is_dead = FALSE;
    world_collision_geo_spike(&world, &spike);
    TEST_ASSERT_EQUAL_INT(TRUE, world.geo.is_dead);

    /* Test that geo is colliding with a spike directly above */
    spike.x = world.geo.x;
    world.geo.is_dead = FALSE;
    world_collision_geo_spike(&world, &spike);
    TEST_ASSERT_EQUAL_INT(TRUE, world.geo.is_dead);

    /* Test that geo is colliding with a spike above it and to the right */
    spike.x = world.geo.x + spike.size / 2;
    world.geo.is_dead = FALSE;
    world_collision_geo_spike(&world, &spike);
    TEST_ASSERT_EQUAL_INT(TRUE, world.geo.is_dead);
}

void test_world_collision_geo_spike_left(void) {
    spike.x = world.geo.x + world.geo.size;

    /* Test that geo is colliding with a spike to the right and slightly above */
    spike.y = world.geo.y + spike.size / 2;
    world.geo.is_dead = FALSE;
    world_collision_geo_spike(&world, &spike);
    TEST_ASSERT_EQUAL_INT(TRUE, world.geo.is_dead);

    /* Test that geo is colliding with spike directly to the right */
    spike.y = world.geo.y;
    world.geo.is_dead = FALSE;
    world_collision_geo_spike(&world, &spike);
    TEST_ASSERT_EQUAL_INT(TRUE, world.geo.is_dead);

    /* Test that geo is colliding with a spike to the right and slightly below */
    spike.y = world.geo.y - spike.size / 2;
    world.geo.is_dead = FALSE;
    world_collision_geo_spike(&world, &spike);
    TEST_ASSERT_EQUAL_INT(TRUE, world.geo.is_dead);
}

void test_world_collision_geo_spike_none(void) {
    spike.x = world.geo.x + world.geo.size + 1;
    world.geo.is_dead = FALSE;
    unsigned int original_ground_y = world.geo.ground_y;

    /* Test that geo is not colliding with a spike to the right and above */
    spike.y = world.geo.y + spike.size / 2;
    world_collision_geo_spike(&world, &spike);
    TEST_ASSERT_EQUAL_INT(FALSE, world.geo.is_dead);
    TEST_ASSERT_EQUAL_INT(original_ground_y, world.geo.ground_y);

    /* Test that geo is not colliding with spike directly to the right */
    spike.y = world.geo.y;
    world_collision_geo_spike(&world, &spike);
    TEST_ASSERT_EQUAL_INT(FALSE, world.geo.is_dead);
    TEST_ASSERT_EQUAL_INT(original_ground_y, world.geo.ground_y);

    /* Test that geo is not colliding with a spike to the right and below */
    spike.y = world.geo.y - spike.size / 2;
    world_collision_geo_spike(&world, &spike);
    TEST_ASSERT_EQUAL_INT(FALSE, world.geo.is_dead);
    TEST_ASSERT_EQUAL_INT(original_ground_y, world.geo.ground_y);

    /* Test that geo is not colliding with a spike directly above */
    spike.x = world.geo.x;
    spike.y = world.geo.y + spike.size + 1;
    world_collision_geo_spike(&world, &spike);
    TEST_ASSERT_EQUAL_INT(FALSE, world.geo.is_dead);
    TEST_ASSERT_EQUAL_INT(original_ground_y, world.geo.ground_y);

    /* Test that geo is not colliding with a spike directly below */
    spike.y = world.geo.y - world.geo.size - 1;
    world_collision_geo_spike(&world, &spike);
    TEST_ASSERT_EQUAL_INT(FALSE, world.geo.is_dead);
    TEST_ASSERT_EQUAL_INT(original_ground_y, world.geo.ground_y);

    /* Test that geo is not colliding with a spike to the left and above */
    spike.x = world.geo.x - spike.size - 1;
    spike.y = world.geo.y + spike.size / 2;
    world_collision_geo_spike(&world, &spike);
    TEST_ASSERT_EQUAL_INT(FALSE, world.geo.is_dead);
    TEST_ASSERT_EQUAL_INT(original_ground_y, world.geo.ground_y);

    /* Test that geo is not colliding with a spike directly left */
    spike.y = world.geo.y;
    world_collision_geo_spike(&world, &spike);
    TEST_ASSERT_EQUAL_INT(FALSE, world.geo.is_dead);
    TEST_ASSERT_EQUAL_INT(original_ground_y, world.geo.ground_y);

    /* Test that geo is not colliding with a spike to the left and below */
    spike.y = world.geo.y - spike.size / 2;
    world_collision_geo_spike(&world, &spike);
    TEST_ASSERT_EQUAL_INT(FALSE, world.geo.is_dead);
    TEST_ASSERT_EQUAL_INT(original_ground_y, world.geo.ground_y);
}

void test_world_collision_geo_lava_top(void) {
    lava.y = world.geo.y;
    world.geo.y = lava.y + lava.size;

    /* Test that geo is landing on lava above and to the left */
    lava.x = world.geo.x - lava.size / 2;
    world.geo.is_dead = FALSE;
    world_collision_geo_lava(&world, &lava);
    TEST_ASSERT_EQUAL_INT(TRUE, world.geo.is_dead);

    /* Test that geo is landing on lava from directly above */
    lava.x = world.geo.x;
    world.geo.is_dead = FALSE;
    world_collision_geo_lava(&world, &lava);
    TEST_ASSERT_EQUAL_INT(TRUE, world.geo.is_dead);

    /* Test that geo is landing on lava above and to the right */
    lava.x = world.geo.x + lava.size / 2;
    world.geo.is_dead = FALSE;
    world_collision_geo_lava(&world, &lava);
    TEST_ASSERT_EQUAL_INT(TRUE, world.geo.is_dead);
}

void test_world_collision_geo_lava_bottom(void) {
    lava.y = world.geo.y + world.geo.size;
    world.geo.y = lava.y - lava.size;

    /* Test that geo is colliding with lava above it and to the left */
    lava.x = world.geo.x - lava.size / 2;
    world.geo.is_dead = FALSE;
    world_collision_geo_lava(&world, &lava);
    TEST_ASSERT_EQUAL_INT(TRUE, world.geo.is_dead);

    /* Test that geo is colliding with lava directly above */
    lava.x = world.geo.x;
    world.geo.is_dead = FALSE;
    world_collision_geo_lava(&world, &lava);
    TEST_ASSERT_EQUAL_INT(TRUE, world.geo.is_dead);

    /* Test that geo is colliding with lava above it and to the right */
    lava.x = world.geo.x + lava.size / 2;
    world.geo.is_dead = FALSE;
    world_collision_geo_lava(&world, &lava);
    TEST_ASSERT_EQUAL_INT(TRUE, world.geo.is_dead);
}

void test_world_collision_geo_lava_left(void) {
    lava.x = world.geo.x + world.geo.size;

    /* Test that geo is colliding with lava to the right and slightly above */
    lava.y = world.geo.y + lava.size / 2;
    world.geo.is_dead = FALSE;
    world_collision_geo_lava(&world, &lava);
    TEST_ASSERT_EQUAL_INT(TRUE, world.geo.is_dead);

    /* Test that geo is colliding with lava directly to the right */
    lava.y = world.geo.y;
    world.geo.is_dead = FALSE;
    world_collision_geo_lava(&world, &lava);
    TEST_ASSERT_EQUAL_INT(TRUE, world.geo.is_dead);

    /* Test that geo is colliding with lava to the right and slightly below */
    lava.y = world.geo.y - lava.size / 2;
    world.geo.is_dead = FALSE;
    world_collision_geo_lava(&world, &lava);
    TEST_ASSERT_EQUAL_INT(TRUE, world.geo.is_dead);
}

void test_world_collision_geo_lava_none(void) {
    lava.x = world.geo.x + world.geo.size + 1;
    world.geo.is_dead = FALSE;
    unsigned int original_ground_y = world.geo.ground_y;

    /* Test that geo is not colliding with lava to the right and above */
    lava.y = world.geo.y + lava.size / 2;
    world_collision_geo_lava(&world, &lava);
    TEST_ASSERT_EQUAL_INT(FALSE, world.geo.is_dead);
    TEST_ASSERT_EQUAL_INT(original_ground_y, world.geo.ground_y);

    /* Test that geo is not colliding with lava directly to the right */
    lava.y = world.geo.y;
    world_collision_geo_lava(&world, &lava);
    TEST_ASSERT_EQUAL_INT(FALSE, world.geo.is_dead);
    TEST_ASSERT_EQUAL_INT(original_ground_y, world.geo.ground_y);

    /* Test that geo is not colliding with lava to the right and below */
    lava.y = world.geo.y - lava.size / 2;
    world_collision_geo_lava(&world, &lava);
    TEST_ASSERT_EQUAL_INT(FALSE, world.geo.is_dead);
    TEST_ASSERT_EQUAL_INT(original_ground_y, world.geo.ground_y);

    /* Test that geo is not colliding with lava directly above */
    lava.x = world.geo.x;
    lava.y = world.geo.y + lava.size + 1;
    world_collision_geo_lava(&world, &lava);
    TEST_ASSERT_EQUAL_INT(FALSE, world.geo.is_dead);
    TEST_ASSERT_EQUAL_INT(original_ground_y, world.geo.ground_y);

    /* Test that geo is not colliding with lava directly below */
    lava.y = world.geo.y - world.geo.size - 1;
    world_collision_geo_lava(&world, &lava);
    TEST_ASSERT_EQUAL_INT(FALSE, world.geo.is_dead);
    TEST_ASSERT_EQUAL_INT(original_ground_y, world.geo.ground_y);

    /* Test that geo is not colliding with lava to the left and above */
    lava.x = world.geo.x - lava.size - 1;
    lava.y = world.geo.y + lava.size / 2;
    world_collision_geo_lava(&world, &lava);
    TEST_ASSERT_EQUAL_INT(FALSE, world.geo.is_dead);
    TEST_ASSERT_EQUAL_INT(original_ground_y, world.geo.ground_y);

    /* Test that geo is not colliding with lava directly left */
    lava.y = world.geo.y;
    world_collision_geo_lava(&world, &lava);
    TEST_ASSERT_EQUAL_INT(FALSE, world.geo.is_dead);
    TEST_ASSERT_EQUAL_INT(original_ground_y, world.geo.ground_y);

    /* Test that geo is not colliding with lava to the left and below */
    lava.y = world.geo.y - lava.size / 2;
    world_collision_geo_lava(&world, &lava);
    TEST_ASSERT_EQUAL_INT(FALSE, world.geo.is_dead);
    TEST_ASSERT_EQUAL_INT(original_ground_y, world.geo.ground_y);
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_world_collision_geo_block_top);
    RUN_TEST(test_world_collision_geo_block_bottom);
    RUN_TEST(test_world_collision_geo_block_left);
    RUN_TEST(test_world_collision_geo_block_none);
    RUN_TEST(test_world_collision_geo_spike_top);
    RUN_TEST(test_world_collision_geo_spike_bottom);
    RUN_TEST(test_world_collision_geo_spike_left);
    RUN_TEST(test_world_collision_geo_spike_none);
    RUN_TEST(test_world_collision_geo_lava_top);
    RUN_TEST(test_world_collision_geo_lava_bottom);
    RUN_TEST(test_world_collision_geo_lava_left);
    RUN_TEST(test_world_collision_geo_lava_none);

    return UNITY_END();
}