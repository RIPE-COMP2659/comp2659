# 1 "test.c" 1
# 1 "./raster.h" 1
# 10 "./raster.h"
typedef char INT8;
typedef short INT16;
typedef long INT32;

typedef unsigned char UINT8;
typedef unsigned short UINT16;
typedef unsigned long UINT32;
# 27 "./raster.h"
void clear_screen(UINT32 *base);
# 41 "./raster.h"
void clear_region(UINT32 *base, INT16 row, INT16 col, UINT16 length, UINT16 width);
# 53 "./raster.h"
void plot_pixel(UINT8 *base, INT16 row, INT16 col);
# 66 "./raster.h"
void plot_horizontal_line(UINT32 *base, INT16 row, INT16 col, UINT16 length, UINT16 color);
# 79 "./raster.h"
void plot_vertical_line(UINT32 *base, INT16 row, INT16 col, UINT16 length, UINT16 color);
# 92 "./raster.h"
void plot_line(UINT32 *base, INT16 start_row, INT16 start_col, INT16 end_row, INT16 end_col, UINT16 color);
# 105 "./raster.h"
void plot_rectangle(UINT32 *base, INT16 row, INT16 col, UINT16 length, UINT16 width);
# 117 "./raster.h"
void plot_square(UINT32 *base, INT16 row, INT16 col, UINT16 side);
# 135 "./raster.h"
void plot_triangle(UINT32 *base, INT16 row, INT16 col, UINT16 base_triangle, UINT16 height, UINT8 direction);
# 148 "./raster.h"
void plot_bitmap_8(UINT8 *base, INT16 row, INT16 col, const UINT8 *bitmap, UINT16 height);
# 161 "./raster.h"
void plot_bitmap_16(UINT8 *base, INT16 row, INT16 col, const UINT16 *bitmap, UINT16 height);
# 174 "./raster.h"
void plot_bitmap_32(UINT8 *base, INT16 row, INT16 col, const UINT32 *bitmap, UINT16 height);
# 186 "./raster.h"
void plot_character(UINT8 *base, INT16 row, INT16 col, char ch);
# 198 "./raster.h"
void plot_string(UINT8 *base, INT16 row, INT16 col, char *ch);
# 209 "./raster.h"
void init_buffers(void);
# 219 "./raster.h"
INT8 *get_buffer1(void);
# 229 "./raster.h"
INT8 *get_buffer2(void);
# 239 "./raster.h"
INT8 *get_buffer3(void);
# 249 "./raster.h"
INT8 *get_buffer4(void);
# 1 "test.c" 2
# 1 "C:/c68/include/stdio.h" 1
# 1 "C:/c68/include/compiler.h" 1
# 11 "C:/c68/include/stdio.h" 2
# 20 "C:/c68/include/stdio.h"
typedef unsigned long  size_t;
# 69 "C:/c68/include/stdio.h"
typedef	struct
	{
	long		_cnt;
	unsigned char	*_ptr;
	unsigned char	*_base;
	unsigned int	_flag;
	int		_file;
	long		_bsiz;
	unsigned char	_ch;
	}
	FILE;


typedef unsigned long fpos_t;
# 92 "C:/c68/include/stdio.h"
extern	FILE	_iob[];
# 108 "C:/c68/include/stdio.h"
 char *	ctermid	(char *) ;




 int	remove	(const char *) ;
 int	rename	(const char *, const char *) ;
 char *	tmpnam	(char *) ;
 FILE *	tmpfile	(void) ;

 int	fclose	(FILE *) ;
 int	fflush	(FILE *) ;

 FILE *	fopen	(const char *, const char *) ;
 FILE *	freopen	(const char *, const char *, FILE *) ;

 void	setbuf	(FILE *, char *) ;
 int	setvbuf	(FILE *, char *, int, size_t) ;
# 132 "C:/c68/include/stdio.h"
 int  fscanf  (FILE *, const char *, ...) ;
 int  scanf   (const char *, ...) ;
 int  sscanf  (const char *, const char *, ...) ;


 int	fprintf	(FILE *, const char *, ...) ;
 int	printf	(const char *, ...) ;
 int	sprintf	(char *, const char *, ...) ;

 int 	vfprintf (FILE *, const char *, char * ) ;
 int 	vprintf	 (const char *, char * ) ;
 int 	vsprintf (char *, const char *, char * ) ;

 int	vscanf  (const char *, char * ) ;
 int	vfscanf (FILE *, const char *, char * ) ;
 int	vsscanf	(const char *, const char *, char * ) ;


 int	fgetc	(FILE *) ;
 char	*fgets	(char *, int, FILE *) ;
 char	*gets	(char *) ;
 int	fputc	(int c, FILE *) ;
 int	fputs	(const char *, FILE *) ;
 int	puts	(const char *) ;

 size_t	fread	(void *, size_t, size_t, FILE *) ;
 size_t	fwrite	(const void *, size_t, size_t, FILE *) ;

 int	fgetpos	(FILE *, fpos_t *) ;
 int	fsetpos	(FILE *, fpos_t *) ;

 int	fseek	(FILE *, long, int) ;
 long	ftell	(FILE *) ;
 void	rewind	(FILE *) ;

 void	perror	(const char *) ;


 FILE	*fdopen	(int, const char *) ;


 FILE *	fopenp	(const char *, const char *) ;
 int 	fungetc	(int, FILE *) ;
 int	pclose	(FILE *) ;
 FILE *	popen	(const char *, const char *) ;
 void	setlinebuf	(FILE *) ;

 void	_binmode	(int) ;
 long 	getl	(FILE *) ;
 long 	putl	(long, FILE *) ;
 short 	getw	(FILE *) ;
 short 	putw	(short, FILE *) ;
 void	_getbuf	(FILE *fp) ;
