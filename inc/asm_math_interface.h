#pragma once

struct results_t {
    int flags;

    union {
        unsigned int u32;
        signed int i32;
    };



};

const int x86_asm_ADD = 0;
const int x86_asm_SUB = 1;
const int x86_asm_ADC = 2;
const int x86_asm_SBB = 3;

// assembly routines with declared C interfaces
extern "C" {

    // unreliable way to get the flags register. dont use but its still there
    int get_flags_register(void); 
    
    // a way to specify one of many operations that need to be performed 
    // in assembly. this is used as part of a wrapper that sets ARM cpu flags appropriately
    const char* gp_operation(results_t* ptr, int a, int b, int carry, int op);

}

// this is STRICTLY FOR PRINTING x86_64 FLAGS, NOT ARM CPU FLAGS
void print_flag_results(int flags);
