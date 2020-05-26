#pragma once

// this file only exists to avoid cluttering up other 
// files that need to know what the opcodes are

#include <iostream>
typedef int32_t opcode_t;

const opcode_t i_ADC   = 0;  // add with carry
const opcode_t i_ADD   = 1;  // add
const opcode_t i_AND   = 2;  // bitwise AND
const opcode_t i_ASR   = 3;  // arithmetic shift right
const opcode_t i_B     = 4;  // unconditional branch
const opcode_t i_Bxx   = 5;  // conditional branch
const opcode_t i_BIC   = 6;  // bit clear
const opcode_t i_BL    = 7;  // branch and link
const opcode_t i_BX    = 8;  // branch and exchange
const opcode_t i_CBNZ  = 9;  // compare and branch on non-zero
const opcode_t i_CBZ   = 10; // compare and branch on zero
const opcode_t i_CMN   = 11; // compare negative
const opcode_t i_CMP   = 12; // compare
const opcode_t i_EOR   = 13; // bitwise XOR
const opcode_t i_LDMIA = 14; // load multiple
const opcode_t i_LDR   = 15; // load word
const opcode_t i_LDRB  = 16; // load byte
const opcode_t i_LDRH  = 17; // load halfword
const opcode_t i_LSL   = 18; // logical shift left
const opcode_t i_LDSB  = 19; // load sign-extended byte
const opcode_t i_LDSH  = 20; // load sign-extended halfword
const opcode_t i_LSR   = 21; // logical shift right
const opcode_t i_MOV   = 22; // move register
const opcode_t i_MUL   = 23; // multiply
const opcode_t i_MVN   = 24; // move negative register
const opcode_t i_NEG   = 25; // negate
const opcode_t i_NOP   = 26; // nop
const opcode_t i_ORR   = 27; // bitwise OR
const opcode_t i_POP   = 28; // pop registers
const opcode_t i_PUSH  = 29; // push registers
const opcode_t i_ROR   = 30; // rotate right
const opcode_t i_SBC   = 31; // subtract with carry
const opcode_t i_STMIA = 32; // store multiple
const opcode_t i_STR   = 33; // store word
const opcode_t i_STRB  = 34; // store byte
const opcode_t i_STRH  = 35; // store halfword
const opcode_t i_SWI   = 36; // software interrupt
const opcode_t i_SUB   = 37; // subtract
const opcode_t i_TST   = 38; // test bits

const int meta_RRR   = 0; // <op> Reg, Reg, Reg
const int meta_RR    = 1; // <op> Reg, Reg
const int meta_RC    = 2; // <op> Reg, #Constant
const int meta_RRC   = 3; // <op> Reg, Reg, #Constant
const int meta_R     = 4; // <op> Reg
const int meta_RC_pc = 5; // <op> Reg, [PC + #Constant]
const int meta_RC_sp = 6; // <op> Reg, [SP + #Constant]
const int meta_C_sp  = 7; // <op> [SP + #Constant]
const int meta_C     = 8;
const int meta_C_lr  = 9;
const int meta_C_pc  = 10;

/*

ADC   = 4
ADD   = 2*(RRR, RRC), 3(RC), 5*(RR), 12*(RC_pc, RC_sp), 13*(C_sp)
AND   = 4
ASR   = 1(RRC), 4(RR) 
B     = 18
Bxx   = 16* 
BIC   = 4 
BL    = 19
BX    = 5* 
CMN   = 4 
CMP   = 3, 4, 5* 
EOR   = 4 
LDMIA = 15
LDR   = 6(RC_pc), 7(RRR), 9(RRC), 11(RC_sp) 
LDRB  = 7(RRR), 9(RRC) 
LDRH  = 8(RRR), 10(RRC) 
LSL   = 1, 4 
LDSB  = 8 
LDSH  = 8
LSR   = 1, 4 
MOV   = 3, 5* 
MUL   = 4 
MVN   = 4
NEG   = 4 
ORR   = 4
POP   = 14*
PUSH  = 14* 
ROR   = 4
SBC   = 4 
STMIA = 15 
STR   = 7, 9, 11 
STRB  = 7, 9
STRH  = 8, 10 
SWI   = 17
SUB   = 2*(RRR, RRC), 3(RC)
TST   = 4

*/
