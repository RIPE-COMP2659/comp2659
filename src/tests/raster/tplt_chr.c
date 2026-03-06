#include "unity.h"
#include "raster.h"
#include <string.h>

#define SCREEN_SIZE_BYTES 32000
#define SCREEN_WIDTH_BYTES 80
#define SCREEN_HEIGHT 400

/* Mock screen buffer for testing */
static UINT8 mock_screen[SCREEN_SIZE_BYTES];

/* Helper to check a specific byte in the mock screen */
static UINT8 get_byte(int row, int col_byte) {
    return mock_screen[(row * SCREEN_WIDTH_BYTES) + col_byte];
}

/* Setup - runs before each test */
void setUp(void) {
    /* Initialize screen with all zeros (black) */
    memset(mock_screen, 0x00, SCREEN_SIZE_BYTES);
}

/* Teardown - runs after each test */
void tearDown(void) {
    /* Nothing to clean up */
}

/* Implement test_plot_character_basic
 * Test plotting individual characters. 
 * We use '!' (ASCII 33) because its bit pattern is distinct.
 */
void test_plot_character_basic(void) {
    /* '!' pattern: rows 2-8 are 0x18, row 10-11 are 0x18. Others 0x00. */
    plot_character(mock_screen, 0, 0, '!');

    /* Check Row 2, Byte 0 (col 0 is byte 0) */
    TEST_ASSERT_EQUAL_HEX8(0x18, get_byte(2, 0));
    /* Check Row 9, Byte 0 (gap in the '!') */
    TEST_ASSERT_EQUAL_HEX8(0x00, get_byte(9, 0));
    /* Check Row 10, Byte 0 (dot of the '!') */
    TEST_ASSERT_EQUAL_HEX8(0x18, get_byte(10, 0));
}

/* Implement test_plot_character_various_positions
 * Test characters at different screen positions to ensure coordinate math is solid.
 */
void test_plot_character_various_positions(void) {
    /* Plot '!' at Row 10, Column 8 (Byte 1) */
    plot_character(mock_screen, 10, 8, '!');

    /* Top of the character should be at row 12 (offset of 2 from start row 10) */
    TEST_ASSERT_EQUAL_HEX8(0x18, get_byte(12, 1));
    
    /* Ensure previous column byte is untouched */
    TEST_ASSERT_EQUAL_HEX8(0x00, get_byte(12, 0));
}

/* Implement test_plot_character_printable_set
 * Test various printable ASCII characters to verify table lookup logic.
 */
void test_plot_character_printable_set(void) {
    int i;
    /* Test Space (ASCII 32) - should be completely empty */
    plot_character(mock_screen, 50, 16, ' ');
    
    for(i = 0; i < 16; i++) {
        TEST_ASSERT_EQUAL_HEX8(0x00, get_byte(50 + i, 2));
    }

    /* Test 'A' (ASCII 65) at Row 100, Col 24 (Byte 3) */
    plot_character(mock_screen, 100, 24, 'A');
    /* Row 2 of 'A' (102 on screen) is pattern 0x1C (center bar top) */
    TEST_ASSERT_NOT_EQUAL_HEX8(0x00, get_byte(102, 3));
}

/* Main function to run all tests */
int main(void) {
    UNITY_BEGIN();
    
    RUN_TEST(test_plot_character_basic);
    RUN_TEST(test_plot_character_various_positions);
    RUN_TEST(test_plot_character_printable_set);
    
    return UNITY_END();
}
