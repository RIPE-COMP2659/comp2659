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

void test_set_video_base_updates_hardware_registers(void) {
    long old_ssp = Super(0);
    UINT32 original_base = (UINT32)get_video_base();
    UINT32 latched_base;

    set_video_base((UINT16 *)original_base);
    latched_base = (UINT32)get_video_base();
    Super(old_ssp);

    TEST_ASSERT_EQUAL_HEX32(original_base, latched_base);
    
    /* Verify the hardware forced the lowest 8 bits to 0 */
    TEST_ASSERT_EQUAL_HEX32(0x00000000UL, latched_base & 0x000000FFUL);
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_set_video_base_updates_hardware_registers);

    return UNITY_END();
}
