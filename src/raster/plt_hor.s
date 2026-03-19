; plt_hor.s
; Authors:
;     Riley Gramlich, rgram060@mtroyal.ca, 201762060
;     Robert Parker Hutcheson, rhutc335@mtroyal.ca, 201762335
;     Isaac Klein, iklei977@mtroyal.ca, 201763977
;     Eduard Mykhailets, emykh268@mtroyal.ca, 201750268
; Course: COMP 2659-001, Computing Machinery II, Winter 2026
; Instructor: Nolan Shaw
;
; PURPOSE: Plot a horizontal line on the screen. The horizontal line is specified by the leftmost pixel of the line and the length of the line.
;
; INPUT: Address(UINT8*): to the start of the screen
;        Position(row,col): the coordinates of the leftmost pixel of the horizontal line
;        Length: the length in pixels of the line
;        Color: color of the line (0 = black, 1 = white)
;
; OUTPUT: None
;
; void plot_horizontal_line(UINT32 *base, UINT16 row, UINT16 col, UINT16 length, UINT16 color);
;________________________________________________________________
; Draw a horizontal line of pixels
;
;                      -- This routine is unoptimized, as we shouldn't be using it ---
;                                       DO NOT USE THIS METHOD
;
; Contact @sudonym-i if you will need to use this routine.

        xdef    _plot_horizontal_line

_plot_horizontal_line:
        link    a6,#0
        movem.l d0-d3/a0,-(sp)

; Check if length is zero
        move.w  16(a6),d3                       ; length offset
        beq     hor_done                        ; if length = 0, exit

check_y_clip:
;--------------------------------------------------------------------------------------------
;               CLIPPING LOGIC (@sudonym-i's recommendation)
;               Check bounds before drawing. Modify start/end points
;               and lengths so the original loop stays unbothered.
;--------------------------------------------------------------------------------------------

; 1. Top/Bottom Clipping (Entirely out of bounds)
        move.w  12(a6),d0                       ; row offset
        tst.w   d0
        blt     hor_done                        ; If row < 0, entirely off top
        cmpi.w  #400,d0
        bge     hor_done                        ; If row >= 400, entirely off bottom

check_left_clip:
; 2. Left Clipping
        move.w  14(a6),d1                       ; col offset
        tst.w   d1
        bge     check_right                     ; If col >= 0, skip left clip

; Handle left clipping (col < 0)
        move.w  d1,d2
        add.w   d3,d2                           ; d2 = col + length
        ble     hor_done                        ; If col + length <= 0, entirely off left

        add.w   d1,d3                           ; Decrease length by the off-screen amount (length += negative col)
        clr.w   d1                              ; Set col to 0 (left edge of screen)
        bra     check_right

check_right:
        cmpi.w  #640,d1
        bge     hor_done                        ; If col >= 640, entirely off right

        move.w  d1,d2
        add.w   d3,d2                           ; d2 = col + length (right edge X)
        cmpi.w  #640,d2
        ble     clip_done                       ; If right edge <= 640, skip right clip

; Handle right clipping
        move.w  #640,d3
        sub.w   d1,d3                           ; length = 640 - col

clip_done:
; Save potentially modified parameters back to stack for original code to use
        move.w  d1,14(a6)                       ; col
        move.w  d3,16(a6)                       ; length

calc_line_offsets:
;--------------------------------------------------------------------------------------------
;               END CLIPPING LOGIC - Original Unoptimized Routine Resumes
;--------------------------------------------------------------------------------------------

        movea.l 8(a6),a0                        ; get base address

; Calculate and add row offset: row * 80 bytes
        move.w  12(a6),d0                       ; row
        mulu.w  #80,d0                          ; screen width in bytes
        adda.l  d0,a0                           ; add row offset

; Calculate and add col byte offset: col / 8
        move.w  14(a6),d1                       ; col
        move.w  d1,d2                           ; save col for bit calculation
        lsr.w   #3,d1                           ; d1 = col / 8 (byte offset)
        adda.w  d1,a0                           ; a0 now points to the byte

prepare_pixel_loop:
; Calculate bit position: 7 - (col % 8)
        andi.w  #7,d2                           ; d2 = col % 8
        moveq   #7,d0
        sub.w   d2,d0                           ; d0 = 7 - (col % 8) = bit position

; Check color parameter
        move.w  18(a6),d1                       ; color
        tst.w   d1                              ; check if color is 0
        beq     pixel_loop_black

pixel_loop_white:
; Set the bit (white)
        bset    d0,(a0)                         ; set bit d0 in byte at (a0)

; Decrement length counter
        subq.w  #1,d3
        beq     hor_done                        ; if counter = 0, we're done

; Move to next pixel (decrement bit position)
        subq.w  #1,d0
        bge     pixel_loop_white                ; if d0 >= 0, stay in same byte

; Crossed byte boundary - move to next byte
        adda.w  #1,a0                           ; safely advance to next byte!
        moveq   #7,d0                           ; reset bit position to 7
        bra     pixel_loop_white

pixel_loop_black:
; Clear the bit (black)
        bclr    d0,(a0)                         ; clear bit d0 in byte at (a0)

; Decrement length counter
        subq.w  #1,d3
        beq     hor_done                        ; if counter = 0, we're done

; Move to next pixel (decrement bit position)
        subq.w  #1,d0
        bge     pixel_loop_black                ; if d0 >= 0, stay in same byte

; Crossed byte boundary - move to next byte
        adda.w  #1,a0                           ; safely advance to next byte!
        moveq   #7,d0                           ; reset bit position to 7
        bra     pixel_loop_black

hor_done:
        movem.l (sp)+,d0-d3/a0
        unlk    a6
        rts
