#include <arm_cpu.h>

arm_cpu(void);

int32_t arm_cpu::get_register_int( const int reg ) {

}

uint32_t arm_cpu::get_register_uint( const int reg ) {

}

float arm_cpu::get_register_fp( const int reg ) {

}

uint32_t arm_cpu::get_program_status_register(void) {

}

// flag get functions
bool arm_cpu::get_flag_negative(void) { return (this->APSR >> 30) & 0x01; }
bool arm_cpu::get_flag_zero(void)     { return (this->APSR >> 29) & 0x01; }
bool arm_cpu::get_flag_carry(void)    { return (this->APSR >> 28) & 0x01; }
bool arm_cpu::get_flag_overflow(void) { return (this->APSR >> 27) & 0x01; }
bool arm_cpu::get_flag_saturate(void) { return (this->APSR >> 26) & 0x01; }

// flag set functions
void arm_cpu::set_flag_negative( bool val ) { val ? ( this->APSR |= (1 << 31) ) : ( this->APSR &= ~(1 << 31) ); }
void arm_cpu::set_flag_zero( bool val )     { val ? ( this->APSR |= (1 << 30) ) : ( this->APSR &= ~(1 << 30) ); }
void arm_cpu::set_flag_carry( bool val )    { val ? ( this->APSR |= (1 << 29) ) : ( this->APSR &= ~(1 << 29) ); }
void arm_cpu::set_flag_overflow( bool val ) { val ? ( this->APSR |= (1 << 28) ) : ( this->APSR &= ~(1 << 28) ); }
void arm_cpu::set_flag_saturate( bool val ) { val ? ( this->APSR |= (1 << 27) ) : ( this->APSR &= ~(1 << 27) ); }
