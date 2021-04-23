#pragma once

#include "core.h"
#include "memory_pool.h"

struct  armstate_t
{
    /* data */
    armv7_m3 cpu;
    memory_t memory;

private:
//public:
    bool ISR_FLAG;

public: 
    //Interrupt enable variable
    static bool RIT;
    static bool EINT0;

    armstate_t(const int mem_endianness);
    armstate_t(void);
    armstate_t(const armstate_t& armstate);

    void set_isr_flag(  void);
    void clear_isr_flag(void);
    bool isr_flagged(   void);

    friend std::ostream& operator<<(std::ostream& os, armstate_t& armstate);
    friend bool operator==(armstate_t& armstate_lhs, armstate_t& armstate_rhs);
    friend bool operator!=(armstate_t& armstate_lhs, armstate_t& armstate_rhs);
    friend bool operator<( armstate_t& armstate_lhs, armstate_t& armstate_rhs);
};

void print_armstate_diff(armstate_t& armstate_lhs, armstate_t& armstate_rhs, std::ostream& os);