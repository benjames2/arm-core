#include <inc/decode_32.h>
#include <string.h>

//Modified immediate constants in Thumb-2 instructions on Table A5-11
static int ThumbExpandImm(int i, int imm3, int imm8){

    int a = (imm8 >> 7) & 0x01;
    int imm32 = 0;

    if(i == 0){
        switch(imm3){
            case 0: 
                imm32 = imm32 | imm8;
                return imm32;
                break;
            case 1:
                if (imm8 == 0)
                    std::runtime_error("In ThumbExpand : case 1 : Unpredictable behavior");
                imm32 = imm32 | (imm8 << 16) | imm8;
                return imm32;
                break;
            case 2:
                if (imm8 == 0)
                    std::runtime_error("In ThumbExpand : case 2 : Unpredictable behavior");
                imm32 = imm32 | (imm8 << 24) | (imm8 << 8);
                return imm32;
                break;
            case 3:
                if (imm8 == 0)
                    std::runtime_error("In ThumbExpand : case 1 : Unpredictable behavior");
                imm32 = imm32 | (imm8 << 24) | (imm8 << 16) | (imm8 << 8) | imm8;
                return imm32;
                break;
            case 4:
                imm8  = imm8 | 0b10000000;
                if (a == 0){
                    imm32 = imm32 | (imm8 << 24);
                    return imm32;
                }
                else{
                    imm32 = imm32 | (imm8 << 23);
                    return imm32;
                } 
                break;
            case 5:
                imm8  = imm8 | 0b10000000;
                if (a == 0){
                    imm32 = imm32 | (imm8 << 22);
                    return imm32;
                }
                else{
                    imm32 = imm32 | (imm8 << 21);
                    return imm32;
                }
                break;
            case 6:
                std::runtime_error ("In ThumbExpand: case 6 not completed for i = 0"); 
                break;
            case 7: 
                std::runtime_error ("In ThumbExpand: case 7 not completed for i = 0");
                break;
            default:
                std::runtime_error("In ThumbExpand : default case for i = 0");
        }
    }
    else{
        imm8  = imm8 | 0b10000000;
        switch(imm3){
            case 0: case 1: case 2: case 3: case 4: case 5:
                std::runtime_error("In ThumbExpand : For i = 1 : case 0, 1, 2, 3, 4, 5 not done ");
                break;
            case 6:
                if(a == 1){
                    imm32 = imm32 | (imm8 << 3);
                    return imm32;
                }
                else 
                    std::runtime_error("In ThumbExpand : case 6 : invalid value of a for i = 1");
                break;
            case 7:
                if(a == 0){
                    imm32 = imm32 | (imm8 << 2);
                    return imm32;
                }
                else{
                    imm32 = imm32 | (imm8 << 1);
                    return imm32;
                }
                break;
            default:
                std::runtime_error("In ThumbExpand : default case for i = 1");
        }
    }
}

