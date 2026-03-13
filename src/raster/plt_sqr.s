;
; PURPOSE: Plots a square on the screen given by the top left pixel, and the length of the sides of the square.
;
; INPUT: Address(UINT32*): to the start of the screen
;        Position(row,col): the coordinates of the top left pixel of the square
;        Side: the length of each side, in pixels, of the square
;
; OUTPUT: None
;
; void plot_square(UINT32 *base, UINT16 row, UINT16 col, UINT16 side);
;________________________________________________________________
; Draw a square region on the screen (special case of rectangle where width = height).
;
;                     -- Optimized for 48 bit widths ONLY (6 bytes wide). --
;                     *Based off our old plan to make sprites 48 pixels wide
;
; Contact @sudonym-i if you will need to use this routine in any other ways

	xdef	_plot_square

_plot_square:	
	link	a6,#-4				; Allocate 4 bytes of local variables: -2(a6) = width, -4(a6) = height
	movem.l	d0-d7/a0-a6,-(sp)

	; Check if length is zero, and initialize local width/height
	move.w	16(a6),d0			; side
	beq	sqr_done
	move.w	d0,-2(a6)			; width = side
	move.w	d0,-4(a6)			; height = side

;--------------------------------------------------------------------------------------------
;               CLIPPING LOGIC
;               A clipped square becomes a rectangle. Modifies independent 
;               width and height variables so the original loop stays unbothered.
;--------------------------------------------------------------------------------------------

	; 1. Top Clipping
	move.w	12(a6),d0			; row
	tst.w	d0
	bge	check_bottom			; If row >= 0, skip top clip

	move.w	d0,d2
	add.w	-4(a6),d2			; d2 = row + height
	ble	sqr_done			; If entirely off top, exit

	add.w	d0,-4(a6)			; height += negative row (decrease height)
	clr.w	12(a6)				; row = 0
	bra	check_bottom

check_bottom:
	cmpi.w	#400,d0
	bge	sqr_done			; If row >= 400, entirely off bottom

	move.w	d0,d2
	add.w	-4(a6),d2			; d2 = row + height (bottom edge Y)
	cmpi.w	#400,d2
	ble	check_left			; If bottom edge <= 400, skip bottom clip

	move.w	#400,d2
	sub.w	d0,d2
	move.w	d2,-4(a6)			; height = 400 - row

check_left:
	move.w	14(a6),d1			; col
	tst.w	d1
	bge	check_right			; If col >= 0, skip left clip

	move.w	d1,d2
	add.w	-2(a6),d2			; d2 = col + width
	ble	sqr_done			; If entirely off left, exit

	add.w	d1,-2(a6)			; width += negative col (decrease width)
	clr.w	14(a6)				; col = 0
	bra	check_right

check_right:
	cmpi.w	#640,d1
	bge	sqr_done			; If col >= 640, entirely off right

	move.w	d1,d2
	add.w	-2(a6),d2			; d2 = col + width (right edge X)
	cmpi.w	#640,d2
	ble	clip_done			; If right edge <= 640, skip right clip

	move.w	#640,d2
	sub.w	d1,d2
	move.w	d2,-2(a6)			; width = 640 - col

clip_done:
;--------------------------------------------------------------------------------------------
;               END CLIPPING LOGIC - Original Unoptimized Routine Resumes
;--------------------------------------------------------------------------------------------

	movea.l	8(a6),a0			; get base address
		
	; Calculate and add row offset: row * 80 bytes
	move.w	12(a6),d0			; row
	mulu.w	#80,d0				; screen width in bytes (result is long)
	adda.l	d0,a0				; add row offset
		
	; Calculate and add col offset: col / 8 (pixels to bytes)
	move.w	14(a6),d0			; col
	lsr.w	#3,d0				; divide by 8 (shift right 3 bits)
	ext.l	d0				; extend to long
	adda.l	d0,a0				; add col offset in bytes

	; Check if width == 48 pixels (6 bytes) for optimization
	move.w	-2(a6),d0			; **MODIFIED: read clipped width**
	cmpi.w	#48,d0				; check if exactly 48 pixels
	bne	unoptimized
		
	; Ensure word alignment for movem.w (address must be even)
	move.l	a0,d1
	btst	#0,d1				; test if address is odd
	bne	unoptimized			; if odd, use unoptimized version
	bra	opt_48x48

opt_48x48:	
	; Optimized for 48x48 sprites (6 bytes = 3 words wide)
	; Uses movem.w to draw 3 words at once
		
	move.w	#$ffff,d1			; set registers to all 1s
	move.w	#$ffff,d2
	move.w	#$ffff,d3
		
	move.w	-4(a6),d7			; **MODIFIED: read clipped height**
	subq.w	#1,d7				; adjust for dbra
		
row_loop_48: 
	movem.w	d1-d3,(a0)			; write 3 words (6 bytes)
	adda.w	#80,a0				; move to next row (80 bytes per row)
	dbra	d7,row_loop_48
	bra	sqr_done
		
unoptimized:	
	; Generic plot square - calculate bytes needed for col through col+width-1
	; Formula: ceiling((col+width)/8) - floor(col/8)
		
	move.w	14(a6),d6			; get col in pixels
	add.w	-2(a6),d6			; **MODIFIED: d6 = col + width**
	addq.w	#7,d6				; d6 = col + width + 7 (for ceiling division)
	lsr.w	#3,d6				; d6 = (col + width + 7) / 8 = ceiling((col+width)/8)
		
	move.w	14(a6),d5			; get col in pixels  
	lsr.w	#3,d5				; d5 = col / 8 = floor(col/8)
		
	sub.w	d5,d6				; d6 = ceiling - floor = num_bytes
	subq.w	#1,d6				; adjust for dbra
		
	move.w	-4(a6),d7			; **MODIFIED: outer loop uses clipped height**
	subq.w	#1,d7				; adjust for dbra
		
row_loop: 
	movea.l	a0,a1				; save row start position
	move.w	d6,d5				; restore column counter
		
col_loop: 
	move.b	#$ff,(a1)+			; set one byte (all pixels), advance
	dbra	d5,col_loop
		
	adda.w	#80,a0				; move to next row
	dbra	d7,row_loop

sqr_done:		
	movem.l	(sp)+,d0-d7/a0-a6
	unlk	a6
	rts

        