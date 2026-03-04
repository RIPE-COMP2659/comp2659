#include "unity.h"
#include "model.h"

void setUp(void) {
}

void tearDown(void) {
}

void test_world_update_camera_bi_blocks_outside_view(void) {
    /* Create blocks completely outside camera view */
    static Block blocks[2];
    static Level level;
    Model test_model;

    blocks[0] = create_block(1000, 100);
    blocks[1] = create_block(1100, 100);
    
    level.blocks = blocks;
    level.blocks_size = 1;
    
    test_model.world.levels = &level;
    test_model.world.camera.x = 0;
    test_model.world.camera.width = SCREEN_WIDTH;
    test_model.cam_min_bi = 0;
    test_model.cam_max_bi = 0;
    
    model_update_camera_bi(&test_model, 0);
    
    TEST_ASSERT_EQUAL_UINT(0, test_model.cam_min_bi);
    TEST_ASSERT_EQUAL_UINT(0, test_model.cam_max_bi);
}

void test_world_update_camera_bi_blocks_span_entire_array(void) {
    /* Create blocks that span entire array and all visible */
    static Block blocks[4];
    static Level level;
    Model test_model;

    blocks[0] = create_block(0, 100);
    blocks[1] = create_block(100, 100);
    blocks[2] = create_block(200, 100);
    blocks[3] = create_block(300, 100);
    
    level.blocks = blocks;
    level.blocks_size = 4;
    
    test_model.world.levels = &level;
    test_model.world.camera.x = 0;
    test_model.world.camera.width = SCREEN_WIDTH;
    test_model.cam_min_bi = 0;
    test_model.cam_max_bi = 0;
    
    model_update_camera_bi(&test_model, 0);
    
    TEST_ASSERT_EQUAL_UINT(0, test_model.cam_min_bi);
    TEST_ASSERT_EQUAL_UINT(3, test_model.cam_max_bi);
}

void test_world_update_camera_bi_min_is_zero_max_partial(void) {
    /* Create blocks where first ones are visible, last is not */
    static Block blocks[4];
    static Level level;
    Model test_model;

    blocks[0] = create_block(50, 100);
    blocks[1] = create_block(100, 100);
    blocks[2] = create_block(200, 100);
    blocks[3] = create_block(1000, 100);
    
    level.blocks = blocks;
    level.blocks_size = 3;
    
    test_model.world.levels = &level;
    test_model.world.camera.x = 0;
    test_model.world.camera.width = SCREEN_WIDTH;
    test_model.cam_min_bi = 0;
    test_model.cam_max_bi = 0;
    
    model_update_camera_bi(&test_model, 0);
    
    TEST_ASSERT_EQUAL_UINT(0, test_model.cam_min_bi);
    TEST_ASSERT_EQUAL_UINT(2, test_model.cam_max_bi);
}

/* TODO: Failing expected 2 was 0 */
void test_world_update_camera_bi_min_offset_max_partial(void) {
    /* Create blocks where some are left of camera, some inside */
    static Block blocks[5];
    static Level level;
    Model test_model;

    blocks[0] = create_block(0, 100);
    blocks[1] = create_block(50, 100);
    blocks[2] = create_block(100, 100);
    blocks[3] = create_block(200, 100);
    blocks[4] = create_block(400, 100);
    
    level.blocks = blocks;
    level.blocks_size = 5;
    
    test_model.world.levels = &level;
    test_model.world.camera.x = 100;
    test_model.world.camera.width = SCREEN_WIDTH;
    test_model.cam_min_bi = 0;
    test_model.cam_max_bi = 0;
    
    model_update_camera_bi(&test_model, 0);
    
    TEST_ASSERT_EQUAL_UINT(2, test_model.cam_min_bi);
    TEST_ASSERT_EQUAL_UINT(4, test_model.cam_max_bi);
}

/* TODO: Failing expected 2 was 0 */
void test_world_update_camera_bi_left_and_right_span(void) {
    /* Create blocks with some left of camera, some visible, some right of camera */
    static Block blocks[5];
    static Level level;
    Model test_model;

    blocks[0] = create_block(0, 100);
    blocks[1] = create_block(50, 100);
    blocks[2] = create_block(100, 100);
    blocks[3] = create_block(200, 100);
    blocks[4] = create_block(1000, 100);
    
    level.blocks = blocks;
    level.blocks_size = 4;
    
    test_model.world.levels = &level;
    test_model.world.camera.x = 100;
    test_model.world.camera.width = SCREEN_WIDTH;
    test_model.cam_min_bi = 0;
    test_model.cam_max_bi = 0;
    
    model_update_camera_bi(&test_model, 0);
    
    TEST_ASSERT_EQUAL_UINT(2, test_model.cam_min_bi);
    TEST_ASSERT_EQUAL_UINT(3, test_model.cam_max_bi);
}

