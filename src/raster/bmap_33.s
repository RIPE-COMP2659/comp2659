; BMAP_33.O - Atari ST M68K Disassembly (Motorola syntax)
; Compiled with SozobonX (Sozobon C)
; Function: _plot_bi

; NOTE: ` denotes a repurposed register

        xdef    _plot_bitmap_33

BITMAP_SIZE   equ     32
SCREEN_WIDTH  equ     640
SCREEN_HEIGHT equ     400
SCREEN_W_BYTE equ     SCREEN_WIDTH / 8

x             equ     8
y             equ     10
base          equ     12
bitmap        equ     16


_plot_bitmap_33:
        link    a6,#-32
        movem.l d3-d7/a3-a5,-(sp)

; Explanation:
;     By default we will assume the bitmap is fully on screen, so min = 0 and max = 31
;     These are the indices for the bitmap array
;     Then, if the x coordinate is negative, we start with that offset for the minimum bitmap index
;     In which case, x_map_min = -x
;     If the that is not the case, it is possible that the bitmap is off the right side of the screen
;     In which case it's the same idea, but this time we factor in the screen width and -1 for the upper
;     bound of the array, so x_map_max = SCREEN_WIDTH - 1 - x
init_x_ranges:
        clr.w   d0                  ; d0 = x_map_min
        move.w  #BITMAP_SIZE-1,d1   ; d1 = x_map_max
        move.w  x(a6),d6            ; d6 = x
        bpl.s   x_upper_check       ; if x >= 0, check_upper
        sub.w   d6,d0               ; Otherwise, x_map_min = -x
        bra.s   init_y_ranges
x_upper_check:
        cmp.w   #SCREEN_WIDTH-BITMAP_SIZE,d6    ; if x < SCREEN_WIDTH_PIXELS - BITMAP_SIZE
        ble.s   init_y_ranges                   ; then x_map_max = BITMAP_SIZE - 1
        move.w  #SCREEN_WIDTH-1,d1              ; Otherwise, x_map_max = SCREEN_WIDTH_PIXELS - 1 - x
        sub.w   d6,d1

; See init_x_ranges, same thing, different dimension
init_y_ranges:
        clr.w   d2                  ; d2 = y_map_min
        move.w  #BITMAP_SIZE-1,d3   ; d3 = y_map_max
        move.w  y(a6),d7            ; d7 = y
        bpl.s   y_upper_check       ; if y >= 0, check_upper
        sub.w   d7,d2               ; Otherwise, y_map_min = -y
        bra.s   init_offsets
y_upper_check:
        cmp.w   #SCREEN_HEIGHT-BITMAP_SIZE,d7   ; if y < SCREEN_HEIGHT - BITMAP_SIZE
        ble.s   init_offsets                    ; then y_map_max = BITMAP_SIZE - 1
        move.w  #SCREEN_HEIGHT-1,d3             ; Otherwise, y_map_max = SCREEN_HEIGHT - 1 - y
        sub.w   d7,d3

; Leaving this out for the time being, but at this area, we can probably make some branches based on the bitmap being
; on screen and byte alignment, but for now we will leave it
; Eg andi.w #7,d5 and beq.s to skip byte manipulation
; Explanation:
;     First, we find the x coordinate (x_screen) of the first pixel of the bitmap on the screen,
;     which is x + x_map_min, in many cases x_map_min is 0, but it accounts for clipping
;     Then, using that, we divide the value by 8 to find the appropriate byte
;     Then, we find the bit offset by using the adjusted x coordinate (x_screen), and mod 8 it
;     Lastly, we find how many bytes we're writing, the last byte minus the first, and + 1 because we're counting
;     the quantity of bytes. We're converting from indices to byte quantity. If there are 3 items in array, the last
;     index + 1 will give you size
;     
init_offsets:
        move.w  d6,d5               ; d5 = x_screen = x
        add.w   d0,d5               ; x_screen = x + x_map_min
        move.w  d5,d4               ; d4 = x_byte = x_screen
        lsr.w   #3,d4               ; x_byte = x_screen / 8
        andi.w  #7,d5               ; `d5 = bit_offset = x_screen % 8
        add.w   d1,d6               ; `d6 = num_bytes = x + x_map_max
        lsr.w   #3,d6               ; num_bytes = (x + x_map_max) / 8
        sub.w   d4,d6               ; num_bytes = (x + x_map_max) / 8 - x_byte
        addq.w  #1,d6               ; num_bytes = (x + x_map_max) / 8 - x_byte + 1
        add.w   d2,d7               ; `d7 = current_byte = y + y_map_min
        ; Can probably optimize by using shifts and adds, but saved for later
        mulu.w  #SCREEN_W_BYTE,d7   ; current_byte = (y + y_map_min) * SCREEN_WIDTH_BYTES
        ext.l   d7                  ; sign extend to 32-bit, likely too large for 16 bits
        add.l   base(a6),d7         ; current_byte = (y + y_map_min) * SCREEN_WIDTH_BYTES + base
        add.w   d4,d7               ; current_byte = (y + y_map_min) * SCREEN_WIDTH_BYTES + base + x_byte

        ; d1 is now free
        ; d2 is free, but will probably be loop counter
        ; d3 could be free, maybe, upper limit of loop
        ; d4 is now free

        movem.l (sp)+,d3-d7/a3-a5
        unlk    a6
        rts

; init