// ===========================================================
// Third level decode functions for 32-bit Thumb2 insructions
// ===========================================================
instruction_32b_t decode_32b_A6_18_ADC_imm(      unsigned int PC, unsigned int instruction_word); //add with carry immediate
instruction_32b_t decode_32b_A6_22_ADD_imm(      unsigned int PC, unsigned int instruction_word); //add mmediate
instruction_32b_t decode_32b_A6_32_AND_imm(      unsigned int PC, unsigned int instruction_word); //bitwise and immediate
instruction_32b_t decode_32b_A6_44_BIC_imm(      unsigned int PC, unsigned int instruction_word); //bitwise clear immeddiate
instruction_32b_t decode_32b_A6_58_CMN_imm(      unsigned int PC, unsigned int instruction_word); //compare negative immediate
instruction_32b_t decode_32b_A6_62_CMP_imm(      unsigned int PC, unsigned int instruction_word); //compare immediate
instruction_32b_t decode_32b_A6_72_EOR_imm(      unsigned int PC, unsigned int instruction_word); //bitwiswe exclusive OR immediate
instruction_32b_t decode_32b_A6_84_LDM(          unsigned int PC, unsigned int instruction_word); //load multiple
instruction_32b_t decode_32b_A6_88_LDR_imm_T3(   unsigned int PC, unsigned int instruction_word); //load register immediate T3 encoding
instruction_32b_t decode_32b_A6_88_LDR_imm_T4(   unsigned int PC, unsigned int instruction_word); //load register immediate T4 encoding
instruction_32b_t decode_32b_A6_90_LDR_lit(      unsigned int PC, unsigned int instruction_word); //load register (literal)
instruction_32b_t decode_32b_A6_92_LDR_reg(      unsigned int PC, unsigned int instruction_word); //load register (register)
instruction_32b_t decode_32b_A6_102_LDRD_imm(    unsigned int PC, unsigned int instruction_word); //load register dual immediate
instruction_32b_t decode_32b_A6_104_LDRD_lit(    unsigned int PC, unsigned int instruction_word); //load register dual (literal)
instruction_32b_t decode_32b_A6_106_LDREX(       unsigned int PC, unsigned int instruction_word); //load register exclusive
instruction_32b_t decode_32b_A6_107_LDREXB(      unsigned int PC, unsigned int instruction_word); //load register exclusive byte
instruction_32b_t decode_32b_A6_108_LDREXH(      unsigned int PC, unsigned int instruction_word); //load register exclusive halfword
instruction_32b_t decode_32b_A6_133_LDRT(        unsigned int PC, unsigned int instruction_word); //load register unprivileged
instruction_32b_t decode_32b_A6_148_MOV_imm(     unsigned int PC, unsigned int instruction_word); //move immediate
instruction_32b_t decode_32b_A6_162_MVN_imm(     unsigned int PC, unsigned int instruction_word); //bitwise not immediate
instruction_32b_t decode_32b_A6_168_ORN_imm(     unsigned int PC, unsigned int instruction_word); //bitwise OR NOT immediate
instruction_32b_t decode_32b_A6_172_ORR_imm(     unsigned int PC, unsigned int instruction_word); //bitwise inclusive OR
instruction_32b_t decode_32b_A6_184_POP(         unsigned int PC, unsigned int instruction_word); //pop multiple registers
instruction_32b_t decode_32b_A6_186_PUSH(        unsigned int PC, unsigned int instruction_word); //push multiple registers
instruction_32b_t decode_32b_A6_198_RSB_imm(     unsigned int PC, unsigned int instruction_word); //reverse substact immediate
instruction_32b_t decode_32b_A6_202_SBC_imm(     unsigned int PC, unsigned int instruction_word); //substract with carry immediate
instruction_32b_t decode_32b_A6_216_STM(         unsigned int PC, unsigned int instruction_word); //store multiple
instruction_32b_t decode_32b_A6_218_STMDB(       unsigned int PC, unsigned int instruction_word); //store multiple decrement before
instruction_32b_t decode_32b_A6_220_STR_imm_T3(  unsigned int PC, unsigned int instruction_word); //
instruction_32b_t decode_32b_A6_220_STR_imm_T4(  unsigned int PC, unsigned int instruction_word); //
instruction_32b_t decode_32b_A6_222_STR_reg(     unsigned int PC, unsigned int instruction_word); //
instruction_32b_t decode_32b_A6_224_STRB_imm_T2( unsigned int PC, unsigned int instruction_word); //
instruction_32b_t decode_32b_A6_224_STRB_imm_T3( unsigned int PC, unsigned int instruction_word); //
instruction_32b_t decode_32b_A6_226_STRB_reg(    unsigned int PC, unsigned int instruction_word); //
instruction_32b_t decode_32b_A6_230_STRD_imm(    unsigned int PC, unsigned int instruction_word); //store register dual immediate
instruction_32b_t decode_32b_A6_232_STREX(       unsigned int PC, unsigned int instruction_word); //store register exclusive
instruction_32b_t decode_32b_A6_233_STREXB(      unsigned int PC, unsigned int instruction_word); //store register excluaive byte
instruction_32b_t decode_32b_A6_234_STREXH(      unsigned int PC, unsigned int instruction_word); //store register exclusive halfword
instruction_32b_t decode_32b_A6_236_STRH_imm_T2( unsigned int PC, unsigned int instruction_word); //
instruction_32b_t decode_32b_A6_236_STRH_imm_T3( unsigned int PC, unsigned int instruction_word); //
instruction_32b_t decode_32b_A6_238_STRH_reg(    unsigned int PC, unsigned int instruction_word); //
instruction_32b_t decode_32b_A6_242_SUB_imm(     unsigned int PC, unsigned int instruction_word); //substract immediate
instruction_32b_t decode_32b_A6_256_TBB(         unsigned int PC, unsigned int instruction_word); //table branch byte 
instruction_32b_t decode_32b_A6_256_TBH(         unsigned int PC, unsigned int instruction_word); //table branch halfword
instruction_32b_t decode_32b_A6_258_TEQ_imm(     unsigned int PC, unsigned int instruction_word); //test equvalence immediate
instruction_32b_t decode_32b_A6_260_TST_imm(     unsigned int PC, unsigned int instruction_word); //test immediate

