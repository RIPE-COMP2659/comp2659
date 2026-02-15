;
;  CREDIT: Comp 2659 LAB assembly code.
;
;     **This solution is higly optimized, do not touch.**
;

                xdef            _clear_screen

base            equ             64              ; offset from SP, not A6

_clear_screen:  movem.l         d0-d7/a0-a6,-(sp)
                lea             zeros,a0
                movem.l         (a0)+,d1-d7/a1-a6
                movea.l         base(sp),a0
                adda.l          #32000,a0
                move.w          #614,d0
fill_loop:      movem.l         d1-d7/a1-a6,-(a0)
                dbra            d0,fill_loop
                movem.l         d1-d5,-(a0)
                movem.l         (sp)+,d0-d7/a0-a6
                rts

zeros:          ds.l            13
