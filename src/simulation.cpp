#include <inc/simulation.h>
#include <inc/fetch.h>
#include <inc/execute.h>
#include <inc/decode.h>
#include <inc/interrupt.h>

void symsimulation(armstate_t w0, std::vector<address32_t>& nas_array, std::map<uint32_t, address32_t>& vector_table){

    auto check_belonging = [](std::vector<armstate_t>& I, armstate_t& armstate_v) ->bool{
        for(auto& armstate : I){
            if (armstate == armstate_v)
                return true;
        }
        return false;
    };

    auto belong_to_nas = [&nas_array](armstate_t& armstate_v) -> bool{
        for(const auto& pc_addr : nas_array){
            if(armstate_v.cpu.get_PC() == pc_addr)
                return true;
        }
        return false;        
    };

    bool skip_simulation = false;
    bool path_complete   = false;
    uint32_t  ss_length  = 0;

    std::set<armstate_pair_t> RU;
    std::vector<abstracted_segment_t> RI;
    std::vector<armstate_t> I{w0};

    auto w_abs = w0;                                     //Initialize w_abs to w0, 
    auto w = w0;                                         //Initialize w to w0
    auto v = w0;                                         //v is supposed to be initialized to null but it should not matter

    static int first_loop, second_loop = 0;
    do
    {
        do
        {
            //First if tested
            std::vector<armstate_pair_t> RC;
            if(!skip_simulation){
                successor(RC, w, vector_table);                  //RC ← simulate-object-code(w, object-code);
                auto wv_pair = RC.back();              
                w = wv_pair.armstate_w;                          //Choose any <w, v>, ∈ RC; last pair element is chosen in RC
                v = wv_pair.armstate_v;
                RC.pop_back();                                   //last element removed
                if(!RC.empty()){
                    for (const auto& pair_wv : RC){              // RU is a set, each pair will be distinct. The pairs in RC gets added to RU
                        RU.insert(pair_wv);                      // If a pair in RC is alreay present in RU, it does not get added to RU because RU is a set
                    }
                }
            }

            // Second if
            if(check_belonging(I, v))                             // if v ∈ I then path-complete ← T RUE;
                path_complete = true;

            //Third if
            if(RC.empty() && refinement_map(w, v) && (!belong_to_nas(v)))
                ss_length++;

            else{

                if(!refinement_map(w,v)){
                    abstracted_segment_t segment = {{w,v},0};  
                    RI.push_back(segment);
                    I.push_back(v);                                 //Maybe "I" should be a set as well. probably nots
                }

                if(refinement_map(w,v) && belong_to_nas(v)){
                    ss_length++;
                    I.push_back(v);
                    abstracted_segment_t segment = {{w_abs, v}, ss_length};
                    RI.push_back(segment);
                }
                else{
                    abstracted_segment_t segment = {{w_abs, w}, ss_length};
                    RI.push_back(segment);
                }

                if(RC.empty()){
                    w_abs = v;
                    ss_length = 0;
                }
                else{
                    w_abs = w;
                    ss_length = 1;
                }
            }

            w = v;

            if(skip_simulation) skip_simulation = false;
///*
            symulation_variables_t var = {
                skip_simulation, path_complete, w_abs, w, v,ss_length, RC, RU, I, RI
            };

            first_loop++;
            std::cout << std::dec;
            std::cout << "First Loop iteration: " << first_loop << std::endl;
            std::cout << var << std::endl;
            //if(first_loop == 140){
             //   throw std::runtime_error("Inside loop over");
           // }
//*/
                

        } while (!path_complete);

        abstracted_segment_t segment = {{w_abs, v}, ss_length};
        RI.push_back(segment);

        if(!RU.empty()){
            auto iterator = RU.begin();                           //choosing the first element in RU
            auto wv_pair = *iterator;
            w = wv_pair.armstate_w;
            v = wv_pair.armstate_v;
            RU.erase(iterator);

            w_abs = w;
            ss_length = 0;
            path_complete = false;
            skip_simulation = true;
        }

        ++second_loop;
        std::cout << "Second Loop iteration: " << second_loop << std::endl;
        std::vector<armstate_pair_t> RC;
        symulation_variables_t var = {
                skip_simulation, path_complete, w_abs, w, v,ss_length, RC, RU, I, RI
            };
        std::cout << var << std::endl;
    } while (!(path_complete && RU.empty()));

}


