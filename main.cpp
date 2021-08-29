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

armstate_t execute_armstate(armstate_t& armstate);
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
        std::cout << armstate.memory << std::endl;
    }

    load_nvic_file(folderpath + "/nvic.txt", armstate.cpu);
    cout << armstate << endl;

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

    //print_disassembly(0x00000220, 0x00000258, armstate.memory); //printing rit isr
    print_disassembly(0x00000220, 0x00000232, armstate.memory);
    print_disassembly(armstate.cpu.get_PC(), last_asm_addr, armstate.memory);

    cout << "=============================================\n";
    cout << "  disassembly complete";
    cout << "\n=============================================\n\n";

    cout << w0 << endl;

/*
    for(int i = 0; i < 40; ++i) {

        auto inst_data        = fetch(armstate.memory, armstate.cpu.PC(), true);
        auto decode_data      = decode(inst_data, armstate.cpu.PC());
        auto new_armstate     = execute(armstate, decode_data);
        vector<armstate_t> successor_states = interrupt_handler(new_armstate, vector_table);

        cout << decode_data << endl;
        if(successor_states.size() == 1)
            print_armstate_diff(armstate, successor_states.front(), cout);
        else if(successor_states.size() == 2)
        {
            print_cpu_pair(successor_states.front().cpu, successor_states.back().cpu, cout);
        }
        else
        {
            throw runtime_error("Successor states is wrong");
        }
        
        armstate = successor_states.front();
    }
//*/
/*
   // int ext_count = 0;
    armstate_t b25back_state, b25front_state, b24back_state, b24front_state;
    vector<armstate_t> b25state, b35state, b45state, n24state, n23state;
    int state_number = 25;
    for(int i = 0; i < 45; ++i){
        
        auto new_armstate = execute_armstate(armstate);
        auto states       = interrupt_handler(new_armstate, vector_table);

        if(states.size() == 1){
            print_armstate_diff(armstate, new_armstate, cout);
            armstate = states.front();
            cout << "Value of var: " << armstate.memory.load_u32(0x10000000) << "\n" << endl;
        }
        else if (states.size() == 2){
            print_cpu_pair_execute_diff(armstate.cpu, states.front().cpu, states.back().cpu, cout);
            if (states.front().cpu.cpu_id == 25){
                //b25state = states;
                b25back_state = states.back();
                b25front_state = states.front();
                b25state = states;
            }
            if (states.front().cpu.cpu_id == 23){
                n23state = states;
            }
            if (states.front().cpu.cpu_id == 35){
                b35state = states;
            }
            if (states.front().cpu.cpu_id == 45){
                b45state = states;
            }
            if (states.front().cpu.cpu_id == 24){
                b24back_state = states.back();
                b24front_state = states.front();
                n24state = states;
            }
            if (states.front().cpu.cpu_id >= 25){
                armstate = states.back();
                cout << "External path. Value of var: " << armstate.memory.load_u32(0x10000000) << "\n" << endl;
            }
            else{ 
                armstate = states.front();
                cout << "Non External path. Value of var: " << armstate.memory.load_u32(0x10000000) << "\n" << endl;
            }
        }
        else{
            throw std::runtime_error("size of states is problematic");
        }
    }

    cout << "==========================================\n";
    cout << "  Last path complete";
    cout << "\n==========================================\n\n";

    armstate = n24state.back();
    vector<armstate_t> n34state, n44state;
    for(int i = 0; i < 44; ++i){
        cout << "Dang\n";
        auto new_armstate = execute_armstate(armstate);
        auto states       = interrupt_handler(new_armstate, vector_table);

        if(states.size() == 1){
            print_armstate_diff(armstate, new_armstate, cout);
            armstate = states.front();
            cout << "Value of var: " << armstate.memory.load_u32(0x10000000) << "\n" << endl;
        }
        else if (states.size() == 2){
            print_cpu_pair_execute_diff(armstate.cpu, states.front().cpu, states.back().cpu, cout);
            
            if (states.front().cpu.cpu_id >= 24){
                armstate = states.back();
                cout << "External path. Value of var: " << armstate.memory.load_u32(0x10000000) << "\n" << endl;
            }
            else{ 
                armstate = states.front();
                cout << "Non External path. Value of var: " << armstate.memory.load_u32(0x10000000) << "\n" << endl;
            }
            if (states.front().cpu.cpu_id == 34){
                n34state = states;
            }
            if (states.front().cpu.cpu_id == 44){
                n44state = states;
            }
        }
        else{
            throw std::runtime_error("size of states is problematic");
        }
    }

    cout << "==========================================\n";
    cout << " Second to Last path complete";
    cout << "\n==========================================\n\n";

    armstate = n23state.back();
    vector<armstate_t> n33state, n43state;
    for(int i = 0; i < 23; ++i){
        cout << "Dang2\n";
        auto new_armstate = execute_armstate(armstate);
        auto states       = interrupt_handler(new_armstate, vector_table);

        if(states.size() == 1){
            print_armstate_diff(armstate, new_armstate, cout);
            armstate = states.front();
            cout << "Value of var: " << armstate.memory.load_u32(0x10000000) << "\n" << endl;
        }
        else if (states.size() == 2){
            print_cpu_pair_execute_diff(armstate.cpu, states.front().cpu, states.back().cpu, cout);
            
            if (states.front().cpu.cpu_id >= 24){
                armstate = states.back();
                cout << "External path. Value of var: " << armstate.memory.load_u32(0x10000000) << "\n" << endl;
            }
            else{ 
                armstate = states.front();
                cout << "Non External path. Value of var: " << armstate.memory.load_u32(0x10000000) << "\n" << endl;
            }
            if (states.front().cpu.cpu_id == 33){
                n33state = states;
            }
            if (states.front().cpu.cpu_id == 43){
                n43state = states;
            }
        }
        else{
            throw std::runtime_error("size of states is problematic");
        }
    }

    cout << "==========================================\n";
    cout << " testing n33 front path";
    cout << "\n==========================================\n\n";
    armstate = n23state.front();

    for(int i = 0; i < 23; ++i){
        auto new_armstate = execute_armstate(armstate);
        auto states       = interrupt_handler(new_armstate, vector_table);

        if(states.size() == 1){
            print_armstate_diff(armstate, new_armstate, cout);
            armstate = states.front();
            cout << "Value of var: " << armstate.memory.load_u32(0x10000000) << "\n" << endl;
        }
        else if (states.size() == 2){
            print_cpu_pair_execute_diff(armstate.cpu, states.front().cpu, states.back().cpu, cout);
            armstate = states.front();
            cout << "Printing difference\n"; 
            print_armstate_diff(n23state.front(), armstate, cout);
        }
        else{
            throw std::runtime_error("size of states is problematic");
        }

    }
    

    //===================================================================================================================================
    if(b24back_state == b25back_state){
            cout << "back state b" << b24back_state.cpu.cpu_id << " is equal to " << "back state b" << b25back_state.cpu.cpu_id << endl;
        }
        else{
            cout << "b24 back different than b25 back \n";
            print_armstate_diff(b24back_state, b25back_state, cout);
        }
        if(b24front_state == b25front_state){
            cout << "front state b" << b24front_state.cpu.cpu_id << " is equal to " << "front state b" << b25front_state.cpu.cpu_id << endl;
        }
        else{
            cout << "b24 front different than b25 front \n";
            print_armstate_diff(b24front_state, b25front_state, cout);
        }
        bool result = equal(n24state.begin(), n24state.end(), b25state.begin());
        if(result){
            cout << "n24 vector state == b25 vector state" << endl;
        }
        else{
            cout << "n24 vector state != b25 vector state" << endl;
        }
        result = equal(b25state.begin(), b25state.end(), b45state.begin());
        if(result){
            cout << "b25 vector state == b45 vector state" << endl;
        }
        else{
            cout << "b25 vector state != b45 vector state" << endl;
        }
        result = equal(n34state.begin(), n34state.end(), b35state.begin());
        if(result){
            cout << "n34 vector state == b35 vector state" << endl;
        }
        else{
            cout << "n34 vector state != b35 vector state" << endl;
        }
        result = equal(n44state.begin(), n44state.end(), b45state.begin());
        if(result){
            cout << "n44 vector state == b45 vector state" << endl;
        }
        else{
            cout << "n44 vector state != b45 vector state" << endl;
        }
        result = equal(n23state.begin(), n23state.end(), n24state.begin());
        if(result){
            cout << "n24 vector state == n23 vector state" << endl;
        }
        else{
            cout << "n23 vector state != n24 vector state" << endl;
        }
        result = equal(n33state.begin(), n33state.end(), n34state.begin());
        if(result){
            cout << "n33 vector state == n34 vector state" << endl;
        }
        else{
            cout << "n33 vector state != n34 vector state" << endl;
            if(n33state.front() == n34state.front()){
                cout << "n33 state vector front is equal to n34 vector state front\n";
            }
            else{
                print_armstate_diff(n33state.front(), n34state.front(), cout);
                cout << n33state.front() << "\n\n" << n34state.front() << endl;
            }
            if(n33state.back() == n34state.back())
                cout << "n33 state vector back is equal to n34 vector state back\n";
        }
        result = equal(n43state.begin(), n43state.end(), n44state.begin());
        if(result){
            cout << "n43 vector state == n44 vector state" << endl;
        }
        else{
            cout << "n43 vector state != n44 vector state" << endl;
        }

    //========================================================================================================================================
//*/

   // cout << "w0 for simulation \n" << w0 << endl; 

    cout << "==========================================\n";
    cout << "  execute complete";
    cout << "\n==========================================\n\n";


    cout << "==========================================\n";
    cout << "  simulation starting";
    cout << "\n==========================================\n\n";


 ///*
    try
    {
        symsimulation(w0, nas_array, vector_table);
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

armstate_t execute_armstate(armstate_t& armstate){

    auto inst_data    = fetch(armstate.memory, armstate.cpu.PC(), true);
    auto decode_data  = decode(inst_data, armstate.cpu.PC());
    auto new_armstate = execute(armstate, decode_data);
    cout << decode_data << endl;

    return new_armstate;
}