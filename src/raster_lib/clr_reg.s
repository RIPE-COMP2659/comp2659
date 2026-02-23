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
; Clear a rectangular region of the screen.
;
; OPTIMIZATION: For best performance, use 32-pixel (4-byte) width with word-aligned starting address.
; Falls back to generic byte-by-byte clearing for other widths or odd addresses.
;



                xdef            _clear_region

base            equ             64              
row             equ             68               
col             equ             70             
length          equ             72
width           equ             74


_clear_region:  
                movem.l         d0-d7/a0-a6,-(sp)

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

                ; Check if width == 32 pixels (4 bytes) for optimization
                move.w          width(sp),d0
                cmpi.w          #32,d0          ; check if exactly 32 pixels
                bne             unoptimized
                
                ; Ensure word alignment for move.l (address must be even)
                move.l          a0,d1
                btst            #0,d1           ; test if address is odd
                bne             unoptimized     ; if odd, use unoptimized version
                bra             opt_32

opt_32:      
                ; Optimized for 32-pixel width (4 bytes = 1 long)
                ; Uses move.l to clear 1 long at once
                
                moveq           #0,d1           ; clear register to zero
                
                move.w          length(sp),d7   ; get height (number of rows in pixels)
                subq.w          #1,d7           ; adjust for dbra
                
row_loop_32:    move.l          d1,(a0)         ; write 1 long (4 bytes)
                adda.w          #80,a0          ; move to next row (80 bytes per row)
                dbra            d7,row_loop_32
                
                movem.l         (sp)+,d0-d7/a0-a6
                rts

unoptimized:    
                ; Generic clear region - calculate bytes needed for col through col+width-1
                ; Formula: ceil((col+width)/8) - floor(col/8)
                
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
