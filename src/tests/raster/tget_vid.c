#include "unity.h"
#include "raster.h"
#include <osbind.h>

/* Setup - runs before each test */
void setUp(void) {
    /* No setup needed */
}

/* Teardown - runs after each test */
void tearDown(void) {
    /* No teardown needed */
}

/*
 * Verify the hardware-register path returns the same framebuffer base
 * as the XBIOS Physbase() call.
 */
void test_get_video_base_matches_physbase(void) {
    long old_ssp = Super(0);
    UINT32 os_base = (UINT32)Physbase();
    UINT32 hw_base = (UINT32)get_video_base();
    Super(old_ssp);

    TEST_ASSERT_EQUAL_HEX32(os_base, hw_base);
    TEST_ASSERT_EQUAL_HEX32(0x00000000UL, hw_base & 0x000000FFUL);
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_get_video_base_matches_physbase);

    return UNITY_END();
}
