#include <iostream>
#include <inst_mov_shifted_register.h>
#include <arm_instruction_asm.h>

// MOVE SHIFTED REGISTER
address_t inst::thumb::move_shifted_register(arm_cpu& cpu, memory_t& mem, uint32_t inst) {

    int r_dest = inst & 0x07;         // b111
    int r_src  = (inst >> 3) & 0x07;  // b111
    int offset = (inst >> 6) & 0x1F;  // b11111
    int op     = (inst >> 11) & 0x03; // b11

    switch(op) {
        case 0: // logical shift left
            {
                uint32_t src_val = cpu.get_register_uint(r_src);
                src_val <<= offset;
                cpu.set_register_uint(r_dest, src_val);
            }
            break;
        case 1: // logical shift right
            {
                uint32_t src_val = cpu.get_register_uint(r_src);
                src_val >>= offset;
                cpu.set_register_uint(r_dest, src_val);
            }
            break;
        case 2: // arithmetic shift right
            {
                int32_t src_val = cpu.get_register_int(r_src);
                src_val = arithmetic_shift_right(src_val, offset);
                cpu.set_register_int(src_val);
            }
            break;
        default:
            throw std::runtime_error("inst::thumb::msr (move shifted register) : unknown operation code");
    }

    // thumb instructions are 2 bytes each
    return cpu.get_register_uint(arm_PC) + 2;

}

// ADD/SUBTRACT (IMMEDIATE)
address_t inst::thumb::add_subtract(arm_cpu& cpu, memory_t& mem, uint32_t inst) {

    int r_dest   = inst & 0x07;
    int r_src    = (inst >> 3)  & 0x07;
    int rn_off   = (inst >> 6)  & 0x07;
    int op       = (inst >> 9)  & 0x01; 
    int imm_flag = (inst >> 10) & 0x01;

    if(op) { // ADD
        if(imm_flag) { // use immediate

            int32_t src_val = cpu.get_register_int(r_src);
            src_val += rn_off;
            cpu.set_register_int(r_dest, src_val);

        }
        else { // use register

            int32_t src_val_s = cpu.get_register_int(r_src);
            int32_t src_val_n = cpu.get_register_int(rn_off);
            src_val += src_val_n;
            cpu.set_register_int(r_dest);

        }
    }
    else { // SUBTRACT   
        if(imm_flag) { // use immediate

            int32_t src_val = cpu.get_register_int(r_src);
            src_val -= rn_off;
            cpu.set_register_int(r_dest, src_val);

        }
        else { // use register

            int32_t src_val_s = cpu.get_register_int(r_src);
            int32_t src_val_n = cpu.get_register_int(rn_off);
            src_val -= src_val_n;
            cpu.set_register_int(r_dest);

        }
    }

    return cpu.get_register_uint(arm_PC) + 2;
}

// MOVE/COMPARE/ADD/SUBTRACT IMMEDIATE
address_t inst::thumb::mcas_imm(arm_cpu& cpu, memory_t& mem, uint32_t inst) {

    int offset = inst & 0xFF;
    int r_dest = (inst >> 8)  & 0x07;
    int op     = (inst >> 11) & 0x03;

    switch(op) {
        case 0: // mov
            {
                cpu.set_register_int(r_dest, offset);
            }
            break;
        case 1: // compare
            {
                // will require asm routine to properly set flags
            }
            break;
        case 2: // add
            {
                int32_t src_val = cpu.get_register_int(r_dest);
                src_val += offset;
                cpu.set_register_int(r_dest, src_val);
            }
            break;
        case 3: // sub
            {
                int32_t src_val = cpu.get_register_int(r_dest);
                src_val -= offset;
                cpu.set_register_int(r_dest, src_val);
            }
            break;
        default:
            throw std::runtime_error("inst::thumb::mcas_imm (move/compare/add/subtract) : unknown operation code");
    }

    return cpu.get_register_uint(arm_PC) + 2;
}


