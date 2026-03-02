#include "unity.h"
#include "lava.h"

/* Test lava */
Lava lava1;
Lava lava2;

/* Setup lava for testing */
void setUp(void) {
    lava1.x = 0;
    lava2.y = 0;
    lava1.size = LAVA_SIZE;
    lava1.sprite = LAVA_SPRITE;

    lava2.x = 100;
    lava2.y = 200;
    lava2.size = LAVA_SIZE;
    lava2.sprite = LAVA_SPRITE;
    lava_placeholder();
}

void tearDown(void) {
    /* clean stuff up here */
}

/* Test basic Lava initialization with normal x and y values */
void test_lava_init_normal(void) {
    TEST_ASSERT_EQUAL_INT(100, lava2.x);
    TEST_ASSERT_EQUAL_INT(200, lava2.y);
}

/* Test Lava initialization with zero values */
void test_lava_init_zero(void) {
    TEST_ASSERT_EQUAL_INT(0, lava1.x);
    TEST_ASSERT_EQUAL_INT(0, lava1.y);
}

/* Test Lava initialization with separated positions */
void test_lava_init_seperated(void) {
    TEST_ASSERT_EQUAL_INT(0, lava1.x);
    TEST_ASSERT_EQUAL_INT(0, lava1.y);
    
    TEST_ASSERT_EQUAL_INT(100, lava2.x);
    TEST_ASSERT_EQUAL_INT(200, lava2.y);
}

/* Test that size is set to LAVA_SIZE */
void test_lava_init_size(void) {
    TEST_ASSERT_EQUAL_INT(LAVA_SIZE, lava1.size);
}

/* Test that sprite pointer is set correctly */
void test_lava_init_sprite(void) {
    TEST_ASSERT_EQUAL_PTR(LAVA_SPRITE, lava1.sprite);
}

/* Test that multiple lava instances can have independent positions */
void test_lava_init_global_sprite(void) {
    /* Verify all lava instances have independent positions */
    TEST_ASSERT_EQUAL_INT(0, lava1.x);
    TEST_ASSERT_EQUAL_INT(0, lava1.y);
    TEST_ASSERT_EQUAL_INT(100, lava2.x);
    TEST_ASSERT_EQUAL_INT(200, lava2.y);

    /* Verify all share the same sprite */
    TEST_ASSERT_EQUAL_PTR(LAVA_SPRITE, lava1.sprite);
    TEST_ASSERT_EQUAL_PTR(LAVA_SPRITE, lava2.sprite);
}

/* Test that sprite dimensions match the configured size */
void test_lava_sprite_size(void) {
    /* Verify sprite array has lava.size rows */
    int num_rows = sizeof(LAVA_SPRITE) / sizeof(LAVA_SPRITE[0]);
    int bits_per_row = (LAVA_SIZE / WORD) * WORD;

    TEST_ASSERT_EQUAL_INT(lava1.size, num_rows);
    TEST_ASSERT_EQUAL_INT(lava1.size, bits_per_row);
}
/* Main function to run all tests */
int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_lava_init_normal);
    RUN_TEST(test_lava_init_zero);
    RUN_TEST(test_lava_init_seperated);
    RUN_TEST(test_lava_init_size);
    RUN_TEST(test_lava_init_sprite);
    RUN_TEST(test_lava_init_global_sprite);
    RUN_TEST(test_lava_sprite_size);

    return UNITY_END();
}
