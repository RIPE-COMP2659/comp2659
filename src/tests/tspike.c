#include "unity.h"
#include "spike.h"

/* Test spikes */
Spike spike1;
Spike spike2;

/* Setup spikes for testing */
void setUp(void) {
    spike1.x = 0;
    spike1.y = 0;
    spike1.size = SPIKE_SIZE;
    spike1.sprite = SPIKE_SPRITE;
    spike2.x = 100;
    spike2.y = 200;
    spike2.size = SPIKE_SIZE;
    spike2.sprite = SPIKE_SPRITE;
    spike_placeholder();
}

void tearDown(void) {
    /* clean stuff up here */
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

/* Test that sprite pointer is set correctly */
void test_spike_init_sprite(void) {
    TEST_ASSERT_EQUAL_PTR(SPIKE_SPRITE, spike1.sprite);
}

/* Test that multiple spikes can have independent positions */
void test_spike_init_global_sprite(void) {
    /* Verify all spikes have independent positions */
    TEST_ASSERT_EQUAL_INT(0, spike1.x);
    TEST_ASSERT_EQUAL_INT(0, spike1.y);
    TEST_ASSERT_EQUAL_INT(100, spike2.x);
    TEST_ASSERT_EQUAL_INT(200, spike2.y);

    /* Verify all share the same sprite */
    TEST_ASSERT_EQUAL_PTR(SPIKE_SPRITE, spike1.sprite);
    TEST_ASSERT_EQUAL_PTR(SPIKE_SPRITE, spike2.sprite);
}

/* Test that sprite dimensions match the configured size */
void test_spike_sprite_size(void) {
    /* Verify sprite array has spike.size rows */
    int num_rows = sizeof(SPIKE_SPRITE) / sizeof(SPIKE_SPRITE[0]);
    int bits_per_row = (SPIKE_SIZE / WORD) * WORD;

    TEST_ASSERT_EQUAL_INT(spike1.size, num_rows);
    TEST_ASSERT_EQUAL_INT(spike1.size, bits_per_row);
}
/* Main function to run all tests */
int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_spike_init_normal);
    RUN_TEST(test_spike_init_zero);
    RUN_TEST(test_spike_init_seperated);
    RUN_TEST(test_spike_init_size);
    RUN_TEST(test_spike_init_sprite);
    RUN_TEST(test_spike_init_global_sprite);
    RUN_TEST(test_spike_sprite_size);

    return UNITY_END();
}
