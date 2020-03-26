#include <inc/decode_32.h>
#include <string.h>

static int asBin(const char* bstr) {
    int r = 0;
    const int len = strlen(bstr);

    for(int i = 0; i < len; i++) {
        r <<= 1;
        r |= (bstr[i] == '0' ? 0 : 1);
    }

    return r;
}

//Top level decoding
instruction_32b_t decode_32bit_instruction(unsigned int PC, unsigned int instruction_word) {

    int op1 = (instruction_word >> (11 + 16)) & 0x03;
    int op2 = (instruction_word >> ( 4 + 16)) & 0x7F;
    int op  = (instruction_word >> 15)        & 0x01;

    if(op1 == 0x01) {
        int mask = asBin("1100100");
        if(op2 & mask == asBin("0000000"))
            return decode_32b_A5_20(PC, instruction_word);

        mask = asBin("1100100");
        if(op2 & mask == asBin("0000100"))
            return decode_32b_A5_21(PC, instruction_word);

        mask = asBin("1100000");
        if(op2 & mask == asBin("0100000"))
            return decode_32b_A5_26(PC, instruction_word);

        mask = asBin("1000000");
        if(op2 & mask == asBin("1000000"))
            return decode_32b_A5_31(PC, instruction_word);
    }
    else if(op1 == 0x02) {

        if(op == 0x00) {
            int mask = asBin("0100000");
            if(op2 & mask == 0x00) {
                return decode_32b_A5_14(PC, instruction_word);
            }
            else {
                return decode_32b_A5_17(PC, instruction_word);
            }
        }
        else {
            return decode_32b_A5_18(PC, instruction_word);
        }
    }
    else if(op1 == 0x03) {

        int mask = asBin("1110001");
        if(op2 & mask == 0x00)
            return decode_32b_A5_25(PC, instruction_word);

        mask = asBin("1100111");
        if(op2 & mask == asBin("0000001"))
            return decode_32b_A5_24(PC, instruction_word);
        
        if(op2 & mask == asBin("0000011"))
            return decode_32b_A5_23(PC, instruction_word);

        if(op2 & mask == asBin("0000101"))
            return decode_32b_A5_22(PC, instruction_word);
        
        if(op2 & mask == asBin("0000111"))
            throw std::runtime_error("decode_32bit_instruction : invalid op2 field");

        mask = asBin("1110000");
        if(op2 & mask == asBin("0100000"))
            return decode_32b_A5_27(PC, instruction_word);

        mask = asBin("1111000");
        if(op2 & mask == asBin("0110000"))
            return decode_32b_A5_29(PC, instruction_word);

        if(op2 & mask == asBin("0111000"))
            return decode_32b_A5_30(PC, instruction_word);

        mask = asBin("1000000");
        if(op2 & mask == asBin("1000000"))
            return decode_32b_A5_31(PC, instruction_word);

    }
    else {
        throw std::runtime_error("decode_32bit_instruction : 16-bit instruction passed as 32-bit instruction");
    }

    throw std::runtime_error("decode_32bit_instruction : invalid instruction encoding");

}




//Second level decoding
instruction_32b_t decode_32b_A5_14(unsigned int PC, unsigned int instruction_word) {

    auto& iw = instruction_word;
    instruction_32b_t in;

    int op = (iw >> (4 + 16)) & 0x1F;
    int Rn = (iw >> (0 + 16)) & 0x0F;
    int Rd = (iw >> (8 + 0)) & 0x0F;


}

instruction_32b_t decode_32b_A5_17(unsigned int PC, unsigned int instruction_word) {

}

instruction_32b_t decode_32b_A5_18(unsigned int PC, unsigned int instruction_word) {

}

