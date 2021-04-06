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
#include <inc/interrupt.h>

#include "main.h"

using namespace std;

// leave this include where it is
#include <inc/static_asserts.h>


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
    

    map<uint32_t, address32_t> vector_table;
    load_vector_table_file(folderpath + "/vector_table.txt", vector_table);

    for (auto it = vector_table.begin(); it != vector_table.end(); ++it){
        cout << it->first << "  " << it->second << endl;
    }

    auto w0 = armstate;

    cout << "\n=============================================\n";
    cout << " files loading complete";
    cout << "\n=============================================\n";

    print_disassembly(0x00000220, 0x00000258, armstate.memory);
    print_disassembly(armstate.cpu.get_PC(), last_asm_addr, armstate.memory);

    cout << "=============================================\n";
    cout << "  disassembly complete";
    cout << "\n=============================================\n\n";

///*
    for(int i = 0; i < 20; ++i) {

        auto inst_data    = fetch(armstate.memory, armstate.cpu.PC(), true);
        auto decode_data  = decode(inst_data, armstate.cpu.PC());
        auto new_armstate = execute(armstate, decode_data);
       // new_armstate      = interrupt_handler(new_armstate, vector_table);

        cout << decode_data << endl;
        print_armstate_diff(armstate, new_armstate, cout);
        armstate = new_armstate;
    }
//*/

    cout << "==========================================\n";
    cout << "  execute complete";
    cout << "\n==========================================\n\n";


    cout << "==========================================\n";
    cout << "  simulation starting";
    cout << "\n==========================================\n\n";


 /*
    try
    {
        symsimulation(w0, nas_array);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
//*/
    cout << "==========================================\n";
    cout << "  simulation ended";
    cout << "\n==========================================\n\n";

    return 0;
}
