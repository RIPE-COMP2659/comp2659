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

    xdef           _check_bounds

row             equ             56
col             equ             60
height          equ             64
width           equ             68

_check_bounds:
                movem.l         d2-d7/a0-a6,-(sp)      ; save registers (keep d0,d1 for return)
                move.w          row(sp),d0
                move.w          col(sp),d1
                move.w          height(sp),d2
                move.w          width(sp),d3

; check if either row or col is negative
                tst.w           d0              ; test row
                blt             entirely_out    ; if row < 0, entirely out
                tst.w           d1              ; test col
                blt             left_edge       ; if col < 0, left edge

; check if row + height > screen height (400)
                move.w          d0,d4           ; d4 = row
                add.w           d2,d4           ; d4 = row + height
                cmp.w           #400,d4         ; compare with SCREEN_HEIGHT
                bgt             entirely_out    ; if row + height > 400, entirely out

; check if col + width > screen width (640)
                move.w          d1,d4           ; d4 = col
                add.w           d3,d4           ; d4 = col + width
                cmp.w           #640,d4         ; compare with SCREEN_WIDTH
                bgt             right_edge      ; if col + width > 640, right edge

; if we get here, within bounds
                moveq           #0,d0           ; status = 0 (within bounds)
                move.w          d3,d1           ; return width unchanged
                ext.l           d1              ; extend to long
                bra             done

left_edge:
; col < 0, need to calculate new width
; new_width = width + col (since col is negative)
                move.w          d3,d4           ; d4 = width
                add.w           d1,d4           ; d4 = width + col
                tst.w           d4              ; check if new_width > 0
                ble             entirely_out    ; if new_width <= 0, entirely out
                moveq           #1,d0           ; status = 1 (left edge)
                move.w          d4,d1           ; new width
                ext.l           d1              ; extend to long
                bra             done

right_edge:
; col + width > 640, need to calculate new width
; Must ensure we don't write past byte 79 (last byte in 80-byte row)
; When bitmaps are not byte-aligned, they can span multiple bytes!
                move.w          #640,d4         ; d4 = 640
                sub.w           d1,d4           ; d4 = 640 - col (pixel-based limit)
                tst.w           d4              ; check if new_width > 0
                ble             entirely_out    ; if new_width <= 0, entirely out
                
                ; Calculate bytes that will be touched on screen
                ; bytes_touched = ((col % 8) + width + 7) / 8
                ; We need: starting_byte + bytes_touched <= 80
                
                move.w          d1,d5           ; d5 = col
                move.w          d5,d6           ; d6 = col (copy)
                andi.w          #7,d5           ; d5 = col % 8 (bit offset)
                lsr.w           #3,d6           ; d6 = col / 8 (starting byte)
                
                ; Calculate max width that keeps starting_byte + bytes_touched <= 80
                ; bytes_touched <= 80 - starting_byte
                ; ((bit_offset + width + 7) / 8) <= max_bytes
                ; bit_offset + width + 7 <= max_bytes * 8 + 7
                ; width <= (max_bytes * 8) - bit_offset
                
                move.w          #80,d7          ; d7 = 80
                sub.w           d6,d7           ; d7 = 80 - starting_byte (max bytes we can touch)
                bne             calc_byte_limit ; if > 0, calculate limit
                bra             entirely_out    ; if 0, can't write anything
                
calc_byte_limit:
                lsl.w           #3,d7           ; d7 = max_bytes * 8
                sub.w           d5,d7           ; d7 = (max_bytes * 8) - bit_offset (byte-based width limit)
                
                ; Use minimum of pixel-based limit and byte-based limit
                cmp.w           d7,d4           ; compare pixel-based vs byte-based limit
                ble             use_pixel_limit ; if pixel limit <= byte limit, use it
                move.w          d7,d4           ; else use byte limit
                
use_pixel_limit:
                tst.w           d4              ; final check if width > 0
                ble             entirely_out    ; if width <= 0, entirely out
                moveq           #2,d0           ; status = 2 (right edge)
                move.w          d4,d1           ; new width (safe for both pixel and byte boundaries)
                ext.l           d1              ; extend to long
                bra             done

entirely_out:
                moveq           #3,d0           ; status = 3 (entirely out)
                moveq           #0,d1           ; return 0 for width

done:
                movem.l         (sp)+,d2-d7/a0-a6      ; restore registers (d0,d1 have return values)
                rts
