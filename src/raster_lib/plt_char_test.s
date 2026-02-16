;
; Test version with hardcoded font data to verify bitmap rendering works
;

                xdef            _plot_character
                xref            _plot_bitmap_8
                
base            equ             8               
row             equ             12              
col             equ             14
ch              equ             16              

FONT_HEIGHT     equ             8

; Hardcoded test font - just letters A-Z for testing
test_font:
                dc.b            $00,$00,$00,$00,$00,$00,$00,$00  ; space (0)
                ; ... (ASCII 0-64 as blank)
                
                ; 'A' = ASCII 65
                .rept           65
                dc.b            $00,$00,$00,$00,$00,$00,$00,$00
                .endr
                
                dc.b            $18,$24,$42,$42,$7E,$42,$42,$42  ; 'A'
                dc.b            $7C,$42,$42,$7C,$42,$42,$42,$7C  ; 'B'  
                dc.b            $3C,$42,$40,$40,$40,$40,$42,$3C  ; 'C'

_plot_character:
                link            a6,#0
                movem.l         d0-d7/a0-a6,-(sp)

                ; Use hardcoded font for testing
                lea             test_font,a1
                
                ; Calculate offset
                move.w          ch(a6),d0
                ext.l           d0
                mulu.w          #FONT_HEIGHT,d0
                adda.l          d0,a1
                
                ; Call plot_bitmap_8
                move.w          #FONT_HEIGHT,-(sp)
                move.l          a1,-(sp)
                move.w          col(a6),-(sp)
                move.w          row(a6),-(sp)
                move.l          base(a6),-(sp)
                
                jsr             _plot_bitmap_8
                
                lea             14(sp),sp
                
                movem.l         (sp)+,d0-d7/a0-a6
                unlk            a6
                rts
