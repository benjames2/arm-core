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
#define FORMAT_CMN_IMM     (TOP_32B_2 | SHL(24) | SHL(20) | SHL(11) | SHL(10) | SHL(9) | SHL(8))
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
#define FORMAT_LDREXB      (TOP_32B_1 | SHL(23) | SHL(22) | SHL(20) | SHL(11) | SHL(10) | SHL(9) | SHL(8) | SHL(3) | SHL(2) | SHL(1) | SHL(0))
#define FORMAT_LDREXH      (TOP_32B_1 | SHL(23) | SHL(22) | SHL(20) | SHL(11) | SHL(10) | SHL(9) | SHL(8) | SHL(6) | SHL(4) | SHL(3) | SHL(2) | SHL(1) | SHL(0))
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


static uint32_t get32bformat(std::string format);
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

//============================================
// 32-bit
//============================================

void printBaseFormat(uint32_t number){

    std::cout << "Instruction passed: ";
    for(int i = 31; i != -1; i--){
        std::cout << (((1 << i) & number) ? "1" : "0");
    }
    std::cout << "\n";
}
///*
void test_32b_decode(std::string filename){

    std::cout << "\n=======================================================================\n";
    std::cout << " " << filename << "\n";
    std::cout << "=======================================================================\n\n";
    std::cout << std::flush;
    
    std::ifstream file(filename);
    std::string line;
   
    while (std::getline(file, line)){

        std::stringstream ss0(line);
        std::string commment;
        ss0 >> commment;
        if(commment == "//")
            continue;

        std::string line2;
        std::getline(file, line2);
        std::stringstream ss(line2);
        std::string instruction_name;
        ss >> instruction_name;
        uint32_t base_32b_format = get32bformat(instruction_name);


        std::string stringval;
        uint32_t val, shift;
        while(ss >> stringval){
            if(stringval == ";")
                break;

            val = std::stoul(stringval);

            ss >> shift;
            base_32b_format |= (val << shift); 
        }

        printBaseFormat(base_32b_format);
        auto dec     = decode_32bit_instruction(0x00, base_32b_format);
        auto dec_str = dec.str();

        std::cout << "Expected: " << line << std::endl;
        std::cout << "Decoded:  " << dec_str << std::endl << std::flush;
        assert (line == dec_str);
        std::cout << "SUCCESS\n\n";
    }
    
}
//*/
/*
void test_32b_decode(std::string filename) {

    std::cout << "\n=======================================================================\n";
    std::cout << " " << filename << "\n";
    std::cout << "=======================================================================\n\n";
    std::cout << std::flush;

    std::ifstream is(filename);
    std::string str;

    while(std::getline(is, str)) {
        
        //is.ignore();
        std::stringstream ss(str);
        std::string format;
        ss >> format;
        uint32_t base_format = get32bformat(format);

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
        auto dec     = decode_32bit_instruction(0x00, base_format);
        auto dec_str = dec.str();
        
        std::cout << "Expected : " << strval << std::endl;
        std::cout << "Decoded  : " << dec_str << std::endl << std::flush;
        assert(strval == dec_str);
        std::cout << "SUCCESS\n\n";
    }

}
*/
static uint32_t get32bformat(std::string format) {

    const std::map<std::string, uint32_t> lut = {

        { "ADC_IMM",      FORMAT_ADC_IMM     },   
        { "ADD_IMM",      FORMAT_ADD_IMM     }, 
        { "AND_IMM",      FORMAT_AND_IMM     },  
        { "BIC_IMM",      FORMAT_BIC_IMM     },   
        { "CMN_IMM",      FORMAT_CMN_IMM     },
        { "CMP_IMM",      FORMAT_CMP_IMM     },  
        { "EOR_IMM",      FORMAT_EOR_IMM     }, 
        { "LDM ",         FORMAT_LDM         },
        { "LDR_IMM_T3",   FORMAT_LDR_IMM_T3  },
        { "LDR_IMM_T4",   FORMAT_LDR_IMM_T4  },
        { "LDR_LIT",      FORMAT_LDR_LIT     },
        { "LDR_REG",      FORMAT_LDR_REG     },
        { "LDRD_IMM ",    FORMAT_LDRD_IMM    },
        { "LDRD_LIT",     FORMAT_LDRD_LIT    },
        { "LDREX ",       FORMAT_LDREX       },
        { "LDREXB",       FORMAT_LDREXB      },
        { "LDREXH",       FORMAT_LDREXH      },
        { "LDRT",         FORMAT_LDRT        },
        { "MOV_IMM",      FORMAT_MOV_IMM     },
        { "MVN_IMM",      FORMAT_MVN_IMM     },
        { "ORN_IMM",      FORMAT_ORN_IMM     },
        { "ORR_IMM",      FORMAT_ORR_IMM     },
        { "POP",          FORMAT_POP         },
        { "PUSH",         FORMAT_PUSH        },
        { "RSB_IMM ",     FORMAT_RSB_IMM     },
        { "SBC_IMM",      FORMAT_SBC_IMM     },
        { "STM",          FORMAT_STM         },
        { "STMDB",        FORMAT_STMDB       },
        { "STR_IMM_T3",   FORMAT_STR_IMM_T3  },
        { "STR_IMM_T4",   FORMAT_STR_IMM_T4  },
        { "STR_REG",      FORMAT_STR_REG     },
        { "STRB_IMM_T2",  FORMAT_STRB_IMM_T2 },
        { "STRB_IMM_T3",  FORMAT_STRB_IMM_T3 },
        { "STRB_REG",     FORMAT_STRB_REG    },
        { "STRD_IMM",     FORMAT_STRD_IMM    },
        { "STREX",        FORMAT_STREX       },
        { "STREXB",       FORMAT_STREXB      },
        { "STREXH",       FORMAT_STREXH      },
        { "STRH_IMM_T2",  FORMAT_STRH_IMM_T2 },
        { "STRH_IMM_T3",  FORMAT_STRH_IMM_T3 },
        { "STRH_REG",     FORMAT_STRH_REG    },
        { "SUB_IMM",      FORMAT_SUB_IMM     },
        { "TBB",          FORMAT_TBB         },
        { "TBH",          FORMAT_TBH         },
        { "TEQ_IMM",      FORMAT_TEQ_IMM     },
        { "TST_IMM",      FORMAT_TST_IMM     },


    };

    auto iter = lut.find(format);
    if(iter == lut.end())
        throw std::runtime_error("get32bformat : string representation is not available");
    else
        return iter->second;
}

#undef SHL
