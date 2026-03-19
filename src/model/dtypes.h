/**
 * dtypes.h
 * Authors:
 *     Riley Gramlich, rgram060@mtroyal.ca, 201762060
 *     Robert Parker Hutcheson, rhutc335@mtroyal.ca, 201762335
 *     Isaac Klein, iklei977@mtroyal.ca, 201763977
 *     Eduard Mykhailets, emykh268@mtroyal.ca, 201750268
 * Course: COMP 2659-001, Computing Machinery II, Winter 2026
 * Instructor: Nolan Shaw
 *
 * PURPOSE: Shared data type and constant definitions used throughout
 *          the model and rendering subsystems (booleans, sizes,
 *          screen dimensions and collision codes).
 */
#ifndef DTYPES_H
#define DTYPES_H

/* Data sizes */
#define WORD 16

/* Booleans */
#define TRUE 0
#define FALSE -1

/* Colours */
#define BLACK 1
#define WHITE 0

/* Collision types */
/* TODO: There's probably a better way to use typdef or enum*/
#define COLLISION_TOP 1
#define COLLISION_NONE 0
#define COLLISION_BOTTOM -1
#define COLLISION_LEFT -2
#define COLLISION_ERROR -404

/* Screen dimensions */
/* TODO: Should start splitting up constants from shared data types. */
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 400

#endif /* DTYPES_H */
