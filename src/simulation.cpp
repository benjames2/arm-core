#include <inc/simulation.h>
#include <inc/fetch.h>
#include <inc/execute.h>
#include <inc/decode.h>

void symsimulation(armstate_t w0){

bool skip_simulation = false;
bool path_complete   = false;
int  ss_length = 0;

std::vector<armstate_pair_t> RU, RI;
std::vector<armstate_t> I{w0};
auto w_abs = w0;

    if(!skip_simulation){
        std::vector<armstate_pair_t> RC;
        successor(RC, w0);
        auto wv_pair = RC.back();
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

void successor(std::vector<armstate_pair_t>& RC, armstate_t& armstate_w){

    auto inst_data   = fetch(armstate_w.memory, armstate_w.cpu.PC());
    auto decode_data = decode(inst_data, armstate_w.cpu.PC()); 
    auto armstate_v  = execute(armstate_w, decode_data);

    armstate_pair_t WV = {armstate_w, armstate_v};

    RC.push_back(WV);
}
