#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "core.h"
#include "memory_pool.h"
#include "decode_structure.h"

armv7_m3 execute(armv7_m3& cpu, memory_t& memory, decoded_instruction_t& inst);

/*


    Fetch
    Decode
    Execute


    os << opcodename;
    if(in.S)
        os << 's';



    // rest of instruction

*/
