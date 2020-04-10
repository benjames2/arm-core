#include <iostream>

#include <inc/decode.h>
#include <inc/decode_structure.h>
#include <inc/execute.h>
#include <inc/asm_math_interface.h>

// comment to avoid lots of information
//#define EXECUTE_DEBUG

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
            {
                auto Rd      = new_cpu.get_register(inst.Rd).i32;
                auto Rs      = new_cpu.get_register(inst.Rs).i32;
                int CarryBit = new_cpu.get_CPSR_C();

                results_t results;
                
                // this routine is written in assembly. this is because assembly does 
                // not have undefined behavior when it comes to overflows regardless of 
                // the signed-ness of two operands
                auto msg = gp_operation(&results, Rd, Rs, CarryBit, x86_asm_ADC);

                new_cpu.set_register_i32(inst.Rd, results.i32);
                new_cpu.set_CPSR_C(results.get_x86_flag_Carry());

                new_cpu.cycle_count++;
                if(Rd != 15)
                    new_cpu.PC() += 2;
                else
                    new_cpu.cycle_count++;

                return new_cpu;
            }
        case i_ADD  :// add
            if(inst.meta_opcode == meta_RR) {
                auto Rd      = new_cpu.get_register(inst.Rd).i32;
                auto Rs      = new_cpu.get_register(inst.Rs).i32;

                results_t results;
                
                // this routine is written in assembly. this is because assembly does 
                // not have undefined behavior when it comes to overflows regardless of 
                // the signed-ness of two operands
                auto msg = gp_operation(&results, Rd, Rs, 0, x86_asm_ADD);

                new_cpu.set_register_i32(inst.Rd, results.i32);
                new_cpu.set_CPSR_C(results.get_x86_flag_Carry());

                new_cpu.cycle_count++;
                if(inst.Rd != 15)
                    new_cpu.PC() += 2;
                else
                    new_cpu.cycle_count++;

                return new_cpu;
            }
            else {
                // i_ADD has a lot of forms:
                /*
                case meta_RRR:   (2)
                case meta_RRC:   (2)
                case meta_RC:    (3)
                case meta_RC_pc: (12)
                case meta_RC_sp: (12)
                case meta_C_sp:  (13)
                */
                throw std::runtime_error("execute_t16(i_ADD) : meta opcode not implemented");
            }
        case i_AND  :// bitwise AND
            {
                auto Rd = new_cpu.get_register(inst.Rd).u32;
                auto Rs = new_cpu.get_register(inst.Rs).u32;

                auto res = Rd & Rs;

                // update these two flags with results
                new_cpu.set_CPSR_Z(res == 0);
                new_cpu.set_CPSR_N(res & (1 << 31));
                new_cpu.set_register_u32(inst.Rd, res);

                new_cpu.cycle_count++;
                if(inst.Rd != 15)
                    new_cpu.PC() += 2;
                else
                    new_cpu.cycle_count++;

                return new_cpu;
            }
        case i_ASR  :// arithmetic shift right
            throw std::runtime_error("execute_t16 : opcode not implemented");
        case i_B    :// unconditional branch
            {

                uint32_t PC = new_cpu.PC();
                PC += 4; // prefetch operation

                results_t results;
                auto msg = gp_operation(&results, PC, inst.i32, 0, x86_asm_ADD);
                new_cpu.PC() = results.u32;

                #ifdef EXECUTE_DEBUG
                std::cout 
                    << "execute_t16(i_B) : target -> " << std::hex 
                    << new_cpu.PC() << std::dec << Std::endl;
                #endif // EXECUTE_DEBUG

                return new_cpu;
            }
        case i_Bxx  :// conditional branch
        case i_BIC  :// bit clear
        case i_BL   :// branch and link
        case i_BX   :// branch and exchange
        case i_CMN  :// compare negative
        case i_CMP  :// compare
            throw std::runtime_error("execute_t16 : opcode not implemented");
        case i_EOR  :// bitwise XOR
            {
                auto Rd = new_cpu.get_register(inst.Rd).u32;
                auto Rs = new_cpu.get_register(inst.Rs).u32;

                auto res = Rd ^ Rs;

                // update these two flags with results
                new_cpu.set_CPSR_Z(res == 0);
                new_cpu.set_CPSR_N(res & (1 << 31));
                new_cpu.set_register_u32(inst.Rd, res);

                new_cpu.cycle_count++;
                if(inst.Rd != 15)
                    new_cpu.PC() += 2;
                else
                    new_cpu.cycle_count++;

                return new_cpu;
            }
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
            throw std::runtime_error("execute_t16 : opcode not implemented");
        case i_ORR  :// bitwise OR
            {
                auto Rd = new_cpu.get_register(inst.Rd).u32;
                auto Rs = new_cpu.get_register(inst.Rs).u32;

                auto res = Rd | Rs;

                // update these two flags with results
                new_cpu.set_CPSR_Z(res == 0);
                new_cpu.set_CPSR_N(res & (1 << 31));
                new_cpu.set_register_u32(inst.Rd, res);

                new_cpu.cycle_count++;
                if(inst.Rd != 15)
                    new_cpu.PC() += 2;
                else
                    new_cpu.cycle_count++;

                return new_cpu;
            }
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
            throw std::runtime_error("execute_t16 : opcode not implemented");
        default:
            throw std::runtime_error("execute_t16 : invalid opcode");
    }

}

armv7_m3 execute_t32(armv7_m3& cpu, memory_t& memory, instruction_32b_t& inst) {
    throw std::runtime_error("execute_t32 : not implemented");
    //return cpu;
}
