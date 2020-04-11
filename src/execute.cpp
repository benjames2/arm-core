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

                // maintain cycle count and advance IP as needed
                new_cpu.cycle_count++;
                if(inst.Rd != 15) new_cpu.PC() += 2;
                else              new_cpu.cycle_count++;

                return new_cpu;
            }
        case i_ADD  :// add (updates CPSR.[Negative,Zero,Carry,oVerflow])
            if(inst.meta_opcode == meta_RR) {
                auto Rd      = new_cpu.get_register(inst.Rd).i32;
                auto Rs      = new_cpu.get_register(inst.Rs).i32;

                results_t results;
                
                // this routine is written in assembly. this is because assembly does 
                // not have undefined behavior when it comes to overflows regardless of 
                // the signed-ness of two operands
                auto msg = gp_operation(&results, Rd, Rs, 0, x86_asm_ADD);

                new_cpu.set_register_i32(inst.Rd, results.i32);

                // set flags
                new_cpu.set_CPSR_N(results.get_x86_flag_Sign());
                new_cpu.set_CPSR_Z(results.get_x86_flag_Zero());
                new_cpu.set_CPSR_C(results.get_x86_flag_Carry());
                new_cpu.set_CPSR_V(results.get_x86_flag_Ov());

                new_cpu.cycle_count++;
                if(inst.Rd != 15) new_cpu.PC() += 2;
                else              new_cpu.cycle_count++;

                return new_cpu;
            }
            else if(inst.meta_opcode == meta_RRR) { // (2) Rd = Rs + Rn

                auto Rs = new_cpu.get_register(inst.Rs).i32;
                auto Rn = new_cpu.get_register(inst.Rn).i32;

                results_t results;
                auto msg = gp_operation(&results, Rs, Rn, 0, x86_asm_ADD);

                new_cpu.set_register_i32(inst.Rd, results.i32);
                
                // set flags
                new_cpu.set_CPSR_N(results.get_x86_flag_Sign());
                new_cpu.set_CPSR_Z(results.get_x86_flag_Zero());
                new_cpu.set_CPSR_C(results.get_x86_flag_Carry());
                new_cpu.set_CPSR_V(results.get_x86_flag_Ov());
                
                new_cpu.cycle_count++;
                if(inst.Rd != 15) new_cpu.PC() += 2;
                else              new_cpu.cycle_count++;

                return new_cpu;
            }
            else if(inst.meta_opcode == meta_RRC) { // (2) Rd = Rs + u32

                auto Rs  = new_cpu.get_register(inst.Rs).i32;
                auto u32 = inst.i32;

                results_t results;
                auto msg = gp_operation(&results, Rs, u32, 0, x86_asm_ADD);

                new_cpu.set_register_i32(inst.Rd, results.i32);

                // set flags
                new_cpu.set_CPSR_N(results.get_x86_flag_Sign());
                new_cpu.set_CPSR_Z(results.get_x86_flag_Zero());
                new_cpu.set_CPSR_C(results.get_x86_flag_Carry());
                new_cpu.set_CPSR_V(results.get_x86_flag_Ov());

                new_cpu.cycle_count++;
                if(inst.Rd != 15) new_cpu.PC() += 2;
                else              new_cpu.cycle_count++;

                return new_cpu;
            }
            else if(inst.meta_opcode == meta_RC) { // (3) Rd = Rd + u32
                
                auto Rd  = new_cpu.get_register(inst.Rd).i32;
                auto u32 = inst.i32;

                results_t results;
                auto msg = gp_operation(&results, Rd, u32, 0, x86_asm_ADD);

                new_cpu.set_register_i32(inst.Rd, results.i32);

                // set flags
                new_cpu.set_CPSR_N(results.get_x86_flag_Sign());
                new_cpu.set_CPSR_Z(results.get_x86_flag_Zero());
                new_cpu.set_CPSR_C(results.get_x86_flag_Carry());
                new_cpu.set_CPSR_V(results.get_x86_flag_Ov());

                new_cpu.cycle_count++;
                if(inst.Rd != 15) new_cpu.PC() += 2;
                else              new_cpu.cycle_count++;

                return new_cpu;
            }
            else if(inst.meta_opcode == meta_RC_pc) {

            }
            else {
                // i_ADD has a lot of forms:
                /*
                ##case meta_RRR:   (2)
                ##case meta_RRC:   (2)
                ##case meta_RC:    (3)
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
    
    auto new_cpu = cpu;

    switch (inst.opcode){

        case t32_ADC:   
        case t32_ADD:   
        case t32_ADR:   
        case t32_AND:   
        case t32_ASR:   
        case t32_B:     
        case t32_BFC:   
        case t32_BFI:
            throw std::runtime_error("execute_t32 : opcode not implemented");   
        case t32_BIC:
            {
                if(inst.meta_opcode == meta_t32_imm){
                    
                    auto Rn     = new_cpu.get_register(inst.Rn).i32;
                    auto imm    = inst.i32;
                    auto result = Rn & (~imm);

                    new_cpu.set_register_i32(inst.Rd, result);

                    //Set Flags
                    if(inst.S){
                        new_cpu.set_CPSR_N(result & (1 << 31));
                        new_cpu.set_CPSR_Z(result == 0);
                        new_cpu.set_CPSR_C(false);
                    }
                    
                    //not sure about this
                    new_cpu.cycle_count++;
                    if(inst.Rd != 15) 
                        new_cpu.PC() += 4;
                    else        
                        new_cpu.cycle_count++;

                    return new_cpu;
                }

                else
                    throw std::runtime_error("execute_t32 : invalid meta_opcode for BIC instruction"); 
            }
        case t32_BKPT:  
        case t32_BL:    
        case t32_BLX:   
        case t32_BX:    
        case t32_CBNZ:  
        case t32_CBZ:   
        case t32_CDP:   
        case t32_CDP2:  
        case t32_CLREX: 
        case t32_CLZ:   
        case t32_CMN:   
        case t32_CMP:   
        case t32_CPS:   
        case t32_CPY:   
        case t32_DBG:   
        case t32_DMB:   
        case t32_DSB:   
        case t32_EOR:   
        case t32_ISB:   
        case t32_IT :   
        case t32_LDC:   
        case t32_LDC2:  
        case t32_LDM:   
        case t32_LDMIA: 
        case t32_LDMFD: 
        case t32_LDMDB: 
        case t32_LDMEA: 
        case t32_LDR  : 
        case t32_LDRB : 
        case t32_LDRBT: 
        case t32_LDRD : 
        case t32_LDREX: 
        case t32_LDREXB:
        case t32_LDREXH:
        case t32_LDRH  :
        case t32_LDRHT :
        case t32_LDRSB :
        case t32_LDRSBT:
        case t32_LDRSH :
        case t32_LDRSHT:
        case t32_LDRT  :
        case t32_LSL   :
        case t32_LSR   :
        case t32_MCR   :
        case t32_MCR2  :
        case t32_MCRR  :
        case t32_MCRR2 :
        case t32_MLA   :
        case t32_MLS   :
        case t32_MOV   :
        case t32_MOVT  :
        case t32_MRC   :
        case t32_MRC2  :
        case t32_MRRC  :
        case t32_MRRC2 :
        case t32_MRS   :
        case t32_MSR   :
        case t32_MUL   :
        case t32_MVN   :
        case t32_NEG   :
        case t32_NOP   :
        case t32_ORN   :
            throw std::runtime_error("execute_t32 : opcode not implemented");
        case t32_ORR:
            {
                if(inst.meta_opcode == meta_t32_imm){

                    auto Rn  = new_cpu.get_register(inst.Rn).i32;
                    auto imm = inst.i32;
                    auto result = Rn | imm;

                    new_cpu.set_register_i32(inst.Rd, result);
                    
                    //Set Flags
                    if(inst.S){
                        new_cpu.set_CPSR_N(result & (1 << 31));
                        new_cpu.set_CPSR_Z(result == 0);
                        new_cpu.set_CPSR_C(false);
                    }
                    
                    //not sure about this
                    new_cpu.cycle_count++;
                    if(inst.Rd != 15) 
                        new_cpu.PC() += 4;
                    else        
                        new_cpu.cycle_count++;

                    return new_cpu;
                }

                else
                    throw std::runtime_error("execute_t32 : invalid meta_opcode for ORR instruction");                
            }
        case t32_PLD   :
        case t32_PLI   :
        case t32_POP   :
        case t32_PUSH  :
        case t32_RBIT  :
        case t32_REV   :
        case t32_REV16 :
        case t32_REVSH :
        case t32_ROR   :
        case t32_RRX   :
        case t32_RSB   :
        case t32_SBC   :
        case t32_SBFX  :
        case t32_SDIV  :
        case t32_SEV   :
        case t32_SMLAL :
        case t32_SMULL :
        case t32_SSAT  :
        case t32_STC   :
        case t32_STC2  :
        case t32_STM   :
        case t32_STMIA :
        case t32_STMEA :
        case t32_STMDB :
        case t32_STMFD :
        case t32_STR   :
        case t32_STRB  :
        case t32_STRBT :
        case t32_STRD  :
        case t32_STREX :
        case t32_STREXB:
        case t32_STREXH:
        case t32_STRH  :
        case t32_STRHT :
        case t32_STRT  :
        case t32_SUB   :
        case t32_SVC   :
        case t32_SXTB  :
        case t32_SXTH  :
        case t32_TBB   :
        case t32_TBH   :
        case t32_TEQ   :
        case t32_TST   :
        case t32_UBFX  :
        case t32_UDIV  :
        case t32_UMLAL :
        case t32_UMULL :
        case t32_USAT  :
        case t32_UXTB  :
        case t32_UXTH  :
        case t32_WFE   :
        case t32_WFI   :
        case t32_YIELD :
            throw std::runtime_error("execute_t32 : opcode not implemented");
        default:
            throw std::runtime_error("execute_t32 : invalid opcode");  
    }
}
