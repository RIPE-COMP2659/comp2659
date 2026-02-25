;
; PURPOSE: Plots a single pixel on the screen.
;
; INPUT: Address(UINT32*): to the start of the screen
;        Position(row,col): the location of the pixel to plot
;
; OUTPUT: None
;
; void plot_pixel(UINT8 *base, UINT16 row, UINT16 col);
;________________________________________________________________
; Draw a single pixel ( NEVER USE THIS )
;
; Contact @sudonym-i if you will need to use this routine. Explain yourself!

                xdef            _plot_pixel

base            equ             8               ; offset from A6
row             equ             12              ; UINT16 (2 bytes)
col             equ             14              ; UINT16 (2 bytes)


_plot_pixel:    
                link            a6,#0
                movem.l         d0-d2/a0,-(sp)
                movea.l         base(a6),a0     ; get base address
                
                ; Calculate and add row offset: row * 80 bytes
                move.w          row(a6),d0
                mulu.w          #80,d0          ; screen width in bytes
                adda.l          d0,a0           ; add row offset
                
                ; Calculate and add col byte offset: col / 8
                move.w          col(a6),d1
                move.w          d1,d2           ; save col for bit calculation
                lsr.w           #3,d1           ; d1 = col / 8 (byte offset)
                adda.w          d1,a0           ; a0 now points to the byte
                
                ; Calculate bit position: 7 - (col % 8)
                andi.w          #7,d2           ; d2 = col % 8
                moveq           #7,d0
                sub.w           d2,d0           ; d0 = 7 - (col % 8) = bit position
                
                ; Set the bit
                bset            d0,(a0)         ; set bit d0 in byte at (a0)

                movem.l         (sp)+,d0-d2/a0
                unlk            a6
                rts

                