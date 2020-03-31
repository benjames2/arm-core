#pragma once

#include <array>

struct armv7_m3 {
public:

    union register_t {
        int      i32;
        uint32_t u32;
    };

    // armv7-m needs to execute in one of these modes. they are 
    // both THUMB2 but one is 16-bit and the other is 32-bit
    static const int mode_16 = 0;
    static const int mode_32 = 1;

// private:
public:

    std::array<register_t, 16> reg;
    union {
        uint32_t APSR; // application program status register
        uint32_t CPSR; // current program status register
    };
    
    int current_mode; // mode_16 or mode_32;
    uint64_t cycle_count;

public:

    // initialize all regs to zero. mode is set to -1 so as to be 
    // undefined. user MUST explicitly set it to either 0 (16 bit mode) 
    // or 1 (32 bit mode)
    armv7_m3(void);

    // register data fetching methods
    uint32_t   get_register_u32(int reg);
    int32_t    get_register_i32(int reg);
    register_t get_register(int reg);

    // get specific registers
    uint32_t get_PC(void);
    uint32_t get_MSP(void);
    uint32_t get_PSP(void);
    uint32_t get_SP(void);
    uint32_t get_LR(void);

    uint32_t get_APSR(void); // return the entire flags register

    bool get_APSR_N(void); // get Negative flag
    bool get_APSR_Z(void); // get Zero flag
    bool get_APSR_C(void); // get Carry flag
    bool get_APSR_V(void); // get oVerflow flag
    bool get_APSR_Q(void); // get saturate flag

    void set_APSR_N(bool b); // set Negative flag
    void set_APSR_Z(bool b); // set Zero flag
    void set_APSR_C(bool b); // set Carry flag
    void set_APSR_V(bool b); // set oVerflow flag
    void set_APSR_Q(bool b); // set saturate flag

    uint32_t get_CPSR(void);
    

    int get_current_mode(void);
    void set_current_mode(const int m);

};
