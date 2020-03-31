#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "core.h"
#include "memory_pool.h"

armv7_m3 execute_t16(armv7_m3& cpu, memory_t& memory, instruction_t& inst);
armv7_m3 execute_t32(armv7_m3& cpu, memory_t& memory, instruction_32b_t& inst);


/*


    Fetch
    Decode
    Execute


*/






