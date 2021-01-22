#include <iostream>

#include <inc/decode.h>
#include <inc/decode_structure.h>
#include <inc/execute.h>
#include <inc/asm_math_interface.h>
#include <inc/range.h>
#include <inc/exceptions.h>
#include <inc/stack_operations.h>

// comment to avoid lots of information
//#define EXECUTE_DEBUG

static armstate_t execute_t16(armstate_t& armstate, instruction_16b_t& inst);
static armstate_t execute_t32(armstate_t& armstate, instruction_32b_t& inst);

armstate_t execute(armstate_t& armstate, decoded_instruction_t& inst) {

    armstate.cpu.set_stack_mode(armv7_m3::stack_mode_undefined);

    switch(inst.type) {
        case decoded_instruction_t::t16:
            return ::execute_t16(armstate, inst.di_t16);

        case decoded_instruction_t::t32:
            return ::execute_t32(armstate, inst.di_t32);
        
        default:
            throw std::runtime_error("execute : invalid instruction type");
    }

}

armstate_t execute_t16(armstate_t& armstate, instruction_16b_t& inst) {
    // first things first, switch by opcode
    auto new_armstate = armstate;

    new_armstate.cpu.cpu_id++;

    switch(inst.opcode) {
        case i_ADC  :// **DONE** add with carry
            {
                auto Rd      = new_armstate.cpu.get_register(inst.Rd).i32;
                auto Rs      = new_armstate.cpu.get_register(inst.Rs).i32;
                int CarryBit = new_armstate.cpu.get_CPSR_C();

                results_t results;
                
                // this routine is written in assembly. this is because assembly does 
                // not have undefined behavior when it comes to overflows regardless of 
                // the signed-ness of two operands
                auto msg = gp_operation(&results, Rd, Rs, CarryBit, x86_asm_ADC);

                new_armstate.cpu.set_register_i32(inst.Rd, results.i32);

                //set flas
                new_armstate.cpu.set_CPSR_N(results.get_x86_flag_Sign());
                new_armstate.cpu.set_CPSR_Z(results.get_x86_flag_Zero());
                new_armstate.cpu.set_CPSR_C(results.get_x86_flag_Carry());
                new_armstate.cpu.set_CPSR_V(results.get_x86_flag_Ov());

                // maintain cycle count and advance IP as needed
                new_armstate.cpu.cycle_count++;
                if(inst.Rd != 15) new_armstate.cpu.PC() += 2;
                else              new_armstate.cpu.cycle_count++;

                return new_armstate;
            }
        case i_ADD  :// **DONE** add
            if(inst.meta_opcode == meta_RR) {
                auto Rd      = new_armstate.cpu.get_register(inst.Rd).i32;
                auto Rs      = new_armstate.cpu.get_register(inst.Rs).i32;

                results_t results;
                
                // this routine is written in assembly. this is because assembly does 
                // not have undefined behavior when it comes to overflows regardless of 
                // the signed-ness of two operands
                auto msg = gp_operation(&results, Rd, Rs, 0, x86_asm_ADD);

                new_armstate.cpu.set_register_i32(inst.Rd, results.i32);

                // set flags
                new_armstate.cpu.set_CPSR_N(results.get_x86_flag_Sign());
                new_armstate.cpu.set_CPSR_Z(results.get_x86_flag_Zero());
                new_armstate.cpu.set_CPSR_C(results.get_x86_flag_Carry());
                new_armstate.cpu.set_CPSR_V(results.get_x86_flag_Ov());

                new_armstate.cpu.cycle_count++;
                if(inst.Rd != 15) new_armstate.cpu.PC() += 2;
                else              new_armstate.cpu.cycle_count++;

                return new_armstate;
            }
            else if(inst.meta_opcode == meta_RRR) { // (2) Rd = Rs + Rn

                auto Rs = new_armstate.cpu.get_register(inst.Rs).i32;
                auto Rn = new_armstate.cpu.get_register(inst.Rn).i32;

                results_t results;
                auto msg = gp_operation(&results, Rs, Rn, 0, x86_asm_ADD);

                new_armstate.cpu.set_register_i32(inst.Rd, results.i32);
                
                // set flags
                new_armstate.cpu.set_CPSR_N(results.get_x86_flag_Sign());
                new_armstate.cpu.set_CPSR_Z(results.get_x86_flag_Zero());
                new_armstate.cpu.set_CPSR_C(results.get_x86_flag_Carry());
                new_armstate.cpu.set_CPSR_V(results.get_x86_flag_Ov());
                
                new_armstate.cpu.cycle_count++;
                if(inst.Rd != 15) new_armstate.cpu.PC() += 2;
                else              new_armstate.cpu.cycle_count++;

                return new_armstate;
            }
            else if(inst.meta_opcode == meta_RRC) { // (2) Rd = Rs + u32

                auto Rs  = new_armstate.cpu.get_register(inst.Rs).i32;
                auto u32 = inst.i32;

                results_t results;
                auto msg = gp_operation(&results, Rs, u32, 0, x86_asm_ADD);

                new_armstate.cpu.set_register_i32(inst.Rd, results.i32);

                // set flags
                new_armstate.cpu.set_CPSR_N(results.get_x86_flag_Sign());
                new_armstate.cpu.set_CPSR_Z(results.get_x86_flag_Zero());
                new_armstate.cpu.set_CPSR_C(results.get_x86_flag_Carry());
                new_armstate.cpu.set_CPSR_V(results.get_x86_flag_Ov());

                new_armstate.cpu.cycle_count++;
                if(inst.Rd != 15) new_armstate.cpu.PC() += 2;
                else              new_armstate.cpu.cycle_count++;


                return new_armstate;
            }
            else if(inst.meta_opcode == meta_RC) { // (3) Rd = Rd + u32
                
                auto Rd  = new_armstate.cpu.get_register(inst.Rd).i32;
                auto u32 = inst.i32;

                results_t results;
                auto msg = gp_operation(&results, Rd, u32, 0, x86_asm_ADD);
                new_armstate.cpu.set_register_i32(inst.Rd, results.i32);

                // set flags
                new_armstate.cpu.set_CPSR_N(results.get_x86_flag_Sign());
                new_armstate.cpu.set_CPSR_Z(results.get_x86_flag_Zero());
                new_armstate.cpu.set_CPSR_C(results.get_x86_flag_Carry());
                new_armstate.cpu.set_CPSR_V(results.get_x86_flag_Ov());

                new_armstate.cpu.cycle_count++;
                if(inst.Rd != 15) new_armstate.cpu.PC() += 2;
                else              new_armstate.cpu.cycle_count++;

                
                return new_armstate;
            }
            else if(inst.meta_opcode == meta_RC_pc) { // (12) Rd = PC + i32
                
                uint32_t pc = new_armstate.cpu.PC();
                pc += 4; // prefetch operator

                auto imm = inst.i32;

                results_t results;
                auto msg = gp_operation(&results, pc, imm, 0, x86_asm_ADD);
                new_armstate.cpu.set_register_i32(inst.Rd, results.i32);

                // CPSR is unaffected by this instruction

                new_armstate.cpu.cycle_count++;
                if(inst.Rd != 15) new_armstate.cpu.PC() += 2;
                else              new_armstate.cpu.cycle_count++;

                return new_armstate;
            }
            else if(inst.meta_opcode == meta_RC_sp) { // (12) Rd = PC + i32

                uint32_t sp = new_armstate.cpu.SP();
                auto imm = inst.i32;

                results_t results;
                auto msg = gp_operation(&results, sp, imm, 0, x86_asm_ADD);
                new_armstate.cpu.set_register_i32(inst.Rd, results.i32);

                // CPSR is unaffected by this instruction

                new_armstate.cpu.cycle_count++;
                if(inst.Rd != 15) new_armstate.cpu.PC() += 2;
                else              new_armstate.cpu.cycle_count++;
                
                return new_armstate;
            }
            else if(inst.meta_opcode == meta_C_sp) { // (13) SP += +/-imm

                uint32_t sp = new_armstate.cpu.SP();
                auto imm = inst.i32;
                
                results_t results;
                auto msg = gp_operation(&results, sp, imm, 0, x86_asm_ADD);
                new_armstate.cpu.SP() = results.u32;

                // CPSR codes are unaffected by this instruction

                new_armstate.cpu.cycle_count++;
                new_armstate.cpu.PC() += 2;
                
                return new_armstate;
            }
            else {
                throw std::runtime_error("execute_t16(i_ADD) : meta opcode not implemented");
            }
        case i_AND  :// **DONE** bitwise AND
            {
                auto Rd = new_armstate.cpu.get_register(inst.Rd).u32;
                auto Rs = new_armstate.cpu.get_register(inst.Rs).u32;

                auto res = Rd & Rs;

                // update these two flags with results
                new_armstate.cpu.set_CPSR_Z(res == 0);
                new_armstate.cpu.set_CPSR_N(res & (1 << 31));

                new_armstate.cpu.set_register_u32(inst.Rd, res);

                new_armstate.cpu.cycle_count++;
                if(inst.Rd != 15)
                    new_armstate.cpu.PC() += 2;
                else
                    new_armstate.cpu.cycle_count++;

                return new_armstate;
            }
        case i_ASR  :// **DONE** arithmetic shift right
            {
                if(inst.meta_opcode == meta_RRC){

                    auto Rs     = new_armstate.cpu.get_register(inst.Rs).i32;
                    auto offset = inst.u32;

                    //operation
                    auto result = Rs >> offset;
                    new_armstate.cpu.set_register_u32(inst.Rd, result);

                    //Flags update
                    new_armstate.cpu.set_CPSR_N(result & (1 << 31));
                    new_armstate.cpu.set_CPSR_Z(result == 0);
                    new_armstate.cpu.set_CPSR_C(false);

                    //Cycle count and PC update
                    new_armstate.cpu.cycle_count++;
                    if(inst.Rd != 15)
                        new_armstate.cpu.PC() += 2;
                    else
                        new_armstate.cpu.cycle_count++;

                    return new_armstate;

                }
                else if(inst.meta_opcode == meta_RR){
                
                    auto Rd = new_armstate.cpu.get_register(inst.Rd).i32;
                    auto Rs = new_armstate.cpu.get_register(inst.Rs).i32;

                    //operation
                    auto result = Rd >> Rs;
                    new_armstate.cpu.set_register_u32(inst.Rd, result);

                    //Flags update
                    new_armstate.cpu.set_CPSR_N(result & (1 << 31));
                    new_armstate.cpu.set_CPSR_Z(result == 0);
                    new_armstate.cpu.set_CPSR_C(false);

                    //Cycle count and PC update
                    new_armstate.cpu.cycle_count++;
                    if(inst.Rd != 15)
                        new_armstate.cpu.PC() += 2;
                    else
                        new_armstate.cpu.cycle_count++;

                    return new_armstate;
                }
                else
                    throw std::runtime_error("execute_t16(i_ASR) : invalid meta opcode");
            }
        case i_B    :// **DONE** unconditional branch
            {

                uint32_t PC = new_armstate.cpu.PC();
                PC += 4; // prefetch operation

                results_t results;
                auto msg = gp_operation(&results, PC, inst.i32, 0, x86_asm_ADD);
                new_armstate.cpu.PC() = results.u32;

                #ifdef EXECUTE_DEBUG
                std::cout 
                    << "execute_t16(i_B) : target -> " << std::hex 
                    << new_armstate.cpu.PC() << std::dec << Std::endl;
                #endif // EXECUTE_DEBUG

                //Set cycle count
                new_armstate.cpu.cycle_count += 2;

                return new_armstate;
            }
        case i_Bxx  :// **DONE** conditional branch
            {
                switch(inst.condition_code){
                    case 0: //BEQ: Branch if Z set
                        {
                            auto PC = new_armstate.cpu.PC();
                            PC += 4; // prefetch operation

                            results_t result;
                            auto msg = gp_operation(&result, PC, inst.i32, 0, x86_asm_ADD);

                            //Set PC and cycle count
                            if(new_armstate.cpu.get_CPSR_Z()){
                                new_armstate.cpu.PC() = result.i32;
                                new_armstate.cpu.cycle_count += 2;
                            }
                            else{
                                new_armstate.cpu.PC() += 2;
                                new_armstate.cpu.cycle_count++;
                            }

                            return new_armstate;
                        }
                    case 1://BNE: Branch if Z clear
                        {
                            auto PC = new_armstate.cpu.PC();
                            PC += 4; // prefetch operation

                            results_t result;
                            auto msg = gp_operation(&result, PC, inst.i32, 0, x86_asm_ADD);

                            //Set PC and cycle count
                            if(!new_armstate.cpu.get_CPSR_Z()){
                                new_armstate.cpu.PC() = result.i32;
                                new_armstate.cpu.cycle_count += 2;
                            }
                            else{
                                new_armstate.cpu.PC() += 2;
                                new_armstate.cpu.cycle_count++;
                            }

                            return new_armstate;
                        }
                    case 2: //BCS: Branch if C set
                        {
                            auto PC = new_armstate.cpu.PC();
                            PC += 4; // prefetch operation

                            results_t result;
                            auto msg = gp_operation(&result, PC, inst.i32, 0, x86_asm_ADD);

                            //Set PC and cycle count
                            if(new_armstate.cpu.get_CPSR_C()){
                                new_armstate.cpu.PC() = result.i32;
                                new_armstate.cpu.cycle_count += 2;
                            }
                            else{
                                new_armstate.cpu.PC() += 2;
                                new_armstate.cpu.cycle_count++;
                            }

                            return new_armstate;
                        }
                    case 3: //BCC: Branch if C clear
                        {
                            auto PC = new_armstate.cpu.PC();
                            PC += 4; // prefetch operation

                            results_t result;
                            auto msg = gp_operation(&result, PC, inst.i32, 0, x86_asm_ADD);

                            //Set PC and cycle count
                            if(!new_armstate.cpu.get_CPSR_C()){
                                new_armstate.cpu.PC() = result.i32;
                                new_armstate.cpu.cycle_count += 2;
                            }
                            else{
                                new_armstate.cpu.PC() += 2;
                                new_armstate.cpu.cycle_count++;
                            }

                            return new_armstate;
                        }
                    case 4://BMI: Branch if N set
                        {
                            auto PC = new_armstate.cpu.PC();
                            PC += 4; // prefetch operation

                            results_t result;
                            auto msg = gp_operation(&result, PC, inst.i32, 0, x86_asm_ADD);

                            //Set PC and cycle count
                            if(new_armstate.cpu.get_CPSR_N()){
                                new_armstate.cpu.PC() = result.i32;
                                new_armstate.cpu.cycle_count += 2;
                            }
                            else{
                                new_armstate.cpu.PC() += 2;
                                new_armstate.cpu.cycle_count++;
                            }

                            return new_armstate;
                        }
                    case 5://BPL: Branch if N clear
                        {
                            auto PC = new_armstate.cpu.PC();
                            PC += 4; // prefetch operation

                            results_t result;
                            auto msg = gp_operation(&result, PC, inst.i32, 0, x86_asm_ADD);

                            //Set PC and cycle count
                            if(!new_armstate.cpu.get_CPSR_N()){
                                new_armstate.cpu.PC() = result.i32;
                                new_armstate.cpu.cycle_count += 2;
                            }
                            else{
                                new_armstate.cpu.PC() += 2;
                                new_armstate.cpu.cycle_count++;
                            }

                            return new_armstate;
                        }
                    case 6://BVS: Branch if V set
                        {
                            auto PC = new_armstate.cpu.PC();
                            PC += 4; // prefetch operation

                            results_t result;
                            auto msg = gp_operation(&result, PC, inst.i32, 0, x86_asm_ADD);

                            //Set PC and cycle count
                            if(new_armstate.cpu.get_CPSR_V()){
                                new_armstate.cpu.PC() = result.i32;
                                new_armstate.cpu.cycle_count += 2;
                            }
                            else{
                                new_armstate.cpu.PC() += 2;
                                new_armstate.cpu.cycle_count++;
                            }

                            return new_armstate;
                        }
                    case 7://BVC: Branch if V clear
                        {
                            auto PC = new_armstate.cpu.PC();
                            PC += 4; // prefetch operation

                            results_t result;
                            auto msg = gp_operation(&result, PC, inst.i32, 0, x86_asm_ADD);

                            //Set PC and cycle count
                            if(!new_armstate.cpu.get_CPSR_V()){
                                new_armstate.cpu.PC() = result.i32;
                                new_armstate.cpu.cycle_count += 2;
                            }
                            else{
                                new_armstate.cpu.PC() += 2;
                                new_armstate.cpu.cycle_count++;
                            }

                            return new_armstate;
                        }
                    case 8://BHI: Branch if C set and Z clear
                        {
                            auto PC = new_armstate.cpu.PC();
                            PC += 4; // prefetch operation

                            results_t result;
                            auto msg = gp_operation(&result, PC, inst.i32, 0, x86_asm_ADD);

                            //Set PC and cycle count
                            if((new_armstate.cpu.get_CPSR_C()) && (!new_armstate.cpu.get_CPSR_Z())){
                                new_armstate.cpu.PC() = result.i32;
                                new_armstate.cpu.cycle_count += 2;
                            }
                            else{
                                new_armstate.cpu.PC() += 2;
                                new_armstate.cpu.cycle_count++;
                            }

                            return new_armstate;
                        }
                    case 9://BLS: Branch if C clear or Z set
                        {
                            auto PC = new_armstate.cpu.PC();
                            PC += 4; // prefetch operation

                            results_t result;
                            auto msg = gp_operation(&result, PC, inst.i32, 0, x86_asm_ADD);

                            //Set PC and cycle count
                            if((new_armstate.cpu.get_CPSR_C() == false) || (new_armstate.cpu.get_CPSR_Z())){
                                new_armstate.cpu.PC() = result.i32;
                                new_armstate.cpu.cycle_count += 2;
                            }
                            else{
                                new_armstate.cpu.PC() += 2;
                                new_armstate.cpu.cycle_count++;
                            }

                            return new_armstate;
                        }
                    case 10://BGE: Branch if N set and V set OR N clear and V clear
                        {
                            auto PC = new_armstate.cpu.PC();
                            PC += 4; // prefetch operation

                            results_t result;
                            auto msg = gp_operation(&result, PC, inst.i32, 0, x86_asm_ADD);

                            //Set PC and cycle count
                            if(((new_armstate.cpu.get_CPSR_N() == true)  && (new_armstate.cpu.get_CPSR_V() == true)) ||
                               ((new_armstate.cpu.get_CPSR_N() == false) && (new_armstate.cpu.get_CPSR_V() == false))){

                                new_armstate.cpu.PC() = result.i32;
                                new_armstate.cpu.cycle_count += 2;
                            }
                            else{
                                new_armstate.cpu.PC() += 2;
                                new_armstate.cpu.cycle_count++;
                            }

                            return new_armstate;
                        }
                    case 11://BLT: Branch if N set and V clear OR N clear and V set
                        {
                            auto PC = new_armstate.cpu.PC();
                            PC += 4; // prefetch operation

                            results_t result;
                            auto msg = gp_operation(&result, PC, inst.i32, 0, x86_asm_ADD);

                            //Set PC and cycle count
                            if(((new_armstate.cpu.get_CPSR_N() == true) && (new_armstate.cpu.get_CPSR_V() == false)) ||
                               ((new_armstate.cpu.get_CPSR_N() == false) && (new_armstate.cpu.get_CPSR_V() == true))){

                                new_armstate.cpu.PC() = result.i32;
                                new_armstate.cpu.cycle_count += 2;
                            }
                            else{
                                new_armstate.cpu.PC() += 2;
                                new_armstate.cpu.cycle_count++;
                            }

                            return new_armstate;
                        }
                    case 12://BGT: Branch if Z clear, AND either N set and V set OR N clear and V clear
                        {
                            auto PC = new_armstate.cpu.PC();
                            PC += 4; // prefetch operation

                            results_t result;
                            auto msg = gp_operation(&result, PC, inst.i32, 0, x86_asm_ADD);

                            //Set PC and cycle count
                            if( (new_armstate.cpu.get_CPSR_Z() == false) &&
                                (((new_armstate.cpu.get_CPSR_N() == true) && (new_armstate.cpu.get_CPSR_V() == true)) ||
                                ((new_armstate.cpu.get_CPSR_N() == false) && (new_armstate.cpu.get_CPSR_V() == false))))  {

                                new_armstate.cpu.PC() = result.i32;
                                new_armstate.cpu.cycle_count += 2;
                            }
                            else{
                                new_armstate.cpu.PC() += 2;
                                new_armstate.cpu.cycle_count++;
                            }

                            return new_armstate;
                        }
                    case 13://BLE: Branch if Z set, OR N set and V clear OR N clear and V set
                        {
                            auto PC = new_armstate.cpu.PC();
                            PC += 4; // prefetch operation

                            results_t result;
                            auto msg = gp_operation(&result, PC, inst.i32, 0, x86_asm_ADD);

                            //Set PC and cycle count
                            if( (new_armstate.cpu.get_CPSR_Z()) ||
                                ((new_armstate.cpu.get_CPSR_N()) && (new_armstate.cpu.get_CPSR_V() == false)) ||
                                ((new_armstate.cpu.get_CPSR_N() == false) && (new_armstate.cpu.get_CPSR_V())))    {

                                new_armstate.cpu.PC() = result.i32;
                                new_armstate.cpu.cycle_count += 2;
                            }
                            else{
                                new_armstate.cpu.PC() += 2;
                                new_armstate.cpu.cycle_count++;
                            }

                            return new_armstate;
                        }
                    default:
                        std::runtime_error("execute_t16 (Bxx) : condition code is undefined and should not be used");
                }
            }
        case i_BIC  :// **DONE** bit clear
            {
                auto Rd = new_armstate.cpu.get_register(inst.Rd).u32;
                auto Rs = new_armstate.cpu.get_register(inst.Rs).u32;

                //Operation
                auto result = Rd & (~Rs);

                new_armstate.cpu.set_register_i32(inst.Rd, result);

                //Set Flags
                new_armstate.cpu.set_CPSR_N(result & (1 << 31));
                new_armstate.cpu.set_CPSR_Z(result == 0);
                new_armstate.cpu.set_CPSR_C(false);
                
                //Set PC and cycle count
                new_armstate.cpu.cycle_count++;
                if(inst.Rd != 15) new_armstate.cpu.PC() += 2;
                else              new_armstate.cpu.cycle_count++;

                return new_armstate;  
            }
        case i_BL   :// **DONE** branch and link
            {
                if(inst.meta_opcode == meta_R){
                    auto Rs = new_armstate.cpu.get_register(inst.Rs).u32;

                    new_armstate.cpu.PC() = Rs;

                    //Set cycle count
                    new_armstate.cpu.cycle_count += 2;

                    return new_armstate;
                }
                else
                    throw std::runtime_error("execute_t16(i_BL) : Invalid meta opcode ");
            }
        case i_BX   :// **DONE** branch and exchange
            {
                auto Rs = new_armstate.cpu.get_register(inst.Rs).u32;

                //operation
                new_armstate.cpu.PC() = Rs;

                //Set cycle count
                new_armstate.cpu.cycle_count += 2;

                return new_armstate;
            }
        case i_CBNZ: 
            {
                auto pc = new_armstate.cpu.PC();
                pc += 4; // prefetch operation

                //operation
                results_t result;
                auto msg = gp_operation(&result, pc, inst.u32, 0, x86_asm_ADD);
                new_armstate.cpu.PC() = result.u32;

                //Set cycle count
                new_armstate.cpu.cycle_count += 3;

                return new_armstate;
            }
        case i_CBZ: 
            {
                auto pc = new_armstate.cpu.PC();
                pc += 4; // prefetch operation

                //operation
                results_t result;
                auto msg = gp_operation(&result, pc, inst.u32, 0, x86_asm_ADD);
                new_armstate.cpu.PC() = result.u32;

                //Set cycle count
                new_armstate.cpu.cycle_count += 3;

                return new_armstate;
            }
        case i_CMN  :// **DONE** compare negative
            {
                results_t result;

                auto Rd = new_armstate.cpu.get_register(inst.Rd).i32;
                auto Rs = new_armstate.cpu.get_register(inst.Rs).i32;

                //Operation
                auto msg = gp_operation(&result, Rd, Rs, 0, x86_asm_ADD);

                //Set Flags
                new_armstate.cpu.set_CPSR_N(result.get_x86_flag_Sign());
                new_armstate.cpu.set_CPSR_Z(result.get_x86_flag_Zero());
                new_armstate.cpu.set_CPSR_C(result.get_x86_flag_Carry());
                new_armstate.cpu.set_APSR_V(result.get_x86_flag_Ov());

                //Set PC and cycle count
                new_armstate.cpu.cycle_count++;
                if(inst.Rd != 15) new_armstate.cpu.PC() += 2;
                else              new_armstate.cpu.cycle_count++;

                return new_armstate;
            }
        case i_CMP  :// **DONE** compare
            {
                if(inst.meta_opcode == meta_RC){

                    results_t result;

                    auto Rd     = new_armstate.cpu.get_register_u32(inst.Rd);
                    auto offset = inst.u_immediate;
                    
                    //Operation
                    auto msg = gp_operation(&result, Rd, offset, 0, x86_asm_SUB);  

                    //Set Flags
                    new_armstate.cpu.set_CPSR_N(result.get_x86_flag_Sign()); 
                    new_armstate.cpu.set_CPSR_Z(result.get_x86_flag_Zero());
                    new_armstate.cpu.set_CPSR_C(!result.get_x86_flag_Carry()); 
                    new_armstate.cpu.set_APSR_V(result.get_x86_flag_Ov());

                    //Set PC and cycle count
                    new_armstate.cpu.cycle_count++;
                    if(inst.Rd != 15) new_armstate.cpu.PC() += 2;
                    else              new_armstate.cpu.cycle_count++;

                    return new_armstate;

                }
                else if(inst.meta_opcode == meta_RR){

                    results_t result;

                    auto Rd = new_armstate.cpu.get_register(inst.Rd).i32;
                    auto Rs = new_armstate.cpu.get_register(inst.Rs).i32;

                    //std::cout << "\nr0 : " << std::hex << Rd << std::endl;
                    //std::cout << "r1: " << std::hex << Rs << std::endl;

                    //Operation
                    auto msg = gp_operation(&result, Rd, Rs, 0, x86_asm_SUB);

                    //Set Flags
                    new_armstate.cpu.set_CPSR_N(result.get_x86_flag_Sign());
                    new_armstate.cpu.set_CPSR_Z(result.get_x86_flag_Zero());
                    new_armstate.cpu.set_CPSR_C(!result.get_x86_flag_Carry()); //the carry flag behaves diferently in armV7. it is then inverted
                    new_armstate.cpu.set_APSR_V(result.get_x86_flag_Ov());

                    //Set PC and cycle count
                    new_armstate.cpu.cycle_count++;
                    if(inst.Rd != 15) new_armstate.cpu.PC() += 2;
                    else              new_armstate.cpu.cycle_count++;

                    return new_armstate;
                }
                else 
                    throw std::runtime_error("execute_t16(i_MOV) : Invalid meta opcode ");
            }
        case i_EOR  :// **DONE** bitwise XOR
            {
                auto Rd = new_armstate.cpu.get_register(inst.Rd).u32;
                auto Rs = new_armstate.cpu.get_register(inst.Rs).u32;

                auto res = Rd ^ Rs;

                // update these two flags with results
                new_armstate.cpu.set_CPSR_Z(res == 0);
                new_armstate.cpu.set_CPSR_N(res & (1 << 31));
                new_armstate.cpu.set_register_u32(inst.Rd, res);

                new_armstate.cpu.cycle_count++;
                if(inst.Rd != 15)
                    new_armstate.cpu.PC() += 2;
                else
                    new_armstate.cpu.cycle_count++;

                return new_armstate;
            }
        case i_LDMIA:// **DONE** load multiple
            {
                int  numregs   = 0;
                auto base_addr = new_armstate.cpu.get_register_u32(inst.Rb);

                //operation
                for(int i : range(0, 8)){
                    if(inst.Rlist & (1 << i)){
                        new_armstate.cpu.set_register_u32(i, new_armstate.memory.load_u32(base_addr));
                        base_addr += 4;
                        numregs++;
                    }
                }
                new_armstate.cpu.set_register_u32(inst.Rb, base_addr);

                //upate PC and cycle count
                new_armstate.cpu.PC() += 2;
                new_armstate.cpu.cycle_count = (numregs + 1);

                return new_armstate; 
            }
            //throw std::runtime_error("execute_t16 : opcode not implemented");
        case i_LDR  :// **DONE** load word
            if(inst.meta_opcode == meta_RC_pc) { // (6) Rd = word mem[PC + uimm]

                uint32_t pc = new_armstate.cpu.PC();
                pc += 2;                     // prefetch operation
                if(pc & (1 << 1)) pc += 2;   // bit [1] of PC is forced to be zero to ensure it is word-aligned
                
                uint32_t uimm = inst.u32;

                auto addr = pc + uimm;
                auto bytes = new_armstate.memory.load_u32(addr);
                new_armstate.cpu.set_register_u32(inst.Rd, bytes);

                // maintain cycle count and advance IP as needed
                new_armstate.cpu.cycle_count++;
                if(inst.Rd != 15) new_armstate.cpu.PC() += 2;
                else              new_armstate.cpu.cycle_count++;

                return new_armstate;
            }
            else if(inst.meta_opcode == meta_RRR) { // (7) Rd = word mem[Rb + Ro]

                auto Rb = new_armstate.cpu.get_register(inst.Rb).u32; // base
                auto Ro = new_armstate.cpu.get_register(inst.Ro).u32; // offset
                
                auto addr = Rb + Ro;
                auto bytes = new_armstate.memory.load_u32(addr);
                new_armstate.cpu.set_register_u32(inst.Rd, bytes);

                // maintain cycle count and advance IP as needed
                new_armstate.cpu.cycle_count++;
                if(inst.Rd != 15) new_armstate.cpu.PC() += 2;
                else              new_armstate.cpu.cycle_count++;

                return new_armstate;
            }
            else if(inst.meta_opcode == meta_RRC) { // (9) Rd = word mem[Rb + uimm]

                auto Rb = new_armstate.cpu.get_register(inst.Rb).u32;
                auto uimm = inst.u32;

                auto addr = Rb + uimm;
                auto bytes = new_armstate.memory.load_u32(addr);
                new_armstate.cpu.set_register_u32(inst.Rd, bytes);

                new_armstate.cpu.cycle_count++;
                if(inst.Rd != 15) new_armstate.cpu.PC() += 2;
                else              new_armstate.cpu.cycle_count++;

                return new_armstate;
            }
            else if(inst.meta_opcode == meta_RC_sp) { // (11) Rd = word mem[R13 + uimm]

                auto sp = new_armstate.cpu.SP();
                uint32_t uimm = inst.u32;

                //operation
                auto addr = sp + uimm;
                auto bytes = new_armstate.memory.load_u32(addr);
                new_armstate.cpu.set_register_u32(inst.Rd, bytes);

                // maintain cycle count and advance IP as needed
                new_armstate.cpu.cycle_count++;
                if(inst.Rd != 15) new_armstate.cpu.PC() += 2;
                else              new_armstate.cpu.cycle_count++;

                return new_armstate;
            }
            else {
                // ##6(RC_pc), may need to do some PC bit adjustment on this one 
                throw std::runtime_error("execute_t16(i_LDR) : invalid meta opcode ");
            }
        case i_LDRB :// **DONE** load byte 
            {
                if(inst.meta_opcode == meta_RRR){

                    auto Rb   = new_armstate.cpu.get_register(inst.Rb).u32;
                    auto Ro   = new_armstate.cpu.get_register(inst.Ro).u32;
                    auto addr = Rb + Ro;

                    //Operation
                    auto bytes = new_armstate.memory.load_u8(addr);
                    new_armstate.cpu.set_register_u32(inst.Rd, bytes);//it is automatically zero extended to 32-bits

                    // maintain cycle count and advance IP as needed
                    new_armstate.cpu.cycle_count++;
                    if(inst.Rd != 15) new_armstate.cpu.PC() += 2;
                    else              new_armstate.cpu.cycle_count++;

                    return new_armstate;
                }
                else if(inst.meta_opcode == meta_RRC){
                    
                    auto Rb       = new_armstate.cpu.get_register(inst.Rb).u32;
                    auto offset   = inst.u32;
                    auto addr = Rb + offset;

                    //Operation
                    auto bytes = new_armstate.memory.load_u8(addr);
                    new_armstate.cpu.set_register_u32(inst.Rd, bytes);

                    // maintain cycle count and advance IP as needed
                    new_armstate.cpu.cycle_count++;
                    if(inst.Rd != 15) new_armstate.cpu.PC() += 2;
                    else              new_armstate.cpu.cycle_count++;

                    return new_armstate;
                }
                else
                    throw std::runtime_error("execute_t16(i_LDRB) : invalid meta opcode ");
            }
        case i_LDRH :// **DONE** load halfword
            {
                if(inst.meta_opcode == meta_RRR){

                    auto Rb   = new_armstate.cpu.get_register(inst.Rb).u32;
                    auto Ro   = new_armstate.cpu.get_register(inst.Ro).u32;
                    auto addr = Rb + Ro;

                    //Operation
                    auto bytes = new_armstate.memory.load_u16(addr);
                    new_armstate.cpu.set_register_u32(inst.Rd, bytes);//it is automatically zero extended to 32-bits

                    // maintain cycle count and advance IP as needed
                    new_armstate.cpu.cycle_count++;
                    if(inst.Rd != 15) new_armstate.cpu.PC() += 2;
                    else              new_armstate.cpu.cycle_count++;

                    return new_armstate;
                }
                else if(inst.meta_opcode == meta_RRC){
                    
                    auto Rb       = new_armstate.cpu.get_register(inst.Rb).u32;
                    auto offset   = inst.u32;
                    auto addr     = Rb + offset;

                    //Operation
                    auto bytes = new_armstate.memory.load_u16(addr);
                    new_armstate.cpu.set_register_u32(inst.Rd, bytes);

                    // maintain cycle count and advance IP as needed
                    new_armstate.cpu.cycle_count++;
                    if(inst.Rd != 15) new_armstate.cpu.PC() += 2;
                    else              new_armstate.cpu.cycle_count++;

                    return new_armstate;
                }
                else
                    throw std::runtime_error("execute_t16(i_LDRH) : invalid meta opcode ");
            }
        case i_LSL  :// **DONE** logical shift left
            {
                if(inst.meta_opcode == meta_RRC){

                    auto imm5 = inst.u32;
                    auto Rs   = new_armstate.cpu.get_register_u32(inst.Rs);

                    //Operation
                    auto result = Rs << imm5;
                    new_armstate.cpu.set_register_u32(inst.Rd, result);

                    //Update flags
                    new_armstate.cpu.set_CPSR_N(result & (1 << 31));
                    new_armstate.cpu.set_CPSR_Z(result == 0);
                    if (imm5 != 0)
                        new_armstate.cpu.set_CPSR_C(1 & (Rs >> (32 - imm5)));

                    //maintain cycle count and PC
                    new_armstate.cpu.cycle_count++;
                    if(inst.Rd != 15)
                        new_armstate.cpu.PC() += 2;
                    else
                        new_armstate.cpu.cycle_count++;

                    return new_armstate;
                }
                else if(inst.meta_opcode == meta_RR){

                    auto Rd = new_armstate.cpu.get_register_i32(inst.Rd);
                    auto Rs = new_armstate.cpu.get_register_u32(inst.Rs);

                    //Operation
                    auto result = Rd << Rs;
                    new_armstate.cpu.set_register_u32(inst.Rd, result);

                    //Update flags
                    new_armstate.cpu.set_CPSR_N(result & (1 << 31));
                    new_armstate.cpu.set_CPSR_Z(result == 0);
                    if (Rs != 0)
                        new_armstate.cpu.set_CPSR_C(1 & (Rd >> (32 - Rs)));

                    //maintain cycle count and PC
                    new_armstate.cpu.cycle_count++;
                    if(inst.Rd != 15)
                        new_armstate.cpu.PC() += 2;
                    else
                        new_armstate.cpu.cycle_count++;

                    return new_armstate;
                }
                else
                    throw std::runtime_error("execute_t16(i_LSL) : invalid meta opcode ");
            }
        case i_LDSB :// **DONE** load sign-extended byte
            {
                auto Ro = new_armstate.cpu.get_register_i32(inst.Ro);
                auto Rb = new_armstate.cpu.get_register_i32(inst.Rb);
                auto addr = Ro + Rb;

                //operation
                auto result = new_armstate.memory.load_u8(addr);
                if(result >> 7) result = result | 0xFFFFFF00;
                else            result = result & 0x000000FF;
                new_armstate.cpu.set_register_i32(inst.Rd, result);

                // maintain cycle count and advance IP as needed
                new_armstate.cpu.cycle_count++;
                if(inst.Rd != 15) new_armstate.cpu.PC() += 2;
                else              new_armstate.cpu.cycle_count++;

                return new_armstate;
            }
        case i_LDSH :// **DONE** load sign-extended halfword
            {
                auto Ro = new_armstate.cpu.get_register_i32(inst.Ro);
                auto Rb = new_armstate.cpu.get_register_i32(inst.Rb);
                auto addr = Ro + Rb;

                //operation
                auto result = new_armstate.memory.load_u16(addr);
                if(result >> 15) result = result | 0xFFFFFF00;
                else            result = result & 0x000000FF;
                new_armstate.cpu.set_register_i32(inst.Rd, result);

                // maintain cycle count and advance IP as needed
                new_armstate.cpu.cycle_count++;
                if(inst.Rd != 15) new_armstate.cpu.PC() += 2;
                else              new_armstate.cpu.cycle_count++;

                return new_armstate;
            }
        case i_LSR  :// **DONE** logical shift right
            {
                if(inst.meta_opcode == meta_RRC){

                    auto imm5 = inst.u32;
                    auto Rs   = new_armstate.cpu.get_register_u32(inst.Rs);

                    //Operation
                    auto result = Rs >> imm5;
                    new_armstate.cpu.set_register_u32(inst.Rd, result);

                    //Update flags
                    new_armstate.cpu.set_CPSR_N(result & (1 << 31));
                    new_armstate.cpu.set_CPSR_Z(result == 0);
                    if(imm5 != 0)
                        new_armstate.cpu.set_CPSR_C(1 & (Rs >> (imm5 - 1)));//cary flag is updated to last bit shifted out

                    //maintain cycle count and PC
                    new_armstate.cpu.cycle_count++;
                    if(inst.Rd != 15)
                        new_armstate.cpu.PC() += 2;
                    else
                        new_armstate.cpu.cycle_count++;

                    return new_armstate;
                }
                else if(inst.meta_opcode == meta_RR){

                    auto Rd = new_armstate.cpu.get_register_i32(inst.Rd);
                    auto Rs = new_armstate.cpu.get_register_u32(inst.Rs);

                    //Operation
                    auto result = Rd >> Rs;
                    new_armstate.cpu.set_register_u32(inst.Rd, result);

                    //Update flags
                    new_armstate.cpu.set_CPSR_N(result & (1 << 31));
                    new_armstate.cpu.set_CPSR_Z(result == 0);
                    if (Rs != 0)
                        new_armstate.cpu.set_CPSR_C(1 & (Rd >> (Rs - 1)));

                    //maintain cycle count and PC
                    new_armstate.cpu.cycle_count++;
                    if(inst.Rd != 15)
                        new_armstate.cpu.PC() += 2;
                    else
                        new_armstate.cpu.cycle_count++;

                    return new_armstate;
                }
                else
                    throw std::runtime_error("execute_t16(i_LSR) : invalid meta opcode ");
            }
        case i_MOV  :// **DONE** move register
            {
                if(inst.meta_opcode == meta_RC){

                    auto imm = inst.i_immediate;
                    
                    new_armstate.cpu.set_register_i32(inst.Rd, imm);

                    //Set Flags 
                    new_armstate.cpu.set_CPSR_N(imm & (1 << 31));
                    new_armstate.cpu.set_CPSR_Z(imm == 0);
                    //new_armstate.cpu.set_CPSR_C(false); We assume that the carry flag will never be updated since there is no shifting before
                    //moving the sorce register value to destination register
                    //see on youtube: ARM Programming Tutorial 8- MOV Instruction Set and Barrel Shifter in ARM

                    //Set PC and cycle count
                    new_armstate.cpu.cycle_count++;
                    if(inst.Rd != 15) new_armstate.cpu.PC() += 2;
                    else              new_armstate.cpu.cycle_count++;

                    return new_armstate;
                }
                else if(inst.meta_opcode == meta_RR){

                    auto Hs = new_armstate.cpu.get_register(inst.Rs).i32;
                    
                    new_armstate.cpu.set_register_i32(inst.Rd, Hs);

                    //Set Flags: According to ARM7-TDMI Manual, MOV instruction does not update the flags
                    //new_armstate.cpu.set_CPSR_N(Hs & (1 << 31));
                    //new_armstate.cpu.set_CPSR_Z(Hs == 0);
                    //new_armstate.cpu.set_CPSR_C(false);

                    //Set PC and cycle count
                    new_armstate.cpu.cycle_count++;
                    if(inst.Rd != 15) new_armstate.cpu.PC() += 2;
                    else              new_armstate.cpu.cycle_count++;

                    return new_armstate;
                }
                else 
                    throw std::runtime_error("execute_t16(i_MOV) : Invalid meta opcode ");
            }
        case i_MUL  :// **DONE** multiply
            {
                auto Rd = new_armstate.cpu.get_register_i32(inst.Rd);
                auto Rs = new_armstate.cpu.get_register_i32(inst.Rs);

                //operation
                auto result = Rs * Rd;
                new_armstate.cpu.set_register_i32(inst.Rd, result);

                //flags update
                new_armstate.cpu.set_CPSR_N(result & (1 << 31));
                new_armstate.cpu.set_CPSR_Z(result == 0);

                //Set PC and cycle count
                new_armstate.cpu.cycle_count++;
                if(inst.Rd != 15) new_armstate.cpu.PC() += 2;
                else              new_armstate.cpu.cycle_count++;

                return new_armstate;
            }
        case i_MVN  :// **DONE** move negative register
            {
                auto Rs = new_armstate.cpu.get_register_i32(inst.Rs);

                //operation
                auto result = ~Rs;
                new_armstate.cpu.set_register_i32(inst.Rd, result);

                //flags update
                new_armstate.cpu.set_CPSR_N(result & (1 << 31));
                new_armstate.cpu.set_CPSR_Z(result == 0);
                new_armstate.cpu.set_CPSR_C(false);

                //Set PC and cycle count
                new_armstate.cpu.cycle_count++;
                if(inst.Rd != 15) new_armstate.cpu.PC() += 2;
                else              new_armstate.cpu.cycle_count++;

                return new_armstate;
            }
        case i_NOP  :// **DONE**
            {
                //update PC and cycle count
                new_armstate.cpu.PC() +=2;
                new_armstate.cpu.cycle_count++;

                return new_armstate;
            }
        case i_NEG  :// **DONE** negate
            {
                results_t result;
                auto Rs = new_armstate.cpu.get_register_i32(inst.Rs);

                //operation
                auto msg = gp_operation(&result, 0, Rs, 0, x86_asm_SUB);
                new_armstate.cpu.set_register_i32(inst.Rd, result.i32);

                //flags update
                new_armstate.cpu.set_CPSR_N(result.get_x86_flag_Sign());
                new_armstate.cpu.set_CPSR_Z(result.get_x86_flag_Zero());
                new_armstate.cpu.set_CPSR_C(result.get_x86_flag_Carry());
                new_armstate.cpu.set_CPSR_V(result.get_x86_flag_Ov());

                //Set PC and cycle count
                new_armstate.cpu.cycle_count++;
                if(inst.Rd != 15) new_armstate.cpu.PC() += 2;
                else              new_armstate.cpu.cycle_count++;

                return new_armstate;
            }
        case i_ORR  :// **DONE** bitwise OR
            {
                auto Rd = new_armstate.cpu.get_register(inst.Rd).u32;
                auto Rs = new_armstate.cpu.get_register(inst.Rs).u32;

                auto res = Rd | Rs;

                // update these two flags with results
                new_armstate.cpu.set_CPSR_Z(res == 0);
                new_armstate.cpu.set_CPSR_N(res & (1 << 31));
                new_armstate.cpu.set_register_u32(inst.Rd, res);

                new_armstate.cpu.cycle_count++;
                if(inst.Rd != 15)
                    new_armstate.cpu.PC() += 2;
                else
                    new_armstate.cpu.cycle_count++;

                return new_armstate;
            }
        case i_POP  :// **DONE** pop registers
            {
                new_armstate.cpu.set_stack_mode(armv7_m3::stack_mode_FullDescending);

                address32_t sp = new_armstate.cpu.SP();
                int numregs = 0;

                // pop PC if needed
                if(inst.meta_opcode == meta_C_pc) {
                    sp = pop_word(new_armstate.cpu, new_armstate.memory, sp, 15);
                    new_armstate.cpu.cycle_count++;
                }
                else if(inst.meta_opcode != meta_C)
                    throw ExecuteError("execute(i_POP), invalid meta opcode");

                // pop appropriate GP registers
                for(int i : range(7, -1)) {
                    if(inst.Rlist & (1 << i)) {
                        sp = pop_word(new_armstate.cpu, new_armstate.memory, sp, i);
                        numregs++;
                    }
                }

                if(inst.meta_opcode != meta_C_pc)
                    new_armstate.cpu.PC() += 2;

                new_armstate.cpu.SP() = sp;
                new_armstate.cpu.cycle_count += (numregs + 1);

                return new_armstate;
            }
        case i_PUSH :// **DONE** push registers
            {
                new_armstate.cpu.set_stack_mode(armv7_m3::stack_mode_FullDescending);

                address32_t sp = new_armstate.cpu.SP();
                int numregs = 0;

                // push appropriate GP registers onto stack
                for(int i : range(0, 8)) {
                    if(inst.Rlist & (1 << i)) {
                        sp = push_word(new_armstate.cpu, new_armstate.memory, sp, new_armstate.cpu.get_register_u32(i));
                        numregs++;
                    }
                }

                // push LR if needed
                if(inst.meta_opcode == meta_C_lr)
                    sp = push_word(new_armstate.cpu, new_armstate.memory, sp, new_armstate.cpu.LR());
                else if(inst.meta_opcode != meta_C)
                    throw ExecuteError("execute(i_PUSH), invalid meta opcode");

                new_armstate.cpu.SP() = sp;
                new_armstate.cpu.PC() += 2;
                new_armstate.cpu.cycle_count += (numregs + 1);

                return new_armstate;
            }
        case i_ROR  :// rotate right
            throw std::runtime_error("execute_t16(ROR) : opcode not implemented");
        case i_SBC  ://**DONE** subtract with carry
            {
                auto Rd      = new_armstate.cpu.get_register_i32(inst.Rd);
                auto Rs      = new_armstate.cpu.get_register_i32(inst.Rs);
                int CarryBit = new_armstate.cpu.get_CPSR_C();

                results_t result;
                
                //operation
                auto msg = gp_operation(&result, Rd, Rs, CarryBit, x86_asm_SBB);
                new_armstate.cpu.set_register_i32(inst.Rd, result.i32);

                //set flas
                new_armstate.cpu.set_CPSR_N(result.get_x86_flag_Sign());
                new_armstate.cpu.set_CPSR_Z(result.get_x86_flag_Zero());
                new_armstate.cpu.set_CPSR_C(result.get_x86_flag_Carry());
                new_armstate.cpu.set_CPSR_V(result.get_x86_flag_Ov());

                //maintain cycle count and advance IP as needed
                new_armstate.cpu.cycle_count++;
                if(inst.Rd != 15) new_armstate.cpu.PC() += 2;
                else              new_armstate.cpu.cycle_count++;

                return new_armstate;
            }
        case i_STMIA:// **DONE** store multiple
            {
                int numregs    = 0;
                auto base_addr = new_armstate.cpu.get_register_u32(inst.Rb);

                //operation
                for(int i : range(0, 8)){
                    if(inst.Rlist & (1 << i)){
                        new_armstate.memory.store_u32(base_addr, new_armstate.cpu.get_register_u32(i));
                        base_addr +=4;
                        numregs++;
                    }
                }
                new_armstate.cpu.set_register_u32(inst.Rb, base_addr);

                //update cycle count and PC
                new_armstate.cpu.PC() += 2;
                new_armstate.cpu.cycle_count += (numregs + 1);

                return new_armstate;

            }
        case i_STR  :// **DONE** store word
            {
                if(inst.meta_opcode == meta_RRC){

                    results_t address;

                    auto Rb  = new_armstate.cpu.get_register(inst.Rb).i32;
                    auto Rd  = new_armstate.cpu.get_register(inst.Rd).i32;
                    auto imm = inst.i32;
                    
                    auto msg = gp_operation(&address, Rb, imm, 0, x86_asm_ADD);

                    //Operation
                    new_armstate.memory.store_u32(address.u32, Rd); //should this be address.i32 + 4 ?

                     //update PC and cycle count
                    new_armstate.cpu.PC()        += 2;
                    new_armstate.cpu.cycle_count += 2;//could be one

                    return new_armstate;
                }
                else if(inst.meta_opcode == meta_RC_sp){

                    results_t address;

                    auto SP  = new_armstate.cpu.get_SP();
                    auto Rd  = new_armstate.cpu.get_register(inst.Rd).i32;
                    auto imm = inst.i32;
                    
                    auto msg = gp_operation(&address, SP, imm, 0, x86_asm_ADD);

                    //Operation
                    new_armstate.memory.store_u32(address.u32, Rd); //should this be address.i32 + 4 ?

                     //update PC and cycle count
                    new_armstate.cpu.PC()        += 2;
                    new_armstate.cpu.cycle_count += 2;//could be one

                    return new_armstate;
                }
                else if(inst.meta_opcode == meta_RRR){

                    results_t address;

                    auto Rb = new_armstate.cpu.get_register(inst.Rb).i32;
                    auto Ro = new_armstate.cpu.get_register(inst.Ro).i32;
                    auto Rd = new_armstate.cpu.get_register(inst.Rd).i32;
                    
                    auto msg = gp_operation(&address, Rb, Ro, 0, x86_asm_ADD);

                    //Operation
                    new_armstate.memory.store_u32(address.i32, Rd); //should this be address.i32 + 4 ?

                     //update PC and cycle count
                    new_armstate.cpu.PC()        += 2;
                    new_armstate.cpu.cycle_count += 2;//could be one

                    return new_armstate;
                }
                else 
                    throw std::runtime_error("execute_t16(i_STR) : Invalid meta opcode ");
            }
        case i_STRB ://**DONE** store byte
            {
                if(inst.meta_opcode == meta_RRC){
                    
                    auto imm32 = inst.u32;
                    auto Rb    = new_armstate.cpu.get_register_u32(inst.Rb);
                    auto Rd    = new_armstate.cpu.get_register_u32(inst.Rd);
                    
                    auto addr    = Rb + imm32;
                    auto Rd_byte = Rd & 0xFF;

                    //Operation
                    new_armstate.memory.store_u8(addr, Rd_byte);

                    //update PC and cycle count
                    new_armstate.cpu.PC()        += 2;
                    new_armstate.cpu.cycle_count += 2;//could be one

                    return new_armstate;
                }
                else if(inst.meta_opcode = meta_RRR){

                    auto Rb    = new_armstate.cpu.get_register_u32(inst.Rb);
                    auto Ro    = new_armstate.cpu.get_register_u32(inst.Ro);
                    auto Rd    = new_armstate.cpu.get_register_u32(inst.Rd);
                    
                    auto addr    = Rb + Ro;
                    auto Rd_byte = Rd & 0xFF;

                    //Operation
                    new_armstate.memory.store_u8(addr, Rd_byte);

                    //update PC and cycle count
                    new_armstate.cpu.PC()        += 2;
                    new_armstate.cpu.cycle_count += 2;//could be one

                    return new_armstate;
                }
                else
                    throw std::runtime_error("execute_t16(i_STRB) : Invalid meta opcode ");
            }
        case i_STRH :// **DONE** store halfword
            {
                if(inst.meta_opcode == meta_RRC){
                    
                    auto imm32 = inst.u32;
                    auto Rb    = new_armstate.cpu.get_register_u32(inst.Rb);
                    auto Rd    = new_armstate.cpu.get_register_u32(inst.Rd);
                    
                    auto addr    = Rb + imm32;
                    auto Rd_byte = Rd & 0xFFFF;

                    //Operation
                    new_armstate.memory.store_u16(addr, Rd_byte);

                    //update PC and cycle count
                    new_armstate.cpu.PC()        += 2;
                    new_armstate.cpu.cycle_count += 2;//could be one

                    return new_armstate;
                }
                else if(inst.meta_opcode = meta_RRR){

                    auto Rb    = new_armstate.cpu.get_register_u32(inst.Rb);
                    auto Ro    = new_armstate.cpu.get_register_u32(inst.Ro);
                    auto Rd    = new_armstate.cpu.get_register_u32(inst.Rd);
                    
                    auto addr    = Rb + Ro;
                    auto Rd_byte = Rd & 0xFFFF;

                    //Operation
                    new_armstate.memory.store_u16(addr, Rd_byte);

                    //update PC and cycle count
                    new_armstate.cpu.PC()        += 2;
                    new_armstate.cpu.cycle_count += 2;//could be one

                    return new_armstate;
                }
                else
                    throw std::runtime_error("execute_t16(i_STRH) : Invalid meta opcode ");
            }
        case i_SWI  :// software interrupt
        case i_SUB  :// **DONE** subtract
            {
                if(inst.meta_opcode == meta_RRR){

                    results_t result;

                    auto Rs = new_armstate.cpu.get_register_u32(inst.Rs);
                    auto Rn = new_armstate.cpu.get_register_u32(inst.Rn);

                    //operation
                    auto msg = gp_operation(&result, Rs, Rn, 0, x86_asm_SUB);
                    new_armstate.cpu.set_register_u32(inst.Rd, result.i32);

                    //update flags
                    new_armstate.cpu.set_CPSR_N(result.get_x86_flag_Sign());
                    new_armstate.cpu.set_CPSR_Z(result.get_x86_flag_Zero());
                    new_armstate.cpu.set_CPSR_C(result.get_x86_flag_Carry());
                    new_armstate.cpu.set_CPSR_V(result.get_x86_flag_Ov());

                    //update PC and cycle count
                    new_armstate.cpu.cycle_count++;
                    if(inst.Rd != 15)
                        new_armstate.cpu.PC() += 2;
                    else
                        new_armstate.cpu.cycle_count++;

                    return new_armstate;
                }
                else if(inst.meta_opcode == meta_RRC){

                    results_t result;

                    auto imm3 = inst.u32;
                    auto Rs   = new_armstate.cpu.get_register_u32(inst.Rs);

                    //operation
                    auto msg = gp_operation(&result, Rs, imm3, 0, x86_asm_SUB);
                    new_armstate.cpu.set_register_u32(inst.Rd, result.i32);

                    //update flags
                    new_armstate.cpu.set_CPSR_N(result.get_x86_flag_Sign());
                    new_armstate.cpu.set_CPSR_Z(result.get_x86_flag_Zero());
                    new_armstate.cpu.set_CPSR_C(result.get_x86_flag_Carry());
                    new_armstate.cpu.set_CPSR_V(result.get_x86_flag_Ov());

                    //update PC and cycle count
                    new_armstate.cpu.cycle_count++;
                    if(inst.Rd != 15)
                        new_armstate.cpu.PC() += 2;
                    else
                        new_armstate.cpu.cycle_count++;

                    return new_armstate;
                }
                else if(inst.meta_opcode == meta_RC){

                    results_t result;

                    auto imm8 = inst.u32;
                    auto Rd   = new_armstate.cpu.get_register_u32(inst.Rd);

                    //operation
                    auto msg = gp_operation(&result, Rd, imm8, 0, x86_asm_SUB);
                    new_armstate.cpu.set_register_u32(inst.Rd, result.i32);

                    //update flags
                    new_armstate.cpu.set_CPSR_N(result.get_x86_flag_Sign());
                    new_armstate.cpu.set_CPSR_Z(result.get_x86_flag_Zero());
                    new_armstate.cpu.set_CPSR_C(result.get_x86_flag_Carry());
                    new_armstate.cpu.set_CPSR_V(result.get_x86_flag_Ov());

                    //update PC and cycle count
                    new_armstate.cpu.cycle_count++;
                    if(inst.Rd != 15)
                        new_armstate.cpu.PC() += 2;
                    else
                        new_armstate.cpu.cycle_count++;

                    return new_armstate;
                }
                else 
                    throw std::runtime_error("execute_t16(i_SUB) : Invalid meta opcode ");
            }
        case i_TST  :// test bits
            throw std::runtime_error("execute_t16(STRB, STRH, SWI, SUB, TST) : opcode not implemented");
        default:
            throw std::runtime_error("execute_t16 : invalid opcode");
    }

}

