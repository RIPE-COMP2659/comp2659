#include <stdio.h> /* Temporary printf import */
#include "block.h"
#include "spike.h"

int main(void) {
    /* For now, just a placeholder */
    Block test_block = {100, 200, BLOCK_SIZE, BLOCK_SPRITE};
    Spike test_spike = {300, 400, SPIKE_SIZE, SPIKE_SPRITE};

    printf("Running main!\n");

    return 0;
}