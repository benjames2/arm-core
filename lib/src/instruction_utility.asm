;
; utility file emulates various ARM isntructions with required behavior
;

global arithmetic_shift_right
global rotate_right
global rotate_left

section .data
section .text

align 16
arithmetic_shift_right:
    mov rax, rdi
    sar rax, 1
    ret

align 16
rotate_right:
    mov rax, rdi
    ror rax, 1
    ret

align 16
rotate_left:
    mov rax, rdi
    rol rax, 1
    ret
