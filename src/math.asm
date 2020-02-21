;
; file contains assembly routines used for all math operations used by the emulator
;
; as per the System-V ABI, registers are allocated in the order:
;   rdi, rsi, rdx, rcx, r8, r9
; this order is used when asm routines are called from C routines
; none of these registers are preserved across function calls
; if a caller needs to preserve the values in these registers, it is 
; responsible for saving them
;

global get_flags_register ; just return the flags register, unreliable when mixed with C/C++
;global perform_add        ; standard add, no carry in
;global perform_sub        ; standard subtract, no borrow
global gp_operation       ; general purpose, pass a flag that indicates what the operation is

section .data

msg_add: db "(asm) add", 0x00 ; null terminated
msg_sub: db "(asm) sub", 0x00 ; ...
msg_adc: db "(asm) adc", 0x00 ; add with carry
msg_sbb: db "(asm) sbb", 0x00 ; subtract with borrow
msg_error: db "(asm) results_t ptr is NULL", 0x00
msg_bad_op: db "(asm) op flag does not specify a valid operation", 0x00

section .text

align 16
gp_operation:
    ; rdi : pointer to results_t structure
    ; rsi : a
    ; rdx : b
    ; rcx : carry/borrow value (either 1 or 0)
    ; r8 : operation flag
    ;     0 : add
    ;     1 : subtract
    ;     2 : add w/ carry
    ;     3 : subtract w/ borrow
    ;
    ; for functions that dont use the carry/borrow, it is ignored, but the order 
    ; of arguments MUST be maintained for C compatibility reasons

    push rbp     ; create stack frame
    mov rbp, rsp ; ...

    cmp rdi, 0   ; make sure rdi is non-zero
    jnz gp_begin ; skip over nullptr error handler

    ; error handler: results_t ptr is NULL which means results cant be stored
    mov rax, msg_error
    pop rbp ; destroy stack frame
    ret

    ; figure out which operation has been requested
gp_begin:

  gp_op_eval:
    cmp r8, 0
    jz perform_add ; jump if rb == 0
    cmp r8, 1
    jz perform_sub ; jump if rb == 1
    cmp r8, 2
    jz perform_adc
    ; default behavior is to return an error message

    ; error handler: flag does not specify a valid operation
    mov rax, msg_bad_op
    pop rbp ; destroy stack frame
    ret

align 16
perform_add:
    ; perform the proper addition and return the flags register
    ;
    ; rdi : pointer to results_t structure
    ; rsi : a
    ; rdx : b
    ;
    ; routine performs a += b, yields a
    add esi, edx   ; perform the math operation (32-bit operands)
    pushfq         ; save flags register to stack immediately
    pop rcx        ; stupid way that we have to mov flags to different register
    mov [rdi], ecx ; save flags in structure
    mov [rdi + 4], esi ; save results of math operation in results_t structure

    mov rax, msg_add ; '(asm) add'
    pop rbp          ; destroy stack frame
    ret

align 16
perform_sub:
    ; perform the proper addition and return the flags register
    ;
    ; rdi : pointer to results_t structure
    ; rsi : a
    ; rdx : b
    ;
    ; routine performs a -= b, yields a
    sub esi, edx   ; perform the math operation (32-bit operands)
    pushfq         ; save flags register to stack immediately
    pop rcx        ; stupid way that we have to mov flags to different register
    mov [rdi], ecx ; save flags in structure
    mov [rdi + 4], esi ; save results of math operation in results_t structure

    mov rax, msg_sub ; '(asm) sub'
    pop rbp          ; destroy stack frame
    ret

align 16
perform_adc:
    ;
    ; rdi : pointer to results_t structure
    ; rsi : a
    ; rdx : b
    ; rcx : carry (either zero or non-zero)
    ;
    ; performs a += (b + carry), yields a

    ; make sure the carry flag is set properly
    clc ; clear existing carry flag, dont want it to interfere
    cmp ecx, 0 ; test if rcx is zero
    jz do_adc  ; and skip next instruction if it is
    stc ; set carry flag

  do_adc:
    adc esi, edx ; perform addition with carry flag set properly
    pushfq  ; save flags
    pop rcx ; ... into different register
    mov [rdi], ecx     ; save flags in memory
    mov [rdi + 4], esi ; save result in memory

    mov rax, msg_adc ; '(asm) adc'
    pop rbp          ; destroy stack frame
    ret

align 16
perform_sbb:
    ;
    ; rdi : pointer to results_t structure
    ; rsi : a
    ; rdx : b
    ; rcx : borrow
    ;

    clc        ; clear carry flag
    cmp ecx, 0 ; test if borrow is zero
    jz do_sbb  ; skip next instruction if borrow is zero
    stc        ; set carry flag

  do_sub:
    sbb esi, edx
    pushfq  ; save flags
    pop rcx ; pop flags into different register for sakekeeping
    mov [rdi], ecx     ; store flags in memory
    mov [rdi + 4], esi ; store result in memory

    ; return correct message and clean up stack frame
    mov rax, msg_sbb
    pop rbp
    ret

align 16
get_flags_register:
    ; simply return the rFLAGS register
    ; x86 does not allow mov'ing the flags register directly. we must first 
    ; push and then pop into a different register
    pushfq
    pop rax
    ret
    ; dont ever call this routine for serious calculations