// ========================================================
// Top level decoding
// ========================================================
instruction_32b_t decode_32bit_instruction(unsigned int PC, unsigned int instruction_word) {

    int op1 = (instruction_word >> (11 + 16)) & 0x03;
    int op2 = (instruction_word >> ( 4 + 16)) & 0x7F;
    int op  = (instruction_word >> 15)        & 0x01;

    if(op1 == 0x01) {
        int mask = 0b1100100;
        if(op2 & mask == 0b0000000)
            return decode_32b_A5_20(PC, instruction_word);

        mask = 0b1100100;
        if(op2 & mask == 0b0000100)
            return decode_32b_A5_21(PC, instruction_word);

        mask = 0b1100000;
        if(op2 & mask == 0b0100000)
            return decode_32b_A5_26(PC, instruction_word);

        mask = 0b1000000;
        if(op2 & mask == 0b1000000)
            return decode_32b_A5_31(PC, instruction_word);
    }
    else if(op1 == 0x02) {

        if(op == 0x00) {
            int mask = 0b0100000;
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

        int mask = 0b1110001;
        if(op2 & mask == 0x00)
            return decode_32b_A5_25(PC, instruction_word);

        mask = 0b1100111;
        if(op2 & mask == 0b0000001)
            return decode_32b_A5_24(PC, instruction_word);
        
        if(op2 & mask == 0b0000011)
            return decode_32b_A5_23(PC, instruction_word);

        if(op2 & mask == 0b0000101)
            return decode_32b_A5_22(PC, instruction_word);
        
        if(op2 & mask == 0b0000111)
            throw std::runtime_error("decode_32bit_instruction : invalid op2 field");

        mask = 0b1110000;
        if(op2 & mask == 0b0100000)
            return decode_32b_A5_27(PC, instruction_word);

        mask = 0b1111000;
        if(op2 & mask == 0b0110000)
            return decode_32b_A5_29(PC, instruction_word);

        if(op2 & mask == 0b0111000)
            return decode_32b_A5_30(PC, instruction_word);

        mask = 0b1000000;
        if(op2 & mask == 0b1000000)
            return decode_32b_A5_31(PC, instruction_word);

    }
    else {
        throw std::runtime_error("decode_32bit_instruction : 16-bit instruction passed as 32-bit instruction");
    }

    throw std::runtime_error("decode_32bit_instruction : invalid instruction encoding");

}

// ========================================================
// Second level decoding
// ========================================================
instruction_32b_t decode_32b_A5_14(unsigned int PC, unsigned int instruction_word) {

    int op = (instruction_word >> (4 + 16)) & 0x1F;
    int Rn = (instruction_word >> (0 + 16)) & 0x0F;
    int Rd = (instruction_word >> (8 + 0)) & 0x0F;

    if (Rd != 0b1111){
        int mask = 0b11110;
        if((op & mask) == 0b00000)
            return decode_32b_A6_32_AND_imm(PC, instruction_word);
        
        mask = 0b01000;
        if((op & mask) == 0b01000)
            return decode_32b_A6_72_EOR_imm(PC, instruction_word);

        mask = 0b10000;
        if((op & mask) == 0b10000)
            return decode_32b_A6_22_ADD_imm(PC, instruction_word);

        mask = 0b11010;
        if ((op & mask) == 0b11010)
            return decode_32b_A6_242_SUB_imm(PC, instruction_word);
    }
    else{
        int mask = 0b11110;
        if((op & mask) == 0b00000)
            return decode_32b_A6_260_TST_imm(PC, instruction_word);

        mask = 0b01000;
        if((op & mask) == 0b01000)
            return decode_32b_A6_258_TEQ_imm(PC, instruction_word);

        mask = 0b10000;
        if((op & mask) == 0b10000)
            return decode_32b_A6_58_CMN_imm(PC, instruction_word);

        mask = 0b11010;
        if ((op & mask) == 0b11010)
            return decode_32b_A6_62_CMP_imm(PC, instruction_word);
    }

    if(Rn =! 0b1111){
        int mask = 0b00100;
        if((op & mask) == 0b00100)
            return decode_32b_A6_172_ORR_imm(PC, instruction_word);

        mask = 0b00110;
        if((op & mask) == 0b00110)
            return decode_32b_A6_168_ORN_imm(PC, instruction_word);
    }
    else{
        int mask = 0b00100;
        if((op & mask) == 0b00100)
            return decode_32b_A6_148_MOV_imm(PC, instruction_word);

        mask = 0b00110;
        if((op & mask) == 0b00110)
            return decode_32b_A6_162_MVN_imm(PC, instruction_word);
    }

    int mask = 0b00010;
    if((op & mask) == 0b00010)
        return decode_32b_A6_44_BIC_imm(PC, instruction_word);

    mask = 0b10100;
    if((op & mask) == 0b10100)
        return decode_32b_A6_18_ADC_imm(PC, instruction_word);

    mask = 0b10110;
    if((op & mask) == 0b10110)
        return decode_32b_A6_202_SBC_imm(PC, instruction_word);

    mask = 0b11100;
    if((op & mask) == 0b11100)
        return decode_32b_A6_198_RSB_imm(PC, instruction_word);

    throw std::runtime_error("In decode_32b_A5_14 : Invalid instruction encoding");

}

instruction_32b_t decode_32b_A5_17(unsigned int PC, unsigned int instruction_word) {
    throw std::runtime_error("decode_32b_A5_17 : undefined");
}

instruction_32b_t decode_32b_A5_18(unsigned int PC, unsigned int instruction_word) {
    throw std::runtime_error("decode_32b_A5_18 : undefined");
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
            throw std::runtime_error("In decode_32b_A5_20: L field is invalid for op = 1");
    }

    else if(op == 0x10){
        if(L == 0x00){
            if(Rn == 0x1101)
                return decode_32b_A6_186_PUSH(PC, instruction_word);
            else
                return decode_32b_A6_218_STMDB(PC, instruction_word);
        }
        else
            throw std::runtime_error("In decode_32b_A5_20: L field is invalid for op = 2");
    }

    else{
        throw std::runtime_error("In decode_32b_A5_20: invalid op value");
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
            return decode_32b_A6_230_STRD_imm(PC, instruction_word);
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
                return decode_32b_A6_256_TBB(PC, instruction_word);
            else if(op3 == 0b0001)
                return decode_32b_A6_256_TBH(PC, instruction_word);
            else if(op3 == 0b0100)
                return decode_32b_A6_107_LDREXB(PC, instruction_word);
            else if(op3 == 0b0101)
                return decode_32b_A6_108_LDREXH(PC, instruction_word);
            else{
                throw std::runtime_error("In decode_32b_A5_21: Invalid op3 value for op2 = 1");
            }
        }

        else if(op2 == 0b10){
            return decode_32b_A6_230_STRD_imm(PC, instruction_word);
        }
        else{
            return decode_32b_A6_102_LDRD_imm(PC, instruction_word);
        }
    }

    else if((op1 == 0b10) || (op1 == 0b11)){
        if ((op2 == 0b00) || (op2 == 0b10))
            return decode_32b_A6_230_STRD_imm(PC, instruction_word);
        if((op2 == 0b01) || (op2 == 0b11))
            return decode_32b_A6_104_LDRD_lit(PC, instruction_word); 
    }

    else
        throw std::runtime_error("In decode_32b_A5_21: invalid encoding instruction");
    
}

