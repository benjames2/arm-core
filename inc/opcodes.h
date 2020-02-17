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

const int meta_RRR = 0;
const int meta_RR  = 1;
const int meta_RC  = 2;
const int meta_RRC = 3;

//const int meta_ADD_RRR = 0; // add Reg, Reg, Reg
//const int meta_ADD_RRC = 1; // add Reg, Reg, #const
//const int meta_SUB_RRR = 2; // sub Reg, Reg, Reg
//const int meta_SUB_RRC = 3; // sub Reg, Reg, #const
//const int meta_MOV_RC  = 4; // mov Reg, #const
//const int meta_CMP_RC  = 5; // cmp Reg, #const
//const int meta_ADD_RC  = 6; // add Reg, #const
//const int meta_SUB_RC  = 7; // sub Reg, #const


