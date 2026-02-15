;
; PURPOSE: Plots a single character, as a bitmap from a font table, to the screen.
;
; INPUT: Address(UINT8*): to the start of the screen
;        Position(row,col): the coordinates of the top left pixel of the character
;        ch(char): the character to be written to the screen
;
; OUTPUT: None
;
; void plot_character(UINT8 *base, UINT16 row, UINT16 col, char ch);
;________________________________________________________________
; Plot a single ASCII character using 8x8 system font.
;
; Uses plot_bitmap_8 to render the character.

                xdef            _plot_character
                xref            _plot_bitmap_8

base            equ             8               ; offset from A6
row             equ             12              
col             equ             14
ch              equ             16              ; character (extended to word)

FONT_HEIGHT     equ             8               ; standard 8x8 font


_plot_character:
                link            a6,#0
                movem.l         d0-d7/a0-a6,-(sp)

                ; Get system font using Line-A
                movem.l         a0-a2,-(sp)     ; save these before Line-A call
                dc.w            $A000           ; Line-A initialization
                ; After this: A0 = Line-A vars, A1 = font_ring, A2 = functions
                
                ; Font_ring at A1 contains array of 4 pointers to font headers
                ; Get the 8x16 font (index 2, offset 8)
                movea.l         8(a1),a1        ; get pointer to 8x16 font header
                
                ; Get font bitmap data pointer from header (offset 80)
                movea.l         80(a1),a1       ; a1 now points to font table base
                movem.l         (sp)+,a0-a2     ; restore a0-a2
                
                ; Font is stored in planar format:
                ; Row 0 of ALL characters (bytes 0-255)
                ; Row 1 of ALL characters (bytes 256-511)
                ; ...
                ; Row 15 of ALL characters (bytes 3840-4095)
                ;
                ; To get character ch:
                ;   Row 0: font[ch]
                ;   Row 1: font[ch + 256]
                ;   Row 2: font[ch + 512]
                ;   etc.
                
                ; Get character code and add to font base for row 0
                move.w          ch(a6),d0       ; get character code
                ext.l           d0              ; extend to long
                adda.l          d0,a1           ; a1 now points to row 0 of character
                
                ; Calculate screen position
                movea.l         base(a6),a0     ; get base address
                move.w          row(a6),d0      ; get row
                mulu.w          #80,d0          ; row * 80 bytes per row
                adda.l          d0,a0           ; add row offset
                move.w          col(a6),d0      ; get col
                lsr.w           #3,d0           ; col / 8 = byte offset
                ext.l           d0
                adda.l          d0,a0           ; a0 now points to screen position
                
                ; Plot 16 rows manually (can't use plot_bitmap_8 - data not contiguous)
                moveq           #15,d7          ; 16 rows (0-15) for 8x16 font
                
plot_row_loop:
                move.b          (a1),(a0)       ; copy one byte from font to screen
                adda.w          #256,a1         ; move to next row in font table (256 bytes apart)
                adda.w          #80,a0          ; move to next row on screen (80 bytes per row)
                dbra            d7,plot_row_loop
                
                movem.l         (sp)+,d0-d7/a0-a6
                unlk            a6
                rts


