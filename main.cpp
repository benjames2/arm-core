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
#include <inc/import.h>
#include <inc/test.h>
#include <inc/exceptions.h>

#include "main.h"

using namespace std;

// leave this include where it is
#include <inc/static_asserts.h>

void test_all_decode_fns(void);

int main(int argc, char* argv[]) {

    memory_t mem(memory_t::little_endian);

    //for(auto cptr : { "test/input/assembly-code.txt", "test/input/memory.txt" }) {
    //    load_memory_file(cptr, mem);
    //    std::cout << mem << std::endl;
    //}

    // starting address for machine code
    armv7_m3 armcpu;
    //armcpu.PC() = 0x0224;

    auto sz = import_bin_file("armasm/fullthumb16/main.bin", mem, 0x00000000);
    cout << mem << endl;

    cout << "size of instruction stream: " << sz << " bytes\n";

    for(size_t addr = 0; addr < sz;) {
        auto inst_data = fetch(mem, addr, true);

        try {
            auto decode_data = decode(inst_data, addr);
            cout << decode_data << endl;
        }
        catch(exception& up) {
            cout << up.what() << endl;
        }
        
        addr += 2;
        if(inst_data.type == fetched_instruction_t::t32)
            addr += 2;
    }

    return 0;

    for(address_t addr = 0x00000224; addr <= 0x000002d4;) {

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

    cout << "\n==========================================\n";
    cout << "  disassembly complete";
    cout << "\n==========================================\n\n";

    armv7_m3 cpu;
    cpu.PC() = 0x00000232;

    for(address_t addr = 0x00000224; addr <= 0x000002d4;) {
    //for(cpu.PC() = 0x00000224; cpu.PC() <= 0x000002d4;) {
  
        auto inst_data   = fetch(mem, addr, true); cout << endl;
        auto decode_data = decode(inst_data);
        auto newcpu      = execute(cpu, mem, decode_data);

        addr += 2;
        if(inst_data.type == fetched_instruction_t::t32)
            addr += 2;

        cpu = newcpu;
    }

    return 0;
}

void test_all_decode_fns(void) {
    test_decode_fns("test/testfile.branch.txt");
    test_decode_fns("test/testfile.bottom.txt");
    test_decode_fns("test/testfile.txt");
    test_32b_decode("test/testfile32b.txt");
    std::cout << "INSTRUCTION TESTS PASSED\n\n" << std::flush;
}
