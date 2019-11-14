#ifndef arm_instruction_asm_h
#define arm_instruction_asm_h

extern "C" {

    // arithmetic shift the given value by one bit
    int arithmetic_shift_right(int value);

    // compare two integers as unsigned numbers, returns flags that need to be set
    uint32_t unsigned_compare(uint32_t lhs, uint32_t rhs);

}

#endif // arm_instruction_asm_h