# 192 "C:/c68/include/stdio.h"
 int	_filbuf	(FILE *) ;
# 2 "test.c" 2
# 1 "C:/c68/include/osbind.h" 1
# 1 "C:/c68/include/ostruct.h" 1
# 22 "C:/c68/include/ostruct.h"
typedef struct {
    long b_free;
    long b_total;
    long b_secsiz;
    long b_clsiz;
} _DISKINFO;


typedef struct {
  short time;
  short date;
} _DOSTIME;


typedef struct
{
        unsigned char maxlen;
        unsigned char actuallen;
        char    buffer[255];
} _CCONLINE;
# 48 "C:/c68/include/ostruct.h"
typedef struct _dta {
    char 	    dta_buf[21];
    char            dta_attribute;
    unsigned short  dta_time;
    unsigned short  dta_date;
    long            dta_size;
    char            dta_name[14];
} _DTA;
# 98 "C:/c68/include/ostruct.h"
typedef struct {
  short recsiz;
  short clsiz;
  short clsizb;
  short rdlen;
  short fsiz;
  short fatrec;
  short datrec;
  short numcl;
  short bflags;
} _BPB;




typedef struct _md {
    struct _md	*md_next;
    long	 md_start;
    long	 md_length;
    long	 md_owner;
} _MD;


typedef struct {
    _MD *mp_free;
    _MD *mp_used;
    _MD *mp_rover;
} _MPB;
# 141 "C:/c68/include/ostruct.h"
typedef struct {
    char    *ibuf;
    short   ibufsiz;
    volatile short   ibufhd;
    volatile short   ibuftl;
    short   ibuflow;
    short   ibufhi;
} _IOREC;


typedef struct {
	char	topmode;
	char	buttons;
	char	xparam;
	char	yparam;
	short	xmax;
	short	ymax;
	short	xstart;
	short	ystart;
} _PARAM;


typedef struct {
    void    (*midivec)	(void) ;
    void    (*vkbderr)	(void) ;
    void    (*vmiderr)	(void) ;
    void    (*statvec)	(void *) ;
    void    (*mousevec)	(void *) ;
    void    (*clockvec)	(void *) ;
    void    (*joyvec)	(void *) ;
    long    (*midisys)	(void) ;
    long    (*ikbdsys)	(void) ;
    char    kbstate;
} _KBDVECS;


typedef struct {
    void *unshift;
    void *shift;
    void *caps;
} _KEYTAB;


typedef struct
{
        void    *pb_scrptr;
        int     pb_offset;
        int     pb_width;
        int     pb_height;
        int     pb_left;
        int     pb_right;
        int     pb_screz;
        int     pb_prrez;
        void    *pb_colptr;
        int     pb_prtype;
        int     pb_prport;
        void    *pb_mask;
} _PBDEF;
# 17 "C:/c68/include/osbind.h" 2
# 33 "C:/c68/include/osbind.h"
 long _trap_1_w		(short) ;
 long _trap_1_ww	(short,short) ;
 long _trap_1_wl	(short,long) ;
 long _trap_1_wlw	(short,long,short) ;
 long _trap_1_wwll	(short,short,long,long) ;
 long _trap_1_wlww	(short,long,short,short) ;
 long _trap_1_www	(short,short,short) ;
 long _trap_1_wll	(short,long,long) ;
 long _trap_1_wwlll	(short,short,long,long,long) ;
 long _trap_1_wwwll	(short,short,short,long,long) ;
 long _trap_13_wl	(short,long) ;
 long _trap_13_w	(short) ;
 long _trap_13_ww	(short,short) ;
 long _trap_13_www	(short,short,short) ;
 long _trap_13_wwlwww	(short,short,long,short,short,short) ;
 long _trap_13_wwl	(short,short,long) ;
 long _trap_14_wwl	(short,short,long) ;
 long _trap_14_wwll	(short,short,long,long) ;
 long _trap_14_ww	(short,short) ;
 long _trap_14_w	(short) ;
 long _trap_14_wllw	(short,long,long,short) ;
 long _trap_14_wl	(short,long) ;
 long _trap_14_www	(short,short,short) ;
 long _trap_14_wllwwwww (short,long,long,short,short,short,short,short) ;
 long _trap_14_wllwwwwlw (short,long,long,short,short,short,short,long,short) ;
 long _trap_14_wllwwwwwlw (short,long,long,short,short,short,short,short,long,short) ;
 long _trap_14_wwwwwww	(short,short,short,short,short,short,short) ;
 long _trap_14_wlll	(short,long,long,long) ;
 long _trap_14_wllww	(short,long,long,short,short) ;
 long _trap_14_wwwwl	(short,short,short,short,long) ;
 long _trap_14_wwwl	(short,short,short,long) ;
 long _trap_14_wlwlw	(short,long,short,long,short) ;