instruction_32b_t decode_32b_A5_22(unsigned int PC, unsigned int instruction_word) {

    int op1 = (instruction_word >> (15 + 8)) & 0x03;
    int op2 = (instruction_word >> 6) & 0x3F;
    int Rn  = (instruction_word >> (15 + 1)) & 0x0F;

    if(Rn != 0b1111){
        if(op1 == 0x01)
            return decode_32b_A6_88_LDR_imm_T3(PC, instruction_word);

        else if(op1 == 0x00){
            int mask = 0b100100;
            int mask2 = 0b110000;
            if(((op2 & mask) == 0x100100)  || ((op2 & mask2) == 0x110000))
                return decode_32b_A6_88_LDR_imm_T4(PC, instruction_word);

            mask = 0b111000;
            if((op2 & mask) == 0b111000)
                return decode_32b_A6_133_LDRT(PC, instruction_word);

            if (op2 == 0b00)
                return decode_32b_A6_92_LDR_reg(PC, instruction_word);

        }
        else
            throw std::runtime_error("In decode_32b_A5_22 : invalid value for op1 field with Rn != 0b1111");
        
    }
    else{
        int mask = 0b10;
        if((op1 & mask) == 0b00)
            return decode_32b_A6_90_LDR_lit(PC, instruction_word);
        else
            throw std::runtime_error("In decode_32b_A5_22 : invalid value for op1 field");     
    }

    throw std::runtime_error("In decode_32b_A5_22 : invalid instruction encoding");

}

instruction_32b_t decode_32b_A5_23(unsigned int PC, unsigned int instruction_word) {
    throw std::runtime_error("decode_32b_A5_23 : undefined");
}

instruction_32b_t decode_32b_A5_24(unsigned int PC, unsigned int instruction_word) {
    throw std::runtime_error("decode_32b_A5_24 : undefined");
}

instruction_32b_t decode_32b_A5_25(unsigned int PC, unsigned int instruction_word) {

    int op1 = (instruction_word >> (15 + 6)) & 0x07;    
    int op2 = (instruction_word >>        6) & 0x3F;

    if(op1 == 0b000){
        int mask = 0b100000;
        if((op2 & mask) == 0b100000)
            return decode_32b_A6_224_STRB_imm_T3(PC, instruction_word);

        if((op2 & mask) == 0b000000)
            return decode_32b_A6_226_STRB_reg(PC, instruction_word);
        }

    else if(op1 == 0b001){
        int mask = 0b100000;
        if((op2 & mask) == 0b000000)
            return decode_32b_A6_238_STRH_reg(PC, instruction_word);

        if((op2 & mask) == 0b100000)
            return decode_32b_A6_236_STRH_imm_T3(PC, instruction_word);
    }

    else if(op1 == 0b010){
        int mask = 0b100000;
        if((op2 & mask) == 0b100000)
            return decode_32b_A6_220_STR_imm_T4(PC, instruction_word);

        if((op2 & mask) == 0b000000)
            return decode_32b_A6_222_STR_reg(PC, instruction_word);
    }

    else if(op1 == 0b100)
        return decode_32b_A6_224_STRB_imm_T2(PC, instruction_word);

    else if(op1 == 0b101)
        return decode_32b_A6_236_STRH_imm_T2(PC, instruction_word);

    else if(op1 == 0b110)
        return decode_32b_A6_220_STR_imm_T3(PC, instruction_word);

    else
        std::runtime_error("In decode_32b_A5_25 : invalid op1 value");

    std::runtime_error("In decode_32b_A5_25 : invalid instruction encoding");
        
}

instruction_32b_t decode_32b_A5_26(unsigned int PC, unsigned int instruction_word) {
    throw std::runtime_error("decode_32b_A5_26 : undefined");
}

instruction_32b_t decode_32b_A5_27(unsigned int PC, unsigned int instruction_word) {
    throw std::runtime_error("decode_32b_A5_27 : undefined");
}

instruction_32b_t decode_32b_A5_29(unsigned int PC, unsigned int instruction_word) {
    throw std::runtime_error("decode_32b_A5_29 : undefined");
}

instruction_32b_t decode_32b_A5_30(unsigned int PC, unsigned int instruction_word) {
    throw std::runtime_error("decode_32b_A5_30 : undefined");
}

instruction_32b_t decode_32b_A5_31(unsigned int PC, unsigned int instruction_word) {
    throw std::runtime_error("decode_32b_A5_31 : undefined");
}

// ========================================================
// Third level decoding function (implementations)
// ========================================================

instruction_32b_t decode_32b_A6_18_ADC_imm(unsigned int PC, unsigned int instruction_word){

    instruction_32b_t in;

    in.opcode      = t32_ADC;
    in.meta_opcode = meta_t32_imm;

    in.Rn = (instruction_word >> (15 + 1)) & 0x0F;
    in.Rd = (instruction_word >> 8) & 0x0F;
    in.S  = (instruction_word >> (15 + 5)) & 0x01;

    int i    = (instruction_word >> (15 + 11)) & 0x01;
    int imm3 = (instruction_word >> 12) & 0x07;
    int imm8 = (instruction_word >> 0) & 0xFF;

    in.i32 = ThumbExpandImm(i, imm3, imm8);
    
    return in;
}

instruction_32b_t decode_32b_A6_22_ADD_imm(unsigned int PC, unsigned int instruction_word){

    instruction_32b_t in;

    in.opcode      = t32_ADD;
    in.meta_opcode = meta_t32_imm;
    in.encoding    = instruction_32b_t::encoding_T3;

    in.Rn = (instruction_word >> (15 + 1)) & 0x0F;
    in.Rd = (instruction_word >> 8) & 0x0F;
    in.S  = (instruction_word >> (15 + 5)) & 0x01;

    int i    = (instruction_word >> (15 + 11)) & 0x01;
    int imm3 = (instruction_word >> 12) & 0x07;
    int imm8 = (instruction_word >> 0) & 0xFF;

    in.i32 = ThumbExpandImm(i, imm3, imm8);
    
    return in;
}

