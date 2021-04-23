#include <inc/interrupt.h>
#include <map>


uint32_t special_flag_t::ISR_FLAG = 0;

uint32_t get_exception_number(interrupt_id id){

    const std::map <interrupt_id, uint32_t> lut = {

        {interrupt_id::WDT,      16},
        {interrupt_id::TIMER0,   17},
        {interrupt_id::TIMER1,   18},
        {interrupt_id::TIMER2,   19},
        {interrupt_id::TIMER3,   20},
        {interrupt_id::UART0,    21},
        {interrupt_id::UART1,    22},
        {interrupt_id::UART2,    23},
        {interrupt_id::UART3,    24},
        {interrupt_id::PWM1,     25},
        {interrupt_id::I2C0,     26},
        {interrupt_id::I2C1,     27},
        {interrupt_id::I2C2,     28},
        {interrupt_id::SPI,      29},
        {interrupt_id::SPP0,     30},
        {interrupt_id::SSP1,     31},
        {interrupt_id::PLL0,     32},
        {interrupt_id::RTC,      33},
        {interrupt_id::EINT0,    34},
        {interrupt_id::EINT1,    35},
        {interrupt_id::EINT2,    36},
        {interrupt_id::EINT3,    37},
        {interrupt_id::ADC,      38},
        {interrupt_id::BOD,      39},
        {interrupt_id::USB,      40},
        {interrupt_id::CAN,      41},
        {interrupt_id::GPDMA,    42},
        {interrupt_id::I2S,      43},
        {interrupt_id::ETHERNET, 44},
        {interrupt_id::RIT,      45},
    };

    auto iter = lut.find(id);
    if(iter == lut.end())
        throw std::runtime_error("Invalid Interrupt ID number or ID number not included in look up table(lut)");
    else  
        return iter->second;
}

void enable_interrupt(armstate_t& armstate){

    //const uint32_t rit_enable_value = 0x20000000;
    const uint32_t rit_enable_bit = 29;
    if(armstate.memory.load_u32(special_register_t::NVIC_ENABLE0) & (1 << rit_enable_bit))
        armstate_t::RIT = true;

    const uint32_t eint0_enable_bit = 18;
    if(armstate.memory.load_u32(special_register_t::NVIC_ENABLE0) & (1 << eint0_enable_bit))
        armstate_t::EINT0 = true;
}

bool rit_enabled(){
    return armstate_t::RIT;
}

bool eint0_enabled(){
    return armstate_t::EINT0;
}

void reset_rit_counter(armstate_t& armstate){
    armstate.memory.store_u32(special_register_t::RICOUNTER, 0x2);
}

void rit_procedure(armstate_t& armstate){
    //This can/should be done in the execute stage. When RIT is enabled, the counter should increment after execution of each instruction
    auto rit_counter = armstate.memory.load_u32(special_register_t::RICOUNTER);
    rit_counter++;                        
    armstate.memory.store_u32(special_register_t::RICOUNTER, rit_counter);

}

bool rit_triggered(armstate_t& armstate){

    auto ritcounter_value = armstate.memory.load_u32(special_register_t::RICOUNTER);
    auto compare_value    = armstate.memory.load_u32(special_register_t::RICOMPVAL);
    
    return (ritcounter_value == compare_value);
     
}

armstate_t rit_handler (armstate_t& armstate, std::map<uint32_t, address32_t>& vector_table){
    auto new_armstate = armstate;
    enable_interrupt(new_armstate);

    if (rit_enabled()){
        rit_procedure(new_armstate);
        if(rit_triggered(new_armstate)){
            reset_rit_counter(new_armstate);
            new_armstate = call_isr(new_armstate, interrupt_id::RIT, vector_table);
        }
    }
    return new_armstate;
}

std::vector<armstate_t> eint0_handler(armstate_t& armstate, std::map<uint32_t, address32_t>& vector_table){

    std::vector<armstate_t> successor_states = {armstate};
    //std::cout << "vector size before push " << successor_states.size() << std::endl ;
 
    auto new_armstate = armstate;

    if(!armstate_t::EINT0)
        enable_interrupt(new_armstate);

    if(eint0_enabled()){
        if(!new_armstate.isr_flagged()){
            //We asssume interrupt is always triggered when enabled 
            new_armstate = call_isr(new_armstate, interrupt_id::EINT0, vector_table);
            successor_states.push_back(new_armstate);
        }
    }
    return successor_states;
}