;         move.w  #$1f,-6(a6)
;         suba.l  a5,a5
;         move.w  #$1f,-$a(a6)
;         move.w  x(a6),d4
;         move.w  y(a6),d6
;         tst.w   d6
;         bge.s   .lbl0
;         move.w  d6,d0
;         neg.w   d0
;         move.w  d0,-4(a6)
;         bra.s   .lbl1
; .lbl0:
;         moveq   #32,d0
;         add.w   d6,d0
;         cmp.w   #400,d0
;         ble.s   .lbl1
;         move.w  #399,d0
;         sub.w   d6,d0
;         move.w  d0,-6(a6)
; .lbl1:
;         tst.w   d4
;         bge.s   .lbl2
;         move.w  d4,d0
;         neg.w   d0
;         movea.w d0,a5
;         bra.s   .lbl3
; .lbl2:
;         moveq   #32,d0
;         add.w   d4,d0
;         cmp.w   #640,d0
;         ble.s   .lbl3
;         move.w  #639,d0
;         sub.w   d4,d0
;         move.w  d0,-$a(a6)
; .lbl3:
;         move.w  d4,d0
;         add.w   a5,d0
;         move.w  d0,-$10(a6)
;         move.w  -$10(a6),d0
;         asr.w   #3,d0
;         move.w  d0,-$12(a6)
;         move.w  -$10(a6),d0
;         and.w   #7,d0
;         move.w  d0,d3
;         move.w  -$a(a6),d0
;         add.w   d4,d0
;         asr.w   #3,d0
;         sub.w   -$12(a6),d0
;         addq.w  #1,d0
;         move.w  d0,-$16(a6)
;         move.w  #$ff,d0
;         asr.w   d3,d0
;         move.b  d0,-$17(a6)
;         move.w  #$ff,d0
;         moveq   #8,d1
;         sub.w   d3,d1
;         asl.w   d1,d0
;         move.b  d0,-$18(a6)
;         move.w  -4(a6),d0
;         add.w   d6,d0
;         muls.w  #80,d0
;         ext.l   d0
;         add.l   $c(a6),d0
;         move.w  -$12(a6),d1
;         ext.l   d1
;         add.l   d1,d0
;         movea.l d0,a3
;         movea.w -4(a6),a4
;         bra     .lbl17
; .lbl4:
;         move.w  a4,d0
;         ext.l   d0
;         asl.l   #2,d0
;         movea.l d0,a0
;         move.l  $10(a6),d0
;         adda.l  d0,a0
;         move.l  (a0),d0
;         move.w  a5,d1
;         asl.l   d1,d0
;         move.l  d0,d5
;         moveq   #24,d1
;         lsr.l   d1,d0
;         move.b  d0,-$1d(a6)
;         move.l  d5,d0
;         moveq   #16,d1
;         lsr.l   d1,d0
;         move.b  d0,-$1e(a6)
;         move.l  d5,d0
;         lsr.l   #8,d0
;         move.b  d0,-$1f(a6)
;         move.b  d5,d7
;         tst.w   d3
;         bne.s   .lbl10
;         move.w  -$16(a6),d0
;         subq.w  #4,d0
;         beq.s   .lbl5
;         addq.w  #1,d0
;         beq.s   .lbl6
;         addq.w  #1,d0
;         beq.s   .lbl7
;         addq.w  #1,d0
;         beq.s   .lbl8
;         bra.s   .lbl9
; .lbl5:
;         move.b  d7,3(a3)
; .lbl6:
;         move.b  -$1f(a6),2(a3)
; .lbl7:
;         move.b  -$1e(a6),1(a3)
; .lbl8:
;         move.b  -$1d(a6),(a3)
; .lbl9:
;         bra     .lbl16
; .lbl10:
;         move.w  -$16(a6),d0
;         subq.w  #5,d0
;         beq.s   .lbl11
;         addq.w  #1,d0
;         beq.s   .lbl12
;         addq.w  #1,d0
;         beq.s   .lbl13
;         addq.w  #1,d0
;         beq.s   .lbl14
;         addq.w  #1,d0
;         beq.s   .lbl15
;         bra     .lbl16
; .lbl11:
;         clr.w   d0
;         move.b  4(a3),d0
;         clr.w   d1
;         move.b  -$18(a6),d1
;         not.w   d1
;         and.w   d1,d0
;         clr.w   d1
;         move.b  d7,d1
;         moveq   #8,d2
;         sub.w   d3,d2
;         asl.w   d2,d1
;         or.w    d1,d0
;         move.b  d0,4(a3)
; .lbl12:
;         clr.w   d0
;         move.b  -$1f(a6),d0
;         moveq   #8,d1
;         sub.w   d3,d1
;         asl.w   d1,d0
;         clr.w   d1
;         move.b  d7,d1
;         asr.w   d3,d1
;         or.w    d1,d0
;         move.b  d0,3(a3)
; .lbl13:
;         clr.w   d0
;         move.b  -$1e(a6),d0
;         moveq   #8,d1
;         sub.w   d3,d1
;         asl.w   d1,d0
;         clr.w   d1
;         move.b  -$1f(a6),d1
;         asr.w   d3,d1
;         or.w    d1,d0
;         move.b  d0,2(a3)
; .lbl14:
;         clr.w   d0
;         move.b  -$1d(a6),d0
;         moveq   #8,d1
;         sub.w   d3,d1
;         asl.w   d1,d0
;         clr.w   d1
;         move.b  -$1e(a6),d1
;         asr.w   d3,d1
;         or.w    d1,d0
;         move.b  d0,1(a3)
; .lbl15:
;         clr.w   d0
;         move.b  -$17(a6),d0
;         not.w   d0
;         clr.w   d1
;         move.b  (a3),d1
;         and.w   d1,d0
;         clr.w   d1
;         move.b  -$1d(a6),d1
;         asr.w   d3,d1
;         or.w    d1,d0
;         move.b  d0,(a3)
; .lbl16:
;         lea     80(a3),a3
;         addq.w  #1,a4
; .lbl17:
;         cmpa.w  -6(a6),a4
;         ble     .lbl4

