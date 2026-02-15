;
; PURPOSE: Plot a hoizontal line on the screen. The horizontal line is specified by the leftmost pixel of the line and the length of the line.
;
; INPUT: Address(UINT8*): to the start of the screen
;        Position(row,col): the coordinates of the leftmost pixel of the horizontal line
;        Length: the lenth in pixels of the line
;
; OUTPUT: None
;
; void plot_horizontal_line(UINT32 *base, UINT16 row, UINT16 col, UINT16 length);
;________________________________________________________________
; Draw a horizontal line of pixels
;
;                      -- This routine is unoptimized, as we shouldn't be using it ---
;                                       DO NOT USE THIS METHOD
;
; Contact @sudonym-i if you will need to use this routine.


                xdef            _plot_horizontal_line

base            equ             24              ; offset from SP
row             equ             28              ; UINT16 (2 bytes)
col             equ             30              ; UINT16 (2 bytes)
length          equ             32              ; UINT16 (2 bytes)


_plot_horizontal_line:    
                movem.l         d0-d3/a0,-(sp)
                
                ; Check if length is zero
                move.w          length(sp),d3
                beq             done            ; if length = 0, exit
                
                movea.l         base(sp),a0     ; get base address
                
                ; Calculate and add row offset: row * 80 bytes
                move.w          row(sp),d0
                mulu.w          #80,d0          ; screen width in bytes
                adda.l          d0,a0           ; add row offset
                
                ; Calculate and add col byte offset: col / 8
                move.w          col(sp),d1
                move.w          d1,d2           ; save col for bit calculation
                lsr.w           #3,d1           ; d1 = col / 8 (byte offset)
                adda.w          d1,a0           ; a0 now points to the byte
                
                ; Calculate initial bit position: 7 - (col % 8)
                andi.w          #7,d2           ; d2 = col % 8
                moveq           #7,d0
                sub.w           d2,d0           ; d0 = 7 - (col % 8) = bit position
                
pixel_loop:
                ; Set the bit
                bset            d0,(a0)         ; set bit d0 in byte at (a0)
                
                ; Decrement length counter
                subq.w          #1,d3
                beq             done            ; if counter = 0, we're done
                
                ; Move to next pixel (decrement bit position)
                subq.w          #1,d0
                bge             pixel_loop      ; if d0 >= 0, stay in same byte
                
                ; Crossed byte boundary - move to next byte
                addq.l          #1,a0           ; next byte
                moveq           #7,d0           ; reset bit position to 7
                bra             pixel_loop

done:
                movem.l         (sp)+,d0-d3/a0
                rts