armstate_t call_isr(armstate_t& armstate, interrupt_id exc_id,  std::map<uint32_t, address32_t>& vector_table){

    std::cout << "\ncalling isr " << std::endl; 
    auto new_armstate = armstate;
    new_armstate.set_isr_flag();
    std::cout << "ISR " << new_armstate.isr_flagged() << std::endl;
    new_armstate.cpu.set_stack_mode(armv7_m3::stack_mode_FullDescending);
    auto stack_pointer = armstate.cpu.SP();

    auto R0  = armstate.cpu.get_register_u32(0);
    auto R1  = armstate.cpu.get_register_u32(1);
    auto R2  = armstate.cpu.get_register_u32(2);
    auto R3  = armstate.cpu.get_register_u32(3);
    auto R12 = armstate.cpu.get_register_u32(12);

    stack_pointer = push_word(new_armstate, stack_pointer, armstate.cpu.get_CPSR());
    stack_pointer = push_word(new_armstate, stack_pointer, armstate.cpu.PC());
    stack_pointer = push_word(new_armstate, stack_pointer, armstate.cpu.LR());
    stack_pointer = push_word(new_armstate, stack_pointer, R12);
    stack_pointer = push_word(new_armstate, stack_pointer, R3);
    stack_pointer = push_word(new_armstate, stack_pointer, R2);
    stack_pointer = push_word(new_armstate, stack_pointer, R1);
    stack_pointer = push_word(new_armstate, stack_pointer, R0);


    auto exc_number = get_exception_number(exc_id);
    auto iter       = vector_table.find(exc_number);

    if(iter == vector_table.end())
        throw std::runtime_error("Vector Table is either empty or does not contain the exception number");

    auto isr_addr   = iter->second;  
    auto cpsr       = armstate.cpu.get_CPSR() | exc_number;

    const uint32_t EXC_RETURN = 0xFFFFFFF9;

    new_armstate.cpu.set_register_u32(13, stack_pointer);
    new_armstate.cpu.set_register_u32(14, EXC_RETURN);
    new_armstate.cpu.set_register_u32(15, isr_addr);
    new_armstate.cpu.set_CPSR(cpsr);
    std::cout << "ISR call end " << new_armstate.isr_flagged() << std::endl;
    return new_armstate;
}

armstate_t exit_isr(const armstate_t& armstate){

    auto new_armstate = armstate;
    
    const uint32_t EXC_RETURN = 0xFFFFFFF9;

    if(new_armstate.cpu.PC() == EXC_RETURN){
        std::cout << "exiting isr " << std::endl;
        new_armstate.cpu.set_stack_mode(armv7_m3::stack_mode_FullDescending);
        auto stack_pointer = new_armstate.cpu.SP();
        for(int regnum = 0; regnum < 17; regnum++){
            if ((regnum >= 4 && regnum <= 11) || regnum == 13)
                continue;
            stack_pointer = pop_word(new_armstate, stack_pointer, regnum);
        }

        new_armstate.cpu.set_register_u32(13, stack_pointer);
        special_flag_t::ISR_FLAG = 0;
        new_armstate.clear_isr_flag();
    }
    
    return new_armstate;
}

armstate_t interrupt_handler(armstate_t& armstate, std::map<uint32_t, address32_t>& vector_table){
    //auto new_armstate = armstate;
    std::cout << " before eint0 handler" << std::endl;
    auto successor_states = eint0_handler(armstate, vector_table);
    std::cout << " after  eint0 handler" << std::endl;

    std::cout << "vector size " << successor_states.size() << std::endl;
    //if(successor_states.size() == 2){
      //  print_cpu_pair_diff(successor_states.front().cpu, successor_states.back().cpu, std::cout);
    //}
    //new_armstate      = rit_handler(armstate, vector_table);
    
    auto  new_armstate  = exit_isr(successor_states.front());
    return new_armstate;
}
