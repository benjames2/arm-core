#pragma once

#include <iostream>
#include <string>
#include "opcodes16.h"
#include "opcodes32.h"

struct instruction_t {

    // which specific opcode is this
    opcode_t opcode;
    int meta_opcode; // many opcodes have multiple uses
    int condition_code; // the branch instruction have many conditions 

    uint32_t PC; // always contains PC value of next instruction

    // some instructions contain an immediate
    union {
        int i_immediate;
        unsigned int u_immediate;
    };

    // register references
    int Rs, Rd, Rn, Rb, Ro;
    //int Hs, Hd; // HI register references

    union {
        int Rlist;
        int H;
    };

    std::string str(void);

    instruction_t(void);

};

typedef instruction_t instruction_16b_t;

// operator overload to print instruction_t data (sort of like disassembling)
std::ostream& operator<<(std::ostream& os, instruction_t& in);

struct instruction_32b_t {

    opcode_t opcode;
    int meta_opcode;
    int encoding;

    int P, W, M, U, H, S;
    int Rn, Rd, Rm, Rt, Rt2, Rlist;

    union {
        uint32_t u32;
        int32_t  i32;
        int32_t  imm32;
    };

    static const int encoding_T1 = 1;
    static const int encoding_T2 = 2;
    static const int encoding_T3 = 3;
    static const int encoding_T4 = 4;

};

std::ostream& operator<<(std::ostream& os, instruction_32b_t& in);

struct decoded_instruction_t {

    int instruction_type;

    union {
        instruction_16b_t di_t16;
        instruction_32b_t di_t32;
    };

    static const int t16 = 0;
    static const int t32 = 1;

};


/*

    fetch first 2 bytes

    if 16-bit instruction
        - call 16 bit decode
    else
        fetch the two bytes after that
        - call 32 bit decode

    return decoded_instruction_t

*/
