; BMAP_33.O - Atari ST M68K Disassembly (Motorola syntax)
; Compiled with SozobonX (Sozobon C)
; Function: _plot_bi

    xdef    _plot_bitmap_33

_plot_bitmap_33:
	link	a6,#-$20
	movem.l	d3-d7/a3-a5,-(sp)
	move.w	8(a6),d4
	move.w	$A(a6),d6
	clr.w	-4(a6)
	move.w	#$1F,-6(a6)
	suba.l	a5,a5
	move.w	#$1F,-$A(a6)
	tst.w	d6
	bge.s	.lbl0
	move.w	d6,d0
	neg.w	d0
	move.w	d0,-4(a6)
	bra.s	.lbl1
.lbl0:
	moveq	#32,d0
	add.w	d6,d0
	cmp.w	#400,d0
	ble.s	.lbl1
	move.w	#399,d0
	sub.w	d6,d0
	move.w	d0,-6(a6)
.lbl1:
	tst.w	d4
	bge.s	.lbl2
	move.w	d4,d0
	neg.w	d0
	movea.w	d0,a5
	bra.s	.lbl3
.lbl2:
	moveq	#32,d0
	add.w	d4,d0
	cmp.w	#640,d0
	ble.s	.lbl3
	move.w	#639,d0
	sub.w	d4,d0
	move.w	d0,-$A(a6)
.lbl3:
	move.w	d4,d0
	add.w	a5,d0
	move.w	d0,-$10(a6)
	move.w	-$10(a6),d0
	asr.w	#3,d0
	move.w	d0,-$12(a6)
	move.w	-$10(a6),d0
	and.w	#7,d0
	move.w	d0,d3
	move.w	-$A(a6),d0
	add.w	d4,d0
	asr.w	#3,d0
	sub.w	-$12(a6),d0
	addq.w	#1,d0
	move.w	d0,-$16(a6)
	move.w	#$FF,d0
	asr.w	d3,d0
	move.b	d0,-$17(a6)
	move.w	#$FF,d0
	moveq	#8,d1
	sub.w	d3,d1
	asl.w	d1,d0
	move.b	d0,-$18(a6)
	move.w	-4(a6),d0
	add.w	d6,d0
	muls.w	#80,d0
	ext.l	d0
	add.l	$C(a6),d0
	move.w	-$12(a6),d1
	ext.l	d1
	add.l	d1,d0
	movea.l	d0,a3
	movea.w	-4(a6),a4
	bra	.lbl17
.lbl4:
	move.w	a4,d0
	ext.l	d0
	asl.l	#2,d0
	movea.l	d0,a0
	move.l	$10(a6),d0
	adda.l	d0,a0
	move.l	(a0),d0
	move.w	a5,d1
	asl.l	d1,d0
	move.l	d0,d5
	moveq	#24,d1
	lsr.l	d1,d0
	move.b	d0,-$1D(a6)
	move.l	d5,d0
	moveq	#16,d1
	lsr.l	d1,d0
	move.b	d0,-$1E(a6)
	move.l	d5,d0
	lsr.l	#8,d0
	move.b	d0,-$1F(a6)
	move.b	d5,d7
	tst.w	d3
	bne.s	.lbl10
	move.w	-$16(a6),d0
	subq.w	#4,d0
	beq.s	.lbl5
	addq.w	#1,d0
	beq.s	.lbl6
	addq.w	#1,d0
	beq.s	.lbl7
	addq.w	#1,d0
	beq.s	.lbl8
	bra.s	.lbl9
.lbl5:
	move.b	d7,3(a3)
.lbl6:
	move.b	-$1F(a6),2(a3)
.lbl7:
	move.b	-$1E(a6),1(a3)
.lbl8:
	move.b	-$1D(a6),(a3)
.lbl9:
	bra	.lbl16
.lbl10:
	move.w	-$16(a6),d0
	subq.w	#5,d0
	beq.s	.lbl11
	addq.w	#1,d0
	beq.s	.lbl12
	addq.w	#1,d0
	beq.s	.lbl13
	addq.w	#1,d0
	beq.s	.lbl14
	addq.w	#1,d0
	beq.s	.lbl15
	bra	.lbl16
.lbl11:
	clr.w	d0
	move.b	4(a3),d0
	clr.w	d1
	move.b	-$18(a6),d1
	not.w	d1
	and.w	d1,d0
	clr.w	d1
	move.b	d7,d1
	moveq	#8,d2
	sub.w	d3,d2
	asl.w	d2,d1
	or.w	d1,d0
	move.b	d0,4(a3)
.lbl12:
	clr.w	d0
	move.b	-$1F(a6),d0
	moveq	#8,d1
	sub.w	d3,d1
	asl.w	d1,d0
	clr.w	d1
	move.b	d7,d1
	asr.w	d3,d1
	or.w	d1,d0
	move.b	d0,3(a3)
.lbl13:
	clr.w	d0
	move.b	-$1E(a6),d0
	moveq	#8,d1
	sub.w	d3,d1
	asl.w	d1,d0
	clr.w	d1
	move.b	-$1F(a6),d1
	asr.w	d3,d1
	or.w	d1,d0
	move.b	d0,2(a3)
.lbl14:
	clr.w	d0
	move.b	-$1D(a6),d0
	moveq	#8,d1
	sub.w	d3,d1
	asl.w	d1,d0
	clr.w	d1
	move.b	-$1E(a6),d1
	asr.w	d3,d1
	or.w	d1,d0
	move.b	d0,1(a3)
.lbl15:
	clr.w	d0
	move.b	-$17(a6),d0
	not.w	d0
	clr.w	d1
	move.b	(a3),d1
	and.w	d1,d0
	clr.w	d1
	move.b	-$1D(a6),d1
	asr.w	d3,d1
	or.w	d1,d0
	move.b	d0,(a3)
.lbl16:
	lea	80(a3),a3
	addq.w	#1,a4
.lbl17:
	cmpa.w	-6(a6),a4
	ble	.lbl4
	movem.l	(sp)+,d3-d7/a3-a5
	unlk	a6
	rts
