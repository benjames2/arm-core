#include <iostream>
#include <sstream>
#include <inc/fetch.h>
#include <inc/memory_pool.h>

fetched_instruction_t fetch(memory_t& memory, uint32_t PC, const bool should_print) {

    // contains which encoding is used
    fetched_instruction_t fi;

    // memory_t object handles all endianness issues
    uint32_t first_halfword = memory.load_u16(PC);

    auto pad_hex_number = [](uint32_t addr) -> std::string {
        std::stringstream ss;
        ss << std::hex << addr;
        std::string s = ss.str();

        while(s.size() < 8)
            s = "0" + s;

        return s;
    };

    int flag_bits = (first_halfword >> 11) & 0x1F;
    
    switch(flag_bits) {
        case 0b11101:
        case 0b11110:
        case 0b11111:
            // 32-bit THUMB, need next halfword as well
            {
                if(should_print)
                    std::cout 
                        << "[0x" << std::hex << pad_hex_number(PC) 
                        << std::dec << " 32-bit] " << std::flush;

                uint32_t second_halfword = memory.load_u16(PC+2);

                fi.in   = (first_halfword << 16) | second_halfword;
                fi.type = fetched_instruction_t::t32;
                return fi;
            }
        default:
            // 16-bit THUMB, first halfword is entire instruction
            if(should_print)
                std::cout 
                    << "[0x" << std::hex << pad_hex_number(PC) 
                    << std::dec << " 16-bit] " << std::flush;

            fi.in   = first_halfword;
            fi.type = fetched_instruction_t::t16;
            return fi;
    }

}