instruction_32b_t decode_32b_A5_20(unsigned int PC, unsigned int instruction_word) {

    int op = (instruction_word >> (15 + 8)) & 0x03;
    int L  = (instruction_word >> (15 + 5)) & 0x01;
    int Rn = (instruction_word >> (15 + 1)) & 0x0F;

    if((op == 0x01) && (L == 0x00)){
        return decode_32b_A6_216_STM(PC, instruction_word);
    }

    if(op == 0x01){
        if(L == 0x01){
            if(Rn == 0x1101)
                return decode_32b_A6_184_POP(PC, instruction_word);
            else
                return decode_32b_A6_84_LDM(PC, instruction_word);
        }
        else
            throw std::runtime_error("In Decode_32b_A5_20: L field is invalid for op = 1");
    }

    else if(op == 0x10){
        if(L == 0x00){
            if(Rn == 0x1101)
                return decode_32b_A6_186_PUSH(PC, instruction_word);
            else
                return decode_32b_A6_218_STMDB(PC, instruction_word);
        }
        else
            throw std::runtime_error("In Decode_32b_A5_20: L field is invalid for op = 2");
    }

    else{
        throw std::runtime_error("In decode_32b_A5_20: Invalid op value");
    }

    throw std::runtime_error("In decode_32b_A5_20: invalid instruction encoding");
}

instruction_32b_t decode_32b_A5_21(unsigned int PC, unsigned int instruction_word) {

    int op1 = (instruction_word >> (15 + 8)) & 0x03;
    int op2 = (instruction_word >> (15 + 5)) & 0x03;
    int op3 = (instruction_word >> 4) & 0x0F;

    if(op1 == 0b00){
        if (op2 == 0b00)
            return decode_32b_A6_232_STREX(PC, instruction_word);
        else if(op2 == 0b01)
            return decode_32b_A6_106_LDREX(PC, instruction_word);
        else if(op2 == 0b10)
            return decode_32b_A6_230_STRD(PC, instruction_word);
        else
            return decode_32b_A6_102_LDRD_imm(PC, instruction_word);
    }

    else if(op1 == 0b01){
        if(op2 == 0b00){
            if (op3 == 0b0100)
                return decode_32b_A6_233_STREXB(PC, instruction_word);
            else if(op3 == 0b0101)
                return decode_32b_A6_234_STREXH(PC, instruction_word);
            else{
                throw std::runtime_error("In decode_32b_A5_21: Invalid op3 value for op2 = 0");
            }
            
        }

        else if(op2 == 0b01){
            if(op3 == 0b0000)
                return decode_32b_A6_256_TBB_TBH(PC, instruction_word);
            else if(op3 == 0b0001)
                return decode_32b_A6_256_TBB_TBH(PC, instruction_word);
            else if(op3 == 0b0100)
                return decode_32b_A6_107_LDREXB(PC, instruction_word);
            else if(op3 == 0b0101)
                return decode_32b_A6_108_LDREXH(PC, instruction_word);
            else{
                throw std::runtime_error("In decode_32b_A5_21: Invalid op3 value for op2 = 1");
            }
        }

        else if(op2 == 0b10){
            return decode_32b_A6_230_STRD(PC, instruction_word);
        }
        else{
            return decode_32b_A6_102_LDRD_imm(PC, instruction_word);
        }
    }

    else if((op1 == 0b10) || (op1 == 0b11)){
        if ((op2 == 0b00) || (op2 == 0b10))
            return decode_32b_A6_230_STRD(PC, instruction_word);
        if((op2 == 0b01) || (op2 == 0b11))
            return decode_32b_A6_104_LDRDL(PC, instruction_word); 
    }
    throw std::runtime_error("In decode_32b_A5_21: Invalid encoding instruction");
    
}

instruction_32b_t decode_32b_A5_22(unsigned int PC, unsigned int instruction_word) {

    int op1 = (instruction_word >> (15 + 8)) & 0x03;
    int op2 = (instruction_word >> 6) & 0x3F;
    int Rn  = (instruction_word >> (15 + 1)) & 0x0F;

    if(Rn != 0b1111){
        if(op1 == 0x01)
            return decode_32b_A6_88_LDR_imm(PC, instruction_word);

        else if(op1 == 0x00){
            int mask = 0b100100;
            int mask2 = 0b110000;
            if(((op2 & mask) == 0x100100)  || ((op2 & mask2) == 0x110000))
                return decode_32b_A6_88_LDR_imm(PC, instruction_word);

            mask = 0b111000;
            if((op2 & mask) == 0b111000)
                return decode_32b_A6_133_LDRT(PC, instruction_word);

            if (op2 == 0b00)
                return decode_32b_A6_92_LDR_reg(PC, instruction_word);
        }
    }
    else{
        int mask = 0b10;
        if((op1 & mask) == 0b00)
            return decode_32b_A6_90_LDR(PC, instruction_word);
        else
            throw std::runtime_error("In decode_32b_A5_22 : Invalid value for op1 field");
        
    }

    throw std::runtime_error("In decode_32b_A5_22 : Invalid encoding instruction");

}

