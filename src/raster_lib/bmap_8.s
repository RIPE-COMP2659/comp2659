;
; PURPOSE: Plots a bitmap to the screen given by the top left pixel of the bitmap and the height of bitmap.
;
; INPUT: Address(UINT8*): to the start of the screen
;        Position(row,col): the coordinates of the top left pixel of the bitmap
;        bitmap(UINT8*): pointer to bitmap data (8 pixels wide, 1 byte per row)
;        Height: the length (number of rows) of the height in pixels of the bitmap
;
; OUTPUT: None
;
; void plot_bitmap_8(UINT8 *base, UINT16 row, UINT16 col, const UINT8 *bitmap, UINT16 height);
;________________________________________________________________
; Plot an 8-pixel wide bitmap.
;
;                               Optimal for byte-aligned start addresses. (col % 8 == 0)
;
;
                xdef            _plot_bitmap_8
                xref            _check_bounds
                xref            _plot_clipped_bitmap

base            equ             64
row             equ             68
col             equ             70
bitmap          equ             72
height          equ             76


_plot_bitmap_8: 
                movem.l         d0-d7/a0-a6,-(sp)

;--------------------------------------------------------------------------------------------
;                       !MIGRATE to pass-by-register for faster performance in the future!
;                       This will avoid read/writes to memory, which costs us clock cycles
;--------------------------------------------------------------------------------------------

                ; Check bounds using the _check_bounds routine.

                move.w          #8,-(sp)           ; push width (8 pixels)
                move.w          height(sp),-(sp)   ; push height
                move.w          col(sp),-(sp)      ; push col
                move.w          row(sp),-(sp)      ; push row
                jsr             _check_bounds
                adda.l             #8,sp           ; clean parameters from  stack
                
                ; Check return status
                cmpi.b          #3,d0               ; check if entirely out of bounds (nothing to draw)
                beq             done                ; if so, return immediately
                
                tst.b           d0                  ; check if any clipping needed
                bne             use_clipped         ; if status != 0 (but not 3), use clipped version
                
                ; No clipping needed, continue with normal routine
                movea.l         base(sp),a0     ; get base address (screen starting pointer)
                movea.l         bitmap(sp),a1   ; get bitmap start address
                
                ; Calculate and add row offset: row * 80 bytes
                move.w          row(sp),d0
                mulu.w          #80,d0          ; screen width in bytes
                adda.l          d0,a0           ; add row offset
                
                ; Calculate col byte offset and bit shift
                move.w          col(sp),d6      ; d6 = col (pixel column)
                move.w          d6,d5           ; copy for bit calculation
                lsr.w           #3,d6           ; d6 = col / 8 ( bit shift )
                ext.l           d6
                adda.l          d6,a0           ; add col offset in bytes
                
                andi.w          #7,d5           ; d5 = col % 8 (bit shift amount).
                                                ; Don't really understand it, but its from the labs. 
                beq             aligned_copy    ; if 0, no shifting needed ( happy )
                
                ; Unaligned copy - need to shift bits ( not happy )
                move.w          height(sp),d7   ; get height
                subq.w          #1,d7           ; adjust for dbra
                
                ; Double bit-shift is basically same as (#8 - bit_offset). Potential for optimizations here, but this might be best.
shift_loop:     moveq           #0,d0           ; clear d0
                move.b          (a1)+,d0        ; get bitmap byte into low byte
                lsl.w           #8,d0           ; shift to high byte: 0x00FF --> 0xFF00
                lsr.w           d5,d0           ; shift right by bit offset
                
                ; d0 now has: high byte = bits for current screen byte
                ;             low byte = bits for next screen byte
                
                ; Write to current byte
                move.b          d0,d1           ; get low byte (for next screen byte)
                lsr.w           #8,d0           ; get high byte into low position
                or.b            d0,(a0)         ; OR into current screen byte
                or.b            d1,1(a0)        ; OR into next screen byte
                
                adda.w          #80,a0          ; move to next row
                dbra            d7,shift_loop
                
                movem.l         (sp)+,d0-d7/a0-a6
                rts

aligned_copy:
                ; CASE: Byte-aligned - can use optimized copy
                move.w          height(sp),d7   ; get height (number of rows)
                subq.w          #1,d7           ; adjust for dbra
                
row_loop:       move.b          (a1)+,(a0)      ; copy one byte of bitmap to screen
                adda.w          #80,a0          ; move to next row (80 bytes per row)
                dbra            d7,row_loop
                
                movem.l         (sp)+,d0-d7/a0-a6
                rts

done:
                movem.l         (sp)+,d0-d7/a0-a6
                rts

use_clipped:
                ; Save status and new_width before restoring registers
                ; Create space and store them
                subq.l          #4,sp               ; Make room for 2 words
                move.w          d0,2(sp)            ; Store status
                move.w          d1,(sp)             ; Store new_width
                
                ; Restore registers from offset (skipping our saved values)
                movem.l         4(sp),d0-d7/a0-a6   ; Restore from sp+4
                
                ; Pop our saved values into d6/d7
                move.w          (sp)+,d6            ; d6 = new_width
                move.w          (sp)+,d7            ; d7 = status
                

;--------------------------------------------------------------------------------------------
;                       !MIGRATE to pass-by-register for faster performance in the future!
;                       This will avoid read/writes to memory, which costs us clock cycles
;--------------------------------------------------------------------------------------------

                ; Now push parameters for _plot_clipped_bitmap
                move.w          d6,-(sp)            ; push new_width
                move.w          d7,-(sp)            ; push status
                move.w          #8,-(sp)            ; push width (8 pixels)
                move.w          height(sp),-(sp)    ; push height
                move.l          bitmap(sp),-(sp)    ; push bitmap
                move.w          col+6(sp),-(sp)     ; push col
                move.w          row+8(sp),-(sp)     ; push row
                move.l          base+10(sp),-(sp)   ; push base
                jsr             _plot_clipped_bitmap
                adda.l          #18,sp           ; clean up (4+2+4+2+2+2+2 = 18 bytes)
                rts
