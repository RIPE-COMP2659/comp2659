
;
; PURPOSE: Plots a bitmap to the screen given by the top left pixel of the bitmap and the height of bitmap.
;
; INPUT: Address(UINT8*): to the start of the screen
;        Position(row,col): the coordinates of the top left pixel of the bitmap
;        bitmap(UINT16*): pointer to bitmap data (16 pixels wide, 2 bytes per row)
;        Height: the length (number of rows) of the height in pixels of the bitmap
;
; OUTPUT: None
;
; void plot_bitmap_16(UINT8 *base, UINT16 row, UINT16 col, const UINT16 *bitmap, UINT16 height);
;________________________________________________________________
; Plot a 16-pixel wide bitmap.
; 
; For optimal performance:
;   - Bitmap address is long-word aligned (divisible by 4) -- may be achievable
;   - Height is even
;

        xdef    _plot_bitmap_16
        xref    _check_bounds
        xref    _plot_clipped_bitmap

base    equ     8             
row     equ     12               
col     equ     14
bitmap  equ     16
height  equ     20


_plot_bitmap_16: 
        link    a6,#0
        movem.l d0-d7/a0-a5,-(sp)


;--------------------------------------------------------------------------------------------
;                       !MIGRATE to pass-by-register for faster performance in the future!
;                       This will avoid read/writes to memory, which costs us clock cycles
;--------------------------------------------------------------------------------------------
                ; Check bounds first
        move.w  #16,-(sp)                       ; push width (16 pixels)
        move.w  height(a6),-(sp)                ; push height
        move.w  col(a6),-(sp)                   ; push col
        move.w  row(a6),-(sp)                   ; push row
        jsr     _check_bounds
        adda.l  #8,sp                           ; clean up stack
                
                ; Check return status
        cmpi.b  #3,d0                           ; check if entirely out of bounds
        beq     done                            ; if so, return immediately
                
        tst.b   d0                              ; check if any clipping needed
        bne     use_clipped                     ; if status != 0 (but not 3), use clipped version
                
                ; No clipping needed, continue with optimized routine
        movea.l base(a6),a0                     ; get base address (screen)
        movea.l bitmap(a6),a1                   ; get bitmap data address
                
                ; Calculate and add row offset: row * 80 bytes
        move.w  row(a6),d0
        mulu.w  #80,d0                          ; screen width in bytes
        adda.l  d0,a0                           ; add row offset
                
                ; Calculate col byte offset and bit shift
        move.w  col(a6),d6                      ; d6 = col (pixel column)
        move.w  d6,d5                           ; copy for bit calculation
        lsr.w   #3,d6                           ; d6 = col / 8 (byte offset)
        ext.l   d6                              ; extend to long
        adda.l  d6,a0                           ; add col offset in bytes
        
        andi.w  #7,d5                           ; d5 = col % 8 (bit shift amount)
        bne     unaligned_copy_16               ; if non-zero, use unaligned copy
                
                ; Byte-aligned copy - use optimized paths
                ; Check long word alignment for optimization
        move.l  a0,d1
        btst    #0,d1                           ; test if start address is odd
        bne     byte_copy                       ; if odd, MUST use byte copy (word ops fail on odd addresses!)
        btst    #1,d1                           ; test if screen address is word-aligned but not long-aligned,
                                                ; as 00 implies the value is divisible by 4

        bne     word_copy                       ; if not long-aligned, use word copy
                
        move.l  a1,d1
        btst    #0,d1                           ; test if bitmap address is odd
        bne     byte_copy                       ; if odd, MUST use byte copy
        btst    #1,d1                           ; test if bitmap address is word-aligned but not long-aligned
        bne     word_copy                       ; if not long-aligned, use word copy
                
                ; Check if we have even number of rows for long optimization
        move.w  height(a6),d7
        btst    #0,d7                           ; test if height is odd
        bne     word_copy                       ; if odd, use word copy
                
                ; Optimized long copy (2 rows at a time)
        lsr.w   #1,d7                           ; divide height by 2
        subq.w  #1,d7                           ; adjust for dbra
                
long_loop: move.l (a1)+,d0                      ; get 2 words (4 bytes) of bitmap data
        move.w  d0,80(a0)                       ; plot second row (low word)
        swap    d0                              ; get high word
        move.w  d0,(a0)                         ; plot first row (high word)
        adda.w  #160,a0                         ; move to current_row + 2 (2 rows * 80 bytes)
        dbra    d7,long_loop
        
        movem.l (sp)+,d0-d7/a0-a5
        unlk    a6
        rts

unaligned_copy_16:
                ; Unaligned copy - need to shift bits (col % 8 != 0)
        move.w  height(a6),d7                   ; get height
        subq.w  #1,d7                           ; adjust for dbra
                
shift_loop_16: moveq #0,d0                     ; clear d0
        move.b  (a1)+,d0                        ; get bitmap byte 0 into low byte
        lsl.w   #8,d0                           ; shift to high byte
        lsr.w   d5,d0                           ; shift right by bit offset
                ; d0 now has: high byte = bits for current screen byte
                ;             low byte = bits for next screen byte
        move.b  d0,d1                           ; get low byte (for next screen byte)
        lsr.w   #8,d0                           ; get high byte into low position
        or.b    d0,(a0)                         ; OR into current screen byte
        or.b    d1,1(a0)                        ; OR into next screen byte
        
        moveq   #0,d0
        move.b  (a1)+,d0                        ; get bitmap byte 1 into low byte
        lsl.w   #8,d0
        lsr.w   d5,d0
        move.b  d0,d1
        lsr.w   #8,d0
        or.b    d0,1(a0)                        ; OR into screen byte 1
        or.b    d1,2(a0)                        ; OR into screen byte 2
        
        adda.w  #80,a0                          ; move to next row
        dbra    d7,shift_loop_16
                
        movem.l (sp)+,d0-d7/a0-a5
        unlk    a6
        rts

word_copy:      
                ; Check if screen address is word-aligned (even)
        move.l  a0,d1
        btst    #0,d1                           ; test if screen address is odd
        bne     byte_copy                       ; if odd, must use byte copy
                
                ; Check if bitmap address is word-aligned
        move.l  a1,d1
        btst    #0,d1                           ; test if bitmap address is odd
        bne     byte_copy                       ; if odd, must use byte copy
                
                ; Standard word-by-word copy
        move.w  height(a6),d7                   ; get height (number of rows)
        subq.w  #1,d7                           ; adjust for dbra
                
row_loop: move.w (a1)+,(a0)                     ; copy one word of bitmap to screen
        adda.w  #80,a0                          ; move to next row (80 bytes per row)
        dbra    d7,row_loop
                
        movem.l (sp)+,d0-d7/a0-a5
        unlk    a6
        rts

byte_copy:
                ; Byte-by-byte copy for misaligned addresses
        move.w  height(a6),d7                   ; get height (number of rows)
        subq.w  #1,d7                           ; adjust for dbra
                
byte_loop: move.b (a1)+,(a0)+                   ; copy first byte
        move.b  (a1)+,(a0)                      ; copy second byte
        adda.w  #79,a0                          ; move to next row (80 - 1 already advanced)
        dbra    d7,byte_loop
                
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
        move.w  #16,-(sp)                       ; push width (16 pixels)
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
