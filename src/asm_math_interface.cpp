#include <inc/asm_math_interface.h>

bool results_t::get_x86_flag_Carry(void) { return (this->flags & 0x0001) ? 1 : 0; }
bool results_t::get_x86_flag_Par(void) { return (this->flags & 0x0004) ? 1 : 0; }
bool results_t::get_x86_flag_Adj(void) { return (this->flags & 0x0010) ? 1 : 0; }
bool results_t::get_x86_flag_Zero(void) { return (this->flags & 0x0040) ? 1 : 0; }
bool results_t::get_x86_flag_Sign(void) { return (this->flags & 0x0080) ? 1 : 0; }
bool results_t::get_x86_flag_Ov(void) { return (this->flags & 0x0800) ? 1 : 0; }
