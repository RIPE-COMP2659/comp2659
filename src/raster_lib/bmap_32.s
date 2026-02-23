
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
; Optimized for:
;   - Bitmap address is long-word aligned (divisible by 4) -- dont worry if this isnt achievable
;   - Height is even
;

                xdef            _plot_bitmap_32
                xref            _check_bounds
                xref            _plot_clipped_bitmap

base            equ             64             
row             equ             68               
col             equ             70
bitmap          equ             72              ; pointer to bitmap data
height          equ             76


_plot_bitmap_32: 
                movem.l        d0-d7/a0-a6,-(sp)
;--------------------------------------------------------------------------------------------
;                       !MIGRATE to pass-by-register for faster performance in the future!
;                       This will avoid read/writes to memory, which costs us clock cycles
;--------------------------------------------------------------------------------------------

                ; Check bounds first
                move.w          #32,-(sp)           ; push width (32 pixels)
                move.w          height+2(sp),-(sp)  ; push height
                move.w          col+4(sp),-(sp)     ; push col
                move.w          row+6(sp),-(sp)     ; push row
                jsr             _check_bounds
                adda.l             #8,sp            ; clean up stack
                
                ; Check return status
                cmpi.b          #3,d0               ; check if entirely out of bounds
                beq             done                ; if so, return immediately
                
                tst.b           d0                  ; check if any clipping needed
                bne             use_clipped         ; if status != 0 (but not 3), use clipped version
                
                ; No clipping needed, continue with optimized routine
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
                btst            #0,d1           ; test if start address is odd
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
                
movem_loop:     ; (happy)
                movem.l         (a1)+,d0-d1     ; get 2 longs (8 bytes) of bitmap data
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

byte_copy:      ; Byte-by-byte copy for misaligned addresses (unhappy)
             
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
                move.w          #32,-(sp)           ; push width (32 pixels)
                move.w          height(sp),-(sp)    ; push height
                move.l          bitmap(sp),-(sp)    ; push bitmap
                move.w          col+6(sp),-(sp)     ; push col
                move.w          row+8(sp),-(sp)     ; push row
                move.l          base+10(sp),-(sp)   ; push base
                jsr             _plot_clipped_bitmap
                lea             18(sp),sp           ; clean up (4+2+4+2+2+2+2 = 18 bytes)
                rts

