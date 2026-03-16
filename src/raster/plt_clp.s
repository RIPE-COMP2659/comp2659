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

evaluate_clip_status:
                ; Get status and new_width from parameters (already computed by caller)
        move.w  status(a6),d7                   ; get status in d7
        move.w  new_width(a6),d6                ; get new_width in d6

        cmpi.b  #1,d7
        beq     plot_left_clip                  ; if left edge clipping

        cmpi.b  #2,d7
        beq     plot_right_clip                 ; if right edge clipping

        bra     done_clip                       ; safety - should never reach here

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
                ; This will be handled in the plotting routine
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

                ; Extract sub-byte bit shift before computing byte offset
        move.w  d1,d5
        andi.w  #7,d5                           ; d5 = col % 8 (bit shift)

        move.w  d1,d0
        lsr.w   #3,d0                           ; col / 8
        ext.l   d0
        adda.l  d0,a0                           ; add col byte offset

        tst.w   d5
        bne     do_plot_shifted                 ; non-byte-aligned: use shifted path

calc_plot_bytes:
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
        movea.l a0,a2                           ; save screen position
        bra.s   plot_byte_entry                 ; enter loop at check
plot_byte_loop:
        move.b  (a1)+,d1                        ; get bitmap byte
        or.b    d1,(a2)+                        ; OR onto screen
plot_byte_entry:
        dbra    d7,plot_byte_loop               ; decrement and branch if not -1
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
                
calc_lclip_bytes:
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

                ; If skipped pixels are not byte-aligned, use shifted left-clip path
        move.w  d5,d0
        andi.w  #7,d0                           ; abs(col) % 8
        bne     do_plot_left_clip_shifted
                
                ; Plot bitmap row by row
        subq.w  #1,d2                           ; height - 1 for dbra
plot_lclip_row:
        movea.l a0,a2                           ; save screen position
        move.w  d4,d1                           ; bytes to copy this row
        adda.w  d7,a1                           ; skip left bytes
        bra.s   plot_lclip_entry                ; enter loop at check
plot_lclip_byte:
        move.b  (a1)+,d0                        ; get bitmap byte
        or.b    d0,(a2)+                        ; OR onto screen
plot_lclip_entry:
        dbra    d1,plot_lclip_byte              ; decrement and branch if not -1
                ; Move to next row in original bitmap (skip remaining bytes)
        move.w  d6,d1                           ; original bytes per row
        sub.w   d4,d1                           ; bytes already copied
        sub.w   d7,d1                           ; bytes skipped at start
        ext.l   d1
        adda.l  d1,a1                           ; advance to next row
                
        adda.w  #80,a0                          ; next screen row
        dbra    d2,plot_lclip_row
                
        rts

;----------------------------------------------------------------
; Internal subroutine: do_plot_left_clip_shifted
; Plots a left-clipped bitmap when abs(col) is not byte-aligned.
; Input:
;   a0 = screen base + row*80 (col is already clipped to 0)
;   a1 = bitmap row start
;   d2 = height
;   d4 = clipped bytes per row
;   d5 = pixels to skip from left
;   d6 = original bytes per row
;   d7 = bytes to skip from left
;----------------------------------------------------------------
do_plot_left_clip_shifted:
        move.w  d3,d0
        andi.w  #7,d0                           ; trailing visible bits in last byte (0..7)
        movea.w d0,a3                           ; keep for per-row tail-byte masking

        move.w  d5,d0
        andi.w  #7,d0                           ; d0 = bit shift (1..7)
        move.w  d0,d5                           ; d5 = shift
        moveq   #8,d3
        sub.w   d5,d3                           ; d3 = 8 - shift

        subq.w  #1,d2                           ; height - 1 for dbra

plot_lclip_shift_row:
        movea.l a0,a2                           ; destination row pointer
        movea.l a1,a4                           ; source row base
        adda.w  d7,a4                           ; source row after byte skip
        movea.l a1,a5                           ; source row base copy
        adda.w  d6,a5                           ; source row end (one-past-last byte)

                ; Save original last destination byte if partial tail exists
        move.w  a3,d0
        beq.s   plot_lclip_no_tail_save
        move.w  d4,d1
        subq.w  #1,d1
        move.b  0(a2,d1.w),d0
        move.b  d0,-(sp)
plot_lclip_no_tail_save:

        move.w  d4,d0                           ; bytes to output this row
        subq.w  #1,d0                           ; dbra counter (inclusive)
        blt.s   plot_lclip_shift_advance

plot_lclip_shift_byte:
        move.w  d0,-(sp)                        ; save byte-loop counter

                ; curr byte (or 0 if out of source bytes)
        moveq   #0,d1
        cmpa.l  a5,a4
        bge.s   plot_lclip_curr_done
        move.b  (a4)+,d1
