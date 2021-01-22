#pragma once

#include "core.h"
#include "memory_pool.h"

address32_t push_word(armv7_m3& cpu, memory_t& memory, address32_t addr, uint32_t word);
address32_t push_word(armv7_m3& cpu, memory_t& memory, address32_t addr, int32_t  word);

address32_t pop_word(armv7_m3& cpu, memory_t& memory, address32_t addr, int regn);
