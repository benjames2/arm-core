#pragma once

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

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <assert.h>
#include <inc/decode.h>

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
        auto dec     = decode_instruction(0x00, base_format);
        auto dec_str = dec.str();
        
        std::cout << "Expected : " << strval << std::endl;
        std::cout << "Decoded  : " << dec_str << std::endl << std::flush;
        assert(strval == dec_str);
        std::cout << "SUCCESS\n\n";
    }

}

#undef SHL
