#pragma once

// decode.h/.cpp contains functions that generate an instruction_t 
// type that completely describes the operations needed to complete 
// a full instruction execute action. it contains all data needed 
// to carry out an instruction

#include <iostream>
#include "opcodes.h"

struct instruction_t {

    // which specific opcode is this
    opcode_t opcode;
    int meta_opcode; // many opcodes have multiple uses

    uint32_t PC; // always contains PC value of next instruction

    // some instructions contain an immediate
    union {
        int i_immediate;
        unsigned int u_immediate;
    };

    // register references
    int Rs, Rd, Rn, Rb, Ro;
    int Hs, Hd; // HI register references

    int Rlist;

    instruction_t(void);

};

// highest level decode function
instruction_t decode_instruction(unsigned int PC, unsigned int instruction_word);


instruction_t decode_format_1(  unsigned int PC, unsigned int instruction_word ); // move shifted register
instruction_t decode_format_2(  unsigned int PC, unsigned int instruction_word ); // add/subtract
instruction_t decode_format_3(  unsigned int PC, unsigned int instruction_word ); // move/compare/add/subtract immediate
instruction_t decode_format_4(  unsigned int PC, unsigned int instruction_word ); // ALU operations
instruction_t decode_format_5(  unsigned int PC, unsigned int instruction_word ); // HI register operations/branch exchange (technically 11 instructions)
instruction_t decode_format_6(  unsigned int PC, unsigned int instruction_word ); // PC-relative load
instruction_t decode_format_7(  unsigned int PC, unsigned int instruction_word ); // load/store with register offset
instruction_t decode_format_8(  unsigned int PC, unsigned int instruction_word ); // load/store sign-extended byte/halfword
instruction_t decode_format_9(  unsigned int PC, unsigned int instruction_word ); // load/store with immediate offset
instruction_t decode_format_10( unsigned int PC, unsigned int instruction_word ); // load/store halfword
instruction_t decode_format_11( unsigned int PC, unsigned int instruction_word ); // SP-relative load/store
instruction_t decode_format_12( unsigned int PC, unsigned int instruction_word ); // load address
instruction_t decode_format_13( unsigned int PC, unsigned int instruction_word ); // add offset to stack pointer
instruction_t decode_format_14( unsigned int PC, unsigned int instruction_word ); // push/pop registers
instruction_t decode_format_15( unsigned int PC, unsigned int instruction_word ); // multiple load/store
instruction_t decode_format_16( unsigned int PC, unsigned int instruction_word ); // conditional branch
instruction_t decode_format_17( unsigned int PC, unsigned int instruction_word ); // software interrupt
instruction_t decode_format_18( unsigned int PC, unsigned int instruction_word ); // unconditional branch
instruction_t decode_format_19( unsigned int PC, unsigned int instruction_word ); // long branch with link