void test_world_update_camera_si_spikes_outside_view(void) {
    /* Create spikes completely outside camera view */
    static Spike spikes[2];
    static Level level;
    Model test_model;

    spikes[0] = create_spike(1000, 100);
    spikes[1] = create_spike(1100, 100);
    level.spikes = spikes;
    level.spikes_size = 1;
    
    test_model.world.levels = &level;
    test_model.world.camera.x = 0;
    test_model.world.camera.width = SCREEN_WIDTH;
    test_model.cam_min_si = 0;
    test_model.cam_max_si = 0;
    
    model_update_camera_si(&test_model, 0);
    
    TEST_ASSERT_EQUAL_UINT(0, test_model.cam_min_si);
    TEST_ASSERT_EQUAL_UINT(0, test_model.cam_max_si);
}

void test_world_update_camera_si_spikes_span_entire_array(void) {
    /* Create spikes that span entire array and all visible */
    static Spike spikes[4];
    static Level level;
    Model test_model;

    spikes[0] = create_spike(0, 100);
    spikes[1] = create_spike(100, 100);
    spikes[2] = create_spike(200, 100);
    spikes[3] = create_spike(300, 100);
    level.spikes = spikes;
    level.spikes_size = 4;
    
    test_model.world.levels = &level;
    test_model.world.camera.x = 0;
    test_model.world.camera.width = SCREEN_WIDTH;
    test_model.cam_min_si = 0;
    test_model.cam_max_si = 0;
    
    model_update_camera_si(&test_model, 0);
    
    TEST_ASSERT_EQUAL_UINT(0, test_model.cam_min_si);
    TEST_ASSERT_EQUAL_UINT(3, test_model.cam_max_si);
}

void test_world_update_camera_si_min_is_zero_max_partial(void) {
    /* Create spikes where first ones are visible, last is not */
    static Spike spikes[4];
    static Level level;
    Model test_model;

    spikes[0] = create_spike(50, 100);
    spikes[1] = create_spike(100, 100);
    spikes[2] = create_spike(200, 100);
    spikes[3] = create_spike(1000, 100);

    level.spikes = spikes;
    level.spikes_size = 4;

    test_model.world.levels = &level;
    test_model.world.camera.x = 0;
    test_model.world.camera.width = SCREEN_WIDTH;
    test_model.cam_min_si = 0;
    test_model.cam_max_si = 0;
    
    model_update_camera_si(&test_model, 0);
    
    TEST_ASSERT_EQUAL_UINT(0, test_model.cam_min_si);
    TEST_ASSERT_EQUAL_UINT(2, test_model.cam_max_si);
}

void test_world_update_camera_si_min_offset_max_partial(void) {
    /* Create spikes where some are left of camera, some inside */
    static Spike spikes[5];
    static Level level;
    Model test_model;

    spikes[0] = create_spike(0, 100);
    spikes[1] = create_spike(50, 100);
    spikes[2] = create_spike(100, 100);
    spikes[3] = create_spike(200, 100);
    spikes[4] = create_spike(400, 100);

    level.spikes = spikes;
    level.spikes_size = 5;

    test_model.world.levels = &level;
    test_model.world.camera.x = 100;
    test_model.world.camera.width = SCREEN_WIDTH;
    test_model.cam_min_si = 0;
    test_model.cam_max_si = 0;
    
    model_update_camera_si(&test_model, 0);
    
    TEST_ASSERT_EQUAL_UINT(2, test_model.cam_min_si);
    TEST_ASSERT_EQUAL_UINT(4, test_model.cam_max_si);
}

void test_world_update_camera_si_left_and_right_span(void) {
    /* Create spikes with some left of camera, some visible, some right of camera */
    static Spike spikes[5];
    static Level level;
    Model test_model;

    spikes[0] = create_spike(0, 100);
    spikes[1] = create_spike(50, 100);
    spikes[2] = create_spike(100, 100);
    spikes[3] = create_spike(200, 100);
    spikes[4] = create_spike(1000, 100);

    level.spikes = spikes;
    level.spikes_size = 5;

    test_model.world.levels = &level;
    test_model.world.camera.x = 100;
    test_model.world.camera.width = SCREEN_WIDTH;
    test_model.cam_min_si = 0;
    test_model.cam_max_si = 0;
    
    model_update_camera_si(&test_model, 0);
    
    TEST_ASSERT_EQUAL_UINT(2, test_model.cam_min_si);
    TEST_ASSERT_EQUAL_UINT(3, test_model.cam_max_si);
}

