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
        int32_t i_immediate;
        uint32_t u_immediate;
        int32_t i32;
        uint32_t u32;
    };

    // register references
    int Rs, Rd, Rn, Rb, Ro;
    //int Hs, Hd; // HI register references

    union {
        int Rlist;
        int H;
    };

    std::string str(void);
    
    //instruction_t(void);

};

typedef instruction_t instruction_16b_t;

// operator overload to print instruction_t data (sort of like disassembling)
std::ostream& operator<<(std::ostream& os, const instruction_t& in);

struct instruction_32b_t {

    opcode_t opcode;
    int meta_opcode;
    int encoding;

    int P, W, M, U, H, S;
    int Rn, Rd, Rm, Rt, Rt2, Rlist;

    union {
        uint32_t u32;
        int32_t  i32;
    };

    static const int encoding_T1 = 1;
    static const int encoding_T2 = 2;
    static const int encoding_T3 = 3;
    static const int encoding_T4 = 4;

    std::string str(void);

};

// operator overload to print instruction_32b_t data
std::ostream& operator<<(std::ostream& os, const instruction_32b_t& in);

