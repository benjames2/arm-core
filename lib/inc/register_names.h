#ifndef register_names_h
#define register_names_h

const int arm_reg0  = 0;
const int arm_reg1  = 1;
const int arm_reg2  = 2;
const int arm_reg3  = 3;
const int arm_reg4  = 4;
const int arm_reg5  = 5;
const int arm_reg6  = 6;
const int arm_reg7  = 7;
const int arm_reg8  = 8;
const int arm_reg9  = 9;
const int arm_reg10 = 10;
const int arm_reg11 = 11;
const int arm_reg12 = 12;
const int arm_reg13 = 13;
const int arm_reg14 = 14;
const int arm_reg15 = 15;

// some registers have alternate names
const int arm_SP = 13; // stack pointer
const int arm_LR = 14; // link register
const int arm_PC = 15; // program counter

const int arm_mode_THUMB = 0;
const int arm_mode_ARM   = 1;

#endif // register_names_h
