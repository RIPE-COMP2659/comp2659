; See .h file for documentation

        xdef    _installVector
        xdef    _ikbdisr

vectorNum       equ     4
handler         equ     6

; See .h file for documentation
_installVector:
        moveq   #0,d1
        move.w  vectorNum(sp),d1
        lsl.l   #2,d1
        movea.l d1,a0
        move.l  (a0),d0
        move.l  handler(sp),(a0)
        rts

; See .h file for documentation
_ikbdisr:
        rte
