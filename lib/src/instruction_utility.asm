;
; utility file emulates various ARM isntructions with required behavior
;

global arithmetic_shift_right
global rotate_right
global rotate_left

section .data

    align 16
    flag_N: dd 0x80000000
    flag_Z: dd 0x40000000
    flag_C: dd 0x20000000
    flag_V: dd 0x10000000
    flag_Q: dd 0x08000000

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
