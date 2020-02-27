#include <inc/decode.h>
#include <stdexcept>
#include <iostream>

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

    //this->Hs = -1;
    //this->Hd = -1;
    this->Rlist = -1;
}

std::ostream& operator<<(std::ostream& os, instruction_t& in) {

    switch(in.opcode) {

        case i_ADC   : // add with carry
            std::cout << "ADC " << in.Rd << ", " << in.Rs << '\n'; // 
            break;
        case i_ADD   : // add
            std::cout << "ADD ";
            //ADD   = 2*(RRR, RRC), 3(RC), 5*(RR), 12*(RC_pc, RC_sp), 13*(C_sp)
            break;        
                    
        case i_AND   : // bitwise AND
        case i_ASR   : // arithmetic shift right
        case i_B     : // unconditional branch
        case i_Bxx   : // conditional branch
        case i_BIC   : // bit clear
        case i_BL    : // branch and link
        case i_BX    : // branch and exchange
        case i_CMN   : // compare negative
        case i_CMP   : // compare
        case i_EOR   : // bitwise XOR
        case i_LDMIA : // load multiple
        case i_LDR   : // load word
        case i_LDRB  : // load byte
        case i_LDRH  : // load halfword
        case i_LSL   : // logical shift left
        case i_LDSB  : // load sign-extended byte
        case i_LDSH  : // load sign-extended halfword
        case i_LSR   : // logical shift right
        case i_MOV   : // move register
        case i_MUL   : // multiply
        case i_MVN   : // move negative register
        case i_NEG   : // negate
        case i_ORR   : // bitwise OR
        case i_POP   : // pop registers
        case i_PUSH  : // push registers
            break;
        
        case i_ROR   : // rotate right
            //ROR   = 4
            std::cout << "ROR Rd:" << in.Rd << ", Rs:" << in.Rs;
            break;
        
        case i_SBC   : // subtract with carry
            //SBC   = 4
            std::cout << "SBC Rd:" << in.Rd << ", Rs:" << in.Rs;
            break;
        
        case i_STMIA : // store multiple
            //STMIA = 15
            std::cout << "STMIA Rb:" << in.Rb << ", Rlist:" << in.Rlist;
            break;

        case i_STR   : // store word
            //STR   = 7, 9, 11 
            std::cout << "STR ";
            switch(in.meta_opcode) {
                case meta_RRR:   std::cout << 'r' << in.Rd << ", [r" << in.Rb << ", r" << in.Ro << ']';          break;
                case meta_RRC:   std::cout << 'r' << in.Rb << ", [r" << in.Rb << ", #" << in.u_immediate << ']'; break;
                case meta_RC_sp: std::cout << 'r' << in.Rd << ", [SP, #" << in.u_immediate << ']';               break;
                default: 
                    std::runtime_error("opcode(STR) : invalid metaopcode");
            }
            break;

        case i_STRB  : // store byte
            //STRB  = 7, 9
            std::cout << "STRB ";
            switch(in.meta_opcode) {
                case meta_RRR: std::cout << 'r' << in.Rd << ", [r" << in.Rb << ", r" << in.Ro << ']';          break;
                case meta_RRC: std::cout << 'r' << in.Rb << ", [r" << in.Rb << ", #" << in.u_immediate << ']'; break;
                default:
                    std::runtime_error(" STRB: Invalid meta opcode");                
            }
            break;

        case i_STRH  : // store halfword
            //STRH  = 8, 10
            std::cout << "STRH ";
            switch (in.meta_opcode) {
                case meta_RRR: std::cout << 'r' << in.Rd <<", [r" << in.Rb << ", r" << in.Ro << ']';           break;
                case meta_RRC: std::cout << 'r' << in.Rd << ", [r" << in.Rb << ", #" << in.u_immediate << ']'; break;
                default:
                    std::runtime_error(" STRH: invalid meta_opcode");
            }
            break;

        case i_SWI   : // software interrupt
            //SWI   = 17
            std::cout << "SWI #" << in.u_immediate;
            break;

        case i_SUB   : // subtract
            //SUB   = 2*(RRR, RRC), 3(RC)
            std::cout << "SUB " ;
            switch (in.meta_opcode) {
                case meta_RRR: std::cout << 'r' << in.Rd << ", r" << in.Rs << ", r" << in.Rn;          break;
                case meta_RRC: std::cout << 'r' << in.Rd << ", r" << in.Rs << ",  #"<< in.u_immediate; break;   
                case meta_RC:  std::cout << 'r' << in.Rd << ", #"<< in.u_immediate;                    break;     
                default:
                    std::runtime_error("SUB: invalid meta_opcode");
            }
            break;

        case i_TST   : // test bits
            // TST   = 4
            std::cout << "TST r" << in.Rs << ", r" << in.Rd;
            break;

        default:
            throw std::runtime_error("instruction_t : invalid opcode");

    }

}

