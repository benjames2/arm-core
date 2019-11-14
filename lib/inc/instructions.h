#ifndef inst_mov_shifted_register_h
#define inst_mov_shifted_register_h

#include <arm_cpu.h>
#include <memory_pool.h>
#include <arm_instruction_asm.h>

// MSR : move shifted register

namespace inst {
namespace thumb {

    address_t move_shifted_register(arm_cpu& cpu, memory_t& mem, uint32_t inst);

    address_t add_subtract(arm_cpu& cpu, memory_t& mem, uint32_t inst);

    address_t mcas_imm(arm_cpu& cpu, memory_t& mem, uint32_t inst);

} // thumb
} // inst

#endif // inst_mov_shifted_register_h