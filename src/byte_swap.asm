;
; This asm file contains small routines that perform small
; tasks like byte-swapping in a few instructions
;
; for the byte-swapping routines, the integer to be swapped is passed in RDI
; swapped integer is returned in RAX
;

global byte_swap_64
global byte_swap_32
global byte_swap_16

section .data

section .text

align 16
byte_swap_64:
    ; dont need to generate a stack frame because
    ; this is a leaf routine. the same goes for
    ; the other byte swap routines
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