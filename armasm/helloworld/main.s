
.global _start
.global main

.text
_start:
    .thumb 
    b main

main:
    .thumb
    lsl r7, r3, #17
    lsr r3, r2, #23

.end
