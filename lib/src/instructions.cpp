#include <iostream>
#include <instructions.h>
#include <arm_instruction_asm.h>

address_t inst::execute_instruction( arm_cpu& cpu, memory_t& mem ) {
    const int cpumode = cpu.get_mode();

    switch(cpumode) {
        case arm_mode_THUMB:
            {
                uint16_t instruction = mem.load_u16(cpu.get_register_uint(arm_PC));
                return inst::thumb::decode(cpu, mem, instruction);
            }
            break;
        case arm_mode_ARM:
        default:
            throw std::runtime_error(
                    "inst::execute_instruction : unknown CPU mode: " +
                    std::to_string(cpumode));
    }
}

// decode ARM thumb instruction
address_t inst::thumb::decode( arm_cpu& cpu, memory_t& mem, uint32_t inst ) {
    const int op_field = (inst >> 13) & 0x07;

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
        case 2:
            {
                // take the next three bits
                int sop = (inst >> 10) & 0x07;
                if(sop == 0)
                    return inst::thumb::alu_operations(cpu, mem, inst);
                else if(sop == 1)
                    return inst::thumb::hi_reg_ops_brnch_exch(cpu, mem, inst);

                sop = (inst >> 11) & 0x03;
                if(sop == 1)
                    return inst::thumb::pc_relative_load(cpu, mem, inst);

                sop = (inst >> 9) & 0x09;
                if(sop == 8)
                    return inst::thumb::load_store_w_register_offset(cpu, mem, inst);
                else if(sop == 9)
                    return inst::thumb::load_store_s_ext_byte_halfword(cpu, mem, inst);

                // nothing has happened by now, this is an invalid instruction
                throw std::runtime_error(
                        "inst::thumb::decode : invalid instruction detected at PC value: " +
                        std::to_string(cpu.get_register_uint(arm_PC)));
            }
            break;
        case 3:
            {
                return inst::thumb::load_store_w_imm_offset(cpu, mem, inst);
            }
            break;
        case 4:
            {
                int f = (inst >> 12) & 0x01;
                if(f == 0)
                    return inst::thumb::load_store_halfword(cpu, mem, inst);
                else
                    return inst::thumb::sp_relative_load_store(cpu, mem, inst);
            }
            break;
        case 5:
            {
                int f = (inst >> 12) & 0x01;
                if(f == 0)
                    return inst::thumb::load_address(cpu, mem, inst);

                f = (inst >> 8) & 0x1F;
                if(f == 0x10)
                    return inst::thumb::add_offset_sp(cpu, mem, inst);

                f = (inst >> 9) & 0x0F;
                if((f & 0xB) == 0xA)
                    return inst::thumb::push_pop_regs(cpu, mem, inst);

                throw std::runtime_error(
                        "inst::thumb::decode : invalid instruction detected at PC value: " +
                        std::to_string(cpu.get_register_uint(arm_PC)));
            }
            break;
        case 6:
        case 7:
        default:
            throw std::runtime_error("inst::thumb::decode : unknown operation");
    }

}

