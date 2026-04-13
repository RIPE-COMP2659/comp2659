; set_vid.s
; Purpose: Set Atari ST video framebuffer base by writing hardware video
;          base registers directly.
; Input:   base (UINT16*), passed on the C stack at 4(sp).
; Output:  No return value.
; Notes:   Caller must be in Supervisor mode before invoking this routine.
;
; void set_video_base(UINT16 *base);

        xdef    _set_video_base

VIDEO_BASE_HI  equ     $FFFF8201
VIDEO_BASE_MID equ     $FFFF8203

_set_video_base:
        move.l  4(sp),d0                ; d0 = requested 24-bit framebuffer base.
        lsr.l   #8,d0                   ; low word now holds top/mid bytes: 0x00HHMM.

        move.b  d0,VIDEO_BASE_MID       ; write middle byte to $FFFF8203.
        lsr.w   #8,d0
        move.b  d0,VIDEO_BASE_HI        ; write high byte to $FFFF8201.

        rts
