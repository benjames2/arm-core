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
#include <inc/stack_operations.h>
#include <inc/import.h>
#include <inc/test.h>
#include <inc/exceptions.h>
#include <inc/range.h>

#include "main.h"

using namespace std;

// leave this include where it is
#include <inc/static_asserts.h>

void test_all_decode_fns(void);
void import_bin(void);

int main(int argc, char* argv[]) {

    armv7_m3 armcpu;
    memory_t mem(memory_t::little_endian);

    for(auto cptr : { "test/input_ext/assembly-code.txt", "test/input_ext/memory.txt" }) {
        load_memory_file(cptr, mem);
        std::cout << mem << std::endl;
    }
    load_nvic_file("test/input_ext/nvic.txt", armcpu);
    cout << armcpu << endl;



///*
    int count = 1;
    for(address_t addr = 0x00000238; addr <= 0x0000027C;) {

       // if(addr > 0x00000256 && addr < 0x00000270 ){
       //     addr += 2; continue;
       // }

        auto inst_data   = fetch(mem, addr, true);
        
        try {
            auto dec_inst = decode(inst_data, addr);
            cout << count << "- ";
            cout << dec_inst << endl;
            count++;
        }
        catch(runtime_error& ex){
            cout << ex.what() << endl;
        }

        addr += 2;
        if(inst_data.type == fetched_instruction_t::t32)
            addr += 2;
    }
//*/

    cout << "=============================================\n";
    cout << "  disassembly complete";
    cout << "\n=============================================\n\n";

    return 0;
///*
    for(int i = 0; i < 10; i++) {

        auto inst_data   = fetch(mem, armcpu.PC(), true);
        auto decode_data = decode(inst_data, armcpu.PC());
        auto newcpu      = execute(armcpu, mem, decode_data);
    
        armcpu = newcpu;
        cout << decode_data << endl;
        cout << newcpu << endl;
    }
//*/

 /*   
    for(address_t addr = 0x00000224; addr <= 0x000002d4;) {

        //if(addr > 0x00000256 && addr < 0x00000270 ){
        //    addr += 2; continue;
        //}

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
    test_decode_fns("test/instruction_test/testfile.branch.txt");
    test_decode_fns("test/instruction_test/testfile.bottom.txt");
    test_decode_fns("test/instruction_test/testfile.txt");
    test_32b_decode("test/instruction_test/testfile32b.txt");
    std::cout << "INSTRUCTION TESTS PASSED\n\n" << std::flush;
}

void import_bin(void){

    memory_t mem(memory_t::little_endian);

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
}