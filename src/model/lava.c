/**
 * lava.c
 * Authors:
 *     Riley Gramlich, rgram060@mtroyal.ca, 201762060
 *     Robert Parker Hutcheson, rhutc335@mtroyal.ca, 201762335
 *     Isaac Klein, iklei977@mtroyal.ca, 201763977
 *     Eduard Mykhailets, emykh268@mtroyal.ca, 201750268
 * Course: COMP 2659-001, Computing Machinery II, Winter 2026
 * Instructor: Nolan Shaw
 *
 * PURPOSE: Lava entity implementation and sprite data.
 */
#include "lava.h"

/* Global Lava sprite */
static const unsigned int LAVA_SPRITE[LAVA_SIZE][LAVA_SIZE / WORD] = {
    {0x0000, 0x0000}, /* 1, white */
    {0x0000, 0x0000},

    {0xAAAA, 0xAAAA}, /* 3, checkerboard */
    {0x5555, 0x5555},
    {0xAAAA, 0xAAAA},
    {0x5555, 0x5555},
    {0xAAAA, 0xAAAA},
    {0x5555, 0x5555},
    {0xAAAA, 0xAAAA},
    {0x5555, 0x5555},
    {0xAAAA, 0xAAAA},
    {0x5555, 0x5555}, /* 12 */

    {0xFFFF, 0xFFFF}, /* 13, black */
    {0xFFFF, 0xFFFF},
    {0xFFFF, 0xFFFF},
    {0xFFFF, 0xFFFF},
    {0xFFFF, 0xFFFF},
    {0xFFFF, 0xFFFF},
    {0xFFFF, 0xFFFF},
    {0xFFFF, 0xFFFF}, /* 20 */
    {0xFFFF, 0xFFFF},
    {0xFFFF, 0xFFFF},
    {0xFFFF, 0xFFFF},
    {0xFFFF, 0xFFFF},
    {0xFFFF, 0xFFFF},
    {0xFFFF, 0xFFFF},
    {0xFFFF, 0xFFFF},
    {0xFFFF, 0xFFFF}, /* 28 */
    {0xFFFF, 0xFFFF},
    {0xFFFF, 0xFFFF},
    {0xFFFF, 0xFFFF},
    {0xFFFF, 0xFFFF} /* 32 */
};

Lava create_lava(unsigned int x, unsigned int y)
{
    Lava lava;

    lava.x = x;
    lava.y = y;
    lava.size = LAVA_SIZE;
    lava.sprite = LAVA_SPRITE;

    return lava;
}
