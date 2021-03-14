#include <iostream>
#include <iomanip>
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
#include <inc/armstate.h>
#include <inc/simulation.h>

#include "main.h"

using namespace std;

// leave this include where it is
#include <inc/static_asserts.h>

void test_all_decode_fns(void);
void import_bin(void);

std::string to_hex(uint8_t addr) {
    std::string s = "0x";
    for(int i = 1; i >= 0; i--) {
        s.push_back("0123456789ABCDEF"[(addr >> (i*4)) & 0xF]);
    }
    return s;
}

int main(int argc, char* argv[]) {

    if(argc < 2){
        cout << "Usage: " << argv[0] << " <folderpath_missing/>" << endl << flush;
		return 1;
    }
    string folderpath(argv[1]); 

    armstate_t armstate;
    address32_t last_asm_addr = 0;

    for(auto cptr : { folderpath + "/assembly-code.txt", folderpath + "/memory.txt" }) {
        load_memory_file(cptr, armstate.memory, last_asm_addr);
        //std::cout << armstate.memory << std::endl;
    }

    load_nvic_file(folderpath + "/nvic.txt", armstate.cpu);
    //cout << armstate << endl;

    vector<address32_t> nas_array;
    load_nas_file(folderpath + "/nas.txt", nas_array);

    auto w0 = armstate;

    auto w1 = armstate;
    auto w2 = armstate;

/*
    for(const auto& pc_addr : nas_array){
        
        cout << hex << pc_addr << endl;

    }

    

    memory_t mem1(memory_t::little_endian);
    mem1.store_u32(0x0, 0x10);
    memory_t mem2(memory_t::little_endian);
    mem2.store_u32(0x0, 0x1);
    //mem2.store_u32(0x16, 0xbeee);
    //mem2.store_u32(0x22, 0xdeed);

    cout << mem1 << endl;
    cout << mem2 << endl;
    print_memory_diff(mem1, mem2, cout);
    
    //std::array<armstate_t, 5> w = {w0, w0, w0, w0, w0};
*/
    cout << "\n=============================================\n";
    cout << " files loading complete";
    cout << "\n=============================================\n";

/*
    int count = 1;
    for(address32_t addr = armstate.cpu.get_PC(); addr <= last_asm_addr;) {

       // if(addr > 0x00000256 && addr < 0x00000270 ){
       //     addr += 2; continue;
       // }

        auto inst_data   = fetch(armstate.memory, addr, true);
        
        try {
            auto dec_inst = decode(inst_data, addr);
            cout << setw(2) << count << "- ";
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

///*
    for(int i = 0; i < 140; ++i) {

        auto inst_data    = fetch(armstate.memory, armstate.cpu.PC(), true);
        auto decode_data  = decode(inst_data, armstate.cpu.PC());
        auto new_armstate = execute(armstate, decode_data);
    
        cout << decode_data << endl;
        print_armstate_diff(armstate, new_armstate, cout);
        cout << to_hex(ref_map(new_armstate)) << endl;
       // cout << new_armstate.memory << endl;
        armstate = new_armstate;
        //w[i] = new_armstate;
        if (i == 131)
            w1 = new_armstate;
        if (i == 34)
            w2 = new_armstate;
        //cout << new_armstate << endl;
        //cout << newcpu << endl;
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
//*/
    cout << "==========================================\n";
    cout << "  execute complete";
    cout << "\n==========================================\n\n";


    cout << "==========================================\n";
    cout << "  simulation starting";
    cout << "\n==========================================\n\n";

    try
    {
        symsimulation(w0, nas_array);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
    

    cout << "==========================================\n";
    cout << "  simulation ended";
    cout << "\n==========================================\n\n";

   // cout << w0 << endl << w1 << endl;
   // print_armstate_diff(w0, w1, cout);
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