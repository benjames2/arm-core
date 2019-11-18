#include <iostream>
#include <instructions.h>
#include <arm_instruction_asm.h>

// decode ARM thumb instruction
address_t inst::thumb::decode(arm_cpu& cpu, memory_t& mem, uint32_t inst) {
    int op_field = (inst >> 13) & 0x07;

    switch(op_field) {
        case 0:
            {
                int op = (inst >> 11) & 0x03;
                if(op == 3) {
                    return inst::thumb::move_shifted_register(cpu, mem, inst);
                }
                else {
                    return inst::thumb::add_subtract(cpu, mem, inst);
                }
            }
            break; // the breaks in this switch case shouldnt be required but imma leave them there because im paranoid
        case 1:
            return inst::thumb::mcas_imm(cpu, mem, inst);
            break;
        /*
        case 2:
            {
                int op = (inst >> 12) & 0x01;
                if(op == 1) {
                    ;
                }
            }
            break;
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        */
        default:
            throw std::runtime_error("inst::thumb::decode : unknown operation");
    }

}

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
                for(int i = 0; i < offset; i++)
                    src_val = arithmetic_shift_right(src_val);
                cpu.set_register_int(r_dest, src_val);
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
            src_val_s += src_val_n;
            cpu.set_register_int(r_dest, src_val_s);

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
            src_val_s -= src_val_n;
            cpu.set_register_int(r_dest, src_val_s);

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

address_t inst::thumb::alu_operations(arm_cpu& cpu, memory_t& mem, uint32_t inst) {

    int r_dest = inst & 0x07;
    int r_src  = (inst >> 3) & 0x07;
    int op     = (inst >> 6) & 0x0F;

    union {
        int32_t i32;
        uint32_t u32;
    } src;

    union {
        int32_t i32;
        uint32_t u32;
    } dest;

    src.u32  = cpu.get_register_uint(r_src);
    dest.u32 = cpu.get_register_uint(r_dest);

    switch(op) {
        case 0: // AND Rd, Rs
            {
                dest.u32 &= src.u32;
                cpu.set_register_uint(r_dest, dest.u32);
            }
            break;
        case 1: // EOR Rd, Rs
            {
                dest.u32 ^= src.u32;
                cpu.set_register_uint(r_dest, dest.u32);
            }
            break;
        case 2: // LSL Rd, Rs
            {
                dest.u32 <<= src.u32;
                cpu.set_register_uint(r_dest, dest.u32);
            }
            break;
        case 3: // LSR Rd, Rs
            {
                dest.u32 >>= src.u32;
                cpu.set_register_uint(r_dest, dest.u32);
            }
            break;
        //case 4: // ASR Rd, Rs
        case 5: // ADC Rd, Rs
            {
                dest.i32 += src.i32;
                if(cpu.get_flag_carry())
                    dest.i32++;
                cpu.set_register_int(r_dest, dest.i32);
            }
            break;
        //case 6: // SBC Rd, Rs
        //case 7: // ROR Rd, Rs
            {
                for(int i = 0; i < src.u32; i++)
                    dest.u32 = rotate_right(dest.u32);
                cpu.set_register_uint(r_dest, dest.u32);
            }
            break;
        //case 8: // TST Rd, Rs
        case 9: // NEG Rd, Rs
            {
                dest.i32 = -src.i32;
                cpu.set_register_int(r_dest, dest.i32);
            }
            break;
        //case 10: // CMP Rd, Rs
        //case 11: // CMN Rd, Rs
        case 12: // ORR Rd, Rs
            {
                dest.u32 |= src.u32;
                cpu.set_register_uint(r_dest, dest.u32);
            }
            break;
        //case 13: // MUL Rd, Rs
        case 14: // BIC Rd, Rs
            {
                dest.u32 &= ~src.u32;
                cpu.set_register_uint(r_dest, dest.u32);
            }
            break;
        case 15: // MVN Rd, Rs
            {
                dest.u32 = ~src.u32;
                cpu.set_register_uint(r_dest, dest.u32);
            }
            break;
        default:
            throw std::runtime_error(
                    "inst::thumb::alu_operations : unknown opcode (" +
                    std::to_string(op) + ")");
    }

    return cpu.get_register_uint(arm_PC) + 2;

}

address_t inst::thumb::hi_reg_ops_brnch_exch( arm_cpu& cpu, memory_t& mem, uint32_t inst ) {

    int op   = (inst >> 8) & 0x03;
    int h1   = (inst >> 7) & 0x01;
    int h2   = (inst >> 6) & 0x01;
    int RsHs = (inst >> 3) & 0x07;
    int RdHd = (inst >> 0) & 0x07;

    switch(op) {
        case 0: // add
            {
                if(h1) RdHd += 8;
                if(h2) RsHs += 8;

                int rd = cpu.get_register_int(RdHd);
                int rs = cpu.get_register_int(RsHs);
                rd += rs;
                cpu.set_register_int(RdHd, rd);
            }
            break;
        //case 1: // cmp

        case 2: // mov
            {
                if(h1) RdHd += 8;
                if(h2) RsHs += 8;

                int rs = cpu.get_register_int(RsHs);
                cpu.set_register_int(RdHd, rs);
            }
            break;
        case 3: // bx
            {
                if(h2) RsHs += 8;
                address_t addr = cpu.get_register_uint(RsHs);

                if(addr & 0x01) {
                    cpu.set_mode(arm_mode_THUMB);
                }
                else {
                    cpu.set_mode(arm_mode_ARM);
                }

                addr &= ~0x00000001;
                return addr;
            }
            break;
        default:
            throw std::runtime_error(
                "inst::thumb::hi_reg_ops_brnch_exch : unknown opcode (" +
                std::to_string(op) + ")");
    }

    return cpu.get_register_uint(arm_PC) + 2;
}

address_t inst::thumb::pc_relative_load( arm_cpu& cpu, memory_t& mem, uint32_t inst ) {

    int Rd = (inst >> 8) & 0x07;
    uint32_t word8 = inst & 0xFF;
    word8 <<= 2;
    word8 += cpu.get_register_uint(arm_PC);
    uint32_t val = mem.load_u32(word8);

    cpu.set_register_uint(Rd, val);

    return cpu.get_register_uint(arm_PC) + 2;
}

address_t inst::thumb::load_store_w_register_offset( arm_cpu& cpu, memory_t& mem, uint32_t inst ) {

    int L  = (inst >> 11) & 0x01; // 0: store, 1: load
    int B  = (inst >> 10) & 0x01; // 0: word,  1: byte
    int Ro = (inst >> 6)  & 0x07;
    int Rb = (inst >> 3)  & 0x07;
    int Rd = (inst >> 0)  & 0x07;

    address_t baseaddr = cpu.get_register_uint(Rb);
    address_t offset   = cpu.get_register_uint(Ro);
    baseaddr += offset;

    if(L) { // Load
        if(B) { // Byte
            uint32_t b = mem.load_u8(baseaddr);
            cpu.set_register_uint(Rd, b);
        }
        else { // Word
            uint32_t w = mem.load_u32(baseaddr);
            cpu.set_register_uint(Rd, w);
        }
    }
    else { // Store
        if(B) { // Byte
            uint32_t b = cpu.get_register_uint(Rd);
            b &= 0xFF;
            mem.store_u8(baseaddr, b);
        }
        else { // Word
            uint32_t b = cpu.get_register_uint(Rd);
            mem.store_u32(baseaddr, b);
        }
    }

    

}
