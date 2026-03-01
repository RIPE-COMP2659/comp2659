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
; Implements Bresenham's line drawing algorithm with per-pixel clipping.

	xdef	_plot_line

_plot_line:
	link	a6,#0
	movem.l	d0-d7/a0-a1,-(sp)
		
	; Save color in a1
	move.w	20(a6),d0			; color
	andi.w	#1,d0				; ensure color is 0 or 1
	movea.w	d0,a1				; a1 = color (preserved)
		
	; Initialize current position
	move.w	12(a6),d4			; d4 = current_row
	move.w	14(a6),d5			; d5 = current_col
		
	; Calculate dx = abs(end_col - start_col)
	move.w	18(a6),d0			; end_col
	move.w	14(a6),d1			; start_col
	sub.w	d1,d0				; d0 = end_col - start_col
	bge	dx_positive
	neg.w	d0				; d0 = abs(dx)
dx_positive:
	move.w	d0,d6				; d6 = dx
		
	; Calculate dy = abs(end_row - start_row)
	move.w	16(a6),d0			; end_row
	move.w	12(a6),d1			; start_row
	sub.w	d1,d0				; d0 = end_row - start_row
	bge	dy_positive
	neg.w	d0				; d0 = abs(dy)
dy_positive:
	move.w	d0,d7				; d7 = dy
		
	; Determine step direction for x (col)
	moveq	#1,d2				; step_x = 1
	move.w	18(a6),d0
	cmp.w	14(a6),d0
	bge	check_y_step
	moveq	#-1,d2				; step_x = -1
		
check_y_step:
	; Determine step direction for y (row)
	moveq	#1,d3				; step_y = 1
	move.w	16(a6),d0
	cmp.w	12(a6),d0
	bge	init_bresenham
	moveq	#-1,d3				; step_y = -1
		
init_bresenham:
	; Register allocation:
	; a1 = color (preserved)
	; d1 = error
	; d2 = step_x, d3 = step_y
	; d4 = current_row, d5 = current_col
	; d6 = dx (preserved), d7 = dy (preserved)

	; Determine if line is more horizontal or vertical
	cmp.w	d7,d6
	blt	v_loop_start			; if dx < dy, go to vertical loop
		
h_loop_start:
	; Horizontal-ish line (dx >= dy)
	; error = 2*dy - dx
	move.w	d7,d1
	lsl.w	#1,d1
	sub.w	d6,d1				; d1 = 2dy - dx
h_loop:
	bsr	plot_pixel_clipped
	cmp.w	18(a6),d5			; current_col == end_col?
	beq	line_done
	
	tst.w	d1				; error < 0?
	blt	h_no_y
	
	add.w	d3,d4				; row += step_y
	move.w	d7,d0
	sub.w	d6,d0				; dy - dx
	lsl.w	#1,d0				; 2(dy - dx)
	add.w	d0,d1				; error += 2(dy - dx)
	bra	h_step_x
h_no_y:
	move.w	d7,d0
	lsl.w	#1,d0				; 2*dy
	add.w	d0,d1				; error += 2*dy
h_step_x:
	add.w	d2,d5				; col += step_x
	bra	h_loop

v_loop_start:
	; Vertical-ish line (dy > dx)
	; error = 2*dx - dy
	move.w	d6,d1
	lsl.w	#1,d1
	sub.w	d7,d1				; d1 = 2dx - dy
v_loop:
	bsr	plot_pixel_clipped
	cmp.w	16(a6),d4			; current_row == end_row?
	beq	line_done
	
	tst.w	d1				; error < 0?
	blt	v_no_x
	
	add.w	d2,d5				; col += step_x
	move.w	d6,d0
	sub.w	d7,d0				; dx - dy
	lsl.w	#1,d0				; 2(dx - dy)
	add.w	d0,d1				; error += 2(dx - dy)
	bra	v_step_y
v_no_x:
	move.w	d6,d0
	lsl.w	#1,d0				; 2*dx
	add.w	d0,d1				; error += 2*dx
v_step_y:
	add.w	d3,d4				; row += step_y
	bra	v_loop

line_done:
	movem.l	(sp)+,d0-d7/a0-a1
	unlk	a6
	rts

; --- Internal Helper: Plot Pixel at (d4, d5) with Clipping ---
plot_pixel_clipped:
	; Bounds checking
	tst.w	d4				; row < 0?
	blt	pixel_out
	cmpi.w	#400,d4				; row >= 400?
	bge	pixel_out
	tst.w	d5				; col < 0?
	blt	pixel_out
	cmpi.w	#640,d5				; col >= 640?
	bge	pixel_out

	; Pixel is on screen, calculate address and plot
	movem.l	d0-d1/a0,-(sp)			; preserve working registers
	movea.l	8(a6),a0			; base address
	
	move.w	d4,d0				; row
	mulu.w	#80,d0				; row * 80 bytes
	adda.l	d0,a0
	
	move.w	d5,d0				; col
	move.w	d0,d1				; copy col for bit math
	lsr.w	#3,d0				; col / 8 (byte offset)
	adda.w	d0,a0				; a0 = final byte address
	
	andi.w	#7,d1				; col % 8
	not.w	d1
	andi.w	#7,d1				; bit position = 7 - (col % 8)
	
	move.w	a1,d0				; check color
	beq	plot_pixel_black
	bset	d1,(a0)				; set bit to 1 (white)
	bra	plot_pixel_exit
plot_pixel_black:
	bclr	d1,(a0)				; clear bit to 0 (black)
plot_pixel_exit:
	movem.l	(sp)+,d0-d1/a0			; restore registers
pixel_out:
	rts