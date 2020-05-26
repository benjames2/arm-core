#include <iostream>
#include <inc/core.h>
#include <inc/memory_pool.h>
#include <inc/exceptions.h>

address_t push_word(armv7_m3& cpu, memory_t& memory, address_t addr, uint32_t word) {
    
    switch(cpu.get_stack_mode()) {
        case armv7_m3::stack_mode_FullAscending:
            addr += 4;
            memory.store_u32(addr, word);
            break;
        case armv7_m3::stack_mode_FullDescending:
            addr -= 4;
            memory.store_u32(addr, word);
            break;
        case armv7_m3::stack_mode_EmptyAscending:
            memory.store_u32(addr, word);
            addr += 4;
            break;
        case armv7_m3::stack_mode_EmptyDescending:
            memory.store_u32(addr, word);
            addr -= 4;
        default:
            throw StackError("when calling push_word(), invalid stack mode");
    }

    return addr;
}

address_t push_word(armv7_m3& cpu, memory_t& memory, address_t addr, int32_t word) {
    
     switch(cpu.get_stack_mode()) {
        case armv7_m3::stack_mode_FullAscending:
            addr += 4;
            memory.store_i32(addr, word);
            break;
        case armv7_m3::stack_mode_FullDescending:
            addr -= 4;
            memory.store_i32(addr, word);
            break;
        case armv7_m3::stack_mode_EmptyAscending:
            memory.store_i32(addr, word);
            addr += 4;
            break;
        case armv7_m3::stack_mode_EmptyDescending:
            memory.store_i32(addr, word);
            addr -= 4;
        default:
            throw StackError("when calling push_word(), invalid stack mode");
    }

    return addr;
}

address_t pop_word(armv7_m3& cpu, memory_t& memory, address_t addr, int regn) {
    
    uint32_t val = 0;

    switch(cpu.get_stack_mode()) {
        case armv7_m3::stack_mode_FullAscending:
            val = memory.load_u32(addr);
            addr -= 4;
            break;
        case armv7_m3::stack_mode_FullDescending:
            val = memory.load_u32(addr);
            addr += 4;
            break;
        case armv7_m3::stack_mode_EmptyAscending:
            addr -= 4;
            val = memory.load_u32(addr);
            break;
        case armv7_m3::stack_mode_EmptyDescending:
            addr += 4;
            val = memory.load_u32(addr);
            break;
        default:
            throw StackError("when calling pop_word(), invalid stack mode");
    }

    cpu.set_register_u32(regn, val);
    return addr;
}

