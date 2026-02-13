
                xdef            _clear_region

base            equ             64              ; offset from SP, not A6
row             equ             68               
col             equ             70             
length          equ             72
width           equ             74

_clear_region:  movem.l         d0-7/a0-6,-(sp)

                movea.l         base(sp),a0
                adda.l          col(sp),a0

                move.w          row(sp),d0
                mulu.w          80,d0           ; screen width in bytes
                adda.l          d0,a0

                move.w          length(sp),d1   ; d1 = number of rows (length)
                move.w          width(sp),d2    ; d2 = width in bytes
                
                ; Calculate how to break down width:
                ; num_longs = width / 4
                ; num_words = (width % 4) / 2
                ; num_bytes = width % 2
                
                move.w          d2,d3           ; d3 = width
                lsr.w           #2,d3           ; d3 = width / 4 = num_longs
                
                move.w          d2,d4           ; d4 = width
                andi.w          #3,d4           ; d4 = width % 4
                move.w          d4,d5           ; d5 = width % 4
                lsr.w           #1,d5           ; d5 = (width % 4) / 2 = num_words
                
                move.w          d2,d6           ; d6 = width
                andi.w          #1,d6           ; d6 = width % 2 = num_bytes
                
                ; Check if we can fit in one movem (max 13 longs = 52 bytes)
                cmpi.w          #13,d3          ; can we fit all longs in registers?
                ble             fits_in_registers
                
large_width:
                ; Width > 52 bytes: use multiple movem operations per row
                lea             zeros_13,a1
                movem.l         (a1),d1-d7/a1-a6  ; load 13 longs of zeros
                
                move.w          length(sp),d0   ; d0 = row counter
                subq.w          #1,d0           ; adjust for dbra
                move.w          d0,-(sp)        ; save on stack
                
row_loop_large:  
                move.l          a0,-(sp)        ; save row start
                move.w          80(sp),d0       ; reload width (adjusted for stack)
                
chunk_loop:     
                cmpi.w          #52,d0          ; can write full 52-byte chunk?
                blt             handle_remainder
                
                movem.l         d1-d7/a1-a6,(a0)  ; write 52 bytes
                lea             52(a0),a0       
                subi.w          #52,d0          
                bra             chunk_loop
                
handle_remainder:
                bra             rem_check_long
                
rem_do_long:
                move.l          d1,(a0)+        
                subi.w          #4,d0
                
rem_check_long:
                cmpi.w          #4,d0
                bge             rem_do_long
                
                cmpi.w          #2,d0
                blt             rem_check_byte
                move.w          d1,(a0)+        
                subi.w          #2,d0
                
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
                ; Width <= 52 bytes: use single movem per row with exact register count
                lea             zeros_13,a1
                
                ; Load zeros into required number of registers
                ; We'll load all 13 and use only what we need in movem
                movem.l         (a1),d1-d7/a1-a6
                
                ; Calculate movem register mask based on num_longs in d3
                ; d3 = 0: no movem needed
                ; d3 = 1-7: use d1-dX
                ; d3 = 8-13: use d1-d7/a1-aX
                
                move.w          length(sp),d0   ; d0 = row counter  
                subq.w          #1,d0           ; adjust for dbra
                
                tst.w           d3              ; any longs to write?
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
row_loop_1:     move.l          d1,(a0)
                lea             4(a0),a0
                tst.w           d5
                beq             check_bytes_1
                move.w          d1,(a0)+
check_bytes_1:  tst.w           d6
                beq             row_done_1
                move.b          d1,(a0)+
row_done_1:     suba.l          d2,a0
                adda.w          #80,a0
                dbra            d0,row_loop_1
                bra             exit_clear
                
use_2_reg:
row_loop_2:     movem.l         d1-d2,(a0)
                lea             8(a0),a0
                tst.w           d5
                beq             check_bytes_2
                move.w          d1,(a0)+
check_bytes_2:  tst.w           d6
                beq             row_done_2
                move.b          d1,(a0)+
row_done_2:     suba.l          d2,a0
                adda.w          #80,a0
                dbra            d0,row_loop_2
                bra             exit_clear
                
use_3_reg:
row_loop_3:     movem.l         d1-d3,(a0)
                lea             12(a0),a0
                tst.w           d5
                beq             check_bytes_3
                move.w          d1,(a0)+
check_bytes_3:  tst.w           d6
                beq             row_done_3
                move.b          d1,(a0)+
row_done_3:     suba.l          d2,a0
                adda.w          #80,a0
                dbra            d0,row_loop_3
                bra             exit_clear
                
use_4_reg:
row_loop_4:     movem.l         d1-d4,(a0)
                lea             16(a0),a0
                tst.w           d5
                beq             check_bytes_4
                move.w          d1,(a0)+
check_bytes_4:  tst.w           d6
                beq             row_done_4
                move.b          d1,(a0)+