bool refinement_map(armstate_t& armstate_w, armstate_t& armstate_v){

    address32_t addr = 0x2009C034; //stepper motor
    //address32_t addr = 0x10000000;   //external interrupt benchmark

    uint32_t motor_state_w = armstate_w.memory.load_u32(addr);
    uint32_t motor_state_v = armstate_v.memory.load_u32(addr);

    return motor_state_w == motor_state_v;

}


uint8_t ref_map(armstate_t& armstate){

    uint8_t motor_state = (armstate.memory.load_u8(0x2009c034 + 3)) & 0xF0;
    return motor_state;
}


bool operator==(armstate_pair_t& lhs_pair, armstate_pair_t& rhs_pair){
    if(lhs_pair.armstate_w != rhs_pair.armstate_w)
        return false;
    if(lhs_pair.armstate_v != rhs_pair.armstate_v)
        return false;

    return true;
}

bool operator<(const armstate_pair_t& lhs_pair, const armstate_pair_t& rhs_pair){

    if(lhs_pair.armstate_w.cpu.cpu_id != rhs_pair.armstate_w.cpu.cpu_id)
        return (lhs_pair.armstate_w.cpu.cpu_id < rhs_pair.armstate_w.cpu.cpu_id);
    else
        return (lhs_pair.armstate_v.cpu.cpu_id < rhs_pair.armstate_v.cpu.cpu_id);
    
}

void successor(std::vector<armstate_pair_t>& RC, armstate_t& armstate_w, std::map<uint32_t, address32_t>& vector_table){

    auto inst_data   = fetch(armstate_w.memory, armstate_w.cpu.PC());
    auto decode_data = decode(inst_data, armstate_w.cpu.PC()); 
    auto armstate_v  = execute(armstate_w, decode_data);
    auto states      = interrupt_handler(armstate_v, vector_table);

    if (states.size() == 1){
        armstate_pair_t WV = {armstate_w, states.front()};
        RC.push_back(WV);
    }
    else if(states.size() == 2){
        armstate_pair_t WV1 = {armstate_w, states.front()};
        armstate_pair_t WV2 = {armstate_w, states.back()};
        RC.push_back(WV1);
        RC.push_back(WV2);
    }
    else
    {
        throw std::runtime_error("In symulation: size of state is incoherent");
    }
   
}

std::ostream& operator<<(std::ostream& os, armstate_pair_t& armstate_pair){

    os << " ARMSTATE W " << armstate_pair.armstate_w << "\n";

    os << " ARMSTATE V " << armstate_pair.armstate_v << std::flush;

    return os;
}

 std::ostream& operator<<(std::ostream& os, symulation_variables_t& variable){

    os << "SKIP_SIMULATION: " << variable.skip_simulation << std::endl;
    os << "PATH_COMPLETE: "   << variable.path_complete   << std::endl;

    os << "W_abs: " << std::hex << variable.w_abs.cpu.cpu_id << std::endl;
    os << "W: "     << std::hex << variable.w.cpu.cpu_id     << std::endl;
    os << "V: "     << std::hex << variable.v.cpu.cpu_id     << std::endl;

    os << std::dec;
    os << "SS-Length: " << variable.ss_length << std::endl;

    os << "Content of RC" << std::endl;
    for(auto& pair : variable.RC){
        os << "State W: " << std::hex << pair.armstate_w.cpu.cpu_id << "  "; 
        os << "State V: " << std::hex << pair.armstate_v.cpu.cpu_id << "  "; 
        os << std::endl;
    }

    os << "Content of RU" << std::endl;
    for(auto& pair : variable.RU){
        os << "State W: " << std::hex << pair.armstate_w.cpu.cpu_id << "  "; 
        os << "State V: " << std::hex << pair.armstate_v.cpu.cpu_id << "  "; 
        os << std::endl;
    }

    os << "States stored in I" << std::endl;
    for(auto& state : variable.I){
        os << "State W: " << std::dec << state.cpu.cpu_id<< "  ";  
        os << std::endl;
    }

    os << "Abstracted Segment" << std::endl;
    for(auto& segment : variable.RI){
        os << "Segment: <" << std::dec << segment.state_pair.armstate_w.cpu.cpu_id << ", ";
        os << segment.state_pair.armstate_v.cpu.cpu_id << ", ";
        os << std::dec << segment.stuttering_length << ">";
        os << std::endl;
    }

    os << std::flush;
    return os;
 }