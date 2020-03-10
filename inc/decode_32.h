#pragma once

#include "decode_structure.h"

// toplevel decode function for 32-bit Thumb2 instructions
instruction_t decode_32bit_instruction(unsigned int PC, unsigned int instruction_word);

// Load/store multiple on page A5-20
instruction_t decode_32b_A5_20(unsigned int PC, unsigned int instruction_word);

// Load/store double or exclusive, table branch on page A5-21
instruction_t decode_32b_A5_21(unsigned int PC, unsigned int instruction_word);

// Data processing (shifted register) on page A5-26
instruction_t decode_32b_A5_26(unsigned int PC, unsigned int instruction_word);

// Data processing (modified immediate) on page A5-14
instruction_t decode_32b_A5_14(unsigned int PC, unsigned int instruction_word);

// Data processing (plain binary immediate) on page A5-17
instruction_t decode_32b_A5_17(unsigned int PC, unsigned int instruction_word);

// Branches and miscellaneous control on page A5-18
instruction_t decode_32b_A5_18(unsigned int PC, unsigned int instruction_word);

// Store single data item on page A5-25
instruction_t decode_32b_A5_25(unsigned int PC, unsigned int instruction_word);

// Load byte, memory hints on page A5-24
instruction_t decode_32b_A5_24(unsigned int PC, unsigned int instruction_word);

// Load halfword, unallocated memory hints on page A5-23
instruction_t decode_32b_A5_23(unsigned int PC, unsigned int instruction_word);

// Load word on page A5-22
instruction_t decode_32b_A5_22(unsigned int PC, unsigned int instruction_word);

// Data processing (register) on page A5-27
instruction_t decode_32b_A5_27(unsigned int PC, unsigned int instruction_word);

// Multiply, multiply accumulate, and absolute difference on page A5-29
instruction_t decode_32b_A5_29(unsigned int PC, unsigned int instruction_word);

// Long multiply, long multiply accumulate, and divide on page A5-30
instruction_t decode_32b_A5_30(unsigned int PC, unsigned int instruction_word);

// Coprocessor instructions on page A5-31
instruction_t decode_32b_A5_31(unsigned int PC, unsigned int instruction_word);

