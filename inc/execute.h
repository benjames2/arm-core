#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "core.h"
#include "memory_pool.h"
#include "decode_structure.h"
#include "decode.h"

armstate_t execute(armstate_t& armstate, decoded_instruction_t& inst);
