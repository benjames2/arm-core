#include <inc/armstate.h>
#include <inc/interrupt.h>

bool armstate_t::RIT   = false;   //using uint8_t for some reason prints junk values
bool armstate_t::EINT0 = false;

armstate_t::armstate_t(const int mem_endianness) : memory{mem_endianness}{this->ISR_FLAG = false;}

armstate_t::armstate_t(void) : memory{memory_t::little_endian}{ this->ISR_FLAG = false;}

armstate_t::armstate_t(const armstate_t& armstate) : memory(armstate.memory){
    this->cpu = armstate.cpu;
    this->ISR_FLAG = armstate.ISR_FLAG;
    std::cout << "Armstate constructor" << std::endl;
}

std::ostream& operator<<(std::ostream& os, armstate_t& armstate){

    os << armstate.cpu << std::endl;
    os << "RIT   ENABLED: " << armstate_t::RIT     << std::endl;
    os << "EINT0 ENABLED: " << armstate_t::EINT0   << std::endl;
    os << "ISR FLAG     : " << armstate.isr_flagged() << std::endl;
    os << armstate.memory << std::flush;

    return os;
}

void armstate_t::set_isr_flag(void){
    this->ISR_FLAG = true;
}

void armstate_t::clear_isr_flag(void){
    this->ISR_FLAG = false;
}

bool armstate_t::isr_flagged(void){
    return this->ISR_FLAG;
}

bool operator==(armstate_t& armstate_lhs, armstate_t& armstate_rhs){

    if (armstate_lhs.cpu != armstate_rhs.cpu)
        return false;

    if (armstate_lhs.memory != armstate_rhs.memory)
        return false;

    return true;
}

bool operator!=(armstate_t& armstate_lhs, armstate_t& armstate_rhs){
    return !(armstate_lhs == armstate_rhs);
}

bool operator<(armstate_t& armstate_lhs, armstate_t& armstate_rhs){

    return (armstate_lhs.cpu < armstate_rhs.cpu);
}

void print_armstate_diff(armstate_t& armstate_lhs, armstate_t& armstate_rhs, std::ostream& os){

    print_cpu_diff(armstate_lhs.cpu, armstate_rhs.cpu, os);
    os << "RIT   ENABLED: " << armstate_t::RIT     << std::endl;
    os << "EINT0 ENABLED: " << armstate_t::EINT0   << std::endl;
    os << "RIT   COUNTER: " << armstate_rhs.memory.load_u32(special_register_t::RICOUNTER)   << std::endl;
    os << "ISR FLAG     : " << armstate_rhs.isr_flagged() << std::endl;
    print_memory_diff(armstate_lhs.memory, armstate_rhs.memory, os);
    os << std::flush;
}