#include <inc/decode.h>
#include <stdexcept>

instruction_t::instruction_t(void) {
    this->opcode = -1;

    this->meta_opcode = -1; // some opcodes get shared

    this->PC = 0x00;
    this->i_immediate = 0x00;
    this->u_immediate = 0x00;

    this->Rs = -1;
    this->Rd = -1;
    this->Rn = -1;
    this->Rb = -1;
    this->Ro = -1;

    this->Hs = -1;
    this->Hd = -1;
    this->Rlist = -1;

}

// toplevel decode function
instruction_t decode_instruction(unsigned int PC, unsigned int instruction_word) {

    int superfamily = (instruction_word >> 13) & 0x07;

    switch(superfamily) {
        case 0:
            {
                int op = (instruction_word >> 10) & 0x03;
                if(op == 0x03)
                    return decode_format_2(PC, instruction_word);
                else
                    return decode_format_1(PC, instruction_word);
            }
            break;
        case 1:
            return decode_format_3(PC, instruction_word); break;
        case 2:
            {
                int flag = (instruction_word >> 10) & 0x07;
                if(flag == 0x00)
                    return decode_format_4(PC, instruction_word);
                else if(flag == 0x01)
                    return decode_format_5(PC, instruction_word);
            
                flag = (instruction_word >> 11) & 0x03;
                if(flag == 0x01)
                    return decode_format_6(PC, instruction_word);

                flag = (instruction_word >> 9) & 0x09;
                if(flag == 0x08)
                    return decode_format_7(PC, instruction_word);
                else if(flag == 0x09)
                    return decode_format_8(PC, instruction_word);
            
                throw std::runtime_error(
                    "decode_instruction (superfamily: " + 
                    std::to_string(superfamily) + ")");
            }
            break;
        case 3:
            return decode_format_9(PC, instruction_word);
            break;
        case 4:
            {
                int flag = (instruction_word >> 12) & 0x01;
                if(flag == 0x00) {
                    return decode_format_10(PC, instruction_word);
                }
                else {
                    return decode_format_11(PC, instruction_word);
                }
            }
            break;
        case 5:
            {
                int flag = (instruction_word >> 12) & 0x01;
                if(flag == 0x00)
                    return decode_format_12(PC, instruction_word);

                flag = (instruction_word >> 8) & 0x1F;
                if(flag == 0x10)
                    return decode_format_13(PC, instruction_word);
                
                flag = (instruction_word >> 9) & 0xB;
                if(flag == 0x0A)
                    return decode_format_14(PC, instruction_word);

                throw std::runtime_error(
                    "decode_instruction (superfamily: " + 
                    std::to_string(superfamily) + ")");
            }
            break;
        case 6:
            {
                int flag = (instruction_word >> 12) & 0x01;
                if(flag == 0) {
                    return decode_format_15(PC, instruction_word);
                }

                flag = (instruction_word >> 8) & 0x1F;
                if(flag == 0x1F)
                    return decode_format_17(PC, instruction_word);

                flag = (instruction_word >> 12) & 0x01;
                if(flag == 0x01)
                    return decode_format_16(PC, instruction_word);
            
                throw std::runtime_error(
                    "decode_instruction (superfamily: " + 
                    std::to_string(superfamily) + ")");
            }
            break;
        case 7:
            {
                int flag = (instruction_word >> 11) & 0x03;
                if(flag == 0x00)
                    return decode_format_18(PC, instruction_word);

                flag = (instruction_word >> 12) & 0x01;
                if(flag == 0x01)
                    return decode_format_19(PC, instruction_word);
            }
            break;
        default:
            throw std::runtime_error("decode_instruction : unknown internal error");
    }
}

instruction_t decode_format_1(  unsigned int PC, unsigned int instruction_word ) {

    instruction_t inst;

    inst.u_immediate = (instruction_word >> 6) & 0x1F;
    inst.Rs          = (instruction_word >> 3) & 0x07;
    inst.Rd          = (instruction_word >> 0) & 0x07;

    int op = (instruction_word >> 11) & 0x03;
    switch(op) {
        case 0:
            inst.opcode = i_LSL; 
            inst.meta_opcode = meta_RRC;
            break;
        case 1:
            inst.opcode = i_LSR; 
            inst.meta_opcode = meta_RRC;
            break;
        case 2:
            inst.opcode = i_ASR; 
            inst.meta_opcode = meta_RRC;
            break;
        default:
            throw std::runtime_error("decode_format_1 : OP field is invalid");
    }

    return inst;
}

