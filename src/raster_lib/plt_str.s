
;
; PURPOSE: Plots a string, as a sequence of bitmaps from a font table, to the screen.
;
; INPUT: Address(UINT8*): to the start of the screen
;        Position(row,col): the coordinates of the top left pixel of the string
;        str(char*): pointer to the null-terminated string to be written to the screen
;
; OUTPUT: None
;
; void plot_string(UINT8 *base, UINT16 row, UINT16 col, char *str);
;________________________________________________________________
; Plot a null-terminated string using plot_character.
;
; Uses the plt_character() method to print an entire string.
;

                xdef            _plot_string
                xref            _plot_character

base            equ             8              
row             equ             12              
col             equ             14
str             equ             16              ; pointer to string

CHAR_WIDTH      equ             8               ; characters are 8 pixels wide


_plot_string:
                link            a6,#0
                movem.l         d0-d7/a0-a6,-(sp)

                move.l          str(a6),a0      ; get string pointer
                move.w          col(a6),d6      ; current column position
                
char_loop:
                move.b          (a0)+,d7        ; get next character
                beq             done            ; if null terminator, we're done
                
                ; Call plot_character(base, row, current_col, ch)
                ext.w           d7              ; extend char to word
                move.w          d7,-(sp)        ; push character
                move.w          d6,-(sp)        ; push current column
                move.w          row(a6),-(sp)   ; push row
                move.l          base(a6),-(sp)  ; push base
                
                jsr             _plot_character
                
                lea             10(sp),sp       ; clean up parameters
                
                ; Advance to next character position
                addi.w          #CHAR_WIDTH,d6  ; move 8 pixels to the right
                
                bra             char_loop
                
done:
                movem.l         (sp)+,d0-d7/a0-a6
                unlk            a6
                rts

