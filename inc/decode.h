#pragma once

#include "decode_structure.h"
#include "fetch.h"

struct decoded_instruction_t {

    int type;

    union {
        instruction_16b_t di_t16;
        instruction_32b_t di_t32;
    };

    static const int t16 = 0;
    static const int t32 = 1;

};

auto decode(fetched_instruction_t instruction, uint32_t PC) -> decoded_instruction_t;

std::ostream& operator<<(std::ostream& os, const decoded_instruction_t& inst);