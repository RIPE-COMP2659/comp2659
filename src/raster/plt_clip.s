;
;  Helper function to plot all clipped bitmaps 
;
; void plot_clipped_bitmap(UINT8 *base, UINT16 row, UINT16 col, const void *bitmap, UINT16 height, UINT16 width, UINT16 status, UINT16 new_width);
;________________________________________________________________

;--------------------------------------------------------------------------------------------
;                       !MIGRATE to pass-by-register for faster performance in the future!
;                       This will avoid read/writes to memory, which costs us clock cycles
;--------------------------------------------------------------------------------------------


        xdef    _plot_clipped_bitmap

base    equ     8 
row     equ     12               
col     equ     14
bitmap  equ     16              
height  equ     20
width   equ     22
status  equ     24
new_width equ   26


_plot_clipped_bitmap:
        link    a6,#0
        movem.l d0-d7/a0-a5,-(sp)

                ; Get status and new_width from parameters (already computed by caller)
        move.w  status(a6),d7                   ; get status in d7
        move.w  new_width(a6),d6                ; get new_width in d6
                
                ; Check status and handle accordingly
        cmpi.b  #3,d7
        beq     done_clip                       ; if entirely out of bounds, return
                
        cmpi.b  #0,d7
        beq     plot_normal                     ; if no clipping needed, plot normally
                
        cmpi.b  #1,d7
        beq     plot_left_clip                  ; if left edge clipping
                
        cmpi.b  #2,d7
        beq     plot_right_clip                 ; if right edge clipping
                
        bra     done_clip                       ; safety - should never reach here

plot_normal:
                ; No clipping needed - plot full bitmap
        movea.l base(a6),a0                     ; get screen base
        movea.l bitmap(a6),a1                   ; get bitmap data
        move.w  row(a6),d0
        move.w  col(a6),d1
        move.w  height(a6),d2
        move.w  width(a6),d3                    ; clipped width = original width
        move.w  width(a6),d4                    ; original width
        bsr     do_plot
        bra     done_clip

plot_left_clip:
                ; Left edge clipping: adjust col to 0, adjust bitmap pointer
                ; new_width is in d6
        movea.l base(a6),a0                     ; get screen base
        movea.l bitmap(a6),a1                   ; get bitmap data
        move.w  row(a6),d0                      ; row unchanged
        move.w  col(a6),d5                      ; get original col (negative)
        neg.w   d5                              ; d5 = abs(col) = pixels to skip
        moveq   #0,d1                           ; new col = 0
        move.w  height(a6),d2                   ; height unchanged
        move.w  d6,d3                           ; use new_width from check_bounds
        move.w  width(a6),d4                    ; original bitmap width
                
                ; Adjust bitmap pointer: skip (abs(col)/8) bytes per row
                ; For simplicity, we'll handle this in the plotting routine
        bsr     do_plot_left_clip
        bra     done_clip

plot_right_clip:
                ; Right edge clipping: use reduced width
                ; new_width is in d6
        movea.l base(a6),a0                     ; get screen base
        movea.l bitmap(a6),a1                   ; get bitmap data
        move.w  row(a6),d0
        move.w  col(a6),d1
        move.w  height(a6),d2
        move.w  d6,d3                           ; use new_width from check_bounds
        move.w  width(a6),d4                    ; original bitmap width
        bsr     do_plot
        bra     done_clip

done_clip:
        movem.l (sp)+,d0-d7/a0-a5
        unlk    a6
        rts

;----------------------------------------------------------------
; Internal subroutine: do_plot
; Plots a bitmap with given parameters (no clipping on right edge)
; Input:
;   a0 = screen base
;   a1 = bitmap data
;   d0 = row
;   d1 = col
;   d2 = height
;   d3 = clipped width (in pixels, may be less than original)
;   d4 = original bitmap width (in pixels)
;----------------------------------------------------------------
do_plot:
                ; Calculate screen position
        mulu.w  #80,d0                          ; row * 80 bytes
        ext.l   d0
        adda.l  d0,a0                           ; add row offset
                
        move.w  d1,d0
        lsr.w   #3,d0                           ; col / 8
        ext.l   d0
        adda.l  d0,a0                           ; add col offset
                
                ; Calculate bytes per row of bitmap (original width)
        move.w  d4,d5                           ; original width in pixels
        addq.w  #7,d5                           ; round up
        lsr.w   #3,d5                           ; original bytes per row
                
                ; Calculate bytes to copy (clipped width)
        move.w  d3,d6                           ; clipped width in pixels
        addq.w  #7,d6                           ; round up
        lsr.w   #3,d6                           ; clipped bytes to copy
                
                ; Plot bitmap row by row
        subq.w  #1,d2                           ; height - 1 for dbra
plot_row_loop:
        move.w  d6,d7                           ; bytes to copy this row
        subq.w  #1,d7                           ; adjust for dbra
        movea.l a0,a2                           ; save screen position
                
plot_byte_loop:
        move.b  (a1)+,d1                        ; get bitmap byte
        or.b    d1,(a2)+                        ; OR onto screen
        dbra    d7,plot_byte_loop
                
                ; Advance bitmap pointer to next row (skip remaining bytes)
        move.w  d5,d1                           ; original bytes per row
        sub.w   d6,d1                           ; bytes not copied
        ext.l   d1
        adda.l  d1,a1                           ; skip to next row
                
        adda.w  #80,a0                          ; next screen row
        dbra    d2,plot_row_loop
                
        rts

;----------------------------------------------------------------
; Internal subroutine: do_plot_left_clip
; Plots a bitmap with left edge clipping
; Input:
;   a0 = screen base
;   a1 = bitmap data
;   d0 = row
;   d1 = col (should be 0)
;   d2 = height
;   d3 = clipped width (in pixels)
;   d4 = original bitmap width (in pixels)
;   d5 = pixels to skip from left
;----------------------------------------------------------------
do_plot_left_clip:
                ; Calculate screen position (col = 0)
        mulu.w  #80,d0                          ; row * 80 bytes
        ext.l   d0
        adda.l  d0,a0                           ; add row offset
                
                ; Calculate original bitmap width in bytes
        move.w  d4,d6                           ; original width in pixels
        addq.w  #7,d6
        lsr.w   #3,d6                           ; original bytes per row
                
                ; Calculate skip offset in bytes
        move.w  d5,d7
        lsr.w   #3,d7                           ; bytes to skip = pixels_to_skip / 8
                
                ; Calculate bytes to copy per row (clipped width)
        move.w  d3,d4                           ; clipped width in pixels
        addq.w  #7,d4
        lsr.w   #3,d4                           ; bytes to copy
                
                ; Plot bitmap row by row
        subq.w  #1,d2                           ; height - 1 for dbra
plot_lclip_row:
        adda.w  d7,a1                           ; skip left bytes
        move.w  d4,d1                           ; bytes to copy this row
        subq.w  #1,d1                           ; adjust for dbra
        movea.l a0,a2                           ; save screen position
                
plot_lclip_byte:
        move.b  (a1)+,d0                        ; get bitmap byte
        or.b    d0,(a2)+                        ; OR onto screen
        dbra    d1,plot_lclip_byte
                
                ; Move to next row in original bitmap (skip remaining bytes)
        move.w  d6,d1                           ; original bytes per row
        sub.w   d4,d1                           ; bytes already copied
        sub.w   d7,d1                           ; bytes skipped at start
        ext.l   d1
        adda.l  d1,a1                           ; advance to next row
                
        adda.w  #80,a0                          ; next screen row
        dbra    d2,plot_lclip_row
                
        rts