instruction_t decode_format_2(  unsigned int PC, unsigned int instruction_word ) {

    instruction_t inst;

    inst.Rs          = (instruction_word >> 3) & 0x07;
    inst.Rd          = (instruction_word >> 0) & 0x07;

    int Op = (instruction_word >> 9)  & 0x01;
    int I  = (instruction_word >> 10) & 0x01;

    // these two bits are important
    int OpI = (I & (Op << 1));

    switch(OpI) {
        case 0:
            inst.opcode = i_ADD;
            inst.meta_opcode = meta_RRR;
            inst.Rn = (instruction_word >> 6) & 0x07;
            break;
        case 1:
            inst.opcode = i_ADD;
            inst.meta_opcode = meta_RRC;
            inst.u_immediate = (instruction_word >> 6) & 0x07;
            break;
        case 2:
            inst.opcode = i_SUB;
            inst.meta_opcode = meta_RRR;
            inst.Rn = (instruction_word >> 6) & 0x07;
            break;
        case 3:
            inst.opcode = i_SUB;
            inst.meta_opcode = meta_RRC;
            inst.u_immediate = (instruction_word >> 6) & 0x07;
            break;
        default:
            throw std::runtime_error("decode_format_2 : unknown internal error with Op field or I field");
    }

    return inst;
}

instruction_t decode_format_3(  unsigned int PC, unsigned int instruction_word ) {

    instruction_t inst;

    inst.Rd = (instruction_word >> 8) & 0x07;
    inst.u_immediate = instruction_word & 0xFF;

    int Op = (instruction_word >> 11) & 0x03;

    switch(Op) {
        case 0:
            inst.opcode = i_MOV;
            inst.meta_opcode = meta_RC;
            break;
        case 1:
            inst.opcode = i_CMP;
            inst.meta_opcode = meta_RC;
            break;
        case 2:
            inst.opcode = i_ADD;
            inst.meta_opcode = meta_RC;
            break;
        case 3:
            inst.opcode = i_SUB;
            inst.meta_opcode = meta_RC;
            break;
        default:
            throw std::runtime_error("decode_format_3 : Op field is invalid");
    }

    return inst;
}

instruction_t decode_format_4(  unsigned int PC, unsigned int instruction_word ) {

    instruction_t inst;

    inst.Rs          = (instruction_word >> 3) & 0x07;
    inst.Rd          = (instruction_word >> 0) & 0x07;

    int Op = (instruction_word >> 6) & 0x0F;

    switch(Op) {
        
    }

    return inst;
}

instruction_t decode_format_5(  unsigned int PC, unsigned int instruction_word ) {

}

instruction_t decode_format_6(  unsigned int PC, unsigned int instruction_word ) {

}

instruction_t decode_format_7(  unsigned int PC, unsigned int instruction_word ) {

}

instruction_t decode_format_8(  unsigned int PC, unsigned int instruction_word ) {

}

instruction_t decode_format_9(  unsigned int PC, unsigned int instruction_word ) {

}

instruction_t decode_format_10( unsigned int PC, unsigned int instruction_word ) {

}

instruction_t decode_format_11( unsigned int PC, unsigned int instruction_word ) {

}

instruction_t decode_format_12( unsigned int PC, unsigned int instruction_word ) {

}

instruction_t decode_format_13( unsigned int PC, unsigned int instruction_word ) {

}

instruction_t decode_format_14( unsigned int PC, unsigned int instruction_word ) {

}

instruction_t decode_format_15( unsigned int PC, unsigned int instruction_word ) {

}

instruction_t decode_format_16( unsigned int PC, unsigned int instruction_word ) {

}

instruction_t decode_format_17( unsigned int PC, unsigned int instruction_word ) {

}

instruction_t decode_format_18( unsigned int PC, unsigned int instruction_word ) {

}

instruction_t decode_format_19( unsigned int PC, unsigned int instruction_word ) {

}


