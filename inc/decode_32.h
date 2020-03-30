#pragma once

#include "decode_structure.h"

// toplevel decode function for 32-bit Thumb2 instructions
instruction_32b_t  decode_32bit_instruction(unsigned int PC, unsigned int instruction_word);


/**********************************************************************************
    Second level decode functions for 32-bit Thumb2 insructions
***********************************************************************************/
// Load/store multiple on page A5-20
instruction_32b_t  decode_32b_A5_20(unsigned int PC, unsigned int instruction_word);

// Load/store double or exclusive, table branch on page A5-21
instruction_32b_t  decode_32b_A5_21(unsigned int PC, unsigned int instruction_word);

// Data processing (shifted register) on page A5-26
instruction_32b_t  decode_32b_A5_26(unsigned int PC, unsigned int instruction_word);

// Data processing (modified immediate) on page A5-14
instruction_32b_t  decode_32b_A5_14(unsigned int PC, unsigned int instruction_word);

// Data processing (plain binary immediate) on page A5-17
instruction_32b_t  decode_32b_A5_17(unsigned int PC, unsigned int instruction_word);

// Branches and miscellaneous control on page A5-18
instruction_32b_t  decode_32b_A5_18(unsigned int PC, unsigned int instruction_word);

// Store single data item on page A5-25
instruction_32b_t  decode_32b_A5_25(unsigned int PC, unsigned int instruction_word);

// Load byte, memory hints on page A5-24
instruction_32b_t  decode_32b_A5_24(unsigned int PC, unsigned int instruction_word);

// Load halfword, unallocated memory hints on page A5-23
instruction_32b_t  decode_32b_A5_23(unsigned int PC, unsigned int instruction_word);

// Load word on page A5-22
instruction_32b_t  decode_32b_A5_22(unsigned int PC, unsigned int instruction_word);

// Data processing (register) on page A5-27
instruction_32b_t  decode_32b_A5_27(unsigned int PC, unsigned int instruction_word);

// Multiply, multiply accumulate, and absolute difference on page A5-29
instruction_32b_t  decode_32b_A5_29(unsigned int PC, unsigned int instruction_word);

// Long multiply, long multiply accumulate, and divide on page A5-30
instruction_32b_t  decode_32b_A5_30(unsigned int PC, unsigned int instruction_word);

// Coprocessor instructions on page A5-31
instruction_32b_t  decode_32b_A5_31(unsigned int PC, unsigned int instruction_word);


