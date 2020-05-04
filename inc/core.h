#pragma once

#include <iostream>
#include <array>

struct armv7_m3 {
public:

    union register_t {
        int      i32;
        uint32_t u32;
    };

    const static int stack_mode_undefined = -1;
    const static int stack_mode_FullAscending   = 0;
    const static int stack_mode_FullDescending  = 1;
    const static int stack_mode_EmptyAscending  = 2;
    const static int stack_mode_EmptyDescending = 3;

//private:
public:

    std::array<register_t, 16> reg;

    union {
        uint32_t APSR; // application program status register
        uint32_t CPSR; // current program status register
    };
    
    int stack_mode;
    uint64_t cycle_count;
    int64_t  cpu_id;

public:

    armv7_m3(void);

    int get_stack_mode(void);
    void set_stack_mode(const int newmode);
    uint64_t get_cycle_count(void);

    uint32_t& PC(void);
    uint32_t& SP(void);
    uint32_t& LR(void);

    // register data fetching methods
    uint32_t   get_register_u32(int reg);
    int32_t    get_register_i32(int reg);
    register_t get_register(    int reg);

    void set_register_i32(int reg, int32_t value);
    void set_register_u32(int reg, uint32_t value);

    // get specific registers
    uint32_t get_PC( void);
    uint32_t get_MSP(void);
    uint32_t get_PSP(void);
    uint32_t get_SP( void);
    uint32_t get_LR( void);

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
    void set_CPSR(uint32_t cpsr);
    void set_APSR(uint32_t apsr);

    // ALU flags are exactly the same as for the APSR
    bool get_CPSR_N(void); // get Negative flag
    bool get_CPSR_Z(void); // get Zero flag
    bool get_CPSR_C(void); // get Carry flag
    bool get_CPSR_V(void); // get oVerflow flag
    bool get_CPSR_Q(void); // get saturate flag

    void set_CPSR_N(bool b); // set Negative flag
    void set_CPSR_Z(bool b); // set Zero flag
    void set_CPSR_C(bool b); // set Carry flag
    void set_CPSR_V(bool b); // set oVerflow flag
    void set_CPSR_Q(bool b); // set saturate flag

    // bits specific to the CPSR
    int  get_CPSR_IT(void); // If-Then state bits (7:0)
    int  get_CPSR_GE(void); // Greater-then Equal flags (3:0)
    bool get_CPSR_J( void); // Jazelle (some weird JVM thing, we'll never need it)
    bool get_CPSR_E( void); // endianness bit
    bool get_CPSR_A( void); // asynchronous abort bit
    bool get_CPSR_I( void); // IRQ mask bit
    bool get_CPSR_F( void); // FIRQ mask bit
    bool get_CPSR_T( void); // THUMB execution state bit. for us, we'll always be in THUMB state
    int  get_CPSR_M( void); // Mode field (4:0)

    friend std::ostream& operator<<(std::ostream& os, armv7_m3& cpu);

};