row_done_4:     suba.l          d2,a0
                adda.w          #80,a0
                dbra            d0,row_loop_4
                bra             exit_clear
                
use_5_reg:
row_loop_5:     movem.l         d1-d5,(a0)
                lea             20(a0),a0
                tst.w           d5
                beq             check_bytes_5
                move.w          d1,(a0)+
check_bytes_5:  tst.w           d6
                beq             row_done_5
                move.b          d1,(a0)+
row_done_5:     suba.l          d2,a0
                adda.w          #80,a0
                dbra            d0,row_loop_5
                bra             exit_clear
                
use_6_reg:
row_loop_6:     movem.l         d1-d6,(a0)
                lea             24(a0),a0
                tst.w           d5
                beq             check_bytes_6
                move.w          d1,(a0)+
check_bytes_6:  tst.w           d6
                beq             row_done_6
                move.b          d1,(a0)+
row_done_6:     suba.l          d2,a0
                adda.w          #80,a0
                dbra            d0,row_loop_6
                bra             exit_clear
                
use_7_reg:
row_loop_7:     movem.l         d1-d7,(a0)
                lea             28(a0),a0
                tst.w           d5
                beq             check_bytes_7
                move.w          d1,(a0)+
check_bytes_7:  tst.w           d6
                beq             row_done_7
                move.b          d1,(a0)+
row_done_7:     suba.l          d2,a0
                adda.w          #80,a0
                dbra            d0,row_loop_7
                bra             exit_clear
                
use_8_reg:
row_loop_8:     movem.l         d1-d7/a1,(a0)
                lea             32(a0),a0
                tst.w           d5
                beq             check_bytes_8
                move.w          d1,(a0)+
check_bytes_8:  tst.w           d6
                beq             row_done_8
                move.b          d1,(a0)+
row_done_8:     suba.l          d2,a0
                adda.w          #80,a0
                dbra            d0,row_loop_8
                bra             exit_clear
                
use_9_reg:
row_loop_9:     movem.l         d1-d7/a1-a2,(a0)
                lea             36(a0),a0
                tst.w           d5
                beq             check_bytes_9
                move.w          d1,(a0)+
check_bytes_9:  tst.w           d6
                beq             row_done_9
                move.b          d1,(a0)+
row_done_9:     suba.l          d2,a0
                adda.w          #80,a0
                dbra            d0,row_loop_9
                bra             exit_clear
                
use_10_reg:
row_loop_10:    movem.l         d1-d7/a1-a3,(a0)
                lea             40(a0),a0
                tst.w           d5
                beq             check_bytes_10
                move.w          d1,(a0)+
check_bytes_10: tst.w           d6
                beq             row_done_10
                move.b          d1,(a0)+
row_done_10:    suba.l          d2,a0
                adda.w          #80,a0
                dbra            d0,row_loop_10
                bra             exit_clear
                
use_11_reg:
row_loop_11:    movem.l         d1-d7/a1-a4,(a0)
                lea             44(a0),a0
                tst.w           d5
                beq             check_bytes_11
                move.w          d1,(a0)+
check_bytes_11: tst.w           d6
                beq             row_done_11
                move.b          d1,(a0)+
row_done_11:    suba.l          d2,a0
                adda.w          #80,a0
                dbra            d0,row_loop_11
                bra             exit_clear
                
use_12_reg:
row_loop_12:    movem.l         d1-d7/a1-a5,(a0)
                lea             48(a0),a0
                tst.w           d5
                beq             check_bytes_12
                move.w          d1,(a0)+
check_bytes_12: tst.w           d6
                beq             row_done_12
                move.b          d1,(a0)+
row_done_12:    suba.l          d2,a0
                adda.w          #80,a0
                dbra            d0,row_loop_12
                bra             exit_clear
                
use_13_reg:
row_loop_13:    movem.l         d1-d7/a1-a6,(a0)
                lea             52(a0),a0
                tst.w           d5
                beq             check_bytes_13
                move.w          d1,(a0)+
check_bytes_13: tst.w           d6
                beq             row_done_13
                move.b          d1,(a0)+
row_done_13:    suba.l          d2,a0
                adda.w          #80,a0
                dbra            d0,row_loop_13
                bra             exit_clear

no_movem_needed:
                ; Only words and/or bytes to write
row_loop_tiny:  
                tst.w           d5
                beq             check_bytes_tiny
                move.w          d1,(a0)+
                
check_bytes_tiny:
                tst.w           d6
                beq             row_done_tiny
                move.b          d1,(a0)+
                
row_done_tiny:
                suba.l          d2,a0
                adda.w          #80,a0
                dbra            d0,row_loop_tiny
                bra             exit_clear

exit_clear:
                movem.l         (sp)+,d0-7/a0-6
                rts


zeros_13:            ds.l            13