/*******************************************************************************************************************************
                        Third level decode functions for 32-bit Thumb2 insructions
*******************************************************************************************************************************/
instruction_32b_t decode_32b_A6_18_ADC_imm(      unsigned int PC, unsigned int instruction_word); //add with carry immediate
instruction_32b_t decode_32b_A6_22_ADD_imm(      unsigned int PC, unsigned int instruction_word); //add mmediate
instruction_32b_t decode_32b_A6_32_AND_imm(      unsigned int PC, unsigned int instruction_word); //bitwise and immediate
instruction_32b_t decode_32b_A6_44_BIC_imm(      unsigned int PC, unsigned int instruction_word); //bitwise clear immeddiate
instruction_32b_t decode_32b_A6_58_CMN_imm(      unsigned int PC, unsigned int instruction_word); //compare negative immediate
instruction_32b_t decode_32b_A6_62_CMP_imm(      unsigned int PC, unsigned int instruction_word); //compare immediate
instruction_32b_t decode_32b_A6_72_EOR_imm(      unsigned int PC, unsigned int instruction_word); //bitwiswe exclusive OR immediate
instruction_32b_t decode_32b_A6_84_LDM(          unsigned int PC, unsigned int instruction_word); //load multiple
instruction_32b_t decode_32b_A6_88_LDR_imm_T3(   unsigned int PC, unsigned int instruction_word); //load register immediate T3 encoding
instruction_32b_t decode_32b_A6_88_LDR_imm_T4(   unsigned int PC, unsigned int instruction_word); //load register immediate T4 encoding
instruction_32b_t decode_32b_A6_90_LDR_lit(      unsigned int PC, unsigned int instruction_word); //load register (literal)
instruction_32b_t decode_32b_A6_92_LDR_reg(      unsigned int PC, unsigned int instruction_word); //load register (register)
instruction_32b_t decode_32b_A6_102_LDRD_imm(    unsigned int PC, unsigned int instruction_word); //load register dual immediate
instruction_32b_t decode_32b_A6_104_LDRD_lit(    unsigned int PC, unsigned int instruction_word); //load register dual (literal)
instruction_32b_t decode_32b_A6_106_LDREX(       unsigned int PC, unsigned int instruction_word); //load register exclusive
instruction_32b_t decode_32b_A6_107_LDREXB(      unsigned int PC, unsigned int instruction_word); //load register exclusive byte
instruction_32b_t decode_32b_A6_108_LDREXH(      unsigned int PC, unsigned int instruction_word); //load register exclusive halfword
instruction_32b_t decode_32b_A6_133_LDRT(        unsigned int PC, unsigned int instruction_word); //load register unprivileged
instruction_32b_t decode_32b_A6_148_MOV_imm(     unsigned int PC, unsigned int instruction_word); //move immediate
instruction_32b_t decode_32b_A6_162_MVN_imm(     unsigned int PC, unsigned int instruction_word); //bitwise not immediate
instruction_32b_t decode_32b_A6_168_ORN_imm(     unsigned int PC, unsigned int instruction_word); //bitwise OR NOT immediate
instruction_32b_t decode_32b_A6_172_ORR_imm(     unsigned int PC, unsigned int instruction_word); //bitwise inclusive OR
instruction_32b_t decode_32b_A6_184_POP(         unsigned int PC, unsigned int instruction_word); //pop multiple registers
instruction_32b_t decode_32b_A6_186_PUSH(        unsigned int PC, unsigned int instruction_word); //push multiple registers
instruction_32b_t decode_32b_A6_198_RSB_imm(     unsigned int PC, unsigned int instruction_word); //reverse substact immediate
instruction_32b_t decode_32b_A6_202_SBC_imm(     unsigned int PC, unsigned int instruction_word); //substract with carry immediate
instruction_32b_t decode_32b_A6_216_STM(         unsigned int PC, unsigned int instruction_word); //store multiple
instruction_32b_t decode_32b_A6_218_STMDB(       unsigned int PC, unsigned int instruction_word); //store multiple decrement before
instruction_32b_t decode_32b_A6_220_STR_imm_T3(  unsigned int PC, unsigned int instruction_word); //
instruction_32b_t decode_32b_A6_220_STR_imm_T4(  unsigned int PC, unsigned int instruction_word); //
instruction_32b_t decode_32b_A6_222_STR_reg(     unsigned int PC, unsigned int instruction_word); //
instruction_32b_t decode_32b_A6_224_STRB_imm_T2( unsigned int PC, unsigned int instruction_word); //
instruction_32b_t decode_32b_A6_224_STRB_imm_T3( unsigned int PC, unsigned int instruction_word); //
instruction_32b_t decode_32b_A6_226_STRB_reg(    unsigned int PC, unsigned int instruction_word); //
instruction_32b_t decode_32b_A6_232_STREX(       unsigned int PC, unsigned int instruction_word); //store register exclusive
instruction_32b_t decode_32b_A6_230_STRD_imm(    unsigned int PC, unsigned int instruction_word); //store register dual immediate
instruction_32b_t decode_32b_A6_233_STREXB(      unsigned int PC, unsigned int instruction_word); //store register excluaive byte
instruction_32b_t decode_32b_A6_234_STREXH(      unsigned int PC, unsigned int instruction_word); //store register exclusive halfword
instruction_32b_t decode_32b_A6_236_STRH_imm_T2( unsigned int PC, unsigned int instruction_word); //
instruction_32b_t decode_32b_A6_236_STRH_imm_T3( unsigned int PC, unsigned int instruction_word); //
instruction_32b_t decode_32b_A6_238_STRH_reg(    unsigned int PC, unsigned int instruction_word); //
instruction_32b_t decode_32b_A6_242_SUB_imm(     unsigned int PC, unsigned int instruction_word); //substract immediate
instruction_32b_t decode_32b_A6_256_TBB(         unsigned int PC, unsigned int instruction_word); //table branch byte 
instruction_32b_t decode_32b_A6_256_TBH(         unsigned int PC, unsigned int instruction_word); //table branch halfword
instruction_32b_t decode_32b_A6_258_TEQ_imm(     unsigned int PC, unsigned int instruction_word); //test equvalence immediate
instruction_32b_t decode_32b_A6_260_TST_imm(     unsigned int PC, unsigned int instruction_word); //test immediate