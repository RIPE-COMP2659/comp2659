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
;     DOC THE Y STUFF
;     DOC THE BITMASK STUFF
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
        move.l  d7,a0               ; a0 = current_byte address
        move.w  #$00FF,d1           ; `d1 = bitmask = 1111 1111 1111 1111
        lsr.w   d5,d1               ; bitmask = bitmask >> bit_offset
        move.w  d1,d4               ; `d4 = right_bitmask = bitmask >> bit_offset
        not.w   d4                  ; right_bitmask = ~(left_bitmask)
        move.w  d0,-(sp)            ; push x_map_min
        bra     row_while

; Everything below here needs a revisit, but time is not permitting right now
row_loop:
        move.l  bitmap(a6),a1      ; a1 = bitmap base pointer
        move.w  d2,d7              ; d7 = y_map_i
        ext.l   d7                 ; sign extend to 32-bit
        asl.l   #2,d7              ; d7 = y_map_i * 4
        move.l  0(a1,d7),d7      ; d7 = bitmap[y_map_i]
        move.w  0(sp),d0           ; d0 = x_map_min
        lsl.l   d0,d7              ; d7 = row_bits = bitmap[y_map_i] << x_map_min
        move.l  d7,-(sp)           ; push row_bits
                                   ; 0(sp) = b3, 1(sp) = b2, 2(sp) = b1, 3(sp) = b0
                                   ; 4(sp) = x_map_min (word)
        tst.w   d5
        bne.s   unaligned_blit

aligned_blit:
        move.w  d6,d7              ; d7 = num_bytes scratch
        subq.w  #4,d7
        beq.s   aligned_4
        addq.w  #1,d7
        beq.s   aligned_3
        addq.w  #1,d7
        beq.s   aligned_2
        addq.w  #1,d7
        beq.s   aligned_1
        bra     blit_done

aligned_4:
        move.b  3(sp),3(a0)         ; dst[3] = b3
aligned_3:
        move.b  2(sp),2(a0)         ; dst[2] = b2
aligned_2:
        move.b  1(sp),1(a0)         ; dst[1] = b1
aligned_1:
        move.b  0(sp),0(a0)         ; dst[0] = b0
        bra     blit_done

unaligned_blit:
        move.w  #8,d7
        sub.w   d5,d7              ; d7 = 8 - bit_offset
        move.w  d7,-(sp)           ; push 8 - bit_offset
                                   ; 0(sp) = 8-bit_offset (word)
                                   ; 2(sp) = b3, 3(sp) = b2, 4(sp) = b1, 5(sp) = b0
                                   ; 6(sp) = x_map_min (word)
        move.w  d6,d7              ; d7 = num_bytes scratch
        subq.w  #5,d7
        beq.s   unaligned_5
        addq.w  #1,d7
        beq.s   unaligned_4
        addq.w  #1,d7
        beq.s   unaligned_3
        addq.w  #1,d7
        beq.s   unaligned_2
        addq.w  #1,d7
        beq.s   unaligned_1
        bra.s   unaligned_done

unaligned_5:
        clr.w   d7
        move.b  5(sp),d7           ; d7 = b3
        move.w  0(sp),d0           ; d0 = 8 - bit_offset
        asl.w   d0,d7              ; d7 = b3 << (8 - bit_offset)
        clr.w   d0
        move.b  4(a0),d0           ; d0 = dst[4]
        and.w   d1,d0              ; d0 = dst[4] & ~right_mask
        or.w    d0,d7              ; d7 = result
        move.b  d7,4(a0)
unaligned_4:
        clr.w   d7
        move.b  4(sp),d7           ; d7 = b2
        move.w  0(sp),d0           ; d0 = 8 - bit_offset
        asl.w   d0,d7              ; d7 = b2 << (8 - bit_offset)
        clr.w   d0
        move.b  5(sp),d0           ; d0 = b3
        lsr.w   d5,d0              ; d0 = b3 >> bit_offset
        or.w    d0,d7              ; d7 = result
        move.b  d7,3(a0)
unaligned_3:
        clr.w   d7
        move.b  3(sp),d7           ; d7 = b1
        move.w  0(sp),d0           ; d0 = 8 - bit_offset
        asl.w   d0,d7              ; d7 = b1 << (8 - bit_offset)
        clr.w   d0
        move.b  4(sp),d0           ; d0 = b2
        lsr.w   d5,d0              ; d0 = b2 >> bit_offset
        or.w    d0,d7              ; d7 = result
        move.b  d7,2(a0)
unaligned_2:
        clr.w   d7
        move.b  2(sp),d7           ; d7 = b0
        move.w  0(sp),d0           ; d0 = 8 - bit_offset
        asl.w   d0,d7              ; d7 = b0 << (8 - bit_offset)
        clr.w   d0
        move.b  3(sp),d0           ; d0 = b1
        lsr.w   d5,d0              ; d0 = b1 >> bit_offset
        or.w    d0,d7              ; d7 = result
        move.b  d7,1(a0)
unaligned_1:
        clr.w   d7
        move.b  2(sp),d7           ; d7 = b0
        lsr.w   d5,d7              ; d7 = b0 >> bit_offset
        clr.w   d0
        move.b  (a0),d0            ; d0 = dst[0]
        and.w   d4,d0              ; d0 = dst[0] & ~left_mask
        or.w    d0,d7              ; d7 = result
        move.b  d7,(a0)

unaligned_done:
        addq.l  #2,sp              ; pop 8 - bit_offset

blit_done:
        addq.l  #4,sp              ; pop row_bits
        lea     SCREEN_W_BYTE(a0),a0
        addq.w  #1,d2              ; y_map_i++

row_while:
        cmp.w   d3,d2              ; while y_map_i <= y_map_max
        ble     row_loop

        addq.l  #2,sp              ; pop x_map_min
        movem.l (sp)+,d3-d7/a3-a5
        unlk    a6
        rts
