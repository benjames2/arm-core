#include <iostream>
#include <inc/core.h>
#include <inc/memory_pool.h>
#include <inc/exceptions.h>
#include <inc/stack_operations.h>

address32_t push_word(armstate_t& armstate, address32_t addr, uint32_t word) {
    
    switch(armstate.cpu.get_stack_mode()) {
        case armv7_m3::stack_mode_FullAscending:
            addr += 4;
            armstate.memory.store_u32(addr, word);
            break;
        case armv7_m3::stack_mode_FullDescending:
            addr -= 4;
            armstate.memory.store_u32(addr, word);
            break;
        case armv7_m3::stack_mode_EmptyAscending:
            armstate.memory.store_u32(addr, word);
            addr += 4;
            break;
        case armv7_m3::stack_mode_EmptyDescending:
            armstate.memory.store_u32(addr, word);
            addr -= 4;
        default:
            throw StackError("when calling push_word(), invalid stack mode");
    }

    return addr;
}

address32_t push_word(armstate_t& armstate, address32_t addr, int32_t word) {
    
     switch(armstate.cpu.get_stack_mode()) {
        case armv7_m3::stack_mode_FullAscending:
            addr += 4;
            armstate.memory.store_i32(addr, word);
            break;
        case armv7_m3::stack_mode_FullDescending:
            addr -= 4;
            armstate.memory.store_i32(addr, word);
            break;
        case armv7_m3::stack_mode_EmptyAscending:
            armstate.memory.store_i32(addr, word);
            addr += 4;
            break;
        case armv7_m3::stack_mode_EmptyDescending:
            armstate.memory.store_i32(addr, word);
            addr -= 4;
        default:
            throw StackError("when calling push_word(), invalid stack mode");
    }

    return addr;
}

address32_t pop_word(armstate_t& armstate, address32_t addr, int regn) {
    
    uint32_t val = 0;

    switch(armstate.cpu.get_stack_mode()) {
        case armv7_m3::stack_mode_FullAscending:
            val = armstate.memory.load_u32(addr);
            addr -= 4;
            break;
        case armv7_m3::stack_mode_FullDescending:
            val = armstate.memory.load_u32(addr);
            addr += 4;
            break;
        case armv7_m3::stack_mode_EmptyAscending:
            addr -= 4;
            val = armstate.memory.load_u32(addr);
            break;
        case armv7_m3::stack_mode_EmptyDescending:
            addr += 4;
            val = armstate.memory.load_u32(addr);
            break;
        default:
            throw StackError("when calling pop_word(), invalid stack mode");
    }

    if (regn == 16)
        armstate.cpu.set_CPSR(val);
    else
        armstate.cpu.set_register_u32(regn, val);
    
    return addr;
}

