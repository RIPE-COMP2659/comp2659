;
; PURPOSE: Plots a triangle on the screen given by the coordinate of the 90° angle,
;          the length of the base, the length of the height, and the direction of the triangle.
;
; INPUT: Address(UINT32*): to the start of the screen
;        Position(row,col): the coordinates of the pixel of the 90° angle of the triangle
;        base_triangle: the length of the base in pixels
;        Height: the lenth (number of rows) of the height in pixels of the triangle
;        Direction: Describes where the coordinate is relative to the rest of the triangle
;              0 - Coordinate is the top left point of the triangle
;              1 - Coordinate is the top right point of the triangle
;              2 - Coordinate is the bottom left point of the triangle
;              3 - Coordinate is the bottom right point of the triangle
;
; OUTPUT: None
;
; void plot_triangle(UINT32 *base, UINT16 row, UINT16 col, UINT16 base_triangle, UINT16 height, UINT8 direction);
;________________________________________________________________
; Draws a filled right-angled triangle by drawing horizontal lines row by row.
; Uses plot_horizontal_line for each scanline.

                xdef            _plot_triangle
                xref            _plot_horizontal_line

; Stack offsets AFTER movem.l d0-d7/a0-a6,-(sp) which pushes 60 bytes
base            equ             64              ; UINT32* (4 bytes)
row             equ             68              ; UINT16 (2 bytes)
col             equ             70              ; UINT16 (2 bytes)
base_tri        equ             72              ; UINT16 (2 bytes)
height          equ             74              ; UINT16 (2 bytes)
direction       equ             76              ; UINT8 (1 byte, but padded to 2)


_plot_triangle:
                movem.l         d0-d7/a0-a6,-(sp)
                
                ; Check direction and branch
                move.b          direction(sp),d0
                andi.w          #$FF,d0                 ; clear upper byte
                cmpi.w          #0,d0
                beq             dir_top_left
                cmpi.w          #1,d0
                beq             dir_top_right
                cmpi.w          #2,d0
                beq             dir_bottom_left
                cmpi.w          #3,d0
                beq             dir_bottom_right
                bra             done                    ; invalid direction

dir_top_left:
                ; Direction 0: 90° angle at top-left
                ; Triangle expands rightward and downward
                ; Row i has line of length: (base * (i+1)) / height
                
                move.w          height(sp),d7           ; loop counter
                beq             done                    ; if height = 0, exit
                subq.w          #1,d7                   ; adjust for dbra
                moveq           #0,d6                   ; current row offset (0 to height-1)
                
tl_loop:        ; Calculate line length: (base_tri * (d6 + 1)) / height
                move.w          d6,d0
                addq.w          #1,d0                   ; d0 = row_offset + 1
                move.w          base_tri(sp),d1
                mulu.w          d1,d0                   ; d0 = base * (row_offset + 1)
                move.w          height(sp),d1
                divu.w          d1,d0                   ; d0 = (base * (row_offset + 1)) / height
                
                ; Draw horizontal line at (row + d6, col, length = d0)
                move.w          row(sp),d2
                add.w           d6,d2                   ; current_row = row + d6
                
                ; Call plot_horizontal_line(base, current_row, col, length)
                move.w          d0,-(sp)                ; push length
                move.w          col+2(sp),-(sp)         ; push col (+2 for length)
                move.w          d2,-(sp)                ; push current_row
                move.l          base+6(sp),-(sp)        ; push base (+6 for 3 words)
                jsr             _plot_horizontal_line
                adda.w          #10,sp                  ; clean up stack (4+2+2+2)
                
                addq.w          #1,d6                   ; increment row offset
                dbra            d7,tl_loop
                bra             done

dir_top_right:
                ; Direction 1: 90° angle at top-right
                ; Triangle expands leftward and downward
                ; Row i has line of length: (base * (i+1)) / height, ending at col
                
                move.w          height(sp),d7
                beq             done
                subq.w          #1,d7
                moveq           #0,d6
                
tr_loop:        move.w          d6,d0
                addq.w          #1,d0
                move.w          base_tri(sp),d1
                mulu.w          d1,d0
                move.w          height(sp),d1
                divu.w          d1,d0                   ; d0 = line length
                
                move.w          row(sp),d2
                add.w           d6,d2                   ; current_row
                
                move.w          col(sp),d3
                sub.w           d0,d3                   ; start_col = col - length
                
                ; Call plot_horizontal_line(base, current_row, start_col, length)
                move.w          d0,-(sp)
                move.w          d3,-(sp)
                move.w          d2,-(sp)
                move.l          base+6(sp),-(sp)
                jsr             _plot_horizontal_line
                adda.w          #10,sp
                
                addq.w          #1,d6
                dbra            d7,tr_loop
                bra             done

dir_bottom_left:
                ; Direction 2: 90° angle at bottom-left
                ; Triangle expands rightward and upward
                ; Row i (from top) has line of length: (base * i) / height
                
                move.w          height(sp),d7
                beq             done
                subq.w          #1,d7
                move.w          d7,d6                   ; start from height-1, go down to 0
                
bl_loop:        move.w          d6,d0
                addq.w          #1,d0
                move.w          base_tri(sp),d1
                mulu.w          d1,d0
                move.w          height(sp),d1
                divu.w          d1,d0                   ; d0 = line length
                
                move.w          row(sp),d2
                sub.w           d6,d2                   ; current_row = row - d6 (going upward)
                
                ; Call plot_horizontal_line(base, current_row, col, length)
                move.w          d0,-(sp)
                move.w          col+2(sp),-(sp)
                move.w          d2,-(sp)
                move.l          base+6(sp),-(sp)
                jsr             _plot_horizontal_line
                adda.w          #10,sp
                
                dbra            d6,bl_loop
                bra             done

dir_bottom_right:
                ; Direction 3: 90° angle at bottom-right
                ; Triangle expands leftward and upward
                
                move.w          height(sp),d7
                beq             done
                subq.w          #1,d7
                move.w          d7,d6
                
br_loop:        move.w          d6,d0
                addq.w          #1,d0
                move.w          base_tri(sp),d1
                mulu.w          d1,d0
                move.w          height(sp),d1
                divu.w          d1,d0                   ; d0 = line length
                
                move.w          row(sp),d2
                sub.w           d6,d2                   ; current_row = row - d6
                
                move.w          col(sp),d3
                sub.w           d0,d3                   ; start_col = col - length
                
                ; Call plot_horizontal_line(base, current_row, start_col, length)
                move.w          d0,-(sp)
                move.w          d3,-(sp)
                move.w          d2,-(sp)
                move.l          base+6(sp),-(sp)
                jsr             _plot_horizontal_line
                adda.w          #10,sp
                
                dbra            d6,br_loop

done:
                movem.l         (sp)+,d0-d7/a0-a6
                rts

