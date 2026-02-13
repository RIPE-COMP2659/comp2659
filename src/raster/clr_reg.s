;
; void clear_region(UINT32 *base, UINT16 row, UINT16 col, UINT16 length, UINT16 width);
;
; Clear a rectangular regin of the screen.
; The length of this routine is due to an optimization which aims to
; break down the requested width into chunks that can be cleared with a single movem instruction.
;
;                                   Breaks the section into WORDS (16-bit)
;
;  -------------------------------------------------------------------------------------------------------
;  | STATUS: Assembles without errors. Otherwise untested, as linking to C code needs to be figured out. |
;  -------------------------------------------------------------------------------------------------------

                xdef            _clear_region

base            equ             64              ; offset from SP, not A6
row             equ             68               
col             equ             70             
length          equ             72
width           equ             74


_clear_region:  movem.l         d0-d7/a0-a6,-(sp)

                movea.l         base(sp),a0
                adda.l          col(sp),a0

                move.w          row(sp),d0
                mulu.w          80,d0           ; screen width in bytes
                adda.l          d0,a0

                move.w          length(sp),d1   ; d1 = number of rows (length)
                move.w          width(sp),d2    ; d2 = width in bytes
                
                ; Calculate how to break down width:
                ; num_words = width / 2
                ; num_bytes = width % 2
                
                move.w          d2,d3           ; d3 = width
                lsr.w           #1,d3           ; d3 = width / 2 = num_words
                
                move.w          d2,d6           ; d6 = width
                andi.w          #1,d6           ; d6 = width % 2 = num_bytes
                
                ; Check if we can fit in one movem (max 13 words = 26 bytes)
                cmpi.w          #13,d3          ; can we fit all words in registers?
                ble             fits_in_registers
                
large_width:
                ; Width > 26 bytes: use multiple movem operations per row
                lea             zeros_13,a1
                movem   (a1),d1-d7/a1-a6  ; load 13 words of zeros
                
                move.w          length(sp),d0   ; d0 = row counter
                subq.w          #1,d0           ; adjust for dbra
                move.w          d0,-(sp)        ; save on stack
                
row_loop_large:  
                move.l          a0,-(sp)        ; save row start
                move.w          80(sp),d0       ; reload width (adjusted for stack)
                
chunk_loop:     
                cmpi.w          #26,d0          ; can write full 26-byte chunk?
                blt             handle_remainder
                
                movem   d1-d7/a1-a6,(a0)  ; write 26 bytes
                lea             26(a0),a0       
                subi.w          #26,d0          
                bra             chunk_loop
                
handle_remainder:
                bra             rem_check_word
                
rem_do_word:
                move.w          d1,(a0)+        
                subi.w          #2,d0
                
rem_check_word:
                cmpi.w          #2,d0
                bge             rem_do_word
                
rem_check_byte:
                tst.w           d0
                beq             row_done_large
                move.b          d1,(a0)+        
                
row_done_large:
                movea.l         (sp)+,a0        ; restore row start
                adda.w          #80,a0          ; next row
                move.w          (sp),d0         ; reload counter
                dbra            d0,row_loop_large
                
                addq.w          #2,sp           ; pop counter
                bra             exit_clear
                
fits_in_registers:
                ; Width <= 26 bytes: use single movem per row with exact register count
                lea             zeros_13,a1
                
                ; Load zeros into required number of registers
                ; We'll load all 13 and use only what we need in movem
                movem   (a1),d1-d7/a1-a6
                
                ; Calculate movem register mask based on num_words in d3
                ; d3 = 0: no movem needed
                ; d3 = 1-7: use d1-dX
                ; d3 = 8-13: use d1-d7/a1-aX
                
                move.w          length(sp),d0   ; d0 = row counter  
                subq.w          #1,d0           ; adjust for dbra
                
                tst.w           d3              ; any words to write?
                beq             no_movem_needed
                
                ; Determine which movem instruction to use
                cmpi.w          #1,d3
                beq             use_1_reg
                cmpi.w          #2,d3
                beq             use_2_reg
                cmpi.w          #3,d3
                beq             use_3_reg
                cmpi.w          #4,d3
                beq             use_4_reg
                cmpi.w          #5,d3
                beq             use_5_reg
                cmpi.w          #6,d3
                beq             use_6_reg
                cmpi.w          #7,d3
                beq             use_7_reg
                cmpi.w          #8,d3
                beq             use_8_reg
                cmpi.w          #9,d3
                beq             use_9_reg
                cmpi.w          #10,d3
                beq             use_10_reg
                cmpi.w          #11,d3
                beq             use_11_reg
                cmpi.w          #12,d3
                beq             use_12_reg
                bra             use_13_reg      ; d3 = 13
                
