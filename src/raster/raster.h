#ifndef RASTER_H
#define RASTER_H

/*----- Function: clear_screen -----

PURPOSE: Clears the entire screen.

 INPUT: Address(UINT32*): to the start of the screen

 OUTPUT: None

*/
void clear_screen(UINT32 *base);

/*----- Function: clear_region -----

 PURPOSE: Clear a region of the screen. The section is specified by the coordinates of the top left corner, and the height and width of the region.

 INPUT: Address(UINT32*): to the start of the screen
        Position(row,col): the coordinates of the top left pixel of the region
        Length: the lenth (number of rows) in pixels of the region
        Width: the width (number of columns) in pixels of the region

 OUTPUT: None

*/
void clear_region(UINT32 *base, UINT16 row, UINT16 col, UINT16 length, UINT16 width);

/*----- Function: plot_pixel -----

 PURPOSE: Plots a single pixel on the screen.

 INPUT: Address(UINT32*): to the start of the screen
        Position(row,col): the location of the pixel to plot

 OUTPUT: None

*/
void plot_pixel(UINT8 *base, UINT16 row, UINT16 col);

/*----- Function: plot_horizontal_line -----

 PURPOSE: Plot a hoizontal line on the screen. The horizontal line is specified by the leftmost pixel of the line and the length of the line.

 INPUT: Address(UINT8*): to the start of the screen
        Position(row,col): the coordinates of the leftmost pixel of the horizontal line
        Length: the lenth in pixels of the line

 OUTPUT: None
*/
void plot_horizontal_line(UINT32 *base, UINT16 row, UINT16 col, UINT16 length);

/*----- Function: plot_vertical_line -----

 PURPOSE: Plot a hoizontal line on the screen. The vertical line is specified by the topmost pixel of the line and the length of the line.

 INPUT: Address(UINT32*): to the start of the screen
        Position(row,col): the coordinates of the topmost pixel of the vertical line
        Length: the lenth in pixels of the line

 OUTPUT: None
*/
void plot_vertical_line(UINT32 *base, UINT16 row, UINT16 col, UINT16 length);

/*----- Function: plot_line -----

 PURPOSE: Plots a line on the screen between the two given points.

 INPUT: Address(UINT32*): to the start of the screen
        Position(start_row,start_col): the coordinates of the start of the line
        Position(end_row,end_col): the coordinates of the end of the line

 OUTPUT: None
*/
void plot_line(UINT32 *base, UINT16 start_row, UINT16 start_col, UINT16 end_row, UINT16 end_col);

/*----- Function: plot_rectangle -----

 PURPOSE: Plots a rectangle on the screen given by the top left pixel, and the length and width of the rectangle.

 INPUT: Address(UINT32*): to the start of the screen
        Position(row,col): the coordinates of the top left pixel of the rectangle
        Length: the lenth (number of rows) in pixels of the rectangle
        Width: the width (number of columns) in pixels of the rectangle

 OUTPUT: None
*/
void plot_rectangle(UINT32 *base, UINT16 row, UINT16 col, UINT16 length, UINT16 width);

/*----- Function: plot_square -----

 PURPOSE: Plots a square on the screen given by the top left pixel, and the length of the sides of the square.

 INPUT: Address(UINT32*): to the start of the screen
        Position(row,col): the coordinates of the top left pixel of the square
        Side: the lenth of each side, in pixels, of the square

 OUTPUT: None
*/
void plot_square(UINT32 *base, UINT16 row, UINT16 col, UINT16 side);

/*----- Function: plot_triangle -----

 PURPOSE: Plots a triangle on the screen given by the coordinate of the 90° angle, the length of the base, the length of the height, and the direction of the triangle.

 INPUT: Address(UINT8*): to the start of the screen
        Position(row,col): the coordinates of the pixel of the 90° angle of the triangle
        Height: the lenth (number of rows) of the height in pixels of the triangle
        Direction: Describes where the coordinate is relative to the rest of the triangle
              0 - Coordinate is the top left point of the triangle
              1 - Coordinate is the top right point of the triangle
              2 - Coordinate is the bottom left point of the triangle
              3 - Coordinate is the bottom right point of the triangle


 OUTPUT: None
*/
void plot_triangle(UINT32 *base, UINT16 row, UINT16 col, UINT16 base, UINT16 height, UINT8 direction);

/*----- Function: plot_bitmap_8 -----

 PURPOSE: Plots a bitmap to the screen given by the top left pixel of the bitmap and the height of bitmap.

 INPUT: Address(UINT16*): to the start of the screen
        Position(row,col): the coordinates of the top left pixel of the bitmap
        Height: the lenth (number of rows) of the height in pixels of the bitmap

 OUTPUT: None
*/
void plot_bitmap_8(UINT8 *base, UINT16 row, UINT16 col, UINT16 height);

/*----- Function: plot_bitmap_16 -----

 PURPOSE: Plots a bitmap to the screen given by the top left pixel of the bitmap and the height of bitmap.

 INPUT: Address(UINT16*): to the start of the screen
        Position(row,col): the coordinates of the top left pixel of the bitmap
        Height: the lenth (number of rows) of the height in pixels of the bitmap

 OUTPUT: None
*/
void plot_bitmap_16(UINT16 *base, UINT16 row, UINT16 col, UINT16 height);

/*----- Function: plot_bitmap_32 -----

 PURPOSE: Plots a bitmap to the screen given by the top left pixel of the bitmap and the height of bitmap.

 INPUT: Address(UINT32*): to the start of the screen
        Position(row,col): the coordinates of the top left pixel of the bitmap
        Height: the lenth (number of rows) of the height in pixels of the bitmap

 OUTPUT: None
*/
void plot_bitmap_32(UINT32 *base, UINT16 row, UINT16 col, UINT16 height);

/*----- Function: plot_character -----

 PURPOSE: Plots a single character, as a bitmap from a font table, to the screen.

 INPUT: Address(UINT32*): to the start of the screen
        Position(row,col): the coordinates of the top left pixel of the character
        ch(char): the character to be written to the screen

 OUTPUT: None
*/
void plot_character(UINT8 *base, UINT16 row, UINT16 col, char ch);

/*----- Function: plot_string -----

 PURPOSE: Plots a string, as a sequence of bitmaps from a font table, to the screen.

 INPUT: Address(UINT32*): to the start of the screen
        Position(row,col): the coordinates of the top left pixel of the string
        ch(c-string): the string to be written to the screen

 OUTPUT: None
*/
void plot_string(UINT8 *base, UINT16 row, UINT16 col, char *ch);

#endif
