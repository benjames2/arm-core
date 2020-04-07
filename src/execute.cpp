#include <iostream>

#include <inc/decode.h>
#include <inc/decode_structure.h>
#include <inc/execute.h>
#include <inc/asm_math_interface.h>

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
    // first things first, switch by opcode

    auto new_cpu = cpu;

    switch(inst.opcode) {
        case i_ADC  :// add with carry
            
        case i_ADD  :// add
        case i_AND  :// bitwise AND
        case i_ASR  :// arithmetic shift right
        case i_B    :// unconditional branch
        case i_Bxx  :// conditional branch
        case i_BIC  :// bit clear
        case i_BL   :// branch and link
        case i_BX   :// branch and exchange
        case i_CMN  :// compare negative
        case i_CMP  :// compare
        case i_EOR  :// bitwise XOR
        case i_LDMIA:// load multiple
        case i_LDR  :// load word
        case i_LDRB :// load byte
        case i_LDRH :// load halfword
        case i_LSL  :// logical shift left
        case i_LDSB :// load sign-extended byte
        case i_LDSH :// load sign-extended halfword
        case i_LSR  :// logical shift right
        case i_MOV  :// move register
        case i_MUL  :// multiply
        case i_MVN  :// move negative register
        case i_NEG  :// negate
        case i_ORR  :// bitwise OR
        case i_POP  :// pop registers
        case i_PUSH :// push registers
        case i_ROR  :// rotate right
        case i_SBC  :// subtract with carry
        case i_STMIA:// store multiple
        case i_STR  :// store word
        case i_STRB :// store byte
        case i_STRH :// store halfword
        case i_SWI  :// software interrupt
        case i_SUB  :// subtract
        case i_TST  :// test bits
        default:
            throw std::runtime_error("execute_t16 : invalid opcode");
    }

}

armv7_m3 execute_t32(armv7_m3& cpu, memory_t& memory, instruction_32b_t& inst) {
    return cpu;
}