# 103 "C:/c68/include/osbind.h"
 long _trap_1_ 	(short,...) ;
 long _trap_14_ 	(short,...) ;
 long _trap_13_ 	(short,...) ;
# 3 "test.c" 2




void disable_cursor();
void fill_screen(UINT32 *base, char pattern);
void test_clear_screen(UINT8 *base);
void test_clear_region(UINT8 *base);
void test_plot_pixel(UINT8 *base);
void test_plot_horizontal_line(UINT8 *base);
void test_plot_vertical_line(UINT8 *base);
void test_plot_line(UINT8 *base);
void test_plot_rectangle(UINT8 *base);
void test_plot_square(UINT8 *base);
void test_plot_triangle(UINT8 *base);
void test_plot_bitmap_8(UINT8 *base);
void test_plot_bitmap_16(UINT8 *base);
void test_plot_bitmap_32(UINT8 *base);
void test_plot_character(UINT8 *base);
void test_plot_string(UINT8 *base);

int main()
{
    UINT8 *base = (UINT8 *)(void*)_trap_14_w((short)0x2) ;

    disable_cursor();


    fill_screen((UINT32 *)base, -1);
    _trap_1_w((short)0x08) ;
    test_clear_screen(base);
    _trap_1_w((short)0x08) ;


    test_clear_region(base);
    _trap_1_w((short)0x08) ;


    test_plot_pixel(base);
    _trap_1_w((short)0x08) ;


    test_plot_horizontal_line(base);
    _trap_1_w((short)0x08) ;


    test_plot_vertical_line(base);
    _trap_1_w((short)0x08) ;


    test_plot_line(base);
    _trap_1_w((short)0x08) ;


    test_plot_rectangle(base);
    _trap_1_w((short)0x08) ;


    test_plot_square(base);
    _trap_1_w((short)0x08) ;


    test_plot_triangle(base);
    _trap_1_w((short)0x08) ;


    test_plot_bitmap_8(base);
    _trap_1_w((short)0x08) ;


    test_plot_bitmap_16(base);
    _trap_1_w((short)0x08) ;


    test_plot_bitmap_32(base);
    _trap_1_w((short)0x08) ;


    test_plot_character(base);
    _trap_1_w((short)0x08) ;


    test_plot_string(base);
    _trap_1_w((short)0x08) ;

    return 0;
}

void test_clear_screen(UINT8 *base)
{
    clear_screen((UINT32 *)base);
}

void test_clear_region(UINT8 *base)
{
    int i;


    fill_screen((UINT32 *)base, -1);


    clear_region((UINT32 *)base, 0, 0, 48, 48);


    clear_region((UINT32 *)base, 50, 80, 48, 48);


    clear_region((UINT32 *)base, 10, 5, 10, 10);


    clear_region((UINT32 *)base, 100, 50, 20, 100);


    clear_region((UINT32 *)base, 150, 200, 50, 8);


    clear_region((UINT32 *)base, 200, 5, 15, 4);
}

void test_plot_pixel(UINT8 *base)
{
    int i;


    clear_screen((UINT32 *)base);


    for (i = 0; i < 100; i++)
    {
        plot_pixel(base, 10, i);
    }


    for (i = 20; i < 120; i++)
    {
        plot_pixel(base, i, 50);
    }


    for (i = 0; i < 50; i++)
    {
        plot_pixel(base, 30 + i, 100 + i);
    }


    for (i = 200; i <= 280; i++)
    {
        plot_pixel(base, 200, i);
        plot_pixel(base, 250, i);
    }
    for (i = 200; i <= 250; i++)
    {
        plot_pixel(base, i, 200);
        plot_pixel(base, i, 280);
    }


    for (i = 0; i < 64; i += 8)
    {
        plot_pixel(base, 300, i);
        plot_pixel(base, 301, i + 4);
        plot_pixel(base, 302, i + 7);
    }
}

void test_plot_horizontal_line(UINT8 *base)
{

    clear_screen((UINT32 *)base);


    plot_horizontal_line((UINT32 *)base, 10, 10, 50, 1);


    plot_horizontal_line((UINT32 *)base, 20, 0, 100, 1);


    plot_horizontal_line((UINT32 *)base, 30, 1, 40, 1);
    plot_horizontal_line((UINT32 *)base, 31, 2, 40, 1);
    plot_horizontal_line((UINT32 *)base, 32, 3, 40, 1);
    plot_horizontal_line((UINT32 *)base, 33, 4, 40, 1);
    plot_horizontal_line((UINT32 *)base, 34, 5, 40, 1);
    plot_horizontal_line((UINT32 *)base, 35, 6, 40, 1);
    plot_horizontal_line((UINT32 *)base, 36, 7, 40, 1);


    plot_horizontal_line((UINT32 *)base, 50, 5, 300, 1);


    plot_horizontal_line((UINT32 *)base, 70, 10, 1, 1);
    plot_horizontal_line((UINT32 *)base, 72, 10, 3, 1);
    plot_horizontal_line((UINT32 *)base, 74, 10, 5, 1);
    plot_horizontal_line((UINT32 *)base, 76, 10, 8, 1);
    plot_horizontal_line((UINT32 *)base, 78, 10, 10, 1);


    plot_horizontal_line((UINT32 *)base, 100, 7, 16, 1);
    plot_horizontal_line((UINT32 *)base, 102, 15, 16, 1);


    plot_horizontal_line((UINT32 *)base, 120, 0, 640, 1);


    plot_horizontal_line((UINT32 *)base, 130, 100, 0, 1);


    plot_horizontal_line((UINT32 *)base, 150, 50, 100, 1);
    plot_horizontal_line((UINT32 *)base, 155, 60, 100, 1);
    plot_horizontal_line((UINT32 *)base, 160, 70, 100, 1);
    plot_horizontal_line((UINT32 *)base, 165, 80, 100, 1);
    plot_horizontal_line((UINT32 *)base, 170, 90, 100, 1);
}

