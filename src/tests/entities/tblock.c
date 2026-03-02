#include "unity.h"
#include "block.h"

/* Test blocks */
Block block1;
Block block2;

/* Setup blocks for testing */
void setUp(void) {
    block1 = create_block(0, 0);
    block2 = create_block(100, 200);
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

/* Test Block size is set to BLOCK_SIZE */
void test_block_init_size(void) {
    TEST_ASSERT_EQUAL_INT(BLOCK_SIZE, block1.size);
}

/* Test that multiple blocks can have independent positions */
void test_block_init_global_sprite(void) {
    /* Verify all blocks have independent positions */
    TEST_ASSERT_EQUAL_INT(0, block1.x);
    TEST_ASSERT_EQUAL_INT(0, block1.y);
    TEST_ASSERT_EQUAL_INT(100, block2.x);
    TEST_ASSERT_EQUAL_INT(200, block2.y);

    /* Verify all share the same sprite */
    TEST_ASSERT_EQUAL_PTR(block2.sprite, block1.sprite);
}

/* Main function to run all tests */
int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_block_init_normal);
    RUN_TEST(test_block_init_zero);
    RUN_TEST(test_block_init_seperated);
    RUN_TEST(test_block_init_size);
    RUN_TEST(test_block_init_global_sprite);

    return UNITY_END();
}
