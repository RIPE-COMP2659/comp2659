
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

SCREEN_HEIGHT equ       400                             ; Screen height in pixels

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


 ; Vertical clipping first - check if top edge is off screen
        move.w  row(a6),d0
        bge.s   check_bottom                            ; If row >= 0, skip top clip

                ; Handle top clipping (row < 0)
        move.w  d0,d1
        neg.w   d1                                      ; d1 = pixels off top (|row|)
        cmp.w   height(a6),d1                           ; Is the whole bitmap off top?
        bge     done                                    ; If so, entirely off screen, skip drawing

                ; Partially off top
        add.w   d0,height(a6)                           ; Decrease height by clipped rows
        clr.w   row(a6)                                 ; Set row to 0 (top of screen)

                ; Advance bitmap pointer (2 bytes per row for 16-bit)
        move.l  bitmap(a6),a0
        ext.l   d1                                      ; Extend word to long for address math
        lsl.l   #1,d1                                   ; Multiply by 2 (2 bytes per row)
        adda.l  d1,a0                                   ; Advance pointer
        move.l  a0,bitmap(a6)                           ; Save updated bitmap pointer

check_bottom:
        move.w  row(a6),d0
        cmp.w   #SCREEN_HEIGHT,d0
        bge     done                                    ; If row >= SCREEN_HEIGHT, entirely off bottom

        add.w   height(a6),d0                           ; d0 = row + height (bottom edge Y)
        cmp.w   #SCREEN_HEIGHT,d0
        ble.s   v_clip_done                             ; If bottom edge <= SCREEN_HEIGHT, skip bottom clip

                ; Partially off bottom
        move.w  #SCREEN_HEIGHT,d0
        sub.w   row(a6),d0                              ; d0 = SCREEN_HEIGHT - row
        move.w  d0,height(a6)                           ; Update height to fit exactly on screen

v_clip_done:
        
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
                
                ; Calculate and add col offset: col / 8 (pixels to bytes)
        move.w  col(a6),d0
        lsr.w   #3,d0                           ; divide by 8 (shift right 3 bits)
        ext.l   d0                              ; extend to long
        adda.l  d0,a0                           ; add col offset in bytes
                
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