void test_plot_vertical_line(UINT8 *base)
{

    clear_screen((UINT32 *)base);


    plot_vertical_line((UINT32 *)base, 10, 10, 50, 1);


    plot_vertical_line((UINT32 *)base, 0, 20, 100, 1);


    plot_vertical_line((UINT32 *)base, 10, 1, 40, 1);
    plot_vertical_line((UINT32 *)base, 10, 9, 40, 1);
    plot_vertical_line((UINT32 *)base, 10, 17, 40, 1);
    plot_vertical_line((UINT32 *)base, 10, 25, 40, 1);
    plot_vertical_line((UINT32 *)base, 10, 33, 40, 1);
    plot_vertical_line((UINT32 *)base, 10, 41, 40, 1);
    plot_vertical_line((UINT32 *)base, 10, 49, 40, 1);


    plot_vertical_line((UINT32 *)base, 5, 80, 300, 1);


    plot_vertical_line((UINT32 *)base, 10, 100, 1, 1);
    plot_vertical_line((UINT32 *)base, 20, 100, 3, 1);
    plot_vertical_line((UINT32 *)base, 30, 100, 5, 1);
    plot_vertical_line((UINT32 *)base, 40, 100, 8, 1);
    plot_vertical_line((UINT32 *)base, 50, 100, 10, 1);


    plot_vertical_line((UINT32 *)base, 100, 0, 30, 1);
    plot_vertical_line((UINT32 *)base, 100, 8, 30, 1);
    plot_vertical_line((UINT32 *)base, 100, 16, 30, 1);
    plot_vertical_line((UINT32 *)base, 100, 24, 30, 1);


    plot_vertical_line((UINT32 *)base, 0, 150, 400, 1);


    plot_vertical_line((UINT32 *)base, 100, 200, 0, 1);


    plot_horizontal_line((UINT32 *)base, 150, 200, 100, 1);
    plot_horizontal_line((UINT32 *)base, 180, 200, 100, 1);
    plot_horizontal_line((UINT32 *)base, 210, 200, 100, 1);
    plot_vertical_line((UINT32 *)base, 150, 200, 60, 1);
    plot_vertical_line((UINT32 *)base, 150, 230, 60, 1);
    plot_vertical_line((UINT32 *)base, 150, 260, 60, 1);
    plot_vertical_line((UINT32 *)base, 150, 290, 60, 1);
}

void test_plot_line(UINT8 *base)
{

    clear_screen((UINT32 *)base);


    plot_line((UINT32 *)base, 10, 10, 10, 100, 1);


    plot_line((UINT32 *)base, 20, 20, 80, 20, 1);


    plot_line((UINT32 *)base, 10, 120, 60, 170, 1);


    plot_line((UINT32 *)base, 60, 180, 10, 230, 1);


    plot_line((UINT32 *)base, 100, 50, 180, 70, 1);


    plot_line((UINT32 *)base, 100, 100, 120, 200, 1);


    plot_line((UINT32 *)base, 150, 250, 150, 150, 1);


    plot_line((UINT32 *)base, 200, 50, 150, 50, 1);


    plot_line((UINT32 *)base, 150, 300, 200, 250, 1);


    plot_line((UINT32 *)base, 200, 350, 150, 300, 1);


    plot_line((UINT32 *)base, 250, 400, 300, 350, 1);
    plot_line((UINT32 *)base, 300, 350, 300, 450, 1);
    plot_line((UINT32 *)base, 300, 450, 250, 400, 1);


    plot_line((UINT32 *)base, 320, 500, 360, 500, 1);
    plot_line((UINT32 *)base, 340, 480, 340, 520, 1);
    plot_line((UINT32 *)base, 325, 485, 355, 515, 1);
    plot_line((UINT32 *)base, 325, 515, 355, 485, 1);


    plot_line((UINT32 *)base, 380, 100, 380, 100, 1);


    plot_line((UINT32 *)base, 10, 550, 380, 630, 1);
}

