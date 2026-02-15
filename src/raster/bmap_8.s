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
; Bitmap format: 1 byte per row, 8 pixels wide (1 bit per pixel)

                xdef            _plot_bitmap_8

base            equ             64              ; offset from SP, not A6
row             equ             68               
col             equ             70
bitmap          equ             72              ; pointer to bitmap data
height          equ             76


_plot_bitmap_8: movem.l         d0-d7/a0-a6,-(sp)

                movea.l         base(sp),a0     ; get base address (screen)
                movea.l         bitmap(sp),a1   ; get bitmap data address
                
                ; Calculate and add row offset: row * 80 bytes
                move.w          row(sp),d0
                mulu.w          #80,d0          ; screen width in bytes
                adda.l          d0,a0           ; add row offset
                
                ; Calculate and add col offset: col / 8 (pixels to bytes)
                move.w          col(sp),d0
                lsr.w           #3,d0           ; divide by 8 (shift right 3 bits)
                ext.l           d0              ; extend to long
                adda.l          d0,a0           ; add col offset in bytes
                
                ; For word optimization, only need bitmap to be word-aligned
                ; (Screen address doesn't matter - we're writing bytes)
                move.l          a1,d1
                btst            #0,d1           ; test if bitmap address is odd
                bne             byte_copy       ; if odd, use byte copy
                
                ; Check if we have even number of rows for word optimization
                move.w          height(sp),d7
                btst            #0,d7           ; test if height is odd
                bne             byte_copy       ; if odd, use byte copy
                
                ; Optimized word copy (2 rows at a time)
                lsr.w           #1,d7           ; divide height by 2
                subq.w          #1,d7           ; adjust for dbra
                
word_loop:      move.w          (a1)+,d0        ; get 2 bytes: d0 = [row0][row1]
                move.w          d0,d1           ; copy to d1
                lsr.w           #8,d1           ; shift right 8: d1 = [00][row0]
                move.b          d1,(a0)         ; write row0 byte to current row
                move.b          d0,80(a0)       ; write row1 byte (low byte) to next row
                adda.w          #160,a0         ; move to row+2 (2 rows * 80 bytes)
                dbra            d7,word_loop
                
                movem.l         (sp)+,d0-d7/a0-a6
                rts

byte_copy:      
                ; Standard byte-by-byte copy
                move.w          height(sp),d7   ; get height (number of rows)
                subq.w          #1,d7           ; adjust for dbra
                
row_loop:       move.b          (a1)+,(a0)      ; copy one byte of bitmap to screen
                adda.w          #80,a0          ; move to next row (80 bytes per row)
                dbra            d7,row_loop
                
                movem.l         (sp)+,d0-d7/a0-a6
                rts