instruction_32b_t decode_32b_A6_32_AND_imm(unsigned int PC, unsigned int instruction_word){

    instruction_32b_t in;

    in.opcode      = t32_ADD;
    in.meta_opcode = meta_t32_imm;

    in.Rn = (instruction_word >> (15 + 1)) & 0x0F;
    in.Rd = (instruction_word >> 8) & 0x0F;
    in.S  = (instruction_word >> (15 + 5)) & 0x01;

    int i    = (instruction_word >> (15 + 11)) & 0x01;
    int imm3 = (instruction_word >> 12) & 0x07;
    int imm8 = (instruction_word >> 0) & 0xFF;

    in.i32 = ThumbExpandImm(i, imm3, imm8);


    return in;
}

instruction_32b_t decode_32b_A6_44_BIC_imm(unsigned int PC, unsigned int instruction_word){

    instruction_32b_t in;

    in.opcode      = t32_BIC;
    in.meta_opcode = meta_t32_imm;

    in.Rn = (instruction_word >> (15 + 1)) & 0x0F;
    in.Rd = (instruction_word >> 8) & 0x0F;
    in.S  = (instruction_word >> (15 + 5)) & 0x01;

    int i    = (instruction_word >> (15 + 11)) & 0x01;
    int imm3 = (instruction_word >> 12) & 0x07;
    int imm8 = (instruction_word >> 0) & 0xFF;

    in.i32 = ThumbExpandImm(i, imm3, imm8);

    
    return in;
}

instruction_32b_t decode_32b_A6_58_CMN_imm(unsigned int PC, unsigned int instruction_word){

    instruction_32b_t in;

    in.opcode      = t32_CMN;
    in.meta_opcode = meta_t32_imm;

    in.Rn = (instruction_word >> (15 + 1)) & 0x0F;

    int i    = (instruction_word >> (15 + 11)) & 0x01;
    int imm3 = (instruction_word >> 12) & 0x07;
    int imm8 = (instruction_word >> 0) & 0xFF;

    in.i32 = ThumbExpandImm(i, imm3, imm8);
    
    return in;
}

instruction_32b_t decode_32b_A6_62_CMP_imm(unsigned int PC, unsigned int instruction_word){

    instruction_32b_t in;

    in.opcode      = t32_CMP;
    in.meta_opcode = meta_t32_imm;

    in.Rn = (instruction_word >> (15 + 1)) & 0x0F;

    int i    = (instruction_word >> (15 + 11)) & 0x01;
    int imm3 = (instruction_word >> 12) & 0x07;
    int imm8 = (instruction_word >> 0) & 0xFF;

    in.i32 = ThumbExpandImm(i, imm3, imm8);
    
    return in;
}

instruction_32b_t decode_32b_A6_72_EOR_imm(unsigned int PC, unsigned int instruction_word){

    instruction_32b_t in;

    in.opcode      = t32_EOR;
    in.meta_opcode = meta_t32_imm;

    in.Rn = (instruction_word >> (15 + 1)) & 0x0F;
    in.Rd = (instruction_word >> 8) & 0x0F;
    in.S  = (instruction_word >> (15 + 5)) & 0x01;

    int i    = (instruction_word >> (15 + 11)) & 0x01;
    int imm3 = (instruction_word >> 12) & 0x07;
    int imm8 = (instruction_word >> 0) & 0xFF;

    in.i32 = ThumbExpandImm(i, imm3, imm8);
    
    return in;
}

instruction_32b_t decode_32b_A6_84_LDM(unsigned int PC, unsigned int instruction_word){

    instruction_32b_t in;

    in.opcode = t32_LDM;
    in.P      = (instruction_word >> 15) & 0x01;
    in.M      = (instruction_word >> 14) & 0x01;
    in.W      = (instruction_word >> (15 + 6)) & 0x01;
    in.Rn     = (instruction_word >> (15 + 1)) & 0x0F;
    in.Rlist  = (instruction_word >> 0) & 0x1FFF;

    return in;
}

instruction_32b_t decode_32b_A6_88_LDR_imm_T3(unsigned int PC, unsigned int instruction_word){

    instruction_32b_t in;

    in.opcode      = t32_LDR;
    in.meta_opcode = meta_t32_imm;
    in.encoding    = instruction_32b_t::encoding_T3;

    in.Rn   = (instruction_word >> (15 + 1)) & 0x0F;
    in.Rt   = (instruction_word >> 12) & 0x0F;
    in.u32  = (instruction_word >> 0) & 0xFFF; // I believe this will be zero extended to 32 bits anyway 
    
    return in;
}

instruction_32b_t decode_32b_A6_88_LDR_imm_T4(unsigned int PC, unsigned int instruction_word){

    instruction_32b_t in;
     
    in.opcode      = t32_LDR;
    in.meta_opcode = meta_t32_imm;
    in.encoding    = instruction_32b_t::encoding_T4;

    in.P   = (instruction_word >> 10) & 0x01;
    in.U   = (instruction_word >>  9) & 0x01;
    in.W   = (instruction_word >>  8) & 0x01;
    in.Rn  = (instruction_word >> (15 + 1)) & 0x0F;
    in.Rt  = (instruction_word >> 12) & 0x0F;
    in.i32 = (instruction_word >>  0) & 0xFF; // not sure for i32 or u32

    return in;
}

instruction_32b_t decode_32b_A6_90_LDR_lit(unsigned int PC, unsigned int instruction_word){

    instruction_32b_t in;

    in.opcode      = t32_LDR;
    in.meta_opcode = meta_t32_literal;

    in.U      = (instruction_word >> (15 + 8)) & 0x01;
    in.Rt     = (instruction_word >> 12) & 0x0F;
    in.u32    = (instruction_word >> 0) & 0xFFF;

    return in;
}

instruction_32b_t decode_32b_A6_92_LDR_reg(unsigned int PC, unsigned int instruction_word){

    instruction_32b_t in;

    in.opcode      = t32_LDR;
    in.meta_opcode = meta_t32_reg;

    in.Rn   = (instruction_word >>(15 +1)) & 0x0F;
    in.Rt   = (instruction_word >> 12) & 0x0F;
    in.Rm   = (instruction_word >> 0) & 0x0F;
    in.u32  = (instruction_word >> 4) & 0x03;

    return in;
}

