#include <inc/interrupt.h>
#include <map>



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

void enable_rit(armstate_t& armstate){

    const uint32_t rit_enable_value = 0x2000000;
    if(armstate.memory.load_u32(special_register_t::NVIC_ENABLE0) == rit_enable_value)
        armstate_t::RIT = 1;
}

bool rit_enabled(){
    return armstate_t::RIT;
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

armstate_t rit_handler (armstate_t& armstate){
    auto new_armstate = armstate;
    enable_rit(new_armstate);

    if (rit_enabled()){
        rit_procedure(new_armstate);
        if(rit_triggered(new_armstate))
            new_armstate = call_isr(new_armstate, interrupt_id::RIT);
    }
    return new_armstate;
}

armstate_t call_isr(armstate_t& armstate, interrupt_id id){

    auto new_armstate = armstate;
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


    const auto rit_exception_number = 0x2D;
    auto cpsr = armstate.cpu.get_CPSR() | rit_exception_number;

    new_armstate.cpu.set_register_u32(13, stack_pointer);
    new_armstate.cpu.set_CPSR(cpsr);


    return new_armstate;
}
