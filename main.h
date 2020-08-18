#pragma once

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <map>
#include <assert.h>
#include <inc/core.h>
#include <inc/memory_pool.h>


void load_nvic_file(std::string filename, armv7_m3& cpu) {

    std::cout << "Loading '" << filename << "'..." << std::flush;

    std::ifstream is(filename);
    is >> std::hex;

    int regnum;
    while(is >> regnum) {
        uint32_t v;
        is >> v;

        if(regnum <= 15 && regnum >= 0)
            cpu.set_register_u32(regnum, v);
        else if(regnum == 16)
            cpu.set_APSR(v);
        else
            throw std::runtime_error(
                "load_nvic : invalid register reference 'r" + 
                std::to_string(regnum) + "'");
    }

    std::cout << "DONE" << std::flush;
}

void load_memory_file(const std::string filename, memory_t& mempool, address_t& end_address) {

    std::cout << "Loading '" << filename << "'..." << std::flush;

    std::ifstream is(filename);
    std::string str;

    uint32_t addr;
    while(is >> std::hex >> addr) {
        is >> std::dec >> str;

        if(str.size() == 4) {
            std::stringstream ss(str);
            uint16_t u16;
            ss >> std::hex >> u16;
            mempool.store_u16(addr, u16);
        }
        else if(str.size() == 8) {
            std::stringstream ss(str);
            uint32_t u32;
            ss >> std::hex >> u32;

            mempool.store_u16(addr, (u32 >> 16) & 0xFFFF);
            mempool.store_u16(addr+2, u32 & 0xFFFF);
        }
        else {
            throw std::runtime_error(
                "while reading file '" + filename + "' invalid data "
                "entry size: " + std::to_string(str.size()));
        }

    }

    if (filename == "test/input_ext/assembly-code.txt")
        end_address = addr;

    std::cout << "DONE\n" << std::flush;
}