instruction_32b_t decode_32b_A6_102_LDRD_imm(unsigned int PC, unsigned int instruction_word){
    
    instruction_32b_t in;

    int imm8 = (instruction_word >> 0) & 0xFF;

    in.opcode      = t32_LDRD;
    in.meta_opcode = meta_t32_imm;

    in.P   = (instruction_word >> (15 + 9)) & 0x01;
    in.U   = (instruction_word >> (15 + 8)) & 0x01;
    in.W   = (instruction_word >> (15 + 6)) & 0x01;
    in.Rn  = (instruction_word >>(15 +1)) & 0x0F;
    in.Rt  = (instruction_word >> 12) & 0x0F;
    in.Rt2 = (instruction_word >> 8) & 0x0F;
    in.u32 = imm8 << 2;

    return in;
}

instruction_32b_t decode_32b_A6_104_LDRD_lit(unsigned int PC, unsigned int instruction_word){

    instruction_32b_t in;

    in.opcode      = t32_LDRD;
    in.meta_opcode = meta_t32_literal;

    in.P   = (instruction_word >> (15 + 9)) & 0x01;
    in.U   = (instruction_word >> (15 + 8)) & 0x01;
    in.Rt  = (instruction_word >> 12) & 0x0F;
    in.Rt2 = (instruction_word >> 8) & 0x0F;

    return in;
}

instruction_32b_t decode_32b_A6_106_LDREX(unsigned int PC, unsigned int instruction_word){
    
    instruction_32b_t in;

    in.opcode = t32_LDREX;

    in.Rn  = (instruction_word >>(15 +1)) & 0x0F;
    in.Rt  = (instruction_word >> 12) & 0x0F;
    in.u32 = (instruction_word >> 0) & 0xFF;

    return in;
}

instruction_32b_t decode_32b_A6_107_LDREXB(unsigned int PC, unsigned int instruction_word){

    instruction_32b_t in;

    in.opcode = t32_LDREXB;

    in.Rn = (instruction_word >>(15 +1)) & 0x0F;
    in.Rt = (instruction_word >> 12) & 0x0F;

    return in;
}

instruction_32b_t decode_32b_A6_108_LDREXH(unsigned int PC, unsigned int instruction_word){

    instruction_32b_t in;

    in.opcode = t32_LDREXH;

    in.Rn = (instruction_word >>(15 +1)) & 0x0F;
    in.Rt = (instruction_word >> 12) & 0x0F;

    return in;
}

instruction_32b_t decode_32b_A6_133_LDRT(unsigned int PC, unsigned int instruction_word){

    instruction_32b_t in;

    in.opcode = t32_LDRT;

    in.Rn  = (instruction_word >>(15 +1)) & 0x0F;
    in.Rt  = (instruction_word >> 12) & 0x0F;
    in.u32 = (instruction_word >> 0) & 0xFF;

    return in;
}

instruction_32b_t decode_32b_A6_148_MOV_imm(unsigned int PC, unsigned int instruction_word){

    //T2 Encoding
    instruction_32b_t in;

    in.opcode      = t32_MOV;
    in.meta_opcode = meta_t32_imm;

    in.S  = (instruction_word >> (15 + 5)) & 0x01;
    in.Rd = (instruction_word >> 8) & 0x0F;

    int i    = (instruction_word >> (15 + 11)) & 0x01;
    int imm3 = (instruction_word >> 12) & 0x07;
    int imm8 = (instruction_word >> 0) & 0xFF;

    in.i32 = ThumbExpandImm(i, imm3, imm8);

    return in;
}

instruction_32b_t decode_32b_A6_162_MVN_imm(unsigned int PC, unsigned int instruction_word){

    instruction_32b_t in;

    in.opcode      = t32_MVN;
    in.meta_opcode = meta_t32_imm;

    in.S  = (instruction_word >> (15 + 5)) & 0x01;
    in.Rd = (instruction_word >> 8) & 0x0F;

    int i    = (instruction_word >> (15 + 11)) & 0x01;
    int imm3 = (instruction_word >> 12) & 0x07;
    int imm8 = (instruction_word >> 0) & 0xFF;

    in.i32 = ThumbExpandImm(i, imm3, imm8);
    
    return in;
}

instruction_32b_t decode_32b_A6_168_ORN_imm(unsigned int PC, unsigned int instruction_word){

    instruction_32b_t in;

    in.opcode      = t32_ORN;
    in.meta_opcode = meta_t32_imm;
    
    in.S  = (instruction_word >> (15 + 5)) & 0x01;
    in.Rd = (instruction_word >> 8) & 0x0F;

    int i    = (instruction_word >> (15 + 11)) & 0x01;
    int imm3 = (instruction_word >> 12) & 0x07;
    int imm8 = (instruction_word >> 0) & 0xFF;

    in.i32 = ThumbExpandImm(i, imm3, imm8);

    return in;
}

instruction_32b_t decode_32b_A6_172_ORR_imm(unsigned int PC, unsigned int instruction_word){

    instruction_32b_t in;

    in.opcode = t32_ORR;
    in.meta_opcode = meta_t32_imm;

    in.S  = (instruction_word >> (15 + 5)) & 0x01;
    in.Rd = (instruction_word >> 8) & 0x0F;

    int i    = (instruction_word >> (15 + 11)) & 0x01;
    int imm3 = (instruction_word >> 12) & 0x07;
    int imm8 = (instruction_word >> 0) & 0xFF;

    in.i32 = ThumbExpandImm(i, imm3, imm8);
    
    return in;
}

