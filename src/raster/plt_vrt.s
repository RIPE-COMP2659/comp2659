;
; PURPOSE: Plot a hoizontal line on the screen. The vertical line is specified by the topmost pixel of the line and the length of the line.
;
; INPUT: Address(UINT32*): to the start of the screen
;        Position(row,col): the coordinates of the topmost pixel of the vertical line
;        Length: the lenth in pixels of the line
;        Color: color of the line (0 = black, 1 = white)
;
; OUTPUT: None
;
; void plot_vertical_line(UINT32 *base, UINT16 row, UINT16 col, UINT16 length, UINT16 color);
;________________________________________________________________
; Draw a vertical line of pixels
;
;
;                      -- This routine is unoptimized, as we shouldn't be using it ---
;                                       DO NOT USE THIS METHOD
;
; Contact @sudonym-i if you will need to use this routine.

	xdef	_plot_vertical_line

_plot_vertical_line:	
	link	a6,#0
	movem.l	d0-d3/a0,-(sp)
		
	; Check if length is zero
	move.w	16(a6),d3			; length offset
	beq	vrt_done			; if length = 0, exit

;--------------------------------------------------------------------------------------------
;               CLIPPING LOGIC (@sudonym-i's recommendation)
;               Check bounds before drawing. Modify start/end points 
;               and lengths so the original loop stays unbothered.
;--------------------------------------------------------------------------------------------
	
	; 1. Left/Right Clipping (Entirely out of bounds)
	move.w	14(a6),d1			; col offset
	tst.w	d1
	blt	vrt_done			; If col < 0, entirely off left
	cmpi.w	#640,d1
	bge	vrt_done			; If col >= 640, entirely off right

	; 2. Top Clipping
	move.w	12(a6),d0			; row offset
	bge	check_bottom			; If row >= 0, skip top clip

	; Handle top clipping (row < 0)
	move.w	d0,d2
	add.w	d3,d2				; d2 = row + length
	ble	vrt_done			; If row + length <= 0, entirely off top

	add.w	d0,d3				; Decrease length by the off-screen amount (length += negative row)
	clr.w	d0				; Set row to 0 (top of screen)
	bra	check_bottom

check_bottom:
	cmpi.w	#400,d0
	bge	vrt_done			; If row >= 400, entirely off bottom

	move.w	d0,d2
	add.w	d3,d2				; d2 = row + length (bottom edge Y)
	cmpi.w	#400,d2
	ble	clip_done			; If bottom edge <= 400, skip bottom clip

	; Handle bottom clipping
	move.w	#400,d3
	sub.w	d0,d3				; length = 400 - row

clip_done:
	; Save potentially modified parameters back to stack for original code to use
	move.w	d0,12(a6)			; row
	move.w	d3,16(a6)			; length

;--------------------------------------------------------------------------------------------
;               END CLIPPING LOGIC - Original Unoptimized Routine Resumes
;--------------------------------------------------------------------------------------------
		
	movea.l	8(a6),a0			; get base address
		
	; Calculate and add row offset: row * 80 bytes
	move.w	12(a6),d0			; row
	mulu.w	#80,d0				; screen width in bytes
	adda.l	d0,a0				; add row offset
		
	; Calculate and add col byte offset: col / 8
	move.w	14(a6),d1			; col
	move.w	d1,d2				; save col for bit calculation
	lsr.w	#3,d1				; d1 = col / 8 (byte offset)
	adda.w	d1,a0				; a0 now points to the byte
		
	; Calculate bit position: 7 - (col % 8)
	andi.w	#7,d2				; d2 = col % 8
	moveq	#7,d0
	sub.w	d2,d0				; d0 = 7 - (col % 8) = bit position
		
	; Check color parameter
	move.w	18(a6),d1			; color
	tst.w	d1				; check if color is 0
	beq	pixel_loop_black
		
pixel_loop_white:
	; Set the bit (white)
	bset	d0,(a0)				; set bit d0 in byte at (a0)
		
	; Decrement length counter
	subq.w	#1,d3
	beq	vrt_done			; if counter = 0, we're done
		
	; Move to next row (add 80 bytes)
	adda.w	#80,a0				; move down one row
	bra	pixel_loop_white

pixel_loop_black:
	; Clear the bit (black)
	bclr	d0,(a0)				; clear bit d0 in byte at (a0)
		
	; Decrement length counter
	subq.w	#1,d3
	beq	vrt_done			; if counter = 0, we're done
		
	; Move to next row (add 80 bytes)
	adda.w	#80,a0				; move down one row
	bra	pixel_loop_black

vrt_done:
	movem.l	(sp)+,d0-d3/a0
	unlk	a6
	rts