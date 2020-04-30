#pragma once

#include "core.h"
#include "memory_pool.h"

void push_word(armv7_m3& cpu, memory_t& memory, int32_t w);

void push_word(armv7_m3& cpu, memory_t& memory, uint32_t w);

void push_register(armv7_m3& cpu, memory_t& memory, int regn);

uint32_t pop_word(armv7_m3& cpu, memory_t& memory);
