#ifndef inst_mov_shifted_register_h
#define inst_mov_shifted_register_h

#include <arm_cpu.h>
#include <memory_pool.h>
#include <arm_instruction_asm.h>

// MSR : move shifted register

namespace inst::thumb {

    int move_shifted_register(arm_cpu& cpu, memory_t& mem, uint32_t inst);

    int add_subtract(arm_cpu& cpu, memory_t& mem, uint32_t inst);

    int mcas_imm(arm_cpu& cpu, memory_t& mem, uint32_t inst);

}

#endif // inst_mov_shifted_register_h