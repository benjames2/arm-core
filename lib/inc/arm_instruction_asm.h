#ifndef arm_instruction_asm_h
#define arm_instruction_asm_h

extern "C" {

    // arithmetic shift the given value by one bit
    int arithmetic_shift_right(int value);

    // bits shifted off the left end are shifted back in on the right
    uint32_t rotate_left(uint32_t value);

    // bits shifted off the right end are shifted back in on the left
    uint32_t rotate_right(uint32_t value);

    // compare two integers as unsigned numbers, returns flags that need to be set
    uint32_t unsigned_compare(uint32_t lhs, uint32_t rhs);

}

#endif // arm_instruction_asm_h
