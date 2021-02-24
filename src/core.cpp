#include <inc/core.h>
#include <sstream>
#include <iomanip>

armv7_m3::armv7_m3(void) {
    // initialize all registers to zero. the PC will 
    // need to be set by the user
    for(int i = 0; i < 16; i++)
        this->reg[i].u32 = 0x00000000;

    this->cycle_count = 0;
    this->stack_mode  = armv7_m3::stack_mode_undefined;
    this->cpu_id      = 0;
}

uint64_t armv7_m3::get_cycle_count(void) const{
    return this->cycle_count;
}

// register access
armv7_m3::register_t armv7_m3::get_register(int reg) const {
    if(reg > 15 || reg < 0)
        throw std::runtime_error("armv7_m3::get_register : invalid register access. valid accesses are r(0-15)");
    return this->reg[reg]; 
}

uint32_t armv7_m3::get_register_u32(int reg) { 
    return this->get_register(reg).u32;
}

int32_t armv7_m3::get_register_i32(int reg) const {
    return this->get_register(reg).i32;
}

void armv7_m3::set_register_i32(int reg, int32_t value) {
    if(reg > 15 || reg < 0)
        throw std::runtime_error(
            "armv7_m3::set_register_i32 : invalid register "
            "access. valid accesses are r(0-15)");

    this->reg[reg].i32 = value;
}

void armv7_m3::set_register_u32(int reg, uint32_t value) {
    if(reg > 15 || reg < 0)
        throw std::runtime_error(
            "armv7_m3::set_register_u32 : invalid register "
            "access. valid accesses are r(0-15)");

    this->reg[reg].u32 = value;
}

uint32_t& armv7_m3::PC(void) { return this->reg[15].u32; }
uint32_t& armv7_m3::LR(void) { return this->reg[14].u32; }
uint32_t& armv7_m3::SP(void) { return this->reg[13].u32; }

uint32_t armv7_m3::get_PC( void) { return this->reg[15].u32; }
uint32_t armv7_m3::get_MSP(void) { return this->reg[13].u32; }
uint32_t armv7_m3::get_PSP(void) { return this->reg[13].u32; }
uint32_t armv7_m3::get_SP( void) { return this->reg[13].u32; }
uint32_t armv7_m3::get_LR( void) { return this->reg[14].u32; }

bool armv7_m3::get_APSR_N(void) { return (this->APSR & (1 << 31)) ? 1 : 0; }
bool armv7_m3::get_APSR_Z(void) { return (this->APSR & (1 << 30)) ? 1 : 0; }
bool armv7_m3::get_APSR_C(void) { return (this->APSR & (1 << 29)) ? 1 : 0; }
bool armv7_m3::get_APSR_V(void) { return (this->APSR & (1 << 28)) ? 1 : 0; }
bool armv7_m3::get_APSR_Q(void) { return (this->APSR & (1 << 27)) ? 1 : 0; }

void armv7_m3::set_APSR_N(bool b) { if(b) this->APSR |= (1 << 31); else this->APSR &= ~(1 << 31); }
void armv7_m3::set_APSR_Z(bool b) { if(b) this->APSR |= (1 << 30); else this->APSR &= ~(1 << 30); }
void armv7_m3::set_APSR_C(bool b) { if(b) this->APSR |= (1 << 29); else this->APSR &= ~(1 << 29); }
void armv7_m3::set_APSR_V(bool b) { if(b) this->APSR |= (1 << 28); else this->APSR &= ~(1 << 28); }
void armv7_m3::set_APSR_Q(bool b) { if(b) this->APSR |= (1 << 27); else this->APSR &= ~(1 << 27); }

bool armv7_m3::get_CPSR_N(void) const { return (this->CPSR & (1 << 31)) ? 1 : 0; }
bool armv7_m3::get_CPSR_Z(void) const { return (this->CPSR & (1 << 30)) ? 1 : 0; }
bool armv7_m3::get_CPSR_C(void) const { return (this->CPSR & (1 << 29)) ? 1 : 0; }
bool armv7_m3::get_CPSR_V(void) const { return (this->CPSR & (1 << 28)) ? 1 : 0; }
bool armv7_m3::get_CPSR_Q(void) const { return (this->CPSR & (1 << 27)) ? 1 : 0; }

