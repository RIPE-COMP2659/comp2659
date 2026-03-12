#include "unity.h"
#include "raster.h"
#include <string.h>

#define SCREEN_SIZE_BYTES 32000
#define SCREEN_WIDTH_BYTES 80
#define SCREEN_HEIGHT 400

/* Mock screen buffer for testing */
static UINT32 mock_screen[SCREEN_SIZE_BYTES / 4];

/* Helper to check if a single pixel at (row, col) is black (0) or white (1) */
static int get_pixel(UINT32 *base, INT16 row, INT16 col)
{
    UINT8 *byte_ptr = (UINT8 *)base + (row * SCREEN_WIDTH_BYTES) + (col / 8);
    int bit_pos = 7 - (col % 8);
    return (*byte_ptr >> bit_pos) & 1;
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

/* Implement test_plot_triangle_basic
 * Test basic triangle drawing (Direction 0: Top-Left)
 */
void test_plot_triangle_basic(void) {
    /* 90-degree angle at (10, 10), base 10, height 10, Direction 0 */
    plot_triangle(mock_screen, 10, 10, 10, 10, 0);

    /* Verify the base (first row at the 90-degree corner) has full width */
    /* Formula: (10 * (10 - 0)) / 10 = 10 pixels */
    TEST_ASSERT_EQUAL_INT_MESSAGE(1, get_pixel(mock_screen, 10, 10), "Top-Left (Base): start pixel missing");
    TEST_ASSERT_EQUAL_INT_MESSAGE(1, get_pixel(mock_screen, 10, 19), "Top-Left (Base): end pixel missing");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, get_pixel(mock_screen, 10, 20), "Top-Left (Base): overdrawn past width");

    /* Verify the tip (last row, offset 9) has a minimal width */
    /* Formula: (10 * (10 - 9)) / 10 = 1 pixel */
    TEST_ASSERT_EQUAL_INT_MESSAGE(1, get_pixel(mock_screen, 19, 10), "Top-Left (Tip): tip pixel missing");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, get_pixel(mock_screen, 19, 11), "Top-Left (Tip): overdrawn past tip width");
}

/* Implement test_plot_triangle_all_directions
 * Test triangles in all 4 directions to ensure coordinate expansion logic is correct
 */
void test_plot_triangle_all_directions(void) {
    /* 1. Top-Right (Direction 1): Expands left and down */
    plot_triangle(mock_screen, 30, 50, 10, 10, 1);
    /* Base at row 30, spans col 40 to 49 (col - length to col - 1) */
    TEST_ASSERT_EQUAL_INT_MESSAGE(1, get_pixel(mock_screen, 30, 40), "Top-Right base expansion failed");
    TEST_ASSERT_EQUAL_INT_MESSAGE(1, get_pixel(mock_screen, 30, 49), "Top-Right base anchor failed");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, get_pixel(mock_screen, 30, 50), "Top-Right base overdrawn right");
    /* Tip at row 39, spans col 49 to 49 */
    TEST_ASSERT_EQUAL_INT_MESSAGE(1, get_pixel(mock_screen, 39, 49), "Top-Right tip failed");

    /* Clear screen between direction tests */
    memset(mock_screen, 0x00, SCREEN_SIZE_BYTES);

    /* 2. Bottom-Left (Direction 2): Expands right and up */
    plot_triangle(mock_screen, 100, 10, 10, 10, 2);
    /* Base at row 100, spans col 10 to 19 */
    TEST_ASSERT_EQUAL_INT_MESSAGE(1, get_pixel(mock_screen, 100, 10), "Bottom-Left base start failed");
    TEST_ASSERT_EQUAL_INT_MESSAGE(1, get_pixel(mock_screen, 100, 19), "Bottom-Left base end failed");
    /* Tip at row 91 (100 - 9), spans col 10 to 10 */
    TEST_ASSERT_EQUAL_INT_MESSAGE(1, get_pixel(mock_screen, 91, 10), "Bottom-Left tip failed");

    /* Clear screen between direction tests */
    memset(mock_screen, 0x00, SCREEN_SIZE_BYTES);

    /* 3. Bottom-Right (Direction 3): Expands left and up */
    plot_triangle(mock_screen, 200, 50, 10, 10, 3);
    /* Base at row 200, spans col 40 to 49 */
    TEST_ASSERT_EQUAL_INT_MESSAGE(1, get_pixel(mock_screen, 200, 40), "Bottom-Right base expansion failed");
    TEST_ASSERT_EQUAL_INT_MESSAGE(1, get_pixel(mock_screen, 200, 49), "Bottom-Right base anchor failed");
    /* Tip at row 191 (200 - 9), spans col 49 to 49 */
    TEST_ASSERT_EQUAL_INT_MESSAGE(1, get_pixel(mock_screen, 191, 49), "Bottom-Right tip failed");
}

/* Main function to run all tests */
int main(void) {
    UNITY_BEGIN();
    
    RUN_TEST(test_plot_triangle_basic);
    RUN_TEST(test_plot_triangle_all_directions);
    
    return UNITY_END();
}