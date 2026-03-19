; plt_rec.s
; Authors:
;     Riley Gramlich, rgram060@mtroyal.ca, 201762060
;     Robert Parker Hutcheson, rhutc335@mtroyal.ca, 201762335
;     Isaac Klein, iklei977@mtroyal.ca, 201763977
;     Eduard Mykhailets, emykh268@mtroyal.ca, 201750268
; Course: COMP 2659-001, Computing Machinery II, Winter 2026
; Instructor: Nolan Shaw
;
; PURPOSE: Plots a rectangle on the screen given by the top left pixel, and the length and width of the rectangle.
;
; INPUT: Address(UINT32*): to the start of the screen
;        Position(row,col): the coordinates of the top left pixel of the rectangle
;        Length: the length (number of rows) in pixels of the rectangle
;        Width: the width (number of columns) in pixels of the rectangle
;
; OUTPUT: None
;
; void plot_rectangle(UINT32 *base, UINT16 row, UINT16 col, UINT16 length, UINT16 width);
;________________________________________________________________
; Draw a rectangular region on the screen.
;
;                     -- Optimized for 48 bit widths ONLY (6 bytes wide). --
;                     *Based off our old plan to make sprites 48 pixels wide
;
; Contact @sudonym-i if you will need to use this routine in any other ways

        xdef    _plot_rectangle

_plot_rectangle: 
        link    a6,#0
        movem.l d0-d7/a0-a6,-(sp)

; Check if length is zero
        move.w  16(a6),d3                       ; length
        beq     rec_done
    
; Check if width is zero
        move.w  18(a6),d4                       ; width
        beq     rec_done

;--------------------------------------------------------------------------------------------
;               CLIPPING LOGIC
;               Check bounds before drawing. Modify start/end points 
;               and dimensions so the original loop stays unbothered.
;--------------------------------------------------------------------------------------------

check_top_clip:
; 1. Top Clipping
        move.w  12(a6),d0                       ; row
        tst.w   d0
        bge     check_bottom                    ; If row >= 0, skip top clip

        move.w  d0,d2
        add.w   16(a6),d2                       ; d2 = row + length
        ble     rec_done                        ; If entirely off top, exit

        add.w   d0,16(a6)                       ; length += negative row (decrease length)
        clr.w   12(a6)                          ; row = 0
        bra     check_bottom

check_bottom:
        cmpi.w  #400,d0
        bge     rec_done                        ; If row >= 400, entirely off bottom

        move.w  d0,d2
        add.w   16(a6),d2                       ; d2 = row + length (bottom edge Y)
        cmpi.w  #400,d2
        ble     check_left                      ; If bottom edge <= 400, skip bottom clip

        move.w  #400,d2
        sub.w   d0,d2
        move.w  d2,16(a6)                       ; length = 400 - row

check_left:
        move.w  14(a6),d1                       ; col
        tst.w   d1
        bge     check_right                     ; If col >= 0, skip left clip

        move.w  d1,d2
        add.w   18(a6),d2                       ; d2 = col + width
        ble     rec_done                        ; If entirely off left, exit

        add.w   d1,18(a6)                       ; width += negative col (decrease width)
        clr.w   14(a6)                          ; col = 0
        bra     check_right

check_right:
        cmpi.w  #640,d1
        bge     rec_done                        ; If col >= 640, entirely off right

        move.w  d1,d2
        add.w   18(a6),d2                       ; d2 = col + width (right edge X)
        cmpi.w  #640,d2
        ble     clip_done                       ; If right edge <= 640, skip right clip

        move.w  #640,d2
        sub.w   d1,d2
        move.w  d2,18(a6)                       ; width = 640 - col

clip_done:

calc_rect_offsets:
        movea.l 8(a6),a0                        ; get base address
        
; Calculate and add row offset: row * 80 bytes
        move.w  12(a6),d0                       ; row
        mulu.w  #80,d0                          ; screen width in bytes (result is long)
        adda.l  d0,a0                           ; add row offset
        
; Calculate and add col offset: col / 8 (pixels to bytes)
        move.w  14(a6),d0                       ; col
        lsr.w   #3,d0                           ; divide by 8 (shift right 3 bits)
        ext.l   d0                              ; extend to long
        adda.l  d0,a0                           ; add col offset in bytes

check_rec_alignment:
; Check if width == 48 pixels (6 bytes) for optimization
        move.w  18(a6),d0                       ; width
        cmpi.w  #48,d0                          ; check if exactly 48 pixels
        bne     unoptimized
        
; Ensure word alignment for movem.w (address must be even)
        move.l  a0,d1
        btst    #0,d1                           ; test if address is odd
        bne     unoptimized                     ; if odd, use unoptimized version
        bra     opt_48x48

opt_48x48:  
; Optimized for 48-width sprites (6 bytes = 3 words wide)
; Uses movem.w to draw 3 words at once
        
        move.w  #$ffff,d1                       ; set registers to all 1s
        move.w  #$ffff,d2
        move.w  #$ffff,d3
        
        move.w  16(a6),d7                       ; length (height)
        subq.w  #1,d7                           ; adjust for dbra
        
row_loop_48: 
        movem.w d1-d3,(a0)                      ; write 3 words (6 bytes)
        adda.w  #80,a0                          ; move to next row (80 bytes per row)
        dbra    d7,row_loop_48
        bra     rec_done
        
unoptimized:    
; Generic plot rectangle - calculate bytes needed for col through col+width-1
; Formula: ceiling((col+width)/8) - floor(col/8)
        
        move.w  14(a6),d6                       ; get col in pixels
        add.w   18(a6),d6                       ; d6 = col + width
        addq.w  #7,d6                           ; d6 = col + width + 7 (for ceiling division)
        lsr.w   #3,d6                           ; d6 = (col + width + 7) / 8 = ceiling((col+width)/8)
        
        move.w  14(a6),d5                       ; get col in pixels  
        lsr.w   #3,d5                           ; d5 = col / 8 = floor(col/8)
        
        sub.w   d5,d6                           ; d6 = ceiling - floor = num_bytes
        subq.w  #1,d6                           ; adjust for dbra
        
prep_unoptimized_rec:
        move.w  16(a6),d7                       ; length (height)
        subq.w  #1,d7                           ; adjust for dbra
        
row_loop: 
        movea.l a0,a1                           ; save row start position
        move.w  d6,d5                           ; restore column counter
        
col_loop: 
        move.b  #$ff,(a1)+                      ; set one byte (all pixels), advance
        dbra    d5,col_loop
        
        adda.w  #80,a0                          ; move to next row
        dbra    d7,row_loop
        
rec_done:
        movem.l (sp)+,d0-d7/a0-a6
        unlk    a6
        rts

	
