#include <inc/core.h>

armv7_m3::armv7_m3(void) {
    // initialize all registers to zero. the PC will 
    // need to be set by the user
    for(int i = 0; i < 16; i++)
        this->reg[i].u32 = 0x00000000;

    this->cycle_count = 0;
}

// register access
armv7_m3::register_t armv7_m3::get_register(int reg) { return this->reg[reg]; }
uint32_t armv7_m3::get_register_u32(int reg) { return this->reg[reg].u32; }
int32_t armv7_m3::get_register_i32(int reg) { return this->reg[reg].i32; }

uint32_t armv7_m3::get_PC(void)  { return this->reg[15].u32; }
uint32_t armv7_m3::get_MSP(void) { return this->reg[13].u32; }
uint32_t armv7_m3::get_PSP(void) { return this->reg[13].u32; }
uint32_t armv7_m3::get_SP(void)  { return this->reg[13].u32; }
uint32_t armv7_m3::get_LR(void)  { return this->reg[14].u32; }

bool armv7_m3::get_APSR_N(void) { return (this->APSR & (1 << 31)) ? 1 : 0; }
bool armv7_m3::get_APSR_Z(void) { return (this->APSR & (1 << 30)) ? 1 : 0; }
bool armv7_m3::get_APSR_C(void) { return (this->APSR & (1 << 29)) ? 1 : 0; }
bool armv7_m3::get_APSR_V(void) { return (this->APSR & (1 << 28)) ? 1 : 0; }
bool armv7_m3::get_APSR_Q(void) { return (this->APSR & (1 << 27)) ? 1 : 0; }

bool armv7_m3::get_CPSR_N(void) { return (this->APSR & (1 << 31)) ? 1 : 0; }
bool armv7_m3::get_CPSR_Z(void) { return (this->APSR & (1 << 30)) ? 1 : 0; }
bool armv7_m3::get_CPSR_C(void) { return (this->APSR & (1 << 29)) ? 1 : 0; }
bool armv7_m3::get_CPSR_V(void) { return (this->APSR & (1 << 28)) ? 1 : 0; }
bool armv7_m3::get_CPSR_Q(void) { return (this->APSR & (1 << 27)) ? 1 : 0; }

// fields specific to the CPSR
int armv7_m3::get_CPSR_IT(void) { return ((this->CPSR >> 8) & 0b11111100) | ((this->CPSR >> 25) & 0b11); }
int armv7_m3::get_CPSR_GE(void) { return ((this->CPSR >> 16) & 0x0F); }
int armv7_m3::get_CPSR_M(void)  { return this->CPSR & 0x1F; }
bool armv7_m3::get_CPSR_J(void) { return ((this->CPSR >> 24) & 0x01); }
bool armv7_m3::get_CPSR_E(void) { return ((this->CPSR >> 9) & 0x01); }
bool armv7_m3::get_CPSR_A(void) { return ((this->CPSR >> 8) & 0x01); }
bool armv7_m3::get_CPSR_I(void) { return ((this->CPSR >> 7) & 0x01); }
bool armv7_m3::get_CPSR_F(void) { return ((this->CPSR >> 6) & 0x01); }
bool armv7_m3::get_CPSR_T(void) { return ((this->CPSR >> 5) & 0x01); }


