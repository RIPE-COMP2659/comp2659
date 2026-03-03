#include "unity.h"
#include "spike.h"

/* Test spikes */
Spike spike1;
Spike spike2;

/* Setup before each test */
void setUp(void) {
    spike1 = create_spike(0, 0);
    spike2 = create_spike(100, 200);
}

/* Teardown after each test */
void tearDown(void) {
    /* Nothing currently to cleanup */
}

/* Test basic Spike initialization with normal x and y values */
void test_spike_init_normal(void) {
    TEST_ASSERT_EQUAL_INT(100, spike2.x);
    TEST_ASSERT_EQUAL_INT(200, spike2.y);
}

/* Test Spike initialization with zero values */
void test_spike_init_zero(void) {
    TEST_ASSERT_EQUAL_INT(0, spike1.x);
    TEST_ASSERT_EQUAL_INT(0, spike1.y);
}

/* Test Spike initialization with separated positions */
void test_spike_init_seperated(void) {
    TEST_ASSERT_EQUAL_INT(0, spike1.x);
    TEST_ASSERT_EQUAL_INT(0, spike1.y);
    
    TEST_ASSERT_EQUAL_INT(100, spike2.x);
    TEST_ASSERT_EQUAL_INT(200, spike2.y);
}

/* Test that size is set to SPIKE_SIZE */
void test_spike_init_size(void) {
    TEST_ASSERT_EQUAL_INT(SPIKE_SIZE, spike1.size);
}

/* Test that multiple spikes can have independent positions */
void test_spike_init_global_sprite(void) {
    /* Verify all spikes have independent positions */
    TEST_ASSERT_EQUAL_INT(0, spike1.x);
    TEST_ASSERT_EQUAL_INT(0, spike1.y);
    TEST_ASSERT_EQUAL_INT(100, spike2.x);
    TEST_ASSERT_EQUAL_INT(200, spike2.y);

    /* Verify all share the same sprite */
    TEST_ASSERT_EQUAL_PTR(spike2.sprite, spike1.sprite);
}

/* Run all tests */
int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_spike_init_normal);
    RUN_TEST(test_spike_init_zero);
    RUN_TEST(test_spike_init_seperated);
    RUN_TEST(test_spike_init_size);
    RUN_TEST(test_spike_init_global_sprite);

    return UNITY_END();
}
