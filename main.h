#pragma once

//============================
// 16-bit base formats
//============================
#define SHL(n) (1 << n)

#define BASE_FORMAT_1  (0) // no bits on by default
#define BASE_FORMAT_2  (SHL(12) | SHL(11) | 0)
#define BASE_FORMAT_3  (SHL(13) | 0)
#define BASE_FORMAT_4  (SHL(14) | 0)
#define BASE_FORMAT_5  (SHL(14) | SHL(10) | 0)
#define BASE_FORMAT_6  (SHL(14) | SHL(11) | 0)
#define BASE_FORMAT_7  (SHL(14) | SHL(12) | 0)
#define BASE_FORMAT_8  (SHL(14) | SHL(12) | SHL(9) | 0)
#define BASE_FORMAT_9  (SHL(14) | SHL(13) | 0)
#define BASE_FORMAT_10 (SHL(15) | 0)
#define BASE_FORMAT_11 (SHL(15) | SHL(12) | 0)
#define BASE_FORMAT_12 (SHL(15) | SHL(13) | 0)
#define BASE_FORMAT_13 (SHL(15) | SHL(13) | SHL(12) | 0)
#define BASE_FORMAT_14 (SHL(15) | SHL(13) | SHL(12) | SHL(10) | 0)
#define BASE_FORMAT_15 (SHL(15) | SHL(14) | 0)
#define BASE_FORMAT_16 (SHL(15) | SHL(14) | SHL(12) | 0)
#define BASE_FORMAT_17 (SHL(15) | SHL(14) | SHL(12) | SHL(11) | SHL(10) | SHL(9) | SHL(8) | 0)
#define BASE_FORMAT_18 (SHL(15) | SHL(14) | SHL(13) | 0)
#define BASE_FORMAT_19 (SHL(15) | SHL(14) | SHL(13) | SHL(12) | 0)

//======================================
// 32-bit instruction formats
//======================================
#define TOP_32B_1 (SHL(31) | SHL(30) | SHL(29) | SHL(27) | 0)
#define TOP_32B_2 (SHL(31) | SHL(30) | SHL(29) | SHL(28) | 0)
#define TOP_32B_3 (SHL(31) | SHL(30) | SHL(29) | SHL(28) | SHL(27) | 0)

#define FORMAT_ADC_IMM     (TOP_32B_2 | SHL(24) | SHL(22))
#define FORMAT_ADD_IMM     (TOP_32B_2 | SHL(24))
#define FORMAT_AND_IMM     (TOP_32B_2)
#define FORMAT_BIC_IMM     (TOP_32B_2 | SHL(21))
#define FORMAT_CMN_IMM     (TOP_32B_2 | SHL(24) | SHL(20) | SHL(11) | SHL(10) SHL(9) | SHL(8))
#define FORMAT_CMP_IMM     (TOP_32B_2 | SHL(24) | SHL(23) | SHL(21) | SHL(20) | SHL(11) | SHL(10) | SHL(9) | SHL(8))
#define FORMAT_EOR_IMM     (TOP_32B_2 | SHL(23))
#define FORMAT_LDM         (TOP_32B_1 | SHL(23) | SHL(20))
#define FORMAT_LDR_IMM_T3  (TOP_32B_3 | SHL(23) | SHL(22) | SHL(20))
#define FORMAT_LDR_IMM_T4  (TOP_32B_3 | SHL(22) | SHL(20) | SHL(11))
#define FORMAT_LDR_LIT     (TOP_32B_3 | SHL(22) | SHL(20) | SHL(19) | SHL(18) | SHL(17) | SHL(16))
#define FORMAT_LDR_REG     (TOP_32B_3 | SHL(22) | SHL(20))
#define FORMAT_LDRD_IMM    (TOP_32B_1 | SHL(22) | SHL(20))
#define FORMAT_LDRD_LIT    (TOP_32B_1 | SHL(22) | SHL(20) | SHL(19) | SHL(18) | SHL(17) | SHL(16))
#define FORMAT_LDREX       (TOP_32B_1 | SHL(22) | SHL(20) | SHL(11) | SHL(10) | SHL(9) | SHL(8))
#define FORMAT_LDREXB      (TOP_32B_1 | SHL(23) | SHL(22) | SHL(20) | SHL(11) | SHL(10) | SHL(9) | SHL(8) | SHL(3) | SHL(2) SHL(1) | SHL(0))
#define FORMAT_LDREXH      (TOP_32B_1 | SHL(23) | SHL(22) | SHL(20) | SHL(11) | SHL(10) | SHL(9) | SHL(8) | SHL(6) |SHL(4) | SHL(3) | SHL(2) SHL(1) | SHL(0))
#define FORMAT_LDRT        (TOP_32B_3 | SHL(22) | SHL(20) | SHL(11) | SHL(10) | SHL(9))
#define FORMAT_MOV_IMM     (TOP_32B_2 | SHL(25) | SHL(22))
#define FORMAT_MVN_IMM     (TOP_32B_2 | SHL(22) | SHL(21) | SHL(19) | SHL(18) | SHL(17) | SHL(16))
#define FORMAT_ORN_IMM     (TOP_32B_2 | SHL(22) | SHL(21))
#define FORMAT_ORR_IMM     (TOP_32B_2 | SHL(22))
#define FORMAT_POP         (TOP_32B_1 | SHL(23) | SHL(21) | SHL(20) | SHL(19) | SHL(18) | SHL(16))
#define FORMAT_PUSH        (TOP_32B_1 | SHL(23) | SHL(21) | SHL(19) | SHL(18) | SHL(16))
#define FORMAT_RSB_IMM     (TOP_32B_2 | SHL(24) | SHL(23) | SHL(22))
#define FORMAT_SBC_IMM     (TOP_32B_2 | SHL(24) | SHL(22) | SHL(21))
#define FORMAT_STM         (TOP_32B_1 | SHL(23))
#define FORMAT_STMDB       (TOP_32B_1 | SHL(24))
#define FORMAT_STR_IMM_T3  (TOP_32B_3 | SHL(23) | SHL(22))
#define FORMAT_STR_IMM_T4  (TOP_32B_3 | SHL(22) | SHL(11))
#define FORMAT_STR_REG     (TOP_32B_3 | SHL(22))
#define FORMAT_STRB_IMM_T2 (TOP_32B_3 | SHL(23))
#define FORMAT_STRB_IMM_T3 (TOP_32B_3 | SHL(11))
#define FORMAT_STRB_REG    (TOP_32B_3)
#define FORMAT_STRD_IMM    (TOP_32B_1 | SHL(22))
#define FORMAT_STREX       (TOP_32B_1 | SHL(22))
#define FORMAT_STREXB      (TOP_32B_1 | SHL(23) | SHL(22) | SHL(11) | SHL(10) | SHL(9) | SHL(8) | SHL(6))
#define FORMAT_STREXH      (TOP_32B_1 | SHL(23) | SHL(22) | SHL(11) | SHL(10) | SHL(9) | SHL(8) | SHL(6) | SHL(4))
#define FORMAT_STRH_IMM_T2 (TOP_32B_3 | SHL(23) | SHL(21))
#define FORMAT_STRH_IMM_T3 (TOP_32B_3 | SHL(21) | SHL(11))
#define FORMAT_STRH_REG    (TOP_32B_3 | SHL(21) | SHL(21))
#define FORMAT_SUB_IMM     (TOP_32B_2 | SHL(24) | SHL(23) | SHL(21))
#define FORMAT_TBB         (TOP_32B_1 | SHL(23) | SHL(22) | SHL(20) | SHL(15) | SHL(14) | SHL(13) | SHL(12))
#define FORMAT_TBH         (TOP_32B_1 | SHL(23) | SHL(22) | SHL(20) | SHL(15) | SHL(14) | SHL(13) | SHL(12))
#define FORMAT_TEQ_IMM     (TOP_32B_2 | SHL(23) | SHL(20) | SHL(11) | SHL(10) | SHL(9) | SHL(8))
#define FORMAT_TST_IMM     (TOP_32B_2 | SHL(20) | SHL(11) | SHL(10) | SHL(9) | SHL(8))