use_1_reg:
row_loop_1:     move.w          d1,(a0)
                lea             2(a0),a0
                tst.w           d6
                beq             row_done_1
                move.b          d1,(a0)+
row_done_1:     suba.l          d2,a0
                adda.w          #80,a0
                dbra            d0,row_loop_1
                bra             exit_clear
                
use_2_reg:
row_loop_2:     movem   d1-d2,(a0)
                lea             4(a0),a0
                tst.w           d6
                beq             row_done_2
                move.b          d1,(a0)+
row_done_2:     suba.l          d2,a0
                adda.w          #80,a0
                dbra            d0,row_loop_2
                bra             exit_clear
                
use_3_reg:
row_loop_3:     movem   d1-d3,(a0)
                lea             6(a0),a0
                tst.w           d6
                beq             row_done_3
                move.b          d1,(a0)+
row_done_3:     suba.l          d2,a0
                adda.w          #80,a0
                dbra            d0,row_loop_3
                bra             exit_clear
                
use_4_reg:
row_loop_4:     movem   d1-d4,(a0)
                lea             8(a0),a0
                tst.w           d6
                beq             row_done_4
                move.b          d1,(a0)+
row_done_4:     suba.l          d2,a0
                adda.w          #80,a0
                dbra            d0,row_loop_4
                bra             exit_clear
                
use_5_reg:
row_loop_5:     movem   d1-d5,(a0)
                lea             10(a0),a0
                tst.w           d6
                beq             row_done_5
                move.b          d1,(a0)+
row_done_5:     suba.l          d2,a0
                adda.w          #80,a0
                dbra            d0,row_loop_5
                bra             exit_clear
                
use_6_reg:
row_loop_6:     movem   d1-d6,(a0)
                lea             12(a0),a0
                tst.w           d6
                beq             row_done_6
                move.b          d1,(a0)+
row_done_6:     suba.l          d2,a0
                adda.w          #80,a0
                dbra            d0,row_loop_6
                bra             exit_clear
                
use_7_reg:
row_loop_7:     movem   d1-d7,(a0)
                lea             14(a0),a0
                tst.w           d6
                beq             row_done_7
                move.b          d1,(a0)+
row_done_7:     suba.l          d2,a0
                adda.w          #80,a0
                dbra            d0,row_loop_7
                bra             exit_clear
                
use_8_reg:
row_loop_8:     movem   d1-d7/a1,(a0)
                lea             16(a0),a0
                tst.w           d6
                beq             row_done_8
                move.b          d1,(a0)+
row_done_8:     suba.l          d2,a0
                adda.w          #80,a0
                dbra            d0,row_loop_8
                bra             exit_clear
                
use_9_reg:
row_loop_9:     movem   d1-d7/a1-a2,(a0)
                lea             18(a0),a0
                tst.w           d6
                beq             row_done_9
                move.b          d1,(a0)+
row_done_9:     suba.l          d2,a0
                adda.w          #80,a0
                dbra            d0,row_loop_9
                bra             exit_clear
                
use_10_reg:
row_loop_10:    movem   d1-d7/a1-a3,(a0)
                lea             20(a0),a0
                tst.w           d6
                beq             row_done_10
                move.b          d1,(a0)+
row_done_10:    suba.l          d2,a0
                adda.w          #80,a0
                dbra            d0,row_loop_10
                bra             exit_clear
                
use_11_reg:
row_loop_11:    movem   d1-d7/a1-a4,(a0)
                lea             22(a0),a0
                tst.w           d6
                beq             row_done_11
                move.b          d1,(a0)+
row_done_11:    suba.l          d2,a0
                adda.w          #80,a0
                dbra            d0,row_loop_11
                bra             exit_clear
                
use_12_reg:
row_loop_12:    movem   d1-d7/a1-a5,(a0)
                lea             24(a0),a0
                tst.w           d6
                beq             row_done_12
                move.b          d1,(a0)+
row_done_12:    suba.l          d2,a0
                adda.w          #80,a0
                dbra            d0,row_loop_12
                bra             exit_clear
                
use_13_reg:
row_loop_13:    movem   d1-d7/a1-a6,(a0)
                lea             26(a0),a0
                tst.w           d6
                beq             row_done_13
                move.b          d1,(a0)+
row_done_13:    suba.l          d2,a0
                adda.w          #80,a0
                dbra            d0,row_loop_13
                bra             exit_clear

no_movem_needed:
                ; Only byte to write
row_loop_tiny:  
                tst.w           d6
                beq             row_done_tiny
                move.b          d1,(a0)+
                
row_done_tiny:
                suba.l          d2,a0
                adda.w          #80,a0
                dbra            d0,row_loop_tiny
                bra             exit_clear

exit_clear:
                movem.l         (sp)+,d0-d7/a0-a6
                rts


zeros_13:            ds.w            13