void armv7_m3::set_CPSR_N(bool b) { if(b) this->CPSR |= (1 << 31); else this->CPSR &= ~(1 << 31); }
void armv7_m3::set_CPSR_Z(bool b) { if(b) this->CPSR |= (1 << 30); else this->CPSR &= ~(1 << 30); }
void armv7_m3::set_CPSR_C(bool b) { if(b) this->CPSR |= (1 << 29); else this->CPSR &= ~(1 << 29); }
void armv7_m3::set_CPSR_V(bool b) { if(b) this->CPSR |= (1 << 28); else this->CPSR &= ~(1 << 28); }
void armv7_m3::set_CPSR_Q(bool b) { if(b) this->CPSR |= (1 << 27); else this->CPSR &= ~(1 << 27); }

// fields specific to the CPSR
int  armv7_m3::get_CPSR_IT(void) { return ((this->CPSR >> 8) & 0b11111100) | ((this->CPSR >> 25) & 0b11); }
int  armv7_m3::get_CPSR_GE(void) { return ((this->CPSR >> 16) & 0x0F);}
int  armv7_m3::get_CPSR_M( void) { return   this->CPSR & 0x1F; }
bool armv7_m3::get_CPSR_J( void) { return ((this->CPSR >> 24) & 0x01);}
bool armv7_m3::get_CPSR_E( void) { return ((this->CPSR >> 9) & 0x01); }
bool armv7_m3::get_CPSR_A( void) { return ((this->CPSR >> 8) & 0x01); }
bool armv7_m3::get_CPSR_I( void) { return ((this->CPSR >> 7) & 0x01); }
bool armv7_m3::get_CPSR_F( void) { return ((this->CPSR >> 6) & 0x01); }
bool armv7_m3::get_CPSR_T( void) { return ((this->CPSR >> 5) & 0x01); }

// these two do exactly the same thing
void armv7_m3::set_CPSR(uint32_t cpsr) { this->CPSR = cpsr; }
void armv7_m3::set_APSR(uint32_t apsr) { this->APSR = apsr; }

uint32_t armv7_m3::get_CPSR(void) const { return this->CPSR; }
uint32_t armv7_m3::get_APSR(void) { return this->APSR; }

int  armv7_m3::get_stack_mode(void)              { return this->stack_mode; }
void armv7_m3::set_stack_mode(const int newmode) { this->stack_mode = newmode; }

std::ostream& operator<<(std::ostream& os, armv7_m3 const& cpu) {

    auto padhexnumber = [](const unsigned int number) {
        std::stringstream ss;
        ss << std::hex << number;
        
        auto s = ss.str(); 

        while (s.size() < 8)
            s = "0" + s;

        return s;
    };
    
    os << std::dec;
    os << "\n  cpu state " << cpu.cpu_id << "\n";

    for(int i = 0; i < cpu.reg.size(); ++i){

        os << "R" << std::dec << i; 
        if(i < 10)
            os << std::hex << std::setw(6) << ":" << std::setw(4) << "Ox" << padhexnumber(cpu.get_register_i32(i)) << "\n";
        else{
            if(i < 13) 
                os << std::hex << std::setw(5) << ":" << std::setw(4) << "Ox"  << padhexnumber(cpu.get_register_i32(i)) << "\n";
            if(i == 13)
                os << std::hex << "(SP):" << std::setw(4) << "Ox" << padhexnumber(cpu.get_register_i32(i)) << "\n";
            if(i == 14)
                os << std::hex << "(LR):" << std::setw(4) << "Ox" << padhexnumber(cpu.get_register_i32(i)) << "\n";
            if(i == 15)
                os << std::hex << "(PC):" << std::setw(4) << "Ox" << padhexnumber(cpu.get_register_i32(i)) << "\n";
        }
    }

    os << "xPSR" << std::setw(4) <<  ":" << std::setw(4) << "Ox" << padhexnumber(cpu.get_CPSR()) << "\n";

    os << "N    Z    C    V   Q\n";
    os << 
        cpu.get_CPSR_N() << std::setw(5) <<
        cpu.get_CPSR_Z() << std::setw(5) <<
        cpu.get_CPSR_C() << std::setw(5) <<
        cpu.get_CPSR_V() << std::setw(4) <<
        cpu.get_CPSR_Q() << "\n";
        
    os << "Cycle  :" << std::dec << std::setw(3) << cpu.get_cycle_count() << std::endl;

    return os;
}

