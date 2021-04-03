#include <inc/interrupt.h>



void rit_enable(armstate_t& armstate){

    const uint32_t rit_enable_value = 0x2000000;
    if(armstate.memory.load_u32(special_register_t::NVIC_ENABLE0) == rit_enable_value)
        armstate_t::RIT = 1;
}

void rit_procedure(armstate_t& armstate){
    if(armstate_t::RIT == 1){ //This can/should be done in the execute stage. When RIT is enabled, the counter should increment after execution of each instruction
        auto rit_counter = armstate.memory.load_u32(special_register_t::RICOUNTER);
        rit_counter++;                        
        armstate.memory.store_u32(special_register_t::RICOUNTER, rit_counter);
    }
}

bool rit_triggered(armstate_t& armstate){

    auto ritcounter_value = armstate.memory.load_u32(special_register_t::RICOUNTER);
    auto compare_value    = armstate.memory.load_u32(special_register_t::RICOMPVAL);
    
    if(ritcounter_value == compare_value)
        return true;
    else 
        return false;
}

armstate_t rit_handler (armstate_t& armstate){
    rit_enable(armstate);
    rit_procedure(armstate);

    if(rit_triggered(armstate))
        auto new_armstate = call_isr(armstate);
}

armstate_t call_isr(armstate_t& armstate){

    auto new_armstate = armstate;

    new_armstate.cpu.set_stack_mode(armv7_m3::stack_mode_FullDescending);



    
}
