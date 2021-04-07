#pragma once
#include <vector>
#include <set>
#include "armstate.h"



struct armstate_pair_t{

    //armstate_w is the predecessor state and armstate_v is the successor state
    armstate_t armstate_w;
    armstate_t armstate_v;

    friend bool operator==(armstate_pair_t& pair_lhs, armstate_pair_t& pair_rhs);
    friend bool operator<(const armstate_pair_t& lhs_pair, const armstate_pair_t& rhs_pair);
    friend std::ostream& operator<<(std::ostream& os, armstate_pair_t& armstate_pair);
};

struct abstracted_segment_t{
    /* data */
    armstate_pair_t state_pair;
    uint32_t stuttering_length;
};

struct symulation_variables_t {

    bool& skip_simulation;
    bool& path_complete;
    armstate_t& w_abs;
    armstate_t& w;
    armstate_t& v;
    uint32_t& ss_length;
    std::vector<armstate_pair_t>& RC;
    std::set<armstate_pair_t>& RU;
    std::vector<armstate_t>& I;
    std::vector<abstracted_segment_t>& RI;

    friend std::ostream& operator<<(std::ostream& os, symulation_variables_t& variable);
};

bool refinement_map(armstate_t& armstate_w, armstate_t& armstate_v);
uint8_t ref_map(armstate_t& armstate);
void successor(std::vector<armstate_pair_t>& RC, armstate_t& armstate_w, std::map<uint32_t, address32_t>& vector_table);
void symsimulation(armstate_t w0, std::vector<address32_t>& nas_array, std::map<uint32_t, address32_t>& vector_table);

//void get_union(std::vector<armstate_pair_t>& RU, std::vector<armstate_pair_t>& RC);  //do not need anymore. Using a set for RU takes care of this
