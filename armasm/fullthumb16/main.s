@.global _start
@.global main
.syntax unified

.section .text
_start: 
    @b main

main:

    @ format 1
    lsls r0, r3, #23
    lsrs r1, r4, #17
    asrs r2, r5, #15

    @ format 2
    adds r7, r6, r5
    adds r3, r4, #7
    subs r4, r1, r2
    subs r0, r0, #7

    @ format 3
    movs r2, #255
    cmp r5, #128
    adds r1, #32
    subs r3, #230

    @ format 4
    ands r6, r3
    eors r2, r7
    lsls r3, r0
    lsrs r0, r1
    asrs r4, r6
    adcs r1, r2
    sbcs r5, r4
    rors r6, r3
    tst r2, r7
    negs r3, r0
    cmp r0, r1
    cmn r4, r6
    orrs r1, r2
    muls r5, r4
    bics r6, r3
    mvns r2, r7

    @ format 5
    add r3, r8  @ these three add insts use a hi register
    add r12, r7
    add r10, r9
    cmp r4, r9  @ ... cmp
    cmp r13, r6
    cmp r11, r10
    mov r5, r10
    mov r14, r3
    mov r10, r11
    bx r7
    bx r14

    @ format 6
    ldr r3, [PC, #12]

    @ format 7
    str  r3, [r0, r7]
    strb r4, [r1, r6]
    ldr  r6, [r2, r3]
    ldrb r1, [r3, r4]

    @ format 8
    strh r2, [r4, r0]
    ldrh r0, [r5, r1]
    ldsb r1, [r3, r2] 
    ldsh r3, [r4, r1]

    @ format 9
    str r4, [r2, #124]
    ldr r2, [r7, #92]
    strb r3, [r0, #31]
    ldrb r7, [r6, #23]

    @ format 10
    strh r0, [r4, #62]
    ldrh r7, [r2, #2]

    @ format 11
    str r3, [SP, #1020]
    ldr r5, [SP, #960]

    @ format 12
    add r0, PC, #1020
    add r4, SP, #960

    @ format 13
    add SP, #508
    add.n SP, #-508 @ <-- needs the explicit narrow convert flag

    @ format 14 <-- TODO : properly implement decode/execute/print logic
    push { r0, r1, r6, r7 }
    push { r0, r1, r6, r7, LR }
    push { r0-r7 }
    push { r0-r7, LR }
    pop { r0, r1, r6, r7 }
    pop { r0, r1, r6, r7, PC }
    pop { r0-r7 }
    pop { r0-r7, PC }

    @ format 15
    stmia r3!, { r0, r4-r7 }
    ldmia r3!, { r0-r2, r7 }

    @ format 16
    beq extra
    bne extra
    bcs extra
    bcc extra
    bmi extra 
    bpl extra
    bvs extra
    bvc extra
    bhi extra
    bls extra
    bge extra
    blt extra
    bgt extra
    ble extra

    @ format 17
    swi 255
    swi 0

    @ format 18
    b extra

    @ format 19
    bl extra @ cannot coerce into 16-bit instruction :(
             @ according to the 32-bit manual, a 16-bit form 
             @ of this instruction does not exist

endprog: @ leave this label at the end of the .text section
.align 10

.section .data
    nothing:
    .word (endprog-_start) @ <-- last part of file
