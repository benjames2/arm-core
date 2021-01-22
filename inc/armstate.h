#pragma once

#include "core.h"
#include "memory_pool.h"

struct  armstate_t
{
    /* data */
    armv7_m3 cpu;
    memory_t memory;


public:

    armstate_t(const int mem_endianness);

    friend std::ostream operator<<(std::ostream& os, armstate_t& armstate);
};
