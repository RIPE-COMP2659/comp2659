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

base            equ             64              ; offset from SP, not A6
row             equ             68               
col             equ             70
bitmap          equ             72              ; pointer to bitmap data
height          equ             76


_plot_bitmap_8: 
                movem.l         d0-d7/a0-a6,-(sp)

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
                
                ; Double bit-shift is basically same as (#8 - bit_offset). Potential for optimizations here.
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
                ; Byte-aligned - can use optimized copy
                move.w          height(sp),d7   ; get height (number of rows)
                subq.w          #1,d7           ; adjust for dbra
                
row_loop:       move.b          (a1)+,(a0)      ; copy one byte of bitmap to screen
                adda.w          #80,a0          ; move to next row (80 bytes per row)
                dbra            d7,row_loop
                
                movem.l         (sp)+,d0-d7/a0-a6
                rts
