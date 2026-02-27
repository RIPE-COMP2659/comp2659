#ifndef DTYPES_H
#define DTYPES_H

/* Data sizes */
#define WORD 16

/* Booleans */
#define TRUE 0
#define FALSE -1

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