instruction_32b_t decode_32b_A5_23(unsigned int PC, unsigned int instruction_word) {

}

instruction_32b_t decode_32b_A5_24(unsigned int PC, unsigned int instruction_word) {

}

instruction_32b_t decode_32b_A5_25(unsigned int PC, unsigned int instruction_word) {

}

instruction_32b_t decode_32b_A5_26(unsigned int PC, unsigned int instruction_word) {
    
}

instruction_32b_t decode_32b_A5_27(unsigned int PC, unsigned int instruction_word) {

}

instruction_32b_t decode_32b_A5_29(unsigned int PC, unsigned int instruction_word) {

}

instruction_32b_t decode_32b_A5_30(unsigned int PC, unsigned int instruction_word) {

}

instruction_32b_t decode_32b_A5_31(unsigned int PC, unsigned int instruction_word) {

}




//Third level decoding
instruction_32b_t decode_32b_A6_84_LDM(unsigned PC, unsigned int instruction_word){

    instruction_32b_t in;

    in.P = (instruction_word >> 15) & 0x01;
    in.M = (instruction_word >> 14) & 0x01;
    in.W = (instruction_word >> (15 + 6)) & 0x01;
    in.Rn = (instruction_word >> (15 + 1)) & 0x0F;
    in.Rlist = (instruction_word >> 0) & 0x1FFF;

    return in;
}

instruction_32b_t decode_32b_A6_102_LDRD_imm(unsigned PC, unsigned int instruction_word){
    
    instruction_32b_t in;

    in.P = (instruction_word >> (15 + 9)) & 0x01;
    in.U = (instruction_word >> (15 + 8)) & 0x01;
    in.W = (instruction_word >> (15 + 6)) & 0x01;
    in.Rn = (instruction_word >>(15 +1)) & 0x0F;
    in.Rt = (instruction_word >> 12) & 0x0F;
    in.Rt2 = (instruction_word >> 8) & 0x0F;
    in.u32 = (instruction_word >> 0) & 0xFF;

    return in;
}

instruction_32b_t decode_32b_A6_104_LDRDL(unsigned PC, unsigned int instruction_word){

    instruction_32b_t in;

    in.P = (instruction_word >> (15 + 9)) & 0x01;
    in.U = (instruction_word >> (15 + 8)) & 0x01;
    in.Rt = (instruction_word >> 12) & 0x0F;
    in.Rt2 = (instruction_word >> 8) & 0x0F;

    return in;
}

instruction_32b_t decode_32b_A6_106_LDREX(unsigned PC, unsigned int instruction_word){
    
    instruction_32b_t in;

    in.Rn = (instruction_word >>(15 +1)) & 0x0F;
    in.Rt = (instruction_word >> 12) & 0x0F;
    in.u32 = (instruction_word >> 0) & 0xFF;

    return in;

}

instruction_32b_t decode_32b_A6_107_LDREXB(unsigned PC, unsigned int instruction_word){

    instruction_32b_t in;

    in.Rn = (instruction_word >>(15 +1)) & 0x0F;
    in.Rt = (instruction_word >> 12) & 0x0F;

    return in;
}

instruction_32b_t decode_32b_A6_108_LDREXH(unsigned PC, unsigned int instruction_word){

    instruction_32b_t in;

    in.Rn = (instruction_word >>(15 +1)) & 0x0F;
    in.Rt = (instruction_word >> 12) & 0x0F;

    return in;
}

