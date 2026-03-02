#include "unity.h"
#include "raster.h"
#include <string.h>

#define SCREEN_SIZE_BYTES 32000
#define SCREEN_SIZE_LONGS (SCREEN_SIZE_BYTES / 4)

/* Mock screen buffer for testing */
static UINT32 mock_screen[SCREEN_SIZE_LONGS];

/* Setup - runs before each test */
void setUp(void) {
    /* Initialize screen with a known pattern (all white) */
    memset(mock_screen, 0xFF, SCREEN_SIZE_BYTES);
}

/* Teardown - runs after each test */
void tearDown(void) {
    /* Nothing to clean up for these tests */
}

/* Test that clear_screen zeros out the entire screen buffer */
void test_clear_screen_zeros_all_bytes(void) {
    int i;
    
    /* Call the assembly function */
    clear_screen(mock_screen);
    
    /* Verify all longs are now zero */
    for (i = 0; i < SCREEN_SIZE_LONGS; i++) {
        TEST_ASSERT_EQUAL_UINT32(0x00000000, mock_screen[i]);
    }
}

/* Test that clear_screen can be called multiple times */
void test_clear_screen_idempotent(void) {
    int i;
    
    /* Clear twice */
    clear_screen(mock_screen);
    clear_screen(mock_screen);
    
    /* Should still be all zeros */
    for (i = 0; i < SCREEN_SIZE_LONGS; i++) {
        TEST_ASSERT_EQUAL_UINT32(0x00000000, mock_screen[i]);
    }
}

/* Test that clear_screen works when buffer already clean */
void test_clear_screen_already_clear(void) {
    int i;
    
    /* Start with zeros */
    memset(mock_screen, 0x00, SCREEN_SIZE_BYTES);
    
    /* Clear it */
    clear_screen(mock_screen);
    
    /* Should still be all zeros */
    for (i = 0; i < SCREEN_SIZE_LONGS; i++) {
        TEST_ASSERT_EQUAL_UINT32(0x00000000, mock_screen[i]);
    }
}

/* Main function to run all tests */
int main(void) {
    UNITY_BEGIN();
    
    RUN_TEST(test_clear_screen_zeros_all_bytes);
    RUN_TEST(test_clear_screen_idempotent);
    RUN_TEST(test_clear_screen_already_clear);
    
    return UNITY_END();
}