plot_lclip_curr_done:

                ; next byte (or 0), then combine
        lsl.w   d5,d1                           ; curr << shift
        moveq   #0,d0
        cmpa.l  a5,a4
        bge.s   plot_lclip_next_done
        move.b  (a4),d0
        lsr.w   d3,d0                           ; next >> (8 - shift)
plot_lclip_next_done:
        or.b    d0,d1
        move.b  d1,(a2)+                        ; write composed destination byte

        move.w  (sp)+,d0                        ; restore byte-loop counter
        dbra    d0,plot_lclip_shift_byte

plot_lclip_shift_advance:
                ; Restore non-sprite low bits in last destination byte for partial tails
        move.w  a3,d0
        beq.s   plot_lclip_tail_done
        move.l  #$ff,d1
        lsr.w   d0,d1                           ; d1 low byte = preserve mask
        move.w  d1,d0
        not.w   d0                              ; d0 low byte = sprite-bit mask
        and.b   d0,-1(a2)                       ; keep only sprite bits in written byte
        moveq   #0,d0
        move.b  (sp)+,d0                        ; original destination byte
        and.b   d1,d0                           ; keep only preserved low bits
        or.b    d0,-1(a2)                       ; merge original tail bits back
plot_lclip_tail_done:

        adda.w  d6,a1                           ; next source row (original stride)
        adda.w  #80,a0                          ; next destination row
        dbra    d2,plot_lclip_shift_row

        rts

;----------------------------------------------------------------
; Internal subroutine: do_plot_shifted
; Plots a right-clipped bitmap with a non-byte-aligned column.
; Branches here from do_plot when col % 8 != 0.
; Input:
;   a0 = screen base + row*80 + col/8  (byte-aligned start address)
;   a1 = bitmap data
;   d2 = height
;   d3 = clipped width (in pixels)
;   d4 = original bitmap width (in pixels)
;   d5 = bit shift (col % 8, range 1-7)
;----------------------------------------------------------------
do_plot_shifted:
        moveq   #8,d6
        sub.w   d5,d6                           ; d6 = 8 - shift (complement)

                ; Calculate orig bytes per row
        move.w  d4,d0
        addq.w  #7,d0
        lsr.w   #3,d0                           ; d0 = orig bytes per row

                ; Calculate clipped bytes per row
        move.w  d3,d1
        addq.w  #7,d1
        lsr.w   #3,d1                           ; d1 = clipped bytes per row

                ; Compute skip = orig - clipped (for bitmap row advance)
        sub.w   d1,d0
        ext.l   d0
        move.l  d0,d7                           ; d7 = bytes to skip after each row
        move.w  d1,d4                           ; d4 = clipped bytes per row

        subq.w  #1,d2                           ; height - 1 for dbra

shifted_do_plot_row:
        movea.l a0,a2                           ; a2 = dest ptr for this row

                ; Mask first dest byte: preserve top `shift` bits, clear sprite bits
        moveq   #-1,d0
        lsl.w   d6,d0                           ; d0 low byte = 0xFF << (8-shift)
        and.b   d0,(a2)

                ; First source byte: shift and OR into first dest byte, compute carry
        moveq   #0,d0
        move.b  (a1)+,d0                        ; d0 = first source byte (zero-extended)
        move.w  d0,d1                           ; d1 = source copy for carry
        lsr.w   d5,d0                           ; d0 low byte = source >> shift
        or.b    d0,(a2)+                        ; OR into first dest byte, advance ptr
        lsl.w   d6,d1                           ; d1 low byte = carry = source << (8-shift)
        move.w  d1,d3                           ; d3 = carry for next dest byte

                ; Process remaining source bytes
        move.w  d4,d1                           ; d1 = total clipped bytes
        subq.w  #2,d1                           ; dbra is inclusive; remaining count is (d4 - 1)
        blt.s   shifted_do_plot_advance         ; if no remaining bytes, skip inner loop

shifted_do_plot_byte_loop:
        move.w  d1,-(sp)                        ; save loop counter
        moveq   #0,d0
        move.b  (a1)+,d0                        ; d0 = next source byte (zero-extended)
        move.w  d0,d1                           ; d1 = source copy for new carry
        lsr.w   d5,d0                           ; d0 = source >> shift
        or.b    d3,d0                           ; combine: old carry | shifted source
        move.b  d0,(a2)+                        ; write dest byte
        lsl.w   d6,d1                           ; d1 = new carry in low byte
        move.w  d1,d3                           ; d3 = new carry
        move.w  (sp)+,d1                        ; restore loop counter
        dbra    d1,shifted_do_plot_byte_loop

shifted_do_plot_advance:
        adda.l  d7,a1                           ; skip remaining bitmap bytes this row
        adda.w  #80,a0                          ; advance screen to next row
        dbra    d2,shifted_do_plot_row

        rts

