#include <inc/armstate.h>

armstate_t::armstate_t(const int mem_endianness) : memory{mem_endianness}{}

armstate_t::armstate_t(void) : memory{memory_t::little_endian}{}

std::ostream& operator<<(std::ostream& os, armstate_t& armstate){

    os << armstate.cpu << armstate.memory << std::flush;

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