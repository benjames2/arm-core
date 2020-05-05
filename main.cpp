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

int main(int argc, char* argv[]) {

    armv7_m3 armcpu;
    memory_t mem(memory_t::little_endian);

    for(int i : range(0, 16)){
        armcpu.set_register_u32(i, i);
    }

    armcpu.set_stack_mode(armv7_m3::stack_mode_FullDescending);

    auto addr = push_word(armcpu, mem, 12, armcpu.get_register_u32(12));
    auto addr1 = push_word(armcpu, mem, addr, armcpu.get_register_u32(11));
    auto addr2 = push_word(armcpu, mem, addr1, armcpu.get_register_u32(10));

    cout << addr  << endl;
    cout << addr1 << endl;
    cout << addr2 << endl;
    cout << mem << endl;

    auto addr3 = pop_word(armcpu, mem, addr2, 0);
    auto addr4 = pop_word(armcpu, mem, addr3, 1);
    auto addr5 = pop_word(armcpu, mem, addr4, 2);

    cout << addr3  << endl;
    cout << addr4 << endl;
    cout << addr5 << endl;
    cout << mem << endl;

    cout << hex << armcpu.get_register_u32(0) << armcpu.get_register_u32(1) << armcpu.get_register_u32(2) << endl;


    return 0;

/*
    armcpu.set_stack_mode(armv7_m3::stack_mode_FullAscending); 


    auto addr  = push_word(armcpu, mem, 0, 0xEFBEADED);
    auto addr1 = push_word(armcpu, mem, addr, 0xDEADBEEF);

    auto addr2 = pop_word(armcpu, mem, addr1, 0);
    auto addr3 = pop_word(armcpu, mem, addr2, 1);

    cout << mem << "\n";
    cout << addr << "\n";
    cout << addr1 << "\n";
    cout << addr2 << "\n";
    cout << addr3 << "\n";
    cout << hex << armcpu.get_register_u32(0) << "\n";
    cout << hex << armcpu.get_register_u32(1) << "\n";

    return 0;
//*/
    //for(auto cptr : { "test/input/assembly-code.txt", "test/input/memory.txt" }) {
    //    load_memory_file(cptr, mem);
    //    std::cout << mem << std::endl;
    //}

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
    for(auto cptr : { "test/input/assembly-code.txt", "test/input/memory.txt" }) {
        load_memory_file(cptr, mem);
        std::cout << mem << std::endl;
    }
    load_nvic("test/input/nvic.txt", armcpu);
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
    int count = 1;
    for(address_t addr = 0x00000224; addr <= 0x000002d4;) {

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

//    cout << "=============================================\n";
//    cout << "  disassembly complete";
//    cout << "\n=============================================\n\n";

 ///*   
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
    test_decode_fns("test/testfile.branch.txt");
    test_decode_fns("test/testfile.bottom.txt");
    test_decode_fns("test/testfile.txt");
    test_32b_decode("test/testfile32b.txt");
    std::cout << "INSTRUCTION TESTS PASSED\n\n" << std::flush;
}
