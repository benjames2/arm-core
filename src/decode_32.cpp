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

instruction_t decode_32bit_instruction(unsigned int PC, unsigned int instruction_word) {

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

}

instruction_t decode_32b_A5_20(unsigned int PC, unsigned int instruction_word) {

}

instruction_t decode_32b_A5_21(unsigned int PC, unsigned int instruction_word) {

}

instruction_t decode_32b_A5_26(unsigned int PC, unsigned int instruction_word) {
    
}

instruction_t decode_32b_A5_14(unsigned int PC, unsigned int instruction_word) {

}

instruction_t decode_32b_A5_17(unsigned int PC, unsigned int instruction_word) {

}

instruction_t decode_32b_A5_18(unsigned int PC, unsigned int instruction_word) {

}

instruction_t decode_32b_A5_25(unsigned int PC, unsigned int instruction_word) {

}

instruction_t decode_32b_A5_24(unsigned int PC, unsigned int instruction_word) {

}

instruction_t decode_32b_A5_23(unsigned int PC, unsigned int instruction_word) {

}

instruction_t decode_32b_A5_22(unsigned int PC, unsigned int instruction_word) {

}

instruction_t decode_32b_A5_27(unsigned int PC, unsigned int instruction_word) {

}

instruction_t decode_32b_A5_29(unsigned int PC, unsigned int instruction_word) {

}

instruction_t decode_32b_A5_30(unsigned int PC, unsigned int instruction_word) {

}

instruction_t decode_32b_A5_31(unsigned int PC, unsigned int instruction_word) {

}
