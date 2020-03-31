#include <inc/core.h>

armv7_m3::armv7_m3(void) {
    for(int i = 0; i < 16; i++)
        this->reg[i].u32 = 0x00000000;

    this->current_mode = -1;
    this->cycle_count = 0;
}

armv7_m3::register_t armv7_m3::get_register(int reg) { return this->reg[reg]; }

uint32_t armv7_m3::get_register_u32(int reg) { return this->reg[reg].u32; }

int32_t armv7_m3::get_register_i32(int reg) { return this->reg[reg].i32; }

uint32_t armv7_m3::get_PC(void) { return this->reg[15].u32; }

uint32_t armv7_m3::get_MSP(void) { return this->reg[13].u32; }

uint32_t armv7_m3::get_PSP(void) { return this->reg[13].u32; }

uint32_t armv7_m3::get_SP(void) { return this->reg[13].u32; }

uint32_t armv7_m3::get_LR(void) { return this->reg[14].u32; }

bool armv7_m3::get_APSR_N(void) { return (this->APSR & (1 << 31)) ? 1 : 0; }

bool armv7_m3::get_APSR_Z(void) { return (this->APSR & (1 << 30)) ? 1 : 0; }

bool armv7_m3::get_APSR_C(void) { return (this->APSR & (1 << 29)) ? 1 : 0; }

bool armv7_m3::get_APSR_V(void) { return (this->APSR & (1 << 28)) ? 1 : 0; }

bool armv7_m3::get_APSR_Q(void) { return (this->APSR & (1 << 27)) ? 1 : 0; }

int  armv7_m3::get_current_mode(void) {
    return this->current_mode;
}

void armv7_m3::set_current_mode(const int m) {
    switch(m) {
        case 0: case 1:
            this->current_mode = m;
            break;
        default:
            throw std::runtime_error("armv7_m3::set_current_mode : invalid mode selected");
    }
}