void test_plot_rectangle(UINT8 *base)
{

    clear_screen((UINT32 *)base);


    plot_rectangle((UINT32 *)base, 0, 0, 48, 48);


    plot_rectangle((UINT32 *)base, 50, 80, 48, 48);


    plot_rectangle((UINT32 *)base, 10, 5, 10, 10);


    plot_rectangle((UINT32 *)base, 100, 50, 20, 100);


    plot_rectangle((UINT32 *)base, 150, 200, 50, 8);


    plot_rectangle((UINT32 *)base, 200, 5, 15, 4);


    plot_rectangle((UINT32 *)base, 250, 10, 1, 1);


    plot_rectangle((UINT32 *)base, 10, 300, 30, 20);
    plot_rectangle((UINT32 *)base, 50, 350, 25, 35);
    plot_rectangle((UINT32 *)base, 100, 400, 40, 15);


    plot_rectangle((UINT32 *)base, 200, 300, 100, 150);
}

void test_plot_square(UINT8 *base)
{

    clear_screen((UINT32 *)base);


    plot_square((UINT32 *)base, 0, 0, 48);


    plot_square((UINT32 *)base, 50, 80, 48);


    plot_square((UINT32 *)base, 10, 5, 10);


    plot_square((UINT32 *)base, 100, 50, 25);


    plot_square((UINT32 *)base, 150, 200, 8);


    plot_square((UINT32 *)base, 200, 5, 15);


    plot_square((UINT32 *)base, 250, 10, 1);


    plot_square((UINT32 *)base, 10, 300, 20);
    plot_square((UINT32 *)base, 50, 350, 30);
    plot_square((UINT32 *)base, 100, 400, 40);


    plot_square((UINT32 *)base, 200, 300, 100);
}

void test_plot_triangle(UINT8 *base)
{

    clear_screen((UINT32 *)base);


    plot_triangle((UINT32 *)base, 10, 10, 50, 50, 0);


    plot_triangle((UINT32 *)base, 10, 150, 50, 50, 1);


    plot_triangle((UINT32 *)base, 100, 10, 50, 50, 2);


    plot_triangle((UINT32 *)base, 100, 150, 50, 50, 3);


    plot_triangle((UINT32 *)base, 150, 10, 30, 40, 0);
    plot_triangle((UINT32 *)base, 150, 60, 20, 30, 0);
    plot_triangle((UINT32 *)base, 150, 100, 40, 20, 0);


    plot_triangle((UINT32 *)base, 200, 100, 30, 40, 1);
    plot_triangle((UINT32 *)base, 200, 150, 20, 30, 1);


    plot_triangle((UINT32 *)base, 300, 10, 30, 40, 2);
    plot_triangle((UINT32 *)base, 300, 60, 20, 30, 2);


    plot_triangle((UINT32 *)base, 350, 100, 30, 40, 3);
    plot_triangle((UINT32 *)base, 350, 150, 20, 30, 3);


    plot_triangle((UINT32 *)base, 10, 300, 80, 100, 0);
    plot_triangle((UINT32 *)base, 10, 500, 80, 100, 1);
    plot_triangle((UINT32 *)base, 200, 300, 80, 100, 2);
    plot_triangle((UINT32 *)base, 200, 500, 80, 100, 3);
}

void disable_cursor()
{
    printf("\033f");
    fflush((&_iob[1]) );
}

void test_plot_bitmap_8(UINT8 *base)
{



    const UINT8 checkerboard[] = {
        0xAA,
        0x55,
        0xAA,
        0x55,
        0xAA,
        0x55,
        0xAA,
        0x55
    };


    const UINT8 solid_block[] = {
        0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF};


    const UINT8 arrow_down[] = {
        0x18,
        0x18,
        0x18,
        0x18,
        0x18,
        0x18,
        0xFF,
        0x7E,
        0x3C,
        0x18,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00
    };


    const UINT8 smiley[] = {
        0x3C,
        0x42,
        0xA5,
        0x81,
        0xA5,
        0x99,
        0x42,
        0x3C
    };


    const UINT8 cross[] = {
        0x18,
        0x18,
        0x18,
        0xFF,
        0xFF,
        0x18,
        0x18,
        0x18
    };


    const UINT8 diagonal[] = {
        0x80,
        0x40,
        0x20,
        0x10,
        0x08,
        0x04,
        0x02,
        0x01
    };


    clear_screen((UINT32 *)base);


    plot_bitmap_8(base, 10, 0, checkerboard, 8);


    plot_bitmap_8(base, 10, 16, solid_block, 8);


    plot_bitmap_8(base, 10, 25, smiley, 8);


    plot_bitmap_8(base, 30, 0, cross, 8);
    plot_bitmap_8(base, 30, 10, diagonal, 8);
    plot_bitmap_8(base, 30, 20, checkerboard, 8);
    plot_bitmap_8(base, 30, 30, smiley, 8);


    plot_bitmap_8(base, 50, 8, arrow_down, 16);


    plot_bitmap_8(base, 80, 0, solid_block, 8);
    plot_bitmap_8(base, 80, 8, checkerboard, 8);
    plot_bitmap_8(base, 80, 16, cross, 8);
    plot_bitmap_8(base, 80, 24, diagonal, 8);
    plot_bitmap_8(base, 80, 32, smiley, 8);
    plot_bitmap_8(base, 80, 40, solid_block, 8);


    plot_bitmap_8(base, 100, 0, checkerboard, 8);
    plot_bitmap_8(base, 100, 10, smiley, 8);
    plot_bitmap_8(base, 100, 20, cross, 8);
    plot_bitmap_8(base, 110, 0, diagonal, 8);
    plot_bitmap_8(base, 110, 10, solid_block, 8);
    plot_bitmap_8(base, 110, 20, checkerboard, 8);
    plot_bitmap_8(base, 120, 0, cross, 8);
    plot_bitmap_8(base, 120, 10, diagonal, 8);
    plot_bitmap_8(base, 120, 20, smiley, 8);


    plot_bitmap_8(base, 140, 0, arrow_down, 15);


    plot_bitmap_8(base, 170, 1, smiley, 8);
    plot_bitmap_8(base, 170, 11, smiley, 8);
    plot_bitmap_8(base, 170, 21, smiley, 8);
    plot_bitmap_8(base, 170, 31, smiley, 8);


    clear_screen((UINT32 *)base);
    plot_string(base, 5, 10, "Left Edge Clipping:");
    plot_bitmap_8(base, 20, -4, solid_block, 8);
    plot_bitmap_8(base, 30, -2, checkerboard, 8);
    plot_bitmap_8(base, 40, 0, cross, 8);


    plot_string(base, 60, 10, "Right Edge Clipping:");
    plot_bitmap_8(base, 80, 636, solid_block, 8);
    plot_bitmap_8(base, 90, 638, checkerboard, 8);
    plot_bitmap_8(base, 100, 632, cross, 8);


    plot_bitmap_8(base, -20, 100, smiley, 8);
    plot_bitmap_8(base, 450, 100, smiley, 8);
    plot_bitmap_8(base, 100, -20, smiley, 8);
    plot_bitmap_8(base, 100, 650, smiley, 8);
    _trap_1_w((short)0x08) ;
}

