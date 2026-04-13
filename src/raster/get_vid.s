; get_vid.s
; Authors:
;     Riley Gramlich, rgram060@mtroyal.ca, 201762060
;     Robert Parker Hutcheson, rhutc335@mtroyal.ca, 201762335
;     Isaac Klein, iklei977@mtroyal.ca, 201763977
;     Eduard Mykhailets, emykh268@mtroyal.ca, 201750268
; Course: COMP 2659-001, Computing Machinery II, Winter 2026
; Instructor: Nolan Shaw
;
; PURPOSE: Return the current physical framebuffer base address by reading
;          Atari ST video hardware registers directly.
;
; UINT16 *get_video_base(void);

        xdef    _get_video_base

VIDEO_BASE_HI  equ     $FFFF8201
VIDEO_BASE_MID equ     $FFFF8203

_get_video_base:
        clr.l   d0                      ; clear full 32-bit return register
        move.b  VIDEO_BASE_HI,d0        ; d0 = 0x000000HH
        lsl.l   #8,d0                   ; d0 = 0x0000HH00
        move.b  VIDEO_BASE_MID,d0       ; d0 = 0x0000HHMM
        lsl.l   #8,d0                   ; d0 = 0x00HHMM00 (24-bit base, aligned)
        rts
