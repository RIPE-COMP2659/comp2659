;
;   Helper function to check whether we are within the bounds of the screen buffer.
;
; Return cases (in the `char` byte return value):
;   0 -> Does not exceed any bounds
;   1 -> exceeds bounds but isnt entirely out of bounds LEFT EDGE OF SCREEN
;   2 -> exceeds bounds but isnt entirely out of bounds RIGHT EDGE OF SCREEN 
;   3 -> entirely out of bounds ( Does not need to be drawn at all )
;
; UINT32, the new width of the clipped bitmap (NOT always used)
; 
;
; char, INT32 check_bounds(int row, int col, int height, int width) 
;
; // `char` is a 1 byte integer. This is so that calling scope can do switch cases (returns 2 values because its assembly, and I can do that)
;________________________________________________________________

;--------------------------------------------------------------------------------------------
;                       !MIGRATE to pass-by-register for faster performance in the future!
;                       This will avoid read/writes to memory, which costs us clock cycles
;--------------------------------------------------------------------------------------------




        xdef    _check_bounds

row     equ     8
col     equ     10
height  equ     12
width   equ     14

_check_bounds:
        link    a6,#0
        movem.l d2-d7/a0-a5,-(sp)               ; save registers (keep d0,d1 for return, preserve a6!)
        move.w  row(a6),d0
        move.w  col(a6),d1
        move.w  height(a6),d2
        move.w  width(a6),d3

; Check for right edge
        cmp.w   #400,d0                         ; compare with SCREEN_HEIGHT
        bge     entirely_out                    ; if row + height > 400, entirely out
; Check for bottom edge (row + height <= 0)
        move.w  d0,d4                           ; d4 = row
        add.w   d2,d4                           ; d4 = row + height
        cmp.w   0,d4
        ble.w   entirely_out

; Check for right edge
        cmp.w   #640,d1                         ; compare with SCREEN_WIDTH
        bge     entirely_out                    ; if col >= 640, entirely out
; Check for left edge
        move.w  d1,d4                           ; d4 = col
        add.w   d3,d4                           ; d4 = col + width
        cmp.w   0,d4                            ; compare with 0
        ble.w   entirely_out                    ; if col + width <= 0, entirely out     

; check if col + width > screen width (640)
        move.w  d1,d4                           ; d4 = col
        add.w   d3,d4                           ; d4 = col + width
        cmp.w   #640,d4                         ; compare with SCREEN_WIDTH
        bgt     right_edge                      ; if col + width > 640, right edge

; check if col < 0 (left edge)
        tst.w   d1                              ; test if col is negative
        bmi     left_edge                       ; Tests for N bit

; if we get here, within bounds
        moveq   #0,d0                           ; status = 0 (within bounds)
        move.w  d3,d1                           ; return width unchanged
        ext.l   d1                              ; extend to long
        bra     done

left_edge:
; col < 0, need to calculate new width
; new_width = width + col (since col is negative)
; We know col + width > 0 (checked earlier), so new_width will be positive
        move.w  d3,d4                           ; d4 = width
        add.w   d1,d4                           ; d4 = width + col = new width. NOTE, COL is negative in this case
        moveq   #1,d0                           ; status = 1 (left edge)
        move.w  d4,d1                           ; return new width
        ext.l   d1                              ; extend to long. Avoid junk.
        bra     done

right_edge:
; col + width > 640, need to calculate new width
; Must ensure we don't write past byte 79 (last byte in 80-byte row)
; When bitmaps are not byte-aligned, they can span multiple bytes!
; We know col < 640 (checked earlier), so 640 - col will be positive
        move.w  #640,d4                         ; d4 = 640
        sub.w   d1,d4                           ; d4 = 640 - col (pixel-based limit)
        moveq   #2,d0                           ; status = 2 (right edge)
        move.w  d4,d1                           ; return new width = 640 - col
        ext.l   d1                              ; extend to long. Avoid junk.
        bra     done


entirely_out:
        moveq   #3,d0                           ; status = 3 (entirely out)
        moveq   #0,d1                           ; return 0 for width

done:
        movem.l (sp)+,d2-d7/a0-a5               ; restore registers (d0,d1 have return values)
        unlk    a6
        rts
