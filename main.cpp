#include <iostream>
#include <inc/core.h>
#include <inc/memory_pool.h>
#include <inc/decode_structure.h>
#include <inc/decode_16.h>
#include <inc/decode_32.h>
#include <inc/decode.h>
#include <inc/asm_math_interface.h>
#include <inc/fetch.h>
#include <inc/execute.h>

#include "main.h"

using namespace std;

// for the assembly portion to work properly with the rest of the 
// system, certain alignment requirements must be met:
static_assert(sizeof(results_t) == 8,          "sizeof results_t incorrect (should be 8)");
static_assert(offsetof(results_t, flags) == 0, "alignment of .flags in results_t incorrect (should be 0)");
static_assert(offsetof(results_t, u32) == 4,   "alignment of .u32 in results_t incorrect (should be 4)");
static_assert(offsetof(results_t, i32) == 4,   "alignment of .i32 in results_t incorrect (should be 4)");

int main(int argc, char* argv[]) {

    //test_decode_fns("test/testfile.branch.txt");
    //test_decode_fns("test/testfile.bottom.txt");
    //test_decode_fns("test/testfile.txt");
    //test_32b_decode("test/testfile32b.txt");
    //std::cout << "INSTRUCTION TESTS PASSED\n\n" << std::flush;


    memory_t mem(memory_t::little_endian);

    for(auto cptr : { "test/input_rit/assembly-code.txt", "test/input_rit/memory.txt" }) {
        load_memory_file(cptr, mem);
        cout << mem << std::endl;
    }

    // loading nvic file to get starting address for machine code
    armv7_m3 cpu;
    load_nvic_file("test/input_rit/nvic.txt", cpu);
    cout << cpu << "\n";


    for(address_t addr = 0x00000220; addr <= 0x000002c6;) {

        switch(addr){
            case 0x00000256: 
            case 0x00000258:
            case 0x0000025a:
            case 0x0000025c:
            case 0x0000025e:
            case 0x00000260: 
            case 0x00000262:
            case 0x00000264:
            case 0x00000266:
            case 0x00000268:
            case 0x0000026a: 
            case 0x0000026c:
            case 0x0000026e:
                addr += 2; continue;
        }

        auto inst_data   = fetch(mem, addr, true);
        
        if(inst_data.type == fetched_instruction_t::t16) {
            try {
                auto dec_inst = decode_16bit_instruction(addr, inst_data.in);
                cout << dec_inst << endl;
            }
            catch(runtime_error& ex){
                cout << ex.what() << endl;
            }
            addr += 2;
        }
        else {
            // catch those pesky 32-bit decode errors
            try {
                auto decoded_inst = decode(inst_data, addr);
                cout << decoded_inst << endl;
            }
            catch(runtime_error& ex) {
                cout << ex.what() << endl;
            } 
            addr += 4;
        }
    
    }

    cout << "=============================================\n";
    cout << "  disassembly complete";
    cout << "\n=============================================\n\n";

 ///*   
    for(address_t addr = 0x00000220; addr <= 0x000002c6;) {
        
        auto inst_data   = fetch(mem, addr);
        auto decode_data = decode(inst_data, addr);
        auto newcpu      = execute(cpu, mem, decode_data);

       std::cout << "\n" << newcpu << endl;

        addr +=2;
        if(inst_data.t32)
            addr+=2;
    }

    cout << "\n\n==========================================\n";
    cout << "  execute complete";
    cout << "\n==========================================\n\n";
//*/
    return 0;
}

void print_flag_results(int flags) {

    cout << "    " << (flags & 0x0001 ? "1 : CY(Carry)"           : "0 : NC(No Carry)") << endl;
    cout << "    " << (flags & 0x0004 ? "1 : PE(Parity Even)"     : "0 : PO(Parity Odd)") << endl;
    cout << "    " << (flags & 0x0010 ? "1 : AC(Auxiliary Carry)" : "0 : NA(No Auxiliary Carry)") << endl;
    cout << "    " << (flags & 0x0040 ? "1 : ZR(Zero)"            : "0 : NZ(Not Zero)") << endl;
    cout << "    " << (flags & 0x0080 ? "1 : NG(Negative)"        : "0 : PL(Positive)") << endl;
    cout << "    " << (flags & 0x0800 ? "1 : OV(Overflow)"        : "0 : NV(Not Overflow)") << endl;

}