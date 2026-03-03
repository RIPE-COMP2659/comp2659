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

        xdef    _plot_bitmap_8
        xref    _check_bounds
        xref    _plot_clipped_bitmap

SCREEN_HEIGHT equ 400                           ; Screen height in pixels

base    equ     8
row     equ     12
col     equ     14
bitmap  equ     16
height  equ     20


_plot_bitmap_8: 
        link    a6,#0
        movem.l d0-d7/a0-a5,-(sp)

;--------------------------------------------------------------------------------------------
;                       !MIGRATE to pass-by-register for faster performance in the future!
;                       This will avoid read/writes to memory, which costs us clock cycles
;--------------------------------------------------------------------------------------------

                ; Check bounds using the _check_bounds routine.

                ; Vertical clipping first - check if top edge is off screen
        move.w  row(a6),d0
        bge.s   check_bottom                    ; If row >= 0, skip top clip

                ; Handle top clipping (row < 0)
        move.w  d0,d1
        neg.w   d1                              ; d1 = pixels off top (|row|)
        cmp.w   height(a6),d1                   ; Is the whole bitmap off top?
        bge     done                            ; If so, entirely off screen, skip drawing

                ; Partially off top
        add.w   d0,height(a6)                   ; Decrease height by clipped rows
        clr.w   row(a6)                         ; Set row to 0 (top of screen)

                ; Advance bitmap pointer (1 byte per row for 8-bit)
        move.l  bitmap(a6),a0
        ext.l   d1                              ; Extend word to long for address math
        adda.l  d1,a0                           ; Advance pointer
        move.l  a0,bitmap(a6)                   ; Save updated bitmap pointer

check_bottom:
        move.w  row(a6),d0
        cmp.w   #SCREEN_HEIGHT,d0
        bge     done                            ; If row >= SCREEN_HEIGHT, entirely off bottom

        add.w   height(a6),d0                   ; d0 = row + height (bottom edge Y)
        cmp.w   #SCREEN_HEIGHT,d0
        ble.s   v_clip_done                     ; If bottom edge <= SCREEN_HEIGHT, skip bottom clip

                ; Partially off bottom
        move.w  #SCREEN_HEIGHT,d0
        sub.w   row(a6),d0                      ; d0 = SCREEN_HEIGHT - row
        move.w  d0,height(a6)                   ; Update height to fit exactly on screen

v_clip_done:

        move.w  #8,-(sp)                        ; push width (8 pixels)
        move.w  height(a6),-(sp)                ; push height
        move.w  col(a6),-(sp)                   ; push col
        move.w  row(a6),-(sp)                   ; push row
        jsr     _check_bounds
        adda.l  #8,sp                           ; clean parameters from  stack

                ; Check return status
        cmpi.b  #3,d0                           ; check if entirely out of bounds (nothing to draw)
        beq     done                            ; if so, return immediately
                
        tst.b   d0                              ; check if any clipping needed
        bne     use_clipped                     ; if status != 0 (but not 3), use clipped version
                
                ; No clipping needed, continue with normal routine
        movea.l base(a6),a0                     ; get base address (screen starting pointer)
        movea.l bitmap(a6),a1                   ; get bitmap start address
                
                ; Calculate and add row offset: row * 80 bytes
        move.w  row(a6),d0
        mulu.w  #80,d0                          ; screen width in bytes
        adda.l  d0,a0                           ; add row offset
                
                ; Calculate col byte offset and bit shift
        move.w  col(a6),d6                      ; d6 = col (pixel column)
        move.w  d6,d5                           ; copy for bit calculation
        lsr.w   #3,d6                           ; d6 = col / 8 ( bit shift )
        ext.l   d6
        adda.l  d6,a0                           ; add col offset in bytes
                
        andi.w  #7,d5                           ; d5 = col % 8 (bit shift amount).
                                                ; Don't really understand it, but its from the labs. 
        beq     aligned_copy                    ; if 0, no shifting needed ( happy )
                
                ; Unaligned copy - need to shift bits ( not happy )
        move.w  height(a6),d7                   ; get height
        subq.w  #1,d7                           ; adjust for dbra
                
                ; Calculate (8-d5) for masking
        moveq   #8,d6
        sub.w   d5,d6                           ; d6 = 8 - bit_offset
                
shift_loop: moveq #0,d0                         ; clear d0
        move.b  (a1)+,d0                        ; get bitmap byte into low byte
        lsl.w   #8,d0                           ; shift to high byte: 0x00FF --> 0xFF00
        lsr.w   d5,d0                           ; shift right by bit offset
                
                ; d0 now has: high byte = bits for current screen byte
                ;             low byte = bits for next screen byte
                
                ; Write to current byte with masking
        moveq   #-1,d4                          ; start with all 1s
        lsl.w   d6,d4                           ; shift left by (8-d5) to create mask for high bits
        and.b   d4,(a0)                         ; preserve the high bits
        move.b  d0,d1                           ; get low byte (for next screen byte)
        lsr.w   #8,d0                           ; get high byte into low position
        or.b    d0,(a0)                         ; OR bitmap bits into current screen byte
                
                ; Write to next byte with masking  
        move.l  #$ff,d4                         ; start with 0x0000FF
        lsr.l   d5,d4                           ; shift right by d5 to create mask for low bits
        and.b   d4,1(a0)                        ; preserve the low bits
        or.b    d1,1(a0)                        ; OR bitmap bits into next screen byte
                
        adda.w  #80,a0                          ; move to next row
        dbra    d7,shift_loop
                
        movem.l (sp)+,d0-d7/a0-a5
        unlk    a6
        rts

aligned_copy:
                ; CASE: Byte-aligned - can use optimized copy
        move.w  height(a6),d7                   ; get height (number of rows)
        subq.w  #1,d7                           ; adjust for dbra
                
row_loop: move.b (a1)+,(a0)                     ; copy one byte of bitmap to screen
        adda.w  #80,a0                          ; move to next row (80 bytes per row)
        dbra    d7,row_loop
                
        movem.l (sp)+,d0-d7/a0-a5
        unlk    a6
        rts

done:
        movem.l (sp)+,d0-d7/a0-a5
        unlk    a6
        rts

use_clipped:
        move.b  d0,d7                           ; d7 = status from check_bounds
        move.w  d1,d6                           ; d6 = new_width from check_bounds
                
;--------------------------------------------------------------------------------------------
;                       !MIGRATE to pass-by-register for faster performance in the future!
;                       This will avoid read/writes to memory, which costs us clock cycles
;--------------------------------------------------------------------------------------------

                ; Now push parameters for _plot_clipped_bitmap
        move.w  d6,-(sp)                        ; push new_width
        move.w  d7,-(sp)                        ; push status
        move.w  #8,-(sp)                        ; push width (8 pixels)
        move.w  height(a6),-(sp)                ; push height
        move.l  bitmap(a6),-(sp)                ; push bitmap
        move.w  col(a6),-(sp)                   ; push col
        move.w  row(a6),-(sp)                   ; push row
        move.l  base(a6),-(sp)                  ; push base
        jsr     _plot_clipped_bitmap
        adda.l  #20,sp                          ; clean up parameters
        movem.l (sp)+,d0-d7/a0-a5               ; restore saved registers
        unlk    a6
        rts