armstate_t execute_t32(armstate_t& armstate, instruction_32b_t& inst) {
    
    auto new_armstate = armstate;

    new_armstate.cpu.cpu_id++;

    switch (inst.opcode){

        case t32_ADC:
            {
                if(inst.meta_opcode == meta_t32_imm){
                    
                    auto Rn       = new_armstate.cpu.get_register(inst.Rn).i32;
                    auto imm      = inst.i32;
                    int carryBit = new_armstate.cpu.get_CPSR_C(); 

                    results_t result;

                    auto msg = gp_operation(&result, Rn, imm, carryBit, x86_asm_ADC);

                    new_armstate.cpu.set_register_i32(inst.Rd, result.i32);

                    //Set Flags
                    if(inst.S){
                        new_armstate.cpu.set_CPSR_N(result.get_x86_flag_Sign());
                        new_armstate.cpu.set_CPSR_Z(result.get_x86_flag_Zero());
                        new_armstate.cpu.set_CPSR_C(result.get_x86_flag_Carry());
                        new_armstate.cpu.set_CPSR_V(result.get_x86_flag_Ov());
                    }
                    
                    //not sure about this
                    new_armstate.cpu.cycle_count++;
                    if(inst.Rd != 15) 
                        new_armstate.cpu.PC() += 4;
                    else        
                        new_armstate.cpu.cycle_count++;

                    return new_armstate;
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

                                auto Rn  = new_armstate.cpu.get_register(inst.Rn).i32;
                                auto imm = inst.i32;

                                auto msg = gp_operation(&result, Rn, imm, 0, x86_asm_ADD);

                                new_armstate.cpu.set_register_i32(inst.Rd, result.i32);

                                //Set Flags
                                if(inst.S){
                                    new_armstate.cpu.set_CPSR_N(result.get_x86_flag_Sign());
                                    new_armstate.cpu.set_CPSR_Z(result.get_x86_flag_Zero());
                                    new_armstate.cpu.set_CPSR_C(result.get_x86_flag_Carry());
                                    new_armstate.cpu.set_CPSR_V(result.get_x86_flag_Ov());
                                }

                                //Set PC and cycle counts
                                new_armstate.cpu.cycle_count++;
                                if(inst.Rd != 15)
                                    new_armstate.cpu.PC() += 4;
                                else
                                    new_armstate.cpu.cycle_count++;

                                

                                return new_armstate;
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

                    auto Rn  = new_armstate.cpu.get_register(inst.Rn).i32;
                    auto imm = inst.i32;
                    auto result = Rn & imm;

                    new_armstate.cpu.set_register_i32(inst.Rd, result);
                    
                    //Set Flags
                    if(inst.S){
                        new_armstate.cpu.set_CPSR_N(result & (1 << 31));
                        new_armstate.cpu.set_CPSR_Z(result == 0);
                        new_armstate.cpu.set_CPSR_C(false);
                    }
                    
                    //Set PC and cycle count
                    new_armstate.cpu.cycle_count++;
                    if(inst.Rd != 15) 
                        new_armstate.cpu.PC() += 4;
                    else        
                        new_armstate.cpu.cycle_count++;

                    return new_armstate;
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
                    
                    auto Rn     = new_armstate.cpu.get_register(inst.Rn).i32;
                    auto imm    = inst.i32;
                    auto result = Rn & (~imm);

                    new_armstate.cpu.set_register_i32(inst.Rd, result);

                    //Set Flags
                    if(inst.S){
                        new_armstate.cpu.set_CPSR_N(result & (1 << 31));
                        new_armstate.cpu.set_CPSR_Z(result == 0);
                        new_armstate.cpu.set_CPSR_C(false);
                    }
                    
                    //not sure about this
                    new_armstate.cpu.cycle_count++;
                    if(inst.Rd != 15) 
                        new_armstate.cpu.PC() += 4;
                    else        
                        new_armstate.cpu.cycle_count++;

                    return new_armstate;
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

                    auto Rn  = new_armstate.cpu.get_register(inst.Rn).i32;
                    auto imm = inst.i32;

                    results_t result;

                    auto msg = gp_operation(&result, Rn, imm, 0, x86_asm_ADD);
                    
                    //Set Flags
                    new_armstate.cpu.set_CPSR_N(result.get_x86_flag_Sign());
                    new_armstate.cpu.set_CPSR_Z(result.get_x86_flag_Zero());
                    new_armstate.cpu.set_CPSR_C(false); 
                    new_armstate.cpu.set_CPSR_V(result.get_x86_flag_Ov());
                    
                    
                    //Set PC and cycle count
                    new_armstate.cpu.cycle_count++;
                    if(inst.Rd != 15) 
                        new_armstate.cpu.PC() += 4;
                    else        
                        new_armstate.cpu.cycle_count++;

                    

                    return new_armstate;
                }
                else
                    throw std::runtime_error("execute_t32 : invalid meta_opcode for CMN instruction");
            }   
        case t32_CMP:
            {
                if(inst.meta_opcode == meta_t32_imm){

                    auto Rn  = new_armstate.cpu.get_register(inst.Rn).i32;
                    auto imm = inst.i32;

                    results_t result;

                    auto msg = gp_operation(&result, Rn, imm, 0, x86_asm_SUB);
                    
                    //Set Flags
                    new_armstate.cpu.set_CPSR_N(result.get_x86_flag_Sign());
                    new_armstate.cpu.set_CPSR_Z(result.get_x86_flag_Zero());
                    new_armstate.cpu.set_CPSR_C(result.get_x86_flag_Carry()); 
                    new_armstate.cpu.set_CPSR_V(result.get_x86_flag_Ov());
                    
                    
                    //Set PC and cycle count
                    new_armstate.cpu.cycle_count++;
                    if(inst.Rd != 15) 
                        new_armstate.cpu.PC() += 4;
                    else        
                        new_armstate.cpu.cycle_count++;

                    

                    return new_armstate;
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

                    auto Rn  = new_armstate.cpu.get_register(inst.Rn).i32;
                    auto imm = inst.i32;
                    auto result = Rn ^ imm;

                    new_armstate.cpu.set_register_i32(inst.Rd, result);
                    
                    //Set Flags
                    if(inst.S){
                        new_armstate.cpu.set_CPSR_N(result & (1 << 31));
                        new_armstate.cpu.set_CPSR_Z(result == 0);
                        new_armstate.cpu.set_CPSR_C(false);
                    }
                    
                    //Set PC and cycle count
                    new_armstate.cpu.cycle_count++;
                    if(inst.Rd != 15) 
                        new_armstate.cpu.PC() += 4;
                    else        
                        new_armstate.cpu.cycle_count++;

                    

                    return new_armstate;
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
            if(inst.meta_opcode == meta_t32_imm){
                switch(inst.encoding){
                    case instruction_32b_t::encoding_T3:
                        {
                            auto imm = inst.i32;
                            auto pc  = new_armstate.cpu.PC();
                            pc += 4;

                            auto addr  = pc + imm;
                            auto bytes = new_armstate.memory.load_u32(addr);

                            new_armstate.cpu.set_register_i32(inst.Rd, bytes);

                            //Set PC and count cycle
                            new_armstate.cpu.cycle_count++;
                            if(inst.Rd != 15)
                                new_armstate.cpu.PC() += 4;
                            else
                                new_armstate.cpu.cycle_count++;

                            return new_armstate;
                        }
                    default:
                        throw std::runtime_error("execute_t32 : invalid encoding for LDR instruction");
                }
            }
            else
                throw std::runtime_error("execute_t32 : invalid meta_opcode for LDR instruction");
        case t32_LDRB:
            {
                if(inst.meta_opcode == meta_t32_reg){
                    
                    auto Rn      = new_armstate.cpu.get_register_u32(inst.Rn);
                    auto Rm      = new_armstate.cpu.get_register_u32(inst.Rm);
                    auto shift_l = inst.u32;

                    Rm = Rm << shift_l;

                    auto addr = Rn + Rm;

                    //operation
                    auto byte = new_armstate.memory.load_u8(addr);
                    new_armstate.cpu.set_register_u32(inst.Rt, byte);

                    //Set PC and count cycle
                    new_armstate.cpu.cycle_count++;
                    if(inst.Rd != 15)
                        new_armstate.cpu.PC() += 4;
                    else
                        new_armstate.cpu.cycle_count++;

                    return new_armstate;
                }
                else
                    throw std::runtime_error("execute_t32 : invalid meta_opcode for LDRB instruction");
            } 
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
            if(inst.meta_opcode == meta_t32_imm){
                switch(inst.encoding){
                    case instruction_32b_t::encoding_T2:
                        {
                            auto result = inst.i32;

                            new_armstate.cpu.set_register_i32(inst.Rd, result);

                            //Set Flags
                            if(inst.S){
                                new_armstate.cpu.set_CPSR_N(result & (1 << 31));
                                new_armstate.cpu.set_APSR_Z(result == 0);
                                new_armstate.cpu.set_CPSR_C(false);
                            }

                            //not sure about this
                            new_armstate.cpu.cycle_count++;
                            if(inst.Rd != 15) 
                                new_armstate.cpu.PC() += 4;
                            else        
                                new_armstate.cpu.cycle_count++;

                            return new_armstate;
                        }
                    default:
                        throw std::runtime_error("execute_t32 : invalid encoding for MOV instruction");
                }
            }
            else
                throw std::runtime_error("execute_t32 : invalid meta_opcode for BIC instruction");
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
            if(inst.meta_opcode == meta_t32_imm){

                auto Rn  = new_armstate.cpu.get_register(inst.Rn).u32;
                auto imm = inst.u32;
                auto result = Rn | imm;

                new_armstate.cpu.set_register_u32(inst.Rd, result);
                
                //Set Flags
                if(inst.S){
                    new_armstate.cpu.set_CPSR_N(result & (1 << 31));
                    new_armstate.cpu.set_CPSR_Z(result == 0);
                    new_armstate.cpu.set_CPSR_C(false);
                }
                
                //not sure about this
                new_armstate.cpu.cycle_count++;
                if(inst.Rd != 15) 
                    new_armstate.cpu.PC() += 4;
                else        
                    new_armstate.cpu.cycle_count++;

                return new_armstate;
            }
            else
                throw std::runtime_error("execute_t32 : invalid meta_opcode for ORR instruction");                
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
            throw std::runtime_error("execute_t32 : opcode not implemented");
        case t32_STR   :
            {
                if(inst.meta_opcode == meta_t32_imm){

                    switch(inst.encoding){
                        case instruction_32b_t::encoding_T3:
                            {
                                auto imm32 = inst.u32;
                                auto Rn    = new_armstate.cpu.get_register_u32(inst.Rn);
                                auto Rt    = new_armstate.cpu.get_register_u32(inst.Rt);

                                auto addr  = Rn + imm32;

                                //operation
                                new_armstate.memory.store_u32(addr, Rt);

                                //update PC and cycle count
                                new_armstate.cpu.PC()        += 4;
                                new_armstate.cpu.cycle_count += 2;//could be one

                                return new_armstate;

                            }
                        default:
                            throw std::runtime_error("execute_t32 : invalid encoding for STR instruction");
                    }
                }
                else if(inst.meta_opcode == meta_t32_reg){
                    throw std::runtime_error("execute_t32 : meta_t32_reg not implemeented for STR instruction");
                }
                else
                    throw std::runtime_error("execute_t32 : invalid meta_opcode for STR instruction");
            }
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
