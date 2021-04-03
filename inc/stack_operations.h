#pragma once

#include "core.h"
#include "memory_pool.h"
#include "armstate.h"

address32_t push_word(armstate_t& armstate, address32_t addr, uint32_t word);
address32_t push_word(armstate_t& armstate, address32_t addr, int32_t  word);

address32_t pop_word(armstate_t& armstate, address32_t addr, int regn);
