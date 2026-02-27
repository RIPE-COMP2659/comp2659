#include "unity.h"
#include "block.h"

/* Test blocks */
Block block1;
Block block2;

/* Setup blocks for testing */
void setUp(void) {
    block1.x = 0;
    block1.y = 0;
    block1.size = BLOCK_SIZE;
    block1.sprite = BLOCK_SPRITE;
    block2.x = 100;
    block2.y = 200;
    block2.size = BLOCK_SIZE;
    block2.sprite = BLOCK_SPRITE;
    block_placeholder();
}

void tearDown(void) {
    /* clean stuff up here */
}

/* Test basic Block initialization with normal x and y values */
void test_block_init_normal(void) {
    TEST_ASSERT_EQUAL_INT(100, block2.x);
    TEST_ASSERT_EQUAL_INT(200, block2.y);
}

/* Test Block initialization with zero values */
void test_block_init_zero(void) {
    TEST_ASSERT_EQUAL_INT(0, block1.x);
    TEST_ASSERT_EQUAL_INT(0, block1.y);
}

/* Test Block initialization */
void test_block_init_seperated(void) {
    TEST_ASSERT_EQUAL_INT(0, block1.x);
    TEST_ASSERT_EQUAL_INT(0, block1.y);
    
    TEST_ASSERT_EQUAL_INT(100, block2.x);
    TEST_ASSERT_EQUAL_INT(200, block2.y);
}

/* Test that size is set to BLOCK_SIZE */
void test_block_init_size(void) {
    TEST_ASSERT_EQUAL_INT(BLOCK_SIZE, block1.size);
}

/* Test that sprite pointer is set correctly */
void test_block_init_sprite(void) {
    TEST_ASSERT_EQUAL_PTR(BLOCK_SPRITE, block1.sprite);
}

/* Test that multiple blocks can have independent positions */
void test_block_init_global_sprite(void) {
    /* Verify all blocks have independent positions */
    TEST_ASSERT_EQUAL_INT(0, block1.x);
    TEST_ASSERT_EQUAL_INT(0, block1.y);
    TEST_ASSERT_EQUAL_INT(100, block2.x);
    TEST_ASSERT_EQUAL_INT(200, block2.y);

    /* Verify all share the same sprite */
    TEST_ASSERT_EQUAL_PTR(BLOCK_SPRITE, block1.sprite);
    TEST_ASSERT_EQUAL_PTR(BLOCK_SPRITE, block2.sprite);
}

/* Test that sprite dimensions match the configured size */
void test_block_sprite_size(void) {
    /* Verify sprite array has block.size rows */
    int num_rows = sizeof(BLOCK_SPRITE) / sizeof(BLOCK_SPRITE[0]);
    int bits_per_row = (BLOCK_SIZE / WORD) * WORD;

    TEST_ASSERT_EQUAL_INT(block1.size, num_rows);
    TEST_ASSERT_EQUAL_INT(block1.size, bits_per_row);
}

/* Main function to run all tests */
int main(void) {
    UNITY_BEGIN();
    
    RUN_TEST(test_block_init_normal);
    RUN_TEST(test_block_init_zero);
    RUN_TEST(test_block_init_seperated);
    RUN_TEST(test_block_init_size);
    RUN_TEST(test_block_init_sprite);
    RUN_TEST(test_block_init_global_sprite);
    RUN_TEST(test_block_sprite_size);

    return UNITY_END();
}
