
                xdef            _clear_region

base            equ             64              ; offset from SP, not A6
row             equ             68               
col             equ             70             
length          equ             72
width           equ             74

_clear_region:  movem.l         d0-7/a0-6,-(sp)
                lea             zeros,a0

                movea.l         base(sp),a0
                adda.l          col(sp),a0

                move.w          row(sp),d0
                mulu.w          80,d0       # screen width in bytes
                adda.l          d0,a0

                move.w          length(sp),d1
                move.w          width(sp),d2

                

zeros_long:          ds.l            4
zeros_13:            ds.l            1