#include "unity.h"
#include "world.h"

Block block;
World world;

void setUp(void) {
    world = get_world();
    block = (Block){world.geo.x, world.ground_y + BLOCK_SIZE, BLOCK_SIZE, BLOCK_SPRITE};
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
    TEST_ASSERT_EQUAL_INT(original_ground_y, world.geo.ground_y);

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

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_world_collision_geo_block_top);
    RUN_TEST(test_world_collision_geo_block_bottom);
    RUN_TEST(test_world_collision_geo_block_left);
    RUN_TEST(test_world_collision_geo_block_none);

    return UNITY_END();
}