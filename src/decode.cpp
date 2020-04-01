#include <iostream>
#include <stdexcept>

#include <inc/fetch.h>
#include <inc/decode.h>
#include <inc/decode_16.h>
#include <inc/decode_32.h>

/*

    fetch first 2 bytes

    if 16-bit instruction
        - call 16 bit decode
    else
        fetch the two bytes after that
        - call 32 bit decode

    return decoded_instruction_t

*/

auto decode(fetched_instruction_t instruction, uint32_t PC) -> decoded_instruction_t {

    decoded_instruction_t fin;

    switch(instruction.type) {
        case fetched_instruction_t::t16:
            {
                fin.di_t16 = decode_16bit_instruction(PC, instruction.in);
                fin.type   = decoded_instruction_t::t16;
                return fin;
            }
        case fetched_instruction_t::t32:
            {
                fin.di_t32 = decode_32bit_instruction(PC, instruction.in);
                fin.type   = decoded_instruction_t::t32;
                return fin;
            }
        default:
            throw std::runtime_error("decode : invalid instruction type (must be either t16 or t32)");
    }
}
