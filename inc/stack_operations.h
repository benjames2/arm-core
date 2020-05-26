#pragma once

#include "core.h"
#include "memory_pool.h"

address_t push_word(armv7_m3& cpu, memory_t& memory, address_t addr, uint32_t word);
address_t push_word(armv7_m3& cpu, memory_t& memory, address_t addr, int32_t  word);

address_t pop_word(armv7_m3& cpu, memory_t& memory, address_t addr, int regn);
