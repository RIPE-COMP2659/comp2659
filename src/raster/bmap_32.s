
;
; PURPOSE: Plots a bitmap to the screen given by the top left pixel of the bitmap and the height of bitmap.
;
; INPUT: Address(UINT8*): to the start of the screen
;        Position(row,col): the coordinates of the top left pixel of the bitmap
;        bitmap(UINT32*): pointer to bitmap data (32 pixels wide, 4 bytes per row)
;        Height: the length (number of rows) of the height in pixels of the bitmap
;
; OUTPUT: None
;
; void plot_bitmap_32(UINT8 *base, UINT16 row, UINT16 col, const UINT32 *bitmap, UINT16 height);
;________________________________________________________________
; Plot a 32-pixel wide bitmap.
;
; Bitmap format: 4 bytes (1 long) per row, 32 pixels wide (1 bit per pixel)

                xdef            _plot_bitmap_32

base            equ             64              ; offset from SP, not A6
row             equ             68               
col             equ             70
bitmap          equ             72              ; pointer to bitmap data
height          equ             76


_plot_bitmap_32: movem.l        d0-d7/a0-a6,-(sp)

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
                
                ; Check long word alignment for optimization
                move.l          a0,d1
                btst            #0,d1           ; test if screen address is odd
                bne             long_copy       ; if odd, use long copy
                btst            #1,d1           ; test if screen address is word-aligned but not long-aligned
                bne             long_copy       ; if not long-aligned, use long copy
                
                move.l          a1,d1
                btst            #0,d1           ; test if bitmap address is odd
                bne             long_copy       ; if odd, use long copy
                btst            #1,d1           ; test if bitmap address is word-aligned but not long-aligned
                bne             long_copy       ; if not long-aligned, use long copy
                
                ; Check if we have even number of rows for movem optimization
                move.w          height(sp),d7
                btst            #0,d7           ; test if height is odd
                bne             long_copy       ; if odd, use long copy
                
                ; Optimized movem copy (2 rows at a time)
                lsr.w           #1,d7           ; divide height by 2
                subq.w          #1,d7           ; adjust for dbra
                
movem_loop:     movem.l         (a1)+,d0-d1     ; get 2 longs (8 bytes) of bitmap data
                move.l          d0,(a0)         ; plot first row
                move.l          d1,80(a0)       ; plot second row
                adda.w          #160,a0         ; move to row+2 (2 rows * 80 bytes)
                dbra            d7,movem_loop
                
                movem.l         (sp)+,d0-d7/a0-a6
                rts

long_copy:
                ; Check if screen address is word-aligned (even)
                move.l          a0,d1
                btst            #0,d1           ; test if screen address is odd
                bne             byte_copy       ; if odd, must use byte copy
                
                ; Check if bitmap address is word-aligned
                move.l          a1,d1
                btst            #0,d1           ; test if bitmap address is odd
                bne             byte_copy       ; if odd, must use byte copy
                
                ; Standard long-by-long copy
                move.w          height(sp),d7   ; get height (number of rows)
                subq.w          #1,d7           ; adjust for dbra
                
row_loop:       move.l          (a1)+,(a0)      ; copy one long of bitmap to screen
                adda.w          #80,a0          ; move to next row (80 bytes per row)
                dbra            d7,row_loop
                
                movem.l         (sp)+,d0-d7/a0-a6
                rts

byte_copy:
                ; Byte-by-byte copy for misaligned addresses
                move.w          height(sp),d7   ; get height (number of rows)
                subq.w          #1,d7           ; adjust for dbra
                
byte_loop:      move.b          (a1)+,(a0)+     ; copy byte 1
                move.b          (a1)+,(a0)+     ; copy byte 2
                move.b          (a1)+,(a0)+     ; copy byte 3
                move.b          (a1)+,(a0)      ; copy byte 4
                adda.w          #77,a0          ; move to next row (80 - 3 already advanced)
                dbra            d7,byte_loop
                
                movem.l         (sp)+,d0-d7/a0-a6
                rts

