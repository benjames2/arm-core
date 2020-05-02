#include <iostream>
#include <inc/core.h>
#include <inc/memory_pool.h>

void push_word(armv7_m3& cpu, memory_t& memory, uint32_t w) {

    switch(cpu.get_stack_mode()) {
        case armv7_m3::stack_mode_IncrementBefore:
            cpu.SP() += 4;
            memory.store_u32(cpu.SP(), w);
            break;
        case armv7_m3::stack_mode_DecrementBefore:
            cpu.SP() -= 4;
            memory.store_u32(cpu.SP(), w);
            break;
        case armv7_m3::stack_mode_IncrementAfter:
            memory.store_u32(cpu.SP(), w);
            cpu.SP() += 4;
            break;
        case armv7_m3::stack_mode_DecrementAfter:
            memory.store_u32(cpu.SP(), w);
            cpu.SP() -= 4;
            break;
        default:
            throw std::runtime_error("armv7_m3::push_word : invalid stack mode specified");
    }

}

uint32_t pop_word(armv7_m3& cpu, memory_t& memory) {

    uint32_t w = 0;

    switch(cpu.get_stack_mode()) {
        case armv7_m3::stack_mode_IncrementBefore:
            cpu.SP() += 4;
            w = memory.load_u32(cpu.SP());
            break;
        case armv7_m3::stack_mode_DecrementBefore:
            cpu.SP() -= 4;
            w = memory.load_u32(cpu.SP());
            break;
        case armv7_m3::stack_mode_IncrementAfter:
            w = memory.load_u32(cpu.SP());
            cpu.SP() += 4;
            break;
        case armv7_m3::stack_mode_DecrementAfter:
            w = memory.load_u32(cpu.SP());
            cpu.SP() -= 4;
            break;
        default:
            throw std::runtime_error("armv7_m3::push_word : invalid stack mode specified");
    }

    return w;
}

void push_word(armv7_m3& cpu, memory_t& memory, int32_t w) {
    
    union {
        int32_t i32;
        uint32_t u32;
    };
    
    i32 = w;

    // call the other push_word function
    push_word(cpu, memory, u32);
}

void push_register(armv7_m3& cpu, memory_t& memory, int regn) {
    push_word(cpu, memory, cpu.get_register_u32(regn));
}

uint32_t push_word(armv7_m3& cpu, memory_t& memory, address_t addr, uint32_t w) {

    

}

uint32_t push_word(armv7_m3& cpu, memory_t& memory, address_t addr, int32_t w) {
    union {
        int32_t i32;
        uint32_t u32;
    };

    i32 = w;
    return push_word(cpu, memory, addr, u32);
}