void print_cpu_diff(armv7_m3 const& old_cpu, armv7_m3 const& new_cpu, std::ostream& os){

    auto padhexnumber = [](const unsigned int number) {
        std::stringstream ss;
        ss << std::hex << number;
        
        auto s = ss.str(); 

        while (s.size() < 8)
            s = "0" + s;

        return s;
    };

    os << std::dec;
    os << "  cpu state " << new_cpu.cpu_id << "\n";

    for(int i = 0; i < new_cpu.reg.size(); ++i){

        if(i < 10){
            if(old_cpu.get_register_i32(i) != new_cpu.get_register_i32(i)){
                os << "R" << std::dec << i;
                os << std::hex << std::setw(6) << ":" << std::setw(4) << "Ox" << padhexnumber(new_cpu.get_register_i32(i)) << std::endl;
            }
        }
        else{
            if(i < 13){
                if(old_cpu.get_register_i32(i) != new_cpu.get_register_i32(i)){
                    os << "R" << std::dec << i;
                    os << std::hex << std::setw(6) << ":" << "  Ox" << padhexnumber(new_cpu.get_register_i32(i)) << "\n";
                }
            }
            if(i == 13){
                if(old_cpu.get_register_i32(i) != new_cpu.get_register_i32(i)){
                    os << "R" << std::dec << i;
                    os << std::hex << "(SP):  Ox" << padhexnumber(new_cpu.get_register_i32(i)) << "\n";
                }
            }
            if(i == 14){
                if(old_cpu.get_register_i32(i) != new_cpu.get_register_i32(i)){
                    os << "R" << std::dec << i << "";
                    os << std::hex << "(LR):  Ox" << padhexnumber(new_cpu.get_register_i32(i)) << "\n";
                }
            }
            if(i == 15){
                if(old_cpu.get_register_i32(i) != new_cpu.get_register_i32(i)){
                    os << "R" << std::dec << i;
                    os << std::hex << "(PC):  Ox" << padhexnumber(new_cpu.get_register_i32(i)) << "\n";
                }
            }
        }
        
    }

    if(old_cpu.get_CPSR() != new_cpu.get_CPSR()){
        os << "xPSR   :  0x" << padhexnumber(new_cpu.get_CPSR()) << "\n";
        os << "N    Z    C    V   Q\n";
        os << 
            new_cpu.get_CPSR_N() << std::setw(5) <<
            new_cpu.get_CPSR_Z() << std::setw(5) <<
            new_cpu.get_CPSR_C() << std::setw(5) <<
            new_cpu.get_CPSR_V() << std::setw(4) <<
            new_cpu.get_CPSR_Q() << "\n";
        }
    
    os << "Cycle  :" << std::dec << std::setw(3) << new_cpu.get_cycle_count() << "\n\n" << std::endl;
}

bool operator==(armv7_m3 const& armcore_w, armv7_m3 const& armcore_v){

    if (armcore_w.get_cycle_count() != armcore_v.get_cycle_count())
        return false;

    if (armcore_w.get_CPSR() != armcore_v.get_CPSR())
        return false;

    for(int i = 0; i < armcore_w.reg.size(); ++i){
        if (armcore_w.get_register_i32(i) != armcore_v.get_register_i32(i))
            return false;
    }

    return true;

}

bool operator!=(armv7_m3 const& armcore_lhs, armv7_m3 const& armcore_rhs){
    return !(armcore_lhs == armcore_rhs);
}

bool operator<(armv7_m3 const& armcore_lhs, armv7_m3 const& armcore_rhs){
    return (armcore_lhs.cpu_id < armcore_rhs.cpu_id);
}