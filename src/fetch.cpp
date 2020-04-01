#include <inc/fetch.h>
#include <inc/memory_pool.h>

fetched_instruction_t fetch(memory_t& memory, uint32_t PC) {

    // contains which encoding is used
    fetched_instruction_t fi;

    // memory_t object handles all endianness issues
    uint32_t first_halfword = memory.load_u16(PC);

    int flag_bits = (first_halfword >> 11) & 0x1F;
    switch(flag_bits) {
        case 0b11101:
        case 0b11110:
        case 0b11111:
            // 32-bit THUMB, need next halfword as well
            {
                // 4 byte instructions NEED to sit on 4 byte boundaries
                if(PC % 4)
                    throw std::runtime_error("fetch : THUMB32 instructions must lie on 32-bit boundary");

                uint32_t second_halfword = memory.load_u16(PC+2);

                fi.in   = (first_halfword << 16) | second_halfword;
                fi.type = fetched_instruction_t::t32;
                return fi;
            }
        default:
            // 16-bit THUMB, first halfword is entire instruction
            fi.in   = first_halfword;
            fi.type = fetched_instruction_t::t16;
            return fi;
    }

}