void test_plot_bitmap_16(UINT8 *base)
{



    const UINT16 checkerboard_16[] = {
        0xAAAA,
        0x5555,
        0xAAAA,
        0x5555,
        0xAAAA,
        0x5555,
        0xAAAA,
        0x5555
    };


    const UINT16 solid_block_16[] = {
        0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
        0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF};


    const UINT16 smiley_16[] = {
        0x0FF0,
        0x3FFC,
        0x7FFE,
        0x7E7E,
        0xF3CF,
        0xF3CF,
        0xFFFF,
        0xFFFF,
        0xFFFF,
        0xF3CF,
        0xF3CF,
        0x7FFE,
        0x7FFE,
        0x3FFC,
        0x0FF0,
        0x0000
    };


    const UINT16 frame_16[] = {
        0xFFFF,
        0xFFFF,
        0xC003,
        0xC003,
        0xC003,
        0xC003,
        0xC003,
        0xC003,
        0xC003,
        0xC003,
        0xC003,
        0xC003,
        0xC003,
        0xC003,
        0xFFFF,
        0xFFFF
    };


    const UINT16 diamond_16[] = {
        0x0180,
        0x03C0,
        0x07E0,
        0x0FF0,
        0x1FF8,
        0x3FFC,
        0x7FFE,
        0xFFFF,
        0xFFFF,
        0x7FFE,
        0x3FFC,
        0x1FF8,
        0x0FF0,
        0x07E0,
        0x03C0,
        0x0180
    };


    clear_screen((UINT32 *)base);


    plot_bitmap_16(base, 10, 0, checkerboard_16, 8);


    plot_bitmap_16(base, 10, 32, solid_block_16, 8);


    plot_bitmap_16(base, 10, 48, smiley_16, 16);


    plot_bitmap_16(base, 30, 0, frame_16, 16);
    plot_bitmap_16(base, 30, 20, diamond_16, 16);
    plot_bitmap_16(base, 30, 40, checkerboard_16, 8);


    plot_bitmap_16(base, 60, 0, solid_block_16, 8);
    plot_bitmap_16(base, 60, 16, checkerboard_16, 8);
    plot_bitmap_16(base, 60, 32, frame_16, 16);
    plot_bitmap_16(base, 60, 48, diamond_16, 16);


    plot_bitmap_16(base, 100, 0, checkerboard_16, 8);
    plot_bitmap_16(base, 100, 20, smiley_16, 16);
    plot_bitmap_16(base, 100, 40, frame_16, 16);
    plot_bitmap_16(base, 110, 0, diamond_16, 16);
    plot_bitmap_16(base, 110, 20, solid_block_16, 8);
    plot_bitmap_16(base, 110, 40, checkerboard_16, 8);


    plot_bitmap_16(base, 140, 0, smiley_16, 15);


    plot_bitmap_16(base, 170, 0, diamond_16, 16);
    plot_bitmap_16(base, 170, 24, diamond_16, 16);
    plot_bitmap_16(base, 170, 48, diamond_16, 16);


    clear_screen((UINT32 *)base);
    plot_string(base, 5, 10, "Left Edge Clipping (16px):");
    plot_bitmap_16(base, 20, -8, solid_block_16, 8);
    plot_bitmap_16(base, 30, -4, checkerboard_16, 8);
    plot_bitmap_16(base, 40, 0, frame_16, 16);


    plot_string(base, 60, 10, "Right Edge Clipping (16px):");
    plot_bitmap_16(base, 80, 632, solid_block_16, 8);
    plot_bitmap_16(base, 90, 636, checkerboard_16, 8);
    plot_bitmap_16(base, 100, 624, frame_16, 16);


    plot_string(base, 120, 10, "Top Edge Clipping (16px):");
    plot_bitmap_16(base, -8, 100, solid_block_16, 8);
    plot_bitmap_16(base, -4, 120, checkerboard_16, 8);
    plot_bitmap_16(base, 0, 140, frame_16, 16);


    plot_string(base, 340, 10, "Bottom Edge Clipping (16px):");
    plot_bitmap_16(base, 392, 100, smiley_16, 16);
    plot_bitmap_16(base, 396, 120, checkerboard_16, 8);
    plot_bitmap_16(base, 384, 140, frame_16, 16);


    plot_bitmap_16(base, -30, 100, smiley_16, 16);
    plot_bitmap_16(base, 450, 100, smiley_16, 16);
    plot_bitmap_16(base, 100, -30, smiley_16, 16);
    plot_bitmap_16(base, 100, 650, smiley_16, 16);
    _trap_1_w((short)0x08) ;
}

