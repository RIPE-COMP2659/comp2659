#include "unity.h"
#include "lava.h"

/* Test lava */
Lava lava1;
Lava lava2;

/* Setup lava for testing */
void setUp(void) {
    lava1 = create_lava(0, 0);
    lava2 = create_lava(100, 200);
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

/* Test that multiple lava instances can have independent positions */
void test_lava_init_global_sprite(void) {
    /* Verify all lava instances have independent positions */
    TEST_ASSERT_EQUAL_INT(0, lava1.x);
    TEST_ASSERT_EQUAL_INT(0, lava1.y);
    TEST_ASSERT_EQUAL_INT(100, lava2.x);
    TEST_ASSERT_EQUAL_INT(200, lava2.y);

    /* Verify all share the same sprite */
    TEST_ASSERT_EQUAL_PTR(lava1.sprite, lava2.sprite);
}

/* Main function to run all tests */
int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_lava_init_normal);
    RUN_TEST(test_lava_init_zero);
    RUN_TEST(test_lava_init_seperated);
    RUN_TEST(test_lava_init_size);
    RUN_TEST(test_lava_init_global_sprite);

    return UNITY_END();
}
