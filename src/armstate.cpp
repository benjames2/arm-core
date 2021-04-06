#include <inc/armstate.h>


uint32_t armstate_t::RIT   = 0;   //using uint8_t for some reason prints junk values
uint32_t armstate_t::EINT0 = 0;

armstate_t::armstate_t(const int mem_endianness) : memory{mem_endianness}{}

armstate_t::armstate_t(void) : memory{memory_t::little_endian}{}

std::ostream& operator<<(std::ostream& os, armstate_t& armstate){

    os << armstate.cpu << std::endl;
    os << "RIT   ENABLED: " << armstate_t::RIT     << std::endl;
    os << "EINT0 ENABLED: " << armstate_t::EINT0   << std::endl;
    os << armstate.memory << std::flush;

    return os;
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
    print_memory_diff(armstate_lhs.memory, armstate_rhs.memory, os);
    os << std::flush;
}