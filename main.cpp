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

    mem.debug_clear_pages();
    for(auto cptr : { "test/input_rit/assembly-code.txt", "test/input_rit/memory.txt" }) {
        load_memory_file(cptr, mem);
        std::cout << mem << std::endl;
    }
    load_nvic("test/input_rit/nvic.txt", armcpu);

    cout << armcpu << endl;
/*
    for(int i = 0; i < 10; i++) {

        auto inst_data   = fetch(mem, armcpu.PC(), true);
        auto decode_data = decode(inst_data, armcpu.PC());
        auto newcpu      = execute(armcpu, mem, decode_data);
    
        armcpu = newcpu;
        cout << decode_data << endl;
        cout << newcpu << endl;
    }
*/

///*
    for(address_t addr = 0x00000220; addr <= 0x000002c6;) {

        if(addr > 0x00000256 && addr < 0x00000270 ){
            addr += 2; continue;
        }

        auto inst_data   = fetch(mem, addr, true);
        
        try {
            auto dec_inst = decode(inst_data, addr);
            cout << dec_inst << endl;
        }
        catch(runtime_error& ex){
            cout << ex.what() << endl;
        }

        addr += 2;
        if(inst_data.type == fetched_instruction_t::t32)
            addr += 2;
    }
//*/

//    cout << "=============================================\n";
//    cout << "  disassembly complete";
//    cout << "\n=============================================\n\n";

 ///*   
    for(address_t addr = 0x00000220; addr <= 0x000002c6;) {

        if(addr > 0x00000256 && addr < 0x00000270 ){
            addr += 2; continue;
        }

        auto inst_data   = fetch(mem, addr);
        auto decode_data = decode(inst_data, addr);
        cout << decode_data;
        auto newcpu      = execute(armcpu, mem, decode_data);

        //std::cout << "Before" << armcpu << endl;
        std::cout << newcpu << endl;

        addr +=2;
        if(inst_data.type == fetched_instruction_t::t32)
            addr+=2;
    }

    cout << "\n\n==========================================\n";
    cout << "  execute complete";
    cout << "\n==========================================\n\n";
//*/
    return 0;
}

void test_all_decode_fns(void) {
    test_decode_fns("test/testfile.branch.txt");
    test_decode_fns("test/testfile.bottom.txt");
    test_decode_fns("test/testfile.txt");
    test_32b_decode("test/testfile32b.txt");
    std::cout << "INSTRUCTION TESTS PASSED\n\n" << std::flush;
}
