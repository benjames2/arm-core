#ifndef inst_mov_shifted_register_h
#define inst_mov_shifted_register_h

#include <arm_cpu.h>
#include <memory_pool.h>
#include <arm_instruction_asm.h>

namespace inst {
namespace thumb {

    // decode a given ARM thumb instruction
    address_t decode(arm_cpu& cpu, memory_t& mem, uint32_t inst);

    address_t move_shifted_register( arm_cpu& cpu, memory_t& mem, uint32_t inst );

    address_t add_subtract( arm_cpu& cpu, memory_t& mem, uint32_t inst );

    address_t mcas_imm( arm_cpu& cpu, memory_t& mem, uint32_t inst );

    // many of the operations in this instruction are not defined
    // yet. it will likely throw an exception
    address_t alu_operations( arm_cpu& cpu, memory_t& mem, uint32_t inst );

    // none of the compare instructions are implemented
    address_t hi_reg_ops_brnch_exch( arm_cpu& cpu, memory_t& mem, uint32_t inst );

    address_t pc_relative_load( arm_cpu& cpu, memory_t& mem, uint32_t inst );

    address_t load_store_w_register_offset( arm_cpu& cpu, memory_t& mem, uint32_t inst );

    address_t load_store_s_ext_byte_halfword( arm_cpu& cpu, memory_t& mem, uint32_t inst );

    address_t load_store_w_imm_offset( arm_cpu& cpu, memory_t& mem, uint32_t inst );

    address_t load_store_halfword( arm_cpu& cpu, memory_t& mem, uint32_t inst );

    address_t sp_relative_load_store( arm_cpu& cpu, memory_t& mem, uint32_t inst );

    address_t load_address( arm_cpu& cpu, memory_t& mem, uint32_t inst );

    address_t add_offset_sp( arm_cpu& cpu, memory_t& mem, uint32_t inst );

    address_t push_pop_regs( arm_cpu& cpu, memory_t& mem, uint32_t inst );

    address_t multiple_load_store( arm_cpu& cpu, memory_t& mem, uint32_t inst );

    address_t conditional_branch( arm_cpu& cpu, memory_t& mem, uint32_t inst );

    address_t software_interrupt( arm_cpu& cpu, memory_t& mem, uint32_t inst );

} // thumb

address_t execute_instruction( arm_cpu& cpu, memory_t& mem );

} // inst

#endif // inst_mov_shifted_register_h
