#pragma once
#include <vector>
#include "armstate.h"


struct armstate_pair_t{

    armstate_t armstate_w;
    armstate_t armstate_v;

    friend bool operator==(armstate_pair_t& pair_lhs, armstate_pair_t& pair_rhs);
};

std::vector<armstate_pair_t> RC;

bool refinement_map(armstate_t& armstate_w, armstate_t& armstate_v);
void successor(std::vector<armstate_pair_t>& RC, armstate_t& armstate_w);
void symsimulation(armstate_t w0);