#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <assert.h>
#include <inc/decode_structure.h>
#include <inc/decode_16.h>
#include <inc/decode_32.h>
#include <inc/memory_pool.h>

void load_memory_file(std::string filename, memory_t& mempool) {

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

    std::cout << "DONE\n" << std::flush;
}

static uint32_t getFormat(int format) {
    switch(format) {
        case 1:  return BASE_FORMAT_1;
        case 2:  return BASE_FORMAT_2;
        case 3:  return BASE_FORMAT_3;
        case 4:  return BASE_FORMAT_4;
        case 5:  return BASE_FORMAT_5;
        case 6:  return BASE_FORMAT_6;
        case 7:  return BASE_FORMAT_7;
        case 8:  return BASE_FORMAT_8;
        case 9:  return BASE_FORMAT_9;
        case 10: return BASE_FORMAT_10;
        case 11: return BASE_FORMAT_11;
        case 12: return BASE_FORMAT_12;
        case 13: return BASE_FORMAT_13;
        case 14: return BASE_FORMAT_14;
        case 15: return BASE_FORMAT_15;
        case 16: return BASE_FORMAT_16;
        case 17: return BASE_FORMAT_17;
        case 18: return BASE_FORMAT_18;
        case 19: return BASE_FORMAT_19;
        default:
            throw std::runtime_error("getFormat : invalid format specifier");
    }
}

void test_decode_fns(std::string filename) {

    std::cout << "\n=======================================================================\n";
    std::cout << " " << filename << "\n";
    std::cout << "=======================================================================\n\n";
    std::cout << std::flush;

    std::ifstream is(filename);
    std::string str;

    while(std::getline(is, str)) {

        //is.ignore();
        std::stringstream ss(str);
        int format;
        ss >> format;
        uint32_t base_format = getFormat(format);

        std::string strval;
        uint32_t shft, val;
        while(ss >> strval) {
            if(strval == ";")
                break;

            val = std::stoul(strval);
            ss >> shft;

            base_format |= (val << shft);
        }

        std::getline(is, strval);
        //is.ignore();
        auto dec     = decode_16bit_instruction(0x00, base_format);
        auto dec_str = dec.str();
        
        std::cout << "Expected : " << strval << std::endl;
        std::cout << "Decoded  : " << dec_str << std::endl << std::flush;
        assert(strval == dec_str);
        std::cout << "SUCCESS\n\n";
    }

}

#undef SHL
