#pragma once
#include <vector>
#include "armstate.h"


struct armstate_pair{

    armstate_t& armstate_w;
    armstate_t& armstate_v;
};

std::vector<armstate_pair> RC;

bool ref_map(armstate_t& armstate_w, armstate_t& armstate_v);
void successor(std::vector<armstate_pair>& RC, armstate_t& armstate_w);
void symsimulation(armstate_t w0);