// top-level decode function
instruction_t decode_instruction(unsigned int PC, unsigned int instruction_word) {

    int superfamily = (instruction_word >> 13) & 0x07;

    switch(superfamily) {
        case 0:
            {
                int op = (instruction_word >> 11) & 0x03;
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

    inst.Rd          = (instruction_word >> 0) & 0x07;
    inst.Rs          = (instruction_word >> 3) & 0x07;

    int Op = (instruction_word >> 9)  & 0x01;
    int I  = (instruction_word >> 10) & 0x01;

    // these two bits are important
    int OpI = (I | (Op << 1));

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

    inst.Rd          = (instruction_word >> 8) & 0x07;
    inst.u_immediate = (instruction_word >> 0) & 0xFF;
    inst.meta_opcode = meta_RC;

    int Op = (instruction_word >> 11) & 0x03;

    switch(Op) {
        case 0:
            inst.opcode = i_MOV;
            break;
        case 1:
            inst.opcode = i_CMP;
            break;
        case 2:
            inst.opcode = i_ADD;
            break;
        case 3:
            inst.opcode = i_SUB;
            break;
        default:
            throw std::runtime_error("decode_format_3 : Op field is invalid");
    }

    return inst;
}

instruction_t decode_format_4(  unsigned int PC, unsigned int instruction_word ) {

    instruction_t inst;

    inst.Rd          = (instruction_word >> 0) & 0x07;
    inst.Rs          = (instruction_word >> 3) & 0x07;
    inst.meta_opcode = meta_RR;

    int Op = (instruction_word >> 6) & 0x0F;

    switch(Op) {
        case 0:  inst.opcode = i_AND; break;
        case 1:  inst.opcode = i_EOR; break;
        case 2:  inst.opcode = i_LSL; break;
        case 3:  inst.opcode = i_LSR; break;
        case 4:  inst.opcode = i_ASR; break;
        case 5:  inst.opcode = i_ADC; break;
        case 6:  inst.opcode = i_SBC; break;
        case 7:  inst.opcode = i_ROR; break;
        case 8:  inst.opcode = i_TST; break;
        case 9:  inst.opcode = i_NEG; break;
        case 10: inst.opcode = i_CMP; break;
        case 11: inst.opcode = i_CMN; break;
        case 12: inst.opcode = i_ORR; break;
        case 13: inst.opcode = i_MUL; break;
        case 14: inst.opcode = i_BIC; break;
        case 15: inst.opcode = i_MVN; break;
        default:
            throw std::runtime_error("decode_format_4 : Op field is invalid");
    }

    return inst;
}

instruction_t decode_format_5(  unsigned int PC, unsigned int instruction_word ) {

    instruction_t inst;

    inst.Rd          = (instruction_word >> 0) & 0x07;
    inst.Rs          = (instruction_word >> 3) & 0x07;
    inst.meta_opcode = meta_RR;

    int Op = (instruction_word >> 8) & 0x03;

    switch(Op) {
        case 0: inst.opcode = i_ADD; break;
        case 1: inst.opcode = i_CMP; break;
        case 2: inst.opcode = i_MOV; break;
        case 3: 
            inst.opcode = i_BX;
            inst.meta_opcode = meta_R;
            break;
        default:
            throw std::runtime_error("decode_format_5 : Op field is invalid");
    }

    int H1 = (instruction_word >> 7) & 0x01;
    int H2 = (instruction_word >> 6) & 0x01;

    if(H1) inst.Rd += 8;
    if(H2) inst.Rs += 8;

    return inst;
}

instruction_t decode_format_6(  unsigned int PC, unsigned int instruction_word ) {

    instruction_t inst;

    inst.Rd          = (instruction_word >> 8) & 0x07;
    inst.u_immediate = (instruction_word >> 0) & 0xFF;
    inst.u_immediate <<= 2;

    inst.opcode      = i_LDR;
    inst.meta_opcode = meta_RC_pc;

    return inst;
}

instruction_t decode_format_7(  unsigned int PC, unsigned int instruction_word ) {

    instruction_t inst;

    inst.Rd = (instruction_word >> 0) & 0x07;
    inst.Rb = (instruction_word >> 3) & 0x07;
    inst.Ro = (instruction_word >> 6) & 0x07;

    int LB = (instruction_word >> 10) & 0x03;

    inst.meta_opcode = meta_RRR;

    switch(LB) {
        case 0: inst.opcode = i_STR;  break;
        case 1: inst.opcode = i_STRB; break;
        case 2: inst.opcode = i_LDR;  break;
        case 3: inst.opcode = i_LDRB; break;
        default:
            throw std::runtime_error("decode_format_7 : LB field invalid");
    }

    return inst;
}

instruction_t decode_format_8(  unsigned int PC, unsigned int instruction_word ) {

    instruction_t inst;

    inst.Rd = (instruction_word >> 0) & 0x07;
    inst.Rb = (instruction_word >> 3) & 0x07;
    inst.Ro = (instruction_word >> 6) & 0x07;

    inst.meta_opcode = meta_RRR;

    int H = (instruction_word >> 11) & 0x01;
    int S = (instruction_word >> 10) & 0x01;

    int SH = (H | (S << 1));

    switch (SH){
        case 0: inst.opcode = i_STRH; break;
        case 1: inst.opcode = i_LDRH; break;
        case 2: inst.opcode = i_LDSB; break;
        case 3: inst.opcode = i_LDSH; break;
        default:
            throw std::runtime_error("Decode format_8 : SH field invalid");
    }

    return inst;

}

instruction_t decode_format_9(  unsigned int PC, unsigned int instruction_word ) {

    instruction_t inst;

    inst.Rd = (instruction_word >> 0) & 0x07;
    inst.Rb = (instruction_word >> 3) & 0x07;
    inst.u_immediate = (instruction_word >> 6) & 0x1F;

    inst.meta_opcode = meta_RC;

    int L = (instruction_word >> 11) & 0x01;
    int B = (instruction_word >> 12) & 0x01;

    int LB = (B | (L << 1));

    switch (LB)
    {
        case 0:
            inst.opcode = i_STR;  break;
        case 1:
            inst.opcode = i_STRB; break;
        case 2: 
            inst.opcode = i_LDR;  break;
        case 3:
            inst.opcode = i_LDRB; break;
        default:
            throw std::runtime_error("Decode format_9 : LB field invalid");
    } 

    return inst;
}

instruction_t decode_format_10( unsigned int PC, unsigned int instruction_word ) {

    instruction_t inst;

    inst.Rd = (instruction_word >> 0) & 0x07;
    inst.Rb = (instruction_word >> 3) & 0x07;
    inst.u_immediate = (instruction_word >> 6) & 0x1F;

    inst.meta_opcode = meta_RRC;
    
    int L = (instruction_word >> 11) & 0x01;

    if(L == 0) {
        inst.opcode = i_STRH;
    }
    else {
        inst.opcode = i_LDRH;
    }

    return inst;
}

instruction_t decode_format_11( unsigned int PC, unsigned int instruction_word ) {

    instruction_t inst;

    inst.u_immediate = (instruction_word >> 0) & 0x0FF;
    inst.Rd          = (instruction_word >> 8) & 0x07;
    inst.meta_opcode = meta_RC_sp;

    int L = (instruction_word >> 11) & 0x01;

    if ( L == 0 ) {
        inst.opcode = i_STR;
    }
    else{
        inst.opcode = i_LDR;
    }

    return inst;
}

instruction_t decode_format_12( unsigned int PC, unsigned int instruction_word ) {

    instruction_t inst;

    inst.u_immediate = (instruction_word >> 0) & 0xFF;
    inst.Rd          = (instruction_word >> 8) & 0x07;

    int SP = (instruction_word >> 11) & 0x01;

    inst.opcode = i_ADD; 

    if (SP == 0 ){
        inst.meta_opcode = meta_RC_pc;
    }
    else{
        inst.meta_opcode = meta_RC_sp;
    }
    
    return inst;
}

instruction_t decode_format_13( unsigned int PC, unsigned int instruction_word ) {

    instruction_t inst;

    inst.u_immediate = (instruction_word >>  0) & 0x7F;
    inst.u_immediate <<= 2;

    inst.meta_opcode = meta_C_sp; 
    inst.opcode      = i_ADD;
    
    int S = (instruction_word >> 7) & 0x01;

    if (S == 1)
        inst.i_immediate *= -1;

    return inst;
}

instruction_t decode_format_14( unsigned int PC, unsigned int instruction_word ) {
    
    instruction_t inst;

    inst.Rlist = (instruction_word >> 0) & 0xFF;

    int R = (instruction_word >> 8) & 0x01;
    int L = (instruction_word >> 11) & 0x01;

    int LR = (R | (L << 1));

    switch (LR) {
        case 0:
            inst.opcode = i_PUSH;
            inst.meta_opcode = meta_C;
            break;
        case 1:
            inst.opcode = i_PUSH;
            inst.meta_opcode = meta_C_lr;
            break;
        case 2: 
            inst.opcode = i_POP;
            inst.meta_opcode = meta_C;
            break;
        case 3:
            inst.opcode = i_POP;
            inst.meta_opcode = meta_C_pc;
            break;
        default:
        throw std::runtime_error("Decode format_14 : LR field invalid");
    }

    return inst;
}

instruction_t decode_format_15( unsigned int PC, unsigned int instruction_word ) {
    
    instruction_t inst;

    inst.Rlist = (instruction_word >> 0) & 0xFF;
    inst.Rb    = (instruction_word >> 8) & 0x07;
    int L = (instruction_word >> 11) & 0x01;

    if(L == 0) {
        inst.opcode = i_STMIA;
    }
    else {
        inst.opcode = i_LDMIA;
    }

    return inst;
}

instruction_t decode_format_16( unsigned int PC, unsigned int instruction_word ) {

    instruction_t inst;

    inst.u_immediate    = (instruction_word >> 0) & 0xFF;
    
    // sign extend 8-bit immediate
    if((inst.u_immediate >> 7) & 0x01)
        inst.u_immediate |= 0xFFFFFF00;

    inst.opcode         = i_Bxx;
    inst.condition_code = (instruction_word >> 8) & 0x0F;

    return inst;
}

instruction_t decode_format_17( unsigned int PC, unsigned int instruction_word ) {

    instruction_t inst;

    inst.u_immediate = (instruction_word >> 0) & 0xFF;
    inst.opcode      = i_SWI;

    return inst;
}

instruction_t decode_format_18( unsigned int PC, unsigned int instruction_word ) {

    instruction_t inst;

    // immediate is an 11 bit field
    inst.u_immediate = (instruction_word >> 0) & 0x7FF;
    inst.opcode = i_B;

    // shift left to get a 12 bit number
    inst.u_immediate <<= 1;

    // sign extend the 12-bit number to 32-bits
    if((inst.u_immediate >> 11) & 0x01)
        inst.u_immediate |= 0xFFFFF000; 

    return inst;
}

instruction_t decode_format_19( unsigned int PC, unsigned int instruction_word ) {

    instruction_t inst;

    inst.u_immediate = (instruction_word >> 0) & 0x7FF;
    inst.opcode      = i_BL;

    int H = (instruction_word >> 11) & 0x01;
  
    if(H == 0)
        inst.u_immediate <<= 12; // shift left 12 bits
    else
        inst.u_immediate <<= 1;  // shift left 1 bit
  
    return inst;
}