// MOVE SHIFTED REGISTER
address_t inst::thumb::move_shifted_register( arm_cpu& cpu, memory_t& mem, uint32_t inst ) {

    const int r_dest = inst & 0x07;         // b111
    const int r_src  = (inst >> 3) & 0x07;  // b111
    const int offset = (inst >> 6) & 0x1F;  // b11111
    const int op     = (inst >> 11) & 0x03; // b11

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
address_t inst::thumb::add_subtract( arm_cpu& cpu, memory_t& mem, uint32_t inst ) {

    const int r_dest   = inst & 0x07;
    const int r_src    = (inst >> 3)  & 0x07;
    const int rn_off   = (inst >> 6)  & 0x07;
    const int op       = (inst >> 9)  & 0x01;
    const int imm_flag = (inst >> 10) & 0x01;

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
address_t inst::thumb::mcas_imm( arm_cpu& cpu, memory_t& mem, uint32_t inst ) {

    const int offset = inst & 0xFF;
    const int r_dest = (inst >> 8)  & 0x07;
    const int op     = (inst >> 11) & 0x03;

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

address_t inst::thumb::alu_operations( arm_cpu& cpu, memory_t& mem, uint32_t inst ) {

    const int r_dest = inst & 0x07;
    const int r_src  = (inst >> 3) & 0x07;
    const int op     = (inst >> 6) & 0x0F;

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

    const int op   = (inst >> 8) & 0x03;
    const int h1   = (inst >> 7) & 0x01;
    const int h2   = (inst >> 6) & 0x01;
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

    const int Rd         = (inst >> 8) & 0x07;
    const uint32_t word8 = (inst >> 0) & 0xFF;

    address_t baseaddr = cpu.get_register_uint(arm_PC);
    baseaddr += (word8 << 2);
    uint32_t val = mem.load_u32(baseaddr);

    cpu.set_register_uint(Rd, val);
    if(Rd == arm_PC)
        return cpu.get_register_uint(arm_PC);
    else
        return cpu.get_register_uint(arm_PC) + 2;
}

address_t inst::thumb::load_store_w_register_offset( arm_cpu& cpu, memory_t& mem, uint32_t inst ) {

    const int L  = (inst >> 11) & 0x01; // 0: store, 1: load
    const int B  = (inst >> 10) & 0x01; // 0: word,  1: byte
    const int Ro = (inst >> 6)  & 0x07;
    const int Rb = (inst >> 3)  & 0x07;
    const int Rd = (inst >> 0)  & 0x07;

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

        // its possible that a load could override
        // the existing instruction pointer value
        if(Rd == arm_PC)
            return cpu.get_register_uint(arm_PC);

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

    return cpu.get_register_uint(arm_PC) + 2;

}

address_t inst::thumb::load_store_s_ext_byte_halfword( arm_cpu& cpu, memory_t& mem, uint32_t inst ) {

    const int H  = (inst >> 11) & 0x01;
    const int S  = (inst >> 10) & 0x01;
    const int Ro = (inst >> 6) & 0x07;
    const int Rb = (inst >> 3) & 0x07;
    const int Rd = (inst >> 0) & 0x07;

    address_t baseaddr = cpu.get_register_uint(Rb);
    address_t offset   = cpu.get_register_uint(Ro);
    baseaddr += offset;

    if(S == 0) {
        if(H == 0) { // store halfword

            uint32_t val = cpu.get_register_uint(Rd);
            mem.store_u16(baseaddr, val & 0x0000FFFF);

        }
        else { // load halfword

            uint32_t val = 0;
            val += mem.load_u16(baseaddr);
            cpu.set_register_uint(Rd, val);

            if(Rd == arm_PC)
                return cpu.get_register_uint(arm_PC);

        }
    }
    else {
        if(H == 0) { // load sign-ext byte

            uint32_t b = mem.load_u8(baseaddr);
            if(b & 0x80)
                b |= 0xFFFFFF00;

            cpu.set_register_uint(Rd, b);
            if(Rd == arm_PC)
                return cpu.get_register_uint(arm_PC);

        }
        else { // load sign-ext halfword

            uint32_t hw = mem.load_u16(baseaddr);
            if(hw & 0x8000)
                hw |= 0xFFFF0000;

            cpu.set_register_uint(Rd, hw);
            if(Rd == arm_PC)
                return cpu.get_register_uint(arm_PC);

        }
    }

    return cpu.get_register_uint(arm_PC) + 2;

}

address_t inst::thumb::load_store_w_imm_offset( arm_cpu& cpu, memory_t& mem, uint32_t inst ) {

    const int B            = (inst >> 12) & 0x01;
    const int L            = (inst >> 11) & 0x01;
    const uint32_t offset5 = (inst >> 6) & 0x1F;
    const int Rb           = (inst >> 3) & 0x07;
    const int Rd           = (inst >> 0) & 0x07;

    address_t baseaddr = cpu.get_register_uint(Rb);
    baseaddr += offset5;

    if(B == 0) {
        if(L == 0) { // store word

            uint32_t val = cpu.get_register_uint(Rd);
            mem.store_u32(baseaddr, val);

        }
        else { // load word

            uint32_t val = mem.load_u32(baseaddr);
            cpu.set_register_uint(Rd, val);

            if(Rd == arm_PC)
                return cpu.get_register_uint(arm_PC);

        }
    }
    else {
        if(L == 0) { // store byte

            uint32_t val = cpu.get_register_uint(Rd);
            mem.store_u8(baseaddr, val & 0x000000FF);

        }
        else { // load byte

            uint32_t val = 0;
            val += mem.load_u8(baseaddr);
            cpu.set_register_uint(Rd, val);

            if(Rd == arm_PC)
                return cpu.get_register_uint(arm_PC);

        }
    }

    return cpu.get_register_uint(arm_PC) + 2;
}

address_t inst::thumb::load_store_halfword( arm_cpu& cpu, memory_t& mem, uint32_t inst ) {

    const int L       = (inst >> 11) & 0x01;
    const int offset5 = (inst >> 6) & 0x1F;
    const int Rb      = (inst >> 3) & 0x07;
    const int Rd      = (inst >> 0) & 0x07;

    address_t baseaddr = cpu.get_register_uint(Rb);
    baseaddr += (offset5 << 1);

    if(L == 0) { // store halfword

        uint32_t val = cpu.get_register_uint(Rd);
        mem.store_u16(baseaddr, val & 0x0000FFFF);

    }
    else { // load halfword

        uint32_t val = 0;
        val += mem.load_u16(baseaddr);

        cpu.set_register_uint(Rd, val);
        if(Rd == arm_PC)
            return cpu.get_register_uint(arm_PC);

    }

    return cpu.get_register_uint(arm_PC) + 2;
}

address_t inst::thumb::sp_relative_load_store( arm_cpu& cpu, memory_t& mem, uint32_t inst ) {

    const int L          = (inst >> 11) & 0x01;
    const int Rd         = (inst >> 8) & 0x07;
    const uint32_t word8 = (inst >> 0) & 0xFF;

    address_t baseaddr = cpu.get_register_uint(arm_SP);
    baseaddr += (word8 << 2);

    if(L == 0) { // store

        uint32_t val = cpu.get_register_uint(Rd);
        mem.store_u32(baseaddr, val);

    }
    else { // load

        uint32_t val = mem.load_u32(baseaddr);
        cpu.set_register_uint(Rd, val);

        if(Rd == arm_PC)
            return cpu.get_register_uint(arm_PC);

    }

    return cpu.get_register_uint(arm_PC) + 2;

}

address_t inst::thumb::load_address( arm_cpu& cpu, memory_t& mem, uint32_t inst ) {

    const int sp    = (inst >> 11) & 0x01;
    const int Rd    = (inst >> 8) & 0x07;
    const int word8 = (inst >> 0) & 0xFF;

    if(sp == 0) { // load addr PC

        address_t addr = cpu.get_register_uint(arm_PC);
        addr += (word8 << 2);
        uint32_t val = mem.load_u32(addr);
        cpu.set_register_uint(Rd, val);

    }
    else { // load addr SP

        address_t addr = cpu.get_register_uint(arm_SP);
        addr += (word8 << 2);
        uint32_t val = mem.load_u32(addr);
        cpu.set_register_uint(Rd, val);

    }

    if(Rd == arm_PC)
        return cpu.get_register_uint(arm_PC);
    else
        return cpu.get_register_uint(arm_PC) + 2;
}

address_t inst::thumb::add_offset_sp( arm_cpu& cpu, memory_t& mem, uint32_t inst ) {

    const int S          = (inst >> 7) & 0x01;
    const uint32_t word7 = (inst >> 0) & 0x7F;

    int stack_value = cpu.get_register_int(arm_SP);

    if(S) // word7 is negative
        stack_value -= (word7 << 2);
    else // word7 is positive
        stack_value += (word7 << 2);

    cpu.set_register_int(arm_SP, stack_value);

    return cpu.get_register_uint(arm_PC) + 2;
}

address_t inst::thumb::push_pop_regs( arm_cpu& cpu, memory_t& mem, uint32_t inst ) {

    const int L     = (inst >> 11) & 0x01;
    const int R     = (inst >> 8) & 0x01;
    const int rList = (inst >> 0) & 0xFF;

    if(L == 0) { // store

        address_t sp = cpu.get_register_uint(arm_SP);

        if(R == 1) {
            sp -= 4;
            uint32_t reg_val = cpu.get_register_uint(arm_LR);
            mem.store_u32(sp, reg_val);
        }

        for(int i : { 0, 1, 2, 3, 4, 5, 6, 7 }) {
            if(rList & (1 << i)) {
                sp -= 4;
                uint32_t reg_val = cpu.get_register_uint(i);
                mem.store_u32(i, reg_val);
            }
        }

        cpu.set_register_uint(arm_SP, sp);

    }
    else { // load

        address_t sp = cpu.get_register_uint(arm_SP);

        for(int i : { 7, 6, 5, 4, 3, 2, 1, 0 }) {
            if(rList & (1 << i)) {
                uint32_t mem_val = mem.load_u32(sp);
                cpu.set_register_uint(i, mem_val);
                sp += 4;
            }
        }

        if(R == 1) {
            uint32_t mem_val = mem.load_u32(sp);
            cpu.set_register_uint(arm_PC, mem_val);
            sp += 4;
        }

        cpu.set_register_uint(arm_SP, sp);

        if(R == 1)
            return cpu.get_register_uint(arm_PC);
    }

    return cpu.get_register_uint(arm_PC) + 2;
}

address_t inst::thumb::multiple_load_store( arm_cpu& cpu, memory_t& mem, uint32_t inst ) {

    const int L     = (inst >> 11) & 0x01;
    const int Rb    = (inst >> 8) & 0x07;
    const int rList = (inst >> 0) & 0xFF;

    address_t baseaddr = cpu.get_register_uint(Rb);

    if(L == 0) { // store

        for(int i : { 0, 1, 2, 3, 4, 5, 6, 7 }) {
            if(rList & (1 << i)) {
                baseaddr -= 4;
                uint32_t regval = cpu.get_register_uint(i);
                mem.store_u32(i, regval);
            }
        }

    }
    else { // load

        for(int i : { 7, 6, 5, 4, 3, 2, 1, 0 }) {
            if(rList & (1 << i)) {
                uint32_t memval = mem.load_u32(baseaddr);
                cpu.set_register_uint(i, memval);
                baseaddr += 4;
            }
        }

    }

    cpu.set_register_uint(Rb, baseaddr);

    return cpu.get_register_uint(arm_PC) + 2;
}

address_t inst::thumb::conditional_branch( arm_cpu& cpu, memory_t& mem, uint32_t inst ) {

    const int cond     = (inst >> 8) & 0x0F;
    const char offset8 = (inst >> 0) & 0xFF;

    bool branch_taken = false;

    switch(cond) {
        case 0:  // beq
            if(cpu.get_flag_zero())
                branch_taken = true;
            break;
        case 1:  // bne
            if(!cpu.get_flag_zero())
                branch_taken = true;
            break;
        case 2:  // bcs
            if(cpu.get_flag_carry())
                branch_taken = true;
            break;
        case 3:  // bcc
            if(!cpu.get_flag_carry())
                branch_taken = true;
            break;
        case 4:  // bmi
            if(cpu.get_flag_negative())
                branch_taken = true;
            break;
        case 5:  // bpl
            if(!cpu.get_flag_negative())
                branch_taken = true;
            break;
        case 6:  // bvs
            if(cpu.get_flag_overflow())
                branch_taken = true;
            break;
        case 7:  // bvc
            if(!cpu.get_flag_overflow())
                branch_taken = true;
            break;
        case 8:  // bhi
            if(cpu.get_flag_carry() && !cpu.get_flag_zero())
                branch_taken = true;
            break;
        case 9:  // bls
            if(!cpu.get_flag_carry() || cpu.get_flag_zero())
                branch_taken = true;
            break;
        case 10: // bge
            if(cpu.get_flag_negative() == cpu.get_flag_overflow())
                branch_taken = true;
            break;
        case 11: // blt
            if(cpu.get_flag_negative() != cpu.get_flag_overflow())
                branch_taken = true;
            break;
        case 12: // bgt
            if(!cpu.get_flag_zero() && (cpu.get_flag_negative() == cpu.get_flag_overflow()))
                branch_taken = true;
            break;
        case 13: // ble
            if(cpu.get_flag_zero() && (cpu.get_flag_negative() != cpu.get_flag_overflow()))
                branch_taken = true;
            break;
        default:
            throw std::runtime_error(
                "inst::thumb::conditional_branch : unknown condition (" +
                std::to_string(cond) + ")");
    }

    if(branch_taken)
        return cpu.get_register_uint(arm_PC) + (int(offset8) << 1);
    else
        return cpu.get_register_uint(arm_PC) + 2;
}

address_t software_interrupt( arm_cpu& cpu, memory_t& mem, uint32_t inst ) {



}