instruction_32b_t decode_32b_A6_184_POP(unsigned PC, unsigned int instruction_word){

    instruction_32b_t in;

    in.P = (instruction_word >> 15) & 0x01;
    in.M = (instruction_word >> 14) & 0x01;
    in.Rlist = (instruction_word >> 0) & 0x1FFF;

    return in;

}

instruction_32b_t decode_32b_A6_186_PUSH(unsigned PC, unsigned int instruction_word){

    instruction_32b_t in;

    in.M = (instruction_word >> 14) & 0x01;
    in.Rlist = (instruction_word >> 0) & 0x1FFF;

    return in;
}

instruction_32b_t decode_32b_A6_216_STM(unsigned PC, unsigned int instruction_word){

    instruction_32b_t in;

    in.W = (instruction_word >> (15 + 6)) & 0x01;
    in.M = (instruction_word >> 14) & 0x01;
    in.Rn = (instruction_word >> (15 + 1)) & 0x0F;
    in.Rlist = (instruction_word >> 0) & 0x1FFF;

    return in;
}

instruction_32b_t decode_32b_A6_218_STMDB(unsigned PC, unsigned int instruction_word){

    instruction_32b_t in;

    in.W = (instruction_word >> (15 + 6)) & 0x01;
    in.M = (instruction_word >> 14) & 0x01;
    in.Rn = (instruction_word >> (15 + 1)) & 0x0F;
    in.Rlist = (instruction_word >> 0) & 0x1FFF;

    return in;
}

instruction_32b_t decode_32b_A6_232_STREX(unsigned PC, unsigned int instruction_word){
    
    instruction_32b_t in;

    in.Rn = (instruction_word >>(15 +1)) & 0x0F;
    in.Rt = (instruction_word >> 12) & 0x0F;
    in.Rd = (instruction_word >> 8) & 0x0F;
    in.u32 = (instruction_word >> 0) & 0xFF;

    return in;

}

instruction_32b_t decode_32b_A6_230_STRD(unsigned PC, unsigned int instruction_word){
    
    instruction_32b_t in;

    in.P = (instruction_word >> (15 + 9)) & 0x01;
    in.U = (instruction_word >> (15 + 8)) & 0x01;
    in.W = (instruction_word >> (15 + 6)) & 0x01;
    in.Rn = (instruction_word >>(15 +1)) & 0x0F;
    in.Rt = (instruction_word >> 12) & 0x0F;
    in.Rt2 = (instruction_word >> 8) & 0x0F;
    in.u32 = (instruction_word >> 0) & 0xFF;

    return in;
}


instruction_32b_t decode_32b_A6_233_STREXB(unsigned PC, unsigned int instruction_word){
    
    instruction_32b_t in;

    in.Rn = (instruction_word >>(15 +1)) & 0x0F;
    in.Rt = (instruction_word >> 12) & 0x0F;
    in.Rd = (instruction_word >> 0) & 0x0F;

    return in;
}

instruction_32b_t decode_32b_A6_234_STREXH(unsigned PC, unsigned int instruction_word){
    
    instruction_32b_t in;

    in.Rn = (instruction_word >>(15 +1)) & 0x0F;
    in.Rt = (instruction_word >> 12) & 0x0F;
    in.Rd = (instruction_word >> 0) & 0x0F;

    return in;
}

instruction_32b_t decode_32b_A6_256_TBB_TBH(unsigned PC, unsigned int instruction_word){
    
    instruction_32b_t in;

    in.Rn = (instruction_word >>(15 +1)) & 0x0F;
    in.Rm = (instruction_word >> 0) & 0x0F;
    in.H  = (instruction_word >> 4) & 0x01;

    return in;

}

instruction_32b_t decode_32b_A6_88_LDR_imm(unsigned PC, unsigned int instruction_word){


}

instruction_32b_t decode_32b_A6_133_LDRT(unsigned PC, unsigned int instruction_word){

    
}

instruction_32b_t decode_32b_A6_92_LDR_reg(unsigned PC, unsigned int instruction_word){

    
}

instruction_32b_t decode_32b_A6_90_LDR(unsigned PC, unsigned int instruction_word){

    
}