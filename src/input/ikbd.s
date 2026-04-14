; See .h file for documentation

        xdef    _installVector
        xdef    _ikbdisr

        xref    _handle_ikbd_byte

vectorNum       equ     4
handler         equ     6

MFP_ISRB        equ     $FFFA11
MFP_IKBD_BIT    equ     6

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
        movem.l d0-d2/a0-a2,-(sp)      ; save caller-saved registers
        jsr     _handle_ikbd_byte
        movem.l (sp)+,d0-d2/a0-a2      ; restore
        bclr    #MFP_IKBD_BIT,MFP_ISRB ; clear in-service bit
        rte
