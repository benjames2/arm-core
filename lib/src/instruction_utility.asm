;
; utility file emulates various ARM isntructions with required behavior
;

global arithmetic_shift_right

section .data
section .text

align 16
arithmetic_shift_right:
    mov rax, rdi
    sar rax, rsi
    ret


