;
; PURPOSE: Plots a line on the screen between the two given points.
;
; INPUT: Address(UINT32*): to the start of the screen
;        Position(start_row,start_col): the coordinates of the start of the line
;        Position(end_row,end_col): the coordinates of the end of the line
;        Color: color of the line (0 = black, 1 = white)
;
; OUTPUT: None
;
; void plot_line(UINT32 *base, UINT16 start_row, UINT16 start_col, UINT16 end_row, UINT16 end_col, UINT16 color);
;________________________________________________________________
; Implements Bresenham's line drawing algorithm
;
;                                   --- Using the routine is not recommended. ---
;
; Source: https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
;
; Contact @sudonym-i if you will need to use this routine.

                xdef            _plot_line

base            equ             40              
start_row       equ             44              
start_col       equ             46              
end_row         equ             48              
end_col         equ             50              
color           equ             52             


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
                
                ; Check color for vertical-ish line
                move.w          color(sp),d0
                tst.w           d0
                beq             vert_line_black
                
                ; Vertical-ish line (dy > dx) - WHITE
                ; error = dx - dy
                move.w          d6,d1
                sub.w           d7,d1                   ; d1 = error
                
vert_loop_white:
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
                
                ; Set the bit (white)
                bset            d0,(a0)
                
                ; Check if we've reached the end
                cmp.w           end_row(sp),d4
                beq             line_done
                
                ; Update error and position
                tst.w           d1
                bmi.s           vert_no_x_step_white
                
                ; error >= 0: step in x direction
                add.w           d2,d5                   ; current_col += step_x
                move.w          d7,d0
                lsl.w           #1,d0
                sub.w           d0,d1                   ; error -= 2*dy
                
vert_no_x_step_white:
                add.w           d3,d4                   ; current_row += step_y
                move.w          d6,d0
                lsl.w           #1,d0
                add.w           d0,d1                   ; error += 2*dx
                bra             vert_loop_white
                
vert_line_black:
                ; Vertical-ish line (dy > dx) - BLACK
                ; error = dx - dy
                move.w          d6,d1
                sub.w           d7,d1                   ; d1 = error
                
vert_loop_black:
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
                
                ; Clear the bit (black)
                bclr            d0,(a0)
                
                ; Check if we've reached the end
                cmp.w           end_row(sp),d4
                beq             line_done
                
                ; Update error and position
                tst.w           d1
                bmi.s           vert_no_x_step_black
                
                ; error >= 0: step in x direction
                add.w           d2,d5                   ; current_col += step_x
                move.w          d7,d0
                lsl.w           #1,d0
                sub.w           d0,d1                   ; error -= 2*dy
                
vert_no_x_step_black:
                add.w           d3,d4                   ; current_row += step_y
                move.w          d6,d0
                lsl.w           #1,d0
                add.w           d0,d1                   ; error += 2*dx
                bra             vert_loop_black
                
horiz_line:
                ; Check color for horizontal-ish line
                move.w          color(sp),d0
                tst.w           d0
                beq             horiz_line_black
                
                ; Horizontal-ish line (dx >= dy) - WHITE
                ; error = dy - dx
                move.w          d7,d1
                sub.w           d6,d1                   ; d1 = error
                
horiz_loop_white:
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
                
                ; Set the bit (white)
                bset            d0,(a0)
                
                ; Check if we've reached the end
                cmp.w           end_col(sp),d5
                beq.s           line_done
                
                ; Update error and position
                tst.w           d1
                bmi.s           horiz_no_y_step_white
                
                ; error >= 0: step in y direction
                add.w           d3,d4                   ; current_row += step_y
                move.w          d6,d0
                lsl.w           #1,d0
                sub.w           d0,d1                   ; error -= 2*dx
                
horiz_no_y_step_white:
                add.w           d2,d5                   ; current_col += step_x
                move.w          d7,d0
                lsl.w           #1,d0
                add.w           d0,d1                   ; error += 2*dy
                bra.s           horiz_loop_white
                
horiz_line_black:
                ; Horizontal-ish line (dx >= dy) - BLACK
                ; error = dy - dx
                move.w          d7,d1
                sub.w           d6,d1                   ; d1 = error
                
horiz_loop_black:
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
                
                ; Clear the bit (black)
                bclr            d0,(a0)
                
                ; Check if we've reached the end
                cmp.w           end_col(sp),d5
                beq.s           line_done
                
                ; Update error and position
                tst.w           d1
                bmi.s           horiz_no_y_step_black
                
                ; error >= 0: step in y direction
                add.w           d3,d4                   ; current_row += step_y
                move.w          d6,d0
                lsl.w           #1,d0
                sub.w           d0,d1                   ; error -= 2*dx
                
horiz_no_y_step_black:
                add.w           d2,d5                   ; current_col += step_x
                move.w          d7,d0
                lsl.w           #1,d0
                add.w           d0,d1                   ; error += 2*dy
                bra.s           horiz_loop_black
                
line_done:
                movem.l         (sp)+,d0-d7/a0
                rts
