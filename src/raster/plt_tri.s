;
; PURPOSE: Plots a triangle on the screen given by the coordinate of the 90° angle,
;          the length of the base, the length of the height, and the direction of the triangle.
;
; INPUT: Address(UINT32*): to the start of the screen
;        Position(row,col): the coordinates of the pixel of the 90° angle of the triangle
;        base_triangle: the length of the base in pixels
;        Height: the lenth (number of rows) of the height in pixels of the triangle
;        Direction: Describes where the coordinate is relative to the rest of the triangle
;              0 - Coordinate is the top left point of the triangle
;              1 - Coordinate is the top right point of the triangle
;              2 - Coordinate is the bottom left point of the triangle
;              3 - Coordinate is the bottom right point of the triangle
;
; OUTPUT: None
;
; void plot_triangle(UINT32 *base, UINT16 row, UINT16 col, UINT16 base_triangle, UINT16 height, UINT8 direction);
;________________________________________________________________
; Draws a filled right-angled triangle by drawing horizontal lines row by row.
; Uses plot_horizontal_line for each scanline.
; plt_tri.s
; Authors:
;     Riley Gramlich, rgram060@mtroyal.ca, 201762060
;     Robert Parker Hutcheson, rhutc335@mtroyal.ca, 201762335
;     Isaac Klein, iklei977@mtroyal.ca, 201763977
;     Eduard Mykhailets, emykh268@mtroyal.ca, 201750268
; Course: COMP 2659-001, Computing Machinery II, Winter 2026
; Instructor: Nolan Shaw
;
; PURPOSE: Plots a triangle on the screen given by the coordinate of the 90° angle, the length of the base, the length of the height, and the direction of the triangle.
;
        xdef    _plot_triangle

base    equ     8                               ; UINT32* (4 bytes)
row     equ     12                              ; UINT16 (2 bytes)
col     equ     14                              ; UINT16 (2 bytes)
base_tri equ    16                              ; UINT16 (2 bytes)
height  equ     18                              ; UINT16 (2 bytes)
direction equ   20                              ; UINT8 (1 byte, but padded to 2)

_plot_triangle:
        link    a6,#0
        movem.l d0-d7/a0-a1,-(sp)
                
        ; Check height and direction
        move.w  height(a6),d7                   ; d7 = total height
        beq     done                            ; if height = 0, exit
        
        move.w  direction(a6),d0
        andi.w  #3,d0                           ; mask to 4 valid directions (0,1,2,3)
        
        cmpi.w  #0,d0
        beq     dir_top_left
        cmpi.w  #1,d0
        beq     dir_top_right
        cmpi.w  #2,d0
        beq     dir_bottom_left
        bra     dir_bottom_right

dir_top_left:
        ; Direction 0: 90° angle at top-left
        ; Triangle expands rightward and downward
        ; Row i has line of length: (base * (height - i)) / height
                
        moveq   #0,d6                           ; current row offset (0 to height-1)
        subq.w  #1,d7                           ; adjust for dbra
                
tl_loop:
        bsr     calc_length                     ; d0 = calculated line length
        
        move.w  row(a6),d1
        add.w   d6,d1                           ; current_row = row + d6
                
        ; Call plot_horizontal_line(base, current_row, col, length)
        move.w  d0,-(sp)                        ; push length
        move.w  col(a6),-(sp)                   ; push col
        move.w  d1,-(sp)                        ; push current_row
        move.l  base(a6),-(sp)                  ; push base
        jsr     _plot_horizontal_line
        adda.w  #10,sp                          ; clean up stack (4+2+2+2)
                
        addq.w  #1,d6                           ; increment row offset
        dbra    d7,tl_loop
        bra     done

dir_top_right:
        ; Direction 1: 90° angle at top-right
        ; Triangle expands leftward and downward
                
        moveq   #0,d6                           ; current row offset
        subq.w  #1,d7                           ; adjust for dbra
                
tr_loop:
        bsr     calc_length                     ; d0 = calculated line length
        
        move.w  row(a6),d1
        add.w   d6,d1                           ; current_row = row + d6
        
        move.w  col(a6),d2
        sub.w   d0,d2                           ; start_col = col - length
                
        ; Call plot_horizontal_line(base, current_row, start_col, length)
        move.w  d0,-(sp)
        move.w  d2,-(sp)
        move.w  d1,-(sp)
        move.l  base(a6),-(sp)
        jsr     _plot_horizontal_line
        adda.w  #10,sp
                
        addq.w  #1,d6
        dbra    d7,tr_loop
        bra     done

dir_bottom_left:
        ; Direction 2: 90° angle at bottom-left
        ; Triangle expands rightward and upward
                
        moveq   #0,d6                           ; current row offset
        subq.w  #1,d7                           ; adjust for dbra
                
bl_loop:
        bsr     calc_length                     ; d0 = calculated line length
        
        move.w  row(a6),d1
        sub.w   d6,d1                           ; current_row = row - d6 (going upward)
                
        ; Call plot_horizontal_line(base, current_row, col, length)
        move.w  d0,-(sp)
        move.w  col(a6),-(sp)
        move.w  d1,-(sp)
        move.l  base(a6),-(sp)
        jsr     _plot_horizontal_line
        adda.w  #10,sp
                
        addq.w  #1,d6
        dbra    d7,bl_loop
        bra     done

dir_bottom_right:
        ; Direction 3: 90° angle at bottom-right
        ; Triangle expands leftward and upward
                
        moveq   #0,d6                           ; current row offset
        subq.w  #1,d7                           ; adjust for dbra
                
br_loop:
        bsr     calc_length                     ; d0 = calculated line length
        
        move.w  row(a6),d1
        sub.w   d6,d1                           ; current_row = row - d6 (going upward)
        
        move.w  col(a6),d2
        sub.w   d0,d2                           ; start_col = col - length
                
        ; Call plot_horizontal_line(base, current_row, start_col, length)
        move.w  d0,-(sp)
        move.w  d2,-(sp)
        move.w  d1,-(sp)
        move.l  base(a6),-(sp)
        jsr     _plot_horizontal_line
        adda.w  #10,sp
                
        addq.w  #1,d6
        dbra    d7,br_loop
        bra     done


; ---------------------------------------------------------------
; Helper Routine: calc_length
; Calculates the length of the horizontal line for the current row
; Equation: d0 = (base_tri * (height - d6)) / height
; ---------------------------------------------------------------
calc_length:
        move.w  height(a6),d0
        sub.w   d6,d0                           ; d0 = remaining height
        mulu.w  base_tri(a6),d0                 ; d0 = base_tri * remaining height
        divu.w  height(a6),d0                   ; d0 = (base_tri * remaining height) / total height
        rts

done:
        movem.l (sp)+,d0-d7/a0-a1
        unlk    a6
        rts

        
