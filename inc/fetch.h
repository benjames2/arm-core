#pragma once

#include <cstddef>

#include "memory_pool.h"

struct fetched_instruction_t {
    int type;
    uint32_t in;

    // instruction_type field has one of these two values:
    static const int t16 = 0;
    static const int t32 = 1;
};

fetched_instruction_t fetch(memory_t& memory);
