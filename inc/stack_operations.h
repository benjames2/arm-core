#pragma once

#include "core.h"
#include "memory_pool.h"

// functions for regular push/pop instructions
void push_word(armv7_m3& cpu, memory_t& memory, int32_t w);
void push_word(armv7_m3& cpu, memory_t& memory, uint32_t w);
void push_register(armv7_m3& cpu, memory_t& memory, int regn);
uint32_t pop_word(armv7_m3& cpu, memory_t& memory);


// helper functions for stm** and ldm** instructions

// returns the 'next' address
uint32_t push_word(armv7_m3& cpu, memory_t& memory, address_t addr, uint32_t w);
uint32_t push_word(armv7_m3& cpu, memory_t& memory, address_t addr, int32_t w);
