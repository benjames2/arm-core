
.global _start
.global main

.text
_start: 
    .thumb
    b main

main:
    .thumb
    @ format 1
    lsl r0, r3, #23
    lsr r1, r4, #17
    asr r2, r5, #15

    @ format 2
    add r7, r6, r5
    add r3, r4, #7
    sub r4, r1, r2
    sub r0, r0, #7

    @ format 3
    mov r2, #255
    cmp r5, #128
    add r1, #32
    sub r3, #230

    @format 4
    and r6, r3
    eor r2, r7
    lsl r3, r0
    lsr r0, r1
    asr r4, r6
    adc r1, r2
    sbc r5, r4
    ror r6, r3
    tst r2, r7
    neg r3, r0
    cmp r0, r1
    cmn r4, r6
    orr r1, r2
    mul r5, r4
    bic r6, r3
    mvn r2, r7

    @format 5
    add r3, r8
    add r12, r7
    add r10, r9
    cmp r4, r9
    cmp r13, r6
    cmp r11, r10
    mov r5, r10
    mov r14, r3
    mov r10, r11
    bx r7
    bx r14

    @format 6
    .arm
    adcs r8, r9, #2