void test_world_update_camera_li_lava_outside_view(void) {
    /* Create lava completely outside camera view */
    static Lava lava[2];
    static Level level;
    Model test_model;

    lava[0] = create_lava(1000, 100);
    lava[1] = create_lava(1100, 100);
    
    level.lava = lava;
    level.lava_size = 2;
    
    test_model.world.levels = &level;
    test_model.world.camera.x = 0;
    test_model.world.camera.width = SCREEN_WIDTH;
    test_model.cam_min_li = 0;
    test_model.cam_max_li = 0;
    
    model_update_camera_li(&test_model, 0);
    
    TEST_ASSERT_EQUAL_UINT(0, test_model.cam_min_li);
    TEST_ASSERT_EQUAL_UINT(0, test_model.cam_max_li);
}

void test_world_update_camera_li_lava_span_entire_array(void) {
    /* Create lava that span entire array and all visible */
    static Lava lava[4];
    static Level level;
    Model test_model;

    lava[0] = create_lava(0, 100);
    lava[1] = create_lava(100, 100);
    lava[2] = create_lava(200, 100);
    lava[3] = create_lava(300, 100);
    
    level.lava = lava;
    level.lava_size = 4;
    
    test_model.world.levels = &level;
    test_model.world.camera.x = 0;
    test_model.world.camera.width = SCREEN_WIDTH;
    test_model.cam_min_li = 0;
    test_model.cam_max_li = 0;
    
    model_update_camera_li(&test_model, 0);
    
    TEST_ASSERT_EQUAL_UINT(0, test_model.cam_min_li);
    TEST_ASSERT_EQUAL_UINT(3, test_model.cam_max_li);
}

void test_world_update_camera_li_min_is_zero_max_partial(void) {
    /* Create lava where first ones are visible, last is not */
    static Lava lava[4];
    static Level level;
    Model test_model;

    lava[0] = create_lava(50, 100);
    lava[1] = create_lava(100, 100);
    lava[2] = create_lava(200, 100);
    lava[3] = create_lava(1000, 100);
    
    level.lava = lava;
    level.lava_size = 4;
    
    test_model.world.levels = &level;
    test_model.world.camera.x = 0;
    test_model.world.camera.width = SCREEN_WIDTH;
    test_model.cam_min_li = 0;
    test_model.cam_max_li = 0;
    
    model_update_camera_li(&test_model, 0);
    
    TEST_ASSERT_EQUAL_UINT(0, test_model.cam_min_li);
    TEST_ASSERT_EQUAL_UINT(2, test_model.cam_max_li);
}

void test_world_update_camera_li_min_offset_max_partial(void) {
    /* Create lava where some are left of camera, some inside */
    static Lava lava[5];
    static Level level;
    Model test_model;

    lava[0] = create_lava(0, 100);
    lava[1] = create_lava(50, 100);
    lava[2] = create_lava(100, 100);
    lava[3] = create_lava(200, 100);
    lava[4] = create_lava(400, 100);
    
    level.lava = lava;
    level.lava_size = 5;
    
    test_model.world.levels = &level;
    test_model.world.camera.x = 100;
    test_model.world.camera.width = SCREEN_WIDTH;
    test_model.cam_min_li = 0;
    test_model.cam_max_li = 0;
    
    model_update_camera_li(&test_model, 0);
    
    TEST_ASSERT_EQUAL_UINT(2, test_model.cam_min_li);
    TEST_ASSERT_EQUAL_UINT(4, test_model.cam_max_li);
}

void test_world_update_camera_li_left_and_right_span(void) {
    /* Create lava with some left of camera, some visible, some right of camera */
    static Lava lava[5];
    static Level level;
    Model test_model;

    lava[0] = create_lava(0, 100);
    lava[1] = create_lava(50, 100);
    lava[2] = create_lava(100, 100);
    lava[3] = create_lava(200, 100);
    lava[4] = create_lava(1000, 100);
    
    level.lava = lava;
    level.lava_size = 5;
    
    test_model.world.levels = &level;
    test_model.world.camera.x = 100;
    test_model.world.camera.width = SCREEN_WIDTH;
    test_model.cam_min_li = 0;
    test_model.cam_max_li = 0;
    
    model_update_camera_li(&test_model, 0);
    
    TEST_ASSERT_EQUAL_UINT(2, test_model.cam_min_li);
    TEST_ASSERT_EQUAL_UINT(3, test_model.cam_max_li);
}

int main(void) {
    UNITY_BEGIN();

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
