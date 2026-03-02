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
    if (row < 0 || row >= SCREEN_HEIGHT || col_byte < 0 || col_byte >= SCREEN_WIDTH_BYTES)
        return 0;
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

/* Implement test_plot_string_basic
 * Test plotting a simple string "!!"
 * Since '!' is 8 pixels wide, the first '!' should be at Byte 0, 
 * and the second '!' should be at Byte 1.
 */
void test_plot_string_basic(void) {
    /* Plot "!!" at row 0, col 0 */
    plot_string(mock_screen, 0, 0, "!!");

    /* '!' pattern has 0x18 at rows 2-8 */
    /* Verify first character at Byte 0 */
    TEST_ASSERT_EQUAL_HEX8_MESSAGE(0x18, get_byte(2, 0), "First character of string failed");
    
    /* Verify second character at Byte 1 (col 8) */
    TEST_ASSERT_EQUAL_HEX8_MESSAGE(0x18, get_byte(2, 1), "Second character of string failed");
}

/* Implement test_plot_string_empty
 * Test plotting an empty string. No memory should be modified.
 */
void test_plot_string_empty(void) {
    int i;
    int modified = 0;
    
    plot_string(mock_screen, 0, 0, "");

    /* Check first few rows/bytes to ensure they are still zero */
    for (i = 0; i < 100; i++) {
        if (mock_screen[i] != 0x00) {
            modified = 1;
            break;
        }
    }

    TEST_ASSERT_FALSE_MESSAGE(modified, "Empty string modified screen memory");
}

/* Implement test_plot_string_various_positions
 * Test strings at different screen positions.
 * Plotting "!" at (row 10, col 16) should put the pattern at Byte 2 of Row 10.
 */
void test_plot_string_various_positions(void) {
    plot_string(mock_screen, 10, 16, "!");

    /* row 10 + font offset 2 = row 12. col 16 = byte 2. */
    TEST_ASSERT_EQUAL_HEX8(0x18, get_byte(12, 2));

    /* Ensure adjacent bytes are untouched */
    TEST_ASSERT_EQUAL_HEX8(0x00, get_byte(12, 1));
    TEST_ASSERT_EQUAL_HEX8(0x00, get_byte(12, 3));
}

/* Main function to run all tests */
int main(void) {
    UNITY_BEGIN();
    
    RUN_TEST(test_plot_string_basic);
    RUN_TEST(test_plot_string_empty);
    RUN_TEST(test_plot_string_various_positions);
    
    return UNITY_END();
}
