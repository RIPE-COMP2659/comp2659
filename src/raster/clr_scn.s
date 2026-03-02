;
; PURPOSE: Clears the entire screen.
;
; INPUT: Address(UINT32*): to the start of the screen
;
; OUTPUT: None
;
; void clear_screen(UINT32 *base);
;________________________________________________________________
;  CREDIT: Comp 2659 LAB assembly code.
;
;     **This solution is higly optimized, do not touch.**
;

        xdef    _clear_screen

base    equ     8                               ; offset from A6

_clear_screen: link a6,#0
        movem.l d0-d7/a0-a5,-(sp)
        lea     zeros,a0
        movem.l (a0)+,d1-d7/a1-a5
        movea.l base(a6),a0
        adda.l  #32000,a0
        move.w  #665,d0
fill_loop: movem.l d1-d7/a1-a5,-(a0)
        dbra    d0,fill_loop
        movem.l d1-d7/a1,-(a0)
        movem.l (sp)+,d0-d7/a0-a5
        unlk    a6
        rts

zeros:  ds.l    13
