#include "unity.h"
#include "helloworld.h"

void setUp(void) {
    // set stuff up here
}

void tearDown(void) {
    // clean stuff up here
}

// Test that add_numbers correctly adds positive numbers
void test_add_numbers_positive_values(void) {
    TEST_ASSERT_EQUAL_INT(15, add_numbers(12, 3));
    TEST_ASSERT_EQUAL_INT(100, add_numbers(50, 50));
    TEST_ASSERT_EQUAL_INT(7, add_numbers(4, 3));
}

// Test that add_numbers correctly handles zero
void test_add_numbers_with_zero(void) {
    TEST_ASSERT_EQUAL_INT(5, add_numbers(5, 0));
    TEST_ASSERT_EQUAL_INT(10, add_numbers(0, 10));
    TEST_ASSERT_EQUAL_INT(0, add_numbers(0, 0));
}

// Test that add_numbers correctly handles negative numbers
void test_add_numbers_negative_values(void) {
    TEST_ASSERT_EQUAL_INT(-5, add_numbers(-2, -3));
    TEST_ASSERT_EQUAL_INT(2, add_numbers(5, -3));
    TEST_ASSERT_EQUAL_INT(-8, add_numbers(-10, 2));
}

// Test edge cases with large numbers
void test_add_numbers_large_values(void) {
    TEST_ASSERT_EQUAL_INT(2000000, add_numbers(1000000, 1000000));
    TEST_ASSERT_EQUAL_INT(0, add_numbers(100, -100));
}

// Main function to run all tests
int main(void) {
    UNITY_BEGIN();
    
    RUN_TEST(test_add_numbers_positive_values);
    RUN_TEST(test_add_numbers_with_zero);
    RUN_TEST(test_add_numbers_negative_values);
    RUN_TEST(test_add_numbers_large_values);
    
    return UNITY_END();
}