void test_plot_bitmap_32(UINT8 *base)
{



    const UINT32 checkerboard_32[] = {
        0xAAAAAAAA,
        0x55555555,
        0xAAAAAAAA,
        0x55555555,
        0xAAAAAAAA,
        0x55555555,
        0xAAAAAAAA,
        0x55555555,
        0xAAAAAAAA,
        0x55555555,
        0xAAAAAAAA,
        0x55555555,
        0xAAAAAAAA,
        0x55555555,
        0xAAAAAAAA,
        0x55555555,
        0xAAAAAAAA,
        0x55555555,
        0xAAAAAAAA,
        0x55555555,
        0xAAAAAAAA,
        0x55555555,
        0xAAAAAAAA,
        0x55555555,
        0xAAAAAAAA,
        0x55555555,
        0xAAAAAAAA,
        0x55555555,
        0xAAAAAAAA,
        0x55555555,
        0xAAAAAAAA,
        0x55555555
    };


    const UINT32 solid_block_32[] = {
        0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
        0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF};


    const UINT32 smiley_32[] = {
        0x00FFFF00,
        0x03FFFF80,
        0x07FFFFC0,
        0x0FF00FF0,
        0x1FE00FFC,
        0x3FC003FE,
        0x7F8001FF,
        0x7F8001FF,
        0xFF0000FF,
        0xFF0000FF,
        0xFF0000FF,
        0x7F8001FF,
        0x3FC003FE,
        0x1FE00FFC,
        0x0FF00FF0,
        0x03FFFF80
    };


    const UINT32 frame_32[] = {
        0xFFFFFFFF,
        0xFFFFFFFF,
        0xFFFFFFFF,
        0xC0000003,
        0xC0000003,
        0xC0000003,
        0xC0000003,
        0xC0000003,
        0xC0000003,
        0xC0000003,
        0xC0000003,
        0xC0000003,
        0xC0000003,
        0xFFFFFFFF,
        0xFFFFFFFF,
        0xFFFFFFFF
    };


    const UINT32 arrow_32[] = {
        0xFFFFFFFF,
        0xFFFFFFFF,
        0xFFFFFFFF,
        0xFFFFFFFF,
        0xFFFFFFFF,
        0xFFFFFFFF,
        0xFFFFFFFF,
        0xFFFFFFFF,
        0xFFFFFFFF,
        0xFFFFFFFF,
        0xFFFFFFFF,
        0xFFFFFFFF,
        0xFFFFFFFF,
        0xFFFFFFFF,
        0xFFFFFFFF,
        0xFFFFFFFF,
        0xFFFFFFFF,
        0xFFFFFFFF,
        0xFFFFFFFF,
        0xFFFFFFFF,
        0xFFFFFFFF,
        0xFFFFFFFF,
        0xFFFFFFFF,
        0xFFFFFFFF,
        0xFFFFFFFF,
        0xFFFFFFFF,
        0xFFFFFFFF,
        0xFFFFFFFF,
        0xFFFFFFFF,
        0xFFFFFFFF,
        0xFFFFFFFF,
        0xFFFFFFFF,
        0xFFFFFFFF,
        0xFFFFFFFF,
        0xFFFFFFFF,
        0xFFFFFFFF,
        0xFFFFFFFF
    };


    clear_screen((UINT32 *)base);


    plot_bitmap_32(base, 10, 0, checkerboard_32, 32);


    plot_bitmap_32(base, 10, 64, solid_block_32, 8);


    plot_bitmap_32(base, 10, 100, smiley_32, 16);


    plot_bitmap_32(base, 30, 0, frame_32, 16);
    plot_bitmap_32(base, 30, 40, arrow_32, 16);
    plot_bitmap_32(base, 30, 80, checkerboard_32, 8);


    plot_bitmap_32(base, 60, 0, solid_block_32, 8);
    plot_bitmap_32(base, 60, 32, checkerboard_32, 8);
    plot_bitmap_32(base, 60, 64, frame_32, 16);
    plot_bitmap_32(base, 60, 96, arrow_32, 16);


    plot_bitmap_32(base, 100, 0, checkerboard_32, 8);
    plot_bitmap_32(base, 100, 40, smiley_32, 16);
    plot_bitmap_32(base, 100, 80, frame_32, 16);
    plot_bitmap_32(base, 110, 0, arrow_32, 16);
    plot_bitmap_32(base, 110, 40, solid_block_32, 8);
    plot_bitmap_32(base, 110, 80, checkerboard_32, 8);


    plot_bitmap_32(base, 140, 0, smiley_32, 15);


    plot_bitmap_32(base, 170, 0, arrow_32, 37);
    plot_bitmap_32(base, 170, 48, arrow_32, 37);
    plot_bitmap_32(base, 170, 96, arrow_32, 37);


    clear_screen((UINT32 *)base);
    plot_string(base, 5, 10, "Left Edge Clipping (32px):");
    plot_bitmap_32(base, 20, -16, solid_block_32, 8);
    plot_bitmap_32(base, 30, -8, checkerboard_32, 8);
    plot_bitmap_32(base, 40, 0, frame_32, 16);


    plot_string(base, 70, 10, "Right Edge Clipping (32px):");
    plot_bitmap_32(base, 90, 624, solid_block_32, 8);
    plot_bitmap_32(base, 100, 632, checkerboard_32, 8);
    plot_bitmap_32(base, 110, 608, frame_32, 16);


    plot_string(base, 140, 10, "Top Edge Clipping (32px):");
    plot_bitmap_32(base, -8, 100, solid_block_32, 8);
    plot_bitmap_32(base, -4, 140, checkerboard_32, 8);
    plot_bitmap_32(base, 0, 180, frame_32, 16);


    plot_string(base, 320, 10, "Bottom Edge Clipping (32px):");
    plot_bitmap_32(base, 384, 100, frame_32, 16);
    plot_bitmap_32(base, 392, 140, checkerboard_32, 8);
    plot_bitmap_32(base, 376, 180, smiley_32, 16);


    plot_bitmap_32(base, -50, 100, smiley_32, 16);
    plot_bitmap_32(base, 450, 100, smiley_32, 16);
    plot_bitmap_32(base, 100, -50, smiley_32, 16);
    plot_bitmap_32(base, 100, 650, smiley_32, 16);
    _trap_1_w((short)0x08) ;
}

