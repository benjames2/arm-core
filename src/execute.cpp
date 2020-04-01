#include <iostream>

#include <inc/decode.h>
#include <inc/decode_structure.h>
#include <inc/execute.h>

static armv7_m3 execute_t16(armv7_m3& cpu, memory_t& memory, instruction_16b_t& inst);
static armv7_m3 execute_t32(armv7_m3& cpu, memory_t& memory, instruction_32b_t& inst);

armv7_m3 execute(armv7_m3& cpu, memory_t& memory, decoded_instruction_t& inst) {

    switch(inst.type) {
        case decoded_instruction_t::t16:
            return ::execute_t16(cpu, memory, inst.di_t16);

        case decoded_instruction_t::t32:
            return ::execute_t32(cpu, memory, inst.di_t32);
        
        default:
            throw std::runtime_error("execute : invalid instruction type");
    }

}

armv7_m3 execute_t16(armv7_m3& cpu, memory_t& memory, instruction_16b_t& inst) {
    return cpu;
}

armv7_m3 execute_t32(armv7_m3& cpu, memory_t& memory, instruction_32b_t& inst) {
    return cpu;
}
