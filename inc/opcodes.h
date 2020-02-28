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
const opcode_t i_CMN   = 9;  // compare negative
const opcode_t i_CMP   = 10; // compare
const opcode_t i_EOR   = 11; // bitwise XOR
const opcode_t i_LDMIA = 12; // load multiple
const opcode_t i_LDR   = 13; // load word
const opcode_t i_LDRB  = 14; // load byte
const opcode_t i_LDRH  = 15; // load halfword
const opcode_t i_LSL   = 16; // logical shift left
const opcode_t i_LDSB  = 17; // load sign-extended byte
const opcode_t i_LDSH  = 18; // load sign-extended halfword
const opcode_t i_LSR   = 19; // logical shift right
const opcode_t i_MOV   = 20; // move register
const opcode_t i_MUL   = 21; // multiply
const opcode_t i_MVN   = 22; // move negative register
const opcode_t i_NEG   = 23; // negate
const opcode_t i_ORR   = 24; // bitwise OR
const opcode_t i_POP   = 25; // pop registers
const opcode_t i_PUSH  = 26; // push registers
const opcode_t i_ROR   = 27; // rotate right
const opcode_t i_SBC   = 28; // subtract with carry
const opcode_t i_STMIA = 29; // store multiple
const opcode_t i_STR   = 30; // store word
const opcode_t i_STRB  = 31; // store byte
const opcode_t i_STRH  = 32; // store halfword
const opcode_t i_SWI   = 33; // software interrupt
const opcode_t i_SUB   = 34; // subtract
const opcode_t i_TST   = 35; // test bits

const int meta_RRR = 0;   // <op> Reg, Reg, Reg
const int meta_RR  = 1;   // <op> Reg, Reg
const int meta_RC  = 2;   // <op> Reg, #Constant
const int meta_RRC = 3;   // <op> Reg, Reg, #Constant
const int meta_R   = 4;   // <op> Reg
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
LDRB  = 7, 9 
LDRH  = 8, 10 
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