void test_plot_character(UINT8 *base)
{
    int i;


    clear_screen((UINT32 *)base);


    plot_character(base, 10, 10, 'A');
    plot_character(base, 10, 20, 'B');
    plot_character(base, 10, 30, 'C');
    plot_character(base, 10, 40, 'D');


    plot_character(base, 30, 10, '0');
    plot_character(base, 30, 20, '1');
    plot_character(base, 30, 30, '2');
    plot_character(base, 30, 40, '3');
    plot_character(base, 30, 50, '4');


    plot_character(base, 50, 10, '!');
    plot_character(base, 50, 20, '@');
    plot_character(base, 50, 30, '#');
    plot_character(base, 50, 40, '$');
    plot_character(base, 50, 50, '%');


    for (i = 0; i < 26; i++)
    {
        plot_character(base, 80, 10 + (i * 8), 'A' + i);
    }


    for (i = 0; i < 26; i++)
    {
        plot_character(base, 100, 10 + (i * 8), 'a' + i);
    }


    for (i = 0; i < 10; i++)
    {
        plot_character(base, 120, 10 + (i * 8), '0' + i);
    }


    plot_character(base, 140, 10, '(');
    plot_character(base, 140, 20, ')');
    plot_character(base, 140, 30, '[');
    plot_character(base, 140, 40, ']');
    plot_character(base, 140, 50, '{');
    plot_character(base, 140, 60, '}');
    plot_character(base, 140, 70, '<');
    plot_character(base, 140, 80, '>');
    plot_character(base, 140, 90, '/');
    plot_character(base, 140, 100, '\\');


    for (i = 0; i < 10; i++)
    {
        plot_character(base, 170 + (i * 10), 10, '*');
    }
}

void test_plot_string(UINT8 *base)
{

    clear_screen((UINT32 *)base);


    plot_string(base, 10, 10, "Hello, World!");


    plot_string(base, 30, 10, "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
    plot_string(base, 50, 10, "abcdefghijklmnopqrstuvwxyz");


    plot_string(base, 70, 10, "0123456789");


    plot_string(base, 90, 10, "!@#$%^&*()_+-=[]{}|;:'\",.<>?/");


    plot_string(base, 110, 10, "Line 1: Testing plot_string");
    plot_string(base, 130, 10, "Line 2: Second line of text");
    plot_string(base, 150, 10, "Line 3: Third line of text");


    plot_string(base, 180, 50, "Offset text");
    plot_string(base, 200, 100, "More offset");
    plot_string(base, 220, 150, "Even more");


    plot_string(base, 250, 10, "A");
    plot_string(base, 250, 30, "Hi");
    plot_string(base, 250, 60, "OK");
    plot_string(base, 250, 90, "Yes");


    plot_string(base, 280, 10, "Atari ST Raster Library");
    plot_string(base, 300, 10, "Character Test Complete");


    plot_string(base, 330, 240, "CENTERED");


    plot_string(base, 350, 10, "");
}

void fill_screen(UINT32 *base, char pattern)
{
    register int i = 0;
    register UINT32 *loc = base;

    while (i++ < 32000  / 4)
        *(loc++) = pattern;
}
