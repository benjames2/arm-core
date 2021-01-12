#pragma once

#include "opcodes16.h"

/*
    comments after each opcode indicate which page of the 
    32-bit instruction manual info about it can be found. 
    this information is written differently to the 16-bit 
    Thumb set. the meta opcodes used here are also different 
    from the ones used for Thumb16
*/

const opcode_t t32_ADC    = 0;   // A6.18(imm), A6.20(reg)
const opcode_t t32_ADD    = 1;   // 
const opcode_t t32_ADR    = 2;
const opcode_t t32_AND    = 3;
const opcode_t t32_ASR    = 4;
const opcode_t t32_B      = 5;
const opcode_t t32_BFC    = 6;
const opcode_t t32_BFI    = 7;
const opcode_t t32_BIC    = 8;
const opcode_t t32_BKPT   = 9;
const opcode_t t32_BL     = 10;
const opcode_t t32_BLX    = 11;
const opcode_t t32_BX     = 12;
const opcode_t t32_CBNZ   = 13;
const opcode_t t32_CBZ    = 14;
const opcode_t t32_CDP    = 15;
const opcode_t t32_CDP2   = 16;
const opcode_t t32_CLREX  = 17;
const opcode_t t32_CLZ    = 18;
const opcode_t t32_CMN    = 19;
const opcode_t t32_CMP    = 20;
const opcode_t t32_CPS    = 21;
const opcode_t t32_CPY    = 22;
const opcode_t t32_DBG    = 23;
const opcode_t t32_DMB    = 24;
const opcode_t t32_DSB    = 25;
const opcode_t t32_EOR    = 26;
const opcode_t t32_ISB    = 27;
const opcode_t t32_IT     = 28;
const opcode_t t32_LDC    = 29;
const opcode_t t32_LDC2   = 30;
const opcode_t t32_LDM    = 31;
const opcode_t t32_LDMIA  = 32;
const opcode_t t32_LDMFD  = 33;
const opcode_t t32_LDMDB  = 34;
const opcode_t t32_LDMEA  = 35;
const opcode_t t32_LDR    = 36;
const opcode_t t32_LDRB   = 37;
const opcode_t t32_LDRBT  = 38;
const opcode_t t32_LDRD   = 39;
const opcode_t t32_LDREX  = 40;
const opcode_t t32_LDREXB = 41;
const opcode_t t32_LDREXH = 42;
const opcode_t t32_LDRH   = 43;
const opcode_t t32_LDRHT  = 44;
const opcode_t t32_LDRSB  = 45;
const opcode_t t32_LDRSBT = 46;
const opcode_t t32_LDRSH  = 47;
const opcode_t t32_LDRSHT = 48;
const opcode_t t32_LDRT   = 49;
const opcode_t t32_LSL    = 50;
const opcode_t t32_LSR    = 51;
const opcode_t t32_MCR    = 52;
const opcode_t t32_MCR2   = 53;
const opcode_t t32_MCRR   = 54;
const opcode_t t32_MCRR2  = 55;
const opcode_t t32_MLA    = 56;
const opcode_t t32_MLS    = 57;
const opcode_t t32_MOV    = 58;
const opcode_t t32_MOVT   = 59;
const opcode_t t32_MRC    = 60;
const opcode_t t32_MRC2   = 61;
const opcode_t t32_MRRC   = 62;
const opcode_t t32_MRRC2  = 63;
const opcode_t t32_MRS    = 64;
const opcode_t t32_MSR    = 65;
const opcode_t t32_MUL    = 66;
const opcode_t t32_MVN    = 67;
const opcode_t t32_NEG    = 68;
const opcode_t t32_NOP    = 69;
const opcode_t t32_ORN    = 70;
const opcode_t t32_ORR    = 71;
const opcode_t t32_PLD    = 72;
const opcode_t t32_PLI    = 73;
const opcode_t t32_POP    = 74;
const opcode_t t32_PUSH   = 75;
const opcode_t t32_RBIT   = 76;
const opcode_t t32_REV    = 77;
const opcode_t t32_REV16  = 78;
const opcode_t t32_REVSH  = 79;
const opcode_t t32_ROR    = 80;
const opcode_t t32_RRX    = 81;
const opcode_t t32_RSB    = 82;
const opcode_t t32_SBC    = 83;
const opcode_t t32_SBFX   = 84;
const opcode_t t32_SDIV   = 85;
const opcode_t t32_SEV    = 86;
const opcode_t t32_SMLAL  = 87;
const opcode_t t32_SMULL  = 88;
const opcode_t t32_SSAT   = 89;
const opcode_t t32_STC    = 90;
const opcode_t t32_STC2   = 91;
const opcode_t t32_STM    = 92;
const opcode_t t32_STMIA  = 93;
const opcode_t t32_STMEA  = 94;
const opcode_t t32_STMDB  = 95;
const opcode_t t32_STMFD  = 96;
const opcode_t t32_STR    = 97;
const opcode_t t32_STRB   = 98;
const opcode_t t32_STRBT  = 99;
const opcode_t t32_STRD   = 100;
const opcode_t t32_STREX  = 101;
const opcode_t t32_STREXB = 102;
const opcode_t t32_STREXH = 103;
const opcode_t t32_STRH   = 104;
const opcode_t t32_STRHT  = 105;
const opcode_t t32_STRT   = 106;
const opcode_t t32_SUB    = 107;
const opcode_t t32_SVC    = 108;
const opcode_t t32_SXTB   = 109;
const opcode_t t32_SXTH   = 110;
const opcode_t t32_TBB    = 111;
const opcode_t t32_TBH    = 112;
const opcode_t t32_TEQ    = 113;
const opcode_t t32_TST    = 114;
const opcode_t t32_UBFX   = 115;
const opcode_t t32_UDIV   = 116;
const opcode_t t32_UMLAL  = 117;
const opcode_t t32_UMULL  = 118;
const opcode_t t32_USAT   = 119;
const opcode_t t32_UXTB   = 120;
const opcode_t t32_UXTH   = 121;
const opcode_t t32_WFE    = 122;
const opcode_t t32_WFI    = 123;
const opcode_t t32_YIELD  = 124;

const int meta_t32_imm          = 0;
const int meta_t32_reg          = 1;
const int meta_t32_SP_imm       = 2;
const int meta_t32_SP_reg       = 3;
const int meta_t32_literal      = 4;
const int meta_t32_shreg        = 5; // shifted register
const int meta_t32_SP_minus_imm = 6;
const int meta_t32_SP_minus_reg = 7;