instruction_32b_t decode_32b_A6_184_POP(unsigned int PC, unsigned int instruction_word){

    instruction_32b_t in;

    in.opcode = t32_POP;

    in.P     = (instruction_word >> 15) & 0x01;
    in.M     = (instruction_word >> 14) & 0x01;
    in.Rlist = (instruction_word >> 0) & 0x1FFF;

    return in;
}

instruction_32b_t decode_32b_A6_186_PUSH(unsigned int PC, unsigned int instruction_word){

    instruction_32b_t in;

    in.opcode = t32_PUSH;

    in.M     = (instruction_word >> 14) & 0x01;
    in.Rlist = (instruction_word >> 0) & 0x1FFF;

    return in;
}

instruction_32b_t decode_32b_A6_198_RSB_imm(unsigned int PC, unsigned int instruction_word){

    instruction_32b_t in;

    in.opcode      = t32_RSB;
    in.meta_opcode = meta_t32_imm;

    in.S  = (instruction_word >> (15 + 5)) & 0x01;
    in.Rd = (instruction_word >> 8) & 0x0F;

    int i    = (instruction_word >> (15 + 11)) & 0x01;
    int imm3 = (instruction_word >> 12) & 0x07;
    int imm8 = (instruction_word >> 0) & 0xFF;

    in.i32 = ThumbExpandImm(i, imm3, imm8);
    
    return in;
}

instruction_32b_t decode_32b_A6_202_SBC_imm(unsigned int PC, unsigned int instruction_word){

    instruction_32b_t in;

    in.opcode      = t32_SBC;
    in.meta_opcode = meta_t32_imm;

    in.S  = (instruction_word >> (15 + 5)) & 0x01;
    in.Rd = (instruction_word >> 8) & 0x0F;

    int i    = (instruction_word >> (15 + 11)) & 0x01;
    int imm3 = (instruction_word >> 12) & 0x07;
    int imm8 = (instruction_word >> 0) & 0xFF;

    in.i32 = ThumbExpandImm(i, imm3, imm8);
    
    return in;
}

instruction_32b_t decode_32b_A6_216_STM(unsigned int PC, unsigned int instruction_word){

    instruction_32b_t in;

    in.opcode = t32_STM;

    in.W     = (instruction_word >> (15 + 6)) & 0x01;
    in.M     = (instruction_word >> 14) & 0x01;
    in.Rn    = (instruction_word >> (15 + 1)) & 0x0F;
    in.Rlist = (instruction_word >> 0) & 0x1FFF;

    return in;
}

instruction_32b_t decode_32b_A6_218_STMDB(unsigned int PC, unsigned int instruction_word){

    instruction_32b_t in;

    in.opcode = t32_STMDB;

    in.W     = (instruction_word >> (15 + 6)) & 0x01;
    in.M     = (instruction_word >> 14) & 0x01;
    in.Rn    = (instruction_word >> (15 + 1)) & 0x0F;
    in.Rlist = (instruction_word >> 0) & 0x1FFF;

    return in;
}

instruction_32b_t decode_32b_A6_220_STR_imm_T3(unsigned int PC, unsigned int instruction_word){

    instruction_32b_t in;

    in.opcode = t32_STR;
    in.meta_opcode = meta_t32_imm;

    in.Rn  = (instruction_word >>(15 +1)) & 0x0F;
    in.Rt  = (instruction_word >> 12) & 0x0F;
    in.u32 = (instruction_word >> 0) & 0xFFF;

    return in;
}

instruction_32b_t decode_32b_A6_220_STR_imm_T4(unsigned int PC, unsigned int instruction_word){

    instruction_32b_t in;

    in.opcode = t32_STR;

    in.P   = (instruction_word >> 10) & 0x01;
    in.U   = (instruction_word >>  9) & 0x01;
    in.W   = (instruction_word >>  8) & 0x01;
    in.Rn  = (instruction_word >> (15 +1)) & 0x0F;
    in.Rt  = (instruction_word >> 12) & 0x0F;
    in.u32 = (instruction_word >> 0) & 0xFF;

    return in;
}

instruction_32b_t decode_32b_A6_222_STR_reg(unsigned int PC, unsigned int instruction_word){
    throw std::runtime_error("In decode_32b_A6_222_STR_reg : undefined");
}

instruction_32b_t decode_32b_A6_224_STRB_imm_T2(unsigned int PC, unsigned int instruction_word){

    instruction_32b_t in;
    
    in.opcode = t32_STRB;
    in.meta_opcode = meta_t32_imm;

    in.Rn  = (instruction_word >>(15 +1)) & 0x0F;
    in.Rt  = (instruction_word >> 12) & 0x0F;
    in.u32 = (instruction_word >> 0) & 0xFFF;


    return in;
}

instruction_32b_t decode_32b_A6_224_STRB_imm_T3(unsigned int PC, unsigned int instruction_word){

    instruction_32b_t in;

    in.opcode = t32_STRB;

    in.P   = (instruction_word >> 10) & 0x01;
    in.U   = (instruction_word >>  9) & 0x01;
    in.W   = (instruction_word >>  8) & 0x01;
    in.Rn  = (instruction_word >> (15 +1)) & 0x0F;
    in.Rt  = (instruction_word >> 12) & 0x0F;
    in.u32 = (instruction_word >> 0) & 0xFF;


    return in;
}

instruction_32b_t decode_32b_A6_226_STRB_reg(unsigned int PC, unsigned int instruction_word){
    throw std::runtime_error("In decode_32b_A6_226_STRB_reg : undefined");
}

