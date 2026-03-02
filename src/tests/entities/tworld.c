#include "unity.h"
#include "world.h"

Block block;
Spike spike;
Lava lava;
World world;

void setUp(void) {
    world = get_world();
    block = create_block(world.geo.x, world.ground_y + BLOCK_SIZE);
    spike.x = world.geo.x;
    spike.y = world.ground_y + SPIKE_SIZE;
    spike.size = SPIKE_SIZE;
    spike.sprite = SPIKE_SPRITE;
    lava.x = world.geo.x;
    lava.y = world.ground_y + LAVA_SIZE;
    lava.size = LAVA_SIZE;
    lava.sprite = LAVA_SPRITE;
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
    unsigned int original_ground_y = world.geo.ground_y;
    block.x = world.geo.x + world.geo.size + 1;
    world.geo.is_dead = FALSE;

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
    unsigned int original_ground_y = world.geo.ground_y;
    spike.x = world.geo.x + world.geo.size + 1;
    world.geo.is_dead = FALSE;

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
    unsigned int original_ground_y = world.geo.ground_y;
    lava.x = world.geo.x + world.geo.size + 1;
    world.geo.is_dead = FALSE;

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

/* TODO: This overall logic doesn't make much sense, it needs a refactor */
void test_world_collision_ground_resets_when_leaving_block(void) {
    /* Setup: geo lands on a block */
    block.x = world.geo.x;
    world.geo.y = block.y + world.geo.size;

    /* Geo collides with block and ground_y updates */
    world_collision_geo_block(&world, &block);
    TEST_ASSERT_EQUAL_INT(block.y, world.geo.ground_y);

    /* Geo is landed on the block */
    geo_update_landed(&world.geo);
    TEST_ASSERT_EQUAL_INT(TRUE, world.geo.is_landed);

    /* Geo moves right (away from block) and falls due to gravity/no support */
    world.geo.x += block.size * 2;
    
    /* world_collision_geo_ground resets ground_y to world default when geo has no collision support */
    world_collision_geo_block(&world, &block);
    world_collision_geo_ground(&world);
    TEST_ASSERT_EQUAL_INT(world.ground_y, world.geo.ground_y);
    
    /* Now geo_update_landed checks with the reset ground_y and determines geo is falling */
    geo_update_landed(&world.geo);
    TEST_ASSERT_EQUAL_INT(FALSE, world.geo.is_landed);
}

void test_world_update_camera_bi_blocks_outside_view(void) {
    /* Create blocks completely outside camera view */
    static Block blocks[2];
    static Level level;
    World test_world;

    blocks[0] = create_block(1000, 100);
    blocks[1] = create_block(1100, 100);
    
    level.blocks = blocks;
    level.blocks_size = 1;
    
    test_world.levels = &level;
    test_world.camera.x = 0;
    test_world.camera.width = SCREEN_WIDTH;
    test_world.cam_min_bi = 0;
    test_world.cam_max_bi = 0;
    
    world_update_camera_bi(&test_world, 0);
    
    TEST_ASSERT_EQUAL_UINT(0, test_world.cam_min_bi);
    TEST_ASSERT_EQUAL_UINT(0, test_world.cam_max_bi);
}

void test_world_update_camera_bi_blocks_span_entire_array(void) {
    /* Create blocks that span entire array and all visible */
    static Block blocks[4];
    static Level level;
    World test_world;

    blocks[0] = create_block(0, 100);
    blocks[1] = create_block(100, 100);
    blocks[2] = create_block(200, 100);
    blocks[3] = create_block(300, 100);
    
    level.blocks = blocks;
    level.blocks_size = 3;
    
    test_world.levels = &level;
    test_world.camera.x = 0;
    test_world.camera.width = SCREEN_WIDTH;
    test_world.cam_min_bi = 0;
    test_world.cam_max_bi = 0;
    
    world_update_camera_bi(&test_world, 0);
    
    TEST_ASSERT_EQUAL_UINT(0, test_world.cam_min_bi);
    TEST_ASSERT_EQUAL_UINT(3, test_world.cam_max_bi);
}

void test_world_update_camera_bi_min_is_zero_max_partial(void) {
    /* Create blocks where first ones are visible, last is not */
    static Block blocks[4];
    static Level level;
    World test_world;

    blocks[0] = create_block(50, 100);
    blocks[1] = create_block(100, 100);
    blocks[2] = create_block(200, 100);
    blocks[3] = create_block(1000, 100);
    
    level.blocks = blocks;
    level.blocks_size = 3;
    
    test_world.levels = &level;
    test_world.camera.x = 0;
    test_world.camera.width = SCREEN_WIDTH;
    test_world.cam_min_bi = 0;
    test_world.cam_max_bi = 0;
    
    world_update_camera_bi(&test_world, 0);
    
    TEST_ASSERT_EQUAL_UINT(0, test_world.cam_min_bi);
    TEST_ASSERT_EQUAL_UINT(2, test_world.cam_max_bi);
}

/* TODO: Failing expected 2 was 0 */
void test_world_update_camera_bi_min_offset_max_partial(void) {
    /* Create blocks where some are left of camera, some inside */
    static Block blocks[5];
    static Level level;
    World test_world;

    blocks[0] = create_block(0, 100);
    blocks[1] = create_block(50, 100);
    blocks[2] = create_block(100, 100);
    blocks[3] = create_block(200, 100);
    blocks[4] = create_block(400, 100);
    
    level.blocks = blocks;
    level.blocks_size = 4;
    
    test_world.levels = &level;
    test_world.camera.x = 100;
    test_world.camera.width = SCREEN_WIDTH;
    test_world.cam_min_bi = 0;
    test_world.cam_max_bi = 0;
    
    world_update_camera_bi(&test_world, 0);
    
    TEST_ASSERT_EQUAL_UINT(2, test_world.cam_min_bi);
    TEST_ASSERT_EQUAL_UINT(4, test_world.cam_max_bi);
}

/* TODO: Failing expected 2 was 0 */
void test_world_update_camera_bi_left_and_right_span(void) {
    /* Create blocks with some left of camera, some visible, some right of camera */
    static Block blocks[5];
    static Level level;
    World test_world;

    blocks[0] = create_block(0, 100);
    blocks[1] = create_block(50, 100);
    blocks[2] = create_block(100, 100);
    blocks[3] = create_block(200, 100);
    blocks[4] = create_block(1000, 100);
    
    level.blocks = blocks;
    level.blocks_size = 4;
    
    test_world.levels = &level;
    test_world.camera.x = 100;
    test_world.camera.width = SCREEN_WIDTH;
    test_world.cam_min_bi = 0;
    test_world.cam_max_bi = 0;
    
    world_update_camera_bi(&test_world, 0);
    
    TEST_ASSERT_EQUAL_UINT(2, test_world.cam_min_bi);
    TEST_ASSERT_EQUAL_UINT(3, test_world.cam_max_bi);
}

void test_world_update_camera_si_spikes_outside_view(void) {
    /* Create spikes completely outside camera view */
    static Spike spikes[2] = {
        {1000, 100, SPIKE_SIZE, SPIKE_SPRITE},
        {1100, 100, SPIKE_SIZE, SPIKE_SPRITE}
    };
    static Level level;
    World test_world;
    
    level.spikes = spikes;
    level.spikes_size = 1;
    
    test_world.levels = &level;
    test_world.camera.x = 0;
    test_world.camera.width = SCREEN_WIDTH;
    test_world.cam_min_si = 0;
    test_world.cam_max_si = 0;
    
    world_update_camera_si(&test_world, 0);
    
    TEST_ASSERT_EQUAL_UINT(0, test_world.cam_min_si);
    TEST_ASSERT_EQUAL_UINT(0, test_world.cam_max_si);
}

void test_world_update_camera_si_spikes_span_entire_array(void) {
    /* Create spikes that span entire array and all visible */
    static Spike spikes[4] = {
        {0, 100, SPIKE_SIZE, SPIKE_SPRITE},
        {100, 100, SPIKE_SIZE, SPIKE_SPRITE},
        {200, 100, SPIKE_SIZE, SPIKE_SPRITE},
        {300, 100, SPIKE_SIZE, SPIKE_SPRITE}
    };
    static Level level;
    World test_world;
    
    level.spikes = spikes;
    level.spikes_size = 3;
    
    test_world.levels = &level;
    test_world.camera.x = 0;
    test_world.camera.width = SCREEN_WIDTH;
    test_world.cam_min_si = 0;
    test_world.cam_max_si = 0;
    
    world_update_camera_si(&test_world, 0);
    
    TEST_ASSERT_EQUAL_UINT(0, test_world.cam_min_si);
    TEST_ASSERT_EQUAL_UINT(3, test_world.cam_max_si);
}

void test_world_update_camera_si_min_is_zero_max_partial(void) {
    /* Create spikes where first ones are visible, last is not */
    static Spike spikes[4] = {
        {50, 100, SPIKE_SIZE, SPIKE_SPRITE},
        {100, 100, SPIKE_SIZE, SPIKE_SPRITE},
        {200, 100, SPIKE_SIZE, SPIKE_SPRITE},
        {1000, 100, SPIKE_SIZE, SPIKE_SPRITE}
    };
    static Level level;
    World test_world;
    
    level.spikes = spikes;
    level.spikes_size = 3;
    
    test_world.levels = &level;
    test_world.camera.x = 0;
    test_world.camera.width = SCREEN_WIDTH;
    test_world.cam_min_si = 0;
    test_world.cam_max_si = 0;
    
    world_update_camera_si(&test_world, 0);
    
    TEST_ASSERT_EQUAL_UINT(0, test_world.cam_min_si);
    TEST_ASSERT_EQUAL_UINT(2, test_world.cam_max_si);
}

void test_world_update_camera_si_min_offset_max_partial(void) {
    /* Create spikes where some are left of camera, some inside */
    static Spike spikes[5] = {
        {0, 100, SPIKE_SIZE, SPIKE_SPRITE},
        {50, 100, SPIKE_SIZE, SPIKE_SPRITE},
        {100, 100, SPIKE_SIZE, SPIKE_SPRITE},
        {200, 100, SPIKE_SIZE, SPIKE_SPRITE},
        {400, 100, SPIKE_SIZE, SPIKE_SPRITE}
    };
    static Level level;
    World test_world;
    
    level.spikes = spikes;
    level.spikes_size = 4;
    
    test_world.levels = &level;
    test_world.camera.x = 100;
    test_world.camera.width = SCREEN_WIDTH;
    test_world.cam_min_si = 0;
    test_world.cam_max_si = 0;
    
    world_update_camera_si(&test_world, 0);
    
    TEST_ASSERT_EQUAL_UINT(2, test_world.cam_min_si);
    TEST_ASSERT_EQUAL_UINT(4, test_world.cam_max_si);
}

void test_world_update_camera_si_left_and_right_span(void) {
    /* Create spikes with some left of camera, some visible, some right of camera */
    static Spike spikes[5] = {
        {0, 100, SPIKE_SIZE, SPIKE_SPRITE},
        {50, 100, SPIKE_SIZE, SPIKE_SPRITE},
        {100, 100, SPIKE_SIZE, SPIKE_SPRITE},
        {200, 100, SPIKE_SIZE, SPIKE_SPRITE},
        {1000, 100, SPIKE_SIZE, SPIKE_SPRITE}
    };
    static Level level;
    World test_world;
    
    level.spikes = spikes;
    level.spikes_size = 4;
    
    test_world.levels = &level;
    test_world.camera.x = 100;
    test_world.camera.width = SCREEN_WIDTH;
    test_world.cam_min_si = 0;
    test_world.cam_max_si = 0;
    
    world_update_camera_si(&test_world, 0);
    
    TEST_ASSERT_EQUAL_UINT(2, test_world.cam_min_si);
    TEST_ASSERT_EQUAL_UINT(3, test_world.cam_max_si);
}

void test_world_update_camera_li_lava_outside_view(void) {
    /* Create lava completely outside camera view */
    static Lava lava[2] = {
        {1000, 100, LAVA_SIZE, LAVA_SPRITE},
        {1100, 100, LAVA_SIZE, LAVA_SPRITE}
    };
    static Level level;
    World test_world;
    
    level.lava = lava;
    level.lava_size = 1;
    
    test_world.levels = &level;
    test_world.camera.x = 0;
    test_world.camera.width = SCREEN_WIDTH;
    test_world.cam_min_li = 0;
    test_world.cam_max_li = 0;
    
    world_update_camera_li(&test_world, 0);
    
    TEST_ASSERT_EQUAL_UINT(0, test_world.cam_min_li);
    TEST_ASSERT_EQUAL_UINT(0, test_world.cam_max_li);
}

void test_world_update_camera_li_lava_span_entire_array(void) {
    /* Create lava that span entire array and all visible */
    static Lava lava[4] = {
        {0, 100, LAVA_SIZE, LAVA_SPRITE},
        {100, 100, LAVA_SIZE, LAVA_SPRITE},
        {200, 100, LAVA_SIZE, LAVA_SPRITE},
        {300, 100, LAVA_SIZE, LAVA_SPRITE}
    };
    static Level level;
    World test_world;
    
    level.lava = lava;
    level.lava_size = 3;
    
    test_world.levels = &level;
    test_world.camera.x = 0;
    test_world.camera.width = SCREEN_WIDTH;
    test_world.cam_min_li = 0;
    test_world.cam_max_li = 0;
    
    world_update_camera_li(&test_world, 0);
    
    TEST_ASSERT_EQUAL_UINT(0, test_world.cam_min_li);
    TEST_ASSERT_EQUAL_UINT(3, test_world.cam_max_li);
}

void test_world_update_camera_li_min_is_zero_max_partial(void) {
    /* Create lava where first ones are visible, last is not */
    static Lava lava[4] = {
        {50, 100, LAVA_SIZE, LAVA_SPRITE},
        {100, 100, LAVA_SIZE, LAVA_SPRITE},
        {200, 100, LAVA_SIZE, LAVA_SPRITE},
        {1000, 100, LAVA_SIZE, LAVA_SPRITE}
    };
    static Level level;
    World test_world;
    
    level.lava = lava;
    level.lava_size = 3;
    
    test_world.levels = &level;
    test_world.camera.x = 0;
    test_world.camera.width = SCREEN_WIDTH;
    test_world.cam_min_li = 0;
    test_world.cam_max_li = 0;
    
    world_update_camera_li(&test_world, 0);
    
    TEST_ASSERT_EQUAL_UINT(0, test_world.cam_min_li);
    TEST_ASSERT_EQUAL_UINT(2, test_world.cam_max_li);
}

void test_world_update_camera_li_min_offset_max_partial(void) {
    /* Create lava where some are left of camera, some inside */
    static Lava lava[5] = {
        {0, 100, LAVA_SIZE, LAVA_SPRITE},
        {50, 100, LAVA_SIZE, LAVA_SPRITE},
        {100, 100, LAVA_SIZE, LAVA_SPRITE},
        {200, 100, LAVA_SIZE, LAVA_SPRITE},
        {400, 100, LAVA_SIZE, LAVA_SPRITE}
    };
    static Level level;
    World test_world;
    
    level.lava = lava;
    level.lava_size = 4;
    
    test_world.levels = &level;
    test_world.camera.x = 100;
    test_world.camera.width = SCREEN_WIDTH;
    test_world.cam_min_li = 0;
    test_world.cam_max_li = 0;
    
    world_update_camera_li(&test_world, 0);
    
    TEST_ASSERT_EQUAL_UINT(2, test_world.cam_min_li);
    TEST_ASSERT_EQUAL_UINT(4, test_world.cam_max_li);
}

void test_world_update_camera_li_left_and_right_span(void) {
    /* Create lava with some left of camera, some visible, some right of camera */
    static Lava lava[5] = {
        {0, 100, LAVA_SIZE, LAVA_SPRITE},
        {50, 100, LAVA_SIZE, LAVA_SPRITE},
        {100, 100, LAVA_SIZE, LAVA_SPRITE},
        {200, 100, LAVA_SIZE, LAVA_SPRITE},
        {1000, 100, LAVA_SIZE, LAVA_SPRITE}
    };
    static Level level;
    World test_world;
    
    level.lava = lava;
    level.lava_size = 4;
    
    test_world.levels = &level;
    test_world.camera.x = 100;
    test_world.camera.width = SCREEN_WIDTH;
    test_world.cam_min_li = 0;
    test_world.cam_max_li = 0;
    
    world_update_camera_li(&test_world, 0);
    
    TEST_ASSERT_EQUAL_UINT(2, test_world.cam_min_li);
    TEST_ASSERT_EQUAL_UINT(3, test_world.cam_max_li);
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
    RUN_TEST(test_world_collision_ground_resets_when_leaving_block);
    RUN_TEST(test_world_update_camera_bi_blocks_outside_view);
    RUN_TEST(test_world_update_camera_bi_blocks_span_entire_array);
    RUN_TEST(test_world_update_camera_bi_min_is_zero_max_partial);
    RUN_TEST(test_world_update_camera_bi_min_offset_max_partial);
    RUN_TEST(test_world_update_camera_bi_left_and_right_span);
    RUN_TEST(test_world_update_camera_si_spikes_outside_view);
    RUN_TEST(test_world_update_camera_si_spikes_span_entire_array);
    RUN_TEST(test_world_update_camera_si_min_is_zero_max_partial);
    RUN_TEST(test_world_update_camera_si_min_offset_max_partial);
    RUN_TEST(test_world_update_camera_si_left_and_right_span);
    RUN_TEST(test_world_update_camera_li_lava_outside_view);
    RUN_TEST(test_world_update_camera_li_lava_span_entire_array);
    RUN_TEST(test_world_update_camera_li_min_is_zero_max_partial);
    RUN_TEST(test_world_update_camera_li_min_offset_max_partial);
    RUN_TEST(test_world_update_camera_li_left_and_right_span);

    return UNITY_END();
}