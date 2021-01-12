#pragma once

//
// a collection of functions used to import various bits of data into various data structures
//

#include <iostream>
#include <string>
#include "core.h"
#include "memory_pool.h"

size_t import_bin_file(std::string filename, memory_t& memory, uint32_t address = 0);

