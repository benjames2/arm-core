;
; This asm file contains small routines that perform small 
; tasks like byte-swapping in a few instructions
;
; for the byte-swapping routines, the integer to be swapped is passed in RDI
;
; this file will cause a compiler warning because it wont 
; link with anything but the final executable. THIS IS OK!!
;

global byte_swap_64
global byte_swap_32
global byte_swap_16

section .data

section .text

align 16
byte_swap_64:
    mov rax, rdi
    bswap rax
    ret

align 16
byte_swap_32:
    mov rax, rdi
    bswap eax
    ret

align 16
byte_swap_16:
    mov rax, rdi
    shl rax, 8
    bswap eax
    shr rax, 8
    ret
