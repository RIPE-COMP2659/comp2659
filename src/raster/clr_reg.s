;
; PURPOSE: Clear a region of the screen. The section is specified by the coordinates of the top left corner, and the height and width of the region.
;
; INPUT: Address(UINT32*): to the start of the screen
;        Position(row,col): the coordinates of the top left pixel of the region
;        Length: the length (number of rows) in pixels of the region
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
; clr_reg.s
; Authors:
;     Riley Gramlich, rgram060@mtroyal.ca, 201762060
;     Robert Parker Hutcheson, rhutc335@mtroyal.ca, 201762335
;     Isaac Klein, iklei977@mtroyal.ca, 201763977
;     Eduard Mykhailets, emykh268@mtroyal.ca, 201750268
; Course: COMP 2659-001, Computing Machinery II, Winter 2026
; Instructor: Nolan Shaw
;
; PURPOSE: Registers and helpers for clearing regions / color registers.
;
SCREEN_HEIGHT equ 400                           ; Screen height in pixels
        xdef    _clear_region

base    equ     8               
row     equ     12               
col     equ     14             
length  equ     16
width   equ     18


_clear_region: 

        link    a6,#0
        movem.l d0-d7/a0-a5,-(sp)

                ; Check vertical bounds

check_top_clip:
                ; Vertical clipping first - check if top edge is off screen
        move.w  row(a6),d0
        bge     check_bottom

                ; Handle top clipping (row < 0)
        move.w  length(a6),d1
        add.w   d0,d1                           ; d1 = length + row (row is negative)
        ble     done                            ; If still <= 0, entirely off screen
        move.w  d1,length(a6)                   ; Update length to clipped height
        clr.w   row(a6)                         ; Set row to 0 (top of screen)
        
  
check_bottom:
        move.w  row(a6),d0
        cmp.w   #SCREEN_HEIGHT,d0
        bge     done                            ; If row >= SCREEN_HEIGHT, entirely off bottom

        add.w   length(a6),d0                   ; d0 = row + height (bottom edge Y)
        cmp.w   #SCREEN_HEIGHT,d0
        ble     v_clip_done                     ; If bottom edge <= SCREEN_HEIGHT, skip bottom clip

                ; Partially off bottom
        move.w  #SCREEN_HEIGHT,d0
        sub.w   row(a6),d0                      ; d0 = SCREEN_HEIGHT - row
        move.w  d0,length(a6)                   ; Update height to fit exactly on screen

v_clip_done:

        tst.w   length(a6)                      ; nothing to clear if length == 0
        beq     done
        tst.w   width(a6)                       ; nothing to clear if width == 0
        beq     done

calc_screen_offsets:
        movea.l base(a6),a0                     ; get base address
                
                ; Calculate and add row offset: row * 80 bytes
        move.w  row(a6),d0
        mulu.w  #80,d0                          ; screen width in bytes (result is long)
        adda.l  d0,a0                           ; add row offset
                
                ; Calculate and add col offset: col / 8 (pixels to bytes)
        move.w  col(a6),d0
        lsr.w   #3,d0                           ; divide by 8 (shift right 3 bits)
        ext.l   d0                              ; extend to long
        adda.l  d0,a0                           ; add col offset in bytes

check_alignment:
                ; Check if width == 32 pixels (4 bytes) for optimization
        move.w  width(a6),d0
        cmpi.w  #32,d0                          ; check if exactly 32 pixels
        bne     unoptimized

                ; 32-pixel fast path only valid when col is byte-aligned
        move.w  col(a6),d0
        andi.w  #7,d0                           ; d0 = col % 8
        bne     unoptimized

                ; Ensure word alignment for move.l (address must be even)
        move.l  a0,d1
        btst    #0,d1                           ; test if address is odd
        bne     unoptimized                     ; if odd, use unoptimized version
        bra     opt_32

opt_32:      
                ; Optimized for 32-pixel width (4 bytes = 1 long)
                ; Uses move.l to clear 1 long at once
                
        moveq   #0,d1                           ; clear register to zero
                
        move.w  length(a6),d7                   ; get height (number of rows in pixels)
        subq.w  #1,d7                           ; adjust for dbra
                
row_loop_32: move.l d1,(a0)                     ; write 1 long (4 bytes)
        adda.w  #80,a0                          ; move to next row (80 bytes per row)
        dbra    d7,row_loop_32
                
        movem.l (sp)+,d0-d7/a0-a5
        unlk    a6
        rts

unoptimized:
                ; Generic clear with edge masking for non-byte-aligned regions.
                ; d2 = start_bit (col % 8)
                ; d3 = end_bit ((col + width) % 8)
                ; d6 = num_bytes = ceil((start_bit + width)/8)

        move.w  col(a6),d2
        andi.w  #7,d2                           ; d2 = start_bit

        move.w  col(a6),d3
        add.w   width(a6),d3
        andi.w  #7,d3                           ; d3 = end_bit

        move.w  d2,d6
        add.w   width(a6),d6
        addq.w  #7,d6
        lsr.w   #3,d6                           ; d6 = number of bytes touched per row

prep_unoptimized_loop:
        move.w  length(a6),d7
        subq.w  #1,d7                           ; adjust for dbra

row_loop:
        movea.l a0,a1                           ; row start pointer
        move.w  d6,d5                           ; d5 = bytes remaining in this row

        cmpi.w  #1,d5
        beq     single_byte_row

                ; First byte: partial clear if start_bit != 0, else full clear
        tst.w   d2
        beq     first_full_byte
        moveq   #-1,d4
        moveq   #8,d0
        sub.w   d2,d0                           ; d0 = 8 - start_bit
        lsl.w   d0,d4                           ; keep high bits before region start
        and.b   d4,(a1)+
        subq.w  #1,d5
        bra     after_first_byte

first_full_byte:
        clr.b   (a1)+
        subq.w  #1,d5

after_first_byte:
                ; Reserve last byte if it is partial
        tst.w   d3
        beq     clear_middle_and_last_full
        subq.w  #1,d5

clear_middle_partial_last:
        tst.w   d5
        beq     apply_last_partial
clear_middle_partial_last_loop:
        clr.b   (a1)+
        subq.w  #1,d5
        bne     clear_middle_partial_last_loop

apply_last_partial:
        move.w  #$00ff,d4
        lsr.w   d3,d4                           ; keep low bits after region end
        and.b   d4,(a1)
        bra     next_row

clear_middle_and_last_full:
        tst.w   d5
        beq     next_row
clear_middle_and_last_full_loop:
        clr.b   (a1)+
        subq.w  #1,d5
        bne     clear_middle_and_last_full_loop
        bra     next_row

single_byte_row:
                ; Region fits in one byte: preserve bits before start and after end.
                ; preserve_mask = high_preserve | low_preserve
        moveq   #0,d4

        tst.w   d2
        beq     single_skip_high
        moveq   #-1,d0
        moveq   #8,d1
        sub.w   d2,d1                           ; d1 = 8 - start_bit
        lsl.w   d1,d0                           ; high_preserve
        or.w    d0,d4

single_skip_high:
        tst.w   d3
        beq     single_apply
        move.w  #$00ff,d0
        lsr.w   d3,d0                           ; low_preserve
        or.w    d0,d4

single_apply:
        and.b   d4,(a1)

next_row:
        adda.w  #80,a0
        dbra    d7,row_loop

done:
        movem.l (sp)+,d0-d7/a0-a5
        unlk    a6
        rts

        
