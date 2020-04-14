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
        case i_ADC  :// **DONE** add with carry
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
        case i_ADD  :// **DONE** add
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
            else if(inst.meta_opcode == meta_RC_pc) { // (12) Rd = PC + i32
                
                uint32_t pc = new_cpu.PC();
                pc += 4; // prefetch operator

                auto imm = inst.i32;

                results_t results;
                auto msg = gp_operation(&results, pc, imm, 0, x86_asm_ADD);
                new_cpu.set_register_i32(inst.Rd, results.i32);

                // CPSR is unaffected by this instruction

                new_cpu.cycle_count++;
                if(inst.Rd != 15) new_cpu.PC() += 2;
                else              new_cpu.cycle_count++;

                return new_cpu;
            }
            else if(inst.meta_opcode == meta_RC_sp) { // (12) Rd = PC + i32

                uint32_t sp = new_cpu.SP();
                auto imm = inst.i32;

                results_t results;
                auto msg = gp_operation(&results, sp, imm, 0, x86_asm_ADD);
                new_cpu.set_register_i32(inst.Rd, results.i32);

                // CPSR is unaffected by this instruction

                new_cpu.cycle_count++;
                if(inst.Rd != 15) new_cpu.PC() += 2;
                else              new_cpu.cycle_count++;

                return new_cpu;
            }
            else if(inst.meta_opcode == meta_C_sp) { // (13) SP += +/-imm

                uint32_t sp = new_cpu.SP();
                auto imm = inst.i32;
                
                results_t results;
                auto msg = gp_operation(&results, sp, imm, 0, x86_asm_ADD);
                new_cpu.SP() = results.u32;

                // CPSR codes are unaffected by this instruction

                new_cpu.cycle_count++;
                new_cpu.PC() += 2;

                return new_cpu;
            }
            else {
                throw std::runtime_error("execute_t16(i_ADD) : meta opcode not implemented");
            }
        case i_AND  :// **DONE** bitwise AND
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
        case i_B    :// **DONE** unconditional branch
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
            throw std::runtime_error("execute_t16 : opcode not implemented");
        case i_BIC  :// bit clear
            {
                auto Rd = new_cpu.get_register(inst.Rd).i32;
                auto Rs = new_cpu.get_register(inst.Rs).i32;

                //Operation
                auto result = Rd & (~Rs);

                new_cpu.set_register_i32(inst.Rd, result);

                //Set Flags
                new_cpu.set_CPSR_N(result & (1 << 31));
                new_cpu.set_CPSR_Z(result == 0);
                new_cpu.set_CPSR_C(false);
                
                //Set PC and cycle count
                new_cpu.cycle_count++;
                if(inst.Rd != 15) new_cpu.PC() += 2;
                else              new_cpu.cycle_count++;

                return new_cpu;  

            }
        case i_BL   :// branch and link
        case i_BX   :// branch and exchange
            throw std::runtime_error("execute_t16 : opcode not implemented");
        case i_CMN  :// compare negative
            {
                results_t result;

                auto Rd = new_cpu.get_register(inst.Rd).i32;
                auto Rs = new_cpu.get_register(inst.Rs).i32;

                //Operation
                auto msg = gp_operation(&result, Rd, Rs, 0, x86_asm_ADD);

                //Set Flags
                new_cpu.set_CPSR_N(result.get_x86_flag_Sign());
                new_cpu.set_CPSR_Z(result.get_x86_flag_Zero());
                new_cpu.set_CPSR_C(result.get_x86_flag_Carry());
                new_cpu.set_APSR_V(result.get_x86_flag_Ov());

                //Set PC and cycle count
                new_cpu.cycle_count++;
                if(inst.Rd != 15) new_cpu.PC() += 2;
                else              new_cpu.cycle_count++;

                return new_cpu;
            }
        case i_CMP  :// compare
            {
                if(inst.meta_opcode == meta_RC){

                    results_t result;

                    auto Rd     = new_cpu.get_register(inst.Rd).i32;
                    auto offset = inst.i_immediate;
                    
                    //Operation
                    auto msg = gp_operation(&result, Rd, offset, 0, x86_asm_SBB); 

                    //Set Flags
                    new_cpu.set_CPSR_N(result.get_x86_flag_Sign());
                    new_cpu.set_CPSR_Z(result.get_x86_flag_Zero());
                    new_cpu.set_CPSR_C(result.get_x86_flag_Carry());
                    new_cpu.set_APSR_V(result.get_x86_flag_Ov());

                    //Set PC and cycle count
                    new_cpu.cycle_count++;
                    if(inst.Rd != 15) new_cpu.PC() += 2;
                    else              new_cpu.cycle_count++;

                    return new_cpu;

                }
                else if(inst.meta_opcode == meta_RR){

                    results_t result;

                    auto Rd = new_cpu.get_register(inst.Rd).i32;
                    auto Rs = new_cpu.get_register(inst.Rs).i32;

                    //Operation
                    auto msg = gp_operation(&result, Rd, Rs, 0, x86_asm_SBB);

                    //Set Flags
                    new_cpu.set_CPSR_N(result.get_x86_flag_Sign());
                    new_cpu.set_CPSR_Z(result.get_x86_flag_Zero());
                    new_cpu.set_CPSR_C(result.get_x86_flag_Carry());
                    new_cpu.set_APSR_V(result.get_x86_flag_Ov());

                    //Set PC and cycle count
                    new_cpu.cycle_count++;
                    if(inst.Rd != 15) new_cpu.PC() += 2;
                    else              new_cpu.cycle_count++;

                    return new_cpu;
                }
                else 
                    throw std::runtime_error("execute_t16(i_MOV) : Invalid meta opcode ");
            }
        case i_EOR  :// **DONE** bitwise XOR
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
            throw std::runtime_error("execute_t16 : opcode not implemented");
        case i_LDR  :// **IN PROGRESS** load word
            if(inst.meta_opcode == meta_RC_pc) { // (6) Rd = word mem[PC + uimm]

                uint32_t pc = new_cpu.PC();
                pc += 4; // prefetch operation
                uint32_t uimm = inst.u32;

                auto addr = pc + uimm;
                auto bytes = memory.load_u32(addr);
                new_cpu.set_register_u32(inst.Rd, bytes);

                // maintain cycle count and advance IP as needed
                new_cpu.cycle_count++;
                if(inst.Rd != 15) new_cpu.PC() += 2;
                else              new_cpu.cycle_count++;

                return new_cpu;
            }
            else if(inst.meta_opcode == meta_RRR) { // (7) Rd = word mem[Rb + Ro]

                auto Rb = new_cpu.get_register(inst.Rb).u32; // base
                auto Ro = new_cpu.get_register(inst.Ro).u32; // offset
                
                auto addr = Rb + Ro;
                auto bytes = memory.load_u32(addr);
                new_cpu.set_register_u32(inst.Rd, bytes);

                // maintain cycle count and advance IP as needed
                new_cpu.cycle_count++;
                if(inst.Rd != 15) new_cpu.PC() += 2;
                else              new_cpu.cycle_count++;

                return new_cpu;
            }
            else if(inst.meta_opcode == meta_RRC) { // (9) Rd = word mem[Rb + uimm]

                auto Rb = new_cpu.get_register(inst.Rb).u32;
                auto uimm = inst.u32;

                auto addr = Rb + uimm;
                auto bytes = memory.load_u32(addr);
                new_cpu.set_register_u32(inst.Rd, bytes);

                new_cpu.cycle_count++;
                if(inst.Rd != 15) new_cpu.PC() += 2;
                else              new_cpu.cycle_count++;

                return new_cpu;
            }
            //else if(inst.meta_opcode == meta_RC_sp) { // (11) Rd = 

                

            //}
            else {
                // ##6(RC_pc), may need to do some PC bit adjustment on this one 
                // ##7(RRR), 
                // ##9(RRC),
                // 11(RC_sp) 
                throw std::runtime_error("execute_t16(i_LDR) : meta opcode not implemented");
            }
        case i_LDRB :// load byte
        case i_LDRH :// load halfword
        case i_LSL  :// logical shift left
        case i_LDSB :// load sign-extended byte
        case i_LDSH :// load sign-extended halfword
        case i_LSR  :// logical shift right
        case i_MOV  :// move register
            {
                if(inst.meta_opcode == meta_RC){

                    auto imm = inst.i_immediate;
                    
                    new_cpu.set_register_i32(inst.Rd, imm);

                    //Set Flags
                    new_cpu.set_CPSR_N(imm & (1 << 31));
                    new_cpu.set_CPSR_Z(imm == 0);
                    new_cpu.set_CPSR_C(false);

                    //Set PC and cycle count
                    new_cpu.cycle_count++;
                    if(inst.Rd != 15) new_cpu.PC() += 2;
                    else              new_cpu.cycle_count++;

                    return new_cpu;

                }
                else if(inst.meta_opcode == meta_RR){

                    auto Hs = new_cpu.get_register(inst.Rs).i32;
                    
                    new_cpu.set_register_i32(inst.Rd, Hs);

                    //Set Flags
                    new_cpu.set_CPSR_N(Hs & (1 << 31));
                    new_cpu.set_CPSR_Z(Hs == 0);
                    new_cpu.set_CPSR_C(false);

                    //Set PC and cycle count
                    new_cpu.cycle_count++;
                    if(inst.Rd != 15) new_cpu.PC() += 2;
                    else              new_cpu.cycle_count++;

                    return new_cpu;
                }
                else 
                    throw std::runtime_error("execute_t16(i_MOV) : Invalid meta opcode ");
            }
        case i_MUL  :// multiply
        case i_MVN  :// move negative register
        case i_NEG  :// negate
            throw std::runtime_error("execute_t16 : opcode not implemented");
        case i_ORR  :// **DONE** bitwise OR
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
            {
                if(inst.meta_opcode == meta_t32_imm){
                    
                    auto Rn       = new_cpu.get_register(inst.Rn).i32;
                    auto imm      = inst.i32;
                    int carryBit = new_cpu.get_CPSR_C(); 

                    results_t result;

                    auto msg = gp_operation(&result, Rn, imm, carryBit, x86_asm_ADC);

                    new_cpu.set_register_i32(inst.Rd, result.i32);

                    //Set Flags
                    if(inst.S){
                        new_cpu.set_CPSR_N(result.get_x86_flag_Sign());
                        new_cpu.set_CPSR_Z(result.get_x86_flag_Zero());
                        new_cpu.set_CPSR_C(result.get_x86_flag_Carry());
                        new_cpu.set_CPSR_V(result.get_x86_flag_Ov());
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
                    throw std::runtime_error("execute_t32 : invalid meta_opcode for ADC instruction");
            }   
        case t32_ADD:
            {
                if(inst.meta_opcode == meta_t32_imm){
                    switch(inst.encoding){
                        case 3:
                            {
                                results_t result;

                                auto Rn  = new_cpu.get_register(inst.Rn).i32;
                                auto imm = inst.i32;

                                auto msg = gp_operation(&result, Rn, imm, 0, x86_asm_ADD);

                                new_cpu.set_register_i32(inst.Rd, result.i32);

                                //Set Flags
                                if(inst.S){
                                    new_cpu.set_CPSR_N(result.get_x86_flag_Sign());
                                    new_cpu.set_CPSR_Z(result.get_x86_flag_Zero());
                                    new_cpu.set_CPSR_C(result.get_x86_flag_Carry());
                                    new_cpu.set_CPSR_V(result.get_x86_flag_Ov());
                                }

                                //Set PC and cycle counts
                                new_cpu.cycle_count++;
                                if(inst.Rd != 15)
                                    new_cpu.PC() += 4;
                                else
                                    new_cpu.cycle_count++;

                                return new_cpu;
                            }
                        default:
                            throw std::runtime_error("execute_t32 : invalid encoding for ADD instruction");
                    }
                }
                else
                    throw std::runtime_error("execute_t32 : invalid meta_opcode for ADD instruction");
            }   
        case t32_ADR:
            throw std::runtime_error("execute_t32 : opcode not implemented");   
        case t32_AND:
            {
                if(inst.meta_opcode == meta_t32_imm){

                    auto Rn  = new_cpu.get_register(inst.Rn).i32;
                    auto imm = inst.i32;
                    auto result = Rn & imm;

                    new_cpu.set_register_i32(inst.Rd, result);
                    
                    //Set Flags
                    if(inst.S){
                        new_cpu.set_CPSR_N(result & (1 << 31));
                        new_cpu.set_CPSR_Z(result == 0);
                        new_cpu.set_CPSR_C(false);
                    }
                    
                    //Set PC and cycle count
                    new_cpu.cycle_count++;
                    if(inst.Rd != 15) 
                        new_cpu.PC() += 4;
                    else        
                        new_cpu.cycle_count++;

                    return new_cpu;
                }
                else
                    throw std::runtime_error("execute_t32 : invalid meta_opcode for AND instruction");
            }   
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
            throw std::runtime_error("execute_t32 : opcode not implemented");   
        case t32_CMN:
            {
                if(inst.meta_opcode == meta_t32_imm){

                    auto Rn  = new_cpu.get_register(inst.Rn).i32;
                    auto imm = inst.i32;

                    results_t result;

                    auto msg = gp_operation(&result, Rn, imm, 0, x86_asm_ADD);
                    
                    //Set Flags
                    new_cpu.set_CPSR_N(result.get_x86_flag_Sign());
                    new_cpu.set_CPSR_Z(result.get_x86_flag_Zero());
                    new_cpu.set_CPSR_C(false); 
                    new_cpu.set_CPSR_V(result.get_x86_flag_Ov());
                    
                    
                    //Set PC and cycle count
                    new_cpu.cycle_count++;
                    if(inst.Rd != 15) 
                        new_cpu.PC() += 4;
                    else        
                        new_cpu.cycle_count++;

                    return new_cpu;
                }
                else
                    throw std::runtime_error("execute_t32 : invalid meta_opcode for CMN instruction");
            }   
        case t32_CMP:
            {
                if(inst.meta_opcode == meta_t32_imm){

                    auto Rn  = new_cpu.get_register(inst.Rn).i32;
                    auto imm = inst.i32;

                    results_t result;

                    auto msg = gp_operation(&result, Rn, imm, 0, x86_asm_SUB);
                    
                    //Set Flags
                    new_cpu.set_CPSR_N(result.get_x86_flag_Sign());
                    new_cpu.set_CPSR_Z(result.get_x86_flag_Zero());
                    new_cpu.set_CPSR_C(result.get_x86_flag_Carry()); 
                    new_cpu.set_CPSR_V(result.get_x86_flag_Ov());
                    
                    
                    //Set PC and cycle count
                    new_cpu.cycle_count++;
                    if(inst.Rd != 15) 
                        new_cpu.PC() += 4;
                    else        
                        new_cpu.cycle_count++;

                    return new_cpu;
                }
                else
                    throw std::runtime_error("execute_t32 : invalid meta_opcode for CMP instruction");
            }    
        case t32_CPS:   
        case t32_CPY:   
        case t32_DBG:   
        case t32_DMB:   
        case t32_DSB:
            throw std::runtime_error("execute_t32 : opcode not implemented");   
        case t32_EOR:
            {
                if(inst.meta_opcode == meta_t32_imm){

                    auto Rn  = new_cpu.get_register(inst.Rn).i32;
                    auto imm = inst.i32;
                    auto result = Rn ^ imm;

                    new_cpu.set_register_i32(inst.Rd, result);
                    
                    //Set Flags
                    if(inst.S){
                        new_cpu.set_CPSR_N(result & (1 << 31));
                        new_cpu.set_CPSR_Z(result == 0);
                        new_cpu.set_CPSR_C(false);
                    }
                    
                    //Set PC and cycle count
                    new_cpu.cycle_count++;
                    if(inst.Rd != 15) 
                        new_cpu.PC() += 4;
                    else        
                        new_cpu.cycle_count++;

                    return new_cpu;
                }
                else
                    throw std::runtime_error("execute_t32 : invalid meta_opcode for EOR instruction");
            }   
        case t32_ISB:   
        case t32_IT :   
        case t32_LDC:   
        case t32_LDC2:  
        case t32_LDM:   
        case t32_LDMIA: 
        case t32_LDMFD: 
        case t32_LDMDB: 
        case t32_LDMEA:
            throw std::runtime_error("execute_t32 : opcode not implemented"); 
        case t32_LDR:
            {
                if(inst.meta_opcode == meta_t32_imm){
                    switch(inst.encoding){
                        case instruction_32b_t::encoding_T3:
                            {
                                auto imm = inst.i32;
                                auto pc  = new_cpu.PC();
                                pc += 4;

                                auto addr  = pc + imm;
                                auto bytes = memory.load_u32(addr);

                                new_cpu.set_register_i32(inst.Rd, bytes);

                                //Set PC and count cycle
                                new_cpu.cycle_count++;
                                if(inst.Rd != 15)
                                    new_cpu.PC() += 4;
                                else
                                    new_cpu.cycle_count++;

                                return new_cpu;
                            }
                        default:
                            throw std::runtime_error("execute_t32 : invalid encoding for LDR instruction");
                    }
                }
                else
                    throw std::runtime_error("execute_t32 : invalid meta_opcode for LDR instruction");
            } 
        case t32_LDRB: 
        case t32_LDRBT: 
        case t32_LDRD: 
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
            throw std::runtime_error("execute_t32 : opcode not implemented");
        case t32_MOV   :
            {
                if(inst.meta_opcode == meta_t32_imm){
                    switch(inst.encoding){
                        case instruction_32b_t::encoding_T2:
                            {
                                auto result = inst.i32;

                                new_cpu.set_register_i32(inst.Rd, result);

                                //Set Flags
                                if(inst.S){
                                    new_cpu.set_CPSR_N(result & (1 << 31));
                                    new_cpu.set_APSR_Z(result == 0);
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
                        default:
                            throw std::runtime_error("execute_t32 : invalid encoding for MOV instruction");
                    }
                }
                else
                    throw std::runtime_error("execute_t32 : invalid meta_opcode for BIC instruction");
            }
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
