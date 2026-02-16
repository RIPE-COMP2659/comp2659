;
; PURPOSE: Plots a line on the screen between the two given points.
;
; INPUT: Address(UINT32*): to the start of the screen
;        Position(start_row,start_col): the coordinates of the start of the line
;        Position(end_row,end_col): the coordinates of the end of the line
;
; OUTPUT: None
;
; void plot_line(UINT32 *base, UINT16 start_row, UINT16 start_col, UINT16 end_row, UINT16 end_col);
;________________________________________________________________
; Implements Bresenham's line drawing algorithm

                xdef            _plot_line

base            equ             40              ; UINT32* (4 bytes)
start_row       equ             44              ; UINT16 (2 bytes)
start_col       equ             46              ; UINT16 (2 bytes)
end_row         equ             48              ; UINT16 (2 bytes)
end_col         equ             50              ; UINT16 (2 bytes)


_plot_line:
                movem.l         d0-d7/a0,-(sp)
                
                ; Initialize current position
                move.w          start_row(sp),d4       ; current_row
                move.w          start_col(sp),d5       ; current_col
                
                ; Calculate dx = abs(end_col - start_col)
                move.w          end_col(sp),d0
                move.w          start_col(sp),d1
                sub.w           d1,d0                   ; d0 = end_col - start_col
                bge.s           dx_positive
                neg.w           d0                      ; d0 = abs(dx)
dx_positive:
                move.w          d0,d6                   ; d6 = dx (preserved)
                
                ; Calculate dy = abs(end_row - start_row)
                move.w          end_row(sp),d0
                move.w          start_row(sp),d1
                sub.w           d1,d0                   ; d0 = end_row - start_row
                bge.s           dy_positive
                neg.w           d0                      ; d0 = abs(dy)
dy_positive:
                move.w          d0,d7                   ; d7 = dy (preserved)
                
                ; Determine step direction for x (col)
                move.w          end_col(sp),d0
                cmp.w           start_col(sp),d0
                bge.s           step_x_positive
                moveq           #-1,d2                  ; step_x = -1
                bra.s           check_y_step
step_x_positive:
                moveq           #1,d2                   ; step_x = 1
                
check_y_step:
                ; Determine step direction for y (row)
                move.w          end_row(sp),d0
                cmp.w           start_row(sp),d0
                bge.s           step_y_positive
                moveq           #-1,d3                  ; step_y = -1
                bra.s           init_bresenham
step_y_positive:
                moveq           #1,d3                   ; step_y = 1
                
init_bresenham:
                ; Register allocation:
                ; d1 = error
                ; d2 = step_x, d3 = step_y
                ; d4 = current_row, d5 = current_col
                ; d6 = dx (preserved), d7 = dy (preserved)
                
                ; Determine if line is more horizontal or vertical
                cmp.w           d7,d6
                bge             horiz_line              ; if dx >= dy, horizontal-ish
                
                ; Vertical-ish line (dy > dx)
                ; error = dx - dy
                move.w          d6,d1
                sub.w           d7,d1                   ; d1 = error
                
vert_loop:
                ; Plot pixel at (d4, d5)
                movea.l         base(sp),a0
                
                ; Calculate row offset: row * 80
                move.w          d4,d0
                mulu.w          #80,d0
                adda.l          d0,a0
                
                ; Calculate col byte offset: col / 8
                move.w          d5,d0
                lsr.w           #3,d0
                adda.w          d0,a0
                
                ; Calculate bit position: 7 - (col % 8)
                move.w          d5,d0
                andi.w          #7,d0
                neg.w           d0
                addi.w          #7,d0                   ; d0 = 7 - (col % 8)
                
                ; Set the bit
                bset            d0,(a0)
                
                ; Check if we've reached the end
                cmp.w           end_row(sp),d4
                beq             line_done
                
                ; Update error and position
                tst.w           d1
                bmi.s           vert_no_x_step
                
                ; error >= 0: step in x direction
                add.w           d2,d5                   ; current_col += step_x
                move.w          d7,d0
                lsl.w           #1,d0
                sub.w           d0,d1                   ; error -= 2*dy
                
vert_no_x_step:
                add.w           d3,d4                   ; current_row += step_y
                move.w          d6,d0
                lsl.w           #1,d0
                add.w           d0,d1                   ; error += 2*dx
                bra             vert_loop
                
horiz_line:
                ; Horizontal-ish line (dx >= dy)
                ; error = dy - dx
                move.w          d7,d1
                sub.w           d6,d1                   ; d1 = error
                
horiz_loop:
                ; Plot pixel at (d4, d5)
                movea.l         base(sp),a0
                
                ; Calculate row offset: row * 80
                move.w          d4,d0
                mulu.w          #80,d0
                adda.l          d0,a0
                
                ; Calculate col byte offset: col / 8
                move.w          d5,d0
                lsr.w           #3,d0
                adda.w          d0,a0
                
                ; Calculate bit position: 7 - (col % 8)
                move.w          d5,d0
                andi.w          #7,d0
                neg.w           d0
                addi.w          #7,d0                   ; d0 = 7 - (col % 8)
                
                ; Set the bit
                bset            d0,(a0)
                
                ; Check if we've reached the end
                cmp.w           end_col(sp),d5
                beq.s           line_done
                
                ; Update error and position
                tst.w           d1
                bmi.s           horiz_no_y_step
                
                ; error >= 0: step in y direction
                add.w           d3,d4                   ; current_row += step_y
                move.w          d6,d0
                lsl.w           #1,d0
                sub.w           d0,d1                   ; error -= 2*dx
                
horiz_no_y_step:
                add.w           d2,d5                   ; current_col += step_x
                move.w          d7,d0
                lsl.w           #1,d0
                add.w           d0,d1                   ; error += 2*dy
                bra.s           horiz_loop
                
line_done:
                movem.l         (sp)+,d0-d7/a0
                rts
