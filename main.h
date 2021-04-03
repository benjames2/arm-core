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

    std::cout << "DONE\n" << std::flush;
}

void load_vector_table_file(const std::string filename, std::map<uint32_t, address32_t>& vector_table){

    std::cout << "Loading '" << filename << "'..." << std::flush;

    std::ifstream is(filename);
    is >> std::hex;

    int exc_number;
    while(is >> exc_number){
        address32_t addr;
        is >> addr;

        if(exc_number < 16 && exc_number > 50)
            throw std::runtime_error("Invalid exception number");
        else
            vector_table.insert(std::pair<uint32_t, address32_t>(exc_number, addr));
    }

    std::cout << "DONE\n" << std::flush; 
}

void load_memory_file(const std::string filename, memory_t& mempool, address32_t& end_address) {

    std::cout << "Loading '" << filename << "'..." << std::flush;

    static short int count = 0;

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

    count++;

    if (count == 1)
        end_address = addr;

    std::cout << "DONE\n" << std::flush;
}

void load_nas_file(const std::string filename, std::vector<address32_t>& nas){

    std::cout << "Loading '" << filename << "'..." <<std::flush;

    std::ifstream is(filename);
    is >> std::hex;

    address32_t pc_addr;
    while(is >> pc_addr){
       nas.push_back(pc_addr);
    }

    std::cout << "DONE\n" << std::flush;

}

void import_bin(void){

    memory_t mem(memory_t::little_endian);
 

    auto sz = import_bin_file("armasm/fullthumb16/main.bin", mem, 0x00000000);
    std::cout << mem << std::endl;
    std::cout << "size of instruction stream: " << sz << " bytes\n";
    for(size_t addr = 0; addr < sz;) {
        auto inst_data = fetch(mem, addr, true);

        try {
            auto decode_data = decode(inst_data, addr);
            std::cout << decode_data << std::endl;
        }
        catch(std::exception& up) {
            std::cout << up.what() << std::endl;
        }
        
        addr += 2;
        if(inst_data.type == fetched_instruction_t::t32)
            addr += 2;
    }
}

std::string to_hex(uint8_t addr) {
    std::string s = "0x";
    for(int i = 1; i >= 0; i--) {
        s.push_back("0123456789ABCDEF"[(addr >> (i*4)) & 0xF]);
    }
    return s;
}

void test_all_decode_fns(void) {
    test_decode_fns("test/instruction_test/testfile.branch.txt");
    test_decode_fns("test/instruction_test/testfile.bottom.txt");
    test_decode_fns("test/instruction_test/testfile.txt");
    test_32b_decode("test/instruction_test/testfile32b.txt");
    std::cout << "INSTRUCTION TESTS PASSED\n\n" << std::flush;
}

void print_disassembly( const address32_t& start_addr, const address32_t& end_addr, memory_t& mem){
    int count = 1;
    for(address32_t addr = start_addr; addr <= end_addr;) {

        auto inst_data   = fetch(mem, addr, true);
        
        try {
            auto dec_inst = decode(inst_data, addr);
            std::cout << std::setw(2) << count << "- ";
            std::cout << dec_inst << std::endl;
            count++;
        }
        catch(std::runtime_error& ex){
            std::cout << ex.what() << std::endl;
        }

        addr += 2;
        if(inst_data.type == fetched_instruction_t::t32)
            addr += 2;
    }
}