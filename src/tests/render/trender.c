/**
 * trender.c
 * Authors:
 *     Riley Gramlich, rgram060@mtroyal.ca, 201762060
 *     Robert Parker Hutcheson, rhutc335@mtroyal.ca, 201762335
 *     Isaac Klein, iklei977@mtroyal.ca, 201763977
 *     Eduard Mykhailets, emykh268@mtroyal.ca, 201750268
 * Course: COMP 2659-001, Computing Machinery II, Winter 2026
 * Instructor: Nolan Shaw
 *
 * Unit tests for the rendering module, which relies on the model, and not
 * using a mock, but it at least will verify the memory changes based on
 * rendering versus just displaying things to the screen
 */

#include "unity.h"
#include "render/render.h"
#include "model/model.h"
#include <string.h>

#define SCREEN_SIZE_BYTES 32000
#define SCREEN_WIDTH_BYTES 80

/* Mock screen buffer for testing assertions */
static UINT8 mock_screen[SCREEN_SIZE_BYTES];

/**
 * Helper to check if a pixel at (row, col) is set
 *
 * Params:
 *     UINT8 *base:
 *         The base address of the framebuffer to check
 *     int row:
 *         The row of the pixel to check
 *     int col:
 *         The column of the pixel to check
 *
 * Returns:
 *     static int:
 *         1 if the pixel is set, 0 otherwise
 */
static int get_pixel(UINT8 *base, int row, int col) {
    UINT8 *byte_ptr = base + (row * SCREEN_WIDTH_BYTES) + (col / 8);
    int bit_pos = 7 - (col % 8);
    return (*byte_ptr >> bit_pos) & 1;
}

/* Setup - runs before each test */
void setUp(void) {
    memset(mock_screen, 0x00, SCREEN_SIZE_BYTES);
}

/* Teardown - runs after each test */
void tearDown(void) {
}

/**
 * Verifies that the ground is plotted and player (Geo) appears at
 * the correct relative coordinates.
 */
void test_render_master_frame(void) {
    Model model = get_model();
    int rel_geo_x, rel_geo_y;
    Camera *camera = &model.world.camera;
    int ground_pixels_found = 0;
    int i;

    /* Initialize camera indices */
    model_update_camera(&model);

    /* Render the model to our mock screen */
    render(&model, mock_screen);

    /* Verify Ground (starts at relative Y of ground_y, across SCREEN_WIDTH) */
    /* Check multiple pixels along the ground line to see if any are set */
    rel_geo_y = camera_get_relative_y(camera, model.world.ground_y);
    for (i = 0; i < 640; i++) { /* Check every pixel across width */
        if (get_pixel(mock_screen, rel_geo_y, i)) ground_pixels_found++;
    }

    /* NOTE: This test isn't great, it depends on the initial world state */
    /* For example, this will fail if we remove lava */
    TEST_ASSERT_EQUAL_INT_MESSAGE(512, ground_pixels_found, "Not enough ground pixels found");

    /* Verify Geo (Player) placement */
    rel_geo_x = camera_get_relative_x(camera, model.world.geo.x);
    rel_geo_y = camera_get_relative_y(camera, model.world.geo.y);

    /* We check the top-left pixel of Geo's 32x32 sprite */
    /* Note: This assumes Geo's sprite has a set pixel at (0,0) */
    TEST_ASSERT_EQUAL_INT_MESSAGE(1, get_pixel(mock_screen, rel_geo_y, rel_geo_x), "Geo (Player) not rendered at expected coordinates");
}

/**
 * Verifies that blocks outside the camera window indices are not rendered
 */
void test_render_culling_bounds(void) {
    Model model = get_model();

    /* Manually set indices to 0 to force culling of all level elements
       NOTE: This is a bit silly, it effectively disables the camera */
    model.cam_min_bi = 0;
    model.cam_max_bi = 0;

    render(&model, mock_screen);

    /* Ensure a block that exists in model.world.levels[0].blocks[0]
       didn't draw
       (Assuming blocks[0] would normally be visible at the start) */
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, get_pixel(mock_screen, 200, 100), "Culling failed: Block rendered outside active camera indices");
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_render_master_frame);
    RUN_TEST(test_render_culling_bounds);
    return UNITY_END();
}