instruction_32b_t decode_32b_A6_230_STRD_imm(unsigned int PC, unsigned int instruction_word){
    
    instruction_32b_t in;

    in.opcode = t32_STRD;
    int imm8  = (instruction_word >> 0) & 0xFF;


    in.P   = (instruction_word >> (15 + 9)) & 0x01;
    in.U   = (instruction_word >> (15 + 8)) & 0x01;
    in.W   = (instruction_word >> (15 + 6)) & 0x01;
    in.Rn  = (instruction_word >>(15 +1)) & 0x0F;
    in.Rt  = (instruction_word >> 12) & 0x0F;
    in.Rt2 = (instruction_word >> 8) & 0x0F;
    in.u32 = imm8 << 2;

    return in;
}

instruction_32b_t decode_32b_A6_232_STREX(unsigned int PC, unsigned int instruction_word){
    
    instruction_32b_t in;

    in.opcode = t32_STREX;
    int imm8  = (instruction_word >> 0) & 0xFF;

    in.Rn  = (instruction_word >>(15 +1)) & 0x0F;
    in.Rt  = (instruction_word >> 12) & 0x0F;
    in.Rd  = (instruction_word >> 8) & 0x0F;
    in.u32 = imm8 << 2;

    return in;
}

instruction_32b_t decode_32b_A6_233_STREXB(unsigned int PC, unsigned int instruction_word){
    
    instruction_32b_t in;

    in.opcode = t32_STREXB;

    in.Rn = (instruction_word >>(15 +1)) & 0x0F;
    in.Rt = (instruction_word >> 12) & 0x0F;
    in.Rd = (instruction_word >> 0) & 0x0F;

    return in;
}

instruction_32b_t decode_32b_A6_234_STREXH(unsigned int PC, unsigned int instruction_word){
    
    instruction_32b_t in;

    in.opcode = t32_STREXH;

    in.Rn = (instruction_word >>(15 +1)) & 0x0F;
    in.Rt = (instruction_word >> 12) & 0x0F;
    in.Rd = (instruction_word >> 0) & 0x0F;

    return in;
}

instruction_32b_t decode_32b_A6_236_STRH_imm_T2(unsigned int PC, unsigned int instruction_word){

    instruction_32b_t in;

    in.opcode = t32_STRH;
    in.meta_opcode = meta_t32_imm;

    in.Rn  = (instruction_word >>(15 +1)) & 0x0F;
    in.Rt  = (instruction_word >> 12) & 0x0F;
    in.u32 = (instruction_word >> 0) & 0xFFF;

    return in;
}

instruction_32b_t decode_32b_A6_236_STRH_imm_T3(unsigned int PC, unsigned int instruction_word){

    instruction_32b_t in;

    in.opcode = t32_STRH;

    in.P   = (instruction_word >> 10) & 0x01;
    in.U   = (instruction_word >>  9) & 0x01;
    in.W   = (instruction_word >>  8) & 0x01;
    in.Rn  = (instruction_word >> (15 +1)) & 0x0F;
    in.Rt  = (instruction_word >> 12) & 0x0F;
    in.u32 = (instruction_word >> 0) & 0xFF;

    return in;
}

instruction_32b_t decode_32b_A6_238_STRH_reg(unsigned int PC, unsigned int instruction_word){
    throw std::runtime_error("In decode_32b_A6_238_STRH_reg : undefined");
}

instruction_32b_t decode_32b_A6_242_SUB_imm(unsigned int PC, unsigned int instruction_word){

    //Encoding T3
    instruction_32b_t in;

    in.opcode      = t32_SUB;
    in.meta_opcode = meta_t32_imm;

    in.S  = (instruction_word >> (15 + 5)) & 0x01;
    in.Rd = (instruction_word >> 8) & 0x0F;

    int i    = (instruction_word >> (15 + 11)) & 0x01;
    int imm3 = (instruction_word >> 12) & 0x07;
    int imm8 = (instruction_word >> 0) & 0xFF;

    in.i32 = ThumbExpandImm(i, imm3, imm8);
    
    return in;
}

instruction_32b_t decode_32b_A6_256_TBB(unsigned int PC, unsigned int instruction_word){
    
    instruction_32b_t in;

    in.opcode = t32_TBB;

    in.Rn = (instruction_word >>(15 +1)) & 0x0F;
    in.Rm = (instruction_word >> 0) & 0x0F;
    in.H  = (instruction_word >> 4) & 0x01;

    return in;
}

instruction_32b_t decode_32b_A6_256_TBH(unsigned int PC, unsigned int instruction_word){
    
    instruction_32b_t in;

    in.opcode = t32_TBH;

    in.Rn = (instruction_word >>(15 +1)) & 0x0F;
    in.Rm = (instruction_word >> 0) & 0x0F;
    in.H  = (instruction_word >> 4) & 0x01;

    return in;
}

instruction_32b_t decode_32b_A6_258_TEQ_imm(unsigned int PC, unsigned int instruction_word){

    instruction_32b_t in;

    in.opcode      = t32_TEQ;
    in.meta_opcode = meta_t32_imm;

    int i    = (instruction_word >> (15 + 11)) & 0x01;
    int imm3 = (instruction_word >> 12) & 0x07;
    int imm8 = (instruction_word >> 0) & 0xFF;

    in.i32 = ThumbExpandImm(i, imm3, imm8);
    
    return in;
}

instruction_32b_t decode_32b_A6_260_TST_imm(unsigned int PC, unsigned int instruction_word){

    instruction_32b_t in;

    in.opcode      = t32_TST;
    in.meta_opcode = meta_t32_imm;

    int i    = (instruction_word >> (15 + 11)) & 0x01;
    int imm3 = (instruction_word >> 12) & 0x07;
    int imm8 = (instruction_word >> 0) & 0xFF;

    in.i32 = ThumbExpandImm(i, imm3, imm8);
    
    return in;
}
