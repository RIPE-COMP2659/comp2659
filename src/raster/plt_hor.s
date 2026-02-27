;
; PURPOSE: Plot a hoizontal line on the screen. The horizontal line is specified by the leftmost pixel of the line and the length of the line.
;
; INPUT: Address(UINT8*): to the start of the screen
;        Position(row,col): the coordinates of the leftmost pixel of the horizontal line
;        Length: the lenth in pixels of the line
;        Color: color of the line (0 = black, 1 = white)
;
; OUTPUT: None
;
; void plot_horizontal_line(UINT32 *base, UINT16 row, UINT16 col, UINT16 length, UINT16 color);
;________________________________________________________________
; Draw a horizontal line of pixels
;
;                      -- This routine is unoptimized, as we shouldn't be using it ---
;                                       DO NOT USE THIS METHOD
;
; Contact @sudonym-i if you will need to use this routine.


        xdef    _plot_horizontal_line

base    equ     8               
row     equ     12             
col     equ     14            
length  equ     16             
color   equ     18            


_plot_horizontal_line:    
        link    a6,#0
        movem.l d0-d3/a0,-(sp)
                
                ; Check if length is zero
        move.w  length(a6),d3
        beq     done                            ; if length = 0, exit
                
        movea.l base(a6),a0                     ; get base address
                
                ; Calculate and add row offset: row * 80 bytes
        move.w  row(a6),d0
        mulu.w  #80,d0                          ; screen width in bytes
        adda.l  d0,a0                           ; add row offset
                
                ; Calculate and add col byte offset: col / 8
        move.w  col(a6),d1
        move.w  d1,d2                           ; save col for bit calculation
        lsr.w   #3,d1                           ; d1 = col / 8 (byte offset)
        adda.w  d1,a0                           ; a0 now points to the byte
                
                ; Calculate initial bit position: 7 - (col % 8)
        andi.w  #7,d2                           ; d2 = col % 8
        moveq   #7,d0
        sub.w   d2,d0                           ; d0 = 7 - (col % 8) = bit position
                
                ; Get color parameter (0 = black/bclr, non-zero = white/bset)
        move.w  color(a6),d1
                
pixel_loop:
                ; Set or clear bit based on color
        tst.w   d1                              ; check color
        beq     clear_bit
        bset    d0,(a0)                         ; white: set bit
        bra     next_pixel
clear_bit:
        bclr    d0,(a0)                         ; black: clear bit
                
next_pixel:
                ; Decrement length counter
        subq.w  #1,d3
        beq     done                            ; if counter = 0, we're done
                
                ; Move to next pixel (decrement bit position)
        subq.w  #1,d0
        bge     pixel_loop                      ; if d0 >= 0, stay in same byte
                
                ; Crossed byte boundary - move to next byte
        addq.l  #1,a0                           ; next byte
        moveq   #7,d0                           ; reset bit position to 7
        bra     pixel_loop

done:
        movem.l (sp)+,d0-d3/a0
        unlk    a6
        rts

