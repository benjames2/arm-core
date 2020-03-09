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

    }
    else if(op1 == 0x02) {

    }
    else if(op1 == 0x03) {

    }
    else {
        throw std::runtime_error("decode_32bit_instruction : 16-bit instruction passed as 32-bit instruction");
    }

}


