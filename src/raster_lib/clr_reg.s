;
; PURPOSE: Clear a region of the screen. The section is specified by the coordinates of the top left corner, and the height and width of the region.
;
; INPUT: Address(UINT32*): to the start of the screen
;        Position(row,col): the coordinates of the top left pixel of the region
;        Length: the lenth (number of rows) in pixels of the region
;        Width: the width (number of columns) in pixels of the region
;
; OUTPUT: None
;
; void clear_region(UINT32 *base, UINT16 row, UINT16 col, UINT16 length, UINT16 width);
;________________________________________________________________
; Clear a rectangular regin of the screen.
;
;                     -- Optimized for 48 bit widths ONLY (6 bytes wide). --
;                 -- This should be the size of all sprites to begin with. --
;
; Contact @sudonym-i if you will need to use this routine in any other ways



                xdef            _clear_region

base            equ             64              ; offset from SP, not A6
row             equ             68               
col             equ             70             
length          equ             72
width           equ             74


_clear_region:  movem.l         d0-d7/a0-a6,-(sp)

                movea.l         base(sp),a0     ; get base address
                
                ; Calculate and add row offset: row * 80 bytes
                move.w          row(sp),d0
                mulu.w          #80,d0          ; screen width in bytes (result is long)
                adda.l          d0,a0           ; add row offset
                
                ; Calculate and add col offset: col / 8 (pixels to bytes)
                move.w          col(sp),d0
                lsr.w           #3,d0           ; divide by 8 (shift right 3 bits)
                ext.l           d0              ; extend to long
                adda.l          d0,a0           ; add col offset in bytes

                ; Check if width == 48 pixels (6 bytes) for optimization
                move.w          width(sp),d0
                cmpi.w          #48,d0          ; check if exactly 48 pixels
                bne             unoptimized
                
                ; Ensure word alignment for movem.w (address must be even)
                move.l          a0,d1
                btst            #0,d1           ; test if address is odd
                bne             unoptimized     ; if odd, use unoptimized version
                bra             opt_48x48

opt_48x48:      
                ; Optimized for 48x48 sprites (6 bytes = 3 words wide)
                ; Uses movem.w to clear 3 words at once
                
                moveq           #0,d1           ; clear registers to zero
                moveq           #0,d2
                moveq           #0,d3
                
                move.w          length(sp),d7   ; get height (number of rows in pixels)
                subq.w          #1,d7           ; adjust for dbra
                
row_loop_48:    movem.w         d1-d3,(a0)      ; write 3 words (6 bytes)
                adda.w          #80,a0          ; move to next row (80 bytes per row)
                dbra            d7,row_loop_48
                
                movem.l         (sp)+,d0-d7/a0-a6
                rts

unoptimized:    
                ; Generic clear region - calculate bytes needed for col through col+width-1
                ; Formula: ceiling((col+width)/8) - floor(col/8)
                
                move.w          col(sp),d6      ; get col in pixels
                add.w           width(sp),d6    ; d6 = col + width
                addq.w          #7,d6           ; d6 = col + width + 7 (for ceiling division)
                lsr.w           #3,d6           ; d6 = (col + width + 7) / 8 = ceiling((col+width)/8)
                
                move.w          col(sp),d5      ; get col in pixels  
                lsr.w           #3,d5           ; d5 = col / 8 = floor(col/8)
                
                sub.w           d5,d6           ; d6 = ceiling - floor = num_bytes
                subq.w          #1,d6           ; adjust for dbra
                
                move.w          length(sp),d7   ; outer loop: rows (in pixels)
                subq.w          #1,d7           ; adjust for dbra
                
row_loop:       movea.l         a0,a1           ; save row start position
                move.w          d6,d5           ; restore column counter
                
col_loop:       clr.b           (a1)+           ; clear one byte, advance
                dbra            d5,col_loop
                
                adda.w          #80,a0          ; move to next row
                dbra            d7,row_loop
                
                movem.l         (sp)+,d0-d7/a0-a6
                rts
