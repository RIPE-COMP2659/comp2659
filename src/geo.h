# include "dtypes.h"

#define GEO_DDY -1
#define GEO_DX 1
#define GEO_DY 0
#define GEO_JUMP_DY 10 /*change for smaller size later*/
#define GEO_SIZE 32

extern const unsigned int GEO_SPRITE[GEO_SIZE][GEO_SIZE / WORD];

typedef struct {
    signed int ddy;
    signed int dx;
    signed int dy;
    signed int is_landed;
    unsigned int x;
    unsigned int y;
    unsigned int size;
    const unsigned int (*sprite)[GEO_SIZE / WORD];
} Geo;

void geo_move(Geo *geo);
void geo_jump(Geo *geo);
Geo create_geo(unsigned int x, unsigned int y);
