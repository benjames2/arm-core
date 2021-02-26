#include <inc/simulation.h>
#include <inc/fetch.h>
#include <inc/execute.h>
#include <inc/decode.h>

void symsimulation(armstate_t w0){

    auto check_belonging = [](std::vector<armstate_t>& I, armstate_t& armstate_v) ->bool{
        for(auto& armstate : I){
            if (armstate == armstate_v)
                return true;
        }
        return false;
    };

    bool skip_simulation = false;
    bool path_complete   = false;
    int  ss_length = 0;

    std::set<armstate_pair_t> RU;
    std::vector<armstate_pair_t> RI;
    std::vector<armstate_t> I{w0};

    auto w_abs = w0;                                     //Initialize w_abs to w0, 
    auto w = w0;                                         //Initialize w to w0
    auto v = w0;                                         //v is supposed to be initialized to null but it should not matter

    //First if tested
    std::vector<armstate_pair_t> RC;
    if(!skip_simulation){
        successor(RC, w0);                                                //RC ← simulate-object-code(w, object-code);
        auto wv_pair = RC.back();              
        w = wv_pair.armstate_w;                                           //Choose any <w, v>, ∈ RC; last pair element is chosen in RC
        v = wv_pair.armstate_v;
        RC.pop_back();                                                    //last element removed
        if(!RC.empty()){
            for (const auto& pair_wv : RC){                                // RU is a set, each pair will be distinct. The pairs in RC gets added to RU
                RU.insert(pair_wv);                                        // If a pair in RC is alreay present in RU, it does not get added to RU because RU is a set
            }
        }
    }

    // Second if
    if(check_belonging(I, v))                        // if v ∈ I then path-complete ← T RU E;
        path_complete = true;

    //Third if
    if(RC.empty() && refinement_map(w, v)){

    }

}


bool refinement_map(armstate_t& armstate_w, armstate_t& armstate_v){

    address32_t addr = 0x2009C034;

    return armstate_w.memory.load_u8(addr) == armstate_v.memory.load_u8(addr);

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

void successor(std::vector<armstate_pair_t>& RC, armstate_t& armstate_w){

    auto inst_data   = fetch(armstate_w.memory, armstate_w.cpu.PC());
    auto decode_data = decode(inst_data, armstate_w.cpu.PC()); 
    auto armstate_v  = execute(armstate_w, decode_data);

    armstate_pair_t WV = {armstate_w, armstate_v};

    RC.push_back(WV);
}

std::ostream& operator<<(std::ostream& os, armstate_pair_t& armstate_pair){

    os << "ARMSTATE W " << armstate_pair.armstate_w << "\n";

    os << "ARMSTATE V " << armstate_pair.armstate_v << std::flush;

    return os;
}
/*
void get_union(std::vector<armstate_pair_t>& RU, std::vector<armstate_pair_t>& RC){

    //Using a set will dictate the order in which the state pairs are stored in RU
    std::set<armstate_pair_t> union_set;

    for (const auto& pair_wv : RC){
        union_set.insert(pair_wv);
    }

    for(const auto& pair_wv : RU){
        union_set.insert(pair_wv);
    }

    //for (auto iter = RU.begin(); iter != RU.end(); ++iter){

    //}

}
*/